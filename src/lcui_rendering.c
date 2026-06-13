/*
 * src/lcui_rendering.c: -- rendering settings.
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.

 * *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI,
 * distributed under the MIT License found in the
 * LICENSE.TXT file in the
 * root directory of this source tree.
 */

#include <ui_server.h>
#include <LCUI/app.h>
#include <LCUI/settings.h>
#include "lcui_settings.h"
#include "setting_field.h"

static lcui_rendering_settings_t g_rendering = {
        .fps_cap = LCUI_DEFAULT_FPS_CAP,
        .parallel_threads = LCUI_DEFAULT_PARALLEL_THREADS,
        .paint_flashing = false
};

static const setting_field_def_t g_rendering_fields[] = {
        DEFINE_SETTING_INT_FIELD(
            lcui_rendering_settings_t, fps_cap, LCUI_DEFAULT_FPS_CAP,
            LCUI_RENDERING_FPS_CAP_MIN, LCUI_RENDERING_FPS_CAP_MAX),
        DEFINE_SETTING_INT_FIELD(lcui_rendering_settings_t, parallel_threads,
                                 LCUI_DEFAULT_PARALLEL_THREADS,
                                 LCUI_RENDERING_PARALLEL_THREADS_MIN,
                                 LCUI_RENDERING_PARALLEL_THREADS_MAX),
        DEFINE_SETTING_BOOL_FIELD(lcui_rendering_settings_t, paint_flashing,
                                  false)
};

static void lcui_rendering_apply(void)
{
        ui_server_set_threads(g_rendering.parallel_threads);
        ui_server_set_paint_flashing_enabled(g_rendering.paint_flashing);
        lcui_set_fps_cap((unsigned)g_rendering.fps_cap);
}

static void lcui_rendering_deserialize(ini_doc_t *doc, bool *dirty, void *data)
{
        (void)data;
        setting_fields_deserialize(
            doc, "rendering", g_rendering_fields,
            sizeof(g_rendering_fields) / sizeof(g_rendering_fields[0]),
            &g_rendering, dirty);
        lcui_rendering_apply();
}

static void lcui_rendering_serialize(ini_doc_t *doc, void *data)
{
        (void)data;
        setting_fields_serialize(
            doc, "rendering", g_rendering_fields,
            sizeof(g_rendering_fields) / sizeof(g_rendering_fields[0]),
            &g_rendering);
}

bool lcui_get_rendering(lcui_rendering_settings_t *out)
{
        if (!lcui_settings_is_loaded()) {
                return false;
        }
        *out = g_rendering;
        return true;
}

bool lcui_set_rendering(const lcui_rendering_settings_t *in)
{
        if (!lcui_settings_is_loaded()) {
                return false;
        }
        if (in->fps_cap < LCUI_RENDERING_FPS_CAP_MIN ||
            in->fps_cap > LCUI_RENDERING_FPS_CAP_MAX) {
                return false;
        }
        if (in->parallel_threads < LCUI_RENDERING_PARALLEL_THREADS_MIN ||
            in->parallel_threads > LCUI_RENDERING_PARALLEL_THREADS_MAX) {
                return false;
        }
        g_rendering = *in;
        lcui_rendering_apply();
        lcui_settings_mark_dirty();
        return true;
}

void lcui_rendering_init(void)
{
        lcui_settings_on_deserialize(lcui_rendering_deserialize, NULL);
        lcui_settings_on_serialize(lcui_rendering_serialize, NULL);
}

void lcui_rendering_destroy(void)
{
        lcui_settings_off_deserialize(lcui_rendering_deserialize, NULL);
        lcui_settings_off_serialize(lcui_rendering_serialize, NULL);
        g_rendering.fps_cap = LCUI_DEFAULT_FPS_CAP;
        g_rendering.parallel_threads = LCUI_DEFAULT_PARALLEL_THREADS;
        g_rendering.paint_flashing = false;
}
