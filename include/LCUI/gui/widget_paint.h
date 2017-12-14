
/* ***************************************************************************
 * widget_paint.h -- LCUI widget paint module.
 * 
 * Copyright (C) 2013-2017 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_paint.h -- LCUI部件绘制模块
 *
 * 版权所有 (C) 2013-2017 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#ifndef __WIDGET_PAINT_H
#define __WIDGET_PAINT_H

LCUI_BEGIN_HEADER

/**
 * 标记部件中的无效区域
 * @param[in] w		区域所在的部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 * @returns 标记成功返回 TRUE，如果该区域处于屏幕可见区域外则标记失败，返回FALSE
 */
LCUI_API LCUI_BOOL Widget_InvalidateArea( LCUI_Widget widget,
					  LCUI_RectF *in_rect, int box_type );

/**
 * 取出部件中的无效区域
 * @param[in] w		部件
 * @param[out] rects	输出的区域列表
 * @return 无效区域的数量
 */
LCUI_API size_t Widget_GetInvalidArea( LCUI_Widget w, LinkedList *rects );

/** 
 * 将部件中的矩形区域转换成指定范围框内有效的矩形区域
 * @param[in]	w		目标部件
 * @param[in]	in_rect		相对于部件呈现框的矩形区域
 * @param[out]	out_rect	转换后的区域
 * @param[in]	box_type	转换后的区域所处的范围框
 */
LCUI_API int Widget_ConvertArea( LCUI_Widget w, LCUI_Rect *in_rect,
				LCUI_Rect *out_rect, int box_type );

/** 将 LCUI_RectF 类型数据转换为无效区域 */
LCUI_API void RectFToInvalidArea( const LCUI_RectF *rect, LCUI_Rect *area );

/** 将 LCUI_Rect 类型数据转换为无效区域 */
LCUI_API void RectToInvalidArea( const LCUI_Rect *rect, LCUI_Rect *area );

/**
 * 渲染指定部件呈现的图形内容
 * @param[in] w		部件
 * @param[in] paint 	进行绘制时所需的上下文
 * @return		返回实际渲染的部件的数量
 */
LCUI_API size_t Widget_Render( LCUI_Widget w, LCUI_PaintContext paint );

LCUI_API void LCUIWidget_InitRenderer( void );

LCUI_API void LCUIWidget_FreeRenderer( void );

LCUI_END_HEADER

#endif
