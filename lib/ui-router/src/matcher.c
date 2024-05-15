/*
 * lib/ui-router/src/matcher.c
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
#include <ui_router/route_record.h>
#include <ui_router/matcher.h>
#include <ui_router/location.h>

router_matcher_t *router_matcher_create(void)
{
        router_matcher_t *matcher;
        static dict_type_t type;

        dict_init_string_key_type(&type);
        matcher = malloc(sizeof(router_matcher_t));
        matcher->name_map = dict_create(&type, NULL);
        matcher->path_map = dict_create(&type, NULL);
        list_create(&matcher->path_list);
        return matcher;
}

static void router_matcher_on_destroy_record(void *data)
{
        router_route_record_destroy(data);
}

void router_matcher_destroy(router_matcher_t *matcher)
{
        dict_destroy(matcher->name_map);
        dict_destroy(matcher->path_map);
        list_destroy_without_node(&matcher->path_list,
                                  router_matcher_on_destroy_record);
        matcher->name_map = NULL;
        matcher->path_map = NULL;
        free(matcher);
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L1282

router_route_record_t *router_matcher_add_route_record(
    router_matcher_t *matcher, const router_config_t *config,
    const router_route_record_t *parent)
{
        size_t i, len;
        router_route_record_t *record;
        const char *base_path = parent ? parent->path : NULL;
        list_node_t *node;

        record = router_route_record_create();
        record->path = router_path_resolve(config->path, base_path, true);
        strmap_extend(record->components, config->components);
        if (config->name) {
                if (dict_fetch_value(matcher->name_map, config->name)) {
                        logger_error(
                            "[router] duplicate named routes definition: "
                            "{ name: \"%s\", path: \"%s\" }\n",
                            config->name, config->path);
                        router_route_record_destroy(record);
                        return NULL;
                }
                record->name = strdup(config->name);
                dict_add(matcher->name_map, record->name, record);
        }
        if (!dict_fetch_value(matcher->path_map, record->path)) {
                list_append_node(&matcher->path_list, &record->node);
                dict_add(matcher->path_map, record->path, record);
        }
        if (parent) {
                list_unlink(&matcher->path_list, &record->node);
                list_link(&matcher->path_list, parent->node.prev,
                          &record->node);
        }
        len = matcher->path_list.length;
        node = matcher->path_list.head.next;
        // ensure wildcard routes are always at the end
        for (i = 0; node && i < len; ++i, node = node->next) {
                if (strcmp(((router_route_t *)node->data)->path, "*") == 0) {
                        list_unlink(&matcher->path_list, node);
                        list_append_node(&matcher->path_list, node);
                        i--;
                        len--;
                }
        }
        record->parent = parent;
        return record;
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L1599

bool router_matcher_match_route(router_route_record_t *record, const char *path,
                                strmap_t *params)
{
        char **nodes;
        char **record_nodes;
        char *path_match = NULL;
        size_t i, j;
        size_t nodes_count;
        size_t record_nodes_count;
        size_t path_match_len = 0;
        size_t path_match_i = 0;
        bool matched = true;
        bool match_all = false;

        nodes_count = strsplit(path, "/", &nodes);
        record_nodes_count = strsplit(record->path, "/", &record_nodes);
        // record->path: "/example/:type/:name/info"
        // path: "/exmaple/food/orange/info"
        for (i = 0, j = 0; i < nodes_count && j < record_nodes_count; ++i) {
                if (!match_all && strcmp(record_nodes[j], "*") == 0) {
                        // record->path: /files/*
                        // path: /files/path/to/file
                        // path_match: path/to/file
                        if (j + 1 != record_nodes_count) {
                                logger_warning(
                                    "[router] %s",
                                    "the asterisk should be at the end\n");
                        }
                        match_all = true;
                }
                if (match_all) {
                        if (path_match_len > 0) {
                                path_match_len += 1;
                        }
                        path_match_len += strlen(nodes[i]);
                        path_match = realloc(
                            path_match, sizeof(char) * (path_match_len + 1));
                        if (path_match_i > 0) {
                                strcpy(path_match + path_match_i, "/");
                                ++path_match_i;
                        }
                        strcpy(path_match + path_match_i, nodes[i]);
                        path_match_i = path_match_len;
                        continue;
                }
                if (strcmp(record_nodes[j], nodes[i]) == 0) {
                } else if (record_nodes[j][0] == ':') {
                        strmap_set(params, record_nodes[j] + 1, nodes[i]);
                } else if (strcmp(record_nodes[j], nodes[i]) != 0) {
                        matched = false;
                        break;
                }
                ++j;
        }
        if (path_match) {
                matched = matched && i == nodes_count;
                if (matched) {
                        strmap_set(params, "pathMatch", path_match);
                }
                free(path_match);
        } else {
                matched = matched && nodes_count == record_nodes_count;
        }
        for (i = 0; i < nodes_count; ++i) {
                free(nodes[i]);
        }
        for (i = 0; i < record_nodes_count; ++i) {
                free(record_nodes[i]);
        }
        free(nodes);
        free(record_nodes);
        return matched;
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L1457

router_route_t *router_matcher_match_by_name(router_matcher_t *matcher,
                                             router_location_t *location,
                                             const router_route_t *current_route)
{
        char *key;
        const char *value;
        router_route_record_t *record;
        router_linkedlist_t param_names;
        router_linkedlist_node_t *node;

        record = dict_fetch_value(matcher->name_map, location->name);
        if (!record) {
                logger_warning("[router] route with name '%s' does not exist",
                               location->name);
                return router_route_create(NULL, location);
        }
        if (!location->params) {
                location->params = strmap_create();
        }
        if (current_route) {
                list_create(&param_names);
                router_path_parse_keys(record->path, &param_names);
                for (list_each(node, &param_names)) {
                        key = node->data;
                        value = strmap_get(current_route->params, key);
                        if (value && !strmap_get(location->params, key)) {
                                strmap_set(location->params, key, value);
                        }
                }
                list_destroy(&param_names, free);
        }
        location->path =
            router_path_fill_params(record->path, location->params);
        return router_route_create(record, location);
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L1481

router_route_t *router_matcher_match_by_path(router_matcher_t *matcher,
                                             router_location_t *location)
{
        router_route_record_t *record;
        router_linkedlist_node_t *node;

        for (list_each(node, &matcher->path_list)) {
                record = node->data;
                if (router_matcher_match_route(record, location->path,
                                               location->params)) {
                        return router_route_create(record, location);
                }
        }
        return router_route_create(NULL, location);
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L1449

router_route_t *router_matcher_match(router_matcher_t *matcher,
                                     const router_location_t *raw_location,
                                     const router_route_t *current_route)
{
        router_route_t *route;
        router_location_t *location;

        location =
            router_location_normalize(raw_location, current_route, false);
        if (location->name) {
                route = router_matcher_match_by_name(matcher, location,
                                                     current_route);
        } else if (location->path) {
                route = router_matcher_match_by_path(matcher, location);
        } else {
                route = router_route_create(NULL, location);
        }
        router_location_destroy(location);
        return route;
}
