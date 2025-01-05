/*
 * lib/css/src/selector.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <css/selector.h>

#define MAX_NAME_LEN 256

enum css_selector_rank {
        GENERAL_RANK = 0,
        TYPE_RANK = 1,
        CLASS_RANK = 10,
        PCLASS_RANK = 10,
        ID_RANK = 100
};

enum css_selector_name_finder_level {
        LEVEL_NONE,
        LEVEL_TYPE,
        LEVEL_ID,
        LEVEL_CLASS,
        LEVEL_CLASS_2,
        LEVEL_STATUS,
        LEVEL_STATUS_2,
        LEVEL_TOTAL_NUM
};

/* 样式表查找器的上下文数据结构 */
typedef struct css_selector_name_collector {
        int level;    /**< 当前选择器层级 */
        int class_i;  /**< 当前处理到第几个类名 */
        int status_i; /**< 当前处理到第几个状态名（伪类名） */
        int name_i;   /**< 选择器名称从第几个字符开始 */
        char name[MAX_NAME_LEN];   /**< 选择器名称缓存 */
        css_selector_node_t *node; /**< 针对的选择器结点 */
} css_selector_name_collector_t;

static void css_selector_init_name_collector(
    css_selector_name_collector_t *sfinder, css_selector_node_t *snode)
{
        sfinder->level = 0;
        sfinder->class_i = 0;
        sfinder->name_i = 0;
        sfinder->status_i = 0;
        sfinder->name[0] = 0;
        sfinder->node = snode;
}

static void css_selector_destroy_name_collector(
    css_selector_name_collector_t *sfinder)
{
        sfinder->name_i = 0;
        sfinder->name[0] = 0;
        sfinder->class_i = 0;
        sfinder->status_i = 0;
        sfinder->node = NULL;
        sfinder->level = LEVEL_NONE;
}

/* 生成选择器全名列表 */
static int css_selector_collect_name(css_selector_name_collector_t *sfinder,
                                     list_t *list)
{
        size_t len, old_len;
        int i, old_level, count = 0;
        char *fullname = sfinder->name + sfinder->name_i;

        old_len = len = strlen(fullname);
        old_level = sfinder->level;
        switch (sfinder->level) {
        case LEVEL_TYPE:
                /* 按类型选择器生成选择器全名 */
                if (!sfinder->node->type) {
                        return 0;
                }
                strcpy(fullname, sfinder->node->type);
                list_append(list, strdup2(fullname));
                break;
        case LEVEL_ID:
                /* 按ID选择器生成选择器全名 */
                if (!sfinder->node->id) {
                        return 0;
                }
                fullname[len++] = '#';
                fullname[len] = 0;
                strcpy(fullname + len, sfinder->node->id);
                list_append(list, strdup2(fullname));
                break;
        case LEVEL_CLASS:
                if (!sfinder->node->classes) {
                        return 0;
                }
                /* 按类选择器生成选择器全名
                 * 假设当前选择器全名为：text#main-btn-text，且有 .a .b .c
                 * 这三个类，那么下面的处理将会拆分成以下三个选择器：
                 * text#test-text.a
                 * text#test-text.b
                 * text#test-text.a
                 */
                fullname[len++] = '.';
                for (i = 0; sfinder->node->classes[i]; ++i) {
                        sfinder->level += 1;
                        sfinder->class_i = i;
                        strcpy(fullname + len, sfinder->node->classes[i]);
                        list_append(list, strdup2(fullname));
                        /* 将当前选择器名与其它层级的选择器名组合 */
                        while (sfinder->level < LEVEL_TOTAL_NUM) {
                                count +=
                                    css_selector_collect_name(sfinder, list);
                                sfinder->level += 1;
                        }
                        sfinder->level = LEVEL_CLASS;
                }
                sfinder->level = LEVEL_CLASS;
                fullname[old_len] = 0;
                sfinder->class_i = 0;
                return count;
        case LEVEL_CLASS_2:
                if (!sfinder->node->classes) {
                        return 0;
                }
                /* 按类选择器生成选择器全名，结果类似于这样：
                 * text#test-text.a.b
                 * text#test-text.a.c
                 * text#test-text.b.c
                 * text#test-text.a.b.c
                 */
                fullname[len++] = '.';
                for (i = 0; sfinder->node->classes[i]; ++i) {
                        if (i <= sfinder->class_i) {
                                continue;
                        }
                        strcpy(fullname + len, sfinder->node->classes[i]);
                        list_append(list, strdup2(fullname));
                        sfinder->class_i = i;
                        count += css_selector_collect_name(sfinder, list);
                        sfinder->class_i = 0;
                        sfinder->level = LEVEL_STATUS;
                        /**
                         * 递归拼接伪类名，例如：
                         * text#main-btn-text:active
                         */
                        count += css_selector_collect_name(sfinder, list);
                        sfinder->level = LEVEL_CLASS_2;
                }
                fullname[old_len] = 0;
                sfinder->level = LEVEL_CLASS_2;
                return count;
        case LEVEL_STATUS:
                if (!sfinder->node->status) {
                        return 0;
                }
                fullname[len++] = ':';
                sfinder->level = LEVEL_STATUS_2;
                /**
                 * 按伪类选择器生成选择器全名
                 * 假设当前选择器全名为：text#main-btn-text:hover:focus:active
                 * 那么下面的循环会将它拆分为以下几个选择器：
                 * text#main-btn-text:active
                 * text#main-btn-text:active:focus
                 * text#main-btn-text:active:focus:hover
                 * text#main-btn-text:active:hover
                 * text#main-btn-text:focus
                 * text#main-btn-text:focus:hover
                 * text#main-btn-text:hover
                 */
                for (i = 0; sfinder->node->status[i]; ++i) {
                        sfinder->status_i = i;
                        strcpy(fullname + len, sfinder->node->status[i]);
                        list_append(list, strdup2(fullname));
                        /**
                         * 递归调用，以一层层拼接出像下面这样的选择器：
                         * text#main-btn-text:active:focus:hover
                         */
                        count += css_selector_collect_name(sfinder, list);
                }
                sfinder->level = LEVEL_STATUS;
                fullname[old_len] = 0;
                sfinder->status_i = 0;
                return count;
        case LEVEL_STATUS_2:
                if (!sfinder->node->status) {
                        return 0;
                }
                /** 按伪类选择器生成选择器全名 */
                for (i = 0; sfinder->node->status[i]; ++i) {
                        if (i <= sfinder->status_i) {
                                continue;
                        }
                        fullname[len] = ':';
                        strcpy(fullname + len + 1, sfinder->node->status[i]);
                        list_append(list, strdup2(fullname));
                        sfinder->status_i = i;
                        count += css_selector_collect_name(sfinder, list);
                        sfinder->status_i = 0;
                }
                fullname[old_len] = 0;
                return count;
        default:
                break;
        }
        for (i = sfinder->level + 1; i < LEVEL_TOTAL_NUM; ++i) {
                if (i == LEVEL_STATUS_2 || i == LEVEL_CLASS_2) {
                        continue;
                }
                sfinder->level = i;
                count += css_selector_collect_name(sfinder, list);
        }
        fullname[old_len] = 0;
        sfinder->level = old_level;
        return count;
}

static int selector_node_save(css_selector_node_t *node, const char *name,
                              int len, char type)
{
        char *str;
        if (len < 1) {
                return 0;
        }
        switch (type) {
        case 0:
                if (node->type) {
                        break;
                }
                len += 1;
                str = malloc(sizeof(char) * len);
                strncpy(str, name, len);
                node->type = str;
                return TYPE_RANK;
        case ':':
                if (strlist_sorted_add(&node->status, name) == 0) {
                        return PCLASS_RANK;
                }
                break;
        case '.':
                if (strlist_sorted_add(&node->classes, name) == 0) {
                        return CLASS_RANK;
                }
                break;
        case '#':
                if (node->id) {
                        break;
                }
                len += 1;
                str = malloc(sizeof(char) * len);
                strncpy(str, name, len);
                node->id = str;
                return ID_RANK;
        default:
                break;
        }
        return 0;
}

int css_selector_node_get_name_list(css_selector_node_t *sn, list_t *names)
{
        int count;
        css_selector_name_collector_t sfinder;
        css_selector_init_name_collector(&sfinder, sn);
        count = css_selector_collect_name(&sfinder, names);
        css_selector_destroy_name_collector(&sfinder);
        return count;
}

int css_selector_node_update(css_selector_node_t *node)
{
        size_t i, len = 0;
        char *fullname;

        node->rank = 0;
        if (node->id) {
                len += strlen(node->id) + 1;
                node->rank += ID_RANK;
        }
        if (node->type) {
                len += strlen(node->type) + 1;
                node->rank += TYPE_RANK;
        }
        if (node->classes) {
                for (i = 0; node->classes[i]; ++i) {
                        len += strlen(node->classes[i]) + 1;
                        node->rank += CLASS_RANK;
                }
        }
        if (node->status) {
                for (i = 0; node->status[i]; ++i) {
                        len += strlen(node->status[i]) + 1;
                        node->rank += PCLASS_RANK;
                }
        }
        if (len > 0) {
                fullname = malloc(sizeof(char) * (len + 1));
                if (!fullname) {
                        return -ENOMEM;
                }
                fullname[0] = 0;
                if (node->type) {
                        strcat(fullname, node->type);
                }
                if (node->id) {
                        strcat(fullname, "#");
                        strcat(fullname, node->id);
                }
                if (node->classes) {
                        for (i = 0; node->classes[i]; ++i) {
                                strcat(fullname, ".");
                                strcat(fullname, node->classes[i]);
                        }
                        len += 1;
                }
                if (node->status) {
                        for (i = 0; node->status[i]; ++i) {
                                strcat(fullname, ":");
                                strcat(fullname, node->status[i]);
                        }
                        len += 1;
                }
        } else {
                fullname = NULL;
        }
        if (node->fullname) {
                free(node->fullname);
        }
        node->fullname = fullname;
        return 0;
}

void css_selector_update(css_selector_t *s)
{
        int i;
        const unsigned char *p;
        unsigned int hash = 5381;
        for (i = 0; i < s->length; ++i) {
                p = (unsigned char *)s->nodes[i]->fullname;
                while (*p) {
                        hash = ((hash << 5) + hash) + (*p++);
                }
        }
        s->hash = hash;
}

int css_selector_append(css_selector_t *selector, css_selector_node_t *node)
{
        const unsigned char *p;

        if (selector->length >= CSS_SELECTOR_MAX_DEPTH) {
                logger_warning("[css] warning: the number of nodes in the "
                               "selector has exceeded the %d limit\n",
                               CSS_SELECTOR_MAX_DEPTH);
                return -1;
        }
        selector->nodes[selector->length++] = node;
        selector->nodes[selector->length] = NULL;
        p = (unsigned char *)node->fullname;
        while (*p) {
                selector->hash =
                    ((selector->hash << 5) + selector->hash) + (*p++);
        }
        return 0;
}

css_selector_t *css_selector_create(const char *selector)
{
        const char *p;
        int name_index, node_index, rank;
        static int batch_num = 0;
        char type = 0, name[MAX_NAME_LEN];
        bool name_valid = false;
        bool escape = false;
        css_selector_node_t *node = NULL;
        css_selector_t *s = calloc(sizeof(css_selector_t), 1);

        s->batch_num = ++batch_num;
        s->nodes = calloc(sizeof(css_selector_node_t), CSS_SELECTOR_MAX_DEPTH);
        if (!selector) {
                s->length = 0;
                s->nodes[0] = NULL;
                return s;
        }
        for (name_index = 0, node_index = 0, p = selector; *p; ++p) {
                if (!node && name_valid) {
                        node = calloc(sizeof(css_selector_node_t), 1);
                        if (node_index >= CSS_SELECTOR_MAX_DEPTH) {
                                logger_warning(
                                    "%s: selector node list is too long.\n",
                                    selector);
                                return NULL;
                        }
                        s->nodes[node_index] = node;
                }
                if (!escape) {
                        switch (*p) {
                        case '\\':
                                escape = true;
                                continue;
                        case ':':
                        case '.':
                        case '#':
                                if (!name_valid) {
                                        name_valid = true;
                                        type = *p;
                                        continue;
                                }
                                /* 保存上个结点 */
                                rank = selector_node_save(node, name,
                                                          name_index, type);
                                name_valid = true;
                                type = *p;
                                if (rank > 0) {
                                        s->rank += rank;
                                        name_index = 0;
                                        continue;
                                }
                                logger_error(
                                    "%s: invalid selector node at %ld.\n",
                                    selector, p - selector - name_index);
                                css_selector_node_destroy(node);
                                node = NULL;
                                name_index = 0;
                                continue;
                        case ' ':
                        case '\r':
                        case '\n':
                        case '\t':
                                if (!name_valid) {
                                        name_index = 0;
                                        node = NULL;
                                        continue;
                                }
                                name_valid = false;
                                rank = selector_node_save(node, name,
                                                          name_index, type);
                                if (rank > 0) {
                                        css_selector_node_update(node);
                                        s->rank += rank;
                                        node = NULL;
                                        name_index = 0;
                                        node_index++;
                                        continue;
                                }
                                logger_error(
                                    "%s: invalid selector node at %ld.\n",
                                    selector, p - selector - name_index);
                                css_selector_node_destroy(node);
                                node = NULL;
                                name_index = 0;
                                continue;
                        default:
                                break;
                        }
                }
		escape = false;
                if (!name_valid) {
                        type = 0;
                        name_valid = true;
                }
                name[name_index++] = *p;
                name[name_index] = 0;
        }
        if (name_valid) {
                if (!node) {
                        if (node_index >= CSS_SELECTOR_MAX_DEPTH) {
                                logger_warning(
                                    "%s: selector node list is too long.\n",
                                    selector);
                                return NULL;
                        }
                        node = calloc(sizeof(css_selector_node_t), 1);
                        s->nodes[node_index] = node;
                }
                rank = selector_node_save(s->nodes[node_index], name,
                                          name_index, type);
                if (rank > 0) {
                        css_selector_node_update(s->nodes[node_index]);
                        s->rank += rank;
                        node_index++;
                } else {
                        css_selector_node_destroy(s->nodes[node_index]);
                }
        }
        s->nodes[node_index] = NULL;
        s->length = node_index;
        css_selector_update(s);
        return s;
}

css_selector_node_t *css_selector_node_duplicate(const css_selector_node_t *src)
{
        int i;
        css_selector_node_t *dst;

        dst = calloc(sizeof(css_selector_node_t), 1);
        dst->id = src->id ? strdup2(src->id) : NULL;
        dst->type = src->type ? strdup2(src->type) : NULL;
        dst->fullname = src->fullname ? strdup2(src->fullname) : NULL;
        if (src->classes) {
                for (i = 0; src->classes[i]; ++i) {
                        strlist_sorted_add(&dst->classes, src->classes[i]);
                }
        }
        if (src->status) {
                for (i = 0; src->status[i]; ++i) {
                        strlist_sorted_add(&dst->status, src->status[i]);
                }
        }
        return dst;
}

void css_selector_node_destroy(css_selector_node_t *node)
{
        if (node->type) {
                free(node->type);
                node->type = NULL;
        }
        if (node->id) {
                free(node->id);
                node->id = NULL;
        }
        if (node->classes) {
                strlist_free(node->classes);
                node->classes = NULL;
        }
        if (node->status) {
                strlist_free(node->status);
                node->status = NULL;
        }
        if (node->fullname) {
                free(node->fullname);
                node->fullname = NULL;
        }
        free(node);
}

void css_selector_destroy(css_selector_t *s)
{
        int i;
        for (i = 0; i < CSS_SELECTOR_MAX_DEPTH; ++i) {
                if (!s->nodes[i]) {
                        break;
                }
                css_selector_node_destroy(s->nodes[i]);
                s->nodes[i] = NULL;
        }
        s->rank = 0;
        s->length = 0;
        s->batch_num = 0;
        free(s->nodes);
        free(s);
}

css_selector_t *css_selector_duplicate(css_selector_t *selector)
{
        int i;
        css_selector_t *s;

        s = css_selector_create(NULL);
        for (i = 0; i < selector->length; ++i) {
                s->nodes[i] = css_selector_node_duplicate(selector->nodes[i]);
        }
        s->nodes[selector->length] = NULL;
        s->length = selector->length;
        s->hash = selector->hash;
        s->rank = selector->rank;
        s->batch_num = selector->batch_num;
        return s;
}
