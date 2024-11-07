/*
 * lib/ui/src/ui_widget.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

void ui_widget_add_state(ui_widget_t *w, ui_widget_state_t state);

void ui_widget_set_content_width(ui_widget_t *w, float width);

void ui_widget_set_content_height(ui_widget_t *w, float height);

void ui_widget_update_box_position(ui_widget_t *w);

void ui_widget_update_box_size(ui_widget_t *w);
void ui_widget_update_box_width(ui_widget_t *w);
void ui_widget_update_box_height(ui_widget_t *w);

void ui_widget_get_sizehint(ui_widget_t *w, ui_sizehint_t *hint);
