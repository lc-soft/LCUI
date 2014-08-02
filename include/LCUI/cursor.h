/* ***************************************************************************
 * LCUI_Cursor.h -- mouse cursor operation set.
 *
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * LCUI_Cursor.h -- 鼠标游标的操作集
 *
 * 版权所有 (C) 2012-2013 归属于
 * 刘超
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

#ifndef __LCUI_CURSOR_H__
#define __LCUI_CURSOR_H__

LCUI_BEGIN_HEADER
	
/* 初始化游标数据 */
void LCUIModule_Cursor_Init( void );

void LCUIModule_Cursor_End( void );

/* 获取鼠标游标的区域范围 */
LCUI_API void LCUICursor_GetRect( LCUI_Rect *rect );

/* 刷新鼠标游标在屏幕上显示的图形 */
LCUI_API void LCUICursor_Refresh( void );

/* 检测鼠标游标是否可见 */
LCUI_API LCUI_BOOL LCUICursor_IsVisible( void );

/* 显示鼠标游标 */
LCUI_API void LCUICursor_Show( void );

/* 隐藏鼠标游标 */
LCUI_API void LCUICursor_Hide( void );

/* 更新鼠标指针的位置 */
LCUI_API void LCUICursor_UpdatePos( void );

/* 设定游标的位置 */
LCUI_API void LCUICursor_SetPos( LCUI_Pos pos );

/** 设置游标的图形 */
LCUI_API int LCUICursor_SetGraph( LCUI_Graph *graph );

/* 获取鼠标指针当前的坐标 */
LCUI_API void LCUICursor_GetPos( LCUI_Pos *pos );

/* 获取鼠标指针将要更新的坐标 */
LCUI_API void LCUICursor_GetNewPos( LCUI_Pos *pos );

/* 检测鼠标游标是否覆盖在矩形区域上 */
LCUI_BOOL LCUICursor_IsCoverRect( LCUI_Rect rect );

/* 将当前鼠标游标的图像叠加至目标图像指定位置 */
int LCUICursor_MixGraph( LCUI_Graph *buff, LCUI_Pos pos );

LCUI_END_HEADER

#endif
