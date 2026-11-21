/*
 * src/lcui_icon.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <yutil.h>
#include "lcui_icon.h"

static const int lcui_icon_sizes[LCUI_ICON_SIZE_COUNT] = { 16, 24,  32, 48,
                                                           64, 128, 256 };

static const unsigned char lcui_icon_16_png[] = {
#include "resources/lcui-icon-16.png.inc"
};

static const unsigned char lcui_icon_24_png[] = {
#include "resources/lcui-icon-24.png.inc"
};

static const unsigned char lcui_icon_32_png[] = {
#include "resources/lcui-icon-32.png.inc"
};

static const unsigned char lcui_icon_48_png[] = {
#include "resources/lcui-icon-48.png.inc"
};

static const unsigned char lcui_icon_64_png[] = {
#include "resources/lcui-icon-64.png.inc"
};

static const unsigned char lcui_icon_128_png[] = {
#include "resources/lcui-icon-128.png.inc"
};

static const unsigned char lcui_icon_256_png[] = {
#include "resources/lcui-icon-256.png.inc"
};

static const unsigned char *const lcui_icon_pngs[LCUI_ICON_SIZE_COUNT] = {
        lcui_icon_16_png, lcui_icon_24_png,  lcui_icon_32_png, lcui_icon_48_png,
        lcui_icon_64_png, lcui_icon_128_png, lcui_icon_256_png
};

static const size_t lcui_icon_png_sizes[LCUI_ICON_SIZE_COUNT] = {
        sizeof(lcui_icon_16_png), sizeof(lcui_icon_24_png),
        sizeof(lcui_icon_32_png), sizeof(lcui_icon_48_png),
        sizeof(lcui_icon_64_png), sizeof(lcui_icon_128_png),
        sizeof(lcui_icon_256_png)
};

static struct {
        lcui_icon_t builtin;
        lcui_icon_t *override;
        bool builtin_ready;
        bool builtin_available;
        bool enabled;
} lcui_icons = { .enabled = true };

static void lcui_icon_init(lcui_icon_t *icon)
{
        int i;

        memset(icon, 0, sizeof(*icon));
        for (i = 0; i < LCUI_ICON_SIZE_COUNT; ++i) {
                pd_canvas_init(&icon->canvas[i]);
        }
}

void lcui_icon_destroy(lcui_icon_t *icon)
{
        int i;

        if (!icon) {
                return;
        }
        for (i = 0; i < LCUI_ICON_SIZE_COUNT; ++i) {
                pd_canvas_destroy(&icon->canvas[i]);
        }
        memset(icon, 0, sizeof(*icon));
}

static bool lcui_icon_ensure_argb(pd_canvas_t *canvas)
{
        unsigned x, y;
        pd_canvas_t argb;

        if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
                return true;
        }
        pd_canvas_init(&argb);
        argb.color_type = PD_COLOR_TYPE_ARGB;
        if (pd_canvas_create(&argb, canvas->width, canvas->height) < 0) {
                return false;
        }
        for (y = 0; y < canvas->height; ++y) {
                for (x = 0; x < canvas->width; ++x) {
                        pd_canvas_set_pixel(&argb, x, y,
                                            pd_canvas_get_pixel(canvas, x, y));
                }
        }
        pd_canvas_destroy(canvas);
        *canvas = argb;
        return true;
}

static bool lcui_icon_generate(lcui_icon_t *icon, pd_canvas_t *source)
{
        int i;

        if (!lcui_icon_ensure_argb(source)) {
                return false;
        }
        for (i = 0; i < LCUI_ICON_SIZE_COUNT; ++i) {
                if (pd_canvas_zoom_bilinear(source, &icon->canvas[i], false,
                                            lcui_icon_sizes[i],
                                            lcui_icon_sizes[i]) != 0) {
                        return false;
                }
                icon->image[i].width = (int)icon->canvas[i].width;
                icon->image[i].height = (int)icon->canvas[i].height;
                icon->image[i].argb = (const uint32_t *)icon->canvas[i].argb;
        }
        icon->count = LCUI_ICON_SIZE_COUNT;
        return true;
}

static bool lcui_icon_read_from_data(pd_canvas_t *source, const void *data,
                                     size_t size)
{
        pd_error_t err;
        pd_image_reader_t *reader;

        pd_canvas_init(source);
        reader = pd_image_reader_create_from_memory(data, size);
        if (!reader) {
                return false;
        }
        err = pd_image_reader_read_header(reader);
        if (err == PD_OK) {
                err = pd_image_reader_read_data(reader, source);
        }
        pd_image_reader_destroy(reader);
        if (err != PD_OK) {
                pd_canvas_destroy(source);
                return false;
        }
        return true;
}

bool lcui_icon_create_from_data(lcui_icon_t *icon, const void *data,
                                size_t size)
{
        pd_canvas_t source;

        lcui_icon_init(icon);
        if (!lcui_icon_read_from_data(&source, data, size)) {
                return false;
        }
        if (!lcui_icon_generate(icon, &source)) {
                pd_canvas_destroy(&source);
                lcui_icon_destroy(icon);
                return false;
        }
        pd_canvas_destroy(&source);
        return true;
}

bool lcui_icon_create_from_file(lcui_icon_t *icon, const char *file)
{
        pd_canvas_t source;

        lcui_icon_init(icon);
        pd_canvas_init(&source);
        if (pd_read_image_from_file(file, &source) != PD_OK) {
                pd_canvas_destroy(&source);
                return false;
        }
        if (!lcui_icon_generate(icon, &source)) {
                pd_canvas_destroy(&source);
                lcui_icon_destroy(icon);
                return false;
        }
        pd_canvas_destroy(&source);
        return true;
}

static bool lcui_icon_load_builtin_sizes(lcui_icon_t *icon)
{
        int i;
        pd_canvas_t canvas;

        lcui_icon_init(icon);
        for (i = 0; i < LCUI_ICON_SIZE_COUNT; ++i) {
                if (!lcui_icon_read_from_data(&canvas, lcui_icon_pngs[i],
                                              lcui_icon_png_sizes[i]) ||
                    !lcui_icon_ensure_argb(&canvas)) {
                        pd_canvas_destroy(&canvas);
                        lcui_icon_destroy(icon);
                        return false;
                }
                icon->canvas[i] = canvas;
                icon->image[i].width = (int)canvas.width;
                icon->image[i].height = (int)canvas.height;
                icon->image[i].argb = (const uint32_t *)canvas.argb;
        }
        icon->count = LCUI_ICON_SIZE_COUNT;
        return true;
}

static bool lcui_icon_load_builtin(void)
{
        if (lcui_icons.builtin_ready) {
                return lcui_icons.builtin_available;
        }
        lcui_icons.builtin_ready = true;
        lcui_icons.builtin_available =
            lcui_icon_load_builtin_sizes(&lcui_icons.builtin);
        if (!lcui_icons.builtin_available) {
                logger_warning("[icon] failed to load pre-rendered icon sizes, "
                               "falling back to scaling\n");
                lcui_icons.builtin_available = lcui_icon_create_from_data(
                    &lcui_icons.builtin, lcui_icon_256_png,
                    sizeof(lcui_icon_256_png));
        }
        if (!lcui_icons.builtin_available) {
                logger_warning("[icon] failed to decode built-in icon\n");
        }
        return lcui_icons.builtin_available;
}

const ptk_icon_image_t *lcui_icon_default_images(int *count)
{
        if (!lcui_icons.enabled) {
                if (count) {
                        *count = 0;
                }
                return NULL;
        }
        if (lcui_icons.override && lcui_icons.override->count > 0) {
                if (count) {
                        *count = lcui_icons.override->count;
                }
                return lcui_icons.override->image;
        }
        if (!lcui_icon_load_builtin()) {
                if (count) {
                        *count = 0;
                }
                return NULL;
        }
        if (count) {
                *count = lcui_icons.builtin.count;
        }
        return lcui_icons.builtin.image;
}

bool lcui_icon_set_default_from_data(const void *data, size_t size)
{
        lcui_icon_t *icon;

        icon = calloc(1, sizeof(lcui_icon_t));
        if (!icon) {
                return false;
        }
        if (!lcui_icon_create_from_data(icon, data, size)) {
                free(icon);
                return false;
        }
        if (lcui_icons.override) {
                lcui_icon_destroy(lcui_icons.override);
                free(lcui_icons.override);
        }
        lcui_icons.override = icon;
        return true;
}

bool lcui_icon_set_default_from_file(const char *file)
{
        lcui_icon_t *icon;

        icon = calloc(1, sizeof(lcui_icon_t));
        if (!icon) {
                return false;
        }
        if (!lcui_icon_create_from_file(icon, file)) {
                free(icon);
                return false;
        }
        if (lcui_icons.override) {
                lcui_icon_destroy(lcui_icons.override);
                free(lcui_icons.override);
        }
        lcui_icons.override = icon;
        return true;
}

void lcui_icon_reset_default(void)
{
        if (!lcui_icons.override) {
                return;
        }
        lcui_icon_destroy(lcui_icons.override);
        free(lcui_icons.override);
        lcui_icons.override = NULL;
}

void lcui_icon_set_default_enabled(bool enable)
{
        lcui_icons.enabled = enable;
}

bool lcui_icon_default_enabled(void)
{
        return lcui_icons.enabled;
}

void lcui_icon_destroy_default(void)
{
        if (lcui_icons.builtin_ready) {
                lcui_icon_destroy(&lcui_icons.builtin);
                lcui_icons.builtin_ready = false;
                lcui_icons.builtin_available = false;
        }
        lcui_icon_reset_default();
        lcui_icons.enabled = true;
}
