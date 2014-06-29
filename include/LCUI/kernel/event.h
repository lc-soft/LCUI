/* ***************************************************************************
 * event.h -- event processing module
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
 * event.h -- 事件处理模块
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
#ifndef __LCUI_KERNEL_EVENT_H__
#define __LCUI_KERNEL_EVENT_H__

LCUI_BEGIN_HEADER

/** 事件 */
typedef struct LCUI_EventRec_ {
	int id;			/**< 事件标识号 */
	const char *name;	/**< 事件名（只读） */
	void *data;		/**< 事件附加数据 */
} LCUI_Event;

typedef void(*EventCallBack)(LCUI_Event*, void*);
typedef void* LCUI_EventBox;

/** 初始化一个事件容器实例 */
void LCUIEventBox_Init( LCUI_EventBox box );

/** 销毁事件容器实例 */
void LCUIEventBox_Destroy( LCUI_EventBox box );

/** 连接事件 */
int LCUIEventBox_Conncet( LCUI_EventBox box, const char *name,
				EventCallBack func, void *data );

/** 解除事件连接 */
int LCUIEventBox_Disconnect( LCUI_EventBox box, int handler_id );

/** 直接将事件发送至事件处理器进行处理 */
int LCUIEventBox_Send( LCUI_EventBox box, const char *name, void *data );

/** 将事件投递给事件处理器，等待处理 */
int LCUIEventBox_Post( LCUI_EventBox box, const char *name, void *data );

/** 分派所有已触发的事件至事件处理器 */
void LCUIEventBox_Dispatch( LCUI_EventBox box );

LCUI_END_HEADER

#endif
