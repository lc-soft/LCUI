/*
 * event.h -- Event processing module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
LCUI_API LCUI_EventTrigger EventTrigger(void);

/** 销毁一个事件触发器 */
LCUI_API void EventTrigger_Destroy(LCUI_EventTrigger trigger);

/**
 * 绑定一个事件处理器
 * @param[in] event_id     事件标识号
 * @param[in] func         事件处理函数
 * @param[in] data         事件处理函数的附加参数
 * @param[in] destroy_data 参数的销毁函数
 * @returns 返回处理器ID
 */
LCUI_API int EventTrigger_Bind(LCUI_EventTrigger trigger, int event_id,
			       LCUI_EventFunc func, void *data,
			       void(*destroy_data)(void*));

/**
 * 解除绑定事件处理器
 * @param[in] trigger  事件触发器
 * @param[in] event_id 事件标识号
 * @param[in] func     事件处理函数
 * @returns 解绑成功返回 0，失败则返回 -1
 */
LCUI_API int EventTrigger_Unbind(LCUI_EventTrigger trigger, int event_id,
				 LCUI_EventFunc func);

/**
 * 根据事件处理器的标识号来解除事件绑定
 * @param[in] trigger    事件触发器
 * @param[in] handler_id 事件处理器的标识号
 * @returns 解绑成功返回 0，失败则返回 -1
 */
LCUI_API int EventTrigger_Unbind2(LCUI_EventTrigger trigger, int handler_id);

/**
 * 根据自定义的判断方法来解除事件绑定
 * @param[in] trigger       事件触发器
 * @param[in] event_id      事件标识号
 * @param[in] compare_func  比较函数，一致返回 1，不一致则返回 0
 * @param[in] key           用于比较的关键数据
 * @returns 解绑成功返回 0，失败则返回 -1
 */
LCUI_API int EventTrigger_Unbind3(LCUI_EventTrigger trigger, int event_id,
				  int(*compare_func)(void*, void*), void *key);

/**
 * 触发事件
 * @param[in] trigger    事件触发器
 * @param[in] event_id   事件标识号
 * @param[in] arg        与事件相关的数据
 * @returns 返回已调用的事件处理器的数量
 */
LCUI_API int EventTrigger_Trigger(LCUI_EventTrigger trigger, int event_id, void *arg);

LCUI_END_HEADER

#endif
