#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

static ui_metrics_t ui_metrics;

float ui_compute(float value, LCUI_StyleType type)
{
	switch (type) {
	case LCUI_STYPE_PX:
		break;
	case LCUI_STYPE_DIP:
		value = value * ui_metrics.density;
		break;
	case LCUI_STYPE_SP:
		value = value * ui_metrics.scaled_density;
		break;
	case LCUI_STYPE_PT:
		value = value * ui_metrics.dpi / 72.0f;
		break;
	default:
		value = 0;
		break;
	}
	return value;
}

float ui_get_scale(void)
{
	return ui_metrics.scale;
}

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
	scale = max(0.5f, scale);
	scale = min(5.0f, scale);
	ui_metrics.scale = scale;
}

void ui_init_metrics(void)
{
	ui_metrics.scale = 1.0f;
	ui_set_dpi(96.0f);
}

const ui_metrics_t *ui_get_metrics(void)
{
	return &ui_metrics;
}
