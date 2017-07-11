/* ***************************************************************************
 * event.h -- event processing module
 * 
 * Copyright (C) 2016-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2016-2017 归属于 刘超 <lc-soft@live.cn>
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
#ifndef LCUI_UTIL_EVENT_H
#define LCUI_UTIL_EVENT_H

LCUI_BEGIN_HEADER

/** 事件数据结构 */
typedef struct LCUI_EventRec_ {
	int type;			/**< 事件类型 */
	void *data;			/**< 事件附加数据 */
} LCUI_EventRec, *LCUI_Event;

typedef void(*LCUI_EventFunc)(LCUI_Event, void*);

typedef struct LCUI_EventTriggerRec_ {
	int handler_base_id;		/**< 事件处理器ID */
	RBTree events;			/**< 事件绑定记录 */
	RBTree handlers;		/**< 事件处理器记录 */
} LCUI_EventTriggerRec, *LCUI_EventTrigger;

/** 构建一个事件触发器 */
LCUI_API LCUI_EventTrigger EventTrigger( void );

/** 销毁一个事件触发器 */
LCUI_API void EventTrigger_Destroy( LCUI_EventTrigger trigger );

/**
 * 绑定一个事件处理器
 * @param[in] event_id     事件标识号
 * @param[in] func         事件处理函数
 * @param[in] data         事件处理函数的附加参数
 * @param[in] destroy_data 参数的销毁函数
 * @returns 返回处理器ID
 */
LCUI_API int EventTrigger_Bind( LCUI_EventTrigger trigger, int event_id,
				LCUI_EventFunc func, void *data,
				void( *destroy_data )(void*) );

/** 
 * 解除绑定事件处理器
 * @param[in] trigger  事件触发器
 * @param[in] event_id 事件标识号
 * @param[in] func     事件处理函数
 * @returns 解绑成功返回 0，失败则返回 -1
 */
LCUI_API int EventTrigger_Unbind( LCUI_EventTrigger trigger, int event_id,
				  LCUI_EventFunc func );

/** 
* 根据事件处理器的标识号来解除事件绑定
* @param[in] trigger    事件触发器
* @param[in] handler_id 事件处理器的标识号
* @returns 解绑成功返回 0，失败则返回 -1
*/
LCUI_API int EventTrigger_Unbind2( LCUI_EventTrigger trigger, int handler_id );

/** 
* 根据自定义的判断方法来解除事件绑定
* @param[in] trigger       事件触发器
* @param[in] event_id      事件标识号
* @param[in] compare_func  比较函数，一致返回 1，不一致则返回 0
* @param[in] key           用于比较的关键数据
* @returns 解绑成功返回 0，失败则返回 -1
*/
LCUI_API int EventTrigger_Unbind3( LCUI_EventTrigger trigger, int event_id,
				   int (*compare_func)(void*, void*), void *key );

/** 
* 触发事件
* @param[in] trigger    事件触发器
* @param[in] event_id   事件标识号
* @param[in] arg        与事件相关的数据
* @returns 返回已调用的事件处理器的数量
*/
LCUI_API int EventTrigger_Trigger( LCUI_EventTrigger trigger, int event_id, void *arg );

LCUI_END_HEADER

#endif
