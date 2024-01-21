/*
 * lib/ui-router/src/route.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "private.h"
#include <ui_router/route.h>
#include <ui_router/location.h>

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L266

router_route_t *router_route_create(const router_route_record_t *record,
				    const router_location_t *location)
{
	router_route_t *route;

	route = malloc(sizeof(router_route_t));
	if (location->name) {
		route->name = strdup(location->name);
	} else if (record && record->name) {
		route->name = strdup(record->name);
	} else {
		route->name = NULL;
	}
	route->path = strdup(location->path ? location->path : "/");
	route->hash = strdup(location->hash ? location->hash : "");
	route->query = strmap_duplicate(location->query);
	route->params = strmap_duplicate(location->params);
	route->full_path = router_location_stringify(location);
	list_create(&route->matched);
	while (record) {
		list_insert(&route->matched, 0, (void *)record);
		record = record->parent;
	}
	return route;
}

void router_route_destroy(router_route_t *route)
{
	router_mem_free(route->name);
	router_mem_free(route->path);
	router_mem_free(route->full_path);
	router_mem_free(route->hash);
	if (route->params) {
		strmap_destroy(route->params);
	}
	if (route->query) {
		strmap_destroy(route->query);
	}
	list_destroy(&route->matched, NULL);
	free(route);
}

const router_route_record_t *router_route_get_matched_record(
    const router_route_t *route, size_t index)
{
	return list_get(&route->matched, index);
}

const char *router_route_get_full_path(const router_route_t *route)
{
	return route->full_path;
}

const char *router_route_get_path(const router_route_t *route)
{
	return route->path;
}

const char *router_route_get_hash(const router_route_t *route)
{
	return route->hash;
}

const char *router_route_get_param(const router_route_t *route, const char *key)
{
	if (!route->params) {
		return NULL;
	}
	return strmap_get(route->params, key);
}

const char *router_route_get_query(const router_route_t *route, const char *key)
{
	if (!route->query) {
		return NULL;
	}
	return strmap_get(route->query, key);
}
