/*
 * lib/ui/src/ui_metrics.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui/metrics.h>

ui_metrics_t ui_metrics = { .scale = 1.0f,
                            .density = 1.0f,
                            .scaled_density = 1.0f,
                            .dpi = 96.0f };

static float ui_compute_density_by_level(ui_density_Level_t level)
{
        switch (level) {
        case UI_DENSITY_LEVEL_SMALL:
                return 0.75f;
        case UI_DENSITY_LEVEL_LARGE:
                return 1.25f;
        case UI_DENSITY_LEVEL_BIG:
                return 1.5f;
        case UI_DENSITY_LEVEL_NORMAL:
        default:
                break;
        }
        return 1.0f;
}

void ui_set_density(float density)
{
        ui_metrics.density = density;
}

void ui_set_scaled_density(float density)
{
        ui_metrics.scaled_density = density;
}

void ui_set_density_level(ui_density_Level_t level)
{
        ui_metrics.density = ui_compute_density_by_level(level);
}

void ui_set_scaled_density_level(ui_density_Level_t level)
{
        ui_metrics.scaled_density = ui_compute_density_by_level(level);
}

void ui_set_scale(float scale)
{
        if (scale < 0.5f) {
                scale = 0.5f;
        } else if (scale > 5.0f) {
                scale = 5.0f;
        }
        ui_metrics.scale = scale;
}
