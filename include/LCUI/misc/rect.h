/* ***************************************************************************
 * rect.h -- Rectangle area handling
 * 
 * Copyright (C) 2012-2015 by
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
 * 版权所有 (C) 2012-2015 归属于
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
	
typedef LinkedList LCUI_DirtyRectList;

/* 将数值转换成LCUI_Rect型结构体 */
LCUI_API LCUI_Rect Rect( int x, int y, int w, int h );

/** 根据容器尺寸，获取指定区域中需要裁剪的区域 */
LCUI_API void LCUIRect_GetCutArea( LCUI_Size box_size, LCUI_Rect rect,
							LCUI_Rect *cut );

/** 将矩形区域范围调整在容器有效范围内 */
LCUI_API void LCUIRect_ValidateArea( LCUI_Rect *rect, LCUI_Size box_size );

/** 检测矩形是否遮盖另一个矩形 */
LCUI_API LCUI_BOOL LCUIRect_IsCoverRect( LCUI_Rect rect1, LCUI_Rect rect2 );


/** 
 * 获取两个矩形中的重叠矩形
 * @param[in] a		矩形A
 * @param[in] b		矩形B
 * @param[out] out	矩形A和B重叠处的矩形
 * @returns 如果两个矩形重叠，则返回TRUE，否则返回FALSE
 */
LCUI_API LCUI_BOOL LCUIRect_GetOverlayRect( const LCUI_Rect *a, 
					    const LCUI_Rect *b, 
					    LCUI_Rect *out );

/** 
 * 根据重叠矩形 rect1，将矩形 rect2 分割成四个矩形
 * 分割方法如下：
 * ┏━━┳━━━━━━┓
 * ┃    ┃     3      ┃
 * ┃ 0  ┣━━━┳━━┃
 * ┃    ┃rect1 ┃    ┃
 * ┃    ┃      ┃ 2  ┃
 * ┣━━┻━━━┫    ┃
 * ┃     1      ┃    ┃
 * ┗━━━━━━┻━━┛
 *
 * rect2 必须被 rect1 完全包含
 */
LCUI_API void LCUIRect_CutFourRect( LCUI_Rect *rect1, LCUI_Rect *rect2, 
				    LCUI_Rect rects[4] );

/** 初始化脏矩形记录 */
LCUI_API void DirtyRectList_Init( LCUI_DirtyRectList *list );

/** 销毁脏矩形记录 */
LCUI_API void DirtyRectList_Destroy( LCUI_DirtyRectList *list );

/** 添加一个脏矩形记录 */
LCUI_API int DirtyRectList_Add( LCUI_DirtyRectList *list, LCUI_Rect *rect );

/** 删除脏矩形 */
LCUI_API int DirtyRectList_Delete( LCUI_DirtyRectList *list, LCUI_Rect *rect );

LCUI_END_HEADER

#endif
