
#ifndef LIBCSS_INCLUDE_CSS_COMPUTED_H
#define LIBCSS_INCLUDE_CSS_COMPUTED_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

#define IS_CSS_LENGTH(S, PROP_KEY) (S)->type_bits.PROP_KEY == CSS_LENGTH_SET

#define IS_CSS_FIXED_LENGTH(S, PROP_KEY)              \
        ((S)->type_bits.PROP_KEY == CSS_LENGTH_SET && \
         (S)->unit_bits.PROP_KEY == CSS_UNIT_PX)

#define IS_CSS_PERCENTAGE(S, PROP_KEY)                \
        ((S)->type_bits.PROP_KEY == CSS_LENGTH_SET && \
         (S)->unit_bits.PROP_KEY == CSS_UNIT_PERCENT)

#define CSS_SET_FIXED_LENGTH(S, PROP_KEY, PROP_VALUE)     \
        do {                                              \
                (S)->PROP_KEY = PROP_VALUE;               \
                (S)->type_bits.PROP_KEY = CSS_LENGTH_SET; \
                (S)->unit_bits.PROP_KEY = CSS_UNIT_PX;    \
        } while (0);

#define CSS_COPY_LENGTH(DEST, SRC, PROP_KEY)                            \
        do {                                                            \
                (DEST)->PROP_KEY = (SRC)->PROP_KEY;                     \
                (DEST)->type_bits.PROP_KEY = (SRC)->type_bits.PROP_KEY; \
                (DEST)->unit_bits.PROP_KEY = (SRC)->unit_bits.PROP_KEY; \
        } while (0);

LIBCSS_INLINE libcss_bool_t is_css_display_inline(const css_computed_style_t *s)
{
        return s->type_bits.display == CSS_DISPLAY_INLINE ||
               s->type_bits.display == CSS_DISPLAY_INLINE_BLOCK ||
               s->type_bits.display == CSS_DISPLAY_INLINE_FLEX;
}

LIBCSS_INLINE libcss_bool_t is_css_display_flex(const css_computed_style_t *s)
{
        return s->type_bits.display == CSS_DISPLAY_FLEX ||
               s->type_bits.display == CSS_DISPLAY_INLINE_FLEX;
}

LIBCSS_INLINE css_numeric_value_t css_padding_x(const css_computed_style_t *s)
{
        return s->padding_left + s->padding_right;
}

LIBCSS_INLINE css_numeric_value_t css_padding_y(const css_computed_style_t *s)
{
        return s->padding_top + s->padding_bottom;
}

LIBCSS_INLINE css_numeric_value_t css_margin_x(const css_computed_style_t *s)
{
        return s->margin_left + s->margin_right;
}

LIBCSS_INLINE css_numeric_value_t css_margin_y(const css_computed_style_t *s)
{
        return s->margin_top + s->margin_bottom;
}

LIBCSS_INLINE css_numeric_value_t css_border_x(const css_computed_style_t *s)
{
        return s->border_left_width + s->border_right_width;
}

LIBCSS_INLINE css_numeric_value_t css_border_y(const css_computed_style_t *s)
{
        return s->border_top_width + s->border_bottom_width;
}

LIBCSS_PUBLIC uint8_t css_computed_padding_top(const css_computed_style_t *s,
                                               css_numeric_value_t *value,
                                               css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_padding_right(const css_computed_style_t *s,
                                                 css_numeric_value_t *value,
                                                 css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_padding_bottom(const css_computed_style_t *s,
                                                  css_numeric_value_t *value,
                                                  css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_padding_left(const css_computed_style_t *s,
                                                css_numeric_value_t *value,
                                                css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_margin_top(const css_computed_style_t *s,
                                              css_numeric_value_t *value,
                                              css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_margin_right(const css_computed_style_t *s,
                                                css_numeric_value_t *value,
                                                css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_margin_bottom(const css_computed_style_t *s,
                                                 css_numeric_value_t *value,
                                                 css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_margin_left(const css_computed_style_t *s,
                                               css_numeric_value_t *value,
                                               css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_top(const css_computed_style_t *s,
                                       css_numeric_value_t *value,
                                       css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_right(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_bottom(const css_computed_style_t *s,
                                          css_numeric_value_t *value,
                                          css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_left(const css_computed_style_t *s,
                                        css_numeric_value_t *value,
                                        css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_display(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t
css_computed_vertical_align(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t css_computed_box_sizing(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t css_computed_position(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t css_computed_min_width(const css_computed_style_t *s,
                                             css_numeric_value_t *value,
                                             css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_min_height(const css_computed_style_t *s,
                                              css_numeric_value_t *value,
                                              css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_max_width(const css_computed_style_t *s,
                                             css_numeric_value_t *value,
                                             css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_max_height(const css_computed_style_t *s,
                                              css_numeric_value_t *value,
                                              css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_width(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_height(const css_computed_style_t *s,
                                          css_numeric_value_t *value,
                                          css_unit_t *unit);

LIBCSS_PUBLIC uint8_t
css_computed_border_top_style(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t
css_computed_border_right_style(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t
css_computed_border_bottom_style(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t
css_computed_border_left_style(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t css_computed_border_top_color(
    const css_computed_style_t *s, css_color_value_t *value);

LIBCSS_PUBLIC uint8_t css_computed_border_right_color(
    const css_computed_style_t *s, css_color_value_t *value);

LIBCSS_PUBLIC uint8_t css_computed_border_bottom_color(
    const css_computed_style_t *s, css_color_value_t *value);

LIBCSS_PUBLIC uint8_t css_computed_border_left_color(
    const css_computed_style_t *s, css_color_value_t *value);

LIBCSS_PUBLIC uint8_t css_computed_background_position_x(
    const css_computed_style_t *s, css_numeric_value_t *value,
    css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_background_position_y(
    const css_computed_style_t *s, css_numeric_value_t *value,
    css_unit_t *unit);

LIBCSS_PUBLIC uint8_t
css_computed_background_width(const css_computed_style_t *s,
                              css_numeric_value_t *value, css_unit_t *unit);

LIBCSS_PUBLIC uint8_t
css_computed_background_height(const css_computed_style_t *s,
                               css_numeric_value_t *value, css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_background_image(
    const css_computed_style_t *s, css_image_value_t *value);

LIBCSS_PUBLIC uint8_t css_computed_box_shadow_x(const css_computed_style_t *s,
                                                css_numeric_value_t *value,
                                                css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_box_shadow_y(const css_computed_style_t *s,
                                                css_numeric_value_t *value,
                                                css_unit_t *unit);

LIBCSS_PUBLIC uint8_t
css_computed_box_shadow_blur(const css_computed_style_t *s,
                             css_numeric_value_t *value, css_unit_t *unit);

LIBCSS_PUBLIC uint8_t
css_computed_box_shadow_spread(const css_computed_style_t *s,
                               css_numeric_value_t *value, css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_box_shadow_color(
    const css_computed_style_t *s, css_color_value_t *value);

LIBCSS_PUBLIC uint8_t css_computed_visibility(const css_computed_style_t *s);

LIBCSS_PUBLIC void css_computed_style_destroy(css_computed_style_t *s);

LIBCSS_PUBLIC int css_cascade_style(const css_style_decl_t *style,
                                    css_computed_style_t *computed);

LIBCSS_PUBLIC void css_compute_absolute_values(
    const css_computed_style_t *parent, css_computed_style_t *s,
    css_metrics_t *m);

LIBCSS_INLINE css_numeric_value_t css_convert_content_box_width(
    css_computed_style_t *s, css_numeric_value_t content_width)
{
        if (css_computed_box_sizing(s) == CSS_BOX_SIZING_BORDER_BOX) {
                return content_width + css_border_x(s) + css_padding_x(s);
        }
        return content_width;
}

LIBCSS_INLINE css_numeric_value_t css_convert_content_box_height(
    css_computed_style_t *s, css_numeric_value_t content_height)
{
        if (css_computed_box_sizing(s) == CSS_BOX_SIZING_BORDER_BOX) {
                return content_height + css_border_y(s) + css_padding_y(s);
        }
        return content_height;
}

LIBCSS_INLINE css_numeric_value_t css_convert_border_box_width(
    css_computed_style_t *s, css_numeric_value_t border_width)
{
        if (css_computed_box_sizing(s) == CSS_BOX_SIZING_CONTENT_BOX) {
                return border_width - css_border_x(s) - css_padding_x(s);
        }
        return border_width;
}

LIBCSS_INLINE css_numeric_value_t css_convert_border_box_height(
    css_computed_style_t *s, css_numeric_value_t border_height)
{
        if (css_computed_box_sizing(s) == CSS_BOX_SIZING_CONTENT_BOX) {
                return border_height - css_border_y(s) - css_padding_y(s);
        }
        return border_height;
}

LIBCSS_END_DECLS

#endif
