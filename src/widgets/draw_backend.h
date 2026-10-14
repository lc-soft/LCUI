/*
 * src/widgets/draw_backend.h: internal drawing seam for self-drawn widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef SRC_WIDGETS_DRAW_BACKEND_H
#define SRC_WIDGETS_DRAW_BACKEND_H

#include <stdbool.h>
#include <pandagl.h>
#include <ui.h>

/* An opaque offscreen surface. It owns a backend-specific pixel buffer (an
 * ARGB canvas internally) and its own physical size derived from a logical
 * size multiplied by the active device scale. Widget code never sees the
 * pixel buffer or the scale: it only names surfaces and works in logical
 * coordinates. The concrete type lives solely in the software backend. */
typedef struct ui_draw_surface ui_draw_surface_t;

/* Allocate an empty surface with no backing until resized. */
ui_draw_surface_t *ui_draw_surface_create(void);

/* Release a surface and its backing. Safe to call with NULL. */
void ui_draw_surface_destroy(ui_draw_surface_t *s);

/* Resize the surface so its backing covers ceil(logical * device_scale) in
 * both axes. The backing is supersampled internally (an integer multiple of
 * the device resolution) to improve the quality of rotation and compositing;
 * callers still work purely in device pixels, and a supersampled surface is
 * box-downsampled back to device resolution when it is blended onto a target.
 * The buffer is (re)allocated only when the physical size changes; otherwise
 * this is a no-op and returns 0. Content is not preserved across a resize.
 * Returns 1 when the backing was rebuilt, 0 when unchanged. */
int ui_draw_surface_resize(ui_draw_surface_t *s, float logical_w,
                           float logical_h);

/* Report the logical size the surface was last resized to. Returns 0 and
 * fills w/h when the surface has a positive physical size, else -1. */
int ui_draw_surface_logical_size(const ui_draw_surface_t *s, float *w,
                                 float *h);

/* Capture a widget subtree into dst, retaining the subtree's alpha. dst must
 * already be resized to the target size; this performs no allocation and does
 * not touch the layout. The subtree is rasterised at the surface's internal
 * supersampled resolution (the active device scale times the supersample
 * factor) so the backing is filled at full quality. Returns 0 on success. */
int ui_draw_capture(ui_widget_t *widget, ui_draw_surface_t *dst);

/* Rotate src around its center by radians into dst. dst must already be
 * resized to the same physical size as src; the existing backing is reused so
 * this allocates nothing. src and dst must be different surfaces. Returns 0 on
 * success. */
int ui_draw_rotate_into(const ui_draw_surface_t *src, ui_draw_surface_t *dst,
                        double radians);

/* Fill the entire surface with transparent pixels. Returns 0 on success. */
int ui_draw_clear(ui_draw_surface_t *dst);

/* Fill a logical-rectangle with an RGBA color, converting to physical pixels
 * internally. dst must already be resized. Returns 0 on success. */
int ui_draw_fill_rect(ui_draw_surface_t *dst, float x_logical, float y_logical,
                      float w_logical, float h_logical, unsigned char r,
                      unsigned char g, unsigned char b, unsigned char a);

/* Fill dst with a tilted, gradient highlight "sheen": a soft-edged band of the
 * given color whose alpha peaks at `color.a` along the band's center line and
 * fades smoothly to 0 at both sides, so it reads as a sliding sheen rather than
 * a solid slab. The band is centered horizontally at `center_x_logical` (its
 * middle row), `spread_logical` wide, and rotated by `angle_deg` about that
 * middle row so its center line is sheared across the height. The whole surface
 * is written every call (transparent outside the band), evaluated per physical
 * pixel with no allocation; dst must already be resized. Returns 0 on success.
 */
int ui_draw_fill_sheen(ui_draw_surface_t *dst, pd_color_t color,
                       float center_x_logical, float spread_logical,
                       float angle_deg);

/* Copy src into dst (same physical size) into dst's existing backing. Returns
 * 0 on success. */
int ui_draw_copy(const ui_draw_surface_t *src, ui_draw_surface_t *dst);

/* Multiply dst alpha by mask alpha in place (destination-in). Both surfaces
 * must be valid; this allocates nothing. Returns 0 on success. */
int ui_draw_mask_into(const ui_draw_surface_t *mask, ui_draw_surface_t *dst);

/* Lighten dst toward the sheen's colour WITHOUT touching dst's alpha: for each
 * pixel, t = sheen.alpha/255 and dst.rgb = dst.rgb*(1-t) + sheen.rgb*t, while
 * dst.alpha is left exactly as-is. This is the highlight-composite for shimmer:
 * it recolours the content's ink (including antialiased edges) but never
 * rewrites their coverage, so text edges do not get a resampled halo. Both
 * surfaces must be the same physical size and valid; allocates nothing. Returns
 * 0 on success. */
int ui_draw_sheen_over(ui_draw_surface_t *dst, const ui_draw_surface_t *sheen);

/* Composite a surface onto a paint target at (x, y) in target pixel space,
 * scaling the surface's source alpha by opacity (typically 1.0). Coordinates
 * are device pixels: a supersampled surface is box-downsampled to device
 * resolution as part of the blend. Returns 0 on success. */
int ui_draw_blend_surface(pd_context_t *target, const ui_draw_surface_t *src,
                          int x, int y, bool with_alpha, float opacity);

/* Composite a sub-rectangle [src_x, src_y, src_w, src_h] of a surface onto a
 * paint target at the non-negative offset (dst_x, dst_y). The source rect is in
 * device pixels and is box-downsampled from the backing when the surface is
 * supersampled. This is the partial-repaint-safe form: pd_canvas_mix does not
 * skip the source rows that a negative offset would push above the target, so
 * blitting the whole box surface at a negative offset smears the top of the
 * surface into the visible sub-region. Callers pass an already-clipped
 * destination offset. No allocation. Returns 0 on success; a degenerate rect is
 * a no-op returning 0.
 */
int ui_draw_blend_surface_rect(pd_context_t *target,
                               const ui_draw_surface_t *src, int src_x,
                               int src_y, int src_w, int src_h, int dst_x,
                               int dst_y, bool with_alpha, float opacity);

/* Composite src onto dst at offset (0, 0) in dst's own pixel space, keeping
 * dst's pixels under src's alpha (dst is typically a transparent display
 * buffer being built up from scratch layers). src is clipped to dst's bounds;
 * opacity scales src's source alpha like ui_draw_blend_surface. Both surfaces
 * must be valid; this allocates nothing. Returns 0 on success. */
int ui_draw_surface_composite(ui_draw_surface_t *dst,
                              const ui_draw_surface_t *src, bool with_alpha,
                              float opacity);

/* A cached display layer: it owns one final-composition surface ("display")
 * at the physical size for a logical box, plus a dirty flag. Self-drawn
 * widgets use it as a render gate: run the full render pass into the display
 * surface only when ui_draw_layer_needs_render() says so, then present the
 * dirty sub-rectangle of the cache to the paint target. Presenting never
 * re-renders, so a widget whose overlap with the dirty region changes (for
 * example another window uncovers part of it) blits the cache instead of
 * paying for a full redraw. Widget code never sees the backing or the scale. */
typedef struct ui_draw_layer ui_draw_layer_t;

/* Allocate an empty layer with no display backing and dirty set (so the first
 * needs_render() is true). */
ui_draw_layer_t *ui_draw_layer_create(void);

/* Release a layer and its display surface. Safe to call with NULL. */
void ui_draw_layer_destroy(ui_draw_layer_t *l);

/* Ensure the display surface covers ceil(logical * device_scale) for the
 * given logical box. When the physical size (or scale) changes the backing is
 * rebuilt and the layer is marked dirty; a stable frame is a cheap no-op that
 * allocates nothing. Returns 1 when the backing changed, 0 when not. */
int ui_draw_layer_ensure(ui_draw_layer_t *l, float logical_w, float logical_h);

/* Mark the cache stale: call whenever the widget's content, geometry, or
 * animation state changes so the next paint runs the full render pass. */
void ui_draw_layer_invalidate(ui_draw_layer_t *l);

/* Whether the full render pass must run before the next present. */
int ui_draw_layer_needs_render(ui_draw_layer_t *l);

/* The display surface, as the widget's full-render target. The render writes
 * the final composited result into it, including the widget's own alpha;
 * transparent places must stay transparent. Valid after ensure() succeeded. */
ui_draw_surface_t *ui_draw_layer_surface(ui_draw_layer_t *l);

/* Clear the dirty flag once the full render pass has written the display
 * surface. */
void ui_draw_layer_mark_rendered(ui_draw_layer_t *l);

/* Blit the intersection of the display surface with the paint target's dirty
 * rect, at a non-negative target offset, onto paint. box_x/box_y/box_w/box_h
 * are the widget's content box in physical pixels of its canvas coordinate
 * space (the same origin as the widget's paint ox/oy/area). Nothing outside
 * the box is drawn; an empty intersection draws nothing. The cache is never
 * re-rendered here: if the widget skipped its full pass because
 * needs_render() was false, this blits the cached pixels. Returns 1 when
 * something was blitted, 0 otherwise. */
int ui_draw_layer_present(ui_draw_layer_t *l, pd_context_t *paint, int box_x,
                          int box_y, int box_w, int box_h);

#endif /* SRC_WIDGETS_DRAW_BACKEND_H */
