/*
 * src/widgets/toast.c: -- Toast widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/icon.h>
#include <LCUI/widgets/spinner.h>
#include <LCUI/widgets/text.h>
#include <LCUI/widgets/toast.h>
#include <ptk.h>
#include <ui/prototype.h>
#include <ui/style.h>

#define UI_TOAST_DEFAULT_DURATION 5000

typedef struct ui_toast {
        ui_widget_t *owner;
        ui_widget_t *content;
        ui_widget_t *icon_host;
        ui_widget_t *icon;
        ui_widget_t *body;
        ui_widget_t *title;
        ui_widget_t *description;
        ui_widget_t *actions;
        ui_widget_t *action;
        ui_widget_t *close;
        ui_toast_type_t type;
        ui_toast_action_cb on_action;
        void *action_arg;
        int duration;
        int timer_id;
        bool closing;
        bool has_title;
        bool has_description;
} ui_toast_t;

static ui_widget_prototype_t *ui_toast_prototype;
static ui_widget_prototype_t *ui_toast_viewport_prototype;
static ui_widget_prototype_t *ui_toast_content_prototype;
static ui_widget_prototype_t *ui_toast_icon_prototype;
static ui_widget_prototype_t *ui_toast_body_prototype;
static ui_widget_prototype_t *ui_toast_title_prototype;
static ui_widget_prototype_t *ui_toast_description_prototype;
static ui_widget_prototype_t *ui_toast_actions_prototype;

static const char *ui_toast_type_names[] = {
        "default", "success", "info", "warning", "error", "loading",
};

static const char *ui_toast_icon_names[] = {
        NULL, "checkmark-circle", "info", "warning", "error-circle", NULL,
};

static const char *ui_toast_css = "\
toast {\
  display: block;\
}\
toast-viewport {\
  position: absolute;\
  left: 0;\
  top: 0;\
  width: 100%;\
  height: 100%;\
  display: flex;\
  flex-direction: column;\
  align-items: flex-end;\
  justify-content: flex-end;\
  gap: 12px;\
  padding: 16px;\
  box-sizing: border-box;\
  pointer-events: none;\
}\
toast-content {\
  display: flex;\
  width: 100%;\
  box-sizing: border-box;\
}\
toast-icon {\
  display: flex;\
  align-items: center;\
  justify-content: center;\
  flex-shrink: 0;\
}\
toast-body {\
  display: flex;\
  flex: 1;\
  flex-direction: column;\
  min-width: 0;\
}\
toast-actions {\
  display: flex;\
  align-items: center;\
  flex-shrink: 0;\
}\
";

static void ui_toast_on_action_click(ui_widget_t *w, ui_event_t *e, void *arg);
static void ui_toast_on_close_click(ui_widget_t *w, ui_event_t *e, void *arg);
static void ui_toast_on_destroy(ui_widget_t *w, ui_event_t *e, void *arg);
static void ui_toast_on_timeout(void *arg);

static ui_toast_t *ui_toast_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_toast_prototype);
}

static ui_widget_t *ui_toast_find_viewport(void)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(ui_root(), index));
             ++index) {
                if (ui_check_widget_type(child, "toast-viewport")) {
                        return child;
                }
        }
        return NULL;
}

ui_widget_t *ui_toast_get_viewport(void)
{
        ui_widget_t *viewport = ui_toast_find_viewport();

        if (viewport) {
                return viewport;
        }
        viewport = ui_create_widget_with_prototype(ui_toast_viewport_prototype);
        ui_widget_append(ui_root(), viewport);
        return viewport;
}

static void ui_toast_update_body_visibility(ui_toast_t *data)
{
        bool visible = data->has_title || data->has_description;

        ui_widget_set_style_string(data->title, "display",
                                   data->has_title ? "block" : "none");
        ui_widget_set_style_string(data->description, "display",
                                   data->has_description ? "block" : "none");
        ui_widget_set_style_string(data->body, "display",
                                   visible ? "flex" : "none");
}

static void ui_toast_update_actions_visibility(ui_toast_t *data)
{
        bool visible = ui_widget_get_child(data->actions, 0) != NULL;

        ui_widget_set_style_string(data->actions, "display",
                                   visible ? "flex" : "none");
}

static ui_widget_t *ui_toast_create_close_button(ui_widget_t *toast)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_SM);
        ui_widget_add_class(button, "toast-close");
        ui_icon_set_name(icon, "dismiss");
        ui_icon_set_size(icon, 16);
        ui_widget_append(button, icon);
        ui_widget_on(button, "click", ui_toast_on_close_click, toast);
        return button;
}

static void ui_toast_build_children(ui_widget_t *toast, ui_toast_t *data)
{
        data->content =
            ui_create_widget_with_prototype(ui_toast_content_prototype);
        data->body = ui_create_widget_with_prototype(ui_toast_body_prototype);
        data->title = ui_create_widget_with_prototype(ui_toast_title_prototype);
        data->description =
            ui_create_widget_with_prototype(ui_toast_description_prototype);
        data->actions =
            ui_create_widget_with_prototype(ui_toast_actions_prototype);

        ui_widget_append(toast, data->content);
        ui_widget_append(data->content, data->body);
        ui_widget_append(data->body, data->title);
        ui_widget_append(data->body, data->description);
        ui_widget_append(data->content, data->actions);
        data->close = ui_toast_create_close_button(toast);
        ui_widget_append(data->content, data->close);

        ui_widget_set_style_string(data->title, "display", "none");
        ui_widget_set_style_string(data->description, "display", "none");
        ui_widget_set_style_string(data->body, "display", "none");
        ui_widget_set_style_string(data->actions, "display", "none");
}

ui_widget_t *ui_create_toast(void)
{
        ui_widget_t *toast =
            ui_create_widget_with_prototype(ui_toast_prototype);
        ui_toast_t *data =
            ui_widget_add_data(toast, ui_toast_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        data->owner = toast;
        data->type = UI_TOAST_TYPE_DEFAULT;
        data->duration = UI_TOAST_DEFAULT_DURATION;
        ui_toast_build_children(toast, data);
        ui_widget_on(toast, "destroy", ui_toast_on_destroy, data);
        return toast;
}

ui_widget_t *ui_toast_get_content(ui_widget_t *toast)
{
        return ui_toast_get_data(toast)->content;
}

ui_widget_t *ui_toast_get_actions(ui_widget_t *toast)
{
        ui_toast_t *data = ui_toast_get_data(toast);

        ui_widget_set_style_string(data->actions, "display", "flex");
        return data->actions;
}

void ui_toast_set_title(ui_widget_t *toast, const char *text)
{
        ui_toast_t *data = ui_toast_get_data(toast);

        data->has_title = text && text[0] != 0;
        ui_text_set_content(data->title, text ? text : "");
        ui_toast_update_body_visibility(data);
}

void ui_toast_set_description(ui_widget_t *toast, const char *text)
{
        ui_toast_t *data = ui_toast_get_data(toast);

        data->has_description = text && text[0] != 0;
        ui_text_set_content(data->description, text ? text : "");
        ui_toast_update_body_visibility(data);
}

void ui_toast_set_type(ui_widget_t *toast, ui_toast_type_t type)
{
        ui_toast_t *data = ui_toast_get_data(toast);
        size_t index;

        if (type < UI_TOAST_TYPE_DEFAULT || type > UI_TOAST_TYPE_LOADING) {
                type = UI_TOAST_TYPE_DEFAULT;
        }
        for (index = 1; index < sizeof(ui_toast_type_names) /
                                    sizeof(ui_toast_type_names[0]);
             ++index) {
                ui_widget_remove_status(toast, ui_toast_type_names[index]);
        }
        if (type != UI_TOAST_TYPE_DEFAULT) {
                ui_widget_add_status(toast, ui_toast_type_names[type]);
        }
        data->type = type;
        if (data->icon_host) {
                ui_widget_destroy(data->icon_host);
                data->icon_host = NULL;
                data->icon = NULL;
        }
        if (type == UI_TOAST_TYPE_DEFAULT) {
                return;
        }
        data->icon_host =
            ui_create_widget_with_prototype(ui_toast_icon_prototype);
        ui_widget_prepend(data->content, data->icon_host);
        if (type == UI_TOAST_TYPE_LOADING) {
                data->icon = ui_create_spinner();
                ui_widget_add_class(data->icon, "toast-spinner");
                ui_widget_append(data->icon_host, data->icon);
                return;
        }
        data->icon = ui_create_icon();
        ui_icon_set_name(data->icon, ui_toast_icon_names[type]);
        ui_icon_set_size(data->icon, 16);
        ui_widget_append(data->icon_host, data->icon);
}

ui_toast_type_t ui_toast_get_type(ui_widget_t *toast)
{
        return ui_toast_get_data(toast)->type;
}

static void ui_toast_on_action_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *toast = e->data;
        ui_toast_t *data = ui_toast_get_data(toast);

        if (data && data->on_action) {
                data->on_action(toast, data->action_arg);
        }
}

void ui_toast_set_action(ui_widget_t *toast, const char *text,
                         ui_toast_action_cb handler, void *arg)
{
        ui_toast_t *data = ui_toast_get_data(toast);
        ui_widget_t *actions;

        data->on_action = handler;
        data->action_arg = arg;
        if (!text || text[0] == 0) {
                if (data->action) {
                        ui_widget_destroy(data->action);
                        data->action = NULL;
                }
                ui_toast_update_actions_visibility(data);
                return;
        }
        actions = ui_toast_get_actions(toast);
        if (!data->action) {
                data->action = ui_create_button();
                ui_button_set_variant(data->action, UI_BUTTON_VARIANT_OUTLINE);
                ui_button_set_size(data->action, UI_BUTTON_SIZE_SM);
                ui_widget_add_class(data->action, "toast-action");
                ui_widget_on(data->action, "click", ui_toast_on_action_click,
                             toast);
                ui_widget_append(actions, data->action);
        }
        ui_button_set_text(data->action, text);
        ui_toast_update_actions_visibility(data);
}

static void ui_toast_cancel_timer(ui_toast_t *data)
{
        if (data->timer_id) {
                ptk_clear_timer(data->timer_id);
                data->timer_id = 0;
        }
}

static void ui_toast_on_timeout(void *arg)
{
        ui_widget_t *toast = arg;
        ui_toast_t *data = ui_toast_get_data(toast);

        if (!data) {
                return;
        }
        data->timer_id = 0;
        ui_toast_close(toast);
}

void ui_toast_set_duration(ui_widget_t *toast, int duration)
{
        ui_toast_t *data = ui_toast_get_data(toast);

        if (data->closing) {
                return;
        }
        ui_toast_cancel_timer(data);
        if (duration == UI_TOAST_DURATION_DEFAULT) {
                duration = data->type == UI_TOAST_TYPE_LOADING
                               ? UI_TOAST_DURATION_PERSISTENT
                               : UI_TOAST_DEFAULT_DURATION;
        }
        data->duration = duration;
        if (duration > 0) {
                data->timer_id =
                    ptk_set_timeout(duration, ui_toast_on_timeout, toast);
        }
}

static void ui_toast_on_close_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_toast_close(e->data);
}

void ui_toast_close(ui_widget_t *toast)
{
        ui_toast_t *data = ui_toast_get_data(toast);

        if (!data || data->closing) {
                return;
        }
        data->closing = true;
        ui_toast_cancel_timer(data);
        ui_widget_set_style_string(toast, "display", "none");
        ui_widget_remove(toast);
}

static void ui_toast_on_destroy(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_toast_cancel_timer(e->data);
}

ui_widget_t *ui_toast_show(const ui_toast_options_t *options)
{
        ui_widget_t *toast = ui_create_toast();

        ui_toast_set_type(toast, options->type);
        ui_toast_set_title(toast, options->title);
        ui_toast_set_description(toast, options->description);
        if (options->action_text) {
                ui_toast_set_action(toast, options->action_text,
                                    options->on_action, options->action_arg);
        }
        ui_widget_append(ui_toast_get_viewport(), toast);
        ui_toast_set_duration(toast, options->duration);
        return toast;
}

void ui_register_toast(void)
{
        ui_toast_prototype = ui_create_widget_prototype("toast", NULL);
        ui_toast_viewport_prototype =
            ui_create_widget_prototype("toast-viewport", NULL);
        ui_toast_content_prototype =
            ui_create_widget_prototype("toast-content", NULL);
        ui_toast_icon_prototype =
            ui_create_widget_prototype("toast-icon", NULL);
        ui_toast_body_prototype =
            ui_create_widget_prototype("toast-body", NULL);
        ui_toast_title_prototype =
            ui_create_widget_prototype("toast-title", "text");
        ui_toast_description_prototype =
            ui_create_widget_prototype("toast-description", "text");
        ui_toast_actions_prototype =
            ui_create_widget_prototype("toast-actions", NULL);
        ui_load_css_string(ui_toast_css, __FILE__);
}
