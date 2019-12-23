/*
 * widget_task.h -- LCUI widget task module.
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

#ifndef LCUI_WIDGET_TASK_H
#define LCUI_WIDGET_TASK_H

LCUI_BEGIN_HEADER

/** 更新当前任务状态，确保部件的任务能够被处理到 */
LCUI_API void Widget_UpdateTaskStatus(LCUI_Widget widget);

/** 添加任务 */
LCUI_API void Widget_AddTask(LCUI_Widget widget, int task_type);

/** 处理部件中当前积累的任务 */
LCUI_API size_t Widget_Update(LCUI_Widget w);

LCUI_API void Widget_UpdateWithProfile(LCUI_Widget w,
				       LCUI_WidgetTasksProfile profile);

/** 为子级部件添加任务 */
LCUI_API void Widget_AddTaskForChildren(LCUI_Widget widget, int task);

/** 初始化 LCUI 部件任务处理功能 */
LCUI_API void LCUIWidget_InitTasks(void);

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
LCUI_API void LCUIWidget_FreeTasks(void);

/** 处理一次当前积累的部件任务 */
LCUI_API size_t LCUIWidget_Update(void);

LCUI_API void LCUIWidget_UpdateWithProfile(LCUI_WidgetTasksProfile profile);

/** 刷新所有部件的样式 */
LCUI_API void LCUIWidget_RefreshStyle(void);

LCUI_END_HEADER

#endif
