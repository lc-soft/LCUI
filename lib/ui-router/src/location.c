/*
 * lib/ui-router/src/location.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "private.h"
#include <ui_router/location.h>

router_location_t *router_location_create(const char *name, const char *path)
{
        router_location_t *location;

        location = malloc(sizeof(router_location_t));
        location->name = name ? strdup(name) : NULL;
        location->path = path ? strdup(path) : NULL;
        location->hash = NULL;
        location->query = NULL;
        location->params = NULL;
        location->normalized = FALSE;
        return location;
}

void router_location_destroy(router_location_t *location)
{
        router_mem_free(location->name);
        router_mem_free(location->hash);
        router_mem_free(location->path);
        if (location->params) {
                strmap_destroy(location->params);
        }
        if (location->query) {
                strmap_destroy(location->query);
        }
        location->query = NULL;
        location->params = NULL;
        free(location);
}

router_location_t *router_location_duplicate(const router_location_t *target)
{
        router_location_t *location;

        location = router_location_create(target->name, target->path);
        location->hash = target->hash ? strdup(target->hash) : NULL;
        location->query = strmap_duplicate(target->query);
        location->params = strmap_duplicate(target->params);
        location->normalized = target->normalized;
        return location;
}

void router_location_set_name(router_location_t *location, const char *name)
{
        router_mem_free(location->name);
        if (name) {
                location->name = strdup(name);
        }
}

static router_location_t *router_location_from_path(
    const router_location_t *raw, const router_route_t *current, bool append)
{
        size_t i;
        char *path = NULL;
        char *hash = NULL;
        char *query_str = NULL;
        const char *base_path = current ? current->path : "/";
        router_location_t *location;
        strmap_t *query;

        if (raw->path) {
                path = strdup(raw->path);
                for (i = 0; path[i]; ++i) {
                        if (path[i] == '#') {
                                hash = strdup(path + i);
                                path[i] = 0;
                                break;
                        }
                }
                for (i = 0; path[i]; ++i) {
                        if (path[i] == '?') {
                                query_str = strdup(path + i + 1);
                                path[i] = 0;
                                break;
                        }
                }
        }
        location = router_location_create(NULL, NULL);
        if (path) {
                location->path = router_path_resolve(path, base_path, append);
                free(path);
        } else {
                location->path = strdup(base_path);
        }
        query = router_parse_query(query_str);
        strmap_extend(query, raw->query);
        location->query = query;
        location->hash = hash;
        location->normalized = TRUE;
        router_mem_free(query_str);
        return location;
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L936

router_location_t *router_location_normalize(const router_location_t *raw,
                                             const router_route_t *current,
                                             bool append)
{
        strmap_t *params;
        router_location_t *location;
        router_route_record_t *record;

        if (raw->normalized || raw->name) {
                return router_location_duplicate(raw);
        }
        if (!raw->path && raw->params && current) {
                location = router_location_duplicate(raw);
                location->normalized = TRUE;
                params = strmap_create();
                strmap_extend(params, current->params);
                strmap_extend(params, raw->params);
                if (current->name) {
                        router_location_set_name(location, current->name);
                        strmap_destroy(location->params);
                        location->params = params;
                } else if (current->matched.length > 0) {
                        record = list_get(&current->matched,
                                          current->matched.length - 1);
                        if (location->path) {
                                free(location->path);
                        }
                        location->path =
                            router_path_fill_params(record->path, params);
                        strmap_destroy(params);
                } else {
                        strmap_destroy(params);
                        logger_warning("%s",
                                       "relative params navigation requires a "
                                       "current route.");
                }
                return location;
        }
        return router_location_from_path(raw, current, append);
}

int router_location_set_param(router_location_t *location, const char *key,
                              const char *value)
{
        if (!location->params) {
                location->params = strmap_create();
        }
        return strmap_set(location->params, key, value);
}

const char *router_location_get_param(const router_location_t *location,
                                      const char *key)
{
        if (!location->params) {
                return NULL;
        }
        return strmap_get(location->params, key);
}

int router_location_set_query(router_location_t *location, const char *key,
                              const char *value)
{
        if (!location->query) {
                location->query = strmap_create();
        }
        return strmap_set(location->query, key, value);
}

const char *router_location_get_query(const router_location_t *location,
                                      const char *key)
{
        if (!location->query) {
                return NULL;
        }
        return strmap_get(location->query, key);
}

const char *router_location_get_path(const router_location_t *location)
{
        return location->path;
}

#define STR_REALLOC(STR, LEN) STR = realloc(STR, sizeof(char) * (LEN + 1))

char *router_location_stringify(const router_location_t *location)
{
        char *path;
        const char *str;
        size_t i, pairs;
        size_t path_len;
        strmap_iterator_t *iter;
        strmap_item_t *item;

        if (location->path) {
                path_len = strlen(location->path);
                path = malloc(sizeof(char) * (path_len + 1));
                strcpy(path, location->path);
        } else {
                path_len = 0;
                path = malloc(sizeof(char) * (path_len + 1));
                strcpy(path, "");
        }
        if (location->query) {
                pairs = 0;
                iter = strmap_get_iterator(location->query);
                while ((item = strmap_next(iter))) {
                        // ?key1=value1&key2=value2
                        i = path_len;
                        str = item->key;
                        path_len += strlen(str) + 2;
                        STR_REALLOC(path, path_len);
                        path[i++] = pairs > 0 ? '&' : '?';
                        path[i] = 0;
                        strcpy(path + i, str);
                        i = path_len - 1;
                        path[i++] = '=';
                        path[i] = 0;
                        str = item->value;
                        path_len += strlen(str);
                        STR_REALLOC(path, path_len);
                        strcpy(path + i, str);
                        pairs++;
                }
                strmap_destroy_iterator(iter);
        }
        if (location->hash) {
                i = path_len;
                path_len += strlen(location->hash);
                STR_REALLOC(path, path_len);
                strcpy(path + i, location->hash);
        }
        return path;
}
