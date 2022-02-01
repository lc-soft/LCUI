﻿#include <LCUI/font.h>
#include "../include/ui.h"
#include "internal.h"

void ui_init(void)
{
	LCUI_InitFontLibrary();
	ui_init_metrics();
	ui_init_widget_id();
	ui_init_widget_prototype();
	ui_init_updater();
	ui_init_events();
	ui_init_css();
	ui_init_image_loader();
	ui_init_root();
}

void ui_destroy(void)
{
	LCUI_FreeFontLibrary();
	ui_trash_clear();
	ui_destroy_root();
	ui_destroy_events();
	ui_destroy_image_loader();
	ui_destroy_widget_id();
	ui_destroy_widget_prototype();
	ui_destroy_css();
}
