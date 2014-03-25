/* ***************************************************************************
 * rect.h -- Rectangle area handling
 * 
 * Copyright (C) 2012-2014 by
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
 * 版权所有 (C) 2012-2014 归属于
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

/*----------------------------- RectQueue ------------------------------*/

typedef struct LCUI_RectQueueRec {
	LCUI_Queue	queue[2];	/* 两个队列 */
	unsigned int	number:1;	/* 正使用的队列的下标，只有0和1 */
} LCUI_RectQueue;

/* 将矩形数据追加至队列 */
LCUI_API int RectQueue_Add( LCUI_Queue *queue, LCUI_Rect rect );

/* 切换队列 */
LCUI_API void
DoubleRectQueue_Switch( LCUI_RectQueue *queue );

/* 初始化储存矩形数据的队列 */
LCUI_API void
DoubleRectQueue_Init( LCUI_RectQueue *queue );

/* 销毁储存矩形数据的队列 */
LCUI_API void
DoubleRectQueue_Destroy( LCUI_RectQueue *queue );

/* 添加矩形区域至可用的队列 */
LCUI_API int
DoubleRectQueue_AddToValid( LCUI_RectQueue *queue, LCUI_Rect rect );

/* 添加矩形区域至当前占用的队列 */
LCUI_API int
DoubleRectQueue_AddToCurrent( LCUI_RectQueue *queue, LCUI_Rect rect );

/* 从可用的队列中取出一个矩形区域 */
LCUI_API LCUI_BOOL
DoubleRectQueue_GetFromValid( LCUI_RectQueue *queue, LCUI_Rect *rect_buff );

/* 从当前占用的队列中取出一个矩形区域 */
LCUI_API LCUI_BOOL
DoubleRectQueue_GetFromCurrent( LCUI_RectQueue *queue, LCUI_Rect *rect_buff );

/*--------------------------- End RectQueue ----------------------------*/

LCUI_END_HEADER

#endif
