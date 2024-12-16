/*
 * lib/ui/src/ui_mutation_observer.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

typedef struct ui_mutation_connection {
	ui_widget_t *widget;
	ui_mutation_observer_t *observer;
	ui_mutation_observer_init_t options;
	list_node_t node;
} ui_mutation_connection_t;
