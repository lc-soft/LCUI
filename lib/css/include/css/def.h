#ifndef LIBCSS_INCLUDE_CSS_DEF_H
#define LIBCSS_INCLUDE_CSS_DEF_H

#include <LCUI/def.h>
#include <LCUI/pandagl/def.h>
#include <LCUI/util.h>

#define CSS_SELECTOR_MAX_LEN 1024
#define CSS_SELECTOR_MAX_DEPTH 32

#define css_string(...) "" #__VA_ARGS__ ""

typedef enum css_property_key_t {
	// position start
	css_key_left,
	css_key_right,
	css_key_top,
	css_key_bottom,
	css_key_position,
	// position end

	// display start
	css_key_visibility,
	css_key_display,
	// display end

	css_key_z_index,
	css_key_opacity,
	css_key_box_sizing,
	css_key_width,
	css_key_height,
	css_key_min_width,
	css_key_min_height,
	css_key_max_width,
	css_key_max_height,

	// margin start
	css_key_margin_top,
	css_key_margin_right,
	css_key_margin_bottom,
	css_key_margin_left,
	// margin end

	// padding start
	css_key_padding_top,
	css_key_padding_right,
	css_key_padding_bottom,
	css_key_padding_left,
	// padding end

	css_key_vertical_align,

	// border start
	css_key_border_top_width,
	css_key_border_top_style,
	css_key_border_top_color,
	css_key_border_right_width,
	css_key_border_right_style,
	css_key_border_right_color,
	css_key_border_bottom_width,
	css_key_border_bottom_style,
	css_key_border_bottom_color,
	css_key_border_left_width,
	css_key_border_left_style,
	css_key_border_left_color,
	css_key_border_top_left_radius,
	css_key_border_top_right_radius,
	css_key_border_bottom_left_radius,
	css_key_border_bottom_right_radius,
	// border end

	// background start
	css_key_background_color,
	css_key_background_image,
	css_key_background_size,
	css_key_background_size_width,
	css_key_background_size_height,
	css_key_background_repeat,
	css_key_background_repeat_x,
	css_key_background_repeat_y,
	css_key_background_position,
	css_key_background_position_x,
	css_key_background_position_y,
	css_key_background_origin,
	// background end

	// box shadow start
	css_key_box_shadow_x,
	css_key_box_shadow_y,
	css_key_box_shadow_spread,
	css_key_box_shadow_blur,
	css_key_box_shadow_color,
	// box shadow end

	// flex style start
	css_key_flex_basis,
	css_key_flex_grow,
	css_key_flex_shrink,
	css_key_flex_direction,
	css_key_flex_wrap,
	css_key_justify_content,
	css_key_align_content,
	css_key_align_items,
	// flex style end

	css_key_color,
	css_key_font_size,
	css_key_font_style,
	css_key_font_weight,
	css_key_font_family,
	css_key_line_height,
	css_key_text_align,
	css_key_content,
	css_key_white_space,

	css_key_pointer_events,
	css_key_focusable,
	STYLE_KEY_TOTAL
} css_property_key_t;

#define css_key_flex_style_start css_key_flex_basis
#define css_key_flex_style_end css_key_align_content
#define css_key_position_start css_key_left
#define css_key_position_end css_key_position
#define css_key_margin_start css_key_margin_top
#define css_key_margin_end css_key_margin_left
#define css_key_padding_start css_key_padding_top
#define css_key_padding_end css_key_padding_left
#define css_key_border_start css_key_border_top_width
#define css_key_border_end css_key_border_bottom_right_radius
#define css_key_background_start css_key_background_color
#define css_key_background_end css_key_background_origin
#define css_key_box_shadow_start css_key_box_shadow_x
#define css_key_box_shadow_end css_key_box_shadow_color

/* FIXME: remove css_keyword_value_t
 * These values do not need to put in css_keyword_value_t, because they are not
 * strongly related and should be defined separately where they are needed.
 */

typedef enum css_keyword_value_t {
	CSS_KEYWORD_NONE,
	CSS_KEYWORD_AUTO,
	CSS_KEYWORD_NORMAL,
	CSS_KEYWORD_INHERIT,
	CSS_KEYWORD_INITIAL,
	CSS_KEYWORD_CONTAIN,
	CSS_KEYWORD_COVER,
	CSS_KEYWORD_LEFT,
	CSS_KEYWORD_CENTER,
	CSS_KEYWORD_RIGHT,
	CSS_KEYWORD_TOP,
	CSS_KEYWORD_TOP_LEFT,
	CSS_KEYWORD_TOP_CENTER,
	CSS_KEYWORD_TOP_RIGHT,
	CSS_KEYWORD_MIDDLE,
	CSS_KEYWORD_CENTER_LEFT,
	CSS_KEYWORD_CENTER_CENTER,
	CSS_KEYWORD_CENTER_RIGHT,
	CSS_KEYWORD_BOTTOM,
	CSS_KEYWORD_BOTTOM_LEFT,
	CSS_KEYWORD_BOTTOM_CENTER,
	CSS_KEYWORD_BOTTOM_RIGHT,
	CSS_KEYWORD_SOLID,
	CSS_KEYWORD_DOTTED,
	CSS_KEYWORD_DOUBLE,
	CSS_KEYWORD_DASHED,
	CSS_KEYWORD_CONTENT_BOX,
	CSS_KEYWORD_PADDING_BOX,
	CSS_KEYWORD_BORDER_BOX,
	CSS_KEYWORD_GRAPH_BOX,
	CSS_KEYWORD_STATIC,
	CSS_KEYWORD_RELATIVE,
	CSS_KEYWORD_ABSOLUTE,
	CSS_KEYWORD_BLOCK,
	CSS_KEYWORD_INLINE_BLOCK,
	CSS_KEYWORD_FLEX,
	CSS_KEYWORD_FLEX_START,
	CSS_KEYWORD_FLEX_END,
	CSS_KEYWORD_STRETCH,
	CSS_KEYWORD_SPACE_BETWEEN,
	CSS_KEYWORD_SPACE_AROUND,
	CSS_KEYWORD_SPACE_EVENLY,
	CSS_KEYWORD_WRAP,
	CSS_KEYWORD_NOWRAP,
	CSS_KEYWORD_ROW,
	CSS_KEYWORD_COLUMN
} css_keyword_value_t;

typedef enum css_unit_t {
	CSS_UNIT_NONE,
	CSS_UNIT_AUTO,
	CSS_UNIT_SCALE,
	CSS_UNIT_PX,
	CSS_UNIT_PT,
	CSS_UNIT_DIP,
	CSS_UNIT_SP,
	CSS_UNIT_COLOR,
	CSS_UNIT_IMAGE,
	CSS_UNIT_KEYWORD,
	CSS_UNIT_INT,
	CSS_UNIT_BOOL,
	CSS_UNIT_STRING,
	CSS_UNIT_WSTRING
} css_unit_t;

#define CSS_UNIT_px CSS_UNIT_PX
#define CSS_UNIT_pt CSS_UNIT_PT
#define CSS_UNIT_int CSS_UNIT_INT
#define CSS_UNIT_color CSS_UNIT_COLOR
#define CSS_UNIT_scale CSS_UNIT_SCALE
#define CSS_UNIT_keyword CSS_UNIT_KEYWORD
#define CSS_UNIT_bool CSS_UNIT_BOOL
#define CSS_UNIT_image CSS_UNIT_IMAGE
#define CSS_UNIT_string CSS_UNIT_STRING
#define CSS_UNIT_wstring CSS_UNIT_WSTRING
#define CSS_UNIT_sp CSS_UNIT_SP
#define CSS_UNIT_dp CSS_UNIT_DIP
#define CSS_UNIT_dip CSS_UNIT_DIP
#define CSS_UNIT_0 CSS_UNIT_NONE
#define CSS_UNIT_none CSS_UNIT_NONE

typedef struct css_unit_value_t {
	LCUI_BOOL is_valid : 2;
	css_unit_t unit : 6;
	union {
		int val_int;
		int val_0;
		int val_none;
		float value;
		float px;
		float val_px;
		float pt;
		float val_pt;
		float dp;
		float val_dp;
		float dip;
		float val_dip;
		float sp;
		float val_sp;
		css_keyword_value_t keyword;
		css_keyword_value_t val_keyword;
		float scale;
		float val_scale;
		char *string;
		char *val_string;
		wchar_t *wstring;
		wchar_t *val_wstring;
		// TODO: delete color and image
		pd_color_t color;
		pd_color_t val_color;
		pd_canvas_t *image;
		pd_canvas_t *val_image;
		LCUI_BOOL val_bool;
	};
} css_unit_value_t;

struct css_style_declaration_t {
	css_unit_value_t *sheet;
	size_t length;
};

typedef struct css_style_declaration_t css_style_declaration_t;
typedef list_t css_style_properties_t;

typedef css_style_properties_t css_style_props_t;
typedef css_style_declaration_t css_style_decl_t;
typedef unsigned css_selector_hash_t;

typedef struct css_style_property_t {
	css_property_key_t key;
	css_unit_value_t style;
	list_node_t node;
} css_style_property_t;

typedef struct css_selector_node_t {
	char *id;       /**< ID */
	char *type;     /**< 类型名称 */
	char **classes; /**< 样式类列表 */
	char **status;  /**< 状态列表 */
	char *fullname; /**< 全名，由 id、type、classes、status 组合而成 */
	int rank;       /**< 权值 */
} css_selector_node_t;

typedef struct css_selector_t {
	int rank;                    /**< 权值，决定优先级 */
	int batch_num;               /**< 批次号 */
	int length;                  /**< 选择器结点长度 */
	css_selector_hash_t hash;    /**< 哈希值 */
	css_selector_node_t **nodes; /**< 选择器结点列表 */
} css_selector_t;

typedef enum css_font_style_t {
	CSS_FONT_STYLE_NORMAL,
	CSS_FONT_STYLE_ITALIC,
	CSS_FONT_STYLE_OBLIQUE,
	CSS_FONT_STYLE_TOTAL_NUM
} css_font_style_t;

typedef enum css_font_weight_t {
	CSS_FONT_WEIGHT_NONE = 0,
	CSS_FONT_WEIGHT_THIN = 100,
	CSS_FONT_WEIGHT_EXTRA_LIGHT = 200,
	CSS_FONT_WEIGHT_LIGHT = 300,
	CSS_FONT_WEIGHT_NORMAL = 400,
	CSS_FONT_WEIGHT_MEDIUM = 500,
	CSS_FONT_WEIGHT_SEMI_BOLD = 600,
	CSS_FONT_WEIGHT_BOLD = 700,
	CSS_FONT_WEIGHT_EXTRA_BOLD = 800,
	CSS_FONT_WEIGHT_BLACK = 900,
	CSS_FONT_WEIGHT_TOTAL_NUM = 9
} css_font_weight_t;

typedef struct css_font_face_t {
	char *font_family;
	css_font_style_t font_style;
	css_font_weight_t font_weight;
	char *src;
} css_font_face_t;

#endif
