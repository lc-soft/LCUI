/*
 * src/widgets/button_group.c: -- Button Group widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI/widgets/button_group.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>

typedef struct ui_button_group {
        ui_button_group_orientation_t orientation;
        bool marking;
} ui_button_group_t;

typedef struct ui_button_group_separator {
        ui_button_group_separator_orientation_t orientation;
} ui_button_group_separator_t;

static ui_widget_prototype_t *ui_button_group_prototype;
static ui_widget_prototype_t *ui_button_group_text_prototype;
static ui_widget_prototype_t *ui_button_group_separator_prototype;

static ui_button_group_t *ui_button_group_get_data(ui_widget_t *w);
static ui_button_group_separator_t *ui_button_group_separator_get_data(
    ui_widget_t *w);

static const char *ui_button_group_orientation_names[] = {
        "horizontal",
        "vertical",
};

static const char *ui_button_group_separator_orientation_names[] = {
        "vertical",
        "horizontal",
};

static const char *ui_button_group_css = "\
button-group {\
  display: flex;\
  box-sizing: border-box;\
  width: fit-content;\
  align-items: stretch;\
}\
button-group:vertical {\
  flex-direction: column;\
}\
button-group:has-group {\
  gap: 8px;\
}\
button-group-text {\
  display: flex;\
  align-items: center;\
}\
button-group-separator {\
  display: block;\
}\
";

static void ui_button_group_on_child_unlink(ui_widget_t *w, ui_event_t *e,
                                            void *arg)
{
        ui_widget_t *group = arg;

        ui_widget_remove_class(w, "button-group-item");
        ui_widget_remove_class(w, "button-group-item-horizontal");
        ui_widget_remove_class(w, "button-group-item-vertical");
        ui_widget_remove_class(w, "button-group-embed");
        ui_widget_remove_class(w, "button-group-embed-horizontal");
        ui_widget_remove_class(w, "button-group-embed-vertical");
        if (group && group->state != UI_WIDGET_STATE_DELETED) {
                ui_widget_request_update(group);
        }
}

/*
 * Select, popover and dropdown-menu are layout wrappers: the visible box is
 * their inner trigger, so they must stay out of the group's border geometry.
 */
static bool ui_button_group_is_embed(ui_widget_t *child)
{
        return ui_check_widget_type(child, "select") ||
               ui_check_widget_type(child, "popover") ||
               ui_check_widget_type(child, "dropdown-menu");
}

static void ui_button_group_mark_child(
    ui_widget_t *group, ui_widget_t *child,
    ui_button_group_orientation_t orientation)
{
        bool embed = ui_button_group_is_embed(child);
        const char *base = embed ? "button-group-embed" : "button-group-item";
        const char *horizontal = embed ? "button-group-embed-horizontal"
                                       : "button-group-item-horizontal";
        const char *vertical = embed ? "button-group-embed-vertical"
                                     : "button-group-item-vertical";

        if (!ui_widget_has_class(child, base)) {
                ui_widget_add_class(child, base);
                ui_widget_remove_class(
                    child, embed ? "button-group-item" : "button-group-embed");
                ui_widget_remove_class(child,
                                       embed ? "button-group-item-horizontal"
                                             : "button-group-embed-horizontal");
                ui_widget_remove_class(child,
                                       embed ? "button-group-item-vertical"
                                             : "button-group-embed-vertical");
                ui_widget_on(child, "unlink", ui_button_group_on_child_unlink,
                             group);
        }
        if (orientation == UI_BUTTON_GROUP_VERTICAL) {
                ui_widget_remove_class(child, horizontal);
                ui_widget_add_class(child, vertical);
        } else {
                ui_widget_remove_class(child, vertical);
                ui_widget_add_class(child, horizontal);
        }
}

static void ui_button_group_toggle_status(ui_widget_t *w, const char *name,
                                          bool enable)
{
        if (enable) {
                ui_widget_add_status(w, name);
        } else {
                ui_widget_remove_status(w, name);
        }
}

static void ui_button_group_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_button_group_t *data = ui_button_group_get_data(w);
        ui_widget_t *child;
        bool has_group = false;
        size_t i;

        if (task != UI_TASK_BEFORE_UPDATE || !data || data->marking) {
                return;
        }
        data->marking = true;
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child,
                                              ui_button_group_prototype)) {
                        has_group = true;
                        ui_widget_remove_class(child, "button-group-item");
                        ui_widget_remove_class(child,
                                               "button-group-item-horizontal");
                        ui_widget_remove_class(child,
                                               "button-group-item-vertical");
                        ui_widget_remove_class(child, "button-group-embed");
                        ui_widget_remove_class(child,
                                               "button-group-embed-horizontal");
                        ui_widget_remove_class(child,
                                               "button-group-embed-vertical");
                        continue;
                }
                ui_button_group_mark_child(w, child, data->orientation);
        }
        ui_button_group_toggle_status(w, "has-group", has_group);
        data->marking = false;
}

static ui_button_group_t *ui_button_group_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_button_group_prototype);
}

static ui_button_group_separator_t *ui_button_group_separator_get_data(
    ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_button_group_separator_prototype);
}

static ui_button_group_orientation_t ui_button_group_orientation_from_name(
    const char *name)
{
        if (name && strcmp(name, "vertical") == 0) {
                return UI_BUTTON_GROUP_VERTICAL;
        }
        return UI_BUTTON_GROUP_HORIZONTAL;
}

static ui_button_group_separator_orientation_t
ui_button_group_separator_orientation_from_name(const char *name)
{
        if (name && strcmp(name, "horizontal") == 0) {
                return UI_BUTTON_GROUP_SEPARATOR_HORIZONTAL;
        }
        return UI_BUTTON_GROUP_SEPARATOR_VERTICAL;
}

static void ui_button_group_sync_orientation(
    ui_widget_t *w, ui_button_group_orientation_t orientation)
{
        ui_button_group_t *data = ui_button_group_get_data(w);
        int i;

        if (!data) {
                return;
        }
        for (i = 0; i < 2; ++i) {
                ui_widget_remove_status(w,
                                        ui_button_group_orientation_names[i]);
        }
        data->orientation = orientation;
        ui_widget_add_status(w, ui_button_group_orientation_names[orientation]);
        ui_widget_request_update(w);
}

static void ui_button_group_separator_sync_orientation(
    ui_widget_t *w, ui_button_group_separator_orientation_t orientation)
{
        ui_button_group_separator_t *data =
            ui_button_group_separator_get_data(w);
        int i;

        if (!data) {
                return;
        }
        for (i = 0; i < 2; ++i) {
                ui_widget_remove_status(
                    w, ui_button_group_separator_orientation_names[i]);
        }
        data->orientation = orientation;
        ui_widget_add_status(
            w, ui_button_group_separator_orientation_names[orientation]);
}

static void ui_button_group_on_init(ui_widget_t *w)
{
        ui_button_group_t *data = ui_widget_add_data(
            w, ui_button_group_prototype, sizeof(ui_button_group_t));

        memset(data, 0, sizeof(*data));
        data->orientation = UI_BUTTON_GROUP_HORIZONTAL;
        ui_widget_add_status(w, "horizontal");
}

static void ui_button_group_on_set_attr(ui_widget_t *w, const char *name,
                                        const char *value)
{
        if (strcmp(name, "orientation") == 0) {
                ui_button_group_sync_orientation(
                    w, ui_button_group_orientation_from_name(value));
        }
}

static void ui_button_group_separator_on_init(ui_widget_t *w)
{
        ui_button_group_separator_t *data =
            ui_widget_add_data(w, ui_button_group_separator_prototype,
                               sizeof(ui_button_group_separator_t));

        memset(data, 0, sizeof(*data));
        data->orientation = UI_BUTTON_GROUP_SEPARATOR_VERTICAL;
        ui_widget_add_status(w, "vertical");
}

static void ui_button_group_separator_on_set_attr(ui_widget_t *w,
                                                  const char *name,
                                                  const char *value)
{
        if (strcmp(name, "orientation") == 0) {
                ui_button_group_separator_sync_orientation(
                    w, ui_button_group_separator_orientation_from_name(value));
        }
}

ui_widget_t *ui_create_button_group(void)
{
        return ui_create_widget_with_prototype(ui_button_group_prototype);
}

ui_widget_t *ui_create_button_group_text(void)
{
        return ui_create_widget_with_prototype(ui_button_group_text_prototype);
}

ui_widget_t *ui_create_button_group_separator(void)
{
        return ui_create_widget_with_prototype(
            ui_button_group_separator_prototype);
}

void ui_button_group_set_orientation(ui_widget_t *group,
                                     ui_button_group_orientation_t orientation)
{
        if (orientation < UI_BUTTON_GROUP_HORIZONTAL ||
            orientation > UI_BUTTON_GROUP_VERTICAL) {
                orientation = UI_BUTTON_GROUP_HORIZONTAL;
        }
        ui_widget_set_attr(group, "orientation",
                           ui_button_group_orientation_names[orientation]);
}

ui_button_group_orientation_t ui_button_group_get_orientation(
    ui_widget_t *group)
{
        ui_button_group_t *data = ui_button_group_get_data(group);

        return data ? data->orientation : UI_BUTTON_GROUP_HORIZONTAL;
}

void ui_button_group_separator_set_orientation(
    ui_widget_t *separator, ui_button_group_separator_orientation_t orientation)
{
        if (orientation < UI_BUTTON_GROUP_SEPARATOR_VERTICAL ||
            orientation > UI_BUTTON_GROUP_SEPARATOR_HORIZONTAL) {
                orientation = UI_BUTTON_GROUP_SEPARATOR_VERTICAL;
        }
        ui_widget_set_attr(
            separator, "orientation",
            ui_button_group_separator_orientation_names[orientation]);
}

ui_button_group_separator_orientation_t
ui_button_group_separator_get_orientation(ui_widget_t *separator)
{
        ui_button_group_separator_t *data =
            ui_button_group_separator_get_data(separator);

        return data ? data->orientation : UI_BUTTON_GROUP_SEPARATOR_VERTICAL;
}

void ui_register_button_group(void)
{
        ui_button_group_prototype =
            ui_create_widget_prototype("button-group", NULL);
        ui_button_group_text_prototype =
            ui_create_widget_prototype("button-group-text", NULL);
        ui_button_group_separator_prototype =
            ui_create_widget_prototype("button-group-separator", NULL);

        ui_button_group_prototype->init = ui_button_group_on_init;
        ui_button_group_prototype->setattr = ui_button_group_on_set_attr;
        ui_button_group_prototype->update = ui_button_group_on_update;
        ui_button_group_separator_prototype->init =
            ui_button_group_separator_on_init;
        ui_button_group_separator_prototype->setattr =
            ui_button_group_separator_on_set_attr;

        ui_load_css_string(ui_button_group_css, __FILE__);
}
