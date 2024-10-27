/*
 * lib/ui-widgets/src/scrollbar.c: -- Scrollbar widget
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <css.h>
#include <ui.h>
#include <ui_widgets/scrollbar.h>
#include <ui_widgets/scrollarea.h>
#include <ptk.h>

typedef struct ui_scrollbar {
        ui_widget_t *thumb;
        bool is_dragging;
        bool is_draggable;
        ui_scrollbar_orientation_t orientation;
        float thumb_x, thumb_y;
        float mouse_x, mouse_y;
} ui_scrollbar_t;

static ui_widget_prototype_t *ui_scrollbar_proto;

static const char *ui_scrollbar_css = "\
scrollbar {\
	display: flex;\
	position: absolute;\
	background-color: #fafafa;\
	box-sizing: border-box;\
}\
\
scrollbar-track {\
	flex: auto;\
}\
\
scrollbar.vertical {\
	top: 0;\
	right: 0;\
	width: 14px;\
	height: 100%;\
	flex-direction: column;\
}\
\
scrollbar.horizontal {\
	bottom: 0;\
	left: 0;\
	width: 100%;\
	height: 14px;\
}\
\
scrollbar-corner {\
	flex: none;\
	width: 14px;\
	height: 14px;\
	display: none;\
}\
\
scrollbar-thumb {\
	top: 0;\
	left: 0;\
	width: 14px;\
	height: 14px;\
	min-width: 14px;\
	min-height: 14px;\
	position: absolute;\
	background-color: #aaa;\
}\
\
scrollbar-thumb:hover {\
	background-color: #bbb;\
}\
\
scrollbar-thumb:active {\
	background-color: #999;\
}\
\
.has-horizontal-scrollbar {\
	padding-bottom: 14px;\
}\
\
.has-vertical-scrollbar {\
	padding-right: 14px;\
}\
\
.has-horizontal-scrollbar scrollbar.vertical scrollbar-corner,\
.has-vertical-scrollbar scrollbar.horizontal scrollbar-corner {\
	display: block;\
}\
";

void ui_scrollbar_update(ui_widget_t *w)
{
        ui_scrollbar_t *that = ui_widget_get_data(w, ui_scrollbar_proto);
        ui_widget_t *track = that->thumb->parent;

        if (!w->parent) {
                return;
        }
        if (!ui_check_widget_type(w->parent, "scrollarea")) {
                logger_error("<scrollbar> must be a child of <scrollarea>\n");
                return;
        }
        if (that->orientation == UI_SCROLLBAR_HORIZONTAL) {
                float scroll_left = ui_scrollarea_get_scroll_left(w->parent);
                float scroll_width = ui_scrollarea_get_scroll_width(w->parent);
                float viewport_width = w->parent->padding_box.width;
                float overflow_width = scroll_width - viewport_width;
                float thumb_left = track->computed_style.padding_left;
                float track_width = track->content_box.width;
                float thumb_width = 0;

                if (scroll_width > 0) {
                        thumb_width =
                            track_width * viewport_width / scroll_width;
                }
                if (thumb_width < that->thumb->computed_style.min_width) {
                        thumb_width = that->thumb->computed_style.min_width;
                }
                if (thumb_width > track_width) {
                        thumb_width = track_width;
                }
                if (overflow_width > 0) {
                        thumb_left += (track_width - thumb_width) *
                                      scroll_left / overflow_width;
                        ui_widget_set_style_unit_value(
                            that->thumb, css_prop_width, thumb_width,
                            CSS_UNIT_PX);
                        ui_widget_set_style_unit_value(that->thumb,
                                                       css_prop_left,
                                                       thumb_left, CSS_UNIT_PX);
                        ui_widget_set_style_keyword_value(
                            w, css_prop_pointer_events, CSS_KEYWORD_AUTO);
                        ui_widget_set_style_keyword_value(
                            w, css_prop_visibility, CSS_KEYWORD_VISIBLE);
                        ui_widget_add_class(w->parent,
                                            "has-horizontal-scrollbar");
                } else {
                        ui_widget_set_style_keyword_value(
                            w, css_prop_pointer_events, CSS_KEYWORD_NONE);
                        ui_widget_set_style_keyword_value(
                            w, css_prop_visibility, CSS_KEYWORD_HIDDEN);
                        ui_widget_remove_class(w->parent,
                                               "has-horizontal-scrollbar");
                }
                return;
        }

        float scroll_top = ui_scrollarea_get_scroll_top(w->parent);
        float scroll_height = ui_scrollarea_get_scroll_height(w->parent);
        float viewport_height = w->parent->padding_box.height;
        float overflow_height = scroll_height - viewport_height;
        float thumb_top = track->computed_style.padding_top;
        float track_height = track->content_box.height;
        float thumb_height = 0;

        if (scroll_height > 0) {
                thumb_height = track_height * viewport_height / scroll_height;
        }
        if (thumb_height < that->thumb->computed_style.min_height) {
                thumb_height = that->thumb->computed_style.min_height;
        }
        if (thumb_height > track_height) {
                thumb_height = track_height;
        }
        if (overflow_height > 0) {
                thumb_top += (track_height - thumb_height) * scroll_top /
                             overflow_height;
                ui_widget_set_style_unit_value(that->thumb, css_prop_height,
                                               thumb_height, CSS_UNIT_PX);
                ui_widget_set_style_unit_value(that->thumb, css_prop_top,
                                               thumb_top, CSS_UNIT_PX);
                ui_widget_set_style_keyword_value(w, css_prop_pointer_events,
                                                  CSS_KEYWORD_AUTO);
                ui_widget_set_style_keyword_value(w, css_prop_visibility,
                                                  CSS_KEYWORD_VISIBLE);
                ui_widget_add_class(w->parent, "has-vertical-scrollbar");
        } else {
                ui_widget_set_style_keyword_value(w, css_prop_pointer_events,
                                                  CSS_KEYWORD_NONE);
                ui_widget_set_style_keyword_value(w, css_prop_visibility,
                                                  CSS_KEYWORD_HIDDEN);
                ui_widget_remove_class(w->parent, "has-vertical-scrollbar");
        }
        DEBUG_MSG("[ui-scrollbar(vertical)] scroll_top=%g, scroll_height=%g, "
                  "overflow_height=%g, track_height=%g, thumb_height=%g, "
                  "thumb_top=%g\n",
                  scroll_top, scroll_height, overflow_height, track_height,
                  thumb_height, thumb_top);
}

static void ui_scrollbar_thumb_on_mousemove(ui_widget_t *thumb, ui_event_t *e,
                                            void *arg)
{
        ui_widget_t *w = e->data;
        ui_scrollbar_t *that = ui_widget_get_data(w, ui_scrollbar_proto);
        css_numeric_value_t pos, size;

        if (!that->is_dragging) {
                return;
        }
        if (that->orientation == UI_SCROLLBAR_HORIZONTAL) {
                size =
                    thumb->parent->content_box.width - thumb->border_box.width;
                pos = that->thumb_x + e->mouse.x - that->mouse_x;
                pos = y_max(0, y_min(pos, size));
                ui_scrollarea_set_scroll_left(
                    w->parent, (ui_scrollarea_get_scroll_width(w->parent) -
                                w->parent->padding_box.width) *
                                   pos / size);
        } else {
                size = thumb->parent->content_box.height -
                       thumb->border_box.height;
                pos = that->thumb_y + e->mouse.y - that->mouse_y;
                pos = y_max(0, y_min(pos, size));
                ui_scrollarea_set_scroll_top(
                    w->parent, (ui_scrollarea_get_scroll_height(w->parent) -
                                w->parent->padding_box.height) *
                                   pos / size);
        }
}

static void ui_scrollbar_thumb_on_mouseup(ui_widget_t *thumb, ui_event_t *e,
                                          void *arg)
{
        ui_widget_t *w = e->data;
        ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

        ui_widget_off(thumb, "mousemove", ui_scrollbar_thumb_on_mousemove, w);
        ui_widget_off(thumb, "mouseup", ui_scrollbar_thumb_on_mouseup, w);
        ui_widget_release_mouse_capture(thumb);
        scrollbar->is_dragging = false;
}

static void Ui_scrollbar_thumb_on_mousedown(ui_widget_t *thumb, ui_event_t *e,
                                            void *arg)
{
        ui_widget_t *w = e->data;
        ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

        if (scrollbar->is_dragging) {
                return;
        }
        scrollbar->thumb_x = thumb->border_box.x;
        scrollbar->thumb_y = thumb->border_box.y;
        scrollbar->mouse_x = e->mouse.x;
        scrollbar->mouse_y = e->mouse.y;
        scrollbar->is_dragging = true;
        ui_widget_set_mouse_capture(thumb);
        ui_widget_on(thumb, "mousemove", ui_scrollbar_thumb_on_mousemove, w);
        ui_widget_on(thumb, "mouseup", ui_scrollbar_thumb_on_mouseup, w);
}

static void ui_scrollbar_on_scroll(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_scrollbar_update(e->data);
}

static void ui_scrollbar_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
        if (!ui_check_widget_type(w->parent, "scrollarea")) {
                logger_error("<scrollbar> must be a child of <scrollarea>\n");
                return;
        }
        ui_widget_on(w->parent, "scroll", ui_scrollbar_on_scroll, w);
}

void ui_scrollbar_set_orientation(ui_widget_t *w,
                                  ui_scrollbar_orientation_t orientation)
{
        ui_scrollbar_t *scrollbar = ui_widget_get_data(w, ui_scrollbar_proto);

        if (orientation == UI_SCROLLBAR_HORIZONTAL) {
                ui_widget_add_class(w, "horizontal");
                ui_widget_remove_class(w, "vertical");
        } else {
                ui_widget_add_class(w, "vertical");
                ui_widget_remove_class(w, "horizontal");
        }
        scrollbar->orientation = orientation;
}

static void ui_scrollbar_on_set_attr(ui_widget_t *w, const char *name,
                                     const char *value)
{
        if (strcmp(name, "orientation") == 0) {
                if (strcmp(value, "horizontal") == 0) {
                        ui_scrollbar_set_orientation(w,
                                                     UI_SCROLLBAR_HORIZONTAL);
                } else {
                        ui_scrollbar_set_orientation(w, UI_SCROLLBAR_VERTICAL);
                }
        }
}

static void ui_scrollbar_on_init(ui_widget_t *w)
{
        ui_widget_t *track;
        ui_widget_t *corner;
        ui_scrollbar_t *that = UI_WDIGET_ADD_DATA(w, ui_scrollbar);

        that->orientation = UI_SCROLLBAR_VERTICAL;
        that->is_dragging = false;
        that->is_draggable = false;
        that->thumb = ui_create_widget("scrollbar-thumb");
        track = ui_create_widget("scrollbar-track");
        corner = ui_create_widget("scrollbar-corner");
        ui_widget_on(that->thumb, "mousedown", Ui_scrollbar_thumb_on_mousedown,
                     w);
        ui_widget_on(w, "link", ui_scrollbar_on_link, NULL);
        ui_widget_append(track, that->thumb);
        ui_widget_append(w, track);
        ui_widget_append(w, corner);

        ui_scrollbar_set_orientation(w, UI_SCROLLBAR_VERTICAL);
}

ui_widget_t *ui_create_scrollbar(void)
{
        return ui_create_widget_with_prototype(ui_scrollbar_proto);
}

void ui_register_scrollbar(void)
{
        ui_create_widget_prototype("scrollbar-thumb", NULL);
        ui_create_widget_prototype("scrollbar-corner", NULL);
        ui_create_widget_prototype("scrollbar-track", NULL);
        ui_scrollbar_proto = ui_create_widget_prototype("scrollbar", NULL);
        ui_scrollbar_proto->init = ui_scrollbar_on_init;
        ui_scrollbar_proto->setattr = ui_scrollbar_on_set_attr;
        ui_load_css_string(ui_scrollbar_css, __FILE__);
}
