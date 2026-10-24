/*
 * src/widgets/dialog.c: -- Dialog widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <yutil.h>
#include <ptk.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/alert-dialog.h>
#include <LCUI/widgets/portal.h>
#include <LCUI/widgets/text.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>
#include "dialog_internal.h"

typedef struct ui_dialog_registry_entry {
        char *key;
        ui_dialog_t *dialog;
        list_node_t node;
} ui_dialog_registry_entry_t;

static ui_widget_prototype_t *ui_dialog_prototype;
static ui_widget_prototype_t *ui_dialog_trigger_prototype;
static ui_widget_prototype_t *ui_dialog_overlay_prototype;
static ui_widget_prototype_t *ui_dialog_content_prototype;
static ui_widget_prototype_t *ui_dialog_header_prototype;
static ui_widget_prototype_t *ui_dialog_title_prototype;
static ui_widget_prototype_t *ui_dialog_description_prototype;
static ui_widget_prototype_t *ui_dialog_footer_prototype;
static list_t ui_dialog_registry;
static list_t ui_dialog_controllers;

static const char *ui_dialog_css = "\
dialog, dialog-trigger, alert-dialog, alert-dialog-trigger {\
  display: inline-block;\
}\
dialog-overlay, alert-dialog-overlay {\
  box-sizing: border-box;\
  display: none;\
  position: absolute;\
  left: 0;\
  top: 0;\
  width: 100%;\
  height: 100%;\
  padding: 16px;\
  align-items: center;\
  justify-content: center;\
}\
dialog-content, alert-dialog-content {\
  box-sizing: border-box;\
  display: flex;\
  flex-direction: column;\
  gap: 16px;\
  width: 100%;\
  max-width: 384px;\
}\
dialog-header, alert-dialog-header {\
  display: flex;\
  flex-direction: column;\
  gap: 8px;\
}\
alert-dialog-content:sm alert-dialog-header {\
  align-items: center;\
}\
dialog-title, dialog-description, alert-dialog-title, alert-dialog-description {\
  display: block;\
}\
dialog-footer, alert-dialog-footer {\
  display: flex;\
  flex-direction: row;\
  gap: 8px;\
  justify-content: flex-end;\
}\
alert-dialog-media {\
  display: flex;\
  align-items: center;\
  justify-content: center;\
  width: 40px;\
  height: 40px;\
  margin-bottom: 8px;\
}\
alert-dialog-content:sm alert-dialog-cancel,\
alert-dialog-content:sm alert-dialog-action {\
  flex: 1;\
}\
button.dialog-close,\
button.dialog-close:active {\
  position: absolute;\
  top: 8px;\
  right: 8px;\
}\
";

static void ui_dialog_bind_structure(ui_dialog_t *data);
static void ui_dialog_release_data(ui_dialog_t *data);
static void ui_dialog_on_trigger_link(ui_widget_t *w, ui_event_t *e, void *arg);
static void ui_dialog_on_overlay_link(ui_widget_t *w, ui_event_t *e, void *arg);
static void ui_dialog_on_content_link(ui_widget_t *w, ui_event_t *e, void *arg);

// helpers

ui_widget_t *ui_dialog_find_descendant(ui_widget_t *w,
                                       ui_widget_prototype_t *proto)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child, proto)) {
                        return child;
                }
                found = ui_dialog_find_descendant(child, proto);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

ui_widget_t *ui_dialog_find_owner(ui_widget_t *w)
{
        while (w) {
                if (ui_check_widget_prototype(w, ui_dialog_prototype)) {
                        return w;
                }
                w = w->parent;
        }
        return NULL;
}

static ui_widget_t *ui_dialog_find_close_node(ui_widget_t *w)
{
        ui_widget_t *child, *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_widget_has_class(child, "dialog-close")) {
                        return child;
                }
                found = ui_dialog_find_close_node(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

// registry

static ui_dialog_registry_entry_t *ui_dialog_find_registry(const char *key)
{
        list_node_t *node;

        if (!key) {
                return NULL;
        }
        for (list_each(node, &ui_dialog_registry)) {
                ui_dialog_registry_entry_t *entry = node->data;

                if (strcmp(entry->key, key) == 0) {
                        return entry;
                }
        }
        return NULL;
}

static ui_dialog_t *ui_dialog_get_loose(const char *key)
{
        ui_dialog_registry_entry_t *entry;

        if (!key) {
                return NULL;
        }
        entry = ui_dialog_find_registry(key);
        if (entry) {
                return entry->dialog;
        }
        entry = calloc(1, sizeof(*entry));
        entry->key = y_strdup(key);
        entry->dialog = calloc(1, sizeof(*entry->dialog));
        entry->dialog->loose = true;
        entry->dialog->dismissible = true;
        entry->dialog->owner = ui_root();
        entry->dialog->key = y_strdup(key);
        entry->dialog->node.data = entry->dialog;
        list_append_node(&ui_dialog_controllers, &entry->dialog->node);
        entry->node.data = entry;
        list_append_node(&ui_dialog_registry, &entry->node);
        return entry->dialog;
}

static void ui_dialog_registry_entry_destroy(void *arg)
{
        ui_dialog_registry_entry_t *entry = arg;

        ui_dialog_release_data(entry->dialog);
        free(entry->dialog->key);
        free(entry->dialog);
        free(entry->key);
        free(entry);
}

// association

static ui_dialog_t *ui_dialog_find_controller_by_overlay(ui_widget_t *overlay)
{
        list_node_t *node;

        for (list_each(node, &ui_dialog_controllers)) {
                ui_dialog_t *data = node->data;

                if (data->overlay == overlay) {
                        return data;
                }
        }
        return NULL;
}

static ui_dialog_t *ui_dialog_resolve(ui_widget_t *w, const char *attr)
{
        ui_widget_t *owner = ui_dialog_find_owner(w);
        ui_widget_t *n;
        const char *key;

        if (owner) {
                return ui_widget_get_data(owner, ui_dialog_prototype);
        }
        /* Deferred children of a transferred overlay resolve through the
         * controller that owns the mounted content root. */
        for (n = w; n && n->parent; n = n->parent) {
                if (n->parent == ui_get_portal_root()) {
                        ui_dialog_t *data =
                            ui_dialog_find_controller_by_overlay(n);
                        if (data) {
                                return data;
                        }
                        break;
                }
        }
        key = ui_widget_get_attr(w, attr);
        return ui_dialog_get_loose(key);
}

static ui_widget_t *ui_dialog_find_trigger_node(ui_widget_t *w)
{
        ui_widget_t *child;

        if (!ui_check_widget_prototype(w, ui_dialog_trigger_prototype)) {
                return w;
        }
        child = ui_widget_get_child(w, 0);
        if (!child) {
                child = ui_create_widget("button");
                child->tab_index = 0;
                ui_widget_append(w, child);
        }
        return child;
}

// focus

static void ui_dialog_focus_first_each(ui_widget_t *w, void *arg)
{
        ui_widget_t **target = arg;

        if (*target) {
                return;
        }
        if (w->tab_index >= 0 && !w->disabled) {
                *target = w;
        }
}

static void ui_dialog_on_last_focus_destroy(ui_widget_t *w, ui_event_t *e,
                                            void *unused)
{
        ui_dialog_t *data = e->data;

        if (data->last_focus == w) {
                data->last_focus = NULL;
        }
}

static void ui_dialog_focus_content(ui_dialog_t *data)
{
        ui_widget_t *target = NULL;

        ui_widget_each(data->content, ui_dialog_focus_first_each, &target);
        if (target) {
                ui_set_focus(target);
        }
}

// open and close

static void ui_dialog_close_data(ui_dialog_t *data)
{
        ui_event_t event;

        if (!data->open) {
                return;
        }
        data->open = false;
        ui_portal_close(data->portal);
        ui_widget_set_attr(data->owner, "open", "false");
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_dialog_on_last_focus_destroy, data);
                ui_set_focus(data->last_focus);
                data->last_focus = NULL;
        }
        ui_event_init(&event, "close");
        ui_widget_emit_event(data->owner, event, NULL);
}

static void ui_dialog_open_data(ui_dialog_t *data)
{
        ui_event_t event;

        if (data->open) {
                return;
        }
        if (!data->content) {
                ui_dialog_bind_structure(data);
        }
        if (!data->portal || !data->content) {
                return;
        }
        data->last_focus = ui_get_focus();
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_dialog_on_last_focus_destroy, data);
                ui_widget_on(data->last_focus, "destroy",
                             ui_dialog_on_last_focus_destroy, data);
        }
        ui_portal_open(data->portal);
        ui_widget_set_style_string(data->overlay, "display", "flex");
        data->open = true;
        ui_widget_set_attr(data->owner, "open", "true");
        ui_dialog_focus_content(data);
        ui_event_init(&event, "open");
        ui_widget_emit_event(data->owner, event, NULL);
}

static void ui_dialog_on_trigger_click(ui_widget_t *w, ui_event_t *e,
                                       void *unused)
{
        ui_dialog_t *data = e->data;
        ui_widget_t *trigger = w;

        if (trigger->parent &&
            ui_check_widget_prototype(trigger->parent,
                                      ui_dialog_trigger_prototype)) {
                trigger = trigger->parent;
        }
        if (data->open) {
                ui_dialog_close_data(data);
                return;
        }
        data->trigger = trigger;
        ui_dialog_open_data(data);
}

static void ui_dialog_bind_trigger(ui_dialog_t *data, ui_widget_t *trigger)
{
        ui_widget_t *node = ui_dialog_find_trigger_node(trigger);

        data->trigger = trigger;
        ui_widget_off(node, "click", ui_dialog_on_trigger_click, data);
        ui_widget_on(node, "click", ui_dialog_on_trigger_click, data);
}

void ui_dialog_bind_data_trigger(ui_widget_t *trigger)
{
        ui_dialog_t *data = ui_dialog_resolve(trigger, "data-dialog-trigger");

        if (data) {
                ui_dialog_bind_trigger(data, trigger);
        }
}

// dismissal

static bool ui_dialog_is_inside(ui_widget_t *target, ui_widget_t *ancestor)
{
        while (target) {
                if (target == ancestor) {
                        return true;
                }
                target = target->parent;
        }
        return false;
}

static void ui_dialog_on_overlay_click(ui_widget_t *w, ui_event_t *e,
                                       void *unused)
{
        ui_dialog_t *data = e->data;

        if (!data->dismissible || data->open == false) {
                return;
        }
        if (ui_dialog_is_inside(e->target, data->content)) {
                return;
        }
        ui_dialog_close_data(data);
}

static void ui_dialog_on_overlay_keydown(ui_widget_t *w, ui_event_t *e,
                                         void *unused)
{
        ui_dialog_t *data = e->data;

        if (data->dismissible && e->key.code == KEY_ESCAPE) {
                ui_dialog_close_data(data);
        }
}

static void ui_dialog_on_portal_outside(ui_widget_t *w, ui_event_t *e,
                                        void *unused)
{
        ui_dialog_t *data = e->data;

        if (data->dismissible) {
                ui_dialog_close_data(data);
        } else if (data->open) {
                /* The portal closes itself on outside events; keep the modal
                 * dialog open by reopening it immediately. */
                ui_portal_open(data->portal);
                ui_widget_set_style_string(data->overlay, "display", "flex");
        }
}

static void ui_dialog_on_close_click(ui_widget_t *w, ui_event_t *e,
                                     void *unused)
{
        ui_dialog_close_data(e->data);
}

static void ui_dialog_bind_close(ui_dialog_t *data, ui_widget_t *node)
{
        ui_widget_off(node, "click", ui_dialog_on_close_click, data);
        ui_widget_on(node, "click", ui_dialog_on_close_click, data);
}

typedef struct ui_dialog_scan_ctx {
        ui_dialog_t *data;
        bool scoped; /* true: inside our own overlay subtree; false: whole-tree
                      */
} ui_dialog_scan_ctx_t;

static bool ui_dialog_match_association(ui_dialog_scan_ctx_t *ctx,
                                        const char *key)
{
        ui_dialog_t *data = ctx->data;

        if (key == NULL) {
                return false;
        }
        if (data->loose) {
                return data->key != NULL && strcmp(key, data->key) == 0;
        }
        if (strcmp(key, "true") == 0) {
                return true;
        }
        /* An explicit key belongs to a loose controller, not to us. */
        return false;
}

static void ui_dialog_scan_close_each(ui_widget_t *w, void *arg)
{
        ui_dialog_scan_ctx_t *ctx = arg;

        if (w == ctx->data->owner) {
                return;
        }
        if (ctx->scoped && ui_widget_has_class(w, "dialog-close")) {
                ui_dialog_bind_close(ctx->data, w);
                return;
        }
        if (ui_dialog_match_association(
                ctx, ui_widget_get_attr(w, "data-dialog-close"))) {
                ui_dialog_bind_close(ctx->data, w);
        }
}

static void ui_dialog_bind_close_nodes(ui_dialog_t *data)
{
        ui_dialog_scan_ctx_t ctx;

        ctx.data = data;
        ctx.scoped = true;
        ui_widget_each(data->overlay, ui_dialog_scan_close_each, &ctx);
        if (data->loose) {
                ctx.scoped = false;
                ui_widget_each(ui_root(), ui_dialog_scan_close_each, &ctx);
        }
}

static void ui_dialog_on_action_click(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_dialog_t *data = e->data;
        ui_event_t event;

        ui_dialog_close_data(data);
        ui_event_init(&event, "action");
        ui_widget_emit_event(data->owner, event, NULL);
}

static void ui_dialog_on_cancel_click(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_dialog_t *data = e->data;
        ui_event_t event;

        ui_dialog_close_data(data);
        ui_event_init(&event, "cancel");
        ui_widget_emit_event(data->owner, event, NULL);
}

static void ui_dialog_bind_alert(ui_dialog_t *data, ui_widget_t *node,
                                 void (*handler)(ui_widget_t *, ui_event_t *,
                                                 void *))
{
        ui_widget_off(node, "click", handler, data);
        ui_widget_on(node, "click", handler, data);
}

static void ui_dialog_scan_alert_each(ui_widget_t *w, void *arg)
{
        ui_dialog_scan_ctx_t *ctx = arg;
        ui_dialog_t *data = ctx->data;

        if (w == data->owner) {
                return;
        }
        if (ctx->scoped && ui_check_widget_type(w, "alert-dialog-action")) {
                ui_dialog_bind_alert(data, w, ui_dialog_on_action_click);
        }
        if (ctx->scoped && ui_check_widget_type(w, "alert-dialog-cancel")) {
                ui_dialog_bind_alert(data, w, ui_dialog_on_cancel_click);
        }
        if (ui_dialog_match_association(
                ctx, ui_widget_get_attr(w, "data-dialog-action"))) {
                ui_dialog_bind_alert(data, w, ui_dialog_on_action_click);
        }
        if (ui_dialog_match_association(
                ctx, ui_widget_get_attr(w, "data-dialog-cancel"))) {
                ui_dialog_bind_alert(data, w, ui_dialog_on_cancel_click);
        }
}

static void ui_dialog_bind_alert_nodes(ui_dialog_t *data)
{
        ui_dialog_scan_ctx_t ctx;

        ctx.data = data;
        ctx.scoped = true;
        ui_widget_each(data->overlay, ui_dialog_scan_alert_each, &ctx);
        if (data->loose) {
                ctx.scoped = false;
                ui_widget_each(ui_root(), ui_dialog_scan_alert_each, &ctx);
        }
}

void ui_dialog_bind_semantic_node(ui_widget_t *w, bool is_action)
{
        ui_dialog_t *data = ui_dialog_resolve(
            w, is_action ? "data-dialog-action" : "data-dialog-cancel");

        if (!data || data->dismissible) {
                return;
        }
        ui_dialog_bind_alert(
            data, w,
            is_action ? ui_dialog_on_action_click : ui_dialog_on_cancel_click);
}

static void ui_dialog_on_content_destroy(ui_widget_t *w, ui_event_t *e,
                                         void *unused)
{
        ui_dialog_t *data = e->data;

        if (data->content == w) {
                data->content = NULL;
                if (data->open) {
                        ui_dialog_close_data(data);
                }
        }
}

static void ui_dialog_on_overlay_destroy(ui_widget_t *w, ui_event_t *e,
                                         void *unused)
{
        ui_dialog_t *data = e->data;

        if (data->overlay != w) {
                return;
        }
        data->open = false;
        data->overlay = NULL;
        data->content = NULL;
        data->trigger = NULL;
        data->portal = NULL;
        data->bound = false;
}

/* Detach every listener this controller placed on widgets that may outlive
 * it.  For widget-held controllers the portal (and with it the overlay) is
 * usually already destroyed by the time ui_dialog_on_destroy runs: the
 * overlay destroy listener clears those pointers, so NULL checks below
 * double as liveness checks.  For loose controllers the registry release
 * happens in lcui_widgets_destroy(), before the widget tree is destroyed,
 * so all referenced widgets are still alive and must be cleaned. */
static void ui_dialog_release_data(ui_dialog_t *data)
{
        if (data->overlay) {
                ui_widget_off(data->overlay, "click",
                              ui_dialog_on_overlay_click, data);
                ui_widget_off(data->overlay, "keydown",
                              ui_dialog_on_overlay_keydown, data);
                ui_widget_off(data->overlay, "destroy",
                              ui_dialog_on_overlay_destroy, data);
        }
        if (data->content && data->content != data->overlay) {
                ui_widget_off(data->content, "destroy",
                              ui_dialog_on_content_destroy, data);
        }
        if (data->portal) {
                ui_widget_off(data->portal, "portal-outside",
                              ui_dialog_on_portal_outside, data);
        }
        if (data->last_focus) {
                ui_widget_off(data->last_focus, "destroy",
                              ui_dialog_on_last_focus_destroy, data);
                data->last_focus = NULL;
        }
        data->open = false;
        list_unlink(&ui_dialog_controllers, &data->node);
}

// structure binding

static void ui_dialog_scan_trigger_each(ui_widget_t *w, void *arg)
{
        ui_dialog_scan_ctx_t *ctx = arg;
        ui_dialog_t *data = ctx->data;

        if (w == data->owner) {
                return;
        }
        if (ui_dialog_match_association(
                ctx, ui_widget_get_attr(w, "data-dialog-trigger"))) {
                ui_dialog_bind_trigger(data, w);
        }
}

static void ui_dialog_scan_triggers(ui_dialog_t *data)
{
        ui_dialog_scan_ctx_t ctx;

        ctx.data = data;
        ctx.scoped = false;
        ui_widget_each(ui_root(), ui_dialog_scan_trigger_each, &ctx);
}

static void ui_dialog_bind_overlay_data(ui_dialog_t *data, ui_widget_t *overlay)
{
        ui_widget_t *portal = overlay->parent;
        ui_widget_t *content;

        if (!portal || !ui_check_widget_type(portal, "portal")) {
                if (data->portal) {
                        portal = data->portal;
                } else {
                        portal = ui_create_portal();
                        ui_widget_append(data->owner, portal);
                }
        }
        data->overlay = overlay;
        data->portal = portal;
        data->bound = true;
        content =
            ui_dialog_find_descendant(overlay, ui_dialog_content_prototype);
        data->content = content ? content : overlay;
        ui_portal_set_content(portal, overlay);
        ui_widget_off(overlay, "click", ui_dialog_on_overlay_click, data);
        ui_widget_on(overlay, "click", ui_dialog_on_overlay_click, data);
        ui_widget_off(overlay, "keydown", ui_dialog_on_overlay_keydown, data);
        ui_widget_on(overlay, "keydown", ui_dialog_on_overlay_keydown, data);
        ui_widget_off(overlay, "destroy", ui_dialog_on_overlay_destroy, data);
        ui_widget_on(overlay, "destroy", ui_dialog_on_overlay_destroy, data);
        if (data->content != overlay) {
                ui_widget_off(data->content, "destroy",
                              ui_dialog_on_content_destroy, data);
                ui_widget_on(data->content, "destroy",
                             ui_dialog_on_content_destroy, data);
        }
        ui_widget_off(portal, "portal-outside", ui_dialog_on_portal_outside,
                      data);
        ui_widget_on(portal, "portal-outside", ui_dialog_on_portal_outside,
                     data);
        ui_dialog_bind_close_nodes(data);
        if (ui_check_widget_type(overlay, "alert-dialog-overlay")) {
                data->dismissible = false;
        }
        if (!data->dismissible) {
                ui_dialog_bind_alert_nodes(data);
        }
        if (data->loose) {
                ui_dialog_scan_triggers(data);
        }
}

void ui_dialog_bind_overlay(ui_widget_t *overlay)
{
        ui_dialog_t *data = ui_dialog_resolve(overlay, "data-dialog-target");

        if (!data) {
                return;
        }
        if (data->overlay == overlay) {
                /* Already bound; rescan so deferred close nodes rebind. */
                ui_dialog_bind_close_nodes(data);
                if (data->loose) {
                        ui_dialog_scan_triggers(data);
                }
                return;
        }
        ui_dialog_bind_overlay_data(data, overlay);
}

static void ui_dialog_bind_structure(ui_dialog_t *data)
{
        ui_widget_t *overlay;
        ui_widget_t *trigger;

        if (data->bound) {
                return;
        }
        overlay = data->overlay;
        if (!overlay) {
                overlay = ui_dialog_find_descendant(
                    data->owner, ui_dialog_overlay_prototype);
        }
        if (overlay) {
                ui_dialog_bind_overlay_data(data, overlay);
        }
        if (!data->trigger) {
                trigger = ui_dialog_find_descendant(
                    data->owner, ui_dialog_trigger_prototype);
                if (trigger) {
                        ui_dialog_bind_trigger(data, trigger);
                }
        }
}

// widgets

static void ui_dialog_on_init(ui_widget_t *w)
{
        ui_dialog_t *data =
            ui_widget_add_data(w, ui_dialog_prototype, sizeof(*data));

        memset(data, 0, sizeof(*data));
        data->owner = w;
        data->dismissible = true;
        data->close_button = true;
        data->node.data = data;
        list_append_node(&ui_dialog_controllers, &data->node);
}

static void ui_dialog_on_update(ui_widget_t *w, ui_task_type_t task)
{
        if (task != UI_TASK_BEFORE_UPDATE) {
                return;
        }
        ui_dialog_bind_structure(ui_dialog_get_data(w));
}

static void ui_dialog_on_destroy(ui_widget_t *w)
{
        ui_dialog_release_data(ui_dialog_get_data(w));
}

static void ui_dialog_trigger_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_dialog_on_trigger_link, NULL);
}

static void ui_dialog_on_trigger_link(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_dialog_t *data = ui_dialog_resolve(w, "data-dialog-trigger");

        if (data) {
                ui_dialog_bind_trigger(data, w);
        }
}

static void ui_dialog_overlay_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_dialog_on_overlay_link, NULL);
}

static void ui_dialog_on_overlay_link(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_dialog_bind_overlay(w);
}

static void ui_dialog_content_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_dialog_on_content_link, NULL);
}

static void ui_dialog_on_content_link(ui_widget_t *w, ui_event_t *e,
                                      void *unused)
{
        ui_widget_t *owner = ui_dialog_find_owner(w);
        ui_dialog_t *data;

        if (!owner) {
                return;
        }
        data = ui_widget_get_data(owner, ui_dialog_prototype);
        data->content = w;
}

static void ui_dialog_title_on_init(ui_widget_t *w)
{
        /* Chain via the static prototype pointer, not w->proto, so that
         * subtypes (e.g. alert-dialog-title) do not re-enter this init. */
        ui_dialog_title_prototype->proto->init(w);
}

static void ui_dialog_description_on_init(ui_widget_t *w)
{
        ui_dialog_description_prototype->proto->init(w);
}

// public API

ui_widget_t *ui_create_dialog(void)
{
        return ui_create_widget_with_prototype(ui_dialog_prototype);
}

ui_dialog_t *ui_dialog_get_data(ui_widget_t *dialog)
{
        return ui_widget_get_data(dialog, ui_dialog_prototype);
}

static void ui_dialog_ensure_default_content(ui_dialog_t *data)
{
        ui_widget_t *content = data->content;
        ui_widget_t *header;
        ui_widget_t *footer;
        ui_widget_t *close;
        ui_widget_t *title;
        ui_widget_t *description;
        ui_widget_t *button;

        if (!content || content == data->overlay) {
                return;
        }
        header = ui_dialog_find_descendant(content, ui_dialog_header_prototype);
        if (!header) {
                header = data->dismissible ? ui_create_dialog_header()
                                           : ui_create_alert_dialog_header();
                title = data->dismissible ? ui_create_dialog_title()
                                          : ui_create_alert_dialog_title();
                description = data->dismissible
                                  ? ui_create_dialog_description()
                                  : ui_create_alert_dialog_description();
                ui_text_set_content(title, data->dismissible
                                               ? "Untitled dialog"
                                               : "Are you absolutely sure?");
                ui_text_set_content(
                    description,
                    data->dismissible
                        ? "Set the purpose of the dialog here."
                        : "This action cannot be undone. This will "
                          "permanently delete your account and remove your "
                          "data from our servers.");
                ui_widget_append(header, title);
                ui_widget_append(header, description);
                ui_widget_append(content, header);
        }
        footer = ui_dialog_find_descendant(content, ui_dialog_footer_prototype);
        if (!footer) {
                footer = data->dismissible ? ui_create_dialog_footer()
                                           : ui_create_alert_dialog_footer();
                if (data->dismissible) {
                        button = ui_create_widget("button");
                        ui_widget_set_attr(button, "variant", "outline");
                        ui_widget_set_attr(button, "data-dialog-close", "true");
                        ui_button_set_text(button, "Cancel");
                        ui_widget_append(footer, button);
                        button = ui_create_widget("button");
                        ui_widget_set_attr(button, "variant", "primary");
                        ui_button_set_text(button, "Confirm");
                        ui_widget_append(footer, button);
                } else {
                        button = ui_create_alert_dialog_cancel();
                        ui_widget_set_attr(button, "variant", "outline");
                        ui_button_set_text(button, "Cancel");
                        ui_widget_append(footer, button);
                        button = ui_create_alert_dialog_action();
                        ui_widget_set_attr(button, "variant", "primary");
                        ui_button_set_text(button, "Continue");
                        ui_widget_append(footer, button);
                }
                ui_widget_append(content, footer);
        }
        if (data->dismissible && data->close_button) {
                close = ui_dialog_find_close_node(content);
                if (!close) {
                        ui_widget_append(content, ui_create_dialog_close());
                }
        }
}

void ui_dialog_build_children(ui_widget_t *dialog)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);
        ui_widget_t *portal;
        ui_widget_t *overlay;
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *child;
        size_t i;

        trigger =
            ui_dialog_find_descendant(dialog, ui_dialog_trigger_prototype);
        if (!trigger) {
                trigger = data->dismissible ? ui_create_dialog_trigger()
                                            : ui_create_alert_dialog_trigger();
                ui_widget_append(dialog, trigger);
        }
        overlay = data->overlay;
        if (!overlay) {
                overlay = ui_dialog_find_descendant(
                    dialog, ui_dialog_overlay_prototype);
        }
        if (!overlay) {
                portal = NULL;
                for (i = 0; (child = ui_widget_get_child(dialog, i)); ++i) {
                        if (ui_check_widget_type(child, "portal")) {
                                portal = child;
                                break;
                        }
                }
                if (!portal) {
                        portal = ui_create_portal();
                        ui_widget_append(dialog, portal);
                }
                overlay = data->dismissible ? ui_create_dialog_overlay()
                                            : ui_create_alert_dialog_overlay();
                content = data->dismissible ? ui_create_dialog_content()
                                            : ui_create_alert_dialog_content();
                ui_widget_append(overlay, content);
                ui_widget_append(portal, overlay);
        } else if (!ui_dialog_find_descendant(overlay,
                                              ui_dialog_content_prototype)) {
                content = data->dismissible ? ui_create_dialog_content()
                                            : ui_create_alert_dialog_content();
                ui_widget_append(overlay, content);
        }
        ui_dialog_bind_trigger(data, trigger);
        ui_dialog_bind_overlay_data(data, overlay);
        ui_dialog_ensure_default_content(data);
        ui_dialog_bind_close_nodes(data);
        if (!data->dismissible) {
                ui_dialog_bind_alert_nodes(data);
        }
}

void ui_dialog_set_open(ui_widget_t *dialog, int open)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);

        ui_dialog_bind_structure(data);
        if (open) {
                ui_dialog_open_data(data);
        } else {
                ui_dialog_close_data(data);
        }
}

void ui_dialog_open(ui_widget_t *dialog)
{
        ui_dialog_set_open(dialog, 1);
}

void ui_dialog_close(ui_widget_t *dialog)
{
        ui_dialog_set_open(dialog, 0);
}

int ui_dialog_is_open(ui_widget_t *dialog)
{
        return ui_dialog_get_data(dialog)->open;
}

void ui_dialog_show_close_button(ui_widget_t *dialog, bool show)
{
        ui_dialog_get_data(dialog)->close_button = show;
}

bool ui_dialog_shows_close_button(ui_widget_t *dialog)
{
        return ui_dialog_get_data(dialog)->close_button;
}

static void ui_dialog_set_slot_text(ui_widget_t *dialog,
                                    ui_widget_prototype_t *proto,
                                    const char *text)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);
        ui_widget_t *node;

        if (!data->bound) {
                ui_dialog_bind_structure(data);
        }
        if (!data->content) {
                return;
        }
        node = ui_dialog_find_descendant(data->content, proto);
        if (node) {
                ui_text_set_content(node, text);
        }
}

void ui_dialog_set_title(ui_widget_t *dialog, const char *text)
{
        ui_dialog_set_slot_text(dialog, ui_dialog_title_prototype, text);
}

void ui_dialog_set_description(ui_widget_t *dialog, const char *text)
{
        ui_dialog_set_slot_text(dialog, ui_dialog_description_prototype, text);
}

void ui_dialog_set_trigger_text(ui_widget_t *dialog, const char *text)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);
        ui_widget_t *trigger = data->trigger;

        if (!data->bound) {
                ui_dialog_bind_structure(data);
                trigger = data->trigger;
        }
        if (!trigger) {
                trigger = ui_dialog_find_descendant(
                    data->owner, ui_dialog_trigger_prototype);
                if (trigger) {
                        ui_dialog_bind_trigger(data, trigger);
                }
        }
        if (trigger) {
                ui_button_set_text(ui_dialog_find_trigger_node(trigger), text);
        }
}

static ui_widget_t *ui_dialog_footer_slot(ui_widget_t *dialog, bool last)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);
        ui_widget_t *footer, *child, *found;

        if (!data->bound) {
                ui_dialog_bind_structure(data);
        }
        if (!data->content) {
                return NULL;
        }
        footer = ui_dialog_find_descendant(data->content,
                                           ui_dialog_footer_prototype);
        if (!footer) {
                return NULL;
        }
        found = NULL;
        for (size_t i = 0; (child = ui_widget_get_child(footer, i)); ++i) {
                found = child;
                if (!last && i == 0) {
                        break;
                }
        }
        return found;
}

void ui_dialog_set_cancel_text(ui_widget_t *dialog, const char *text)
{
        ui_widget_t *node = ui_dialog_footer_slot(dialog, false);

        if (node) {
                ui_button_set_text(node, text);
        }
}

void ui_dialog_set_action_text(ui_widget_t *dialog, const char *text)
{
        ui_widget_t *node = ui_dialog_footer_slot(dialog, true);

        if (node) {
                ui_button_set_text(node, text);
        }
}

ui_widget_t *ui_dialog_get_footer_button(ui_widget_t *dialog, bool is_action)
{
        return ui_dialog_footer_slot(dialog, is_action);
}

void ui_dialog_append_body(ui_widget_t *dialog, ui_widget_t *widget)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);
        ui_widget_t *footer, *close;

        if (!data->bound) {
                ui_dialog_bind_structure(data);
        }
        if (!data->content) {
                return;
        }
        footer = ui_dialog_find_descendant(data->content,
                                           ui_dialog_footer_prototype);
        close = ui_dialog_find_close_node(data->content);
        if (footer) {
                ui_widget_unlink(footer);
        }
        if (close) {
                ui_widget_unlink(close);
        }
        ui_widget_append(data->content, widget);
        if (footer) {
                ui_widget_append(data->content, footer);
        }
        if (close) {
                ui_widget_append(data->content, close);
        }
}

ui_widget_t *ui_dialog_get_content(ui_widget_t *dialog)
{
        ui_dialog_t *data = ui_dialog_get_data(dialog);

        if (!data->bound) {
                ui_dialog_bind_structure(data);
        }
        return data->content;
}

ui_widget_t *ui_create_dialog_trigger(void)
{
        return ui_create_widget_with_prototype(ui_dialog_trigger_prototype);
}

ui_widget_t *ui_create_dialog_overlay(void)
{
        return ui_create_widget_with_prototype(ui_dialog_overlay_prototype);
}

ui_widget_t *ui_create_dialog_content(void)
{
        return ui_create_widget_with_prototype(ui_dialog_content_prototype);
}

ui_widget_t *ui_create_dialog_header(void)
{
        return ui_create_widget_with_prototype(ui_dialog_header_prototype);
}

ui_widget_t *ui_create_dialog_title(void)
{
        return ui_create_widget_with_prototype(ui_dialog_title_prototype);
}

ui_widget_t *ui_create_dialog_description(void)
{
        return ui_create_widget_with_prototype(ui_dialog_description_prototype);
}

ui_widget_t *ui_create_dialog_footer(void)
{
        return ui_create_widget_with_prototype(ui_dialog_footer_prototype);
}

static void ui_dialog_on_factory_close_link(ui_widget_t *w, ui_event_t *e,
                                            void *arg)
{
        ui_dialog_t *data = ui_dialog_resolve(w, "data-dialog-close");

        if (data) {
                ui_dialog_bind_close(data, w);
        }
}

ui_widget_t *ui_create_dialog_close(void)
{
        ui_widget_t *close = ui_create_widget("button");
        ui_widget_t *icon = ui_create_widget("icon");

        ui_widget_add_class(close, "dialog-close");
        ui_widget_set_attr(close, "variant", "ghost");
        ui_widget_set_attr(close, "size", "icon-xs");
        ui_widget_set_attr(close, "data-dialog-close", "true");
        close->tab_index = 0;
        ui_widget_on(close, "link", ui_dialog_on_factory_close_link, NULL);
        ui_widget_set_attr(icon, "name", "dismiss");
        ui_widget_append(close, icon);
        return close;
}

void ui_register_dialog(void)
{
        list_create(&ui_dialog_registry);
        list_create(&ui_dialog_controllers);
        ui_dialog_prototype = ui_create_widget_prototype("dialog", NULL);
        ui_dialog_prototype->init = ui_dialog_on_init;
        ui_dialog_prototype->update = ui_dialog_on_update;
        ui_dialog_prototype->destroy = ui_dialog_on_destroy;
        ui_dialog_trigger_prototype =
            ui_create_widget_prototype("dialog-trigger", NULL);
        ui_dialog_trigger_prototype->init = ui_dialog_trigger_on_init;
        ui_dialog_overlay_prototype =
            ui_create_widget_prototype("dialog-overlay", NULL);
        ui_dialog_overlay_prototype->init = ui_dialog_overlay_on_init;
        ui_dialog_content_prototype =
            ui_create_widget_prototype("dialog-content", NULL);
        ui_dialog_content_prototype->init = ui_dialog_content_on_init;
        ui_dialog_header_prototype =
            ui_create_widget_prototype("dialog-header", NULL);
        ui_dialog_title_prototype =
            ui_create_widget_prototype("dialog-title", "text");
        ui_dialog_title_prototype->init = ui_dialog_title_on_init;
        ui_dialog_description_prototype =
            ui_create_widget_prototype("dialog-description", "text");
        ui_dialog_description_prototype->init = ui_dialog_description_on_init;
        ui_dialog_footer_prototype =
            ui_create_widget_prototype("dialog-footer", NULL);
        ui_load_css_string(ui_dialog_css, __FILE__);
}

void ui_unregister_dialog(void)
{
        list_destroy_without_node(&ui_dialog_registry,
                                  ui_dialog_registry_entry_destroy);
}
