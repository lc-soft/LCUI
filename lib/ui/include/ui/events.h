/*
 * lib/ui/include/ui/events.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_EVENTS_H
#define LIB_UI_INCLUDE_UI_EVENTS_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

/** 设置阻止部件及其子级部件的事件 */
LIBUI_INLINE void ui_widget_block_event(ui_widget_t *w, bool block)
{
        w->event_blocked = block;
}

/** 触发事件，让事件处理器在主循环中调用 */
LIBUI_PUBLIC int ui_widget_post_event(ui_widget_t *w, const ui_event_t *e,
                                      void *arg,
                                      ui_event_arg_destructor_t destroy_arg);

/** 触发事件，直接调用事件处理器 */
LIBUI_PUBLIC int ui_widget_emit_event(ui_widget_t *w, ui_event_t e, void *arg);

/** 自动分配一个可用的事件标识号 */
LIBUI_PUBLIC int ui_alloc_event_id(void);

/** 设置与事件标识号对应的名称 */
LIBUI_PUBLIC int ui_set_event_id(int event_id, const char *event_name);

/** 获取与事件标识号对应的名称 */
LIBUI_PUBLIC const char *ui_get_event_name(int event_id);

/** 获取与事件名称对应的标识号 */
LIBUI_PUBLIC int ui_get_event_id(const char *event_name);

LIBUI_PUBLIC void ui_event_init(ui_event_t *e, const char *name);

LIBUI_PUBLIC void ui_event_destroy(ui_event_t *e);

/**
 * 添加部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_id 事件标识号
 * @param[in] handler 事件处理函数
 * @param[in] data 事件处理函数的附加数据
 * @param[in] destroy_arg 数据的销毁函数
 * @return 成功则返回 0，失败返回负数
 */
LIBUI_PUBLIC ui_event_listener_t *ui_widget_add_event_listener(
    ui_widget_t *widget, int event_id, ui_event_handler_t handler, void *data,
    void (*destroy_arg)(void *));

/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 与事件绑定的函数
 * @return 成功则返回 0，失败返回负数
 */
LIBUI_PUBLIC int ui_widget_remove_event_listener(ui_widget_t *w, int event_id,
                                                 ui_event_handler_t handler,
                                                 void *data);

/**
 * 添加部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 事件处理函数
 * @param[in] data 事件处理函数的附加数据
 * @return 返回已移除的事件监听器数量
 */
LIBUI_PUBLIC ui_event_listener_t *ui_widget_on(ui_widget_t *widget,
                                               const char *event_name,
                                               ui_event_handler_t handler,
                                               void *data);

/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 与事件绑定的函数
 * @return 成功则返回 0，失败返回负数
 */
LIBUI_PUBLIC int ui_widget_off(ui_widget_t *widget, const char *event_name,
                               ui_event_handler_t handler, void *data);

LIBUI_PUBLIC int ui_emit_event(ui_event_t e, void *arg);

LIBUI_PUBLIC int ui_post_event(const ui_event_t *e, void *arg,
                               ui_event_arg_destructor_t destroy_arg);

LIBUI_PUBLIC ui_event_listener_t *ui_add_event_listener(
    ui_widget_t *widget, int event_id, ui_event_handler_t handler, void *arg,
    ui_event_arg_destructor_t destroy_arg);

LIBUI_PUBLIC int ui_remove_event_listener(ui_widget_t *w, int event_id,
                                          ui_event_handler_t handler,
                                          void *data);

LIBUI_PUBLIC ui_event_listener_t *ui_on_event(const char *event_name,
                                              ui_event_handler_t handler,
                                              void *data);
LIBUI_PUBLIC int ui_off_event(const char *event_name,
                              ui_event_handler_t handler, void *data);

/** 清除事件对象，通常在部件销毁时调用该函数，以避免部件销毁后还有事件发送给它
 */
LIBUI_PUBLIC void ui_clear_event_target(ui_widget_t *widget);

/** get current focused widget */
LIBUI_PUBLIC ui_widget_t *ui_get_focus(void);

/** 将一个部件设置为焦点 */
LIBUI_PUBLIC int ui_set_focus(ui_widget_t *widget);

/** 停止部件的事件传播 */
LIBUI_PUBLIC int ui_widget_stop_event_propagation(ui_widget_t *widget);

/** 为部件设置鼠标捕获，设置后将捕获全局范围内的鼠标事件 */
LIBUI_PUBLIC void ui_widget_set_mouse_capture(ui_widget_t *w);

/** 为部件解除鼠标捕获 */
LIBUI_PUBLIC void ui_widget_release_mouse_capture(ui_widget_t *w);

/**
 * 为部件设置触点捕获，设置后将捕获全局范围内的触点事件
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则捕获全部触点
 * @returns 设置成功返回 0，如果其它部件已经捕获该触点则返回 -1
 */
LIBUI_PUBLIC int ui_widget_set_touch_capture(ui_widget_t *w, int point_id);

/**
 * 为部件解除触点捕获
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则解除全部触点的捕获
 */
LIBUI_PUBLIC int ui_widget_release_touch_capture(ui_widget_t *w, int point_id);

LIBUI_END_DECLS

#endif
