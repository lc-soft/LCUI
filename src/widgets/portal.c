/*
 * src/widgets/portal.c: -- Portal widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <LCUI/widgets/portal.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>
#include <ui/css.h>
#include <css.h>

typedef struct ui_portal {
        ui_widget_t *content;
        ui_widget_t *anchor;
        bool open;
} ui_portal_t;

typedef struct ui_portal_content_entry {
        ui_widget_t *content;
        ui_widget_t *portal;
        list_node_t node;
} ui_portal_content_entry_t;

static ui_widget_prototype_t *ui_portal_prototype;
static ui_widget_prototype_t *ui_portal_root_prototype;
static ui_widget_t *ui_portal_root;
static list_t ui_portal_contents;

static const char *ui_portal_css = "\
portal-root {\
  display: none;\
  position: absolute;\
  left: 0;\
  top: 0;\
  width: 100%;\
  height: 100%;\
  z-index: 9999;\
}\
portal-content {\
  position: absolute;\
}\
";

static ui_portal_t *ui_portal_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_portal_prototype);
}

static ui_portal_content_entry_t *ui_portal_find_content(ui_widget_t *content)
{
        list_node_t *node;

        for (list_each(node, &ui_portal_contents)) {
                ui_portal_content_entry_t *entry = node->data;

                if (entry->content == content) {
                        return entry;
                }
        }
        return NULL;
}

static void ui_portal_remove_content_entry(ui_widget_t *content)
{
        ui_portal_content_entry_t *entry;

        entry = ui_portal_find_content(content);
        if (!entry) {
                return;
        }
        list_unlink(&ui_portal_contents, &entry->node);
        free(entry);
}

static void ui_portal_content_on_destroy(ui_widget_t *content, ui_event_t *e,
                                         void *arg)
{
        ui_widget_t *portal = e->data;
        ui_portal_t *data = ui_portal_get_data(portal);

        ui_portal_remove_content_entry(content);
        if (data->content == content) {
                data->content = NULL;
        }
}

static void ui_portal_root_on_destroy(ui_widget_t *w)
{
        ui_portal_root = NULL;
}

static void ui_portal_on_outside(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_portal_close(w);
}

static ui_widget_t *ui_portal_find_owner(ui_widget_t *target)
{
        ui_portal_content_entry_t *entry;

        while (target && target != ui_portal_root) {
                entry = ui_portal_find_content(target);
                if (entry) {
                        return entry->portal;
                }
                target = target->parent;
        }
        return NULL;
}

static void ui_portal_emit_outside(void)
{
        list_node_t *node;
        ui_event_t event;

        for (list_each(node, &ui_portal_contents)) {
                ui_portal_content_entry_t *entry = node->data;
                ui_portal_t *data = ui_portal_get_data(entry->portal);

                if (!data->open) {
                        continue;
                }
                ui_event_init(&event, "portal-outside");
                ui_widget_emit_event(entry->portal, event, NULL);
        }
}

static void ui_portal_root_on_pointer_event(ui_widget_t *w, ui_event_t *e,
                                            void *arg)
{
        ui_widget_t *portal;
        ui_event_t copied;

        portal = ui_portal_find_owner(e->target);
        if (!portal) {
                ui_portal_emit_outside();
                return;
        }
        e->cancel_bubble = true;
        copied = *e;
        copied.cancel_bubble = false;
        ui_widget_emit_event(portal, copied, NULL);
}

static void ui_portal_set_offset(ui_widget_t *content, const char *name,
                                 float value)
{
        char text[32];

        snprintf(text, sizeof(text), "%gpx", value);
        ui_widget_set_style_string(content, name, text);
}

static float ui_portal_get_attr_offset(ui_widget_t *portal, const char *name)
{
        const char *value = ui_widget_get_attr(portal, name);

        return value ? (float)atof(value) : 0;
}

static void ui_portal_update_position(ui_widget_t *portal)
{
        ui_portal_t *data = ui_portal_get_data(portal);
        ui_widget_t *content = data->content;
        const char *side;
        const char *align;
        float x, y;
        float side_offset, align_offset;

        if (!content || !data->anchor) {
                return;
        }
        ui_widget_get_offset(data->anchor, ui_root(), &x, &y);
        side = ui_widget_get_attr(portal, "side");
        align = ui_widget_get_attr(portal, "align");
        side_offset = ui_portal_get_attr_offset(portal, "side-offset");
        align_offset = ui_portal_get_attr_offset(portal, "align-offset");

        if (!side) {
                side = "bottom";
        }
        if (!align) {
                align = "center";
        }
        if (strcmp(side, "bottom") == 0) {
                y += data->anchor->border_box.height + side_offset;
        } else if (strcmp(side, "top") == 0) {
                y -= content->border_box.height + side_offset;
        } else if (strcmp(side, "right") == 0) {
                x += data->anchor->border_box.width + side_offset;
        } else if (strcmp(side, "left") == 0) {
                x -= content->border_box.width + side_offset;
        }
        if (strcmp(align, "center") == 0) {
                if (strcmp(side, "left") == 0 || strcmp(side, "right") == 0) {
                        y += (data->anchor->border_box.height -
                              content->border_box.height) /
                             2;
                } else {
                        x += (data->anchor->border_box.width -
                              content->border_box.width) /
                             2;
                }
        } else if (strcmp(align, "end") == 0) {
                if (strcmp(side, "left") == 0 || strcmp(side, "right") == 0) {
                        y += data->anchor->border_box.height -
                             content->border_box.height;
                } else {
                        x += data->anchor->border_box.width -
                             content->border_box.width;
                }
        }
        if (strcmp(side, "left") == 0 || strcmp(side, "right") == 0) {
                y += align_offset;
        } else {
                x += align_offset;
        }
        ui_portal_set_offset(content, "left", x);
        ui_portal_set_offset(content, "top", y);
}

static void ui_portal_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_portal_t *data = ui_portal_get_data(w);
        ui_widget_t *child;

        if (task != UI_TASK_AFTER_UPDATE) {
                return;
        }
        if (!data->content) {
                child = ui_widget_get_child(w, 0);
                if (child) {
                        ui_portal_set_content(w, child);
                }
        }
}

static void ui_portal_on_destroy(ui_widget_t *w)
{
        ui_portal_t *data = ui_portal_get_data(w);
        ui_widget_t *content = data->content;

        if (!content) {
                return;
        }
        ui_portal_remove_content_entry(content);
        ui_widget_off(content, "destroy", ui_portal_content_on_destroy, w);
        data->content = NULL;
}

static void ui_portal_on_init(ui_widget_t *w)
{
        ui_portal_t *data;

        data = ui_widget_add_data(w, ui_portal_prototype, sizeof(*data));
        data->content = NULL;
        data->anchor = NULL;
        data->open = false;
        ui_widget_on(w, "portal-outside", ui_portal_on_outside, NULL);
}

static void ui_portal_root_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "click", ui_portal_root_on_pointer_event, NULL);
        ui_widget_on(w, "mousedown", ui_portal_root_on_pointer_event, NULL);
}

ui_widget_t *ui_get_portal_root(void)
{
        if (!ui_portal_root) {
                ui_portal_root =
                    ui_create_widget_with_prototype(ui_portal_root_prototype);
                ui_widget_add_class(ui_portal_root, "portal-root");
                ui_widget_append(ui_root(), ui_portal_root);
        }
        return ui_portal_root;
}

static void ui_portal_root_sync_visibility(void)
{
        bool visible = false;
        list_node_t *node;

        for (list_each(node, &ui_portal_contents)) {
                ui_portal_content_entry_t *entry = node->data;
                ui_portal_t *data = ui_portal_get_data(entry->portal);

                if (data->open) {
                        visible = true;
                        break;
                }
        }
        if (visible) {
                ui_widget_set_style_string(ui_portal_root, "display", "block");
        } else {
                ui_widget_set_style_string(ui_portal_root, "display", "none");
        }
}

void ui_portal_set_content(ui_widget_t *portal, ui_widget_t *content)
{
        ui_portal_t *data = ui_portal_get_data(portal);
        ui_portal_content_entry_t *entry;
        ui_widget_t *old_content = data->content;

        if (old_content == content) {
                return;
        }
        if (old_content) {
                ui_portal_remove_content_entry(old_content);
                ui_widget_off(old_content, "destroy",
                              ui_portal_content_on_destroy, portal);
                data->content = NULL;
                if (old_content->state != UI_WIDGET_STATE_DELETED) {
                        ui_widget_remove(old_content);
                }
        }
        if (!content) {
                return;
        }
        ui_widget_append(ui_get_portal_root(), content);
        data->content = content;
        if (data->open) {
                ui_widget_set_style_string(content, "display", "block");
                ui_portal_update_position(portal);
                ui_portal_root_sync_visibility();
        }
        ui_widget_on(content, "destroy", ui_portal_content_on_destroy, portal);
        entry = calloc(1, sizeof(*entry));
        entry->content = content;
        entry->portal = portal;
        entry->node.data = entry;
        list_append_node(&ui_portal_contents, &entry->node);
}

ui_widget_t *ui_portal_get_content(ui_widget_t *portal)
{
        return ui_portal_get_data(portal)->content;
}

void ui_portal_set_anchor(ui_widget_t *portal, ui_widget_t *anchor)
{
        ui_portal_get_data(portal)->anchor = anchor;
}

void ui_portal_set_side(ui_widget_t *portal, const char *side)
{
        ui_widget_set_attr(portal, "side", side);
}

void ui_portal_set_side_offset(ui_widget_t *portal, int offset)
{
        char value[32];

        snprintf(value, sizeof(value), "%d", offset);
        ui_widget_set_attr(portal, "side-offset", value);
}

void ui_portal_set_align(ui_widget_t *portal, const char *align)
{
        ui_widget_set_attr(portal, "align", align);
}

void ui_portal_set_align_offset(ui_widget_t *portal, int offset)
{
        char value[32];

        snprintf(value, sizeof(value), "%d", offset);
        ui_widget_set_attr(portal, "align-offset", value);
}

void ui_portal_open(ui_widget_t *portal)
{
        ui_portal_t *data = ui_portal_get_data(portal);

        data->open = true;
        if (data->content) {
                ui_widget_set_style_string(data->content, "display", "block");
        }
        ui_portal_root_sync_visibility();
        ui_portal_update_position(portal);
        ui_widget_request_update(portal);
        ui_widget_request_update(portal);
}

void ui_portal_close(ui_widget_t *portal)
{
        ui_portal_t *data = ui_portal_get_data(portal);

        data->open = false;
        if (data->content) {
                ui_widget_set_style_string(data->content, "display", "none");
        }
        ui_portal_root_sync_visibility();
}

ui_widget_t *ui_create_portal(void)
{
        return ui_create_widget_with_prototype(ui_portal_prototype);
}

void ui_register_portal(void)
{
        list_create(&ui_portal_contents);
        ui_portal_root_prototype =
            ui_create_widget_prototype("portal-root", NULL);
        ui_portal_root_prototype->init = ui_portal_root_on_init;
        ui_portal_root_prototype->destroy = ui_portal_root_on_destroy;
        ui_portal_prototype = ui_create_widget_prototype("portal", NULL);
        ui_portal_prototype->init = ui_portal_on_init;
        ui_portal_prototype->destroy = ui_portal_on_destroy;
        ui_portal_prototype->update = ui_portal_on_update;
        ui_load_css_string(ui_portal_css, __FILE__);
}
