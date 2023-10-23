#include <pandagl.h>
#include <math.h>
#include <ui/base.h>
#include <css/computed.h>
#include "ui_widget_box.h"

#define SHADOW_WIDTH(s) (s->box_shadow_blur + s->box_shadow_spread)

static void ui_widget_update_canvas_box_width(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;
        const float x = fabsf(s->box_shadow_x);

        w->canvas_box.width =
            w->border_box.width + SHADOW_WIDTH(s) + y_max(x, SHADOW_WIDTH(s));
}

static void ui_widget_update_canvas_box_height(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;
        const float y = fabsf(s->box_shadow_x);

        w->canvas_box.height =
            w->border_box.height + SHADOW_WIDTH(s) + y_max(y, SHADOW_WIDTH(s));
}

static void ui_widget_update_canvas_box_x(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;

        w->canvas_box.x =
            w->border_box.x - y_max(0, SHADOW_WIDTH(s) - s->box_shadow_x);
}

static void ui_widget_update_canvas_box_y(ui_widget_t *w)
{
        const css_computed_style_t *s = &w->computed_style;

        w->canvas_box.y =
            w->border_box.y - y_max(0, SHADOW_WIDTH(s) - s->box_shadow_y);
}

void ui_widget_update_box_position(ui_widget_t *w)
{
        float x = w->layout_x;
        float y = w->layout_y;
        const css_computed_style_t *s = &w->computed_style;

        switch (w->computed_style.type_bits.position) {
        case CSS_POSITION_ABSOLUTE:
        case CSS_POSITION_FIXED:
                if (IS_CSS_FIXED_LENGTH(s, left)) {
                        x = s->left;
                } else if (IS_CSS_FIXED_LENGTH(s, right)) {
                        if (w->parent) {
                                x = w->parent->border_box.width -
                                    w->border_box.width;
                        }
                        x -= w->computed_style.right;
                } else {
                        x = 0;
                }
                if (IS_CSS_FIXED_LENGTH(s, top)) {
                        y = w->computed_style.top;
                } else if (IS_CSS_FIXED_LENGTH(s, bottom)) {
                        if (w->parent) {
                                y = w->parent->border_box.height -
                                    w->border_box.height;
                        }
                        y -= w->computed_style.bottom;
                } else {
                        y = 0;
                }
                break;
        case CSS_POSITION_RELATIVE:
                if (IS_CSS_FIXED_LENGTH(s, left)) {
                        x += s->left;
                } else if (IS_CSS_FIXED_LENGTH(s, right)) {
                        x -= s->right;
                }
                if (IS_CSS_FIXED_LENGTH(s, top)) {
                        y += s->top;
                } else if (IS_CSS_FIXED_LENGTH(s, bottom)) {
                        y -= s->bottom;
                }
        case CSS_POSITION_STATIC:
        default:
                break;
        }
        w->outer_box.x = x;
        w->outer_box.y = y;
        w->border_box.x = x + s->margin_left;
        w->border_box.y = y + s->margin_top;
        w->padding_box.x = w->border_box.x + s->border_left_width;
        w->padding_box.y = w->border_box.y + s->border_top_width;
        w->content_box.x = w->padding_box.x + s->padding_left;
        w->content_box.y = w->padding_box.y + s->padding_top;
        ui_widget_update_canvas_box_x(w);
        ui_widget_update_canvas_box_y(w);
}

void ui_widget_update_canvas_box(ui_widget_t *w)
{
        ui_widget_update_canvas_box_x(w);
        ui_widget_update_canvas_box_y(w);
        ui_widget_update_canvas_box_width(w);
        ui_widget_update_canvas_box_height(w);
}

void ui_widget_update_box_size(ui_widget_t *w)
{
        css_computed_style_t *s = &w->computed_style;
        css_numeric_value_t limit;
        css_unit_t unit;

        if (css_computed_min_width(s, &limit, &unit) == CSS_MIN_WIDTH_SET &&
            unit == CSS_UNIT_PX && s->width < limit) {
                s->width = limit;
        }
        if (css_computed_max_width(s, &limit, &unit) == CSS_MAX_WIDTH_SET &&
            unit == CSS_UNIT_PX && s->width > limit) {
                s->width = limit;
        }
        if (css_computed_min_height(s, &limit, &unit) == CSS_MIN_HEIGHT_SET &&
            unit == CSS_UNIT_PX && s->height < limit) {
                s->height = limit;
        }
        if (css_computed_max_height(s, &limit, &unit) == CSS_MAX_HEIGHT_SET &&
            unit == CSS_UNIT_PX && s->height > limit) {
                s->height = limit;
        }
        if (s->type_bits.box_sizing == CSS_BOX_SIZING_CONTENT_BOX) {
                w->content_box.width = s->width;
                w->content_box.height = s->height;
        } else {
                w->content_box.width =
                    s->width - (s->padding_left + s->padding_right +
                                s->border_left_width + s->border_right_width);
                w->content_box.height =
                    s->height - (s->padding_top + s->padding_bottom +
                                 s->border_top_width + s->border_bottom_width);
        }
        w->border_box.width = w->content_box.width +
                              (s->padding_left + s->padding_right +
                               s->border_left_width + s->border_right_width);
        w->border_box.height = w->content_box.height +
                               (s->padding_top + s->padding_bottom +
                                s->border_top_width + s->border_bottom_width);
        w->padding_box.width =
            w->content_box.width + s->padding_left + s->padding_right;
        w->padding_box.height =
            w->content_box.height + s->padding_top + s->padding_bottom;
        w->outer_box.width =
            w->border_box.width + s->margin_left + s->margin_right;
        w->outer_box.height =
            w->border_box.height + s->margin_top + s->margin_bottom;
        ui_widget_update_canvas_box_width(w);
        ui_widget_update_canvas_box_height(w);
}

void ui_widget_set_content_box_size(ui_widget_t *w, float width, float height)
{
        css_computed_style_t *s = &w->computed_style;

        CSS_SET_FIXED_LENGTH(s, width, css_convert_content_box_width(s, width));
        CSS_SET_FIXED_LENGTH(s, height,
                             css_convert_content_box_height(s, height));
        ui_widget_update_box_size(w);
        w->proto->resize(w, w->content_box.width, w->content_box.height);
}
