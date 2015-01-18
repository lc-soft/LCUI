
/* ***************************************************************************
 * widget_paint.h -- LCUI widget paint module.
 * 
 * Copyright (C) 2013-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2015 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __WIDGET_PAINT_H__
#define __WIDGET_PAINT_H__

LCUI_BEGIN_HEADER

/** 初始化GUI部件绘制器 */
void LCUIWidgetPainter_Init(void);

/** 销毁GUI部件绘制器 */
void LCUIWidgetPainter_Destroy(void);

/** 
 * 标记部件内的一个区域为无效的，以使其重绘
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
LCUI_API int Widget_InvalidateArea( LCUI_Widget w, LCUI_Rect *r, int box_type );

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
 * 将部件的区域推送至屏幕
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
LCUI_API int Widget_PushAreaToScreen( LCUI_Widget w, LCUI_Rect *r, int box_type );

/** 
 * 执行重绘部件前的一些任务
 * @param[in] widget 需要重绘的部件
 * @param[out] area 需要进行重绘的区域
 * @returns 正常返回TRUE，没有无效区域则返回FALSE
 */
LCUI_API LCUI_BOOL Widget_BeginPaint( LCUI_Widget widget, LCUI_Rect *area );

/** 执行重绘部件后的一些任务 */
LCUI_API void Widget_EndPaint( LCUI_Widget widget, LCUI_Rect *area );

/** 更新各个部件的无效区域中的内容 */
int LCUIWidget_ProcInvalidArea(void);

LCUI_END_HEADER

#endif
