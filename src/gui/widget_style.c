/* ***************************************************************************
 * widget_style.c -- widget style library module for LCUI.
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * widget_style.c -- LCUI 的部件样式库模块。
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

#define MAX_NAME_LEN		256
#define MAX_SELECTOR_LEN	1024
#define MAX_NODE_DEPTH		32
#define MAX_SELECTOR_DEPTH	32

#define FindStyleSheet(S, L) FindStyleSheetFromGroup(0, NULL, S, L)

enum SelectorRank {
	GENERAL_RANK = 0,
	TYPE_RANK = 1,
	CLASS_RANK = 10,
	PCLASS_RANK = 10,
	ID_RANK = 100
};

enum SelectorFinderLevel {
	LEVEL_NONE,
	LEVEL_TYPE,
	LEVEL_ID,
	LEVEL_CLASS,
	LEVEL_CLASS_2,
	LEVEL_STATUS,
	LEVEL_STATUS_2,
	LEVEL_TOTAL_NUM
};

/* 样式表查找器的上下文数据结构 */
typedef struct NamesFinderRec_ {
	int level;			/**< 当前选择器层级 */
	int class_i;			/**< 当前处理到第几个类名 */
	int status_i;			/**< 当前处理到第几个状态名（伪类名） */
	int name_i;			/**< 选择器名称从第几个字符开始 */
	char name[MAX_NAME_LEN];	/**< 选择器名称缓存 */
	LCUI_SelectorNode node;		/**< 针对的选择器结点 */
} NamesFinderRec, *NamesFinder;

/** 样式链接记录组 */
typedef struct StyleLinkGroupRec_ {
	Dict *links;			/**< 样式链接表 */
	char *name;			/**< 选择器名称 */
	LCUI_SelectorNodeRec snode;	/**< 选择器结点 */
} StyleLinkGroupRec, *StyleLinkGroup;

/** 样式结点记录 */
typedef struct StyleNodeRec_ {
	int rank;		/**< 权值，决定优先级 */
	int batch_num;		/**< 批次号 */
	char *space;		/**< 所属的空间 */
	char *selector;		/**< 选择器 */
	LCUI_StyleSheet sheet;	/**< 样式表 */
} StyleNodeRec, *StyleNode;

/** 样式链接记录 */
typedef struct StyleLinkRec_ {
	char *selector;		/**< 选择器 */
	StyleLinkGroup group;	/**< 所属组 */
	LinkedList styles;	/**< 作用于当前选择器的样式 */
	Dict *parents;		/**< 父级节点 */
} StyleLinkRec, *StyleLink;

static struct {
	LCUI_BOOL is_inited;
	LCUI_Mutex mutex;		/**< 互斥锁 */
	LinkedList groups;		/**< 样式组列表 */
	LCUI_RBTree cache;		/**< 样式表缓存，以选择器的 hash 值索引 */
} style_library;

const char *global_css = ToString(

* {
	width: auto;
	height:	auto;
	background-color: transparent;
	display: block;
	position: static;
	padding: 0;
	margin: 0;
}

);

static int SortedStrList_Add( char ***strlist, const char *str )
{
	int i, n, len, pos;
	char **newlist, *newstr;

	if( *strlist ) {
		for( i = 0; (*strlist)[i]; ++i );
		len = i + 2;
	} else {
		len = 2;
	}
	newlist = realloc( *strlist, sizeof( char* ) * len );
	if( !newlist ) {
		return -ENOMEM;
	}
	newlist[len - 2] = NULL;
	for( i = 0, pos = -1; newlist[i]; ++i ) {
		n = strcmp( newlist[i], str );
		if( n < 0 ) {
			continue;
		} else if( n == 0 ) {
			return 1;
		} else {
			pos = i;
		}
	}
	n = strlen( str ) + 1;
	newstr = malloc( sizeof(char) * n );
	if( !newstr ) {
		return -ENOMEM;
	}
	strncpy( newstr, str, n );
	if( pos >= 0 ) {
		for( i = len - 2; i > pos; --i ) {
			newlist[i] = newlist[i - 1];
		}
		newlist[pos] = newstr;
	} else {
		pos = len - 2;
	}
	newlist[pos] = newstr;
	newlist[len - 1] = NULL;
	*strlist = newlist;
	return 0;
}

static void SortedStrList_Destroy( char **strlist )
{
	int i;
	for( i = 0; strlist[i]; ++i ) {
		free( strlist[i] );
		strlist[i] = NULL;
	}
	free( strlist );
}

/** 
 * 匹配选择器节点
 * 左边的选择器必须包含右边的选择器的所有属性。
 */
LCUI_BOOL SelectorNode_Match( LCUI_SelectorNode sn1, 
			      LCUI_SelectorNode sn2 )
{
	int i, j;
	if( sn2->id ) {
		if( !sn1->id || strcmp( sn1->id, sn2->id ) != 0 ) {
			return FALSE;
		}
	}
	if( sn2->type && strcmp(sn2->type, "*") != 0 ) {
		if( !sn1->type || strcmp( sn1->type, sn2->type ) != 0 ) {
			return FALSE;
		}
	}
	if( sn2->classes ) {
		if( !sn1->classes ) {
			return FALSE;
		}
		for( i = 0; sn2->classes[i]; ++i ) {
			for( j = 0; sn1->classes[j]; ++j ) {
				if( strcmp( sn2->classes[i],
					    sn1->classes[i] ) == 0 ) {
					j = -1;
					break;
				}
			}
			if( j != -1 ) {
				return FALSE;
			}
		}
	}
	if( sn2->status ) {
		if( !sn1->status ) {
			return FALSE;
		}
		for( i = 0; sn2->status[i]; ++i ) {
			for( j = 0; sn1->status[j]; ++j ) {
				if( strcmp( sn2->status[i],
					    sn1->status[i] ) == 0 ) {
					j = -1;
					break;
				}
			}
			if( j != -1 ) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

static void SelectorNode_Copy( LCUI_SelectorNode dst, LCUI_SelectorNode src )
{
	int i;
	dst->id = src->id ? strdup( src->id ) : NULL;
	dst->type = src->type ? strdup( src->type ) : NULL;
	dst->fullname = src->fullname ? strdup( src->fullname ) : NULL;
	if( src->classes ) {
		for( i = 0; src->classes[i]; ++i ) {
			SortedStrList_Add( &dst->classes, src->classes[i] );
		}
	}
	if( src->status ) {
		for( i = 0; src->status[i]; ++i ) {
			SortedStrList_Add( &dst->status, src->status[i] );
		}
	}
}

static void SelectorNode_Delete( LCUI_SelectorNode node )
{
	if( node->type ) {
		free( node->type );
		node->type = NULL;
	}
	if( node->id ) {
		free( node->id );
		node->id = NULL;
	}
	if( node->classes ) {
		SortedStrList_Destroy( node->classes );
		node->classes = NULL;
	}
	if( node->status ) {
		SortedStrList_Destroy( node->status );
		node->status = NULL;
	}
	if( node->fullname ) {
		free( node->fullname );
		node->fullname = NULL;
	}
	free( node );
}

void Selector_Delete( LCUI_Selector s )
{
	int i;
	for( i = 0; i < MAX_SELECTOR_DEPTH; ++i ) {
		if( !s->nodes[i] ) {
			break;
		}
		SelectorNode_Delete( s->nodes[i] );
		s->nodes[i] = NULL;
	}
	s->rank = 0;
	s->length = 0;
	s->batch_num = 0;
	free( s->nodes );
	free( s );
}

LCUI_StyleSheet StyleSheet( void )
{
	LCUI_StyleSheet ss;
	ss = NEW( LCUI_StyleSheetRec, 1 );
	if( !ss ) {
		return ss;
	}
	ss->length = LCUICSS_GetStyleTotal();
	ss->sheet = NEW( LCUI_StyleRec, ss->length + 1 );
	return ss;
}

void StyleSheet_Clear( LCUI_StyleSheet ss )
{
	int i;
	LCUI_Style s;
	for( i=0; i<ss->length; ++i ) {
		s = &ss->sheet[i];
		switch( s->type ) {
		case SVT_STRING:
		case SVT_WSTRING:
			if( s->is_valid && s->string ) {
				free( s->string );
			}
			s->string = NULL;
		default: break;
		}
		s->is_valid = FALSE;
		s->is_changed = TRUE;
	}
}

void StyleSheet_Delete( LCUI_StyleSheet ss )
{
	StyleSheet_Clear( ss );
	free( ss->sheet );
	free( ss );
}

int StyleSheet_Merge( LCUI_StyleSheet dest, LCUI_StyleSheet src )
{
	LCUI_Style s;
	int i, count, size;
	if( src->length > dest->length ) {
		size = sizeof(LCUI_StyleRec)*src->length;
		s = realloc( dest->sheet, size );
		if( !s ) {
			return -1;
		}
		for( i=dest->length; i<src->length; ++i ) {
			s[i].is_valid = FALSE;
		}
		dest->sheet = s;
		dest->length = src->length;
	}
	for( count=0,i=0; i<src->length; ++i ) {
		s = &dest->sheet[i];
		if( !src->sheet[i].is_valid || s->is_valid ) {
			continue;
		}
		++count;
		switch( src->sheet[i].type ) {
		case SVT_STRING:
			s->string = strdup( src->sheet[i].string );
			break;
		case SVT_WSTRING:
			size = wcslen( src->sheet[i].wstring ) + 1;
			s->wstring = malloc( size * sizeof(wchar_t) );
			wcscpy( s->wstring, src->sheet[i].wstring );
			break;
		default:
			*s = src->sheet[i];
			break;
		}
		s->is_valid = TRUE;
		s->is_changed = TRUE;
		s->type = src->sheet[i].type;
	}
	return 0;
}

int StyleSheet_Replace( LCUI_StyleSheet dest, LCUI_StyleSheet src )
{
	LCUI_Style s;
	int i, count, size;
	if( src->length > dest->length ) {
		size = sizeof(LCUI_StyleRec)*src->length;
		s = realloc( dest->sheet, size );
		if( !s ) {
			return -1;
		}
		for( i=dest->length; i<src->length; ++i ) {
			s[i].is_valid = FALSE;
		}
		dest->sheet = s;
		dest->length = src->length;
	}
	for( count=0,i=0; i<src->length; ++i ) {
		if( !src->sheet[i].is_valid ) {
			continue;
		}
		s = &dest->sheet[i];
		switch( src->sheet[i].type ) {
		case SVT_STRING:
			if( s->is_valid && s->string ) {
				free( s->string );
			}
			s->string = strdup( src->sheet[i].string );
			break;
		case SVT_WSTRING:
			if( s->is_valid && s->string ) {
				free( s->wstring );
			}
			size = wcslen( src->sheet[i].wstring ) + 1;
			s->wstring = malloc( size * sizeof(wchar_t) );
			wcscpy( s->wstring, src->sheet[i].wstring );
			break;
		default:
			*s = src->sheet[i];
			break;
		}
		s->is_valid = TRUE;
		s->is_changed = TRUE;
		s->type = src->sheet[i].type;
		++count;
	}
	return count;
}

/** 初始化样式表查找器 */
static void NamesFinder_Init( NamesFinder sfinder, LCUI_SelectorNode snode )
{
	sfinder->level = 0;
	sfinder->class_i = 0;
	sfinder->name_i = 0;
	sfinder->status_i = 0;
	sfinder->name[0] = 0;
	sfinder->node = snode;
}

/** 销毁样式表查找器 */
static void NamesFinder_Destroy( NamesFinder sfinder )
{
	sfinder->name_i = 0;
	sfinder->name[0] = 0;
	sfinder->class_i = 0;
	sfinder->status_i = 0;
	sfinder->node = NULL;
	sfinder->level = LEVEL_NONE;
}

/* 查找匹配的样式表 */
static int NamesFinder_Find( NamesFinder sfinder, LinkedList *list )
{
	int i, len, old_len, old_level, count = 0;
	char *fullname = sfinder->name + sfinder->name_i;
	old_len = len = strlen( fullname );
	old_level = sfinder->level;
	switch( sfinder->level ) {
	case LEVEL_TYPE: 
		/* 按类型选择器搜索样式表 */
		if( !sfinder->node->type ) {
			return 0;
		}
		strcpy( fullname, sfinder->node->type );
		LinkedList_Append( list, strdup( fullname ) );
		break;
	case LEVEL_ID: 
		/* 按ID选择器搜索样式表 */
		if( !sfinder->node->id ) {
			return 0;
		}
		fullname[len++] = '#';
		fullname[len] = 0;
		strcpy( fullname + len, sfinder->node->id );
		LinkedList_Append( list, strdup( fullname ) );
		break;
	case LEVEL_CLASS:
		if( !sfinder->node->classes ) {
			return 0;
		}
		/* 按类选择器搜索样式表
		* 假设当前选择器全名为：textview#main-btn-text，且有 .a .b .c 
		* 这三个类，那么下面的处理将会拆分成以下三个选择器来匹配样式表：
		* textview#test-text.a
		* textview#test-text.b
		* textview#test-text.a
		*/
		fullname[len++] = '.';
		for( i = 0; sfinder->node->classes[i]; ++i ) {
			sfinder->level += 1;
			strcpy( fullname + len, sfinder->node->classes[i] );
			LinkedList_Append( list, strdup( fullname ) );
			/* 将当前选择器名与其它层级的选择器名组合 */
			while( sfinder->level < LEVEL_TOTAL_NUM ) {
				count += NamesFinder_Find( sfinder, list );
				sfinder->level += 1;
			}
			sfinder->level = LEVEL_CLASS;
			sfinder->class_i += 1;
		}
		sfinder->level = LEVEL_CLASS;
		fullname[old_len] = 0;
		sfinder->class_i = 0;
		return count;
	case LEVEL_CLASS_2:
		if( !sfinder->node->classes ) {
			return 0;
		}
		/* 按类选择器搜索交集样式表，结果类似于这样： 
		* textview#test-text.a.b
		* textview#test-text.a.c
		* textview#test-text.b.c
		* textview#test-text.a.b.c
		*/
		fullname[len++] = '.';
		for( i = 0; sfinder->node->classes[i]; ++i ) {
			if( i <= sfinder->class_i ) {
				continue;
			}
			strcpy( fullname + len, sfinder->node->classes[i] );
			LinkedList_Append( list, strdup( fullname ) );
			sfinder->class_i += 1;
			count += NamesFinder_Find( sfinder, list );
			sfinder->class_i -= 1;
			sfinder->level = LEVEL_STATUS;
			/**
			* 递归拼接伪类名，例如：
			* textview#main-btn-text:active
			*/
			count += NamesFinder_Find( sfinder, list );
			sfinder->level = LEVEL_CLASS_2;
		}
		fullname[old_len] = 0;
		sfinder->level = LEVEL_CLASS_2;
		return count;
	case LEVEL_STATUS:
		if( !sfinder->node->status ) {
			return 0;
		}
		fullname[len++] = ':';
		sfinder->level = LEVEL_STATUS_2;
		/**
		* 按伪类选择器搜索样式表
		* 假设当前选择器全名为：textview#main-btn-text:hover:focus:active
		* 那么下面的循环会将它拆分为以下几个选择器来匹配样式：
		* textview#main-btn-text:active
		* textview#main-btn-text:active:focus
		* textview#main-btn-text:active:focus:hover
		* textview#main-btn-text:active:hover
		* textview#main-btn-text:focus
		* textview#main-btn-text:focus:hover
		* textview#main-btn-text:hover
		*/
		for( i = 0; sfinder->node->status[i]; ++i ) {
			strcpy( fullname + len, sfinder->node->status[i] );
			LinkedList_Append( list, strdup( fullname ) );
			/**
			 * 递归调用，以一层层拼接出像下面这样的选择器：
			 * textview#main-btn-text:active:focus:hover
			 */
			count += NamesFinder_Find( sfinder, list );
			sfinder->status_i += 1;
		}
		sfinder->level = LEVEL_STATUS;
		fullname[old_len] = 0;
		sfinder->status_i = 0;
		return count;
	case LEVEL_STATUS_2:
		if( !sfinder->node->status ) {
			return 0;
		}
		/** 按伪类选择器搜索交集样式表 */
		for( i = 0; sfinder->node->status[i]; ++i ) {
			if( i <= sfinder->status_i ) {
				continue;
			}
			fullname[len] = ':';
			strcpy( fullname + len + 1, sfinder->node->status[i] );
			LinkedList_Append( list, strdup( fullname ) );
			sfinder->status_i += 1;
			count += NamesFinder_Find( sfinder, list );
			sfinder->status_i -= 1;
		}
		fullname[old_len] = 0;
		return count;
	default:break;
	}
	for( i = sfinder->level + 1; i < LEVEL_TOTAL_NUM; ++i ) {
		if( i == LEVEL_STATUS_2 || i == LEVEL_CLASS_2 ) {
			continue;
		}
		sfinder->level = i;
		count += NamesFinder_Find( sfinder, list );
	}
	fullname[old_len] = 0;
	sfinder->level = old_level;
	return count;
}

static int SelectorNode_Save( LCUI_SelectorNode node,
			      const char *name, int len, char type )
{
	char *str;
	if( len < 1 ) {
		return 0;
	}
	switch( type ) {
	case 0:
		if( node->type ) {
			break;
		}
		len += 1;
		str = malloc( sizeof( char ) * len );
		strncpy( str, name, len );
		node->type = str;
		return TYPE_RANK;
	case ':':
		if( SortedStrList_Add( &node->status, name ) == 0 ) {
			return PCLASS_RANK;
		}
		break;
	case '.':
		if( SortedStrList_Add( &node->classes, name ) == 0 ) {
			return CLASS_RANK;
		}
		break;
	case '#':
		if( node->id ) {
			break;
		}
		len += 1;
		str = malloc( sizeof( char ) * len );
		strncpy( str, name, len );
		node->id = str;
		return ID_RANK;
	default: break;
	}
	return 0;
}

static int SelectorNode_GetNames( LCUI_SelectorNode sn, LinkedList *names )
{
	int count;
	NamesFinderRec sfinder;
	NamesFinder_Init( &sfinder, sn );
	count = NamesFinder_Find( &sfinder, names );
	NamesFinder_Destroy( &sfinder );
	return count;
}

static int SelectorNode_Update( LCUI_SelectorNode node )
{
	int i, len = 0;
	char *fullname;

	node->rank = 0;
	if( node->id ) {
		len += strlen( node->id ) + 1;
		node->rank += ID_RANK;
	}
	if( node->type ) {
		len += strlen( node->type ) + 1;
		node->rank += TYPE_RANK;
	}
	if( node->classes ) {
		for( i = 0; node->classes[i]; ++i ) {
			len += strlen( node->classes[i] ) + 1;
			node->rank += CLASS_RANK;
		}
	}
	if( node->status ) {
		for( i = 0; node->status[i]; ++i ) {
			len += strlen( node->status[i] ) + 1;
			node->rank += PCLASS_RANK;
		}
	}
	if( len > 0 ) {
		fullname = malloc( sizeof( char ) * (len + 1) );
		if( !fullname ) {
			return -ENOMEM;
		}
		fullname[0] = 0;
		if( node->type ) {
			strcat( fullname, node->type );
		}
		if( node->id ) {
			strcat( fullname, "#" );
			strcat( fullname, node->id );
		}
		if( node->classes ) {
			for( i = 0; node->classes[i]; ++i ) {
				strcat( fullname, "." );
				strcat( fullname, node->classes[i] );
			}
			len += 1;
		}
		if( node->status ) {
			for( i = 0; node->status[i]; ++i ) {
				strcat( fullname, ":" );
				strcat( fullname, node->status[i] );
			}
			len += 1;
		}
	} else {
		fullname = NULL;
	}
	if( node->fullname ) {
		free( node->fullname );
	}
	node->fullname = fullname;
	return len;
}

void Selector_Update( LCUI_Selector s )
{
	int i;
	const unsigned char *p;
	unsigned int hash = 5381;
	for( i = 0; i < s->length; ++i ) {
		p = (unsigned char*)s->nodes[i]->fullname;
		while( *p ) {
			hash = ((hash << 5) + hash) + (*p++);
		}
	}
	s->hash = hash;
}

LCUI_Selector Selector( const char *selector )
{
	const char *p;
	int ni, si, rank;
	static int batch_num = 0;
	char type = 0, name[MAX_NAME_LEN];
	LCUI_BOOL is_saving = FALSE;
	LCUI_SelectorNode node = NULL;
	LCUI_Selector s = NEW( LCUI_SelectorRec, 1 );

	s->batch_num = ++batch_num;
	s->nodes = NEW( LCUI_SelectorNode, MAX_SELECTOR_DEPTH );
	if( !selector ) {
		s->length = 0;
		s->nodes[0] = NULL;
		return s;
	}
	for( ni = 0, si = 0, p = selector; *p; ++p ) {
		if( !node && is_saving ) {
			node = NEW( LCUI_SelectorNodeRec, 1 );
			if( si >= MAX_SELECTOR_DEPTH ) {
				_DEBUG_MSG( "%s: selector node list is too long.\n",
					    selector );
				return NULL;
			}
			s->nodes[si] = node;
		}
		switch( *p ) {
		case ':':
		case '.':
		case '#':
			if( !is_saving ) {
				is_saving = TRUE;
				type = *p;
				continue;
			}
			/* 保存上个结点 */
			rank = SelectorNode_Save( node, name, ni, type );
			if( rank > 0 ) {
				s->rank += rank;
			} else {
				_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
						selector, p - selector - ni );
				SelectorNode_Delete( node );
				node = NULL;
			}
			ni = 0;
			is_saving = TRUE;
			type = *p;
			continue;
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			if( !is_saving ) {
				ni = 0;
				node = NULL;
				continue;
			}
			is_saving = FALSE;
			rank = SelectorNode_Save( node, name, ni, type );
			if( rank > 0 ) {
				SelectorNode_Update( node );
				s->rank += rank;
				node = NULL;
				ni = 0;
				si++;
				continue;
			}
			_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
				    selector, p - selector - ni );
			SelectorNode_Delete( node );
			node = NULL;
			ni = 0;
			continue;
		default: break;
		}
		if( *p == '-' || *p == '_' || *p == '*'
		 || (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')
		 || (*p >= '0' && *p <= '9') ) {
			if( !is_saving ) {
				type = 0;
				is_saving = TRUE;
			}
			name[ni++] = *p;
			name[ni] = 0;
			continue;
		}
		_DEBUG_MSG( "%s: unknown char 0x%02x at %ld.\n",
			    selector, *p, p - selector );
		return NULL;
	}
	if( is_saving ) {
		rank = SelectorNode_Save( s->nodes[si], name, ni, type );
		if( rank > 0 ) {
			SelectorNode_Update( s->nodes[si] );
			s->rank += rank;
			si++;
		} else {
			SelectorNode_Delete( s->nodes[si] );
		}
	}
	s->nodes[si] = NULL;
	s->length = si;
	Selector_Update( s );
	return s;
}

static void DeleteStyleNode( StyleNode node )
{
	if( node->space ) {
		free( node->space );
		node->space = NULL;
	}
	if( node->selector ) {
		free( node->selector );
		node->selector = NULL;
	}
	StyleSheet_Delete( node->sheet );
	node->sheet = NULL;
}

static StyleLink CreateStyleLink( void )
{
	StyleLink link = NEW( StyleLinkRec, 1 );
	link->group = NULL;
	LinkedList_Init( &link->styles );
	link->parents = Dict_Create( &DictType_StringCopyKey, NULL );
	return link;
}

static void DeleteStyleLink( StyleLink link )
{
	link->group = NULL;
	Dict_Release( link->parents );
	link->parents = NULL;
	LinkedList_Clear( &link->styles, (FuncPtr)DeleteStyleNode );
}

static void OnDeleteStyleLink( void *privdata, void *data )
{
	DeleteStyleLink( data );
}

static StyleLinkGroup CreateStyleLinkGroup( LCUI_SelectorNode snode )
{
	DictType *dtype = NEW( DictType, 1 );
	StyleLinkGroup group = NEW( StyleLinkGroupRec, 1 );
	SelectorNode_Copy( &group->snode, snode );
	group->name = group->snode.fullname;
	*dtype = DictType_StringCopyKey;
	dtype->valDestructor = OnDeleteStyleLink;
	group->links = Dict_Create( dtype, dtype );
	return group;
}

static void DeleteStyleLinkGroup( StyleLinkGroup group )
{
	free( group->name );
	Dict_Release( group->links );
	free( group->links->privdata );
	group->name = NULL;
	group->links = NULL;
}

static void OnDeleteStyleLinkGroup( void *privdata, void *data )
{
	DeleteStyleLinkGroup( data );
}

static Dict *CreateStyleGroup( void )
{
	Dict *dict;
	DictType *dtype = NEW( DictType, 1 );
	*dtype = DictType_StringCopyKey;
	dtype->valDestructor = OnDeleteStyleLinkGroup;
	dict = Dict_Create( dtype, dtype );
	return dict;
}

static void DeleteStyleGroup( Dict *dict )
{
	Dict_Release( dict );
	free( dict->privdata );
	dict->privdata = NULL;
}

/** 根据选择器，选中匹配的样式表 */
static LCUI_StyleSheet SelectStyleSheet( LCUI_Selector selector, 
					 const char *space )
{
	int i, right;
	StyleLink link;
	StyleNode snode;
	StyleLinkGroup slg;
	LinkedListNode *node;
	LCUI_SelectorNode sn;
	Dict *group, *parents;
	char buf[MAX_SELECTOR_LEN];
	char fullname[MAX_SELECTOR_LEN];

	link = NULL;
	parents = NULL;
	for( i = 0, right = selector->length - 1; right >= 0; --right, ++i ) {
		group = LinkedList_Get( &style_library.groups, i );
		if( !group ) {
			group = CreateStyleGroup();
			LinkedList_Append( &style_library.groups, group );
		}
		sn = selector->nodes[right];
		slg = Dict_FetchValue( group, sn->fullname );
		if( !slg ) {
			slg = CreateStyleLinkGroup( sn );
			Dict_Add( group, sn->fullname, slg );
		}
		if( i == 0 ) {
			strcpy( fullname, "*" );
		} else {
			strcpy( fullname, buf );
		}
		link = Dict_FetchValue( slg->links, fullname );
		if( !link ) {
			link = CreateStyleLink();
			link->group = slg;
			link->selector = strdup( fullname );
			Dict_Add( slg->links, fullname, link );
		}
		if( i == 0 ) {
			strcpy( buf, sn->fullname );
		} else {
			strcpy( fullname, buf );
			sprintf( buf, "%s %s", sn->fullname, fullname );
		}
		/* 如果有上一级的父链接记录，则将当前链接添加进去 */
		if( parents ) {
			if( !Dict_FetchValue( parents, sn->fullname ) ) {
				Dict_Add( parents, sn->fullname, link );
			}
		}
		parents = link->parents;
	}
	if( !link ) {
		return NULL;
	}
	for( LinkedList_Each( node, &link->styles ) ) {
		snode = node->data;
		if( snode->space && space ) {
			if( strcmp( snode->space, space ) == 0 ) {
				return snode->sheet;
			}
		} else {
			if( snode->space == space ) {
				return snode->sheet;
			}
		}
	}
	snode = NEW( StyleNodeRec, 1 );
	if( space ) {
		snode->space = NEW( char, strlen(space) + 1 );
		strcpy( snode->space, space );
	} else {
		snode->space = NULL;
	}
	snode->sheet = StyleSheet();
	snode->rank = selector->rank;
	snode->selector = strdup( fullname );
	snode->batch_num = selector->batch_num;
	LinkedList_Append( &link->styles, snode );
	return snode->sheet;
}

int LCUI_PutStyle( LCUI_Selector selector, 
		   LCUI_StyleSheet in_ss, const char *space )
{
	LCUI_StyleSheet ss;
	LCUIMutex_Lock( &style_library.mutex );
	RBTree_Destroy( &style_library.cache );
	ss = SelectStyleSheet( selector, space );
	if( ss ) {
		StyleSheet_Replace( ss, in_ss );
	}
	LCUIMutex_Unlock( &style_library.mutex );
	return 0;
}

static int StyleLink_GetStyleSheets( StyleLink link, LinkedList *outlist )
{
	StyleNode snode, out_snode;
	LinkedListNode *node, *out_node;

	if( !outlist ) {
		return link->styles.length;
	}
	for( LinkedList_Each( node, &link->styles ) ) {
		int pos = -1, i = 0;
		snode = node->data;
		for( LinkedList_Each( out_node, outlist ) ) {
			out_snode = out_node->data;
			if( snode->rank > out_snode->rank ) {
				pos = i;
				break;
			}
			if( snode->rank != out_snode->rank ) {
				i += 1;
				continue;
			}
			if( snode->batch_num > out_snode->batch_num ) {
				pos = i;
				break;
			}
			i += 1;
		}
		if( pos >= 0 ) {
			LinkedList_Insert( outlist, pos, snode );
		} else {
			LinkedList_Append( outlist, snode );
		}
	}
	return link->styles.length;
}

static int FindStyleSheetFromLink( StyleLink link, LCUI_Selector s,
				   int i, LinkedList *list )
{
	int count = 0;
	StyleLink parent;
	LinkedList names;
	LinkedListNode *node;
	LCUI_SelectorNode sn;

	LinkedList_Init( &names );
	count += StyleLink_GetStyleSheets( link, list );
	while( --i >= 0 ) {
		sn = s->nodes[i];
		SelectorNode_GetNames( sn, &names );
		for( LinkedList_Each( node, &names ) ) {
			parent = Dict_FetchValue( link->parents, node->data );
			if( !parent ) {
				continue;
			}
			count += FindStyleSheetFromLink( parent, s, i, list );
		}
		LinkedList_Clear( &names, free );
	}
	return count;
}

/**
 * 从指定组中查找样式表
 * @param[in] group 组号
 * @param[in] name 选择器结点名称，若为 NULL，则根据选择器结点生成名称
 * @param[in] s 选择器
 * @param[out] list 找到的样式表列表
 */
static int FindStyleSheetFromGroup( int group, const char *name, 
				    LCUI_Selector s, LinkedList *list )
{
	int i, count;
	Dict *groups;
	StyleLinkGroup slg;
	LinkedListNode *node;
	LinkedList names;

	groups = LinkedList_Get( &style_library.groups, group );
	if( !groups || s->length < 1 ) {
		return 0;
	}
	count = 0;
	i = s->length - 1;
	LinkedList_Init( &names );
	if( name ) {
		LinkedList_Append( &names, strdup( name ) );
	} else {
		SelectorNode_GetNames( s->nodes[i], &names );
		LinkedList_Append( &names, strdup( "*" ) );
	}
	for( LinkedList_Each( node, &names ) ) {
		DictEntry *entry;
		DictIterator *iter;
		char *name = node->data;
		slg = Dict_FetchValue( groups, name );
		if( !slg ) {
			continue;
		}
		iter = Dict_GetIterator( slg->links );
		while( (entry = Dict_Next( iter )) ) {
			StyleLink link = DictEntry_GetVal( entry );
			count += FindStyleSheetFromLink( link, s, i, list );
		}
	}
	LinkedList_Clear( &names, free );
	return count;
}

void LCUI_PrintStyleSheet( LCUI_StyleSheet ss )
{
	int key;
	const char *name;
	for( key = 0; key<ss->length; ++key ) {
		LCUI_Style s = &ss->sheet[key];
		if( !s->is_valid ) {
			continue;
		}
		name = GetStyleName(key);
		if( name ) {
			printf( "\t%s", name );
		} else {
			printf( "\t<unknown style %d>", key);
		}
		printf( "%s: ", key > STYLE_KEY_TOTAL ? " (+)" : "" );
		switch( s->type ) {
		case SVT_AUTO:
			printf( "auto");
			break;
		case SVT_BOOL: {
			printf( "%s", s->val_bool ? "true" : "false" );
			break;
		}
		case SVT_COLOR: {
			LCUI_Color *clr = &s->val_color;
			if( clr->alpha < 255 ) {
				printf("rgba(%d,%d,%d,%0.2f)", clr->r,
					clr->g, clr->b, clr->a / 255.0);
			} else {
				printf("#%02x%02x%02x", clr->r, clr->g, clr->b);
			}
			break;
		}
		case SVT_PX:
			printf("%dpx", s->val_px);
			break;
		case SVT_STRING:
			printf("%s", s->val_string);
			break;
		case SVT_SCALE:
			printf("%.2lf%%", s->val_scale*100);
			break;
		case SVT_STYLE:{
			printf("%s", GetStyleOptionName( s->val_style ));
			break;
		}
		default:
			printf("%d", s->value);
			break;
		}
		printf( ";\n");
	}
}

void LCUI_PrintSelector( LCUI_Selector selector )
{
	char path[MAX_SELECTOR_LEN];
	LCUI_SelectorNode *sn;

	path[0] = 0;
	for( sn = selector->nodes; *sn; ++sn ) {
		strcat( path, (*sn)->fullname );
		strcat( path, " " );
	}
	printf("path: %s (rank = %d, batch_num = %d)\n", path,
		selector->rank, selector->batch_num);
}

static void LCUI_PrintStyleLink( StyleLink link, const char *selector )
{
	DictEntry *entry;
	DictIterator *iter;
	LinkedListNode *node;
	char fullname[MAX_SELECTOR_LEN];
	if( selector ) {
		sprintf( fullname, "%s %s", link->group->name, selector );
	} else {
		strcpy( fullname, link->group->name );
	}
	for( LinkedList_Each( node, &link->styles ) ) {
		StyleNode snode = node->data;
		printf("\n[%s]\n", snode->space ? snode->space:"<none>");
		printf("%s {\n", fullname);
		LCUI_PrintStyleSheet( snode->sheet );
		printf("}\n");
	}
	iter = Dict_GetIterator( link->parents );
	while( (entry = Dict_Next( iter )) ) {
		StyleLink parent = DictEntry_GetVal( entry );
		LCUI_PrintStyleLink( parent, fullname );
	}
	Dict_ReleaseIterator( iter );
}

void LCUI_PrintStyleLibrary( void )
{
	Dict *group;
	StyleLink link;
	StyleLinkGroup slg;
	DictIterator *iter;
	DictEntry *entry;

	link = NULL;
	printf( "style library begin\n" );
	group = LinkedList_Get( &style_library.groups, 0 );
	iter = Dict_GetIterator( group );
	while( (entry = Dict_Next(iter)) ) {
		DictEntry *entry_slg;
		DictIterator *iter_slg;
		slg = DictEntry_GetVal( entry );
		iter_slg = Dict_GetIterator( slg->links );
		while( (entry_slg = Dict_Next(iter_slg)) ) {
			link = DictEntry_GetVal( entry_slg );
			LCUI_PrintStyleLink( link, NULL );
		}
		Dict_ReleaseIterator( iter_slg );
	}
	Dict_ReleaseIterator( iter );
	printf( "style library end\n" );
}

LCUI_Selector Widget_GetSelector( LCUI_Widget w )
{
	int ni = 0;
	LinkedList list;
	LCUI_Selector s;
	LCUI_Widget parent;
	LinkedListNode *node;
	s = Selector( NULL );
	LinkedList_Init( &list );
	for( parent = w; parent; parent = parent->parent ) {
		if( parent->id || parent->type || 
		    parent->classes || parent->status ) {
			LinkedList_Append( &list, parent );
		}
	}
	if( list.length >= MAX_SELECTOR_DEPTH ) {
		LinkedList_Clear( &list, NULL );
		Selector_Delete( s );
		return NULL;
	}
	for( LinkedList_EachReverse( node, &list ) ) {
		int i;
		LCUI_SelectorNode sn;
		parent = node->data;
		sn = NEW( LCUI_SelectorNodeRec, 1 );
		Widget_Lock( parent );
		if( parent->id ) {
			sn->id = strdup( parent->id );
		}
		if( parent->type ) {
			sn->type = strdup( parent->type );
		}
		if( parent->classes ) {
			for( i = 0; parent->classes[i]; ++i ) {
				SortedStrList_Add( &sn->classes,
						   parent->classes[i] );
			}
		}
		if( parent->status ) {
			for( i = 0; parent->status[i]; ++i ) {
				SortedStrList_Add( &sn->status,
						   parent->status[i] );
			}
		}
		Widget_Unlock( parent );
		SelectorNode_Update( sn );
		s->nodes[ni] = sn;
		s->rank += sn->rank;
		ni += 1;
	}
	LinkedList_Clear( &list, NULL );
	s->nodes[ni] = NULL;
	s->length = ni;
	Selector_Update( s );
	return s;
}

int Widget_HandleChildrenStyleChange( LCUI_Widget w, int type, const char *name )
{
	LCUI_Selector s;
	LinkedList snames;
	LinkedListNode *node;
	int i, n, count = 0;
	char ch, **names = NULL;

	/* 选择相应的前缀 */
	switch( type ) {
	case 0: ch = '.'; break;
	case 1: ch = ':'; break;
	default: return 0;
	}
	LinkedList_Init( &snames );
	s = Widget_GetSelector( w );
	n = strsplit( name, " ", &names );
	/* 为分割出来的字符串加上前缀 */
	for( i = 0; i < n; ++i ) {
		int len = strlen( names[i] ) + 2;
		char *str = malloc( len * sizeof( char ) );
		strncpy( str + 1, names[i], len - 1 );
		str[0] = ch;
		free( names[i] );
		names[i] = str;
	}
	SelectorNode_GetNames( s->nodes[s->length - 1], &snames );
	for( LinkedList_Each( node, &snames ) ) {
		char *sname = node->data;
		/* 过滤掉不包含 name 中存在的名称 */
		for( i = 0; i < n; ++i ) {
			char *p = strstr( sname, names[i] );
			if( p ) {
				p += strlen( names[i] );
				switch( *p ) {
				case 0:
				case ':':
				case '.': break;
				default: continue;
				}
				break;
			}
		}
		if( i < n ) {
			count += FindStyleSheetFromGroup( 1, sname, s, NULL );
		}
	}
	Selector_Delete( s );
	/* 若子部件的样式受到了影响，则标记子部件需要刷新 */
	if( count > 0 ) {
		Widget_AddTaskForChildren( w, WTT_REFRESH_STYLE );
	}
	freestrs( names );
	return count;
}

void Widget_GetInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss )
{
	LCUI_Selector s;
	LCUI_StyleSheet ss;
	LinkedList list;
	LinkedListNode *node;
	LinkedList_Init( &list );
	StyleSheet_Clear( out_ss );
	s = Widget_GetSelector( w );
	LCUIMutex_Lock( &style_library.mutex );
	ss = RBTree_GetData( &style_library.cache, (int)s->hash );
	if( ss ) {
		StyleSheet_Replace( out_ss, ss );
		LCUIMutex_Unlock( &style_library.mutex );
		Selector_Delete( s );
		return;
	}
	ss = StyleSheet();
	FindStyleSheet( s, &list );
	for( LinkedList_Each( node, &list ) ) {
		StyleNode sn = node->data;
		StyleSheet_Merge( ss, sn->sheet );
	}
	LinkedList_Clear( &list, NULL );
	RBTree_Insert( &style_library.cache, (int)s->hash, ss );
	StyleSheet_Replace( out_ss, ss );
	LCUIMutex_Unlock( &style_library.mutex );
	Selector_Delete( s );
}

void Widget_UpdateStyle( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	if( is_update_all ) {
		Widget_AddTask( w, WTT_REFRESH_STYLE );
	} else {
		Widget_AddTask( w, WTT_UPDATE_STYLE );
	}
}

void Widget_ExecUpdateStyle( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	int i, key;
	LCUI_Style s;
	LCUI_StyleSheet ss;
	LCUI_WidgetClass *wc;
	LCUI_BOOL need_update_expend_style = FALSE;
	typedef struct {
		int start, end, task;
		LCUI_BOOL is_valid;
	} TaskMap;
	TaskMap task_map[] = {
		{ key_display_start, key_display_end, WTT_VISIBLE, TRUE },
		{ key_opacity, key_opacity, WTT_OPACITY, TRUE },
		{ key_z_index, key_z_index, WTT_ZINDEX, TRUE },
		{ key_width, key_height, WTT_RESIZE, TRUE },
		{ key_padding_top, key_padding_left, WTT_RESIZE, TRUE },
		{ key_position_start, key_position_end, WTT_POSITION, TRUE },
		{ key_vertical_align, key_vertical_align, WTT_POSITION, TRUE },
		{ key_border_start, key_border_end, WTT_BORDER, TRUE },
		{ key_background_start, key_background_end, WTT_BACKGROUND, TRUE },
		{ key_box_shadow_start, key_box_shadow_end, WTT_SHADOW, TRUE },
		{ key_pointer_events, key_focusable, WTT_PROPS, TRUE },
		{ key_box_sizing, key_box_sizing, WTT_RESIZE, TRUE }
	};
	if( is_update_all ) {
		Widget_GetInheritStyle( w, w->inherited_style );
	}
	ss = w->style;
	w->style = StyleSheet();
	StyleSheet_Merge( w->style, w->custom_style );
	StyleSheet_Merge( w->style, w->inherited_style );
	/* 对比两张样式表，确定哪些需要更新 */
	for( key = 0; key < w->style->length; ++key ) {
		s = &w->style->sheet[key];
		if( ss->sheet[key].is_valid == s->is_valid &&
		    ss->sheet[key].type == s->type &&
		    ss->sheet[key].value == s->value ) {
			continue;
		}
		if( key >= STYLE_KEY_TOTAL ) {
			need_update_expend_style = TRUE;
			break;
		}
		for( i = 0; i < sizeof(task_map) / sizeof(TaskMap); ++i ) {
			if( key >= task_map[i].start && key <= task_map[i].end ) {
				if( !task_map[i].is_valid ) {
					break;
				}
				task_map[i].is_valid = FALSE;
				Widget_AddTask( w, task_map[i].task );
			}
		}
	}
	if( need_update_expend_style ) {
		/* 扩展部分的样式交给该部件自己处理 */
		wc = LCUIWidget_GetClass( w->type );
		wc && wc->methods.update ? wc->methods.update( w ) : 0;
	}
	StyleSheet_Delete( ss );
}

void LCUIWidget_ExitStyle( void )
{
	LinkedList_Clear( &style_library.groups, (FuncPtr)DeleteStyleGroup );
	style_library.is_inited = FALSE;
	LCUICSS_Destroy();
}

void LCUIWidget_InitStyle( void )
{
	RBTree_Init( &style_library.cache );
	RBTree_OnDestroy( &style_library.cache, (FuncPtr)StyleSheet_Delete );
	LinkedList_Init( &style_library.groups );
	LCUIMutex_Init( &style_library.mutex );
	LCUICSS_Init();
	LCUICSS_LoadString( global_css, NULL );
	style_library.is_inited = TRUE;
}
