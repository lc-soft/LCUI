/*
 * lib/ui/include/ui/image.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_IMAGE_H
#define LIB_UI_INCLUDE_UI_IMAGE_H

#include "common.h"
#include "types.h"

#define ui_image_on_event ui_image_add_event_listener
#define ui_image_off_event ui_image_remove_event_listener

typedef struct ui_image_t ui_image_t;
typedef void (*ui_image_event_handler_t)(ui_image_t *, void *);

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC ui_image_t *ui_get_image(const char *path);

/**
 * 创建 UIImage 对象，从指定路径加载数据
 * 注意：UIImage 对象的数据不会被立刻加载
 **/
LIBUI_PUBLIC ui_image_t *ui_image_create(const char *path);

/**
 * 获取已加载的图像数据
 */
LIBUI_PUBLIC pd_canvas_t *ui_image_get_data(ui_image_t *image);

LIBUI_PUBLIC const char *ui_image_get_path(ui_image_t *image);

LIBUI_PUBLIC bool ui_image_is_loaded(ui_image_t *image);

LIBUI_PUBLIC void ui_image_destroy(ui_image_t *image);
LIBUI_PUBLIC int ui_image_add_event_listener(ui_image_t *image,
                                             ui_image_event_handler_t handler,
                                             void *data);
LIBUI_PUBLIC int ui_image_remove_event_listener(
    ui_image_t *image, ui_image_event_handler_t handler, void *data);

/**
 * 为所有 UIImage 对象加载数据
 * 注意：该函数应在 UI 线程外调用
 **/
LIBUI_PUBLIC void ui_load_images(void);

/**
 * 处理 UIImage 的事件
 * 注意：该函数应在 UI 线程内调用
 */
LIBUI_PUBLIC void ui_process_image_events(void);

/**
 * 清理未使用的 UIImage 对象，释放内存资源
 * 注意：该函数应在 UI 线程内调用
 */
LIBUI_PUBLIC void ui_clear_images(void);

LIBUI_END_DECLS

#endif
