/*
 * lib/ui/src/ui.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>
#include <ui/types.h>
#include "ui/base.h"
#include "ui_root.h"
#include "ui_events.h"
#include "ui_image.h"
#include "ui_widget_id.h"
#include "ui_widget_prototype.h"
#include "ui_css.h"
#include "ui_updater.h"

void ui_init(void)
{
	pd_font_library_init();
	ui_init_widget_id();
	ui_init_widget_prototype();
	ui_init_updater();
	ui_init_root();
	ui_init_events();
	ui_init_css();
	ui_init_image_loader();
}

void ui_destroy(void)
{
	pd_font_library_destroy();
	ui_clear_trash();
	ui_destroy_root();
	ui_destroy_events();
	ui_destroy_image_loader();
	ui_destroy_widget_id();
	ui_destroy_widget_prototype();
	ui_destroy_css();
	ui_destroy_updater();
}
