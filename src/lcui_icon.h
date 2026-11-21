/*
 * src/lcui_icon.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_SRC_LCUI_ICON_H
#define LCUI_SRC_LCUI_ICON_H

#include <stdbool.h>
#include <stddef.h>
#include <pandagl.h>
#include <ptk/types.h>

#define LCUI_ICON_SIZE_COUNT 7

typedef struct lcui_icon {
        pd_canvas_t canvas[LCUI_ICON_SIZE_COUNT];
        ptk_icon_image_t image[LCUI_ICON_SIZE_COUNT];
        int count;
} lcui_icon_t;

bool lcui_icon_create_from_data(lcui_icon_t *icon, const void *data,
                                size_t size);
bool lcui_icon_create_from_file(lcui_icon_t *icon, const char *file);
void lcui_icon_destroy(lcui_icon_t *icon);

const ptk_icon_image_t *lcui_icon_default_images(int *count);
bool lcui_icon_set_default_from_data(const void *data, size_t size);
bool lcui_icon_set_default_from_file(const char *file);
void lcui_icon_reset_default(void);
void lcui_icon_set_default_enabled(bool enable);
bool lcui_icon_default_enabled(void);
void lcui_icon_destroy_default(void);

#endif
