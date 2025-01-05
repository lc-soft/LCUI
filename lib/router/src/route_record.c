/*
 * lib/router/src/route_record.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "private.h"
#include <router/route_record.h>

router_route_record_t *router_route_record_create(void)
{
	router_route_record_t *record;

	record = malloc(sizeof(router_route_record_t));
	record->name = NULL;
	record->path = NULL;
	record->components = strmap_create();
	record->node.data = record;
	record->node.prev = NULL;
	record->node.next = NULL;
	record->parent = NULL;
	return record;
}

void router_route_record_destroy(router_route_record_t *record)
{
	if (record->name) {
		free(record->name);
	}
	if (record->path) {
		free(record->path);
	}
	record->name = NULL;
	record->path = NULL;
	strmap_destroy(record->components);
	free(record);
}

void router_route_record_set_path(router_route_record_t *record,
				  const char *path)
{
	router_mem_free(record->path);
	record->path = strdup(path);
}

const char *router_route_record_get_component(
    const router_route_record_t *record, const char *key)
{
	if (!key) {
		key = "default";
	}
	return strmap_get(record->components, key);
}
