/* ***************************************************************************
 * widget.c -- GUI widget helper APIs.
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
 * widget.c -- GUI 部件辅助接口，用于简化部件操作
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

#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

void Widget_SetPadding( LCUI_Widget w, float top, float right, 
			float bottom, float left )
{
	Widget_SetStyle( w, key_padding_top, top, px );
	Widget_SetStyle( w, key_padding_right, right, px );
	Widget_SetStyle( w, key_padding_bottom, bottom, px );
	Widget_SetStyle( w, key_padding_left, left, px );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetMargin( LCUI_Widget w, float top, float right,
		       float bottom, float left )
{
	Widget_SetStyle( w, key_margin_top, top, px );
	Widget_SetStyle( w, key_margin_right, right, px );
	Widget_SetStyle( w, key_margin_bottom, bottom, px );
	Widget_SetStyle( w, key_margin_left, left, px );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetBorder( LCUI_Widget w, float width, int style, LCUI_Color clr )
{
	Widget_SetStyle( w, key_border_top_color, clr, color );
	Widget_SetStyle( w, key_border_right_color, clr, color );
	Widget_SetStyle( w, key_border_bottom_color, clr, color );
	Widget_SetStyle( w, key_border_left_color, clr, color );
	Widget_SetStyle( w, key_border_top_width, width, px );
	Widget_SetStyle( w, key_border_right_width, width, px );
	Widget_SetStyle( w, key_border_bottom_width, width, px );
	Widget_SetStyle( w, key_border_left_width, width, px );
	Widget_SetStyle( w, key_border_top_style, style, style );
	Widget_SetStyle( w, key_border_right_style, style, style );
	Widget_SetStyle( w, key_border_bottom_style, style, style );
	Widget_SetStyle( w, key_border_left_style, style, style );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetBoxShadow( LCUI_Widget w, float x, float y,
			  float blur, LCUI_Color color )
{
	Widget_SetStyle( w, key_box_shadow_x, x, px );
	Widget_SetStyle( w, key_box_shadow_y, y, px );
	Widget_SetStyle( w, key_box_shadow_blur, blur, px );
	Widget_SetStyle( w, key_box_shadow_color, color, color );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Move( LCUI_Widget w, float left, float top )
{
	Widget_SetStyle( w, key_top, top, px );
	Widget_SetStyle( w, key_left, left, px );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Resize( LCUI_Widget w, float width, float height )
{
	Widget_SetStyle( w, key_width, width, px );
	Widget_SetStyle( w, key_height, height, px );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_ResizeWithSurface( LCUI_Widget w, float width, float height )
{
	Widget_Resize( w, width, height );
	Widget_AddTask( w, WTT_RESIZE_WITH_SURFACE );
}

void Widget_Show( LCUI_Widget w )
{
	Widget_SetStyle( w, key_display, TRUE, int );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_Hide( LCUI_Widget w )
{
	Widget_SetStyle( w, key_display, FALSE, int );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetPosition( LCUI_Widget w, LCUI_StyleValue position )
{
	Widget_SetStyle( w, key_position, position, style );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetBoxSizing( LCUI_Widget w, LCUI_StyleValue sizing )
{
	Widget_SetStyle( w, key_box_sizing, sizing, style );
	Widget_UpdateStyle( w, FALSE );
}

void Widget_SetDisabled( LCUI_Widget w, LCUI_BOOL disabled )
{
	w->disabled = disabled;
	if( w->disabled ) {
		Widget_AddStatus( w, "disabled" );
	} else {
		Widget_RemoveStatus( w, "disabled" );
	}
}
