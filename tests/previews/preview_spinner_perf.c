/*
 * tests/previews/preview_spinner_perf.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <LCUI.h>
#include "preview_window.h"
#include <ptk/main.h>
#include "../scenes/spinner_perf_scene.h"

static spinner_perf_scene_t scene;
static uint64_t last_ts;
static float smoothed_ms;

/* Sample the engine's FPS and the frame interval and show them in the status
 * overlay, so scrolling to change the number of visible spinners makes the
 * rendering cost visible. */
static void on_frame(uint64_t timestamp, void *data)
{
        char buf[64];
        float dt = last_ts ? (float)(timestamp - last_ts) : 0.0f;

        last_ts = timestamp;
        smoothed_ms = smoothed_ms * 0.85f + dt * 0.15f;
        snprintf(buf, sizeof(buf), "FPS %u    %.1f ms/frame", lcui_get_fps(),
                 smoothed_ms);
        ui_text_set_content(scene.status, buf);
        lcui_request_frame(on_frame, data);
}

int main(int argc, char **argv)
{
        (void)argc;
        (void)argv;
        lcui_init();
        ui_widget_set_title(ui_root(), L"spinner perf preview");
        spinner_perf_scene_build(&scene);
        lcui_request_frame(on_frame, NULL);
        preview_show_default_window();
        return lcui_main();
}
