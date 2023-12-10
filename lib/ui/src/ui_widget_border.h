/*
 * lib/ui/src/ui_widget_border.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

void ui_widget_paint_border(ui_widget_t *w, pd_context_t *paint,
			    ui_widget_actual_style_t *style);
void ui_widget_crop_content(ui_widget_t *w, pd_context_t *paint,
			    ui_widget_actual_style_t *style);
