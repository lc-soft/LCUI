/*
 * lib/ui/src/ui_widget_id.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <thread.h>
#include <ui/base.h>
#include "ui_widget_id.h"

static dict_t* ui_widget_id_dict;

int ui_widget_destroy_id(ui_widget_t* w)
{
	list_t *list;
	list_node_t *node;

	if (!w->id) {
		return -1;
	}
	list = dict_fetch_value(ui_widget_id_dict, w->id);
	if (!list) {
		return -2;
	}
	for (list_each(node, list)) {
		if (node->data == w) {
			free(w->id);
			w->id = NULL;
			list_unlink(list, node);
			free(node);
			return 0;
		}
	}
	return -3;
}

int ui_widget_set_id(ui_widget_t* w, const char *idstr)
{
	list_t *list;

	ui_widget_destroy_id(w);
	if (!idstr) {
		return -1;
	}
	w->id = strdup2(idstr);
	if (!w->id) {
		goto error_exit;
	}
	list = dict_fetch_value(ui_widget_id_dict, w->id);
	if (!list) {
		list = malloc(sizeof(list_t));
		if (!list) {
			goto error_exit;
		}
		list_create(list);
		if (dict_add(ui_widget_id_dict, w->id, list) != 0) {
			free(list);
			goto error_exit;
		}
	}
	if (!list_append(list, w)) {
		goto error_exit;
	}
	return 0;

error_exit:
	if (w->id) {
		free(w->id);
		w->id = NULL;
	}
	return -2;
}

ui_widget_t* ui_get_widget(const char *id)
{
	list_t *list;
	ui_widget_t* w = NULL;

	if (!id) {
		return NULL;
	}
	list = dict_fetch_value(ui_widget_id_dict, id);
	if (list) {
		w = list_get(list, 0);
	}
	return w;
}


static void ui_widget_id_dict_val_destructor(void *privdata, void *data)
{
	list_t *list = data;
	list_destroy(list, NULL);
	free(list);
}

void ui_init_widget_id(void)
{
	static dict_type_t type;
	dict_init_string_copy_key_type(&type);
	type.val_destructor = ui_widget_id_dict_val_destructor;
	ui_widget_id_dict = dict_create(&type, NULL);
}

void ui_destroy_widget_id(void)
{
	dict_destroy(ui_widget_id_dict);
	ui_widget_id_dict = NULL;
}
