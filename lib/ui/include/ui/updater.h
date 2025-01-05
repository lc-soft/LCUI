/*
 * lib/ui/include/ui/updater.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_UI_INCLUDE_UI_UPDATER_H
#define LIB_UI_INCLUDE_UI_UPDATER_H

#include "common.h"
#include "types.h"

#include <ui/metrics.h>

LIBUI_BEGIN_DECLS

typedef struct ui_updater {
        list_node_t node;
        ui_metrics_t metrics;
        bool refresh_all;
} ui_updater_t;

ui_updater_t *ui_updater_create(void);
void ui_updater_destroy(ui_updater_t *updater);
void ui_updater_update(ui_updater_t *updater, ui_widget_t *root);
size_t ui_updater_update_widget(ui_updater_t *updater, ui_widget_t *w);

LIBUI_END_DECLS

#endif
