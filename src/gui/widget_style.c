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
#include <LCUI/gui/css_library.h>

typedef struct {
	int start, end, task;
	LCUI_BOOL is_valid;
} TaskMap;

/** 部件的缺省样式 */
const char *global_css = ToString(

* {
	width: auto;
	height:	auto;
	background-color: transparent;
	border: 0px solid #000;
	display: block;
	position: static;
	padding: 0;
	margin: 0;
}

);

LCUI_SelectorNode Widget_GetSelectorNode( LCUI_Widget w )
{
	int i;
	ASSIGN( sn, LCUI_SelectorNode );
	ZEROSET( sn, LCUI_SelectorNode );
	Widget_Lock( w );
	if( w->id ) {
		sn->id = strdup( w->id );
	}
	if( w->type ) {
		sn->type = strdup( w->type );
	}
	if( w->classes ) {
		for( i = 0; w->classes[i]; ++i ) {
			sortedstrsadd( &sn->classes, 
				       w->classes[i] );
		}
	}
	if( w->status ) {
		for( i = 0; w->status[i]; ++i ) {
			sortedstrsadd( &sn->status,
				       w->status[i] );
		}
	}
	Widget_Unlock( w );
	SelectorNode_Update( sn );
	return sn;
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
		parent = node->data;
		s->nodes[ni] = Widget_GetSelectorNode( parent );
		s->rank += s->nodes[ni]->rank;
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
			count += LCUI_FindStyleSheetFromGroup( 1, sname,
							       s, NULL );
		}
	}
	Selector_Delete( s );
	LinkedList_Clear( &snames, free );
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
	s = Widget_GetSelector( w );
	LCUI_GetStyleSheet( s, out_ss );
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
	LCUI_BOOL need_update_expend_style = FALSE;
	TaskMap task_map[] = {
		{ key_display_start, key_display_end, WTT_VISIBLE, TRUE },
		{ key_opacity, key_opacity, WTT_OPACITY, TRUE },
		{ key_z_index, key_z_index, WTT_ZINDEX, TRUE },
		{ key_width, key_height, WTT_RESIZE, TRUE },
		{ key_padding_start, key_padding_end, WTT_RESIZE, TRUE },
		{ key_margin_start, key_margin_end, WTT_MARGIN, TRUE },
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
		for( i = 0; i < sizeof( task_map ) / sizeof( TaskMap ); ++i ) {
			if( key >= task_map[i].start &&
			    key <= task_map[i].end ) {
				if( !task_map[i].is_valid ) {
					break;
				}
				task_map[i].is_valid = FALSE;
				Widget_AddTask( w, task_map[i].task );
			}
		}
	}
	if( need_update_expend_style && w->proto && w->proto->update ) {
		/* 扩展部分的样式交给该部件自己处理 */
		w->proto->update( w );
	}
	StyleSheet_Delete( ss );
}

void LCUIWidget_InitStyle( void )
{

	LCUI_InitCSSLibrary();
	LCUI_InitCSSParser();
	LCUI_LoadCSSString( global_css, NULL );
}

void LCUIWidget_ExitStyle( void )
{
	LCUI_ExitCSSLibrary();
	LCUI_ExitCSSParser();
}
