/*
 * src/widgets/field.c: -- Field widget family
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI/widgets/field.h>
#include <LCUI/widgets/label.h>
#include <LCUI/widgets.h>
#include <ui/base.h>
#include <ui/events.h>
#include <css.h>

struct find_type_args {
        const char *target_type;
        bool found;
};

struct checked_args {
        bool found;
};

typedef struct ui_has_class_rule {
        ui_widget_t *target;
        const char *class_name;
        bool (*check)(ui_widget_t *w);
        bool listen_link;
        bool listen_change;
        bool has_parent_listener;
} ui_has_class_rule_t;

static ui_widget_prototype_t *ui_field_prototype = NULL;
static ui_widget_prototype_t *ui_field_set_prototype = NULL;
static ui_widget_prototype_t *ui_field_legend_prototype = NULL;
static ui_widget_prototype_t *ui_field_group_prototype = NULL;
static ui_widget_prototype_t *ui_field_label_prototype = NULL;
static ui_widget_prototype_t *ui_field_description_prototype = NULL;
static ui_widget_prototype_t *ui_field_separator_prototype = NULL;
static ui_widget_prototype_t *ui_field_content_prototype = NULL;
static ui_widget_prototype_t *ui_field_title_prototype = NULL;

static const char *ui_field_css = "\
field {\
  display: flex;\
  flex-direction: column;\
  gap: 4px;\
  width: 100%;\
}\
field:horizontal {\
  flex-direction: row;\
  align-items: center;\
}\
field-set {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
field-legend {\
  font-weight: 600;\
}\
field-group {\
  display: flex;\
  flex-direction: column;\
  gap: 20px;\
  width: 100%;\
}\
field-label {\
  display: flex;\
  gap: 8px;\
}\
field-description {\
  color: #666;\
  font-size: 12px;\
  line-height: 1.5;\
}\
field-content {\
  display: flex;\
  flex: 1;\
  flex-direction: column;\
  gap: 2px;\
}\
field-title {\
  font-weight: 500;\
}\
field-separator {\
  display: block;\
  height: 0;\
  border-top: 1px solid #e5e5e5;\
  margin: 0;\
}\
field.has-field-content {\
  align-items: flex-start;\
}\
field-label.has-field {\
  width: 100%;\
  border: 1px solid #e5e5e5;\
  padding: 8px;\
  border-radius: 6px;\
}\
field-label.has-checked {\
  background-color: rgba(0, 0, 0, 0.05);\
}\
field:disabled, field-set:disabled, field-legend:disabled,\
field-group:disabled, field-label:disabled,\
field-description:disabled, field-content:disabled,\
field-title:disabled, field-separator:disabled {\
  opacity: 0.5;\
}";

static void find_type_each(ui_widget_t *w, void *arg)
{
        struct find_type_args *a = arg;
        if (a->found) {
                return;
        }
        if (w->type && strcmp(w->type, a->target_type) == 0) {
                a->found = true;
        }
}

static void checked_each(ui_widget_t *w, void *arg)
{
        struct checked_args *a = arg;
        if (a->found) {
                return;
        }
        if (!w->type) {
                return;
        }
        if (strcmp(w->type, "checkbox") == 0 ||
            strcmp(w->type, "radio-group-item") == 0 ||
            strcmp(w->type, "switch") == 0) {
                const char *v = ui_widget_get_attr(w, "checked");
                if (v && strcmp(v, "true") == 0) {
                        a->found = true;
                }
        }
}

static bool ui_field_check_has_field_content(ui_widget_t *w)
{
        struct find_type_args a = { "field-content", false };
        ui_widget_each(w, find_type_each, &a);
        return a.found;
}

static bool ui_field_label_check_has_field(ui_widget_t *w)
{
        struct find_type_args a = { "field-content", false };
        ui_widget_each(w, find_type_each, &a);
        return a.found;
}

static bool ui_field_label_check_has_checked(ui_widget_t *w)
{
        struct checked_args a = { false };
        ui_widget_each(w, checked_each, &a);
        return a.found;
}

static void ui_has_class_run_check(ui_has_class_rule_t *rule)
{
        if (rule->check(rule->target)) {
                ui_widget_add_class(rule->target, rule->class_name);
        } else {
                ui_widget_remove_class(rule->target, rule->class_name);
        }
}
static void ui_has_class_on_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_has_class_rule_t *rule = e->data;
        ui_has_class_run_check(rule);
}

static void ui_has_class_on_destroy(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_has_class_rule_t *rule = e->data;
        if (rule->has_parent_listener && w->parent) {
                ui_widget_off(w->parent, "change", ui_has_class_on_change,
                              rule);
        }
}

static void ui_has_class_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_has_class_rule_t *rule = e->data;
        ui_has_class_run_check(rule);
        if (rule->listen_change && w->parent && w->parent->type &&
            strcmp(w->parent->type, "radio-group") == 0) {
                ui_widget_on(w->parent, "change", ui_has_class_on_change, rule);
                rule->has_parent_listener = true;
        }
}

static void ui_has_class_on_unlink(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_has_class_rule_t *rule = e->data;
        ui_has_class_run_check(rule);
        if (rule->has_parent_listener) {
                ui_widget_off(w->parent, "change", ui_has_class_on_change,
                              rule);
                rule->has_parent_listener = false;
        }
}

static void ui_widget_sync_has_class(ui_widget_t *w, const char *class_name,
                                     bool (*check)(ui_widget_t *w),
                                     bool listen_link, bool listen_change)
{
        ui_has_class_rule_t *rule;

        rule = malloc(sizeof(ui_has_class_rule_t));
        rule->target = w;
        rule->class_name = class_name;
        rule->check = check;
        rule->listen_link = listen_link;
        rule->listen_change = listen_change;
        rule->has_parent_listener = false;

        ui_has_class_run_check(rule);

        ui_widget_add_event_listener(w, UI_EVENT_DESTROY,
                                     ui_has_class_on_destroy, rule, free);

        if (listen_link) {
                ui_widget_on(w, "link", ui_has_class_on_link, rule);
                ui_widget_on(w, "unlink", ui_has_class_on_unlink, rule);
        }

        if (listen_change) {
                ui_widget_on(w, "change", ui_has_class_on_change, rule);
        }
}

static void ui_field_sync_orientation(ui_widget_t *w)
{
        const char *v = ui_widget_get_attr(w, "orientation");
        if (v && strcmp(v, "horizontal") == 0) {
                ui_widget_add_status(w, "horizontal");
        } else {
                ui_widget_remove_status(w, "horizontal");
        }
}

static void ui_field_on_set_attr(ui_widget_t *w, const char *name,
                                 const char *value)
{
        if (strcmp(name, "orientation") == 0) {
                ui_field_sync_orientation(w);
        }
}

static void ui_field_on_init(ui_widget_t *w)
{
        ui_field_sync_orientation(w);
        ui_widget_sync_has_class(w, "has-field-content",
                                 ui_field_check_has_field_content, true, false);
}

static void ui_field_label_on_init(ui_widget_t *w)
{
        ui_field_label_prototype->proto->init(w);
        ui_widget_sync_has_class(w, "has-field", ui_field_label_check_has_field,
                                 true, false);
        ui_widget_sync_has_class(w, "has-checked",
                                 ui_field_label_check_has_checked, true, true);
}

ui_widget_t *ui_create_field(void)
{
        return ui_create_widget_with_prototype(ui_field_prototype);
}

ui_widget_t *ui_create_field_set(void)
{
        return ui_create_widget_with_prototype(ui_field_set_prototype);
}

ui_widget_t *ui_create_field_legend(void)
{
        return ui_create_widget_with_prototype(ui_field_legend_prototype);
}

ui_widget_t *ui_create_field_group(void)
{
        return ui_create_widget_with_prototype(ui_field_group_prototype);
}

ui_widget_t *ui_create_field_label(void)
{
        return ui_create_widget_with_prototype(ui_field_label_prototype);
}

ui_widget_t *ui_create_field_description(void)
{
        return ui_create_widget_with_prototype(ui_field_description_prototype);
}

ui_widget_t *ui_create_field_separator(void)
{
        return ui_create_widget_with_prototype(ui_field_separator_prototype);
}

ui_widget_t *ui_create_field_content(void)
{
        return ui_create_widget_with_prototype(ui_field_content_prototype);
}

ui_widget_t *ui_create_field_title(void)
{
        return ui_create_widget_with_prototype(ui_field_title_prototype);
}

void ui_register_field(void)
{
        ui_field_prototype = ui_create_widget_prototype("field", NULL);
        ui_field_prototype->init = ui_field_on_init;
        ui_field_prototype->setattr = ui_field_on_set_attr;
        ui_load_css_string(ui_field_css, __FILE__);
}

void ui_register_field_set(void)
{
        ui_field_set_prototype = ui_create_widget_prototype("field-set", NULL);
}

void ui_register_field_legend(void)
{
        ui_field_legend_prototype =
            ui_create_widget_prototype("field-legend", "text");
}

void ui_register_field_group(void)
{
        ui_field_group_prototype =
            ui_create_widget_prototype("field-group", NULL);
}

void ui_register_field_label(void)
{
        ui_field_label_prototype =
            ui_create_widget_prototype("field-label", "label");
        ui_field_label_prototype->init = ui_field_label_on_init;
}

void ui_register_field_description(void)
{
        ui_field_description_prototype =
            ui_create_widget_prototype("field-description", "text");
}

void ui_register_field_separator(void)
{
        ui_field_separator_prototype =
            ui_create_widget_prototype("field-separator", NULL);
}

void ui_register_field_content(void)
{
        ui_field_content_prototype =
            ui_create_widget_prototype("field-content", NULL);
}

void ui_register_field_title(void)
{
        ui_field_title_prototype =
            ui_create_widget_prototype("field-title", "text");
}
