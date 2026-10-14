/*
 * src/widgets/alert_dialog.c: -- AlertDialog widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI/widgets/alert-dialog.h>
#include <LCUI/widgets/dialog.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/base.h>
#include "dialog_internal.h"

static ui_widget_prototype_t *ui_alert_dialog_prototype;
static ui_widget_prototype_t *ui_alert_dialog_trigger_prototype;
static ui_widget_prototype_t *ui_alert_dialog_overlay_prototype;
static ui_widget_prototype_t *ui_alert_dialog_content_prototype;
static ui_widget_prototype_t *ui_alert_dialog_header_prototype;
static ui_widget_prototype_t *ui_alert_dialog_media_prototype;
static ui_widget_prototype_t *ui_alert_dialog_title_prototype;
static ui_widget_prototype_t *ui_alert_dialog_description_prototype;
static ui_widget_prototype_t *ui_alert_dialog_footer_prototype;
static ui_widget_prototype_t *ui_alert_dialog_cancel_prototype;
static ui_widget_prototype_t *ui_alert_dialog_action_prototype;

static void ui_alert_dialog_on_init(ui_widget_t *w)
{
        ui_dialog_t *data;

        ui_alert_dialog_prototype->proto->init(w);
        data = ui_dialog_get_data(w);
        data->dismissible = false;
}

static void ui_alert_dialog_content_on_init(ui_widget_t *w)
{
        ui_alert_dialog_content_prototype->proto->init(w);
}

static void ui_alert_dialog_content_on_setattr(ui_widget_t *w, const char *name,
                                               const char *value)
{
        if (strcmp(name, "size") != 0) {
                return;
        }
        ui_widget_remove_status(w, "sm");
        if (value != NULL && strcmp(value, "sm") == 0) {
                ui_widget_add_status(w, "sm");
        }
}

static void ui_alert_dialog_cancel_on_link(ui_widget_t *w, ui_event_t *e,
                                           void *arg)
{
        ui_dialog_bind_semantic_node(w, false);
}

static void ui_alert_dialog_action_on_link(ui_widget_t *w, ui_event_t *e,
                                           void *arg)
{
        ui_dialog_bind_semantic_node(w, true);
}

static void ui_alert_dialog_cancel_on_init(ui_widget_t *w)
{
        ui_alert_dialog_cancel_prototype->proto->init(w);
        w->tab_index = 0;
        ui_widget_on(w, "link", ui_alert_dialog_cancel_on_link, NULL);
}

static void ui_alert_dialog_action_on_init(ui_widget_t *w)
{
        ui_alert_dialog_action_prototype->proto->init(w);
        w->tab_index = 0;
        ui_widget_on(w, "link", ui_alert_dialog_action_on_link, NULL);
}

ui_widget_t *ui_create_alert_dialog(void)
{
        return ui_create_widget_with_prototype(ui_alert_dialog_prototype);
}

ui_widget_t *ui_create_alert_dialog_trigger(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_trigger_prototype);
}

ui_widget_t *ui_create_alert_dialog_overlay(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_overlay_prototype);
}

ui_widget_t *ui_create_alert_dialog_content(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_content_prototype);
}

ui_widget_t *ui_create_alert_dialog_header(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_header_prototype);
}

ui_widget_t *ui_create_alert_dialog_media(void)
{
        return ui_create_widget_with_prototype(ui_alert_dialog_media_prototype);
}

ui_widget_t *ui_create_alert_dialog_title(void)
{
        return ui_create_widget_with_prototype(ui_alert_dialog_title_prototype);
}

ui_widget_t *ui_create_alert_dialog_description(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_description_prototype);
}

ui_widget_t *ui_create_alert_dialog_footer(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_footer_prototype);
}

ui_widget_t *ui_create_alert_dialog_cancel(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_cancel_prototype);
}

ui_widget_t *ui_create_alert_dialog_action(void)
{
        return ui_create_widget_with_prototype(
            ui_alert_dialog_action_prototype);
}

void ui_alert_dialog_content_set_size(ui_widget_t *content, const char *size)
{
        ui_widget_set_attr(content, "size", size);
}

void ui_alert_dialog_set_size(ui_widget_t *alert, const char *size)
{
        ui_alert_dialog_content_set_size(ui_dialog_get_content(alert), size);
}

void ui_alert_dialog_set_media(ui_widget_t *alert, ui_widget_t *media)
{
        ui_widget_t *content = ui_dialog_get_content(alert);
        ui_widget_t *header, *child, *siblings[16];
        int n = 0, i;

        if (!content) {
                return;
        }
        header = ui_dialog_find_descendant(content,
                                           ui_alert_dialog_header_prototype);
        if (!header) {
                return;
        }
        for (i = 0; i < 16; ++i) {
                child = ui_widget_get_child(header, 0);
                if (!child) {
                        break;
                }
                ui_widget_unlink(child);
                siblings[n++] = child;
        }
        ui_widget_append(header, media);
        for (i = 0; i < n; ++i) {
                ui_widget_append(header, siblings[i]);
        }
}

void ui_register_alert_dialog(void)
{
        ui_alert_dialog_prototype =
            ui_create_widget_prototype("alert-dialog", "dialog");
        ui_alert_dialog_prototype->init = ui_alert_dialog_on_init;
        ui_alert_dialog_trigger_prototype = ui_create_widget_prototype(
            "alert-dialog-trigger", "dialog-trigger");
        ui_alert_dialog_overlay_prototype = ui_create_widget_prototype(
            "alert-dialog-overlay", "dialog-overlay");
        ui_alert_dialog_content_prototype = ui_create_widget_prototype(
            "alert-dialog-content", "dialog-content");
        ui_alert_dialog_content_prototype->init =
            ui_alert_dialog_content_on_init;
        ui_alert_dialog_content_prototype->setattr =
            ui_alert_dialog_content_on_setattr;
        ui_alert_dialog_header_prototype =
            ui_create_widget_prototype("alert-dialog-header", "dialog-header");
        ui_alert_dialog_media_prototype =
            ui_create_widget_prototype("alert-dialog-media", NULL);
        ui_alert_dialog_title_prototype =
            ui_create_widget_prototype("alert-dialog-title", "dialog-title");
        ui_alert_dialog_description_prototype = ui_create_widget_prototype(
            "alert-dialog-description", "dialog-description");
        ui_alert_dialog_footer_prototype =
            ui_create_widget_prototype("alert-dialog-footer", "dialog-footer");
        ui_alert_dialog_cancel_prototype =
            ui_create_widget_prototype("alert-dialog-cancel", "button");
        ui_alert_dialog_cancel_prototype->init = ui_alert_dialog_cancel_on_init;
        ui_alert_dialog_action_prototype =
            ui_create_widget_prototype("alert-dialog-action", "button");
        ui_alert_dialog_action_prototype->init = ui_alert_dialog_action_on_init;
}
