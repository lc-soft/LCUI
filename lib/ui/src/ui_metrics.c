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
#include "ui_metrics.h"

ui_metrics_t ui_metrics;

static float ui_compute_density_by_level(ui_density_Level_t level)
{
	float density = ui_metrics.dpi / 96.0f;
	switch (level) {
	case UI_DENSITY_LEVEL_SMALL: density *= 0.75f; break;
	case UI_DENSITY_LEVEL_LARGE: density *= 1.25f; break;
	case UI_DENSITY_LEVEL_BIG: density *= 1.5f; break;
	case UI_DENSITY_LEVEL_NORMAL:
	default: break;
	}
	return density;
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

void ui_set_dpi(float dpi)
{
	ui_metrics.dpi = dpi;
	ui_set_density_level(UI_DENSITY_LEVEL_NORMAL);
	ui_set_scaled_density_level(UI_DENSITY_LEVEL_NORMAL);
}

void ui_set_scale(float scale)
{
	scale = y_max(0.5f, scale);
	scale = y_min(5.0f, scale);
	ui_metrics.scale = scale;
}

void ui_init_metrics(void)
{
	ui_metrics.scale = 1.0f;
	ui_set_dpi(96.0f);
}
