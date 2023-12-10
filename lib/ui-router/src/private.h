/*
 * lib/ui-router/src/private.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef UI_ROUTER_PRIVATE_H
#define UI_ROUTER_PRIVATE_H

#include <string.h>
#include <stdlib.h>
#include <ui_router/types.h>
#include <ui_router/utils.h>
#include <ui_router/strmap.h>

#ifdef _WIN32
#pragma warning(disable: 4996)
#endif

#define router_mem_free(ptr)       \
	do {                       \
		if (ptr) {         \
			free(ptr); \
		}                  \
		ptr = NULL;        \
	} while (0)

struct router_location_t {
	char *name;
	char *path;
	char *hash;
	strmap_t *params;
	strmap_t *query;
	bool normalized;
};

struct router_route_t {
	char *name;
	char *path;
	char *full_path;
	char *hash;
	strmap_t *query;
	strmap_t *params;
	router_linkedlist_t matched;
};

struct router_route_record_t {
	char *name;
	char *path;
	const router_route_record_t *parent;
	strmap_t *components;
	router_linkedlist_node_t node;
};

struct router_history_t {
	int index;
	router_route_t *current;
	router_linkedlist_t routes;
	router_linkedlist_t watchers;
};

struct router_config_t {
	char *name;
	char *path;
	strmap_t *components;
};

struct router_matcher_t {
	dict_t *name_map;
	dict_t *path_map;
	router_linkedlist_t path_list;
};

struct router_watcher_t {
	void *data;
	router_callback_t callback;
	router_linkedlist_node_t node;
};

struct router_resolved_t {
	router_route_t *route;
	router_location_t *location;
};

struct router_t {
	char *name;
	char *link_active_class;
	char *link_exact_active_class;
	router_matcher_t *matcher;
	router_history_t *history;
};

#endif
