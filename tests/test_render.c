﻿/*
 * tests/test_render.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <LCUI.h>
#include <platform/main.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define BLOCK_COUNT 60

static struct TestStatus {
	size_t color_index;
	unsigned int fps;
	ui_widget_t* box;
} self;

void UpdateWidgetStyle(ui_widget_t* w, void* arg)
{
	size_t index;
	pd_color_t color;

	index = w->index % BLOCK_COUNT + w->index / BLOCK_COUNT;
	index = ((index + self.color_index) * 256 / BLOCK_COUNT) % 512;
	color.red = 255;
	color.green = (unsigned char)(index > 255 ? 511 - index : index);
	color.blue = 0;
	color.alpha = 255;
	ui_widget_set_style_color_value(w, css_prop_background_color,
					color.value);
}

void UpdateFrame(void* arg)
{
	ui_widget_each(arg, UpdateWidgetStyle, NULL);
}

void UpdateRenderStatus(void* arg)
{
	char str[32];

	sprintf(str, "[size=24px]FPS: %d[/size]", self.fps);
	ui_text_set_content(arg, str);
	self.fps = 0;
}

void InitModal(void)
{
	ui_widget_t* dimmer;
	ui_widget_t* dialog;

	dimmer = ui_create_widget(NULL);
	dialog = ui_create_widget(NULL);
	ui_widget_resize(dialog, 400, 400);
	ui_widget_set_style_string(dialog, "margin", "100px auto");
	ui_widget_set_style_string(dialog, "border-radius", "6px");
	ui_widget_set_style_string(dialog, "opacity", "0.9");
	ui_widget_set_style_string(dialog, "box-shadow",
				   "0 4px 8px rgba(0,0,0,0.4)");
	ui_widget_set_style_string(dialog, "background-color", "#fff");

	ui_widget_set_style_string(dimmer, "top", "0");
	ui_widget_set_style_string(dimmer, "left", "0");
	ui_widget_set_style_string(dimmer, "width", "100%");
	ui_widget_set_style_string(dimmer, "height", "100%");
	ui_widget_set_style_string(dimmer, "position", "absolute");
	ui_widget_set_style_string(dimmer, "background-color",
				   "rgba(0,0,0,0.5)");

	ui_widget_append(dimmer, dialog);
	ui_widget_append(ui_root(), dimmer);
}

void InitBackground(void)
{
	size_t i;
	size_t c;
	size_t n = BLOCK_COUNT;
	ui_widget_t* w;
	ui_widget_t* root;
	pd_color_t color;
	ui_widget_rules_t rules = { 0 };
	const float width = SCREEN_WIDTH * 1.0f / n;
	const float height = SCREEN_HEIGHT * 1.0f / n;

	color.red = 255;
	color.green = 0;
	color.blue = 0;
	color.alpha = 255;
	root = ui_root();
	self.box = ui_create_widget(NULL);
	for (i = 0; i < n * n; ++i) {
		if (i % n == 0) {
			++self.color_index;
		}
		w = ui_create_widget(NULL);
		c = ((i % n + i / n) * 256 / n) % 512;
		color.green = (unsigned char)(c > 255 ? 511 - c : c);
		ui_widget_resize(w, width, height);
		ui_widget_set_style_string(w, "display", "inline-block");
		ui_widget_set_style_color_value(w, css_prop_background_color,
						color.value);
		ui_widget_append(self.box, w);
	}
	rules.cache_children_style = true;
	rules.ignore_classes_change = true;
	rules.ignore_status_change = true;
	rules.max_update_children_count = -1;
	rules.max_render_children_count = 0;
	ui_widget_generate_hash(self.box);
	ui_widget_set_rules(self.box, &rules);
	ui_widget_append(root, self.box);
}

void InitRenderStatus(void)
{
	ui_widget_t* root;
	ui_widget_t* status;

	root = ui_root();
	status = ui_create_widget("text");
	ui_widget_set_style_string(status, "top", "10px");
	ui_widget_set_style_string(status, "right", "10px");
	ui_widget_set_style_string(status, "position", "absolute");
	ui_widget_set_style_string(status, "background", "#000");
	ui_widget_set_style_string(status, "color", "#fff");
	ui_widget_set_style_string(status, "padding", "10px 15px");
	ui_widget_append(root, status);
	UpdateRenderStatus(status);
	lcui_set_interval(1000, UpdateRenderStatus, status);
}

int main(int argc, char* argv[])
{
	size_t i;
	int64_t t;

	logger_set_level(LOGGER_LEVEL_WARNING);
	lcui_init();
	ui_widget_resize(ui_root(), SCREEN_WIDTH, SCREEN_HEIGHT);
	InitBackground();
	InitModal();
	InitRenderStatus();
	printf("running rendering performance test\n");
	t = get_time_ms();
	self.color_index = 0;
	for (i = 0; i < 600; ++i) {
		UpdateFrame(self.box);
		lcui_process_timers();
		lcui_update_ui();
		lcui_render_ui();
		app_present();
		++self.fps;
		++self.color_index;
	}
	t = get_time_delta(t);
	logger_warning(
	    "rendered %zu frames in %.2lfs, rendering speed is %.2lf fps\n", i,
	    t / 1000.f, i * 1000.f / t);
	return 0;
}
