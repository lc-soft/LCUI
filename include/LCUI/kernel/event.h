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
	int id;				/**< 事件标识号 */
	const char *name;		/**< 事件名（只读） */
	void *data;			/**< 事件附加数据 */
	void (*destroy_data)(void*);	/**< 用于销毁事件附加数据的回调函数 */
} LCUI_Event;

typedef void(*EventCallBack)(LCUI_Event*, void*);
typedef void* LCUI_EventBox;

/** 创建一个事件容器实例 */
LCUI_API LCUI_EventBox LCUIEventBox_Create(void);

/** 销毁事件容器实例 */
LCUI_API void LCUIEventBox_Destroy( LCUI_EventBox box );

/** 获取指定名称的事件槽的id */
LCUI_API int LCUIEventBox_GetSlotId( LCUI_EventBox box, const char *name );

/** 绑定事件 */
LCUI_API int LCUIEventBox_Bind( LCUI_EventBox box, const char *event_name,
				EventCallBack func, void *func_data, 
				void (*destroy_data)(void*) );

/** 解除事件连接 */
LCUI_API int LCUIEventBox_Unbind( LCUI_EventBox box, int handler_id );

/** 直接将事件发送至事件处理器进行处理 */
LCUI_API int LCUIEventBox_Send( LCUI_EventBox box, const char *name, void *data );

/** 将事件投递给事件处理器，等待处理 */
int LCUIEventBox_Post( LCUI_EventBox box, const char *name,
		       void *data, void (*destroy_data)(void*) );

/** 从已触发的事件记录中取出一个事件信息 */
LCUI_API int LCUIEventBox_GetEvent( LCUI_EventBox box, LCUI_Event *ebuff );

LCUI_END_HEADER

#endif
