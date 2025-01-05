/*
 * lib/ui/src/ui_widget_observer.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

bool ui_widget_has_observer(ui_widget_t *widget,
			    ui_mutation_record_type_t type);

int ui_widget_add_mutation_record(ui_widget_t *widget,
				  ui_mutation_record_t *record);
