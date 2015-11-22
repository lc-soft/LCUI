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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

#define MAX_NAME_LEN		128
#define MAX_NODE_DEPTH		32
#define MAX_SELECTOR_DEPTH	32

enum SelectorRank {
	GENERAL_RANK = 0,
	TYPE_RANK = 1,
	CLASS_RANK = 10,
	PCLASS_RANK = 10,
	ID_RANK = 100
};

typedef struct StyleListNode {
	LCUI_StyleSheet style;		/**< 样式表 */
	LCUI_Selector selector;		/**< 作用范围 */
} StyleListNode;

typedef struct StyleTreeNode {
	char name[MAX_NAME_LEN];	/**< 作用对象的名称 */
	LinkedList styles;		/**< 其它样式表 */
} StyleTreeNode;

static struct {
	LCUI_BOOL is_inited;
	LCUI_RBTree style;		/**< 样式树 */
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
	return strcmp( ((StyleTreeNode*)data)->name, (const char*)keydata );
}

/** 删除选择器 */
void DeleteSelector( LCUI_Selector *selector )
{
	LCUI_SelectorNode *node_ptr;
	for( node_ptr=(*selector)->list; *node_ptr; ++node_ptr ) {
		if( (*node_ptr)->type ) {
			free( (*node_ptr)->type );
			(*node_ptr)->type = NULL;
		}
		if( (*node_ptr)->class_name ) {
			free( (*node_ptr)->class_name );
			(*node_ptr)->class_name = NULL;
		}
		if( (*node_ptr)->id ) {
			free( (*node_ptr)->id );
			(*node_ptr)->id = NULL;
		}
		if( (*node_ptr)->pseudo_class_name ) {
			free( (*node_ptr)->pseudo_class_name );
			(*node_ptr)->pseudo_class_name = NULL;
		}
		free( *node_ptr );
	}
	(*selector)->rank = 0;
	(*selector)->length = 0;
	free( (*selector)->list );
	free( *selector );
	*selector = NULL;
}

/** 清空样式表 */
void ClearStyleSheet( LCUI_StyleSheet ss )
{
	int i;
	LCUI_Style *s;
	for( i=0; i<ss->length; ++i ) {
		s = &ss->sheet[i];
		switch( s->type ) {
		case SVT_STRING:
			if( s->string ) {
				free( s->string );
			}
			s->string = NULL;
		default: break;
		}
		s->is_valid = FALSE;
	}
}

/** 删除样式表 */
void DeleteStyleSheet( LCUI_StyleSheet *ss )
{
	ClearStyleSheet( *ss );
	free( (*ss)->sheet );
	free( *ss );
	*ss = NULL;
}

static void DestroyStyleListNode( StyleListNode *node )
{
	DeleteStyleSheet( &node->style );
	DeleteSelector( &node->selector );
}

static void DestroyStyleTreeNode( void *data )
{
	StyleTreeNode *node = (StyleTreeNode*)data;
	LinkedList_Clear( &node->styles, DestroyStyleListNode );
}

/** 合并两个样式表 */
int MergeStyleSheet( LCUI_StyleSheet dest, LCUI_StyleSheet src )
{
	int i, count;
	if( src->length > dest->length ) {
		LCUI_Style *sheet;
		sheet = (LCUI_Style*)realloc( dest->sheet, 
			sizeof(LCUI_Style)*src->length );
		if( !sheet ) {
			return -1;
		}
		for( i=dest->length; i<src->length; ++i ) {
			sheet[i].is_valid = FALSE;
		}
		dest->sheet = sheet;
		dest->length = src->length;
	}
	for( count=0,i=0; i<src->length; ++i ) {
		if( !src->sheet[i].is_valid || dest->sheet[i].is_valid ) {
			continue;
		}
		dest->sheet[i] = src->sheet[i];
		dest->sheet[i].is_changed = TRUE;
		if( src->sheet[i].type != SVT_STRING 
		 || !src->sheet[i].string ) {
			++count;
			continue;
		}
		dest->sheet[i].string = strdup( dest->sheet[i].string );
		++count;
	}
	return 0;
}

/** 覆盖样式表 */
int ReplaceStyleSheet( LCUI_StyleSheet dest, LCUI_StyleSheet src )
{
	int i, count;
	if( src->length > dest->length ) {
		LCUI_Style *sheet;
		sheet = (LCUI_Style*)realloc( dest->sheet, 
			sizeof(LCUI_Style)*src->length );
		if( !sheet ) {
			return -1;
		}
		dest->sheet = sheet;
		dest->length = src->length;
	}
	for( count=0,i=0; i<src->length; ++i ) {
		if( !src->sheet[i].is_valid ) {
			continue;
		}
		if( dest->sheet[i].is_valid
		 && dest->sheet[i].type == SVT_STRING ) {
			if( dest->sheet[i].string ) {
				free( dest->sheet[i].string );
			}
			dest->sheet[i].string = NULL;
		}
		dest->sheet[i] = src->sheet[i];
		dest->sheet[i].is_changed = TRUE;
		if( src->sheet[i].type != SVT_STRING 
		 || !src->sheet[i].string ) {
			++count;
			continue;
		}
		dest->sheet[i].string = strdup( dest->sheet[i].string );
		++count;
	}
	return count;
}

/** 初始化 */
void LCUIWidget_InitStyle( void )
{
	RBTree_Init( &style_library.style );
	RBTree_OnJudge( &style_library.style, CompareName );
	RBTree_OnDestroy( &style_library.style, DestroyStyleTreeNode );
	style_library.is_inited = TRUE;
	LCUICSSParser_Init();
	LCUI_LoadCSS( global_css );
}

/** 销毁，释放资源 */
void LCUIWidget_ExitStyle( void )
{
	RBTree_Destroy( &style_library.style );
	style_library.is_inited = FALSE;
	LCUICSSParser_Destroy();
}

LCUI_StyleSheet StyleSheet( void )
{
	LCUI_StyleSheet ss;
	ss = (LCUI_StyleSheet)malloc( sizeof(LCUI_StyleSheetRec_) );
	if( !ss ) {
		return ss;
	}
	ss->length = LCUI_GetStyleTotal();
	ss->sheet = NEW(LCUI_Style, ss->length + 1);
	return ss;
}

static int SaveSelectorNode( LCUI_SelectorNode node, const char *name, char type )
{

	switch( type ) {
	case 0:
		if( node->type ) {
			return -1;
		}
		node->type = strdup( name );
		return 0;
	case ':':
		if( node->pseudo_class_name ) {
			return -2;
		}
		node->pseudo_class_name = strdup( name );
		return PCLASS_RANK;
	case '.':
		if( node->class_name ) {
			return -3;
		}
		node->class_name = strdup( name );
		return CLASS_RANK;
	case '#':
		if( node->id ) {
			return -4;
		}
		node->id = strdup( name );
		return ID_RANK;
	default: break;
	}
	return 0;
}

/** 根据字符串内容生成相应的选择器 */
LCUI_Selector Selector( const char *selector )
{
	int ni, si, rank;
	const char *p;
	char type = 0, name[MAX_NAME_LEN];
	LCUI_BOOL is_saving = FALSE;
	LCUI_SelectorNode node = NULL;
	LCUI_Selector s;

	s = NEW(LCUI_SelectorRec, 1);
	s->list = NEW(LCUI_SelectorNode, MAX_SELECTOR_DEPTH);
	for( ni = 0, si = 0, p = selector; *p; ++p ) {
		if( !node && is_saving ) {
			node = NEW(LCUI_SelectorNodeRec, 1);
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
			/* 保存上个结点 */
			if( is_saving ) {
				rank = SaveSelectorNode( node, name, type );
				if( rank < 0 ) {
					_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
						    selector, p - selector - ni );
					return NULL;
				}
				s->rank += rank;
				ni = 0;
			}
			is_saving = TRUE;
			type = *p;
			continue;
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			if( is_saving ) {
				rank = SaveSelectorNode( node, name, type );
				if(  rank < 0 ) {
					_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
						    selector, p - selector - ni );
					return NULL;
				}
				s->rank += rank;
				is_saving = FALSE;
				++si;
			}
			ni = 0;
			node = NULL;
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
		rank = SaveSelectorNode( s->list[si++], name, type );
		if( rank > 0 ) {
			s->rank += rank;
		}
	}
	s->list[si] = NULL;
	s->length += si;
	return s;
}

static int mystrcmp( const char *str1, const char *str2 )
{
	if( str1 == str2 ) {
		return 0;
	}
	if( str1 == NULL || str2 == NULL ) {
		return -1;
	}
	return strcmp( str1, str2 );
}

/** 判断两个选择器是否相等 */
LCUI_BOOL SelectorIsEqual( LCUI_Selector s1, LCUI_Selector s2 )
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

static LCUI_StyleSheet 
SelectStyleSheetByName( LCUI_Selector selector, const char *name )
{
	int i = 0, pos = -1;
	LinkedListNode *node;
	LCUI_RBTreeNode *tn;
	StyleTreeNode *stn;
	StyleListNode *sln;
	LCUI_SelectorNode new_sn, *sn_ptr;

	tn = RBTree_CustomSearch( &style_library.style, (const void*)name );
	if( !tn ) {
		stn = NEW(StyleTreeNode, 1);
		strncpy( stn->name, name, MAX_NAME_LEN );
		LinkedList_Init( &stn->styles );
		tn = RBTree_CustomInsert( &style_library.style, 
					  (const void*)name, stn );
	}
	stn = (StyleTreeNode*)tn->data;
	LinkedList_ForEach( node, &stn->styles ) {
		sln = (StyleListNode*)node->data;
		if( SelectorIsEqual(sln->selector, selector) ) {
			return sln->style;
		}
		if( pos == -1 ) {
			if( selector->rank > sln->selector->rank ) {
				pos = i;
			}
		}
		i += 1;
	}
	sln = NEW(StyleListNode, 1);
	sln->style = StyleSheet();
	sln->selector = NEW(LCUI_SelectorRec, 1);
	sln->selector->list = NEW(LCUI_SelectorNode, selector->length+1);
	for( sn_ptr = selector->list, i = 0; *sn_ptr; ++sn_ptr, ++i ) {
		new_sn = NEW(LCUI_SelectorNodeRec, 1);
		if( (*sn_ptr)->type ) {
			new_sn->type = strdup( (*sn_ptr)->type );
		}
		if( (*sn_ptr)->id ) {
			new_sn->id = strdup( (*sn_ptr)->id );
		}
		if( (*sn_ptr)->class_name ) {
			new_sn->class_name = strdup( (*sn_ptr)->class_name );
		}
		if( (*sn_ptr)->pseudo_class_name ) {
			new_sn->pseudo_class_name = strdup( (*sn_ptr)->pseudo_class_name );
		}
		sln->selector->list[i] = new_sn;
	}
	sln->selector->list[i] = NULL;
	sln->selector->length = i;
	sln->selector->rank = selector->rank;
	if( pos >= 0 ) {
		LinkedList_Insert( &stn->styles, pos, sln );
	} else {
		LinkedList_Append( &stn->styles, sln );
	}
	return sln->style;
}

static LCUI_StyleSheet SelectStyleSheet( LCUI_Selector selector )
{
	char fullname[MAX_NAME_LEN];
	LCUI_SelectorNode sn = selector->list[selector->length-1];
	/* 优先级：伪类 > 类 > ID > 名称 */
	if( sn->pseudo_class_name ) {
		fullname[0] = ':';
		strncpy( fullname + 1, sn->pseudo_class_name, MAX_NAME_LEN-1 );
		return SelectStyleSheetByName( selector, fullname );
	}
	if( sn->class_name ) {
		fullname[0] = '.';
		strncpy( fullname + 1, sn->class_name, MAX_NAME_LEN-1 );
		return SelectStyleSheetByName( selector, fullname );
	}
	if( sn->id ) {
		fullname[0] = '#';
		strncpy( fullname + 1, sn->id, MAX_NAME_LEN-1 );
		return SelectStyleSheetByName( selector, fullname );
	}
	if( sn->type ) {
		return SelectStyleSheetByName( selector, sn->type );
	}
	return NULL;
}

/** 向样式库添加样式表 */
int LCUI_PutStyle( LCUI_Selector selector, LCUI_StyleSheet in_ss )
{
	LCUI_StyleSheet ss;
	ss = SelectStyleSheet( selector );
	if( ss ) {
		ReplaceStyleSheet( ss, in_ss );
	}
	return 0;
}

/** 匹配元素路径与样式结点路径 */
LCUI_BOOL IsMatchPath( LCUI_Widget *wlist, LCUI_Selector selector )
{
	int i, n;
	LCUI_Widget *obj_ptr = wlist, w;
	LCUI_SelectorNode *sn_ptr = selector->list;
	/* 定位到最后一个元素 */
	sn_ptr += selector->length-1;
	for( ;*obj_ptr; ++obj_ptr );
	--obj_ptr;
	/* 从右到左遍历，进行匹配 */
	for( ; obj_ptr >= wlist && sn_ptr >= selector->list; --obj_ptr ) {
		w = *obj_ptr;
		if( (*sn_ptr)->id ) {
			if( strcmp( w->id, (*sn_ptr)->id ) ) {
				continue;
			}
		}
		if( (*sn_ptr)->type && strcmp("*", (*sn_ptr)->type) ) {
			if( strcmp(w->type, (*sn_ptr)->type) ) {
				continue;
			}
		}
		if( (*sn_ptr)->class_name ) {
			n = ptrslen( w->classes );
			for( i = 0; i < n; ++i ) {
				if( strcmp(w->classes[i],
				 (*sn_ptr)->class_name) == 0 ) {
					break;
				}
			}
			if( i >= n ) {
				continue;
			}
		}
		if( (*sn_ptr)->pseudo_class_name ) {
			n = ptrslen( w->pseudo_classes );
			for( i = 0; i < n; ++i ) {
				if( strcmp(w->pseudo_classes[i],
					(*sn_ptr)->pseudo_class_name) == 0 ) {
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

static int FindStyleNodeByName( const char *name, LCUI_Widget widget,
				LinkedList *list )
{
	LCUI_RBTreeNode *tn;
	StyleListNode *sln;
	LinkedList *styles;
	LinkedListNode *node;
	LCUI_Widget w, wlist[MAX_SELECTOR_DEPTH];
	int n, count = 0;
	
	tn = RBTree_CustomSearch( &style_library.style, (const void*)name );
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
		wlist[n] = NULL;
		w = widget;
		while( --n >= 0 ) {
			wlist[n] = w;
			w = w->parent;
		}
	}
	styles = &((StyleTreeNode*)tn->data)->styles;
	LinkedList_ForEach( node, styles ) {
		sln = (StyleListNode*)node->data;
		/* 如果当前元素在该样式结点的作用范围内 */
		if( IsMatchPath(wlist, sln->selector) ) {
			LinkedList_Append( list, sln->style );
			++count;
		}
	}
	return count;
}

static int FindStyleNode( LCUI_Widget w, LinkedList *list )
{
	int i, count = 0;
	char fullname[MAX_NAME_LEN];
	
	i = ptrslen( w->pseudo_classes );
	/* 记录伪类选择器匹配的样式表 */
	while( --i >= 0 ) {
		fullname[0] = ':';
		strncpy( fullname + 1, w->pseudo_classes[i], MAX_NAME_LEN-1 );
		count += FindStyleNodeByName( fullname, w, list );
	}
	i = ptrslen( w->classes );
	/* 记录类选择器匹配的样式表 */
	while( --i >= 0 ) {
		fullname[0] = '.';
		strncpy( fullname + 1, w->classes[i], MAX_NAME_LEN-1 );
		count += FindStyleNodeByName( fullname, w, list );
	}
	/* 记录ID选择器匹配的样式表 */
	if( w->id ) {
		fullname[0] = '#';
		strncpy( fullname + 1, w->id, MAX_NAME_LEN-1 );
		count += FindStyleNodeByName( fullname, w, list );
	}
	/* 记录名称选择器匹配的样式表 */
	if( w->type ) {
		count += FindStyleNodeByName( w->type, w, list );
	}
	/* 记录作用于全局元素的样式表 */
	count += FindStyleNodeByName( "*", w, list );
	return count;
}

/** 打印样式表的内容 */
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

/** 打印选择器的内容 */
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
	printf("\tpath: %s\n", path);
}

/** 打印样式库中的内容 */
void LCUI_PrintStyleLibrary(void)
{
	LCUI_RBTreeNode *tn;
	LinkedListNode *node;
	StyleTreeNode *stn;
	StyleListNode *sln;
	
	printf("style library begin\n");
	tn = RBTree_First( &style_library.style );
	while( tn ) {
		stn = (StyleTreeNode*)tn->data;
		printf("target: %s\n", stn->name);
		LinkedList_ForEach( node, &stn->styles ) {
			sln = (StyleListNode*)node->data;
			LCUI_PrintSelector( sln->selector );
			LCUI_PrintStyleSheet( sln->style );
		}
		tn = RBTree_Next( tn );
	}
	printf("style library end\n");
}

/** 计算部件继承得到的样式表 */
int Widget_ComputeInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss )
{
	LinkedList list;
	LinkedListNode *node;
	LinkedList_Init( &list );
	FindStyleNode( w, &list );
	ClearStyleSheet( out_ss );
	LinkedList_ForEach( node, &list ) {
		MergeStyleSheet( out_ss, node->data );
	}
	LinkedList_Clear( &list, DeleteStyleSheet );
	return 0;
}

/** 更新当前部件的样式 */
void Widget_Update( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	int i, key;
	LCUI_WidgetClass *wc;
	LCUI_BOOL need_update_expend_style;
	LCUI_Style *s;
	typedef struct {
		int start, end, task;
		LCUI_BOOL is_valid;
	} TaskMap;
	TaskMap task_map[] = {
		{ key_display_start, key_display_end, WTT_VISIBLE, TRUE },
		{ key_width, key_height, WTT_RESIZE, TRUE },
		{ key_padding_top, key_padding_left, WTT_RESIZE, TRUE },
		{ key_position_start, key_position_end, WTT_POSITION, TRUE },
		{ key_border_start, key_border_end, WTT_BORDER, TRUE },
		{ key_background_start, key_background_end, WTT_BACKGROUND, TRUE },
		{ key_box_shadow_start, key_box_shadow_end, WTT_SHADOW, TRUE }
	};
	if( is_update_all ) {
		Widget_ComputeInheritStyle( w, w->inherited_style );
	}
	ClearStyleSheet( w->style );
	MergeStyleSheet( w->style, w->custom_style );
	MergeStyleSheet( w->style, w->inherited_style );
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
