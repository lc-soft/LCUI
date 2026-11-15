/*
 * src/widgets/input_group.c: -- Input Group widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <stdlib.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/input_group.h>
#include <LCUI/widgets/textinput.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>

typedef struct ui_input_group {
        bool ordering;
} ui_input_group_t;

typedef struct ui_input_group_addon {
        ui_input_group_addon_align_t align;
} ui_input_group_addon_t;

static ui_widget_prototype_t *ui_input_group_prototype;
static ui_widget_prototype_t *ui_input_group_addon_prototype;
static ui_widget_prototype_t *ui_input_group_text_prototype;
static ui_widget_prototype_t *ui_input_group_kbd_prototype;

static void ui_input_group_addon_on_click(ui_widget_t *w, ui_event_t *e,
                                          void *arg);

static const char *ui_input_group_align_names[] = {
        "inline-start",
        "inline-end",
        "block-start",
        "block-end",
};

static const char *ui_input_group_addon_statuses[] = {
        "align-inline-start",
        "align-inline-end",
        "align-block-start",
        "align-block-end",
};

static const char *ui_input_group_css = "\
input-group {\
  display: flex;\
  align-items: center;\
  box-sizing: border-box;\
  width: 100%;\
  min-width: 0;\
  height: 32px;\
}\
input-group-addon {\
  display: flex;\
  align-items: center;\
  justify-content: center;\
}\
input-group-text,\
input-group-kbd {\
  display: inline-flex;\
  align-items: center;\
}\
";

static ui_input_group_addon_t *ui_input_group_addon_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_input_group_addon_prototype);
}

static ui_input_group_t *ui_input_group_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_input_group_prototype);
}

static ui_widget_t *ui_input_group_find_root(ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (ui_check_widget_prototype(w, ui_input_group_prototype)) {
                        return w;
                }
        }
        return NULL;
}

static ui_input_group_addon_align_t ui_input_group_addon_align_from_name(
    const char *name)
{
        int i;

        if (name) {
                for (i = 0; i < 4; ++i) {
                        if (strcmp(name, ui_input_group_align_names[i]) == 0) {
                                return (ui_input_group_addon_align_t)i;
                        }
                }
        }
        return UI_INPUT_GROUP_ADDON_INLINE_START;
}

static void ui_input_group_addon_sync_align(ui_widget_t *addon,
                                            ui_input_group_addon_align_t align)
{
        ui_input_group_addon_t *data = ui_input_group_addon_get_data(addon);
        int i;

        if (!data) {
                return;
        }
        if (data->align == align &&
            ui_widget_has_status(addon, ui_input_group_addon_statuses[align])) {
                return;
        }
        for (i = 0; i < 4; ++i) {
                ui_widget_remove_status(addon,
                                        ui_input_group_addon_statuses[i]);
        }
        data->align = align;
        ui_widget_add_status(addon, ui_input_group_addon_statuses[align]);
        {
                ui_widget_t *group = ui_input_group_find_root(addon->parent);

                if (group) {
                        ui_widget_request_update(group);
                }
        }
}

static void ui_input_group_addon_on_init(ui_widget_t *w)
{
        ui_input_group_addon_t *data = ui_widget_add_data(
            w, ui_input_group_addon_prototype, sizeof(ui_input_group_addon_t));

        data->align = UI_INPUT_GROUP_ADDON_INLINE_START;
        ui_widget_add_status(w, ui_input_group_addon_statuses[data->align]);
        ui_widget_on(w, "click", ui_input_group_addon_on_click, NULL);
}

static void ui_input_group_addon_on_set_attr(ui_widget_t *w, const char *name,
                                             const char *value)
{
        if (strcmp(name, "align") == 0) {
                ui_input_group_addon_sync_align(
                    w, ui_input_group_addon_align_from_name(value));
        }
}

static void ui_input_group_toggle_status(ui_widget_t *w, const char *name,
                                         bool enable)
{
        if (enable) {
                ui_widget_add_status(w, name);
        } else {
                ui_widget_remove_status(w, name);
        }
}

static bool ui_input_group_has_disabled(ui_widget_t *w)
{
        ui_widget_t *child;
        size_t i;

        if (w->disabled || ui_widget_has_status(w, "disabled")) {
                return true;
        }
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_input_group_has_disabled(child)) {
                        return true;
                }
        }
        return false;
}

static ui_widget_t *ui_input_group_find_control(ui_widget_t *w,
                                                bool require_class)
{
        ui_widget_t *child;
        ui_widget_t *control;
        size_t i;

        if (require_class && ui_widget_has_class(w, "input-group-control")) {
                return w;
        }
        if (!require_class && ui_check_widget_type(w, "textinput")) {
                return w;
        }
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                control = ui_input_group_find_control(child, require_class);
                if (control) {
                        return control;
                }
        }
        return NULL;
}

static bool ui_input_group_is_button(ui_widget_t *target, ui_widget_t *stop)
{
        for (; target && target != stop; target = target->parent) {
                if (ui_check_widget_type(target, "button")) {
                        return true;
                }
        }
        return false;
}

static void ui_input_group_on_focus(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_add_status(w, "focused");
}

static void ui_input_group_on_blur(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_remove_status(w, "focused");
}

static void ui_input_group_on_disabled(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_request_update(w);
}

static void ui_input_group_addon_on_click(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        ui_widget_t *group;
        ui_widget_t *control;

        if (ui_input_group_is_button(e->target, w)) {
                return;
        }
        for (group = w->parent; group; group = group->parent) {
                if (ui_check_widget_prototype(group,
                                              ui_input_group_prototype)) {
                        break;
                }
        }
        if (!group) {
                return;
        }
        if (group->disabled) {
                return;
        }
        control = ui_input_group_find_control(group, true);
        if (!control) {
                control = ui_input_group_find_control(group, false);
        }
        if (!control || control->disabled) {
                return;
        }
        ui_set_focus(control);
}

static int ui_input_group_child_bucket(ui_widget_t *child)
{
        ui_input_group_addon_align_t align;

        if (!ui_check_widget_prototype(child, ui_input_group_addon_prototype)) {
                return 2;
        }
        align = ui_input_group_addon_get_align(child);
        if (align == UI_INPUT_GROUP_ADDON_BLOCK_START) {
                return 0;
        }
        if (align == UI_INPUT_GROUP_ADDON_INLINE_START) {
                return 1;
        }
        if (align == UI_INPUT_GROUP_ADDON_INLINE_END) {
                return 3;
        }
        return 4;
}

static size_t ui_input_group_child_count(ui_widget_t *w)
{
        size_t count = 0;

        while (ui_widget_get_child(w, count)) {
                ++count;
        }
        return count;
}

static bool ui_input_group_contains(ui_widget_t *w, ui_widget_t *target)
{
        for (; target; target = target->parent) {
                if (target == w) {
                        return true;
                }
        }
        return false;
}

static void ui_input_group_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_input_group_t *data = ui_input_group_get_data(w);
        ui_widget_t **children;
        ui_widget_t **start;
        ui_widget_t **end;
        size_t count;
        size_t start_count = 0;
        size_t end_count = 0;
        size_t i;
        bool block = false;
        bool multiline = false;
        bool disabled = false;
        bool start_ok = true;
        bool end_ok = true;

        if (task != UI_TASK_BEFORE_UPDATE || !data || data->ordering) {
                return;
        }
        ui_input_group_toggle_status(
            w, "focused", ui_input_group_contains(w, ui_get_focus_in(w)));
        count = ui_input_group_child_count(w);
        if (count == 0) {
                ui_input_group_toggle_status(w, "block", false);
                ui_input_group_toggle_status(w, "multiline", false);
                ui_input_group_toggle_status(w, "has-disabled", false);
                return;
        }
        children = malloc(sizeof(ui_widget_t *) * count);
        start = malloc(sizeof(ui_widget_t *) * count);
        end = malloc(sizeof(ui_widget_t *) * count);
        if (!children || !start || !end) {
                free(children);
                free(start);
                free(end);
                return;
        }
        for (i = 0; i < count; ++i) {
                children[i] = ui_widget_get_child(w, i);
                if (ui_widget_has_class(children[i], "input-group-textarea")) {
                        multiline = true;
                }
                if (ui_input_group_has_disabled(children[i])) {
                        disabled = true;
                }
                if (ui_input_group_child_bucket(children[i]) == 0 ||
                    ui_input_group_child_bucket(children[i]) == 4) {
                        block = true;
                }
        }
        for (i = 0; i < count; ++i) {
                int bucket = ui_input_group_child_bucket(children[i]);

                if (bucket == 0 || bucket == 1) {
                        start[start_count++] = children[i];
                }
        }
        for (i = 0; i < count; ++i) {
                int bucket = ui_input_group_child_bucket(children[i]);

                if (bucket == 3 || bucket == 4) {
                        end[end_count++] = children[i];
                }
        }
        /* Start addons are ordered block-start before inline-start; end
         * addons are ordered inline-end before block-end.  A single pass
         * over children preserves relative order inside each bucket. */
        if (start_count > 1) {
                size_t write = 0;

                for (int bucket = 0; bucket < 2; ++bucket) {
                        for (i = 0; i < count; ++i) {
                                if (ui_input_group_child_bucket(children[i]) ==
                                    bucket) {
                                        start[write++] = children[i];
                                }
                        }
                }
        }
        if (end_count > 1) {
                size_t write = 0;

                for (int bucket = 3; bucket < 5; ++bucket) {
                        for (i = 0; i < count; ++i) {
                                if (ui_input_group_child_bucket(children[i]) ==
                                    bucket) {
                                        end[write++] = children[i];
                                }
                        }
                }
        }
        for (i = 0; i < start_count; ++i) {
                if (children[i] != start[i]) {
                        start_ok = false;
                        break;
                }
        }
        for (i = 0; i < end_count; ++i) {
                if (children[count - end_count + i] != end[i]) {
                        end_ok = false;
                        break;
                }
        }
        ui_input_group_toggle_status(w, "block", block);
        ui_input_group_toggle_status(w, "multiline", multiline);
        ui_input_group_toggle_status(w, "has-disabled", disabled);
        if (!start_ok || !end_ok) {
                data->ordering = true;
                if (!start_ok) {
                        for (i = start_count; i > 0; --i) {
                                ui_widget_prepend(w, start[i - 1]);
                        }
                }
                if (!end_ok) {
                        for (i = 0; i < end_count; ++i) {
                                ui_widget_append(w, end[i]);
                        }
                }
                data->ordering = false;
        }
        free(children);
        free(start);
        free(end);
}

static void ui_input_group_on_init(ui_widget_t *w)
{
        ui_input_group_t *data = ui_widget_add_data(w, ui_input_group_prototype,
                                                    sizeof(ui_input_group_t));

        memset(data, 0, sizeof(*data));
        ui_widget_on(w, "focus", ui_input_group_on_focus, NULL);
        ui_widget_on(w, "blur", ui_input_group_on_blur, NULL);
        ui_widget_on(w, "disabled", ui_input_group_on_disabled, NULL);
}

ui_widget_t *ui_create_input_group(void)
{
        return ui_create_widget_with_prototype(ui_input_group_prototype);
}

ui_widget_t *ui_create_input_group_addon(void)
{
        return ui_create_widget_with_prototype(ui_input_group_addon_prototype);
}

void ui_input_group_addon_set_align(ui_widget_t *addon,
                                    ui_input_group_addon_align_t align)
{
        if (align < UI_INPUT_GROUP_ADDON_INLINE_START ||
            align > UI_INPUT_GROUP_ADDON_BLOCK_END) {
                align = UI_INPUT_GROUP_ADDON_INLINE_START;
        }
        ui_widget_set_attr(addon, "align", ui_input_group_align_names[align]);
}

ui_input_group_addon_align_t ui_input_group_addon_get_align(ui_widget_t *addon)
{
        ui_input_group_addon_t *data = ui_input_group_addon_get_data(addon);

        return data ? data->align : UI_INPUT_GROUP_ADDON_INLINE_START;
}

ui_widget_t *ui_create_input_group_text(void)
{
        return ui_create_widget_with_prototype(ui_input_group_text_prototype);
}

ui_widget_t *ui_create_input_group_kbd(void)
{
        return ui_create_widget_with_prototype(ui_input_group_kbd_prototype);
}

void ui_input_group_mark_control(ui_widget_t *control)
{
        ui_widget_add_class(control, "input-group-control");
}

ui_widget_t *ui_create_input_group_input(void)
{
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_add_class(input, "input-group-input");
        ui_input_group_mark_control(input);
        return input;
}

ui_widget_t *ui_create_input_group_textarea(void)
{
        ui_widget_t *input = ui_create_widget("textinput");

        ui_textinput_enable_multiline(input, true);
        ui_widget_add_class(input, "input-group-textarea");
        ui_input_group_mark_control(input);
        return input;
}

ui_widget_t *ui_create_input_group_button(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ui_widget_add_class(button, "input-group-button");
        return button;
}

void ui_register_input_group(void)
{
        ui_input_group_prototype =
            ui_create_widget_prototype("input-group", NULL);
        ui_input_group_addon_prototype =
            ui_create_widget_prototype("input-group-addon", NULL);
        ui_input_group_text_prototype =
            ui_create_widget_prototype("input-group-text", NULL);
        ui_input_group_kbd_prototype =
            ui_create_widget_prototype("input-group-kbd", NULL);

        ui_input_group_addon_prototype->init = ui_input_group_addon_on_init;
        ui_input_group_addon_prototype->setattr =
            ui_input_group_addon_on_set_attr;
        ui_input_group_prototype->init = ui_input_group_on_init;
        ui_input_group_prototype->update = ui_input_group_on_update;

        ui_load_css_string(ui_input_group_css, __FILE__);
}
