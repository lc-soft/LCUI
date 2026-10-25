/*
 * src/widgets/sidebar.c: -- Sidebar widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI/widgets/icon.h>
#include <LCUI/widgets/sidebar.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>

typedef struct ui_sidebar_provider {
        bool open;
} ui_sidebar_provider_t;

static ui_widget_prototype_t *ui_sidebar_provider_prototype;
static ui_widget_prototype_t *ui_sidebar_prototype;
static ui_widget_prototype_t *ui_sidebar_trigger_prototype;
static ui_widget_prototype_t *ui_sidebar_rail_prototype;
static ui_widget_prototype_t *ui_sidebar_header_prototype;
static ui_widget_prototype_t *ui_sidebar_footer_prototype;
static ui_widget_prototype_t *ui_sidebar_content_prototype;
static ui_widget_prototype_t *ui_sidebar_group_prototype;
static ui_widget_prototype_t *ui_sidebar_group_label_prototype;
static ui_widget_prototype_t *ui_sidebar_group_action_prototype;
static ui_widget_prototype_t *ui_sidebar_group_content_prototype;
static ui_widget_prototype_t *ui_sidebar_menu_prototype;
static ui_widget_prototype_t *ui_sidebar_menu_item_prototype;
static ui_widget_prototype_t *ui_sidebar_menu_action_prototype;
static ui_widget_prototype_t *ui_sidebar_menu_sub_prototype;
static ui_widget_prototype_t *ui_sidebar_menu_sub_item_prototype;
static ui_widget_prototype_t *ui_sidebar_menu_badge_prototype;

static const char *ui_sidebar_css = "\
sidebar-provider {\
  display: flex;\
  width: 100%;\
  height: 100%;\
}\
sidebar {\
  display: flex;\
  flex-direction: column;\
  position: relative;\
  flex-shrink: 0;\
  width: 240px;\
  box-sizing: border-box;\
  background: #ffffff;\
  border-right: 1px solid #e5e5e5;\
}\
sidebar:right {\
  border-right: 0;\
  border-left: 1px solid #e5e5e5;\
}\
sidebar-trigger {\
  display: inline-flex;\
  align-items: center;\
  justify-content: center;\
  width: 28px;\
  height: 28px;\
  border: 0;\
  border-radius: 6px;\
  background: transparent;\
  color: #222222;\
  cursor: pointer;\
}\
sidebar-trigger:hover {\
  background: #f5f5f5;\
}\
sidebar-rail {\
  display: block;\
  position: absolute;\
  top: 0;\
  bottom: 0;\
  right: -4px;\
  width: 8px;\
  border: 0;\
  background: transparent;\
  cursor: pointer;\
}\
sidebar-provider:collapsed sidebar:offcanvas {\
  width: 0;\
  border-left-width: 0;\
  border-right-width: 0;\
}\
sidebar-provider:collapsed sidebar:icon {\
  width: 48px;\
}\
sidebar-header, sidebar-footer {\
  display: flex;\
  flex-direction: column;\
  padding: 8px;\
}\
sidebar-content {\
  display: flex;\
  flex-direction: column;\
  flex: 1;\
  min-height: 0;\
}\
sidebar-group {\
  display: flex;\
  flex-direction: column;\
  position: relative;\
  padding: 8px;\
}\
sidebar-group-label {\
  display: flex;\
  align-items: center;\
  height: 32px;\
  padding: 0 8px;\
}\
sidebar-group-action {\
  display: inline-flex;\
  position: absolute;\
  top: 12px;\
  right: 8px;\
  align-items: center;\
  justify-content: center;\
  width: 20px;\
  height: 20px;\
  padding: 0;\
  border: 0;\
  border-radius: 4px;\
  background: transparent;\
  cursor: pointer;\
}\
sidebar-group-content {\
  display: block;\
  width: 100%;\
}\
sidebar-menu {\
  display: flex;\
  flex-direction: column;\
  gap: 4px;\
}\
sidebar-menu-item {\
  display: block;\
  position: relative;\
}\
sidebar-menu-item dropdown-menu {\
  position: absolute;\
  top: 5px;\
  right: 4px;\
  width: 20px;\
  height: 20px;\
}\
sidebar-menu-action {\
  display: inline-flex;\
  position: absolute;\
  top: 5px;\
  right: 4px;\
  align-items: center;\
  justify-content: center;\
  width: 20px;\
  height: 20px;\
  padding: 0;\
  border: 0;\
  border-radius: 4px;\
  background: transparent;\
  cursor: pointer;\
}\
.sidebar-menu-action-trigger {\
  display: inline-flex;\
  position: absolute;\
  top: 0;\
  right: 0;\
  width: 20px;\
  height: 20px;\
  opacity: 0;\
  pointer-events: none;\
}\
sidebar-menu-item:hover .sidebar-menu-action-trigger {\
  opacity: 1;\
  pointer-events: auto;\
}\
sidebar-menu-item:focus .sidebar-menu-action-trigger {\
  opacity: 1;\
  pointer-events: auto;\
}\
dropdown-menu:open .sidebar-menu-action-trigger {\
  opacity: 1;\
  pointer-events: auto;\
}\
sidebar-menu-sub {\
  display: flex;\
  flex-direction: column;\
  padding-left: 24px;\
}\
sidebar-menu-badge {\
  display: inline-flex;\
  position: absolute;\
  right: 8px;\
  top: 7px;\
}\
";

static bool ui_sidebar_value_is_true(const char *value)
{
        return value &&
               (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
}

static ui_sidebar_provider_t *ui_sidebar_provider_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_sidebar_provider_prototype);
}

static ui_widget_t *ui_sidebar_find_provider(ui_widget_t *w)
{
        for (w = w->parent; w; w = w->parent) {
                if (ui_check_widget_type(w, "sidebar-provider")) {
                        return w;
                }
        }
        return NULL;
}

static void ui_sidebar_sync_tooltip_each(ui_widget_t *w, void *arg)
{
        bool collapsed = *(bool *)arg;
        const char *text = ui_widget_get_attr(w, "data-sidebar-tooltip");
        const char *current = ui_widget_get_attr(w, "data-tooltip");

        if (!text) {
                return;
        }
        if (collapsed) {
                const char *side = ui_widget_get_attr(w, "data-tooltip-side");

                if (!current || strcmp(current, text) != 0) {
                        ui_widget_set_attr(w, "data-tooltip", text);
                }
                if (!side || strcmp(side, "right") != 0) {
                        ui_widget_set_attr(w, "data-tooltip-side", "right");
                }
        } else {
                if (current) {
                        ui_widget_set_attr(w, "data-tooltip", NULL);
                }
                if (ui_widget_get_attr(w, "data-tooltip-side")) {
                        ui_widget_set_attr(w, "data-tooltip-side", NULL);
                }
        }
}

static void ui_sidebar_sync_tooltips(ui_widget_t *w, bool collapsed)
{
        ui_widget_each(w, ui_sidebar_sync_tooltip_each, &collapsed);
}

static void ui_sidebar_provider_sync(ui_widget_t *w, bool open, bool notify)
{
        ui_sidebar_provider_t *data = ui_sidebar_provider_get_data(w);

        if (!data || data->open == open) {
                return;
        }
        data->open = open;
        if (open) {
                ui_widget_remove_status(w, "collapsed");
        } else {
                ui_widget_add_status(w, "collapsed");
        }
        ui_sidebar_sync_tooltips(w, !open);
        if (notify) {
                ui_event_t e;

                ui_event_init(&e, "open-change");
                ui_widget_emit_event(w, e, NULL);
        }
}

static void ui_sidebar_provider_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_sidebar_provider_t *data = ui_sidebar_provider_get_data(w);

        if (task == UI_TASK_BEFORE_UPDATE && data) {
                ui_sidebar_sync_tooltips(w, !data->open);
        }
}

static void ui_sidebar_provider_on_init(ui_widget_t *w)
{
        ui_sidebar_provider_t *data;

        data = ui_widget_add_data(w, ui_sidebar_provider_prototype,
                                  sizeof(ui_sidebar_provider_t));
        data->open = true;
}

static void ui_sidebar_provider_on_set_attr(ui_widget_t *w, const char *name,
                                            const char *value)
{
        if (strcmp(name, "open") == 0) {
                ui_sidebar_provider_sync(w, ui_sidebar_value_is_true(value),
                                         true);
        } else if (strcmp(name, "default-open") == 0 &&
                   !ui_widget_get_attr(w, "open")) {
                ui_sidebar_provider_sync(w, ui_sidebar_value_is_true(value),
                                         false);
        }
}

static void ui_sidebar_on_init(ui_widget_t *w)
{
        ui_widget_add_status(w, "offcanvas");
}

static void ui_sidebar_set_mode_status(ui_widget_t *w, const char *status)
{
        ui_widget_remove_status(w, "offcanvas");
        ui_widget_remove_status(w, "icon");
        ui_widget_remove_status(w, "none");
        ui_widget_add_status(w, status);
}

static void ui_sidebar_on_set_attr(ui_widget_t *w, const char *name,
                                   const char *value)
{
        if (strcmp(name, "collapsible") == 0) {
                if (value && strcmp(value, "icon") == 0) {
                        ui_sidebar_set_mode_status(w, "icon");
                } else if (value && strcmp(value, "none") == 0) {
                        ui_sidebar_set_mode_status(w, "none");
                } else {
                        ui_sidebar_set_mode_status(w, "offcanvas");
                }
        } else if (strcmp(name, "side") == 0) {
                if (value && strcmp(value, "right") == 0) {
                        ui_widget_add_status(w, "right");
                } else {
                        ui_widget_remove_status(w, "right");
                }
        }
}

static void ui_sidebar_on_toggle_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *provider = ui_sidebar_find_provider(w);

        if (!provider) {
                return;
        }
        ui_sidebar_provider_toggle(provider);
        e->cancel_bubble = true;
}

static void ui_sidebar_trigger_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "click", ui_sidebar_on_toggle_click, NULL);
        ui_widget_append(w, ui_create_fluent_icon("panel-left"));
}

static void ui_sidebar_rail_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "click", ui_sidebar_on_toggle_click, NULL);
}

static void ui_sidebar_menu_item_on_focus(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        ui_widget_add_status(w, "focus");
}

static void ui_sidebar_menu_item_on_blur(ui_widget_t *w, ui_event_t *e,
                                         void *arg)
{
        ui_widget_remove_status(w, "focus");
}

static void ui_sidebar_menu_item_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "focus", ui_sidebar_menu_item_on_focus, NULL);
        ui_widget_on(w, "blur", ui_sidebar_menu_item_on_blur, NULL);
}

static void ui_sidebar_menu_action_on_init(ui_widget_t *w)
{
        w->tab_index = 0;
}

ui_widget_t *ui_create_sidebar_provider(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_provider_prototype);
}

ui_widget_t *ui_create_sidebar(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_prototype);
}

ui_widget_t *ui_create_sidebar_trigger(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_trigger_prototype);
}

ui_widget_t *ui_create_sidebar_rail(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_rail_prototype);
}

ui_widget_t *ui_create_sidebar_header(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_header_prototype);
}

ui_widget_t *ui_create_sidebar_footer(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_footer_prototype);
}

ui_widget_t *ui_create_sidebar_content(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_content_prototype);
}

ui_widget_t *ui_create_sidebar_group(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_group_prototype);
}

ui_widget_t *ui_create_sidebar_group_label(void)
{
        return ui_create_widget_with_prototype(
            ui_sidebar_group_label_prototype);
}

ui_widget_t *ui_create_sidebar_group_action(void)
{
        return ui_create_widget_with_prototype(
            ui_sidebar_group_action_prototype);
}

ui_widget_t *ui_create_sidebar_group_content(void)
{
        return ui_create_widget_with_prototype(
            ui_sidebar_group_content_prototype);
}

ui_widget_t *ui_create_sidebar_menu(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_menu_prototype);
}

ui_widget_t *ui_create_sidebar_menu_item(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_menu_item_prototype);
}

ui_widget_t *ui_create_sidebar_menu_action(void)
{
        return ui_create_widget_with_prototype(
            ui_sidebar_menu_action_prototype);
}

ui_widget_t *ui_create_sidebar_menu_sub(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_menu_sub_prototype);
}

ui_widget_t *ui_create_sidebar_menu_sub_item(void)
{
        return ui_create_widget_with_prototype(
            ui_sidebar_menu_sub_item_prototype);
}

ui_widget_t *ui_create_sidebar_menu_badge(void)
{
        return ui_create_widget_with_prototype(ui_sidebar_menu_badge_prototype);
}

void ui_sidebar_provider_set_open(ui_widget_t *provider, bool open)
{
        ui_widget_set_attr(provider, "open", open ? "true" : "false");
}

bool ui_sidebar_provider_get_open(ui_widget_t *provider)
{
        ui_sidebar_provider_t *data = ui_sidebar_provider_get_data(provider);

        return data ? data->open : false;
}

void ui_sidebar_provider_toggle(ui_widget_t *provider)
{
        ui_sidebar_provider_set_open(provider,
                                     !ui_sidebar_provider_get_open(provider));
}

void ui_sidebar_set_collapsible(ui_widget_t *sidebar,
                                ui_sidebar_collapsible_t mode)
{
        const char *value = "offcanvas";

        if (mode == UI_SIDEBAR_COLLAPSIBLE_ICON) {
                value = "icon";
        } else if (mode == UI_SIDEBAR_COLLAPSIBLE_NONE) {
                value = "none";
        }
        ui_widget_set_attr(sidebar, "collapsible", value);
}

ui_sidebar_collapsible_t ui_sidebar_get_collapsible(ui_widget_t *sidebar)
{
        if (ui_widget_has_status(sidebar, "icon")) {
                return UI_SIDEBAR_COLLAPSIBLE_ICON;
        }
        if (ui_widget_has_status(sidebar, "none")) {
                return UI_SIDEBAR_COLLAPSIBLE_NONE;
        }
        return UI_SIDEBAR_COLLAPSIBLE_OFFCANVAS;
}

void ui_sidebar_set_side(ui_widget_t *sidebar, ui_sidebar_side_t side)
{
        ui_widget_set_attr(sidebar, "side",
                           side == UI_SIDEBAR_SIDE_RIGHT ? "right" : "left");
}

ui_sidebar_side_t ui_sidebar_get_side(ui_widget_t *sidebar)
{
        return ui_widget_has_status(sidebar, "right") ? UI_SIDEBAR_SIDE_RIGHT
                                                      : UI_SIDEBAR_SIDE_LEFT;
}

void ui_register_sidebar(void)
{
        ui_sidebar_provider_prototype =
            ui_create_widget_prototype("sidebar-provider", NULL);
        ui_sidebar_prototype = ui_create_widget_prototype("sidebar", NULL);
        ui_sidebar_trigger_prototype =
            ui_create_widget_prototype("sidebar-trigger", NULL);
        ui_sidebar_rail_prototype =
            ui_create_widget_prototype("sidebar-rail", NULL);
        ui_sidebar_header_prototype =
            ui_create_widget_prototype("sidebar-header", NULL);
        ui_sidebar_footer_prototype =
            ui_create_widget_prototype("sidebar-footer", NULL);
        ui_sidebar_content_prototype =
            ui_create_widget_prototype("sidebar-content", NULL);
        ui_sidebar_group_prototype =
            ui_create_widget_prototype("sidebar-group", NULL);
        ui_sidebar_group_label_prototype =
            ui_create_widget_prototype("sidebar-group-label", NULL);
        ui_sidebar_group_action_prototype =
            ui_create_widget_prototype("sidebar-group-action", NULL);
        ui_sidebar_group_content_prototype =
            ui_create_widget_prototype("sidebar-group-content", NULL);
        ui_sidebar_menu_prototype =
            ui_create_widget_prototype("sidebar-menu", NULL);
        ui_sidebar_menu_item_prototype =
            ui_create_widget_prototype("sidebar-menu-item", NULL);
        ui_sidebar_menu_action_prototype =
            ui_create_widget_prototype("sidebar-menu-action", NULL);
        ui_sidebar_menu_sub_prototype =
            ui_create_widget_prototype("sidebar-menu-sub", NULL);
        ui_sidebar_menu_sub_item_prototype =
            ui_create_widget_prototype("sidebar-menu-sub-item", NULL);
        ui_sidebar_menu_badge_prototype =
            ui_create_widget_prototype("sidebar-menu-badge", NULL);
        ui_sidebar_provider_prototype->init = ui_sidebar_provider_on_init;
        ui_sidebar_provider_prototype->setattr =
            ui_sidebar_provider_on_set_attr;
        ui_sidebar_provider_prototype->update = ui_sidebar_provider_on_update;
        ui_sidebar_prototype->init = ui_sidebar_on_init;
        ui_sidebar_prototype->setattr = ui_sidebar_on_set_attr;
        ui_sidebar_trigger_prototype->init = ui_sidebar_trigger_on_init;
        ui_sidebar_rail_prototype->init = ui_sidebar_rail_on_init;
        ui_sidebar_menu_item_prototype->init = ui_sidebar_menu_item_on_init;
        ui_sidebar_menu_action_prototype->init = ui_sidebar_menu_action_on_init;
        ui_load_css_string(ui_sidebar_css, __FILE__);
}
