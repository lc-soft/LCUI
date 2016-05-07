/* ***************************************************************************
 * widget_style.c -- widget style library module for LCUI.
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

#define MAX_NAME_LEN		256
#define MAX_NODE_DEPTH		32
#define MAX_SELECTOR_DEPTH	32

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

typedef struct StyleListNodeRec_ {
	LCUI_StyleSheet style;		/**< 样式表 */
	LCUI_Selector selector;		/**< 作用范围 */
} StyleListNodeRec, *StyleListNode;

typedef struct StyleTreeNodeRec_ {
	char name[MAX_NAME_LEN];	/**< 作用对象的名称 */
	LinkedList styles;		/**< 其它样式表 */
} StyleTreeNodeRec, *StyleTreeNode;

/* 样式表查找器的上下文数据结构 */
typedef struct StyleSheetFinderRec_ {
	int level;			/**< 当前选择器层级 */
	int class_i;			/**< 当前处理到第几个类名 */
	int status_i;			/**< 当前处理到第几个状态名（伪类名） */
	int name_i;			/**< 选择器名称从第几个字符开始 */
	char name[MAX_NAME_LEN];	/**< 选择器名称缓存 */
	LinkedList classes;		/**< 已排序的类名列表 */
	LinkedList status;		/**< 已排序的状态列表 */
	LCUI_Widget widget;		/**< 针对的部件 */
} StyleSheetFinderRec, *StyleSheetFinder;

static struct {
	LCUI_BOOL is_inited;
	LCUI_RBTree tree;		/**< 样式树 */
} style_library;

const char *global_css = ToString(

* {
	width: auto;
	height: auto;
	background-color: transparent;
	display: block;
	position: static;
	padding: 0;
	margin: 0;
}

);

/** 获取指针数组的长度 */
static size_t ptrslen( char *const *list )
{
	char *const *p = list;
	if( list == NULL ) {
		return 0;
	}
	while( *p ) {
		p++;
	}
	return p - list;
}

static int CompareName( void *data, const void *keydata )
{
	return strcmp( ((StyleTreeNode)data)->name, (const char*)keydata );
}

static void SelectorNode_Delete( LCUI_SelectorNode *node_ptr )
{
	LCUI_SelectorNode node = *node_ptr;
	if( node->type ) {
		free( node->type );
		node->type = NULL;
	}
	if( node->class_name ) {
		free( node->class_name );
		node->class_name = NULL;
	}
	if( node->id ) {
		free( node->id );
		node->id = NULL;
	}
	if( node->pseudo_class_name ) {
		free( node->pseudo_class_name );
		node->pseudo_class_name = NULL;
	}
	free( node );
	*node_ptr = NULL;
}

void Selector_Delete( LCUI_Selector *s_ptr )
{
	int i;
	LCUI_Selector s = *s_ptr;
	for( i = 0; i<MAX_SELECTOR_DEPTH; ++i ) {
		if( !s->list[i] ) {
			break;
		}
		SelectorNode_Delete( &s->list[i] );
	}
	s->rank = 0;
	s->length = 0;
	s->batch_num = 0;
	free( s->list );
	free( s );
	*s_ptr = NULL;
}

static void DestroyStyleListNode( StyleListNode node )
{
	StyleSheet_Delete( &node->style );
	Selector_Delete( &node->selector );
}

static void DestroyStyleTreeNode( void *data )
{
	StyleTreeNode node = data;
	LinkedList_Clear( &node->styles, (FuncPtr)DestroyStyleListNode );
}

void LCUIWidget_InitStyle( void )
{
	RBTree_Init( &style_library.tree );
	RBTree_OnJudge( &style_library.tree, CompareName );
	RBTree_OnDestroy( &style_library.tree, DestroyStyleTreeNode );
	style_library.is_inited = TRUE;
	LCUICSS_Init();
	LCUICSS_LoadString( global_css );
}

void LCUIWidget_ExitStyle( void )
{
	RBTree_Destroy( &style_library.tree );
	style_library.is_inited = FALSE;
	LCUICSS_Destroy();
}

LCUI_StyleSheet StyleSheet( void )
{
	LCUI_StyleSheet ss;
	ss = malloc( sizeof( LCUI_StyleSheetRec ) );
	if( !ss ) {
		return ss;
	}
	ss->length = LCUICSS_GetStyleTotal();
	ss->sheet = NEW(LCUI_StyleRec, ss->length + 1);
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

void StyleSheet_Delete( LCUI_StyleSheet *ss )
{
	StyleSheet_Clear( *ss );
	free( (*ss)->sheet );
	free( *ss );
	*ss = NULL;
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

static int SelectorNode_Save( LCUI_SelectorNode node,
			      const char *name, int len, char type )
{
	char *str;
	if( len < 1 ) {
		return 0;
	}
	str = malloc( (len + 1)*sizeof( char ) );
	strcpy( str, name );
	switch( type ) {
	case 0:
		if( node->type ) {
			break;
		}
		node->type = str;
		return TYPE_RANK;
	case ':':
		if( node->pseudo_class_name ) {
			break;
		}
		node->pseudo_class_name = str;
		return PCLASS_RANK;
	case '.':
		if( node->class_name ) {
			break;
		}
		node->class_name = str;
		return CLASS_RANK;
	case '#':
		if( node->id ) {
			break;
		}
		node->id = str;
		return ID_RANK;
	default: break;
	}
	free( str );
	return 0;
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
	s->list = NEW( LCUI_SelectorNode, MAX_SELECTOR_DEPTH );
	for( ni = 0, si = 0, p = selector; *p; ++p ) {
		if( !node && is_saving ) {
			node = NEW( LCUI_SelectorNodeRec, 1 );
			if( si >= MAX_SELECTOR_DEPTH ) {
				_DEBUG_MSG( "%s: selector node list is too long.\n",
					    selector );
				return NULL;
			}
			s->list[si] = node;
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
				SelectorNode_Delete( &node );
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
				si++;
				ni = 0;
				node = NULL;
				s->rank += rank;
				continue;
			}
			_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
				    selector, p - selector - ni );
			SelectorNode_Delete( &node );
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
		rank = SelectorNode_Save( s->list[si], name, ni, type );
		if( rank > 0 ) {
			s->rank += rank;
			si++;
		} else {
			SelectorNode_Delete( &s->list[si] );
		}
	}
	s->list[si] = NULL;
	s->length += si;
	return s;
}

LCUI_Selector Selector_Dup( const LCUI_Selector src )
{
	int i;
	LCUI_SelectorNode new_sn, *sn_ptr, sn;
	LCUI_Selector dst = NEW( LCUI_SelectorRec, 1 );
	dst->list = NEW( LCUI_SelectorNode, src->length + 1 );
	for( sn_ptr = src->list, i = 0; *sn_ptr; ++sn_ptr, ++i ) {
		sn = *sn_ptr;
		new_sn = NEW( LCUI_SelectorNodeRec, 1 );
		if( sn->type ) {
			new_sn->type = strdup( sn->type );
		}
		if( sn->id ) {
			new_sn->id = strdup( sn->id );
		}
		if( sn->class_name ) {
			new_sn->class_name = strdup( sn->class_name );
		}
		if( sn->pseudo_class_name ) {
			new_sn->pseudo_class_name = strdup( sn->pseudo_class_name );
		}
		dst->list[i] = new_sn;
	}
	dst->list[i] = NULL;
	dst->length = i;
	dst->rank = src->rank;
	dst->batch_num = src->batch_num;
	return dst;
}

static int mystrcmp( const char *str1, const char *str2 )
{
	if( str1 == str2 ) {
		return 0;
	}
	if( str1 == NULL || str2 == NULL ) {
		return -1;
	}
	return strcasecmp( str1, str2 );
}

LCUI_BOOL Selector_Compare( LCUI_Selector s1, LCUI_Selector s2 )
{
	LCUI_SelectorNode *sn1_ptr = s1->list, *sn2_ptr = s2->list;
	for( ; *sn1_ptr && *sn2_ptr; ++sn1_ptr,++sn2_ptr ) {
		if( mystrcmp((*sn1_ptr)->type, (*sn2_ptr)->type) ) {
			return FALSE;
		}
		if( mystrcmp((*sn1_ptr)->class_name,
			(*sn2_ptr)->class_name) ) {
			return FALSE;
		}
		if( mystrcmp((*sn1_ptr)->pseudo_class_name,
			(*sn2_ptr)->pseudo_class_name) ) {
			return FALSE;
		}
		if( mystrcmp((*sn1_ptr)->id, (*sn2_ptr)->id) ) {
			return FALSE;
		}
	}
	if( sn1_ptr == sn2_ptr ) {
		return TRUE;
	}
	return FALSE;
}

static LCUI_StyleSheet SelectStyleSheetByName( LCUI_Selector selector,
					       const char *name )
{
	StyleTreeNode stn;
	StyleListNode sln;
	LCUI_RBTreeNode *tn;
	LinkedListNode *node;
	int i = 0, pos = -1;
	tn = RBTree_CustomSearch( &style_library.tree, (const void*)name );
	if( !tn ) {
		stn = NEW( StyleTreeNodeRec, 1 );
		strncpy( stn->name, name, MAX_NAME_LEN );
		LinkedList_Init( &stn->styles );
		tn = RBTree_CustomInsert( &style_library.tree,
					  (const void*)name, stn );
	}
	stn = tn->data;
	LinkedList_ForEach( node, &stn->styles ) {
		sln = node->data;
		if( Selector_Compare(sln->selector, selector) ) {
			return sln->style;
		}
		if( pos != -1 ) {
			i += 1;
			continue;
		}
		if( selector->rank > sln->selector->rank ) {
			pos = i;
		} else if( selector->rank == sln->selector->rank ) {
			if( selector->batch_num > sln->selector->batch_num ) {
				pos = i;
				break;
			}
		}
		i += 1;
	}
	sln = NEW( StyleListNodeRec, 1 );
	sln->style = StyleSheet();
	sln->selector = Selector_Dup( selector );
	if( pos >= 0 ) {
		LinkedList_Insert( &stn->styles, pos, sln );
	} else {
		LinkedList_Append( &stn->styles, sln );
	}
	return sln->style;
}

static LCUI_StyleSheet SelectStyleSheet( LCUI_Selector selector )
{
	char fullname[MAX_NAME_LEN] = "";
	LCUI_SelectorNode sn = selector->list[selector->length-1];
	if( sn->type ) {
		strcat( fullname, sn->type );
	}
	if( sn->id ) {
		strcat( fullname, "#" );
		strcat( fullname, sn->id );
	}
	if( sn->class_name ) {
		strcat( fullname, "." );
		strcat( fullname, sn->class_name );
	}
	if( sn->pseudo_class_name ) {
		strcat( fullname, ":" );
		strcat( fullname, sn->pseudo_class_name );
	}
	if( fullname[0] == 0 ) {
		return NULL;
	}
	return SelectStyleSheetByName( selector, fullname );
}

int LCUI_PutStyle( LCUI_Selector selector, LCUI_StyleSheet in_ss )
{
	LCUI_StyleSheet ss;
	ss = SelectStyleSheet( selector );
	if( ss ) {
		StyleSheet_Replace( ss, in_ss );
	}
	return 0;
}

LCUI_BOOL Selector_MatchPath( LCUI_Selector selector, LCUI_Widget *wlist )
{
	int i, n;
	LCUI_Widget *obj_ptr = wlist, w;
	LCUI_SelectorNode sn, *sn_ptr = selector->list;
	/* 定位到最后一个元素 */
	sn_ptr += selector->length-1;
	for( ;*obj_ptr; ++obj_ptr );
	--obj_ptr;
	/* 从右到左遍历，进行匹配 */
	for( ; obj_ptr >= wlist && sn_ptr >= selector->list; --obj_ptr ) {
		w = *obj_ptr;
		sn = *sn_ptr;
		if( sn->id ) {
			if( !w->id || strcmp( w->id, sn->id ) ) {
				continue;
			}
		}
		if( sn->type && strcmp("*", sn->type) ) {
			if( !w->type || strcmp( w->type, sn->type ) ) {
				continue;
			}
		}
		if( sn->class_name ) {
			n = ptrslen( w->classes );
			for( i = 0; i < n; ++i ) {
				if( strcmp( w->classes[i],
					    sn->class_name ) == 0 ) {
					break;
				}
			}
			if( i >= n ) {
				continue;
			}
		}
		if( sn->pseudo_class_name ) {
			n = ptrslen( w->status );
			for( i = 0; i < n; ++i ) {
				if( strcmp( w->status[i],
					    sn->pseudo_class_name ) == 0 ) {
					break;
				}
			}
			if( i >= n ) {
				continue;
			}
		}
		--sn_ptr;
	}
	/* 匹配成功的标志是遍历完选择器列表 */
	if( sn_ptr < selector->list ) {
		return TRUE;
	}
	return FALSE;
}

static int InsertStyleSheet( LinkedList *list, LCUI_Selector s,
			     LCUI_StyleSheet ss )
{
	int pos = -1, i = 0;
	StyleListNode pack;
	LinkedListNode *node;
	LinkedList_ForEach( node, list ) {
		pack = node->data;
		if( s->rank > pack->selector->rank ) {
			pos = i;
			break;
		} else if( s->rank == pack->selector->rank ) {
			if( s->batch_num > pack->selector->batch_num ) {
				pos = i;
				break;
			}
		}
		i += 1;
	}
	pack = NEW( StyleListNodeRec, 1 );
	pack->selector = s;
	pack->style = ss;
	if( pos >= 0 ) {
		LinkedList_Insert( list, pos, pack );
	} else {
		LinkedList_Append( list, pack );
	}
	return pos;
}

static int FindStyleNodeByName( const char *name, LCUI_Widget widget,
				LinkedList *list )
{
	int n, count = 0;
	StyleListNode sln;
	LinkedList *styles;
	LCUI_RBTreeNode *tn;
	LinkedListNode *node;
	LCUI_Widget w, wlist[MAX_SELECTOR_DEPTH];
	tn = RBTree_CustomSearch( &style_library.tree, (const void*)name );
	if( !tn ) {
		return 0;
	}
	for( n = 0, w = widget; w; ++n, w = w->parent );
	if( n >= MAX_SELECTOR_DEPTH ) {
		return -1;
	}
	if( n == 0 ) {
		wlist[0] = NULL;
	} else {
		w = widget;
		wlist[n] = NULL;
		while( --n >= 0 ) {
			wlist[n] = w;
			w = w->parent;
		}
	}
	styles = &((StyleTreeNode)tn->data)->styles;
	LinkedList_ForEach( node, styles ) {
		sln = node->data;
		/* 如果当前元素在该样式结点的作用范围内 */
		if( !Selector_MatchPath( sln->selector, wlist ) ) {
			continue;
		}
		InsertStyleSheet( list, sln->selector, sln->style );
		++count;
	}
	return count;
}

/** 从小到大，排序名称列表 */
static void SortNames( char **names, LinkedList *outnames )
{
	char *name;
	int i, len;
	LinkedListNode *node;
	len = ptrslen( names );
	while( --len >= 0 ) {
		i = 0;
		name = names[len];
		LinkedList_ForEach( node, outnames ) {
			if( strcmp(name, node->data) > 0 ) {
				++i;
				continue;
			}
			LinkedList_Insert( outnames, i, name );
			break;
		}
		if( i >= outnames->length ) {
			LinkedList_Append( outnames, name );
		}
	}
}

/** 初始化样式表查找器 */
static void StyleSheetFinder_Init( StyleSheetFinder sfinder, LCUI_Widget w )
{
	sfinder->level = 0;
	sfinder->class_i = 0;
	sfinder->name_i = 0;
	sfinder->status_i = 0;
	sfinder->name[0] = 0;
	sfinder->widget = w;
	LinkedList_Init( &sfinder->classes );
	LinkedList_Init( &sfinder->status );
	SortNames( w->classes, &sfinder->classes );
	SortNames( w->status, &sfinder->status );
}

/** 销毁样式表查找器 */
static void StyleSheetFinder_Destroy( StyleSheetFinder sfinder )
{
	sfinder->name_i = 0;
	sfinder->name[0] = 0;
	sfinder->class_i = 0;
	sfinder->status_i = 0;
	sfinder->widget = NULL;
	sfinder->level = LEVEL_NONE;
	LinkedList_Clear( &sfinder->classes, NULL );
	LinkedList_Clear( &sfinder->status, NULL );
}

/* 查找匹配的样式表 */
int StyleSheetFinder_Find( StyleSheetFinder sfinder, LinkedList *list )
{
	LinkedListNode *node;
	int i, name_len, len, old_len, old_level, count = 0;
	char *fullname = sfinder->name + sfinder->name_i;
	old_len = len = strlen( fullname );
	old_level = sfinder->level;
	switch( sfinder->level ) {
	case LEVEL_TYPE: 
		/* 按类型选择器搜索样式表 */
		if( !sfinder->widget->type ) {
			return 0;
		}
		strcpy( fullname, sfinder->widget->type );
		count = FindStyleNodeByName( fullname, sfinder->widget, list );
		break;
	case LEVEL_ID: 
		/* 按ID选择器搜索样式表 */
		if( !sfinder->widget->id ) {
			return 0;
		}
		fullname[len++] = '#';
		strcpy( fullname + len, sfinder->widget->id );
		count = FindStyleNodeByName( fullname, sfinder->widget, list );
		fullname[old_len] = 0;
		break;
	case LEVEL_CLASS:
		/* 按类选择器搜索样式表
		 * 假设当前选择器全名为：textview#main-btn-text，且有 .a .b .c 
		 * 这三个类，那么下面的处理将会拆分成以下三个选择器来匹配样式表：
		 * textview#test-text.a
		 * textview#test-text.b
		 * textview#test-text.a
		 */
		fullname[len++] = '.';
		LinkedList_ForEach( node, &sfinder->classes ) {
			strcpy( fullname + len, node->data );
			count += FindStyleNodeByName( fullname, 
						      sfinder->widget, list );
			/* 将当前选择器名与其它层级的选择器名组合 */
			for( i = sfinder->level + 1; i < LEVEL_TOTAL_NUM; ++i ) {
				sfinder->level = i;
				count += StyleSheetFinder_Find( sfinder, list );
			}
			sfinder->level = LEVEL_CLASS;
			sfinder->class_i += 1;
		}
		sfinder->level = LEVEL_CLASS;
		fullname[old_len] = 0;
		sfinder->class_i = 0;
		return count;
	case LEVEL_CLASS_2:
		/* 按类选择器搜索交集样式表，结果类似于这样： 
		 * textview#test-text.a.b
		 * textview#test-text.a.c
		 * textview#test-text.b.c
		 * textview#test-text.a.b.c
		 */
		i = 0;
		fullname[len++] = '.';
		LinkedList_ForEach( node, &sfinder->classes ) {
			if( i <= sfinder->class_i ) {
				i += 1;
				continue;
			}
			name_len = strlen( node->data );
			strcpy( fullname + len, node->data );
			count += FindStyleNodeByName( fullname, 
						      sfinder->widget, list );
			sfinder->class_i += 1;
			count += StyleSheetFinder_Find( sfinder, list );
			sfinder->class_i -= 1;
			sfinder->level = LEVEL_STATUS;
			/** 
			 * 递归拼接伪类名，例如： 
			 * textview#main-btn-text:active
			 */
			count += StyleSheetFinder_Find( sfinder, list );
			sfinder->level = LEVEL_CLASS_2;
			i += 1;
		}
		fullname[old_len] = 0;
		sfinder->level = LEVEL_CLASS_2;
		return count;
	case LEVEL_STATUS:
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
		LinkedList_ForEach( node, &sfinder->status ) {
			strcpy( fullname + len, node->data );
			count += FindStyleNodeByName( fullname, 
						      sfinder->widget, list );
			/**
			 * 递归调用，以一层层拼接出像下面这样的选择器：
			 * textview#main-btn-text:active:focus:hover
			 */
			count += StyleSheetFinder_Find( sfinder, list );
			sfinder->status_i += 1;
		}
		sfinder->level = LEVEL_STATUS;
		fullname[old_len] = 0;
		sfinder->status_i = 0;
		return count;
	case LEVEL_STATUS_2:
		/** 按伪类选择器搜索交集样式表 */
		i = 0;
		LinkedList_ForEach( node, &sfinder->status ) {
			if( i <= sfinder->status_i ) {
				i += 1;
				continue;
			}
			fullname[len] = ':';
			name_len = strlen( node->data );
			strcpy( fullname + len + 1, node->data );
			count += FindStyleNodeByName( fullname, 
						      sfinder->widget, list );
			sfinder->status_i += 1;
			count += StyleSheetFinder_Find( sfinder, list );
			sfinder->status_i -= 1;
			i += 1;
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
		count += StyleSheetFinder_Find( sfinder, list );
	}
	fullname[old_len] = 0;
	sfinder->level = old_level;
	return count;
}

/** 查找样式数据节点 */
static int FindStyleNode( LCUI_Widget w, LinkedList *list )
{
	int count;
	StyleSheetFinderRec sfinder;
	StyleSheetFinder_Init( &sfinder, w );
	count = StyleSheetFinder_Find( &sfinder, list );
	StyleSheetFinder_Destroy( &sfinder );
	/* 记录作用于全局元素的样式表 */
	count += FindStyleNodeByName( "*", w, list );
	return count;
}

void LCUI_PrintStyleSheet( LCUI_StyleSheet ss )
{
	int key;
	const char *name;
	for( key = 0; key<ss->length; ++key ) {
		if( !ss->sheet[key].is_valid ) {
			continue;
		}
		name = GetStyleName(key);
		if( name ) {
			printf( "\t\t%s", name );
		} else {
			printf( "\t\t<unknown style %d>", key);
		}
		printf( "%s: ", key > STYLE_KEY_TOTAL ? " (+)" : "" );
		switch( ss->sheet[key].type ) {
		case SVT_AUTO:
			printf( "auto\n");
			break;
		case SVT_VALUE: {
			LCUI_BOOL b = ss->sheet[key].value;
			printf( "%s\n", b ? "true" : "false" );
			break;
		}
		case SVT_COLOR: {
			LCUI_Color *clr = &ss->sheet[key].color;
			if( clr->alpha < 255 ) {
				printf("rgba(%d,%d,%d,%0.2f)\n", clr->r,
					clr->g, clr->b, clr->a / 255.0);
			} else {
				printf("#%02x%02x%02x\n", clr->r, clr->g, clr->b);
			}
			break;
		}
		case SVT_PX:
			printf("%dpx\n", ss->sheet[key].px);
			break;
		case SVT_STRING:
			printf("%s\n", ss->sheet[key].string);
			break;
		case SVT_SCALE:
			printf("%.2lf%%\n", ss->sheet[key].scale*100);
			break;
		default:
			printf("%d\n", ss->sheet[key].value);
			break;
		}
	}
}

void LCUI_PrintSelector( LCUI_Selector selector )
{
	char path[256];
	LCUI_SelectorNode *sn;

	path[0] = 0;
	for( sn = selector->list; *sn; ++sn ) {
		if( (*sn)->id ) {
			strcat( path, "#" );
			strcat( path, (*sn)->id );
		}
		if( (*sn)->type ) {
			strcat( path, (*sn)->type );
		}
		if( (*sn)->class_name ) {
			strcat( path, "." );
			strcat( path, (*sn)->class_name );
		}
		if( (*sn)->pseudo_class_name ) {
			strcat( path, ":" );
			strcat( path, (*sn)->pseudo_class_name );
		}
		strcat( path, " " );
	}
	printf("\tpath: %s (rank = %d, batch_num = %d)\n", path,
		selector->rank, selector->batch_num);
}

void LCUI_PrintStyleLibrary(void)
{
	LCUI_RBTreeNode *tn;
	LinkedListNode *node;
	StyleTreeNode stn;
	StyleListNode sln;

	printf("style library begin\n");
	tn = RBTree_First( &style_library.tree );
	while( tn ) {
		stn = tn->data;
		printf("target: %s\n", stn->name);
		LinkedList_ForEach( node, &stn->styles ) {
			sln = node->data;
			LCUI_PrintSelector( sln->selector );
			LCUI_PrintStyleSheet( sln->style );
		}
		tn = RBTree_Next( tn );
	}
	printf("style library end\n");
}

int Widget_ComputeInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss )
{
	LinkedList list;
	StyleListNode sln;
	LinkedListNode *node;
	LinkedList_Init( &list );
	FindStyleNode( w, &list );
	StyleSheet_Clear( out_ss );
	LinkedList_ForEach( node, &list ) {
		sln = node->data;
		StyleSheet_Merge( out_ss, sln->style );
	}
	LinkedList_Clear( &list, free );
	return 0;
}

void Widget_UpdateStyle( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	if( is_update_all ) {
		Widget_AddTask( w, WTT_REFRESH_STYLE );
	} else {
		Widget_AddTask( w, WTT_UPDATE_STYLE );
	}
}

void Widget_FlushStyle( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	int i, key;
	LCUI_Style s;
	LCUI_WidgetClass *wc;
	LCUI_BOOL need_update_expend_style;
	typedef struct {
		int start, end, task;
		LCUI_BOOL is_valid;
	} TaskMap;
	TaskMap task_map[] = {
		{ key_display_start, key_display_end, WTT_VISIBLE, TRUE },
		{ key_width, key_height, WTT_RESIZE, TRUE },
		{ key_padding_top, key_padding_left, WTT_RESIZE, TRUE },
		{ key_position_start, key_position_end, WTT_POSITION, TRUE },
		{ key_vertical_align, key_vertical_align, WTT_POSITION, TRUE },
		{ key_border_start, key_border_end, WTT_BORDER, TRUE },
		{ key_background_start, key_background_end, WTT_BACKGROUND, TRUE },
		{ key_box_shadow_start, key_box_shadow_end, WTT_SHADOW, TRUE },
		{ key_pointer_events, key_pointer_events, WTT_PROPS, TRUE },
		{ key_box_sizing, key_box_sizing, WTT_RESIZE, TRUE }
	};
	if( is_update_all ) {
		Widget_ComputeInheritStyle( w, w->inherited_style );
	}
	StyleSheet_Clear( w->style );
	StyleSheet_Merge( w->style, w->custom_style );
	StyleSheet_Merge( w->style, w->inherited_style );
	/* 对比两张样式表，确定哪些需要更新 */
	for( key = 0; key < w->style->length; ++key ) {
		s = &w->style->sheet[key];
		if( !s->is_changed ) {
			continue;
		}
		s->is_changed = FALSE;
		if( key >= STYLE_KEY_TOTAL ) {
			need_update_expend_style = TRUE;
			continue;
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
}
