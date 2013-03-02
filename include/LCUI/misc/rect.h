/* ***************************************************************************
 * rect.h -- Rectangle area handling
 * 
 * Copyright (C) 2013 by
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
 * rect.h -- 矩形区域处理
 *
 * 版权所有 (C) 2013 归属于
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
#ifndef __LCUI_MISC_RECT_H__
#define __LCUI_MISC_RECT_H__

LCUI_BEGIN_HEADER

/* 初始化矩形区域的数据 */
LCUI_EXPORT(void)
Rect_Init( LCUI_Rect *rect );

/* 
 * 功能：检测两个矩形是否成十字架式叠加 
 * 返回值：
 *  1 a竖，b横
 * -1 a衡，b竖
 *  0 不叠加
 **/
LCUI_EXPORT(int)
LCUIRect_IsCrossOverlay( LCUI_Rect a, LCUI_Rect b );

/* 
 * 功能：获取需裁剪的区域
 * 说明：指定容器尺寸和容器中的区域位置及尺寸，即可得到该区域中需要进行裁剪区域
 *  */
LCUI_EXPORT(int)
LCUIRect_GetCutArea( LCUI_Size container, LCUI_Rect rect, LCUI_Rect *cut );

/* 
 * 功能：获取指定区域在容器中的有效显示区域 
 * 说明：指定容器的区域大小，再指定容器中的区域位置及大小，就能得到该容器实际能显示
 * 出的该区域范围。
 * */
LCUI_EXPORT(LCUI_Rect)
LCUIRect_ValidArea( LCUI_Size container, LCUI_Rect rect );

/*
 * 功能：检测两个矩形中，A矩形是否包含B矩形
 * 返回值：两不矩形属于包含关系返回1，否则返回0。
 * */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_IncludeRect( LCUI_Rect a, LCUI_Rect b );

/*
 * 功能：将有重叠部分的两个矩形，进行分割，并得到分割后的矩形
 * 说明：主要用于局部区域刷新里，添加的需刷新的区域有可能会与已添加的区域重叠，为避免
 * 重复刷新同一块区域，需要在添加时对矩形进行分割，得到完全重叠和不重叠的矩形。
 * 参数说明：
 * old ： 已存在的矩形区域
 * new ： 将要添加的矩形区域
 * rq  ： 指向矩形的队列的指针
 * 注意！传递参数时，请勿颠倒old和new位置。
 **/
LCUI_EXPORT(int)
LCUIRect_Cut(	LCUI_Rect	old_rect,
		LCUI_Rect	new_rect, 
		LCUI_Queue	*rects_buff );

/* 获取两矩形重叠部分的矩形 */
LCUI_EXPORT(int)
LCUIRect_GetOverlay( LCUI_Rect a, LCUI_Rect b, LCUI_Rect *out );

/* 将数值转换成LCUI_Rect型结构体 */
LCUI_EXPORT(LCUI_Rect)
Rect( int x, int y, int width, int height );

/* 检测一个点是否被矩形包含 */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_IncludePoint( LCUI_Pos pos, LCUI_Rect rect );

/* 检测两个矩形是否重叠 */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_Overlay( LCUI_Rect a, LCUI_Rect b );

/* 判断两个矩形是否相等 */
LCUI_EXPORT(LCUI_BOOL)
LCUIRect_Equal( LCUI_Rect a, LCUI_Rect b );

/* 判断矩形是否有效 */
LCUI_EXPORT(int)
LCUIRect_IsValid( LCUI_Rect r );

LCUI_END_HEADER

#endif
