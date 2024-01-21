/*
 * lib/ui/src/ui_root.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui.h>
#include "ui_root.h"

static ui_widget_t *ui_root_widget;

ui_widget_t *ui_root(void)
{
	return ui_root_widget;
}

int ui_root_append(ui_widget_t *w)
{
	return ui_widget_append(ui_root_widget, w);
}

void ui_init_root(void)
{
	ui_root_widget = ui_create_widget("root");
	ui_widget_set_title(ui_root_widget, L"LCUI Display");
}

void ui_destroy_root(void)
{
	ui_widget_destroy(ui_root_widget);
}
