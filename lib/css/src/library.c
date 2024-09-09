/*
 * lib/css/src/library.c: -- CSS library operation module.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <css/value.h>
#include <css/style_value.h>
#include <css/style_decl.h>
#include <css/selector.h>
#include <css/properties.h>
#include <css/library.h>
#include "dump.h"

#define LEN(A) sizeof(A) / sizeof(*A)

/** dict_t<string, css_style_link_group_t*> */
typedef dict_t css_style_group_t;

/** 样式链接记录组 */
typedef struct css_style_link_group_t {
        dict_t *links;              /**< 样式链接表 */
        char *name;                 /**< 选择器名称 */
        css_selector_node_t *snode; /**< 选择器结点 */
} css_style_link_group_t;

/** 样式链接记录 */
typedef struct css_style_link_t {
        char *selector;                /**< 选择器 */
        css_style_link_group_t *group; /**< 所属组 */

        /**
         * 作用于当前选择器的样式
         * list_t<css_style_rule_t*>
         */
        list_t styles;

        /**
         * 父级节点
         * dict_t<string, css_style_link_t*>
         */
        dict_t *parents;
} css_style_link_t;

static struct css_library_module_t {
        /**
         * 样式组列表
         * list_t<css_style_group_t*>
         */
        list_t groups;

        /** 字符串池 */
        strpool_t *strpool;

        /**
         * 样式表缓存，以选择器的 hash 值索引
         * dict_t<css_selector_hash_t, css_style_decl_t*>
         */
        dict_t *cache;

} css_library;

static uint64_t ikey_dict_hash(const void *key)
{
        return (*(unsigned int *)key);
}

static int ikey_dict_key_compare(void *privdata, const void *key1,
                                 const void *key2)
{
        return *(unsigned int *)key1 == *(unsigned int *)key2;
}

static void ikey_dict_key_destructor(void *privdata, void *key)
{
        free(key);
}

static void *ikey_dict_key_dup(void *privdata, const void *key)
{
        unsigned int *newkey = malloc(sizeof(unsigned int));
        *newkey = *(unsigned int *)key;
        return newkey;
}

static void css_style_cache_destructor(void *privdata, void *val)
{
        css_style_decl_destroy(val);
}

bool css_selector_node_match(css_selector_node_t *sn1, css_selector_node_t *sn2)
{
        int i, j;
        if (sn2->id) {
                if (!sn1->id || strcmp(sn1->id, sn2->id) != 0) {
                        return false;
                }
        }
        if (sn2->type && strcmp(sn2->type, "*") != 0) {
                if (!sn1->type || strcmp(sn1->type, sn2->type) != 0) {
                        return false;
                }
        }
        if (sn2->classes) {
                if (!sn1->classes) {
                        return false;
                }
                for (i = 0; sn2->classes[i]; ++i) {
                        for (j = 0; sn1->classes[j]; ++j) {
                                if (strcmp(sn2->classes[i], sn1->classes[i]) ==
                                    0) {
                                        j = -1;
                                        break;
                                }
                        }
                        if (j != -1) {
                                return false;
                        }
                }
        }
        if (sn2->status) {
                if (!sn1->status) {
                        return false;
                }
                for (i = 0; sn2->status[i]; ++i) {
                        for (j = 0; sn1->status[j]; ++j) {
                                if (strcmp(sn2->status[i], sn1->status[i]) ==
                                    0) {
                                        j = -1;
                                        break;
                                }
                        }
                        if (j != -1) {
                                return false;
                        }
                }
        }
        return true;
}

static void css_style_rule_destroy(css_style_rule_t *node)
{
        if (node->space) {
                strpool_free_str(node->space);
                node->space = NULL;
        }
        if (node->selector) {
                free(node->selector);
                node->selector = NULL;
        }
        css_style_decl_destroy(node->list);
        node->list = NULL;
        free(node);
}

static css_style_link_t *css_style_link_create(void)
{
        css_style_link_t *link = calloc(sizeof(css_style_link_t), 1);
        static dict_type_t t;

        dict_init_string_copy_key_type(&t);
        link->group = NULL;
        list_create(&link->styles);
        link->parents = dict_create(&t, NULL);
        return link;
}

static void css_style_link_destroy(css_style_link_t *link)
{
        dict_destroy(link->parents);
        list_destroy_without_node(
            &link->styles, (list_item_destructor_t)css_style_rule_destroy);
        free(link->selector);
        link->selector = NULL;
        link->parents = NULL;
        link->group = NULL;
        free(link);
}

static void css_style_link_destructor(void *privdata, void *data)
{
        css_style_link_destroy(data);
}

static css_style_link_group_t *css_style_link_group_create(
    css_selector_node_t *snode)
{
        static dict_type_t dt = { 0 };
        css_style_link_group_t *group;

        dict_init_string_copy_key_type(&dt);
        dt.val_destructor = css_style_link_destructor;
        group = calloc(sizeof(css_style_link_group_t), 1);
        group->snode = css_selector_node_duplicate(snode);
        group->links = dict_create(&dt, NULL);
        group->name = group->snode->fullname;
        return group;
}

static void css_style_link_group_destroy(css_style_link_group_t *group)
{
        dict_type_t *dtype;
        dtype = group->links->priv_data;
        css_selector_node_destroy(group->snode);
        dict_destroy(group->links);
        free(dtype);
        free(group);
}

static void css_style_link_group_destructor(void *privdata, void *data)
{
        css_style_link_group_destroy(data);
}

static css_style_group_t *css_style_group_create(void)
{
        static dict_type_t type = { 0 };

        dict_init_string_copy_key_type(&type);
        type.val_destructor = css_style_link_group_destructor;
        return dict_create(&type, NULL);
}

/** 根据选择器查找匹配的样式存储空间 */
static css_style_decl_t *css_find_style_store(css_selector_t *selector,
                                              const char *space)
{
        int i, right, len;
        css_style_link_t *link;
        css_style_rule_t *snode;
        css_style_link_group_t *slg;
        css_selector_node_t *sn;
        css_style_group_t *group;
        dict_t *parents;

        char buf[CSS_SELECTOR_MAX_LEN];
        char fullname[CSS_SELECTOR_MAX_LEN];

        link = NULL;
        parents = NULL;
        for (i = 0, right = selector->length - 1; right >= 0; --right, ++i) {
                group = list_get(&css_library.groups, i);
                if (!group) {
                        group = css_style_group_create();
                        list_append(&css_library.groups, group);
                }
                sn = selector->nodes[right];
                slg = dict_fetch_value(group, sn->fullname);
                if (!slg) {
                        slg = css_style_link_group_create(sn);
                        dict_add(group, sn->fullname, slg);
                }
                if (i == 0) {
                        strcpy(fullname, "*");
                } else {
                        strcpy(fullname, buf);
                }
                link = dict_fetch_value(slg->links, fullname);
                if (!link) {
                        link = css_style_link_create();
                        link->group = slg;
                        link->selector = strdup2(fullname);
                        dict_add(slg->links, fullname, link);
                }
                if (i == 0) {
                        strcpy(buf, sn->fullname);
                        strcpy(fullname, buf);
                } else {
                        strcpy(fullname, buf);
                        len = snprintf(buf, CSS_SELECTOR_MAX_LEN, "%s %s",
                                       sn->fullname, fullname);
                        if (len < 0) {
                                logger_error("[css-library] %s: "
                                             "selector(%s...) too long\n",
                                             space, fullname);
                                return NULL;
                        }
                        buf[len] = 0;
                }
                /* 如果有上一级的父链接记录，则将当前链接添加进去 */
                if (parents) {
                        if (!dict_fetch_value(parents, sn->fullname)) {
                                dict_add(parents, sn->fullname, link);
                        }
                }
                parents = link->parents;
        }
        if (!link) {
                return NULL;
        }
        snode = calloc(sizeof(css_style_rule_t), 1);
        if (space) {
                snode->space = strpool_alloc_str(css_library.strpool, space);
                strcpy(snode->space, space);
        } else {
                snode->space = NULL;
        }
        snode->node.data = snode;
        snode->list = css_style_decl_create();
        snode->rank = selector->rank;
        snode->selector = strdup2(fullname);
        snode->batch_num = selector->batch_num;
        list_append_node(&link->styles, &snode->node);
        return snode->list;
}

int css_add_style_decl(css_selector_t *selector, const css_style_decl_t *style,
                       const char *space)
{
        css_style_decl_t *list;

        dict_empty(css_library.cache, NULL);
        list = css_find_style_store(selector, space);
        if (list) {
                css_style_decl_merge(list, style);
        }
        return 0;
}

/**
 * 从指定样式链接记录中查找样式表
 * @param[out] outlist 输出样式表列表，按照权重从大到小排序
 */
static size_t css_style_link_get_styles(css_style_link_t *link, list_t *outlist)
{
        size_t i;
        bool found;
        css_style_rule_t *snode, *out_snode;
        list_node_t *node, *out_node;

        if (!outlist) {
                return link->styles.length;
        }
        for (list_each(node, &link->styles)) {
                i = 0;
                found = false;
                snode = node->data;
                for (list_each(out_node, outlist)) {
                        out_snode = out_node->data;
                        if (snode->rank > out_snode->rank ||
                            (snode->rank == out_snode->rank &&
                             snode->batch_num > out_snode->batch_num)) {
                                found = true;
                                break;
                        }
                        i += 1;
                }
                if (found) {
                        list_insert(outlist, i, snode);
                } else {
                        list_append(outlist, snode);
                }
        }
        return link->styles.length;
}

static size_t css_query_selector_from_link(css_style_link_t *link,
                                           const css_selector_t *selector,
                                           int i, list_t *list)
{
        size_t count = 0;
        css_style_link_t *parent;
        list_t names;
        list_node_t *node;
        css_selector_node_t *selector_node;

        list_create(&names);
        count += css_style_link_get_styles(link, list);
        while (--i >= 0) {
                selector_node = selector->nodes[i];
                css_selector_node_get_name_list(selector_node, &names);
                for (list_each(node, &names)) {
                        parent = dict_fetch_value(link->parents, node->data);
                        if (!parent) {
                                continue;
                        }
                        count += css_query_selector_from_link(parent, selector,
                                                              i, list);
                }
                list_destroy(&names, free);
        }
        return count;
}

int css_query_selector_from_group(int group, const char *name,
                                  const css_selector_t *selector, list_t *list)
{
        int i;
        size_t count;
        dict_t *groups;
        css_style_link_group_t *link_group;
        list_node_t *node;
        list_t names;

        groups = list_get(&css_library.groups, group);
        if (!groups || selector->length < 1) {
                return 0;
        }
        count = 0;
        i = selector->length - 1;
        list_create(&names);
        if (name) {
                list_append(&names, strdup2(name));
        } else {
                css_selector_node_get_name_list(selector->nodes[i], &names);
                list_append(&names, strdup2("*"));
        }
        for (list_each(node, &names)) {
                dict_entry_t *entry;
                dict_iterator_t *iter;
                char *name = node->data;
                link_group = dict_fetch_value(groups, name);
                if (!link_group) {
                        continue;
                }
                iter = dict_get_iterator(link_group->links);
                while ((entry = dict_next(iter))) {
                        css_style_link_t *link = dict_get_val(entry);
                        count += css_query_selector_from_link(link, selector, i,
                                                              list);
                }
                dict_destroy_iterator(iter);
        }
        list_destroy(&names, free);
        return (int)count;
}

static void css_each_style_link(css_style_link_t *link, const char *selector,
                                void (*callback)(css_style_rule_t *,
                                                 const char *, void *),
                                void *data)
{
        dict_entry_t *entry;
        dict_iterator_t *iter;
        list_node_t *node;
        char fullname[CSS_SELECTOR_MAX_LEN];

        if (selector) {
                sprintf(fullname, "%s %s", link->group->name, selector);
        } else {
                strcpy(fullname, link->group->name);
        }
        for (list_each(node, &link->styles)) {
                callback(node->data, fullname, data);
        }
        iter = dict_get_iterator(link->parents);
        while ((entry = dict_next(iter))) {
                css_style_link_t *parent = dict_get_val(entry);
                css_each_style_link(parent, fullname, callback, data);
        }
        dict_destroy_iterator(iter);
}

void css_each_style_rule(void (*callback)(css_style_rule_t *, const char *,
                                          void *),
                         void *data)
{
        dict_t *group;
        css_style_link_t *link;
        css_style_link_group_t *link_group;
        dict_iterator_t *iter;
        dict_entry_t *entry;

        link = NULL;
        group = list_get(&css_library.groups, 0);
        iter = dict_get_iterator(group);
        while ((entry = dict_next(iter))) {
                dict_entry_t *entry_slg;
                dict_iterator_t *iter_slg;

                link_group = dict_get_val(entry);
                iter_slg = dict_get_iterator(link_group->links);
                while ((entry_slg = dict_next(iter_slg))) {
                        link = dict_get_val(entry_slg);
                        css_each_style_link(link, NULL, callback, data);
                }
                dict_destroy_iterator(iter_slg);
        }
        dict_destroy_iterator(iter);
}

css_style_decl_t *css_select_style(const css_selector_t *s)
{
        list_t list;
        list_node_t *node;
        css_style_decl_t *style;

        list_create(&list);
        style = css_style_decl_create();
        css_query_selector(s, &list);
        for (list_each(node, &list)) {
                css_style_decl_merge(style,
                                     ((css_style_rule_t *)node->data)->list);
        }
        list_destroy(&list, NULL);
        return style;
}

size_t css_get_groups_length(void)
{
        return css_library.groups.length;
}

css_style_decl_t *css_select_style_with_cache(const css_selector_t *s)
{
        css_style_decl_t *style;

        style = dict_fetch_value(css_library.cache, &s->hash);
        if (style) {
                return style;
        }
        style = css_select_style(s);
        dict_add(css_library.cache, (void *)&s->hash, style);
        return style;
}

void css_init_library(void)
{
        static dict_type_t dt = { 0 };

        dt.val_dup = NULL;
        dt.key_dup = ikey_dict_key_dup;
        dt.key_compare = ikey_dict_key_compare;
        dt.hash_function = ikey_dict_hash;
        dt.key_destructor = ikey_dict_key_destructor;
        dt.val_destructor = css_style_cache_destructor;
        dt.key_destructor = ikey_dict_key_destructor;
        css_library.cache = dict_create(&dt, NULL);
        css_library.strpool = strpool_create();
        list_create(&css_library.groups);
}

void css_destroy_library(void)
{
        dict_destroy(css_library.cache);
        strpool_destroy(css_library.strpool);
        list_destroy(&css_library.groups, (list_item_destructor_t)dict_destroy);
        css_library.strpool = NULL;
        css_library.cache = NULL;
}

static void css_dump_style_rule(css_style_rule_t *rule,
                                const char *selector_text, void *data)
{
        css_dump_context_t *ctx = data;

        DUMPF("\n[%s]", rule->space ? rule->space : "<none>");
        DUMPF("[rank: %d]\n%s ", rule->rank, selector_text);
        css_dump_style_decl(rule->list, ctx);
}

static void css_dump_style_rules(css_dump_context_t *ctx)
{
        css_each_style_rule(css_dump_style_rule, ctx);
}

size_t css_print_style_rules(void)
{
        css_dump_context_t ctx = {
                .data = NULL, .len = 0, .max_len = 0, .func = css_dump_to_stdout
        };

        css_dump_style_rules(&ctx);
        return ctx.len;
}

size_t css_style_rules_to_string(char *str, size_t max_len)
{
        css_dump_context_t ctx = { .data = str,
                                   .len = 0,
                                   .max_len = max_len,
                                   .func = css_dump_to_buffer };

        css_dump_style_rules(&ctx);
        return ctx.len;
}
