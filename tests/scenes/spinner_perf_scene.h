/*
 * tests/scenes/spinner_perf_scene.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_TESTS_SCENES_SPINNER_PERF_SCENE_H
#define LCUI_TESTS_SCENES_SPINNER_PERF_SCENE_H

#include <ui.h>

/* A rendering stress scene: many small spinners plus a couple of large
 * text-rotating spinners inside a scrollarea, so the number of visible
 * self-drawn widgets can be varied by scrolling. Shared by the perf test and
 * the interactive preview. */
#define SPINNER_PERF_COUNT 200
#define SPINNER_PERF_BIG_COUNT 2

/* The outer box is deliberately larger than the 800x600 viewport so the
 * vertical scrollbar and the frame do not eat into the visible area. */
#define SPINNER_PERF_BOX_W 848.0f
#define SPINNER_PERF_BOX_H 632.0f
#define SPINNER_PERF_VIEW_W 800.0f
#define SPINNER_PERF_VIEW_H 600.0f

typedef struct spinner_perf_scene {
        ui_widget_t *box;        /* outer container appended to the root */
        ui_widget_t *scrollarea; /* holds the scrollable content */
        ui_widget_t *content;    /* scrollarea-content */
        ui_widget_t *status;     /* text overlay for preview readouts */
        ui_widget_t *spinners[SPINNER_PERF_COUNT];
        ui_widget_t *big[SPINNER_PERF_BIG_COUNT];
        int count;
        double angle;
} spinner_perf_scene_t;

/* Build the scene into the root and size the root to the outer box. */
void spinner_perf_scene_build(spinner_perf_scene_t *scene);

/* Advance every spinner by `degrees` so a frame re-renders all of them. The
 * perf test drives frames with this instead of a running frame clock. */
void spinner_perf_scene_advance(spinner_perf_scene_t *scene, double degrees);

#endif /* LCUI_TESTS_SCENES_SPINNER_PERF_SCENE_H */
