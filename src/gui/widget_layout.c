/* ***************************************************************************
 * widget_layout.c -- the widget layout processing module.
 *
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
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
 * widget_layout.c -- 部件的布局处理模块。
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
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

#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

 /** 布局环境（上下文）数据 */
typedef struct LCUI_LayoutContextRec_ {
	float x, y;		/**< 坐标 */
	float line_height;	/**< 当前行的高度 */
	float line_width;	/**< 当前行的宽度 */
	LCUI_Widget container;	/**< 容器 */
	LCUI_Widget start;	/**< 当前行第一个部件 */
	LCUI_Widget current;	/**< 当前处理的部件 */
	LCUI_Widget prev;	/**< 上一个部件 */
	int prev_display;	/**< 上一个部件的显示方式 */
	float max_width;	/**< 容器的最大宽度 */
} LCUI_LayoutContextRec, *LCUI_LayoutContext;

#define HandleStyle(W, DATA, FUNC, KEY) do {\
	HandleJustifyContent( DATA, StyleSheet_GetStyle((W)->style, KEY) );\
} while( 0 );

static void HandleJustifyContent( LCUI_FlexLayoutStyle *data, LCUI_Style style )
{
	if( style->type != SVT_STYLE || !style->is_valid ) {
		data->justify_content = SV_FLEX_START;
		return;
	}
	data->justify_content = style->val_style;
}

void Widget_ComputeFlexLayoutStyle( LCUI_Widget w )
{
	LCUI_FlexLayoutStyle *data = &w->computed_style.flex;
	HandleStyle( w, data, HandleJustifyContent, key_justify_content );
}

void Widget_UpdateLayout( LCUI_Widget w )
{
	if( w->computed_style.display == SV_FLEX ) {
		Widget_ComputeFlexLayoutStyle( w );
	}
	Widget_AddTask( w, WTT_LAYOUT );
}

/** 布局当前行的所有元素 */
static void LCUILayout_HandleCurrentLine( LCUI_LayoutContext ctx )
{
	float offset_x = 0;
	LinkedListNode *node;
	LCUI_WidgetStyle *style;
	LCUI_Widget child;

	style = &ctx->container->computed_style;
	if( style->display != SV_FLEX || !ctx->start ) {
		goto finish_layout;
	}
	switch( style->flex.justify_content ) {
	case SV_CENTER:
		offset_x = (ctx->max_width - ctx->line_width) / 2;
		break;
	case SV_FLEX_END:
		offset_x = ctx->max_width - ctx->line_width;
		break;
	case SV_FLEX_START:
	default:
		goto finish_layout;
	}
	node = &ctx->start->node;
	while( node ) {
		if( ctx->current ) {
			if( node == &ctx->current->node ) {
				break;
			}
		}
		child = node->data;
		child->origin_x += offset_x;
		Widget_UpdatePosition( child );
		node = node->next;
	}
	ctx->start = ctx->current;

finish_layout:
	ctx->x = 0;
	ctx->y += ctx->line_height;
	ctx->line_width = 0;
	ctx->line_height = 0;
}

/** 布局当前内联块级元素 */
static void LCUILayout_HandleInlineBlock( LCUI_LayoutContext ctx )
{
	ctx->current->origin_x = ctx->x;
	ctx->x += ctx->current->box.outer.width;
	ctx->line_width += ctx->current->box.outer.width;;
	/* 只考虑小数点后两位 */
	if( ctx->x - ctx->max_width >= 0.01 ) {
		LCUILayout_HandleCurrentLine( ctx );
		ctx->current->origin_x = 0;
	}
	ctx->current->origin_y = ctx->y;
	if( ctx->current->box.outer.height > ctx->line_height ) {
		ctx->line_height = ctx->current->box.outer.height;
	}
}

/** 布局当前块级元素 */
static void LCUILayout_HandleBlock( LCUI_LayoutContext ctx )
{
	ctx->x = 0;
	if( ctx->prev && ctx->prev_display != SV_BLOCK ) {
		LCUILayout_HandleCurrentLine( ctx );
	}
	ctx->current->origin_x = ctx->x;
	ctx->current->origin_y = ctx->y;
	ctx->line_height = 0;
	ctx->y += ctx->current->box.outer.height;
	ctx->start = Widget_GetNext( ctx->current );
}

static LCUI_LayoutContext LCUILayout_Begin( LCUI_Widget w )
{
	ASSIGN( ctx, LCUI_LayoutContext );
	ctx->container = w;
	ctx->x = ctx->y = 0;
	ctx->line_height = ctx->line_width = 0;
	ctx->max_width = Widget_ComputeMaxWidth( w );
	ctx->current = LinkedList_Get( &w->children, 0 );
	ctx->start = ctx->current;
	ctx->prev_display = SV_BLOCK;
	ctx->prev = NULL;
	return ctx;
}

static void LCUILayout_End( LCUI_LayoutContext ctx )
{
	if( ctx->current ) {
		ctx->current = Widget_GetNext( ctx->current );
	}
	LCUILayout_HandleCurrentLine( ctx );
	free( ctx );
}

void Widget_ExecUpdateLayout( LCUI_Widget w )
{
	LinkedListNode *node;
	LCUI_LayoutContext ctx;
	LCUI_WidgetEventRec ev = { 0 };

	ctx = LCUILayout_Begin( w );
	for( LinkedList_Each( node, &w->children ) ) {
		ctx->current = node->data;
		if( ctx->current->computed_style.position != SV_STATIC &&
		    ctx->current->computed_style.position != SV_RELATIVE ) {
			continue;
		}
		switch( ctx->current->computed_style.display ) {
		case SV_INLINE_BLOCK:
			LCUILayout_HandleInlineBlock( ctx );
			break;
		case SV_FLEX:
		case SV_BLOCK:
			LCUILayout_HandleBlock( ctx );
			break;
		case SV_NONE:
		default:
			continue;
		}
		Widget_UpdatePosition( ctx->current );
		Widget_AddState( ctx->current, WSTATE_LAYOUTED );
		ctx->prev_display = ctx->current->computed_style.display;
		ctx->prev = ctx->current;
	}
	LCUILayout_End( ctx );
	if( w->style->sheet[key_width].type == SVT_AUTO ||
	    w->style->sheet[key_height].type == SVT_AUTO ) {
		Widget_AddTask( w, WTT_RESIZE );
	}
	ev.cancel_bubble = TRUE;
	ev.type = WET_AFTERLAYOUT;
	Widget_TriggerEvent( w, &ev, NULL );
}
