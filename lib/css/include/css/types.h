/*
 * lib/css/include/css/types.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_DEF_H
#define LIBCSS_INCLUDE_CSS_DEF_H

#include <yutil.h>

#define CSS_SELECTOR_MAX_LEN 1024
#define CSS_SELECTOR_MAX_DEPTH 32

#define css_string(...) "" #__VA_ARGS__ ""

typedef unsigned char libcss_bool_t;

#define LIBCSS_FALSE 0
#define LIBCSS_TRUE 1

typedef enum {
	// position start
	css_prop_left,
	css_prop_right,
	css_prop_top,
	css_prop_bottom,
	css_prop_position,
	// position end

	// display start
	css_prop_visibility,
	css_prop_display,
	// display end

	css_prop_z_index,
	css_prop_opacity,
	css_prop_box_sizing,
	css_prop_width,
	css_prop_height,
	css_prop_min_width,
	css_prop_min_height,
	css_prop_max_width,
	css_prop_max_height,

	// margin start
	css_prop_margin_top,
	css_prop_margin_right,
	css_prop_margin_bottom,
	css_prop_margin_left,
	// margin end

	// padding start
	css_prop_padding_top,
	css_prop_padding_right,
	css_prop_padding_bottom,
	css_prop_padding_left,
	// padding end

	css_prop_vertical_align,

	// border start
	css_prop_border_top_width,
	css_prop_border_top_style,
	css_prop_border_top_color,
	css_prop_border_right_width,
	css_prop_border_right_style,
	css_prop_border_right_color,
	css_prop_border_bottom_width,
	css_prop_border_bottom_style,
	css_prop_border_bottom_color,
	css_prop_border_left_width,
	css_prop_border_left_style,
	css_prop_border_left_color,
	css_prop_border_top_left_radius,
	css_prop_border_top_right_radius,
	css_prop_border_bottom_left_radius,
	css_prop_border_bottom_right_radius,
	// border end

	// background start
	css_prop_background_color,
	css_prop_background_image,
	css_prop_background_size,
	css_prop_background_repeat,
	css_prop_background_position_x,
	css_prop_background_position_y,
	// background end

	css_prop_box_shadow,

	// flex style start
	css_prop_flex_basis,
	css_prop_flex_grow,
	css_prop_flex_shrink,
	css_prop_flex_direction,
	css_prop_flex_wrap,
	css_prop_justify_content,
	css_prop_align_content,
	css_prop_align_items,
	// flex style end

	css_prop_color,
	css_prop_font_size,
	css_prop_font_style,
	css_prop_font_weight,
	css_prop_font_family,
	css_prop_line_height,
	css_prop_text_align,
	css_prop_content,
	css_prop_white_space,
	css_prop_word_break,

	css_prop_pointer_events,
	STYLE_KEY_TOTAL
} css_prop_key_t;

typedef enum {
	CSS_LENGTH_INHERIT,
	CSS_LENGTH_SET,
	CSS_LENGTH_AUTO
} css_length_t;

typedef enum { CSS_COLOR_INHERIT, CSS_COLOR_COLOR } css_color_t;

typedef enum {
	CSS_ALIGN_CONTENT_INHERIT,
	CSS_ALIGN_CONTENT_STRETCH,
	CSS_ALIGN_CONTENT_FLEX_START,
	CSS_ALIGN_CONTENT_FLEX_END,
	CSS_ALIGN_CONTENT_CENTER,
	CSS_ALIGN_CONTENT_SPACE_BETWEEN,
	CSS_ALIGN_CONTENT_SPACE_AROUND,
	CSS_ALIGN_CONTENT_SPACE_EVENLY
} css_align_content_t;

typedef enum {
	CSS_ALIGN_ITEMS_INHERIT,
	CSS_ALIGN_ITEMS_STRETCH,
	CSS_ALIGN_ITEMS_FLEX_START,
	CSS_ALIGN_ITEMS_FLEX_END,
	CSS_ALIGN_ITEMS_CENTER,
	CSS_ALIGN_ITEMS_BASELINE
} css_align_items_t;

typedef enum {
	CSS_BACKGROUND_COLOR_INHERIT,
	CSS_BACKGROUND_COLOR_COLOR,
	CSS_BACKGROUND_COLOR_CURRENT_COLOR
} css_background_color_t;

typedef enum {
	CSS_BACKGROUND_IMAGE_INHERIT,
	/* Consult pointer in struct to determine which */
	CSS_BACKGROUND_IMAGE_NONE,
	CSS_BACKGROUND_IMAGE_IMAGE
} css_background_image_t;

typedef enum {
	CSS_BACKGROUND_SIZE_INHERIT,
	CSS_BACKGROUND_SIZE_SET,
	CSS_BACKGROUND_SIZE_AUTO,
	CSS_BACKGROUND_SIZE_CONVER,
	CSS_BACKGROUND_SIZE_CONTAIN
} css_background_size_t;

typedef enum {
	CSS_BACKGROUND_POSITION_INHERIT,
	CSS_BACKGROUND_POSITION_SET
} css_background_position_t;

typedef enum {
	CSS_BACKGROUND_REPEAT_INHERIT,
	CSS_BACKGROUND_REPEAT_REPEAT_X,
	CSS_BACKGROUND_REPEAT_REPEAT_Y,
	CSS_BACKGROUND_REPEAT_REPEAT,
	CSS_BACKGROUND_REPEAT_NO_REPEAT
} css_background_repeat_t;

typedef enum {
	CSS_BORDER_RADIUS_INHERIT,
	CSS_BORDER_RADIUS_SET
} css_border_radius_t;

typedef enum {
	CSS_BORDER_COLOR_INHERIT = CSS_BACKGROUND_COLOR_INHERIT,
	CSS_BORDER_COLOR_COLOR = CSS_BACKGROUND_COLOR_COLOR,
	CSS_BORDER_COLOR_CURRENT_COLOR = CSS_BACKGROUND_COLOR_CURRENT_COLOR
} css_border_color_t;

typedef enum {
	CSS_BORDER_STYLE_INHERIT,
	CSS_BORDER_STYLE_NONE,
	CSS_BORDER_STYLE_HIDDEN,
	CSS_BORDER_STYLE_DOTTED,
	CSS_BORDER_STYLE_DASHED,
	CSS_BORDER_STYLE_SOLID,
	CSS_BORDER_STYLE_DOUBLE,
	CSS_BORDER_STYLE_GROOVE,
	CSS_BORDER_STYLE_RIDGE,
	CSS_BORDER_STYLE_INSET,
	CSS_BORDER_STYLE_OUTSET
} css_border_style_t;

typedef enum {
	CSS_BORDER_WIDTH_INHERIT,
	CSS_BORDER_WIDTH_THIN,
	CSS_BORDER_WIDTH_MEDIUM,
	CSS_BORDER_WIDTH_THICK,
	CSS_BORDER_WIDTH_WIDTH
} css_border_width_t;

typedef enum {
	CSS_BOX_SHADOW_NONE,
	CSS_BOX_SHADOW_SET,
} css_box_shadow_t;

typedef enum {
	CSS_BOTTOM_INHERIT,
	CSS_BOTTOM_SET,
	CSS_BOTTOM_AUTO
} css_bottom_t;

typedef enum {
	CSS_BOX_SIZING_INHERIT,
	CSS_BOX_SIZING_CONTENT_BOX,
	CSS_BOX_SIZING_BORDER_BOX
} css_box_sizing_t;

typedef enum {
	CSS_CONTENT_INHERIT,
	CSS_CONTENT_NONE,
	CSS_CONTENT_NORMAL,
	CSS_CONTENT_SET
} css_content_t;

typedef enum {
	CSS_DISPLAY_INHERIT,
	CSS_DISPLAY_INLINE,
	CSS_DISPLAY_BLOCK,
	CSS_DISPLAY_LIST_ITEM,
	CSS_DISPLAY_RUN_IN,
	CSS_DISPLAY_INLINE_BLOCK,
	CSS_DISPLAY_TABLE,
	CSS_DISPLAY_INLINE_TABLE,
	CSS_DISPLAY_TABLE_ROW_GROUP,
	CSS_DISPLAY_TABLE_HEADER_GROUP,
	CSS_DISPLAY_TABLE_FOOTER_GROUP,
	CSS_DISPLAY_TABLE_ROWb,
	CSS_DISPLAY_TABLE_COLUMN_GROUP,
	CSS_DISPLAY_TABLE_COLUMN,
	CSS_DISPLAY_TABLE_CELL,
	CSS_DISPLAY_TABLE_CAPTION,
	CSS_DISPLAY_NONE,
	CSS_DISPLAY_FLEX,
	CSS_DISPLAY_INLINE_FLEX
} css_display_t;

typedef enum {
	CSS_FLEX_BASIS_INHERIT,
	CSS_FLEX_BASIS_SET,
	CSS_FLEX_BASIS_AUTO,
	CSS_FLEX_BASIS_CONTENT
} css_flex_basis_t;

typedef enum {
	CSS_FLEX_DIRECTION_INHERIT,
	CSS_FLEX_DIRECTION_ROW,
	CSS_FLEX_DIRECTION_ROW_REVERSE,
	CSS_FLEX_DIRECTION_COLUMN,
	CSS_FLEX_DIRECTION_COLUMN_REVERSE
} css_flex_direction_t;

typedef enum { CSS_FLEX_GROW_INHERIT, CSS_FLEX_GROW_SET } css_flex_grow_t;

typedef enum { CSS_FLEX_SHRINK_INHERIT, CSS_FLEX_SHRINK_SET } css_flex_shrink_t;

typedef enum {
	CSS_FLEX_WRAP_INHERIT,
	CSS_FLEX_WRAP_NOWRAP,
	CSS_FLEX_WRAP_WRAP,
	CSS_FLEX_WRAP_WRAP_REVERSE
} css_flex_wrap_t;

typedef enum {
	CSS_FONT_FAMILY_INHERIT,
	/* Named fonts exist if pointer is non-NULL */
	CSS_FONT_FAMILY_SERIF,
	CSS_FONT_FAMILY_SANS_SERIF,
	CSS_FONT_FAMILY_CURSIVE,
	CSS_FONT_FAMILY_FANTASY,
	CSS_FONT_FAMILY_MONOSPACE
} css_font_family_t;

typedef enum {
	CSS_FONT_SIZE_INHERIT,
	CSS_FONT_SIZE_XX_SMALL,
	CSS_FONT_SIZE_X_SMALL,
	CSS_FONT_SIZE_SMALL,
	CSS_FONT_SIZE_MEDIUM,
	CSS_FONT_SIZE_LARGE,
	CSS_FONT_SIZE_X_LARGE,
	CSS_FONT_SIZE_XX_LARGE,
	CSS_FONT_SIZE_LARGER,
	CSS_FONT_SIZE_SMALLER,
	CSS_FONT_SIZE_DIMENSION
} css_font_size_t;

typedef enum {
	CSS_FONT_STYLE_INHERIT,
	CSS_FONT_STYLE_NORMAL,
	CSS_FONT_STYLE_ITALIC,
	CSS_FONT_STYLE_OBLIQUE
} css_font_style_t;

typedef enum {
	CSS_FONT_WEIGHT_INHERIT,
	CSS_FONT_WEIGHT_NORMAL,
	CSS_FONT_WEIGHT_BOLD,
	CSS_FONT_WEIGHT_BOLDER,
	CSS_FONT_WEIGHT_LIGHTER,
	CSS_FONT_WEIGHT_100,
	CSS_FONT_WEIGHT_200,
	CSS_FONT_WEIGHT_300,
	CSS_FONT_WEIGHT_400,
	CSS_FONT_WEIGHT_500,
	CSS_FONT_WEIGHT_600,
	CSS_FONT_WEIGHT_700,
	CSS_FONT_WEIGHT_800,
	CSS_FONT_WEIGHT_900
} css_font_weight_t;

typedef enum {
	CSS_HEIGHT_INHERIT,
	CSS_HEIGHT_SET,
	CSS_HEIGHT_AUTO,
	CSS_HEIGHT_FIT_CONTENT
} css_height_t;

typedef enum {
	CSS_JUSTIFY_CONTENT_INHERIT,
	CSS_JUSTIFY_CONTENT_FLEX_START,
	CSS_JUSTIFY_CONTENT_FLEX_END,
	CSS_JUSTIFY_CONTENT_CENTER,
	CSS_JUSTIFY_CONTENT_SPACE_BETWEEN,
	CSS_JUSTIFY_CONTENT_SPACE_AROUND,
	CSS_JUSTIFY_CONTENT_SPACE_EVENLY
} css_justify_content_t;

typedef enum { CSS_LEFT_INHERIT, CSS_LEFT_SET, CSS_LEFT_AUTO } css_left_t;
typedef enum {
	CSS_LINE_HEIGHT_INHERIT,
	CSS_LINE_HEIGHT_NUMBER,
	CSS_LINE_HEIGHT_DIMENSION,
	CSS_LINE_HEIGHT_NORMAL
} css_line_height_t;

typedef enum {
	CSS_MARGIN_INHERIT,
	CSS_MARGIN_SET,
	CSS_MARGIN_AUTO
} css_margin_t;

typedef enum {
	CSS_MAX_HEIGHT_INHERIT,
	CSS_MAX_HEIGHT_SET,
	CSS_MAX_HEIGHT_NONE
} css_max_height_t;

typedef enum {
	CSS_MAX_WIDTH_INHERIT,
	CSS_MAX_WIDTH_SET,
	CSS_MAX_WIDTH_NONE
} css_max_width_t;

typedef enum {
	CSS_MIN_HEIGHT_INHERIT,
	CSS_MIN_HEIGHT_SET,
	CSS_MIN_HEIGHT_AUTO
} css_min_height_t;

typedef enum {
	CSS_MIN_WIDTH_INHERIT,
	CSS_MIN_WIDTH_SET,
	CSS_MIN_WIDTH_AUTO
} css_min_width_t;

typedef enum { CSS_OPACITY_INHERIT, CSS_OPACITY_SET } css_opacity_t;

typedef enum { CSS_PADDING_INHERIT, CSS_PADDING_SET } css_padding_t;

typedef enum {
	CSS_POSITION_INHERIT,
	CSS_POSITION_STATIC,
	CSS_POSITION_RELATIVE,
	CSS_POSITION_ABSOLUTE,
	CSS_POSITION_FIXED
} css_position_t;

typedef enum { CSS_RIGHT_INHERIT, CSS_RIGHT_SET, CSS_RIGHT_AUTO } css_right_t;

typedef enum {
	CSS_TEXT_ALIGN_INHERIT,
	CSS_TEXT_ALIGN_INHERIT_IF_NON_MAGIC,
	CSS_TEXT_ALIGN_LEFT,
	CSS_TEXT_ALIGN_RIGHT,
	CSS_TEXT_ALIGN_CENTER,
	CSS_TEXT_ALIGN_JUSTIFY,
	CSS_TEXT_ALIGN_DEFAULT,
	CSS_TEXT_ALIGN_LIBCSS_LEFT,
	CSS_TEXT_ALIGN_LIBCSS_CENTER,
	CSS_TEXT_ALIGN_LIBCSS_RIGHT
} css_text_align_t;

typedef enum { CSS_TOP_INHERIT, CSS_TOP_SET, CSS_TOP_AUTO } css_top_t;
typedef enum {
	CSS_VERTICAL_ALIGN_INHERIT,
	CSS_VERTICAL_ALIGN_BASELINE,
	CSS_VERTICAL_ALIGN_SUB,
	CSS_VERTICAL_ALIGN_SUPER,
	CSS_VERTICAL_ALIGN_TOP,
	CSS_VERTICAL_ALIGN_TEXT_TOP,
	CSS_VERTICAL_ALIGN_MIDDLE,
	CSS_VERTICAL_ALIGN_BOTTOM,
	CSS_VERTICAL_ALIGN_TEXT_BOTTOM,
	CSS_VERTICAL_ALIGN_SET
} css_vertical_align_t;

typedef enum {
	CSS_VISIBILITY_INHERIT,
	CSS_VISIBILITY_VISIBLE,
	CSS_VISIBILITY_HIDDEN,
	CSS_VISIBILITY_COLLAPSE
} css_visibility_t;

typedef enum {
	CSS_WHITE_SPACE_INHERIT,
	CSS_WHITE_SPACE_NORMAL,
	CSS_WHITE_SPACE_PRE,
	CSS_WHITE_SPACE_NOWRAP,
	CSS_WHITE_SPACE_PRE_WRAP,
	CSS_WHITE_SPACE_PRE_LINE
} css_white_space_t;

typedef enum {
	CSS_WORD_BREAK_INHERIT,
	CSS_WORD_BREAK_NORMAL,
	CSS_WORD_BREAK_BREAK_ALL
} css_word_break_t;

typedef enum {
	CSS_WIDTH_INHERIT,
	CSS_WIDTH_SET,
	CSS_WIDTH_AUTO,
	CSS_WIDTH_FIT_CONTENT
} css_width_t;

typedef enum {
	CSS_Z_INDEX_INHERIT,
	CSS_Z_INDEX_SET,
	CSS_Z_INDEX_AUTO
} css_z_index_t;

typedef enum {
	CSS_POINTER_EVENTS_INHERT,
	CSS_POINTER_EVENTS_AUTO,
	CSS_POINTER_EVENTS_NONE
} css_pointer_events_t;

typedef enum css_style_value_type_t {
	CSS_NO_VALUE,
	CSS_INVALID_VALUE,
	CSS_UNPARSED_VALUE,
	CSS_ARRAY_VALUE,

	CSS_NUMERIC_VALUE,
	CSS_STRING_VALUE,
	CSS_KEYWORD_VALUE,
	CSS_COLOR_VALUE,

	CSS_UNIT_VALUE,
	CSS_BOOLEAN_VALUE,
} css_style_value_type_t;

typedef char *css_image_value_t;
typedef char *css_unparsed_value_t;
typedef char *css_string_value_t;
typedef int32_t css_integer_value_t;
typedef float css_numeric_value_t;
typedef uint32_t css_color_value_t;
typedef void *css_private_value_t;
typedef int css_boolean_value_t;

typedef enum {
	CSS_UNIT_PX,
	CSS_UNIT_PERCENT,
	CSS_UNIT_DIP,
	CSS_UNIT_SP,
	CSS_UNIT_PT
} css_unit_t;

typedef int32_t css_unit_ident_t;

// TODO: 优化内存占用

/** https://developer.mozilla.org/en-US/docs/Web/API/CSSUnitValue */
typedef struct css_unit_value_t {
	css_numeric_value_t value;
	css_unit_t unit;
} css_unit_value_t;

typedef struct css_style_value_t css_style_value_t;
typedef css_style_value_t *css_style_array_value_t;
typedef enum css_keyword_value_t {
	CSS_KEYWORD_INHERIT,
	CSS_KEYWORD_INITIAL,
	CSS_KEYWORD_NONE,
	CSS_KEYWORD_AUTO,
	CSS_KEYWORD_NORMAL,

	CSS_KEYWORD_STATIC,
	CSS_KEYWORD_RELATIVE,
	CSS_KEYWORD_ABSOLUTE,

	CSS_KEYWORD_BLOCK,
	CSS_KEYWORD_INLINE_BLOCK,
	CSS_KEYWORD_FLEX,

	CSS_KEYWORD_LEFT,
	CSS_KEYWORD_CENTER,
	CSS_KEYWORD_RIGHT,
	CSS_KEYWORD_TOP,
	CSS_KEYWORD_MIDDLE,
	CSS_KEYWORD_BOTTOM,

	CSS_KEYWORD_ROW,
	CSS_KEYWORD_COLUMN,

	CSS_KEYWORD_START,
	CSS_KEYWORD_END,
	CSS_KEYWORD_FLEX_START,
	CSS_KEYWORD_FLEX_END,
	CSS_KEYWORD_STRETCH,
	CSS_KEYWORD_SPACE_BETWEEN,
	CSS_KEYWORD_SPACE_AROUND,
	CSS_KEYWORD_SPACE_EVENLY,

	CSS_KEYWORD_WRAP,
	CSS_KEYWORD_NOWRAP,

	CSS_KEYWORD_BREAK_ALL,

	CSS_KEYWORD_ITALIC,
	CSS_KEYWORD_OBLIQUE,
	CSS_KEYWORD_SMALL,
	CSS_KEYWORD_MEDIUM,
	CSS_KEYWORD_LARGE,
	CSS_KEYWORD_BOLD,

	CSS_KEYWORD_HIDDEN,
	CSS_KEYWORD_VISIBLE,

	CSS_KEYWORD_CONTENT_BOX,
	CSS_KEYWORD_PADDING_BOX,
	CSS_KEYWORD_BORDER_BOX,

	CSS_KEYWORD_SOLID,
	CSS_KEYWORD_DOTTED,
	CSS_KEYWORD_DOUBLE,
	CSS_KEYWORD_DASHED,

	CSS_KEYWORD_CONTAIN,
	CSS_KEYWORD_COVER,
	CSS_KEYWORD_REPEAT,
	CSS_KEYWORD_NO_REPEAT,
	CSS_KEYWORD_REPEAT_X,
	CSS_KEYWORD_REPEAT_Y
} css_keyword_value_t;

/** https://developer.mozilla.org/en-US/docs/Web/API/CSSStyleValue */
struct css_style_value_t {
	css_style_value_type_t type;
	union {
		css_private_value_t value;
		css_numeric_value_t numeric_value;
		css_integer_value_t integer_value;
		css_string_value_t string_value;
		css_unit_value_t unit_value;
		css_color_value_t color_value;
		css_image_value_t image_value;
		css_unparsed_value_t unparsed_value;
		css_keyword_value_t keyword_value;
		css_style_array_value_t array_value;
		css_boolean_value_t boolean_value;
	};
};

typedef struct css_valdef_t css_valdef_t;
typedef list_t css_style_decl_t;

typedef css_style_decl_t css_style_decl_t;
typedef unsigned css_selector_hash_t;

/** 样式规则记录 */
typedef struct css_style_rule_t {
	int rank;               /**< 权值，决定优先级 */
	int batch_num;          /**< 批次号 */
	char *space;            /**< 所属的空间 */
	char *selector;         /**< 选择器 */
	css_style_decl_t *list; /**< 样式表 */
	list_node_t node;       /**< 在链表中的结点 */
} css_style_rule_t;

typedef struct css_prop_t {
	css_prop_key_t key;
	css_style_value_t value;
	list_node_t node;
} css_prop_t;

typedef struct css_selector_node_t {
	char *id;
	char *type;
	char **classes;
	char **status;
	char *fullname; /**< 全名，由 id、type、classes、status 组合而成 */
	int rank;
} css_selector_node_t;

typedef struct css_selector_t {
	int rank;                    /**< 权值，决定优先级 */
	int batch_num;               /**< 批次号 */
	int length;                  /**< 选择器结点长度 */
	css_selector_hash_t hash;    /**< 哈希值 */
	css_selector_node_t **nodes; /**< 选择器结点列表 */
} css_selector_t;

typedef struct css_font_face_t {
	char *font_family;
	css_font_style_t font_style;
	css_font_weight_t font_weight;
	char *src;
} css_font_face_t;

typedef struct css_metrics_t {
	float dpi;
	float density;
	float scaled_density;
	float scale;
} css_metrics_t;

typedef struct css_computed_style_t {
	/**
	 * 属性值类型的比特数据
	 * 以比特位为单位分配的存储空间，用于节省属性值的内存占用
	 */
	struct css_type_bits_t {
		uint8_t display : 5;
		uint8_t box_sizing : 2;
		uint8_t visibility : 4;
		uint8_t vertical_align : 4;
		uint8_t pointer_events : 2;
		uint8_t position : 3;

		uint8_t z_index : 2;
		uint8_t opacity : 2;

		uint8_t left : 2;
		uint8_t right : 2;
		uint8_t top : 2;
		uint8_t bottom : 2;
		uint8_t width : 2;
		uint8_t height : 2;
		uint8_t min_width : 2;
		uint8_t min_height : 2;
		uint8_t max_width : 2;
		uint8_t max_height : 2;
		uint8_t margin_left : 2;
		uint8_t margin_right : 2;
		uint8_t margin_top : 2;
		uint8_t margin_bottom : 2;
		uint8_t padding_left : 2;
		uint8_t padding_right : 2;
		uint8_t padding_top : 2;
		uint8_t padding_bottom : 2;

		uint8_t border_top_style : 4;
		uint8_t border_right_style : 4;
		uint8_t border_bottom_style : 4;
		uint8_t border_left_style : 4;

		uint8_t background_image : 2;
		uint8_t background_width : 3;
		uint8_t background_height : 3;
		uint8_t background_repeat : 3;
		uint8_t background_position_x : 1;
		uint8_t background_position_y : 1;

		uint8_t box_shadow : 1;

		uint8_t flex_basis : 2;
		uint8_t flex_grow : 1;
		uint8_t flex_shrink : 1;
		uint8_t flex_direction : 3;
		uint8_t flex_wrap : 2;
		uint8_t justify_content : 3;
		uint8_t align_content : 3;
		uint8_t align_items : 3;

		uint8_t color : 2;
		uint8_t font_size : 4;
		uint8_t font_style : 2;
		uint8_t font_weight : 4;
		uint8_t font_family : 4;
		uint8_t line_height : 2;
		uint8_t text_align : 4;
		uint8_t content : 2;
		uint8_t white_space : 3;
		uint8_t word_break : 3;
	} type_bits;

	/**
	 * 属性值单位的比特数据
	 */
	struct css_unit_bits_t {
		css_unit_t left : 4;
		css_unit_t right : 4;
		css_unit_t top : 4;
		css_unit_t bottom : 4;
		css_unit_t width : 4;
		css_unit_t height : 4;
		css_unit_t min_width : 4;
		css_unit_t min_height : 4;
		css_unit_t max_width : 4;
		css_unit_t max_height : 4;
		css_unit_t margin_left : 4;
		css_unit_t margin_right : 4;
		css_unit_t margin_top : 4;
		css_unit_t margin_bottom : 4;
		css_unit_t padding_left : 4;
		css_unit_t padding_right : 4;
		css_unit_t padding_top : 4;
		css_unit_t padding_bottom : 4;
		css_unit_t background_width : 4;
		css_unit_t background_height : 4;
		css_unit_t background_position_x : 4;
		css_unit_t background_position_y : 4;
		css_unit_t border_top_width : 4;
		css_unit_t border_right_width : 4;
		css_unit_t border_bottom_width : 4;
		css_unit_t border_left_width : 4;
		css_unit_t border_top_left_radius : 4;
		css_unit_t border_top_right_radius : 4;
		css_unit_t border_bottom_left_radius : 4;
		css_unit_t border_bottom_right_radius : 4;
		css_unit_t box_shadow_x : 4;
		css_unit_t box_shadow_y : 4;
		css_unit_t box_shadow_blur : 4;
		css_unit_t box_shadow_spread : 4;
		css_unit_t flex_basis : 4;
		css_unit_t font_size : 4;
		css_unit_t line_height : 4;
	} unit_bits;

	css_numeric_value_t z_index;
	css_numeric_value_t opacity;

	css_numeric_value_t left;
	css_numeric_value_t right;
	css_numeric_value_t top;
	css_numeric_value_t bottom;

	css_numeric_value_t width;
	css_numeric_value_t height;
	css_numeric_value_t min_width;
	css_numeric_value_t min_height;
	css_numeric_value_t max_width;
	css_numeric_value_t max_height;

	css_numeric_value_t margin_left;
	css_numeric_value_t margin_right;
	css_numeric_value_t margin_top;
	css_numeric_value_t margin_bottom;
	css_numeric_value_t padding_left;
	css_numeric_value_t padding_right;
	css_numeric_value_t padding_top;
	css_numeric_value_t padding_bottom;

	css_numeric_value_t flex_basis;
	css_numeric_value_t flex_grow;
	css_numeric_value_t flex_shrink;

	css_color_value_t color;
	css_numeric_value_t font_size;
	char **font_family;

	css_numeric_value_t line_height;
	css_string_value_t content;

	css_numeric_value_t border_top_width;
	css_numeric_value_t border_right_width;
	css_numeric_value_t border_bottom_width;
	css_numeric_value_t border_left_width;
	css_color_value_t border_top_color;
	css_color_value_t border_right_color;
	css_color_value_t border_bottom_color;
	css_color_value_t border_left_color;
	css_numeric_value_t border_top_left_radius;
	css_numeric_value_t border_top_right_radius;
	css_numeric_value_t border_bottom_left_radius;
	css_numeric_value_t border_bottom_right_radius;

	css_color_value_t background_color;
	css_image_value_t background_image;
	css_numeric_value_t background_width;
	css_numeric_value_t background_height;
	css_numeric_value_t background_position_x;
	css_numeric_value_t background_position_y;

	// 针对 LCUI 的需求只支持单个阴影效果，暂不考虑支持设置多个阴影效果
	css_numeric_value_t box_shadow_x;
	css_numeric_value_t box_shadow_y;
	css_numeric_value_t box_shadow_blur;
	css_numeric_value_t box_shadow_spread;
	css_color_value_t box_shadow_color;

	css_style_value_t *custom_props;
	size_t custom_props_count;
} css_computed_style_t;

typedef struct css_propdef_t css_propdef_t;

struct css_propdef_t {
	/**
	 * 属性标识号
	 * 值为 -1 时，则表明它是简写属性
	 */
	int key;
	char *name;
	css_valdef_t *valdef;
	css_style_value_t initial_value;
	int (*parse)(css_propdef_t *, const char *, css_style_decl_t *);
	int (*cascade)(const css_style_array_value_t, css_computed_style_t *);
};

typedef libcss_bool_t (*css_value_parse_func_t)(css_style_value_t *, const char *);

typedef struct css_value_type_record_t css_value_type_record_t;

#endif
