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

#ifndef LCUI_WIDGET_HELPER_H
#define LCUI_WIDGET_HELPER_H

LCUI_BEGIN_HEADER

#define Widget_IsVisible(W) (W)->computed_style.visible

 /** 设置内边距 */
LCUI_API void Widget_SetPadding( LCUI_Widget w, float top, float right,
				 float bottom, float left );

/** 设置外边距 */
LCUI_API void Widget_SetMargin( LCUI_Widget w, float top, float right,
				float bottom, float left );

/** 设置边框样式 */
LCUI_API void Widget_SetBorder( LCUI_Widget w, float width,
				int style, LCUI_Color clr );

/** 设置阴影样式 */
LCUI_API void Widget_SetBoxShadow( LCUI_Widget w, float x, float y,
				   float blur, LCUI_Color color );

/** 移动部件位置 */
LCUI_API void Widget_Move( LCUI_Widget w, float left, float top );

/** 调整部件尺寸 */
LCUI_API void Widget_Resize( LCUI_Widget w, float width, float height );

LCUI_API void Widget_ResizeWithSurface( LCUI_Widget w,
					float width, float height );

LCUI_API void Widget_Show( LCUI_Widget w );

LCUI_API void Widget_Hide( LCUI_Widget w );

LCUI_API void Widget_SetPosition( LCUI_Widget w, LCUI_StyleValue position );

LCUI_API void Widget_SetBoxSizing( LCUI_Widget w, LCUI_StyleValue sizing );

LCUI_END_HEADER

#endif
