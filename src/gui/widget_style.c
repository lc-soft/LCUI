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
#include <LCUI/widget_build.h>

#define MAX_NAME_LEN		128
#define MAX_NODE_DEPTH		32
#define MAX_SELECTOR_DEPTH	32

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
	for( node_ptr=*selector; *node_ptr; ++node_ptr ) {
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
	free( *selector );
	*selector = NULL;
}

/** 删除样式表 */
void DeleteStyleSheet( LCUI_StyleSheet *ss )
{
	free( *ss );
	*ss = NULL;
}

static void DestroyStyleListNode( void *data )
{
	StyleListNode *node = (StyleListNode*)data;
	DeleteStyleSheet( &node->style );
	DeleteSelector( &node->selector );
}

static void DestroyStyleTreeNode( void *data )
{
	LinkedList_Destroy( &((StyleTreeNode*)data)->styles );
}

/** 清空样式表 */
static void ClearStyleSheet( LCUI_StyleSheet ss )
{
	int i;
	for( i=0; i<STYLE_KEY_TOTAL; ++i ) {
		ss[i].is_valid = FALSE;
	}
}


/** 合并两个样式表 */
static void MergeStyleSheet( LCUI_StyleSheet dest, LCUI_StyleSheet source )
{
	int i;
	for( i=0; i<STYLE_KEY_TOTAL; ++i ) {
		if( source[i].is_valid && !dest[i].is_valid ) {
			dest[i] = source[i];
		}
	}
}

/** 覆盖样式表 */
static void ReplaceStyleSheet( LCUI_StyleSheet out, LCUI_StyleSheet src1, LCUI_StyleSheet src2 )
{
	int i;
	for( i=0; i<STYLE_KEY_TOTAL; ++i ) {
		if( src2[i].is_changed || src1[i].is_changed ) {
			if( src2[i].is_valid ) {
				out[i] = src2[i];
			} else {
				out[i] = src1[i];
			}
			out[i].is_changed = TRUE;
		}
		if( src2[i].is_valid ) {
			out[i] = src2[i];
		} else {
			out[i] = src1[i];
		}
		out[i].is_changed = FALSE;
	}
}

/** 初始化 */
void LCUIWidget_InitStyle( void )
{
	RBTree_Init( &style_library.style );
	RBTree_OnJudge( &style_library.style, CompareName );
	RBTree_OnDestroy( &style_library.style, DestroyStyleTreeNode );
	style_library.is_inited = TRUE;
	LCUICssParser_Init();
}

/** 销毁，释放资源 */
void LCUIWidget_ExitStyle( void )
{
	RBTree_Destroy( &style_library.style );
	style_library.is_inited = FALSE;
	LCUICssParser_Destroy();
}

LCUI_StyleSheet StyleSheet( void )
{
	LCUI_StyleSheet ss;
	ss = (LCUI_StyleSheet)calloc( STYLE_KEY_TOTAL, sizeof(LCUI_Style) );
	ss[key_width].type = SVT_AUTO;
	ss[key_width].value = SV_AUTO;
	ss[key_height].type = SVT_AUTO;
	ss[key_height].value = SV_AUTO;
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
		break;
	case ':':
		if( node->pseudo_class_name ) {
			return -2;
		}
		node->pseudo_class_name = strdup( name );
		break;
	case '.':
		if( node->class_name ) {
			return -3;
		}
		node->class_name = strdup( name );
		break;
	case '#':
		if( node->id ) {
			return -4;
		}
		node->id = strdup( name );
		break;
	default: break;
	}
	return 0;
}

/** 根据字符串内容生成相应的选择器 */
LCUI_Selector Selector( const char *selector )
{
	int ni, si;
	const char *p;
	char type = 0, name[MAX_NAME_LEN];
	size_t size;
	LCUI_BOOL is_saving = FALSE;
	LCUI_SelectorNode node = NULL;
	LCUI_Selector s;

	size = sizeof(LCUI_SelectorNode)*MAX_SELECTOR_DEPTH;
	s = (LCUI_Selector)malloc( size );
	for( ni = 0, si = 0, p = selector; *p; ++p ) {
		if( node == NULL && is_saving ) {
			size = sizeof( struct LCUI_SelectorNodeRec_ );
			node = (LCUI_SelectorNode)malloc( size );
			node->id = NULL;
			node->type = NULL;
			node->class_name = NULL;
			node->pseudo_class_name = NULL;
			if( si >= MAX_SELECTOR_DEPTH ) {
				_DEBUG_MSG( "%s: selector node list is too long.\n",
					    selector );
				return NULL;
			}
			s[si] = node;
		}
		if( *p == ':' || *p == '.' || *p == '#' ) {
			/* 保存上个结点 */
			if( is_saving ) {
				if( SaveSelectorNode(node, name, type) != 0 ) {
					_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
						    selector, p - selector - ni );
					return NULL;
				}
				ni = 0;
			}
			is_saving = TRUE;
			type = *p;
			continue;
		}
		if( *p == ' ' || *p == '\r' || *p == '\n' || *p == '\t' ) {
			if( is_saving ) {
				if( SaveSelectorNode(node, name, type) != 0 ) {
					_DEBUG_MSG( "%s: invalid selector node at %ld.\n",
						    selector, p - selector - ni );
					return NULL;
				}
				is_saving = FALSE;
				++si;
			}
			ni = 0;
			node = NULL;
			continue;
		}
		if( (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')
		 || (*p >= '0' && *p <= '9') || *p == '-' || *p == '_' ) {
			if( !is_saving ) {
				type = 0;
				is_saving = TRUE;
			}
			name[ni++] = *p;
			name[ni] = 0;
			continue;
		}
		_DEBUG_MSG( "%s: unknown char %02x at %ld.\n",
			    selector, *p, p - selector );
		return NULL;
	}
	if( is_saving ) {
		SaveSelectorNode( s[si++], name, type );
	}
	s[si] = NULL;
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
	LCUI_SelectorNode *sn1_ptr = s1, *sn2_ptr = s2;
	for( ; sn1_ptr && sn2_ptr; ++sn1_ptr,++sn2_ptr ) {
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
	int i, n;
	LCUI_RBTreeNode *node;
	StyleTreeNode *stn;
	StyleListNode *sln;
	LCUI_SelectorNode sn;

	node = RBTree_CustomSearch( &style_library.style, (const void*)name );
	if( !node ) {
		stn = (StyleTreeNode*)malloc(sizeof(StyleTreeNode));
		strncpy( stn->name, name, MAX_NAME_LEN );
		LinkedList_Init( &stn->styles, sizeof(StyleListNode) );
		LinkedList_SetDataNeedFree( &stn->styles, TRUE );
		LinkedList_SetDestroyFunc( &stn->styles, DestroyStyleListNode );
		node = RBTree_CustomInsert( &style_library.style,
					    (const void*)name, stn );
	}
	stn = (StyleTreeNode*)node->data;
	LinkedList_ForEach( sln, 0, &stn->styles ) {
		if( SelectorIsEqual(sln->selector, selector) ) {
			return sln->style;
		}
	}
	sln = (StyleListNode*)malloc(sizeof(StyleListNode));
	sln->style = StyleSheet();
	for( n=0; selector[n]; ++n );
	++n;
	sln->selector = (LCUI_Selector)malloc( sizeof(LCUI_SelectorNode*)*n );
	for( i=0, n-=1; i<n; ++i ) {
		sn = (LCUI_SelectorNode)malloc( sizeof(struct LCUI_SelectorNodeRec_) );
		sn->type = NULL;
		sn->id = NULL;
		sn->class_name = NULL;
		sn->pseudo_class_name = NULL;
		if( selector[i]->type ) {
			sn->type = strdup( selector[i]->type );
		}
		if( selector[i]->id ) {
			sn->id = strdup( selector[i]->id );
		}
		if( selector[i]->class_name ) {
			sn->class_name = strdup( selector[i]->class_name );
		}
		if( selector[i]->pseudo_class_name ) {
			sn->pseudo_class_name = strdup( selector[i]->pseudo_class_name );
		}
		sln->selector[i] = sn;
	}
	sln->selector[n] = NULL;
	LinkedList_Append( &stn->styles, sln );
	return sln->style;
}

static LCUI_StyleSheet SelectStyleSheet( LCUI_Selector selector )
{
	int depth;
	char fullname[MAX_NAME_LEN];
	LCUI_SelectorNode sn;

	for( depth = 0; selector[depth]; ++depth );
	sn = selector[depth-1];
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
		ReplaceStyleSheet( ss, ss, in_ss );
	}
	return 0;
}

/** 匹配元素路径与样式结点路径 */
LCUI_BOOL IsMatchPath( LCUI_Widget *wlist, LCUI_Selector selector )
{
	int i, n;
	LCUI_SelectorNode *sn_ptr = selector;
	LCUI_Widget *obj_ptr = wlist, w;

	for( ; *obj_ptr && *sn_ptr; ++obj_ptr ) {
		w = *obj_ptr;
		if( (*sn_ptr)->id ) {
			if( strcmp(w->id, (*sn_ptr)->id) ) {
				continue;
			}
		}
		if( (*sn_ptr)->type ) {
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
		++sn_ptr;
	}
	if( *obj_ptr == NULL && *sn_ptr == NULL ) {
		return TRUE;
	}
	return FALSE;
}

static int FindStyleNodeByName( const char *name, LCUI_Widget widget,
				LinkedList *list )
{
	LCUI_RBTreeNode *node;
	StyleListNode *sln;
	LinkedList *styles;
	LCUI_Widget w, wlist[MAX_SELECTOR_DEPTH];
	int n, count = 0;

	node = RBTree_CustomSearch( &style_library.style, (const void*)name );
	if( !node ) {
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
	styles = &((StyleTreeNode*)node->data)->styles;
	LinkedList_ForEach( sln, 0, styles ) {
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

	i = ptrslen( w->classes );
	/* 记录类选择器匹配的样式表 */
	while( --i >= 0 ) {
		fullname[0] = '.';
		strncpy( fullname + 1, w->classes[i], MAX_NAME_LEN-1 );
		count += FindStyleNodeByName( fullname, w, list );
	}
	i = ptrslen( w->pseudo_classes );
	/* 记录伪类选择器匹配的样式表 */
	while( --i >= 0 ) {
		fullname[0] = ':';
		strncpy( fullname + 1, w->pseudo_classes[i], MAX_NAME_LEN-1 );
		count += FindStyleNodeByName( fullname, w, list );
	}
	/* 记录ID选择器匹配的样式表 */
	if( w->id ) {
		fullname[0] = '#';
		strncpy( fullname + 1, w->id, MAX_NAME_LEN - 1 );
		count += FindStyleNodeByName( fullname, w, list );
	}
	/* 记录名称选择器匹配的样式表 */
	if( w->type ) {
		count += FindStyleNodeByName( w->type, w, list );
	}
	return count;
}

static LCUI_PrintStyleSheet( LCUI_StyleSheet ss )
{
	int key;
	for( key = 0; key < STYLE_KEY_TOTAL; ++key ) {
		if( !ss[key].is_valid ) {
			continue;
		}
		printf("\t\t%s: ", GetStyleName(key));
		switch( ss[key].type ) {
		case SVT_BOOLEAN: {
			LCUI_BOOL b = ss[key].value_boolean;
			printf( "%s\n", b ? "true" : "false" );
			break;
		}
		case SVT_COLOR: {
			LCUI_Color *clr = &ss[key].value_color;
			if( clr->alpha < 255 ) {
				printf("rgba(%d,%d,%d,%0.2f)\n", clr->r,
					clr->g, clr->b, clr->a / 255.0);
			} else {
				printf("#%02x%02x%02x\n", clr->r, clr->g, clr->b);
			}
			break;
		}
		case SVT_PX:
			printf("%dpx\n", ss[key].value_px);
			break;
		case SVT_SCALE:
			printf("%.2lf%%\n", ss[key].value_scale*100);
			break;
		default:
			printf("%d\n", ss[key].value);
			break;
		}
	}
}

/** 打印样式库中的内容 */
void LCUI_PrintStyleLibrary(void)
{
	LCUI_RBTreeNode *node;
	StyleTreeNode *stn;
	StyleListNode *sln;
	LCUI_SelectorNode *sn;
	char path[256];
	
	printf("style library begin\n");
	node = RBTree_First( &style_library.style );
	while( node ) {
		stn = (StyleTreeNode*)node->data;
		printf("target: %s\n", stn->name);
		LinkedList_ForEach( sln, 0, &stn->styles ) {
			path[0] = 0;
			for( sn = sln->selector; *sn; ++sn ) {
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
			LCUI_PrintStyleSheet( sln->style );
		}
		node = RBTree_Next( node );
	}
	printf("style library end\n");
}

/** 计算部件继承得到的样式表 */
int Widget_ComputeInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss )
{
	LCUI_StyleSheet ss;
	LinkedList list;

	LinkedList_Init( &list, sizeof(LCUI_StyleSheet) );
	LinkedList_SetDataNeedFree( &list, FALSE );
	FindStyleNode( w, &list );
	ClearStyleSheet( out_ss );
	LinkedList_ForEach( ss, 0, &list ) {
		MergeStyleSheet( out_ss, ss );
	}
	LinkedList_Destroy( &list );
	return 0;
}

/** 更新当前部件的样式 */
void Widget_Update( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	int i, key;
	typedef struct {
		int start, end, task;
		LCUI_BOOL is_valid;
	} TaskMap;
	TaskMap task_map[] = {
		{ key_visible, key_visible, WTT_VISIBLE, TRUE },
		{ key_width, key_height, WTT_RESIZE, TRUE },
		{ key_padding_top, key_padding_left, WTT_RESIZE, TRUE },
		{ key_position_start, key_position_end, WTT_POSITION, TRUE },
		{ key_border_start, key_border_end, WTT_BORDER, TRUE },
		{ key_background_start, key_background_end, WTT_BACKGROUND, TRUE },
		{ key_box_shadow_start, key_box_shadow_end, WTT_SHADOW, TRUE }
	};

	if( is_update_all ) {
		Widget_ComputeInheritStyle( w, w->inherited_css );
	}
	ReplaceStyleSheet( w->base.css, w->inherited_css, w->base.style );
	/* 对比两张样式表，确定哪些需要更新 */
	for( key = 0; key < STYLE_KEY_TOTAL; ++key ) {
		/* 忽略值没有变化的样式 */
		if( !w->base.css[key].is_valid ) {
			if( !w->css[key].is_valid ) {
				continue;
			}
		}
		else if( !w->base.css[key].is_changed ) {
			if( w->base.css[key].type == w->css[key].type ) {
				if( w->base.css[key].value == w->css[key].value ) {
					continue;
				}
			}
			else if( w->base.css[key].value == w->css[key].value ) {
				continue;
			}
		}
		w->base.css[key].is_changed = FALSE;
		w->css[key] = w->base.css[key];
		for( i = 0; i < sizeof(task_map) / sizeof(TaskMap); ++i ) {
			if( !task_map[i].is_valid ) {
				continue;
			}
			if( key >= task_map[i].start && key <= task_map[i].end ) {
				task_map[i].is_valid = FALSE;
				Widget_AddTask( w, task_map[i].task );
			}
		}
	}
}
