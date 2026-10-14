/*
 * src/widgets/shimmer.c: -- LCUI's Shimmer widget
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
#include <LCUI/app.h>
#include <LCUI/widgets/shimmer.h>
#include "draw_backend.h"
#include "shimmer_priv.h"

/* Fraction of the box width used as the highlight spread when it is not set
 * explicitly. */
#define UI_SHIMMER_DEFAULT_SPREAD_RATIO 0.45f
/* One full sweep duration in milliseconds when the caller does not set one. */
#define UI_SHIMMER_DEFAULT_DURATION 2000
/* The highlight tilt in degrees when the caller does not set one. */
#define UI_SHIMMER_DEFAULT_ANGLE 20.0f

typedef struct ui_shimmer {
        bool running;
        pd_color_t color;
        unsigned duration;
        float phase;
        float spread;
        bool spread_set;
        float angle;
        int frame_id;
        uint64_t last_ts;
        bool has_ts;
        ui_draw_layer_t *layer;
        ui_draw_surface_t *content_surface;
        ui_draw_surface_t *band_surface;
        ui_draw_surface_t *sheen_surface;
        /* The captured content is expensive to rebuild (a full subtree render)
         * but only changes when the content itself does; the travelling band
         * changes every animation frame. Keep the capture cached until
         * something marks the content dirty (resize, set_content, or a subtree
         * mutation seen by the observer). */
        bool content_dirty;
        ui_mutation_observer_t *observer;
        ui_widget_t *content;
        ui_widget_t *default_content;
} ui_shimmer_t;

static ui_widget_prototype_t *ui_shimmer_prototype;
static ui_widget_prototype_t *ui_shimmer_content_prototype;

static const char *ui_shimmer_css = "\
shimmer {\
  display: block;\
  width: 100%;\
  height: 20px;\
}\
shimmer-content {\
  position: absolute;\
  box-sizing: border-box;\
  left: -100%;\
  top: -100%;\
  width: 100%;\
  height: 100%;\
}\
shimmer .shimmer-skeleton {\
  position: absolute;\
  box-sizing: border-box;\
  left: 0;\
  top: 0;\
  width: 100%;\
  height: 100%;\
  background-color: #b0b0b0;\
  border-radius: 4px;\
}\
";

static ui_shimmer_t *ui_shimmer_get_data(ui_widget_t *w);
static void ui_shimmer_on_frame(uint64_t timestamp, void *data);
static void ui_shimmer_size_content(ui_widget_t *content);
static void ui_shimmer_reconcile_frame(ui_widget_t *w);
static void ui_shimmer_attach_content(ui_widget_t *content);
static ui_widget_t *ui_shimmer_build_default_content(ui_widget_t *w);
static void ui_shimmer_fill_band(ui_shimmer_t *shimmer, float logical_w,
                                 float logical_h);
static void ui_shimmer_invalidate_content(ui_widget_t *w);
static void ui_shimmer_observe(ui_widget_t *w);

// object 1: content binding

static void ui_shimmer_attach_content(ui_widget_t *content)
{
        ui_widget_t *owner = content->parent;
        ui_shimmer_t *shimmer;

        while (owner && owner->proto != ui_shimmer_prototype) {
                owner = owner->parent;
        }
        if (!owner) {
                return;
        }
        shimmer = ui_shimmer_get_data(owner);
        if (!shimmer || shimmer->content == content) {
                return;
        }
        /* When a real content node links in, drop the auto-created default so
         * the shimmer keeps exactly the one subtree it will capture. */
        if (shimmer->default_content == shimmer->content) {
                ui_widget_destroy(shimmer->default_content);
                shimmer->default_content = NULL;
        }
        shimmer->content = content;
        ui_shimmer_invalidate_content(owner);
}

static ui_widget_t *ui_shimmer_build_default_content(ui_widget_t *w)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);
        ui_widget_t *content, *block;

        content = ui_create_widget_with_prototype(ui_shimmer_content_prototype);
        block = ui_create_widget(NULL);
        ui_widget_add_class(block, "shimmer-skeleton");
        ui_widget_append(content, block);
        /* Pre-set the pointers so the link callback fired by append() treats
         * this content as already tracked and does not destroy it. */
        shimmer->content = content;
        shimmer->default_content = content;
        ui_widget_append(w, content);
        return content;
}

static void ui_shimmer_content_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_shimmer_attach_content(w);
}

static void ui_shimmer_content_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_shimmer_content_on_link, NULL);
}

// object 2: surface and frame lifecycle

/* Mark the cached content capture stale so the next full render rebinds the
 * subtree. Every change that can alter what the capture looks like must go
 * through here (or through a subtree mutation the observer sees), otherwise
 * the shimmer keeps sweeping over a stale bitmap. */
static void ui_shimmer_invalidate_content(ui_widget_t *w)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        if (!shimmer) {
                return;
        }
        shimmer->content_dirty = true;
        ui_draw_layer_invalidate(shimmer->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

static void ui_shimmer_on_mutation(ui_mutation_list_t *records,
                                   ui_mutation_observer_t *observer, void *arg)
{
        ui_shimmer_invalidate_content((ui_widget_t *)arg);
}

/* Watch the widget and its whole subtree so edits made to custom content
 * (through the style, attribute or child-list APIs) invalidate the cache the
 * same way the shimmer's own hooks do. The observation targets the shimmer
 * itself, not the content node: widgets are destroyed children-first, so an
 * observer connected to a child would still hold a dangling pointer when the
 * shimmer tears it down (and connecting to an ancestor covers a replacement
 * content too). */
static void ui_shimmer_observe(ui_widget_t *w)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);
        ui_mutation_observer_init_t options = {
                .child_list = true,
                .subtree = true,
                .properties = true,
                .attributes = true,
        };

        if (!shimmer || shimmer->observer) {
                return;
        }
        shimmer->observer =
            ui_mutation_observer_create(ui_shimmer_on_mutation, w);
        if (shimmer->observer) {
                ui_mutation_observer_observe(shimmer->observer, w, options);
        }
}

static void ui_shimmer_size_content(ui_widget_t *content)
{
        if (!content) {
                return;
        }
        /* The content fills the shimmer box and is pushed offscreen so the
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
static void ui_shimmer_reconcile_frame(ui_widget_t *w)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);
        bool want = shimmer->running && ui_widget_is_visible(w);

        if (want && shimmer->frame_id < 0) {
                shimmer->has_ts = false;
                shimmer->frame_id = lcui_request_frame(ui_shimmer_on_frame, w);
        } else if (!want && shimmer->frame_id >= 0) {
                lcui_cancel_frame(shimmer->frame_id);
                shimmer->frame_id = -1;
        }
}

/* Write the travelling highlight band into the band surface in logical space.
 * The band is a full-height column of the configured colour whose alpha ramps
 * smoothly to 0 at both of its edges and is sheared across the height by the
 * tilt angle, so it reads as a soft-edged sheen rather than a solid slab; the
 * surface backend turns the logical range into device pixels. */
static void ui_shimmer_fill_band(ui_shimmer_t *shimmer, float logical_w,
                                 float logical_h)
{
        double phase, spread, center;

        /* Wrap the phase so set_phase(1.0) is equivalent to set_phase(0.0); the
         * animation already keeps it in [0, 1) but a manual setter may not. */
        phase = fmod(shimmer->phase, 1.0);
        if (phase < 0) {
                phase += 1.0;
        }
        spread = shimmer->spread;
        if (spread < 1.0) {
                spread = 1.0;
        }
        /* Slide the band's center from just off the left edge to just off the
         * right edge across the phase, so the highlight fully enters and exits.
         */
        center = -spread + phase * (logical_w + 2.0 * spread);

        ui_draw_clear(shimmer->band_surface);
        ui_draw_fill_sheen(shimmer->band_surface, shimmer->color, (float)center,
                           (float)spread, shimmer->angle);
}

// object 3: widget callbacks

static ui_shimmer_t *ui_shimmer_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_shimmer_prototype);
}

static void ui_shimmer_on_frame(uint64_t timestamp, void *data)
{
        ui_widget_t *w = data;
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);
        bool want;
        double ms;

        /* lcui_request_frame is one-shot: reaching this callback means the
         * pending request has been consumed, so the stored id is no longer
         * cancellable and resets to -1 before any new request is made. */
        shimmer->frame_id = -1;
        want = shimmer->running && ui_widget_is_visible(w);
        if (!want) {
                return;
        }
        ms = shimmer->has_ts ? (double)(timestamp - shimmer->last_ts) : 0.0;
        shimmer->last_ts = timestamp;
        shimmer->has_ts = true;
        shimmer->phase =
            ui_shimmer_step_phase(shimmer->phase, ms, shimmer->duration);
        ui_draw_layer_invalidate(shimmer->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
        /* Re-arm the one-shot subscription for the next frame; the id is -1
         * here, so reconcile() on a later update can never stack a second
         * pending request on top of this one. */
        shimmer->frame_id = lcui_request_frame(ui_shimmer_on_frame, w);
}

/* Advance the highlight phase by an elapsed time over a duration and wrap to
 * [0, 1). This is the whole clock path so it can be exercised without a running
 * app. */
float ui_shimmer_step_phase(float cur, double ms_elapsed, unsigned duration_ms)
{
        float phase;

        if (duration_ms == 0) {
                return cur;
        }
        phase = fmodf((float)((double)cur + ms_elapsed / (double)duration_ms),
                      1.0f);
        if (phase < 0.0f) {
                phase += 1.0f;
        }
        return phase;
}

static void ui_shimmer_on_init(ui_widget_t *w)
{
        ui_shimmer_t *shimmer;

        shimmer = ui_widget_add_data(w, ui_shimmer_prototype, sizeof(*shimmer));
        memset(shimmer, 0, sizeof(*shimmer));
        shimmer->running = true;
        shimmer->color = pd_argb(220, 255, 255, 255);
        shimmer->duration = UI_SHIMMER_DEFAULT_DURATION;
        shimmer->phase = 0.0f;
        shimmer->spread = 0.0f;
        shimmer->spread_set = false;
        shimmer->angle = UI_SHIMMER_DEFAULT_ANGLE;
        shimmer->frame_id = -1;
        shimmer->layer = ui_draw_layer_create();
        shimmer->content_surface = ui_draw_surface_create();
        shimmer->band_surface = ui_draw_surface_create();
        shimmer->sheen_surface = ui_draw_surface_create();
        /* Provide a default content up front (before children are parsed) so it
         * participates in normal layout; an explicit `shimmer-content` that
         * links in later replaces it via the link callback. */
        ui_shimmer_build_default_content(w);
        shimmer->content_dirty = true;
        ui_shimmer_observe(w);
        /* The frame subscription is started by the first update reconcile
         * rather than here, so a widget that is never shown never requests a
         * frame. */
}

static void ui_shimmer_on_destroy(ui_widget_t *w)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        if (shimmer->frame_id >= 0) {
                lcui_cancel_frame(shimmer->frame_id);
                shimmer->frame_id = -1;
        }
        if (shimmer->observer) {
                ui_mutation_observer_destroy(shimmer->observer);
                shimmer->observer = NULL;
        }
        ui_draw_layer_destroy(shimmer->layer);
        ui_draw_surface_destroy(shimmer->content_surface);
        ui_draw_surface_destroy(shimmer->band_surface);
        ui_draw_surface_destroy(shimmer->sheen_surface);
        shimmer->layer = NULL;
        shimmer->content_surface = NULL;
        shimmer->band_surface = NULL;
        shimmer->sheen_surface = NULL;
}

static void ui_shimmer_on_update(ui_widget_t *w, ui_task_type_t task)
{
        if (task == UI_TASK_UPDATE_STYLE) {
                /* A style refresh can change how the captured subtree looks
                 * (inherited properties), so drop the cached capture. */
                ui_shimmer_invalidate_content(w);
                return;
        }
        if (task != UI_TASK_AFTER_UPDATE) {
                return;
        }
        ui_shimmer_reconcile_frame(w);
}

static void ui_shimmer_on_resize(ui_widget_t *w, float width, float height)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        if (!shimmer->spread_set) {
                shimmer->spread = width * UI_SHIMMER_DEFAULT_SPREAD_RATIO;
        }
        ui_shimmer_size_content(shimmer->content);
        /* Geometry (and the derived default spread) changed: force a full
         * render on the next paint even if the physical box is unchanged. */
        ui_shimmer_invalidate_content(w);
}

static void ui_shimmer_on_paint(ui_widget_t *w, pd_context_t *paint,
                                ui_widget_actual_style_t *style)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);
        pd_rect_t area, overlap;
        float cw = w->content_box.width;
        float ch = w->content_box.height;
        int ox, oy;

        if (!shimmer->content || cw < 1.0f || ch < 1.0f) {
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
         * reallocates when the device size changes, so a stable animation
         * allocates nothing per frame. */
        ui_draw_layer_ensure(shimmer->layer, cw, ch);
        /* Full render only when the widget itself changed (animation tick,
         * phase/colour/spread/angle/content/geometry). A paint that merely
         * overlaps a newly dirty neighbour re-presents the cache untouched. */
        if (ui_draw_layer_needs_render(shimmer->layer)) {
                ui_draw_surface_t *display =
                    ui_draw_layer_surface(shimmer->layer);
                /* 1. Capture the content subtree, retaining its alpha. The
                 * capture is reused across frames because the content usually
                 * does not change; only the band below moves. */
                if (ui_draw_surface_resize(shimmer->content_surface, cw, ch) >
                    0) {
                        shimmer->content_dirty = true;
                }
                ui_draw_surface_resize(shimmer->band_surface, cw, ch);
                ui_draw_surface_resize(shimmer->sheen_surface, cw, ch);
                if (shimmer->content_dirty) {
                        ui_draw_capture(shimmer->content,
                                        shimmer->content_surface);
                        shimmer->content_dirty = false;
                }
                /* 2. Base: the content composited at full strength onto a
                 * clean display, keeping its own colours so nothing is
                 * dimmed. */
                ui_draw_clear(display);
                ui_draw_surface_composite(display, shimmer->content_surface,
                                          true, 1.0f);
                /* 3. The travelling sheen: the tilted gradient band masked by
                 * the content alpha, so the highlight only lands on the ink
                 * and nowhere the content is transparent. Every step reuses
                 * the surfaces' existing backing, adding no allocation. */
                ui_shimmer_fill_band(shimmer, cw, ch);
                ui_draw_copy(shimmer->band_surface, shimmer->sheen_surface);
                ui_draw_mask_into(shimmer->content_surface,
                                  shimmer->sheen_surface);
                ui_draw_sheen_over(display, shimmer->sheen_surface);
                ui_draw_layer_mark_rendered(shimmer->layer);
        }
        ui_draw_layer_present(shimmer->layer, paint, ox, oy, (int)area.width,
                              (int)area.height);
}

static void ui_shimmer_on_setattr(ui_widget_t *w, const char *name,
                                  const char *value)
{
        /* Declarative attributes route to the public setters. `running` uses
         * the HTML boolean convention (a value of "false" disables it);
         * `color` is parsed as a CSS colour string; `duration`, `spread`,
         * `angle` and `phase` are parsed as numbers. Unknown names are ignored.
         */
        if (strcmp(name, "running") == 0) {
                ui_shimmer_set_running(w,
                                       !(value && strcmp(value, "false") == 0));
        } else if (strcmp(name, "color") == 0 && value) {
                css_style_value_t parsed;

                if (css_parse_color_value(&parsed, value) &&
                    parsed.type == CSS_COLOR_VALUE) {
                        css_color_value_t cv = parsed.color_value;
                        ui_shimmer_set_color(
                            w, pd_argb((uint8_t)(cv >> 24), (uint8_t)(cv >> 16),
                                       (uint8_t)(cv >> 8), (uint8_t)cv));
                }
        } else if (strcmp(name, "duration") == 0 && value) {
                ui_shimmer_set_duration(w, (unsigned)atoi(value));
        } else if (strcmp(name, "spread") == 0 && value) {
                ui_shimmer_set_spread(w, (float)atof(value));
        } else if (strcmp(name, "angle") == 0 && value) {
                ui_shimmer_set_angle(w, (float)atof(value));
        } else if (strcmp(name, "phase") == 0 && value) {
                ui_shimmer_set_phase(w, (float)atof(value));
        }
}

// object 4: public API

void ui_shimmer_set_running(ui_widget_t *w, bool running)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        shimmer->running = running;
        if (running) {
                /* Restarting re-enters the clock path; refresh the cache on
                 * the next paint even if the first tick is still queued. */
                ui_draw_layer_invalidate(shimmer->layer);
        }
        ui_shimmer_reconcile_frame(w);
}

void ui_shimmer_set_color(ui_widget_t *w, pd_color_t color)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        shimmer->color = color;
        /* A different highlight colour changes the current frame's appearance.
         */
        ui_draw_layer_invalidate(shimmer->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

void ui_shimmer_set_duration(ui_widget_t *w, unsigned ms)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        shimmer->duration = ms;
        /* The sweep speed feeds the next phase step; refresh the cache so the
         * next rendered frame uses the new timing even before a tick. */
        ui_draw_layer_invalidate(shimmer->layer);
}

void ui_shimmer_set_spread(ui_widget_t *w, float px)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        shimmer->spread = px;
        shimmer->spread_set = true;
        /* A different spread changes the current frame's appearance. */
        ui_draw_layer_invalidate(shimmer->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

void ui_shimmer_set_angle(ui_widget_t *w, float degrees)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        shimmer->angle = degrees;
        /* A different tilt changes the current frame's appearance. */
        ui_draw_layer_invalidate(shimmer->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

void ui_shimmer_set_phase(ui_widget_t *w, float phase)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);

        shimmer->phase = phase;
        /* Moving the band changes the current frame's appearance, so it must
         * dirty the graph box for a repaint independent of the clock, and
         * force the layer's full render pass on that repaint. */
        ui_draw_layer_invalidate(shimmer->layer);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
}

void ui_shimmer_set_content(ui_widget_t *w, ui_widget_t *content)
{
        ui_shimmer_t *shimmer = ui_shimmer_get_data(w);
        ui_widget_t *old = shimmer->content;

        if (old == content) {
                return;
        }
        shimmer->content = content;
        if (shimmer->default_content == old) {
                shimmer->default_content = NULL;
        }
        if (content) {
                if (content->parent != w) {
                        ui_widget_append(w, content);
                }
                ui_shimmer_size_content(content);
                ui_widget_request_refresh_style(content);
        }
        if (old && old->parent == w) {
                ui_widget_destroy(old);
        }
        /* A different captured subtree changes the widget's own appearance. */
        ui_shimmer_invalidate_content(w);
}

ui_widget_t *ui_create_shimmer(void)
{
        return ui_create_widget_with_prototype(ui_shimmer_prototype);
}

void ui_register_shimmer(void)
{
        ui_shimmer_prototype = ui_create_widget_prototype("shimmer", NULL);
        ui_shimmer_prototype->init = ui_shimmer_on_init;
        ui_shimmer_prototype->destroy = ui_shimmer_on_destroy;
        ui_shimmer_prototype->update = ui_shimmer_on_update;
        ui_shimmer_prototype->resize = ui_shimmer_on_resize;
        ui_shimmer_prototype->paint = ui_shimmer_on_paint;
        ui_shimmer_prototype->setattr = ui_shimmer_on_setattr;
        ui_shimmer_content_prototype =
            ui_create_widget_prototype("shimmer-content", NULL);
        ui_shimmer_content_prototype->init = ui_shimmer_content_on_init;
        ui_load_css_string(ui_shimmer_css, __FILE__);
}
