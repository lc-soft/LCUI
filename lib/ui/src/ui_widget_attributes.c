/*
 * lib/ui/src/ui_widget_attributes.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ui/base.h>
#include "ui_widget_attributes.h"

static void ui_widget_attr_destructor(void *privdata, void *data)
{
        ui_widget_attribute_t *attr = data;

        if (attr->value.destructor) {
                attr->value.destructor(attr->value.data);
        }
        free(attr->name);
        attr->name = NULL;
        attr->value.data = NULL;
        free(attr);
}

int ui_widget_set_attr_ex(ui_widget_t *w, const char *name, void *value,
                          int value_type, void (*value_destructor)(void *))
{
        ui_widget_attribute_t *attr;
        static dict_type_t dict_type;
        static bool dict_type_available = false;

        if (!dict_type_available) {
                dict_init_string_key_type(&dict_type);
                dict_type.val_destructor = ui_widget_attr_destructor;
                dict_type_available = true;
        }
        if (!w->attributes) {
                w->attributes = dict_create(&dict_type, NULL);
        }
        attr = dict_fetch_value(w->attributes, name);
        if (attr) {
                if (attr->value.destructor) {
                        attr->value.destructor(attr->value.data);
                }
        } else {
                attr = malloc(sizeof(ui_widget_attribute_t));
                attr->name = strdup2(name);
                dict_add(w->attributes, attr->name, attr);
        }
        attr->value.data = value;
        attr->value.type = value_type;
        attr->value.destructor = value_destructor;
        return 0;
}

int ui_widget_set_attr(ui_widget_t *w, const char *name, const char *value)
{
        int ret;
        char *value_str = NULL;

        if (value) {
                value_str = strdup2(value);
                if (!value_str) {
                        return -ENOMEM;
                }
                if (strcmp(name, "disabled") == 0) {
                        if (!value || strcmp(value, "false") != 0) {
                                ui_widget_set_disabled(w, true);
                        } else {
                                ui_widget_set_disabled(w, false);
                        }
                } else if (strcmp(name, "tabindex") == 0) {
                        if (value) {
                                sscanf(value, "%d", &w->tab_index);
                        }
                }
                ret = ui_widget_set_attr_ex(w, name, value_str,
                                            CSS_STRING_VALUE, free);
        } else {
                ret = ui_widget_set_attr_ex(w, name, NULL, CSS_NO_VALUE, NULL);
        }
        if (w->proto && w->proto->setattr) {
                w->proto->setattr(w, name, value_str);
        }
        return ret;
}

const char *ui_widget_get_attr(ui_widget_t *w, const char *name)
{
        ui_widget_attribute_t *attr;
        if (!w->attributes) {
                return NULL;
        }
        attr = dict_fetch_value(w->attributes, name);
        if (attr) {
                return attr->value.string;
        }
        return NULL;
}

void ui_widget_destroy_attrs(ui_widget_t *w)
{
        if (w->attributes) {
                dict_destroy(w->attributes);
        }
        w->attributes = NULL;
}
