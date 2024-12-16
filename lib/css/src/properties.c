/*
 * lib/css/src/properties.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <css/properties.h>
#include <css/value.h>
#include <css/style_value.h>
#include <css/computed.h>

#define DEFINE_PROP(PROP_KEY, PROP_NAME, VALDEF, INIT)                         \
        extern int css_cascade_##PROP_KEY(const css_style_array_value_t,       \
                                          css_computed_style_t *);             \
        css_register_property_with_key(css_prop_##PROP_KEY, PROP_NAME, VALDEF, \
                                       INIT, css_cascade_##PROP_KEY)

#define DEFINE_SHORTHAND_PROP(PROP_KEY, NAME, VALDEF)                  \
        extern int css_parse_##PROP_KEY(css_propdef_t *, const char *, \
                                        css_style_decl_t *);           \
        css_register_shorthand_property(NAME, VALDEF, css_parse_##PROP_KEY)

static struct css_properties_module {
        /**
         * 样式属性列表
         * css_propdef_t*[]
         */
        css_propdef_t **list;
        unsigned list_length;

        /**
         * 简写样式属性列表
         * css_propdef_t*[]
         */
        css_propdef_t **shorthand_list;
        unsigned shorthand_list_length;

        /**
         * 样式属性表，以名称索引
         * dict_t<string, css_propdef_t>
         */
        dict_t *map;
} css_properties;

static void css_propdef_destroy(css_propdef_t *prop)
{
        if (prop->name) {
                free(prop->name);
        }
        css_valdef_destroy(prop->valdef);
        css_style_value_destroy(&prop->initial_value);
        prop->name = NULL;
        prop->valdef = NULL;
        free(prop);
}

int css_register_shorthand_property(const char *name, const char *syntax,
                                    int (*parse)(css_propdef_t *, const char *,
                                                 css_style_decl_t *))
{
        css_propdef_t *prop;
        css_propdef_t **props;

        prop = calloc(1, sizeof(css_propdef_t));
        if (!prop) {
                return -ENOMEM;
        }
        prop->valdef = css_compile_valdef(syntax);
        if (!prop->valdef) {
                logger_error("[css] [shorthand proeprty: %s] invalid value "
                             "definition: %s\n",
                             name, syntax);
                free(prop);
                return -1;
        }
        props = realloc(css_properties.shorthand_list,
                        (css_properties.shorthand_list_length + 1) *
                            sizeof(css_propdef_t *));
        if (!props) {
                free(prop);
                return -ENOMEM;
        }
        prop->key = -1;
        prop->parse = parse;
        prop->name = strdup2(name);
        props[css_properties.shorthand_list_length] = prop;
        css_properties.shorthand_list_length++;
        css_properties.shorthand_list = props;
        dict_add(css_properties.map, prop->name, prop);
        return 0;
}

static int css_register_property_with_key(
    unsigned key, const char *name, const char *syntax,
    const char *initial_value,
    int (*cascade)(const css_style_array_value_t, css_computed_style_t *))
{
        unsigned i;
        css_propdef_t *prop;
        css_propdef_t **props;

        prop = calloc(1, sizeof(css_propdef_t));
        if (!prop) {
                return -ENOMEM;
        }
        prop->valdef = css_compile_valdef(syntax);
        if (!prop->valdef) {
                logger_error(
                    "[css] [proeprty: %s] invalid value definition: %s\n", name,
                    syntax);
                free(prop);
                return -1;
        }
        if (css_parse_value(prop->valdef, initial_value, &prop->initial_value) <
            0) {
                logger_error("[css] [property: %s] invalid initial value: %s\n",
                             name, initial_value);
                css_valdef_destroy(prop->valdef);
                free(prop);
                return -1;
        }
        prop->name = strdup2(name);
        prop->key = key;
        prop->cascade = cascade;
        if (key >= css_properties.list_length) {
                props = realloc(css_properties.list,
                                (key + 1) * sizeof(css_propdef_t *));
                if (!props) {
                        css_propdef_destroy(prop);
                        return -ENOMEM;
                }
                for (i = css_properties.list_length; i <= key; ++i) {
                        props[i] = NULL;
                }
                css_properties.list = props;
                css_properties.list_length = key + 1;
        }
        css_properties.list[prop->key] = prop;
        dict_add(css_properties.map, prop->name, prop);
        return prop->key;
}

int css_register_property(const char *name, const char *definition,
                          const char *initial_value,
                          int (*cascade)(const css_style_array_value_t,
                                         css_computed_style_t *))
{
        return css_register_property_with_key((int)css_properties.list_length,
                                              name, definition, initial_value,
                                              cascade);
}

css_propdef_t *css_get_propdef_by_name(const char *name)
{
        return dict_fetch_value(css_properties.map, name);
}

css_propdef_t *css_get_propdef(int key)
{
        if (key >= 0 && (size_t)key < css_properties.list_length) {
                return css_properties.list[key];
        }
        return NULL;
}

unsigned css_get_prop_count(void)
{
        return css_properties.list_length;
}

void css_init_properties(void)
{
        static dict_type_t dt = { 0 };

        dict_init_string_key_type(&dt);
        css_properties.map = dict_create(&dt, NULL);
        css_properties.list = NULL;
        css_properties.list_length = 0;
        css_properties.shorthand_list = NULL;
        css_properties.shorthand_list_length = 0;

        css_register_valdef_alias("line-width", "<length>");
        css_register_valdef_alias("line-style", "none | solid");
        css_register_valdef_alias("length-percentage",
                                  "<length> | <percentage>");
        css_register_valdef_alias("shadow", "<length>{2,4} && <color>?");
        css_register_valdef_alias(
            "content-position", "center | start | end | flex-start | flex-end");
        css_register_valdef_alias(
            "content-distribution",
            "space-between | space-around | space-evenly | stretch");
        css_register_valdef_alias("image", "<url>");
        css_register_valdef_alias("bg-image", "none | <image>");
        css_register_valdef_alias(
            "bg-position",
            "["
            "  [ left | center | right | top | bottom | <length-percentage> ] |"
            "  [ left | center | right | <length-percentage> ]"
            "    [ top | center | bottom | <length-percentage> ] |"
            "  [ center | [ left | right ] <length-percentage>? ] &&"
            "    [ center | [ top | bottom ] <length-percentage>? ]"
            "]");
        css_register_valdef_alias(
            "bg-size",
            "[ <length> | <percentage> | auto ]{1,2} | cover | contain");
        css_register_valdef_alias("repeat-style",
                                  "repeat-x | repeat-x | repeat | no-repeat");
        css_register_valdef_alias("flex-grow", "<number>");
        css_register_valdef_alias("flex-shrink", "<number>");
        css_register_valdef_alias("flex-basis",
                                  "auto | content | <length-percentage>");
        css_register_valdef_alias("margin-top", "auto | <length-percentage>");
        css_register_valdef_alias("padding-top", "auto | <length-percentage>");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/visibility */
        DEFINE_PROP(visibility, "visibility", "visible | hidden", "visible");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/width */
        DEFINE_PROP(width, "width", "auto | <length> | <percentage>", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/height */
        DEFINE_PROP(height, "height", "auto | <length> | <percentage>", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/min-width */
        DEFINE_PROP(min_width, "min-width", "auto | <length> | <percentage>",
                    "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/min-height */
        DEFINE_PROP(min_height, "min-height", "auto | <length> | <percentage>",
                    "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/max-width */
        DEFINE_PROP(max_width, "max-width", "auto | <length> | <percentage>",
                    "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/max-height */
        DEFINE_PROP(max_height, "max-height", "auto | <length> | <percentage>",
                    "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/display */
        DEFINE_PROP(display, "display", "none | block | inline-block | flex",
                    "block");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/z-index */
        DEFINE_PROP(z_index, "z-index", "auto | <integer>", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/top */
        DEFINE_PROP(top, "top", "<length> | <percentage> | auto", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSSright */
        DEFINE_PROP(right, "right", "<length> | <percentage> | auto", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/left */
        DEFINE_PROP(left, "left", "<length> | <percentage> | auto", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/bottom */
        DEFINE_PROP(bottom, "bottom", "<length> | <percentage> | auto", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/position */
        DEFINE_PROP(position, "position", "static | relative | absolute",
                    "static");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/opacity */
        DEFINE_PROP(opacity, "opacity", "<number> | <percentage>", "1");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/vertical-align
         */
        DEFINE_PROP(vertical_align, "vertical-align", "middle | bottom | top",
                    "top");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background */
        DEFINE_SHORTHAND_PROP(
            background, "background",
            "<bg-image> || <bg-position> || <bg-size> || <repeat-style> || "
            "<color>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-color */
        DEFINE_PROP(background_color, "background-color", "<color>",
                    "transparent");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-clip */
        DEFINE_PROP(background_clip, "background-clip", "border-box | padding-box | content-box",
                    "border-box");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-position
         */
        DEFINE_SHORTHAND_PROP(background_position, "background-position",
                              "<bg-position>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-position-x
         */
        DEFINE_PROP(background_position_x, "background-position-x",
                    "center | left | right | <length-percentage>", "0%");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-position-y
         */
        DEFINE_PROP(background_position_y, "background-position-y",
                    "center | top | bottom | <length-percentage>", "0%");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-repeat
         */
        DEFINE_PROP(background_repeat, "background-repeat", "<repeat-style>",
                    "repeat");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-size */
        DEFINE_PROP(background_size, "background-size", "<bg-size>",
                    "auto auto");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/background-image */
        DEFINE_PROP(background_image, "background-image", "none | <image>",
                    "none");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/padding-left
         */
        DEFINE_PROP(padding_left, "padding-left", "<padding-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/padding-right
         */
        DEFINE_PROP(padding_right, "padding-right", "<padding-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/padding-top */
        DEFINE_PROP(padding_top, "padding-top", "<padding-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/padding-bottom
         */
        DEFINE_PROP(padding_bottom, "padding-bottom", "<padding-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/padding
         */
        DEFINE_SHORTHAND_PROP(padding, "padding", "<padding-top>{1,4}");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/margin-left */
        DEFINE_PROP(margin_left, "margin-left", "<margin-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/margin-right
         */
        DEFINE_PROP(margin_right, "margin-right", "<margin-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/margin-top */
        DEFINE_PROP(margin_top, "margin-top", "<margin-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/margin-bottom
         */
        DEFINE_PROP(margin_bottom, "margin-bottom", "<margin-top>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/margin
         */
        DEFINE_SHORTHAND_PROP(margin, "margin", "<margin-top>{1,4}");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-color */
        DEFINE_PROP(border_top_color, "border-top-color", "<color>",
                    "transparent");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-right-color
         */
        DEFINE_PROP(border_right_color, "border-right-color", "<color>",
                    "transparent");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-color
         */
        DEFINE_PROP(border_bottom_color, "border-bottom-color", "<color>",
                    "transparent");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-left-color */
        DEFINE_PROP(border_left_color, "border-left-color", "<color>",
                    "transparent");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-color */
        DEFINE_SHORTHAND_PROP(border_color, "border-color", "<color>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-width */
        DEFINE_PROP(border_top_width, "border-top-width", "<line-width>", "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-right-width
         */
        DEFINE_PROP(border_right_width, "border-right-width", "<line-width>",
                    "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-width
         */
        DEFINE_PROP(border_bottom_width, "border-bottom-width", "<line-width>",
                    "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-left-width */
        DEFINE_PROP(border_left_width, "border-left-width", "<line-width>",
                    "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-width */
        DEFINE_SHORTHAND_PROP(border_width, "border-width",
                              "<line-width>{1,4}");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-style */
        DEFINE_PROP(border_top_style, "border-top-style", "<line-style>",
                    "none");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-right-style
         */
        DEFINE_PROP(border_right_style, "border-right-style", "<line-style>",
                    "none");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-style
         */
        DEFINE_PROP(border_bottom_style, "border-bottom-style", "<line-style>",
                    "none");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-left-style */
        DEFINE_PROP(border_left_style, "border-left-style", "<line-style>",
                    "none");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-style */
        DEFINE_SHORTHAND_PROP(border_style, "border-style",
                              "<line-style>{1,4}");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-left-radius
         */
        DEFINE_PROP(border_top_left_radius, "border-top-left-radius",
                    "<length>", "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-top-right-radius
         */
        DEFINE_PROP(border_top_right_radius, "border-top-right-radius",
                    "<length>", "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-left-radius
         */
        DEFINE_PROP(border_bottom_left_radius, "border-bottom-left-radius",
                    "<length>", "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom-right-radius
         */
        DEFINE_PROP(border_bottom_right_radius, "border-bottom-right-radius",
                    "<length>", "0");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-radius
         */
        DEFINE_SHORTHAND_PROP(border_radius, "border-radius", "<length>{1,4}");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-top */
        DEFINE_SHORTHAND_PROP(border_top, "border-top",
                              "<line-width> || <line-style> || <color>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-right */
        DEFINE_SHORTHAND_PROP(border_right, "border-right",
                              "<line-width> || <line-style> || <color>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-bottom */
        DEFINE_SHORTHAND_PROP(border_bottom, "border-bottom",
                              "<line-width> || <line-style> || <color>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border-left */
        DEFINE_SHORTHAND_PROP(border_left, "border-left",
                              "<line-width> || <line-style> || <color>");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/border
         */
        DEFINE_SHORTHAND_PROP(border, "border",
                              "<line-width> || <line-style> || <color>");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/box-shadow */
        DEFINE_PROP(box_shadow, "box-shadow", "none | <shadow>", "none");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/pointer-events
         */
        DEFINE_PROP(pointer_events, "pointer-events", "auto | none", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/box-sizing */
        DEFINE_PROP(box_sizing, "box-sizing", "content-box | border-box",
                    "content-box");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/flex */
        DEFINE_SHORTHAND_PROP(flex, "flex",
                              "initial | none | [ <flex-grow> || <flex-shrink> "
                              "|| <flex-basis> ]");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/flex-basis */
        DEFINE_PROP(flex_basis, "flex-basis", "auto | <length>", "auto");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/flex-direction
         */
        DEFINE_PROP(flex_direction, "flex-direction", "row | column", "row");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/flex-grow */
        DEFINE_PROP(flex_grow, "flex-grow", "<flex-grow>", "0");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/flex-shrink */
        DEFINE_PROP(flex_shrink, "flex-shrink", "<flex-shrink>", "1");

        /** @see https://developer.mozilla.org/en-US/do/cs/Web/CSS/flex-wrap */
        DEFINE_PROP(flex_wrap, "flex-wrap", "nowrap | wrap", "nowrap");

        /** @see
         * https://developer.mozilla.org/en-US/docs/Web/CSS/justify-content */
        DEFINE_PROP(justify_content, "justify-content",
                    "normal | <content-distribution> | <content-position>",
                    "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/align-content
         */
        DEFINE_PROP(align_content, "align-content",
                    "normal | <content-distribution> | <content-position>",
                    "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/align-items */
        DEFINE_PROP(align_items, "align-items",
                    "normal | stretch | flex-start | flex-end | center",
                    "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/color */
        DEFINE_PROP(color, "color", "<color>", "#000");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/font-family */
        DEFINE_PROP(font_family, "font-family", "<font-family>", "system-ui");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/font-size */
        DEFINE_PROP(font_size, "font-size",
                    "small | medium | large | <length> | <percentage>",
                    "medium");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/font-style */
        DEFINE_PROP(font_style, "font-style", "normal | italic | oblique",
                    "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight */
        DEFINE_PROP(font_weight, "font-weight", "normal | bold | <number>",
                    "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/text-align */
        DEFINE_PROP(text_align, "text-align", "left | center | right", "left");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/line-height */
        DEFINE_PROP(line_height, "line-height",
                    "normal | <number> | <length-percentage>", "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/content */
        DEFINE_PROP(content, "content", "<string> | none", "none");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/white-space */
        DEFINE_PROP(white_space, "white-space", "normal | nowrap", "normal");

        /** @see https://developer.mozilla.org/en-US/docs/Web/CSS/word-break */
        DEFINE_PROP(word_break, "word-break", "normal | break-all", "normal");
}

void css_destroy_properties(void)
{
        unsigned i;

        dict_destroy(css_properties.map);
        for (i = 0; i < css_properties.list_length; ++i) {
                if (css_properties.list[i]) {
                        css_propdef_destroy(css_properties.list[i]);
                        css_properties.list[i] = NULL;
                }
        }
        for (i = 0; i < css_properties.shorthand_list_length; ++i) {
                if (css_properties.shorthand_list[i]) {
                        css_propdef_destroy(css_properties.shorthand_list[i]);
                        css_properties.shorthand_list[i] = NULL;
                }
        }
        free(css_properties.list);
        free(css_properties.shorthand_list);
        css_properties.map = NULL;
        css_properties.list = NULL;
        css_properties.list_length = 0;
        css_properties.shorthand_list = NULL;
        css_properties.shorthand_list_length = 0;
}
