/*
 * lib/ui/src/ui_widget_prototype.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <ui/base.h>
#include <ui/prototype.h>
#include "ui_widget_prototype.h"

static dict_t *ui_widget_prototype_dict;

static void ui_widget_on_init(ui_widget_t *w)
{
}

static void ui_widget_on_destroy(ui_widget_t *w)
{
}

static void ui_widget_on_update(ui_widget_t *w, ui_task_type_t task)
{
}

static void ui_widget_on_set_attr(ui_widget_t *w, const char *name,
                                  const char *value)
{
}

static void ui_widget_on_set_text(ui_widget_t *w, const char *text)
{
}

static void ui_widget_on_sizehint(ui_widget_t *w, ui_sizehint_t *hint)
{
}

static void ui_widget_on_resize(ui_widget_t *w, float width, float height)
{
}

static void ui_widget_on_paint(ui_widget_t *w, pd_context_t *paint,
                               ui_widget_actual_style_t *style)
{
}

static ui_widget_prototype_t ui_widget_default_prototype = {
        .name = NULL,
        .init = ui_widget_on_init,
        .destroy = ui_widget_on_destroy,
        .update = ui_widget_on_update,
        .setattr = ui_widget_on_set_attr,
        .settext = ui_widget_on_set_text,
        .sizehint = ui_widget_on_sizehint,
        .resize = ui_widget_on_resize,
        .paint = ui_widget_on_paint
};

static void ui_widget_prototype_dict_val_destructor(void *privdata, void *data)
{
        ui_widget_prototype_t *proto = data;
        free(proto->name);
        free(proto);
}

ui_widget_prototype_t *ui_create_widget_prototype(const char *name,
                                                  const char *parent_name)
{
        ui_widget_prototype_t *proto;
        ui_widget_prototype_t *parent;

        if (dict_fetch_value(ui_widget_prototype_dict, name)) {
                return NULL;
        }
        proto = malloc(sizeof(ui_widget_prototype_t));
        if (parent_name) {
                parent =
                    dict_fetch_value(ui_widget_prototype_dict, parent_name);
                if (parent) {
                        *proto = *parent;
                        proto->proto = parent;
                } else {
                        *proto = ui_widget_default_prototype;
                }
        } else {
                *proto = ui_widget_default_prototype;
        }
        proto->name = strdup2(name);
        if (dict_add(ui_widget_prototype_dict, proto->name, proto) == 0) {
                return proto;
        }
        free(proto->name);
        free(proto);
        return NULL;
}

ui_widget_prototype_t *ui_get_widget_prototype(const char *name)
{
        ui_widget_prototype_t *proto;

        if (!name) {
                return &ui_widget_default_prototype;
        }
        proto = dict_fetch_value(ui_widget_prototype_dict, name);
        if (!proto) {
                return &ui_widget_default_prototype;
        }
        return proto;
}

bool ui_check_widget_type(ui_widget_t *w, const char *type)
{
        const ui_widget_prototype_t *proto;

        if (!w || !w->type) {
                return false;
        }
        if (strcmp(w->type, type) == 0) {
                return true;
        }
        if (!w->proto) {
                return false;
        }
        for (proto = w->proto->proto; proto; proto = proto->proto) {
                if (strcmp(proto->name, type) == 0) {
                        return true;
                }
        }
        return false;
}

bool ui_check_widget_prototype(ui_widget_t *w,
                               const ui_widget_prototype_t *proto)
{
        const ui_widget_prototype_t *p;
        for (p = w->proto; p; p = p->proto) {
                if (p == proto) {
                        return true;
                }
        }
        return false;
}

void *ui_widget_get_data(ui_widget_t *widget, ui_widget_prototype_t *proto)
{
        unsigned i;
        if (!widget->data.list || !proto) {
                return NULL;
        }
        for (i = 0; i < widget->data.length; ++i) {
                if (widget->data.list[i].proto == proto) {
                        return widget->data.list[i].data;
                }
        }
        return NULL;
}

void *ui_widget_add_data(ui_widget_t *widget, ui_widget_prototype_t *proto,
                         size_t data_size)
{
        void *data;
        ui_widget_data_entry_t *list;
        const size_t size = sizeof(ui_widget_data_entry_t);

        list = realloc(widget->data.list, size * (widget->data.length + 1));
        if (!list) {
                return NULL;
        }
        data = malloc(data_size);
        list[widget->data.length].data = data;
        list[widget->data.length].proto = proto;
        widget->data.list = list;
        widget->data.length += 1;
        return data;
}

void ui_widget_destroy_prototype(ui_widget_t *widget)
{
        if (widget->proto && widget->proto->destroy) {
                widget->proto->destroy(widget);
        }
        while (widget->data.length > 0) {
                widget->data.length -= 1;
                free(widget->data.list[widget->data.length].data);
        }
        if (widget->data.list) {
                free(widget->data.list);
        }
        if (widget->type && !widget->proto->name) {
                free(widget->type);
                widget->type = NULL;
        }
        widget->proto = NULL;
}

void ui_init_widget_prototype(void)
{
        static dict_type_t type;

        dict_init_string_key_type(&type);
        type.val_destructor = ui_widget_prototype_dict_val_destructor;
        ui_widget_prototype_dict = dict_create(&type, NULL);
}

void ui_destroy_widget_prototype(void)
{
        dict_destroy(ui_widget_prototype_dict);
        ui_widget_prototype_dict = NULL;
}
