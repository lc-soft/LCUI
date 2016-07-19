
/* ***************************************************************************
 * widget_paint.h -- LCUI widget paint module.
 * 
 * Copyright (C) 2013-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2016 归属于 刘超 <lc-soft@live.cn>
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
 * 标记部件内的一个区域为无效的，以使其重绘
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
LCUI_API void Widget_InvalidateArea( LCUI_Widget w, LCUI_Rect *r, int box_type );

/** 
 * 将部件一个无效区域推送到根级部件中
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 * @returns 推送成功返回 TRUE，如果在推送过程中区域处于部件不可见区域内，则推送失败，返回FALSE
 */
LCUI_API LCUI_BOOL Widget_PushInvalidArea( LCUI_Widget widget,
					   LCUI_Rect *r, int box_type );
/** 
 * 获取部件中的无效区域
 * @param[in] widget	目标部件
 * @area[out] area	无效区域
 */
LCUI_API int Widget_GetInvalidArea( LCUI_Widget widget, LCUI_Rect *area );

/**  
 * 标记部件内的一个区域为有效的
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
LCUI_API void Widget_ValidateArea( LCUI_Widget w, LCUI_Rect *r, int box_type );

/**
 * 处理部件及其子级部件中的脏矩形，并合并至一个记录中
 * @param[in]	w	目标部件
 * @param[out]	rlist	合并后的脏矩形记录
 */
LCUI_API int Widget_ProcInvalidArea( LCUI_Widget w, LinkedList *rlist );

/** 
 * 将部件中的矩形区域转换成指定范围框内有效的矩形区域
 * @param[in]	w		目标部件
 * @param[in]	in_rect		相对于部件呈现框的矩形区域
 * @param[out]	out_rect	转换后的区域
 * @param[in]	box_type	转换后的区域所处的范围框
 */
LCUI_API int Widget_ConvertArea( LCUI_Widget w, LCUI_Rect *in_rect,
				LCUI_Rect *out_rect, int box_type );

/**
 * 渲染指定部件呈现的图形内容
 * @param[in] w		部件
 * @param[in] paint 	进行绘制时所需的上下文
 */
LCUI_API void Widget_Render( LCUI_Widget w, LCUI_PaintContext paint );

LCUI_END_HEADER

#endif
