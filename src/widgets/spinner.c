/*
 * src/widgets/spinner.c: -- LCUI's Spinner widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <css.h>
#include <ui.h>
#include <ui/events.h>
#include <ui/mutation_observer.h>
#include <ui/style.h>
#include <LCUI/app.h>
#include <LCUI/widgets/spinner.h>
#include "draw_backend.h"
#include "spinner_priv.h"

#define UI_SPINNER_PI 3.14159265358979323846
/* Alpha fraction of the ring's gap side relative to the other sides (0..1): a
 * faint, not absent, segment so the mass stays balanced about the centre. */
#define UI_SPINNER_GAP_ALPHA 0.2f

typedef struct ui_spinner {
        bool running;
        float speed;
        double angle;
        int frame_id;
        uint64_t last_ts;
        bool has_ts;
        bool ring_color_set;
        css_color_value_t ring_color;
        ui_draw_layer_t *layer;
        ui_draw_surface_t *content_surface;
        /* The captured content is expensive to rebuild (a full subtree render)
         * but only changes when the content itself does; the rotation changes
         * every animation frame. Keep the capture cached until something marks
         * the content dirty (resize, set_content, ring colour, or a subtree
         * mutation seen by the observer). */
        bool content_dirty;
        ui_mutation_observer_t *observer;
        ui_widget_t *content;
        ui_widget_t *default_content;
        ui_widget_t *ring;
} ui_spinner_t;

static ui_widget_prototype_t *ui_spinner_prototype;
static ui_widget_prototype_t *ui_spinner_content_prototype;

static const char *ui_spinner_css = "\
spinner {\
  display: inline-block;\
  width: 16px;\
  height: 16px;\
  color: #6b7f8f;\
}\
spinner-content {\
  position: absolute;\
  box-sizing: border-box;\
  left: -100%;\
  top: -100%;\
  width: 100%;\
  height: 100%;\
}\
spinner .spinner-ring {\
  box-sizing: border-box;\
  position: absolute;\
  left: 2px;\
  top: 2px;\
  right: 2px;\
  bottom: 2px;\
  border-style: solid;\
  border-width: 2px;\
  border-color: #6b7f8f;\
  border-top-color: rgba(107, 127, 143, 0.2);\
  border-radius: 9999px;\
}\
";

static ui_spinner_t *ui_spinner_get_data(ui_widget_t *w);
static void ui_spinner_on_frame(uint64_t timestamp, void *data);
static void ui_spinner_size_content(ui_widget_t *content);
static void ui_spinner_reconcile_frame(ui_widget_t *w);
static void ui_spinner_attach_content(ui_widget_t *content);
static ui_widget_t *ui_spinner_build_default_content(ui_widget_t *w);
static void ui_spinner_invalidate_content(ui_widget_t *w);
static void ui_spinner_observe(ui_widget_t *w);

// object 1: content binding

static void ui_spinner_attach_content(ui_widget_t *content)
{
        ui_widget_t *owner = content->parent;
        ui_spinner_t *spinner;

        while (owner && owner->proto != ui_spinner_prototype) {
                owner = owner->parent;
        }
        if (!owner) {
                return;
        }
        spinner = ui_spinner_get_data(owner);
        if (!spinner || spinner->content == content) {
                return;
        }
        /* When a real content node links in, drop the auto-created default so
         * the spinner keeps exactly the one subtree it will capture. */
        if (spinner->default_content == spinner->content) {
                ui_widget_destroy(spinner->default_content);
                spinner->default_content = NULL;
                spinner->ring = NULL;
        }
        spinner->content = content;
        ui_spinner_invalidate_content(owner);
}

static ui_widget_t *ui_spinner_build_default_content(ui_widget_t *w)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        ui_widget_t *content, *ring;

        content = ui_create_widget_with_prototype(ui_spinner_content_prototype);
        ring = ui_create_widget(NULL);
        ui_widget_add_class(ring, "spinner-ring");
        ui_widget_append(content, ring);
        /* Pre-set the pointers so the link callback fired by append() treats
         * this content as already tracked and does not destroy it. The ring
         * pointer lets on_update write the inherited `color` into the stroke;
         * custom content is not colour-managed. */
        spinner->content = content;
        spinner->default_content = content;
        spinner->ring = ring;
        ui_widget_append(w, content);
        return content;
}

static void ui_spinner_content_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_spinner_attach_content(w);
}

static void ui_spinner_content_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_spinner_content_on_link, NULL);
}

// object 2: surface and frame lifecycle

/* Mark the cached content capture stale so the next full render rebinds the
 * subtree. Every change that can alter what the capture looks like must go
 * through here (or through a subtree mutation the observer sees), otherwise
 * the spinner keeps rotating a stale bitmap. */
static void ui_spinner_invalidate_content(ui_widget_t *w)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);

        if (!spinner) {
                return;
        }
        spinner->content_dirty = true;
        ui_draw_layer_invalidate(spinner->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

static void ui_spinner_on_mutation(ui_mutation_list_t *records,
                                   ui_mutation_observer_t *observer, void *arg)
{
        ui_spinner_invalidate_content((ui_widget_t *)arg);
}

/* Watch the widget and its whole subtree so edits made to custom content
 * (through the style, attribute or child-list APIs) invalidate the cache the
 * same way the spinner's own hooks do. The observation targets the spinner
 * itself, not the content node: widgets are destroyed children-first, so an
 * observer connected to a child would still hold a dangling pointer when the
 * spinner tears it down (and connecting to an ancestor covers a replacement
 * content too). */
static void ui_spinner_observe(ui_widget_t *w)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        ui_mutation_observer_init_t options = {
                .child_list = true,
                .subtree = true,
                .properties = true,
                .attributes = true,
        };

        if (!spinner || spinner->observer) {
                return;
        }
        spinner->observer =
            ui_mutation_observer_create(ui_spinner_on_mutation, w);
        if (spinner->observer) {
                ui_mutation_observer_observe(spinner->observer, w, options);
        }
}

static void ui_spinner_size_content(ui_widget_t *content)
{
        if (!content) {
                return;
        }
        /* The content fills the spinner box and is pushed offscreen so the
         * normal window render culls it while ui_draw_capture still records it.
         * A percentage box is resolved from the parent on every layout pass, so
         * it tracks resize and DPI changes without lagging a frame behind. */
        ui_widget_set_style_string(content, "position", "absolute");
        ui_widget_set_style_string(content, "box-sizing", "border-box");
        ui_widget_set_style_string(content, "left", "-100%");
        ui_widget_set_style_string(content, "top", "-100%");
        ui_widget_set_style_string(content, "width", "100%");
        ui_widget_set_style_string(content, "height", "100%");
}

/* Reconcile the one-shot frame request against the desired state: the
 * animation runs only while the widget is both running and visible. on_frame
 * re-arms the request itself while running, so this only boots the first
 * frame or cancels a still-pending request when the widget hides. */
static void ui_spinner_reconcile_frame(ui_widget_t *w)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        bool want = spinner->running && ui_widget_is_visible(w);

        if (want && spinner->frame_id < 0) {
                spinner->has_ts = false;
                spinner->frame_id = lcui_request_frame(ui_spinner_on_frame, w);
        } else if (!want && spinner->frame_id >= 0) {
                lcui_cancel_frame(spinner->frame_id);
                spinner->frame_id = -1;
        }
}

// object 3: widget callbacks

static ui_spinner_t *ui_spinner_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_spinner_prototype);
}

static void ui_spinner_on_frame(uint64_t timestamp, void *data)
{
        ui_widget_t *w = data;
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        bool want;
        double dt;

        /* lcui_request_frame is one-shot: reaching this callback means the
         * pending request has been consumed, so the stored id is no longer
         * cancellable and resets to -1 before any new request is made. */
        spinner->frame_id = -1;
        want = spinner->running && ui_widget_is_visible(w);
        if (!want) {
                return;
        }
        dt = spinner->has_ts ? (double)(timestamp - spinner->last_ts) / 1000.0
                             : 0.0;
        spinner->last_ts = timestamp;
        spinner->has_ts = true;
        spinner->angle =
            ui_spinner_step_angle(spinner->angle, spinner->speed, dt);
        ui_draw_layer_invalidate(spinner->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
        /* Re-arm the one-shot subscription for the next frame; the id is -1
         * here, so reconcile() on a later update can never stack a second
         * pending request on top of this one. */
        spinner->frame_id = lcui_request_frame(ui_spinner_on_frame, w);
}

/* Advance a rotation angle in degrees by a duration and wrap to [0, 360). This
 * is the whole clock path so it can be exercised without a running app. */
double ui_spinner_step_angle(double cur, double speed_rps, double dt_sec)
{
        return fmod(cur + speed_rps * 360.0 * dt_sec, 360.0);
}

static void ui_spinner_on_init(ui_widget_t *w)
{
        ui_spinner_t *spinner;

        spinner = ui_widget_add_data(w, ui_spinner_prototype, sizeof(*spinner));
        memset(spinner, 0, sizeof(*spinner));
        spinner->running = true;
        spinner->speed = 1.0f;
        spinner->angle = 0.0;
        spinner->frame_id = -1;
        spinner->layer = ui_draw_layer_create();
        spinner->content_surface = ui_draw_surface_create();
        /* Provide a default content up front (before children are parsed) so it
         * participates in normal layout; an explicit `spinner-content` that
         * links in later replaces it via the link callback. */
        ui_spinner_build_default_content(w);
        spinner->content_dirty = true;
        ui_spinner_observe(w);
        /* The frame subscription is started by the first update reconcile
         * rather than here, so a widget that is never shown never requests a
         * frame. */
}

static void ui_spinner_on_destroy(ui_widget_t *w)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);

        if (spinner->frame_id >= 0) {
                lcui_cancel_frame(spinner->frame_id);
                spinner->frame_id = -1;
        }
        if (spinner->observer) {
                ui_mutation_observer_destroy(spinner->observer);
                spinner->observer = NULL;
        }
        ui_draw_layer_destroy(spinner->layer);
        ui_draw_surface_destroy(spinner->content_surface);
        spinner->layer = NULL;
        spinner->content_surface = NULL;
}

/* Paint reads the four border_X_color slots directly and CSS `current-color`
 * is unimplemented, so a stroke that follows the inherited `color` property
 * must be written here: copy the spinner's computed colour into the ring's
 * left/right/bottom borders and leave the top border transparent as the gap.
 * Only the auto-generated ring is managed; custom content keeps its own
 * styling untouched. */
static void ui_spinner_sync_ring_color(ui_widget_t *w)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        css_color_value_t color;

        if (!spinner->ring || !ui_widget_is_visible(w)) {
                return;
        }
        color = w->computed_style.color;
        if (spinner->ring_color_set && spinner->ring_color == color) {
                return;
        }
        ui_widget_set_style_color_value(spinner->ring,
                                        css_prop_border_left_color, color);
        ui_widget_set_style_color_value(spinner->ring,
                                        css_prop_border_right_color, color);
        ui_widget_set_style_color_value(spinner->ring,
                                        css_prop_border_bottom_color, color);
        /* The gap side keeps the same hue at a low alpha instead of vanishing,
         * so the ring carries ink all the way round and reads as centred, while
         * the faint segment still sweeps as a fading tail. */
        ui_widget_set_style_color_value(
            spinner->ring, css_prop_border_top_color,
            css_color((uint8_t)((color >> 24) * UI_SPINNER_GAP_ALPHA),
                      (uint8_t)(color >> 16), (uint8_t)(color >> 8),
                      (uint8_t)color));
        spinner->ring_color = color;
        spinner->ring_color_set = true;
        ui_spinner_invalidate_content(w);
}

static void ui_spinner_on_update(ui_widget_t *w, ui_task_type_t task)
{
        if (task == UI_TASK_UPDATE_STYLE) {
                /* A style refresh can change how the captured subtree looks
                 * (inherited properties), so drop the cached capture. */
                ui_spinner_invalidate_content(w);
                return;
        }
        if (task != UI_TASK_AFTER_UPDATE) {
                return;
        }
        ui_spinner_reconcile_frame(w);
        ui_spinner_sync_ring_color(w);
}

static void ui_spinner_on_resize(ui_widget_t *w, float width, float height)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);

        ui_spinner_size_content(spinner->content);
        /* Geometry changed: the cached display was composed at the old size
         * and the subtree relayouts, so run a full render on the next paint
         * even if the physical box happens to come back the same. */
        ui_spinner_invalidate_content(w);
}

static void ui_spinner_on_paint(ui_widget_t *w, pd_context_t *paint,
                                ui_widget_actual_style_t *style)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        pd_rect_t area, overlap;
        float cw = w->content_box.width;
        float ch = w->content_box.height;
        int ox, oy;

        if (!spinner->content || cw < 1.0f || ch < 1.0f) {
                return;
        }
        ox = (int)(style->content_box.x - style->canvas_box.x);
        oy = (int)(style->content_box.y - style->canvas_box.y);
        area.x = ox;
        area.y = oy;
        area.width = style->content_box.width;
        area.height = style->content_box.height;
        if (!pd_rect_overlap(&area, &paint->rect, &overlap)) {
                return;
        }
        /* Keep the cached display at the current box size; the backend only
         * reallocates when the physical size changes, so a stable frame
         * allocates nothing. */
        ui_draw_layer_ensure(spinner->layer, cw, ch);
        /* Full render only when the widget itself changed (animation tick,
         * angle, content, colour, geometry). A paint that merely overlaps a
         * newly dirty neighbour re-presents the cache untouched. */
        if (ui_draw_layer_needs_render(spinner->layer)) {
                /* Rebuilding the capture re-runs the whole subtree render; the
                 * rotation below changes every frame but the content usually
                 * does not, so reuse the previous capture unless something
                 * marked it dirty (or the backing had to be rebuilt). */
                if (ui_draw_surface_resize(spinner->content_surface, cw, ch) >
                    0) {
                        spinner->content_dirty = true;
                }
                if (spinner->content_dirty) {
                        ui_draw_capture(spinner->content,
                                        spinner->content_surface);
                        spinner->content_dirty = false;
                }
                ui_draw_rotate_into(spinner->content_surface,
                                    ui_draw_layer_surface(spinner->layer),
                                    spinner->angle * UI_SPINNER_PI / 180.0);
                ui_draw_layer_mark_rendered(spinner->layer);
        }
        ui_draw_layer_present(spinner->layer, paint, ox, oy, (int)area.width,
                              (int)area.height);
}

static void ui_spinner_on_setattr(ui_widget_t *w, const char *name,
                                  const char *value)
{
        /* Declarative attributes route to the public setters. `running` uses
         * the HTML boolean convention (a value of "false" disables it);
         * `speed` and `angle` are parsed as numbers. Unknown names are ignored.
         */
        if (strcmp(name, "running") == 0) {
                ui_spinner_set_running(w,
                                       !(value && strcmp(value, "false") == 0));
        } else if (strcmp(name, "speed") == 0) {
                if (value) {
                        ui_spinner_set_speed(w, (float)atof(value));
                }
        } else if (strcmp(name, "angle") == 0) {
                if (value) {
                        ui_spinner_set_angle(w, atof(value));
                }
        }
}

// object 4: public API

void ui_spinner_set_running(ui_widget_t *w, bool running)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);

        spinner->running = running;
        if (running) {
                /* Restarting re-enters the clock path; refresh the cache on
                 * the next paint even if the first tick is still queued. */
                ui_draw_layer_invalidate(spinner->layer);
        }
        ui_spinner_reconcile_frame(w);
}

void ui_spinner_set_speed(ui_widget_t *w, float revolutions_per_sec)
{
        ui_spinner_get_data(w)->speed = revolutions_per_sec;
}

void ui_spinner_set_angle(ui_widget_t *w, double degrees)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);

        spinner->angle = degrees;
        /* A static angle change alters the current frame's appearance, so it
         * must dirty the graph box for a repaint independent of the clock,
         * and force the layer's full render pass on that repaint. */
        ui_draw_layer_invalidate(spinner->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

void ui_spinner_set_content(ui_widget_t *w, ui_widget_t *content)
{
        ui_spinner_t *spinner = ui_spinner_get_data(w);
        ui_widget_t *old = spinner->content;

        if (old == content) {
                return;
        }
        spinner->content = content;
        if (spinner->default_content == old) {
                spinner->default_content = NULL;
                spinner->ring = NULL;
        }
        if (content) {
                if (content->parent != w) {
                        ui_widget_append(w, content);
                }
                ui_spinner_size_content(content);
                ui_widget_request_refresh_style(content);
        }
        if (old && old->parent == w) {
                ui_widget_destroy(old);
        }
        /* A different captured subtree changes the widget's own appearance. */
        ui_spinner_invalidate_content(w);
}

ui_widget_t *ui_create_spinner(void)
{
        return ui_create_widget_with_prototype(ui_spinner_prototype);
}

void ui_register_spinner(void)
{
        ui_spinner_prototype = ui_create_widget_prototype("spinner", NULL);
        ui_spinner_prototype->init = ui_spinner_on_init;
        ui_spinner_prototype->destroy = ui_spinner_on_destroy;
        ui_spinner_prototype->update = ui_spinner_on_update;
        ui_spinner_prototype->resize = ui_spinner_on_resize;
        ui_spinner_prototype->paint = ui_spinner_on_paint;
        ui_spinner_prototype->setattr = ui_spinner_on_setattr;
        ui_spinner_content_prototype =
            ui_create_widget_prototype("spinner-content", NULL);
        ui_spinner_content_prototype->init = ui_spinner_content_on_init;
        ui_load_css_string(ui_spinner_css, __FILE__);
}
