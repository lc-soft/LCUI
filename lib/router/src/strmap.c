/*
 * lib/router/src/strmap.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "private.h"

struct strmap {
        /** dict_t<char*, strmap_item_t*> */
        dict_t *dict;

        dict_type_t dict_type;

        /** list_t<strmap_item_t*> */
        list_t list;
};

static void strmap_on_list_item_destroy(void *data)
{
        strmap_item_t *item = data;

        free(item->key);
        free(item->value);
        free(item);
}

static void strmap_on_dict_item_destroy(void *privdata, void *data)
{
        strmap_t *map = privdata;
        strmap_item_t *item = data;

        list_unlink(&map->list, &item->node);
        strmap_on_list_item_destroy(item);
}

strmap_t *strmap_create(void)
{
        strmap_t *map;

        map = malloc(sizeof(strmap_t));
        if (map == NULL) {
                return NULL;
        }
        dict_init_string_key_type(&map->dict_type);
        map->dict_type.val_destructor = strmap_on_dict_item_destroy;
        map->dict = dict_create(&map->dict_type, map);
        list_create(&map->list);
        return map;
}

void strmap_destroy(strmap_t *map)
{
        dict_destroy(map->dict);
        list_destroy_without_node(&map->list, strmap_on_list_item_destroy);
        free(map);
}

void strmap_delete(strmap_t *map, const char *key)
{
        dict_delete(map->dict, key);
}

int strmap_set(strmap_t *map, const char *key, const char *value)
{
        strmap_item_t *item;

        item = malloc(sizeof(strmap_item_t));
        if (item == NULL) {
                return -1;
        }
        item->key = strdup(key);
        item->value = strdup(value);
        item->node.data = item;
        list_append_node(&map->list, &item->node);
        if (dict_add(map->dict, (void *)item->key, item) == 0) {
                return 0;
        }
        dict_delete(map->dict, (void *)item->key);
        return dict_add(map->dict, (void *)item->key, item);
}

const char *strmap_get(strmap_t *map, const char *key)
{
        strmap_item_t *item = dict_fetch_value(map->dict, key);
        return item ? item->value : NULL;
}

size_t strmap_extend(strmap_t *target, strmap_t *other)
{
        size_t count = 0;
        list_node_t *node;
        strmap_item_t *item;

        if (!other) {
                return count;
        }
        for (list_each(node, &other->list)) {
                item = node->data;
                strmap_set(target, item->key, item->value);
                ++count;
        }
        return count;
}

strmap_t *strmap_duplicate(strmap_t *target)
{
        strmap_t *dict;

        dict = strmap_create();
        if (target) {
                strmap_extend(dict, target);
        }
        return dict;
}

bool strmap_includes(strmap_t *a, strmap_t *b)
{
        strmap_item_t *item;
        strmap_iterator_t *iter;
        const char *value;

        iter = strmap_get_iterator(b);
        while ((item = strmap_next(iter))) {
                value = strmap_get(a, item->key);
                if (!value || strcmp(value, item->value) != 0) {
                        strmap_destroy_iterator(iter);
                        return false;
                }
        }
        strmap_destroy_iterator(iter);
        return true;
}

bool strmap_equal(strmap_t *a, strmap_t *b)
{
        if (dict_size(a->dict) != dict_size(b->dict)) {
                return false;
        }
        return strmap_includes(a, b);
}

strmap_iterator_t *strmap_get_iterator(strmap_t *map)
{
        strmap_iterator_t *iter;

        iter = malloc(sizeof(strmap_iterator_t));
        if (iter == NULL) {
                return NULL;
        }
        iter->index = 0;
        iter->item = NULL;
        iter->next_item = list_get(&map->list, 0);
        return iter;
}

void strmap_destroy_iterator(strmap_iterator_t *iter)
{
        free(iter);
}

strmap_item_t *strmap_next(strmap_iterator_t *iter)
{
        if (iter->item) {
                iter->index++;
        }
        iter->item = iter->next_item;
        if (iter->item && iter->item->node.next) {
                iter->next_item = iter->item->node.next->data;
        } else {
                iter->next_item = NULL;
        }
        return iter->item;
}