/*
 * widget_style.c -- widget style library module for LCUI.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/css_fontstyle.h>

#define ARRAY_LEN(ARR) sizeof( ARR ) / sizeof( ARR[0] )

typedef struct LCUI_TaskCacheStatus {
	size_t start, end;
	LCUI_WidgetTaskType task;
	LCUI_BOOL is_valid;
} LCUI_TaskStatus;

/** 部件的缺省样式 */
const char *global_css = CodeToString(

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

root {
	focusable: true;
}

);

LCUI_SelectorNode Widget_GetSelectorNode( LCUI_Widget w )
{
	int i;
	ASSIGN( sn, LCUI_SelectorNode );
	ZEROSET( sn, LCUI_SelectorNode );
	if( w->id ) {
		sn->id = strdup2( w->id );
	}
	if( w->type ) {
		sn->type = strdup2( w->type );
	}
	for( i = 0; w->classes && w->classes[i]; ++i ) {
		sortedstrsadd( &sn->classes, w->classes[i] );
	}
	for( i = 0; w->status && w->status[i]; ++i ) {
		sortedstrsadd( &sn->status, w->status[i] );
	}
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
		Widget_AddTaskForChildren( w, LCUI_WTASK_REFRESH_STYLE );
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

void Widget_PrintStyleSheets( LCUI_Widget w )
{
	LCUI_Selector s = Widget_GetSelector( w );
	LCUI_PrintStyleSheetsBySelector( s );
	Selector_Delete( s );
}

void Widget_UpdateStyle( LCUI_Widget w, LCUI_BOOL is_refresh_all )
{
	if( is_refresh_all ) {
		StyleSheet_Clear( w->style );
		Widget_AddTask( w, LCUI_WTASK_REFRESH_STYLE );
	} else {
		Widget_AddTask( w, LCUI_WTASK_UPDATE_STYLE );
	}
}

void Widget_UpdateChildrenStyle( LCUI_Widget w, LCUI_BOOL is_refresh_all )
{
	LinkedListNode *node;
	w->task.for_children = TRUE;
	for( LinkedList_Each( node, &w->children ) ) {
		Widget_UpdateStyle( node->data, is_refresh_all );
		Widget_UpdateChildrenStyle( node->data, is_refresh_all );
	}
}

void Widget_ExecUpdateStyle( LCUI_Widget w, LCUI_BOOL is_update_all )
{
	size_t i, key;
	LCUI_Style s;
	LCUI_StyleSheet ss;
	LCUI_TaskStatus task_status[] = {
		{
			key_visible, key_visible, LCUI_WTASK_VISIBLE, TRUE
		}, {
			key_display, key_display, LCUI_WTASK_DISPLAY, TRUE
		}, {
			key_flex_style_start, key_flex_style_end,
			LCUI_WTASK_LAYOUT, TRUE
		}, {
			key_opacity, key_opacity, LCUI_WTASK_OPACITY, TRUE
		}, {
			key_z_index, key_z_index, LCUI_WTASK_ZINDEX, TRUE
		}, {
			key_width, key_height, LCUI_WTASK_RESIZE, TRUE
		}, {
			key_min_width, key_max_height,
			LCUI_WTASK_RESIZE_WITH_SURFACE, TRUE
		}, {
			key_padding_start, key_padding_end,
			LCUI_WTASK_RESIZE_WITH_SURFACE, TRUE
		}, {
			key_box_sizing, key_box_sizing,
			LCUI_WTASK_RESIZE_WITH_SURFACE, TRUE
		}, {
			key_margin_start, key_margin_end,
			LCUI_WTASK_MARGIN, TRUE
		}, {
			key_position_start, key_position_end,
			LCUI_WTASK_POSITION, TRUE
		}, {
			key_vertical_align, key_vertical_align,
			LCUI_WTASK_POSITION, TRUE
		}, {
			key_border_start, key_border_end,
			LCUI_WTASK_BORDER, TRUE
		}, {
			key_background_start, key_background_end,
			LCUI_WTASK_BACKGROUND, TRUE
		}, {
			key_box_shadow_start, key_box_shadow_end,
			LCUI_WTASK_SHADOW, TRUE
		}, {
			key_pointer_events, key_focusable,
			LCUI_WTASK_PROPS, TRUE
		}
	};

	if( is_update_all ) {
		/* 刷新该部件的相关数据 */
		if( w->proto && w->proto->refresh ) {
			w->proto->refresh( w );
		}
		Widget_GetInheritStyle( w, w->inherited_style );
	}
	ss = w->style;
	w->style = StyleSheet();
	StyleSheet_Merge( w->style, w->custom_style );
	StyleSheet_Merge( w->style, w->inherited_style );
	/* 对比两张样式表，确定哪些需要更新 */
	for( key = 0; key < STYLE_KEY_TOTAL; ++key ) {
		s = &w->style->sheet[key];
		if( ss->sheet[key].is_valid == s->is_valid &&
		    ss->sheet[key].type == s->type &&
		    ss->sheet[key].value == s->value ) {
			continue;
		}
		for( i = 0; i < ARRAY_LEN( task_status ); ++i ) {
			if( key >= task_status[i].start &&
			    key <= task_status[i].end ) {
				if( !task_status[i].is_valid ) {
					break;
				}
				task_status[i].is_valid = FALSE;
				Widget_AddTask( w, task_status[i].task );
			}
		}
	}
	if( w->proto && w->proto->update && 
	    w->style->length > STYLE_KEY_TOTAL ) {
		/* 扩展部分的样式交给该部件自己处理 */
		w->proto->update( w );
	}
	StyleSheet_Delete( ss );
}

void LCUIWidget_InitStyle( void )
{
	LCUI_InitCSSLibrary();
	LCUI_InitCSSParser();
	LCUI_InitCSSFontStyle();
	LCUI_LoadCSSString( global_css, NULL );
}

void LCUIWidget_FreeStyle( void )
{
	LCUI_FreeCSSFontStyle();
	LCUI_FreeCSSLibrary();
	LCUI_FreeCSSParser();
}
