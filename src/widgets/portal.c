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
#include <LCUI/widgets/scrollarea.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>
#include <ui/css.h>
#include <css.h>

typedef struct ui_portal {
        ui_widget_t *content;
        ui_widget_t *anchor;
        ui_widget_t *owner;
        ui_widget_t **scroll_parents;
        float *scroll_tops;
        float *scroll_lefts;
        size_t scroll_parent_count;
        float content_x;
        float content_y;
        bool open;
        bool passthrough;
        bool outside_close;
        bool follow_anchor;
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

static void ui_portal_root_sync_visibility(void);

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
                data->open = false;
        }
        ui_portal_root_sync_visibility();
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

static bool ui_portal_is_inside(ui_portal_t *data, ui_widget_t *target)
{
        for (; target; target = target->parent) {
                if (target == data->content || target == data->anchor) {
                        return true;
                }
        }
        return false;
}

static void ui_portal_emit_outside_except(ui_widget_t *owner_portal)
{
        list_node_t *node;
        ui_event_t event;

        for (list_each(node, &ui_portal_contents)) {
                ui_portal_content_entry_t *entry = node->data;
                ui_portal_t *data = ui_portal_get_data(entry->portal);

                if (entry->portal == owner_portal || !data->open ||
                    !data->outside_close) {
                        continue;
                }
                ui_event_init(&event, "portal-outside");
                ui_widget_emit_event(entry->portal, event, NULL);
        }
}

static void ui_portal_root_on_outside_input(ui_widget_t *w, ui_event_t *e,
                                            void *arg)
{
        list_node_t *node;
        ui_event_t event;

        if (ui_portal_root && e->target == ui_portal_root) {
                return;
        }
        for (list_each(node, &ui_portal_contents)) {
                ui_portal_content_entry_t *entry = node->data;
                ui_portal_t *data = ui_portal_get_data(entry->portal);

                if (!data->open || !data->outside_close ||
                    ui_portal_is_inside(data, e->target)) {
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
        ui_portal_emit_outside_except(portal);
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
        data->content_x = x;
        data->content_y = y;
        ui_portal_set_offset(content, "left", x);
        ui_portal_set_offset(content, "top", y);
}

static void ui_portal_on_anchor_scroll(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *portal = e->data;
        ui_portal_t *data = ui_portal_get_data(portal);
        float left, top, dx = 0, dy = 0;
        size_t i;

        if (!data->open || !data->content) {
                return;
        }
        for (i = 0; i < data->scroll_parent_count; ++i) {
                top = ui_scrollarea_get_scroll_top(data->scroll_parents[i]);
                left = ui_scrollarea_get_scroll_left(data->scroll_parents[i]);
                dx += left - data->scroll_lefts[i];
                dy += top - data->scroll_tops[i];
                data->scroll_tops[i] = top;
                data->scroll_lefts[i] = left;
        }
        if (dx == 0 && dy == 0) {
                return;
        }
        data->content_x -= dx;
        data->content_y -= dy;
        ui_portal_set_offset(data->content, "left", data->content_x);
        ui_portal_set_offset(data->content, "top", data->content_y);
        ui_widget_request_update(portal);
}

static void ui_portal_clear_scroll_parents(ui_portal_t *data)
{
        size_t i;

        for (i = 0; i < data->scroll_parent_count; ++i) {
                ui_widget_t *w = data->scroll_parents[i];

                if (w && w->state != UI_WIDGET_STATE_DELETED) {
                        ui_widget_off(w, "scroll", ui_portal_on_anchor_scroll,
                                      data->owner);
                }
        }
        free(data->scroll_parents);
        free(data->scroll_tops);
        free(data->scroll_lefts);
        data->scroll_parents = NULL;
        data->scroll_tops = NULL;
        data->scroll_lefts = NULL;
        data->scroll_parent_count = 0;
}

static void ui_portal_bind_scroll_parents(ui_portal_t *data)
{
        ui_widget_t *w;
        size_t count = 0;
        size_t i = 0;

        if (!data->anchor || data->scroll_parents) {
                return;
        }
        for (w = data->anchor->parent; w; w = w->parent) {
                if (ui_check_widget_type(w, "scrollarea")) {
                        ++count;
                }
        }
        if (count == 0) {
                return;
        }
        data->scroll_parents = calloc(count, sizeof(ui_widget_t *));
        data->scroll_tops = calloc(count, sizeof(float));
        data->scroll_lefts = calloc(count, sizeof(float));
        if (!data->scroll_parents || !data->scroll_tops ||
            !data->scroll_lefts) {
                free(data->scroll_parents);
                free(data->scroll_tops);
                free(data->scroll_lefts);
                data->scroll_parents = NULL;
                data->scroll_tops = NULL;
                data->scroll_lefts = NULL;
                return;
        }
        for (w = data->anchor->parent; w; w = w->parent) {
                if (ui_check_widget_type(w, "scrollarea")) {
                        data->scroll_parents[i] = w;
                        data->scroll_tops[i] = ui_scrollarea_get_scroll_top(w);
                        data->scroll_lefts[i] =
                            ui_scrollarea_get_scroll_left(w);
                        ++i;
                }
        }
        data->scroll_parent_count = count;
        for (i = 0; i < count; ++i) {
                ui_widget_on(data->scroll_parents[i], "scroll",
                             ui_portal_on_anchor_scroll, data->owner);
        }
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

        ui_portal_clear_scroll_parents(data);
        if (content) {
                ui_portal_remove_content_entry(content);
                ui_widget_off(content, "destroy", ui_portal_content_on_destroy,
                              w);
                data->content = NULL;
                if (content->state != UI_WIDGET_STATE_DELETED) {
                        /* Destroy immediately instead of ui_widget_remove():
                         * the trash would only be processed after prototypes
                         * are torn down at ui_destroy, leaving a dangling
                         * proto. */
                        ui_widget_destroy(content);
                }
        }
        data->open = false;
        ui_portal_root_sync_visibility();
}

static void ui_portal_on_init(ui_widget_t *w)
{
        ui_portal_t *data;

        data = ui_widget_add_data(w, ui_portal_prototype, sizeof(*data));
        data->owner = w;
        data->content = NULL;
        data->passthrough = false;
        data->outside_close = false;
        data->follow_anchor = false;
        data->anchor = NULL;
        data->scroll_parents = NULL;
        data->scroll_tops = NULL;
        data->scroll_lefts = NULL;
        data->scroll_parent_count = 0;
        data->content_x = 0;
        data->content_y = 0;
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
        bool capture = false;
        list_node_t *node;

        if (!ui_portal_root) {
                return;
        }

        for (list_each(node, &ui_portal_contents)) {
                ui_portal_content_entry_t *entry = node->data;
                ui_portal_t *data = ui_portal_get_data(entry->portal);

                if (data->open) {
                        visible = true;
                        if (!data->passthrough) {
                                capture = true;
                        }
                }
        }
        if (visible) {
                ui_widget_set_style_string(ui_portal_root, "display", "block");
                ui_widget_set_style_string(ui_portal_root, "pointer-events",
                                           capture ? "auto" : "none");
        } else {
                ui_widget_set_style_string(ui_portal_root, "display", "none");
        }
}

void ui_portal_set_passthrough(ui_widget_t *portal, bool passthrough)
{
        ui_portal_t *data = ui_portal_get_data(portal);

        data->passthrough = passthrough;
        ui_portal_root_sync_visibility();
}

void ui_portal_set_outside_close(ui_widget_t *portal, bool enabled)
{
        ui_portal_get_data(portal)->outside_close = enabled;
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
        ui_widget_set_style_string(content, "display",
                                   data->open ? "block" : "none");
        if (data->open) {
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
        ui_portal_t *data = ui_portal_get_data(portal);

        ui_portal_clear_scroll_parents(data);
        data->anchor = anchor;
        if (data->follow_anchor) {
                ui_portal_bind_scroll_parents(data);
        }
}

void ui_portal_set_follow_anchor(ui_widget_t *portal, bool follow)
{
        ui_portal_t *data = ui_portal_get_data(portal);

        data->follow_anchor = follow;
        ui_portal_clear_scroll_parents(data);
        if (follow) {
                ui_portal_bind_scroll_parents(data);
        }
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
        ui_widget_on(ui_root(), "click", ui_portal_root_on_outside_input, NULL);
}
