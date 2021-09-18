// Copyright to be defined

#ifndef PANDAGL_INCLUDE_PANDAGL_TYPES_H_
#define PANDAGL_INCLUDE_PANDAGL_TYPES_H_

#include <stdint.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef uint8_t pd_bool_t;

#define NEW(TYPE, COUNT) (TYPE *)calloc(COUNT, sizeof(TYPE))

/** 色彩模式 */
typedef enum pd_color_type_ {
    PD_COLOR_TYPE_INDEX8,   /**< 8位索引 */
    PD_COLOR_TYPE_GRAY8,    /**< 8位灰度 */
    PD_COLOR_TYPE_RGB323,   /**< RGB323 */
    PD_COLOR_TYPE_ARGB2222, /**< ARGB2222 */
    PD_COLOR_TYPE_RGB555,   /**< RGB555 */
    PD_COLOR_TYPE_RGB565,   /**< RGB565 */
    PD_COLOR_TYPE_RGB888,   /**< RGB888 */
    PD_COLOR_TYPE_ARGB8888  /**< RGB8888 */
} pd_color_type;

typedef enum pd_style_value_ {
    PD_STYLE_VALUE_NONE,
    PD_STYLE_VALUE_AUTO,
    PD_STYLE_VALUE_NORMAL,
    PD_STYLE_VALUE_INHERIT,
    PD_STYLE_VALUE_INITIAL,
    PD_STYLE_VALUE_CONTAIN,
    PD_STYLE_VALUE_COVER,
    PD_STYLE_VALUE_LEFT,
    PD_STYLE_VALUE_CENTER,
    PD_STYLE_VALUE_RIGHT,
    PD_STYLE_VALUE_TOP,
    PD_STYLE_VALUE_TOP_LEFT,
    PD_STYLE_VALUE_TOP_CENTER,
    PD_STYLE_VALUE_TOP_RIGHT,
    PD_STYLE_VALUE_MIDDLE,
    PD_STYLE_VALUE_CENTER_LEFT,
    PD_STYLE_VALUE_CENTER_CENTER,
    PD_STYLE_VALUE_CENTER_RIGHT,
    PD_STYLE_VALUE_BOTTOM,
    PD_STYLE_VALUE_BOTTOM_LEFT,
    PD_STYLE_VALUE_BOTTOM_CENTER,
    PD_STYLE_VALUE_BOTTOM_RIGHT,
    PD_STYLE_VALUE_SOLID,
    PD_STYLE_VALUE_DOTTED,
    PD_STYLE_VALUE_DOUBLE,
    PD_STYLE_VALUE_DASHED,
    PD_STYLE_VALUE_CONTENT_BOX,
    PD_STYLE_VALUE_PADDING_BOX,
    PD_STYLE_VALUE_BORDER_BOX,
    PD_STYLE_VALUE_GRAPH_BOX,
    PD_STYLE_VALUE_STATIC,
    PD_STYLE_VALUE_RELATIVE,
    PD_STYLE_VALUE_ABSOLUTE,
    PD_STYLE_VALUE_BLOCK,
    PD_STYLE_VALUE_INLINE_BLOCK,
    PD_STYLE_VALUE_FLEX,
    PD_STYLE_VALUE_FLEX_START,
    PD_STYLE_VALUE_FLEX_END,
    PD_STYLE_VALUE_STRETCH,
    PD_STYLE_VALUE_SPACE_BETWEEN,
    PD_STYLE_VALUE_SPACE_AROUND,
    PD_STYLE_VALUE_SPACE_EVENLY,
    PD_STYLE_VALUE_WRAP,
    PD_STYLE_VALUE_NOWRAP,
    PD_STYLE_VALUE_ROW,
    PD_STYLE_VALUE_COLUMN
} pd_style_value;

/** 样式变量类型 */
typedef enum pd_style_type_ {
    PD_STYPE_NONE,
    PD_STYPE_AUTO,
    PD_STYPE_SCALE,
    PD_STYPE_PX,
    PD_STYPE_PT,
    PD_STYPE_DIP,
    PD_STYPE_SP,
    PD_STYPE_COLOR,
    PD_STYPE_IMAGE,
    PD_STYPE_STYLE,
    PD_STYPE_INT,
    PD_STYPE_BOOL,
    PD_STYPE_STRING,
    PD_STYPE_WSTRING
} pd_style_type;

#define PD_COLOR_TYPE_ARGB PD_COLOR_TYPE_ARGB8888


typedef union pd_color32_t_ {
    int32_t value;
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
} pd_color32_t, pd_color_t;

/** Position in plane coordinate system */
typedef struct pd_pos_t_ {
    int x;
    int y;
} pd_pos_t;

typedef struct pd_rect_t_ {
    int x, y, width, height;
} pd_rect_t;

typedef struct pd_rect_f_t_ {
    float x, y, width, height;
} pd_rect_f_t;

typedef struct pd_canvas_t_ {
    uint32_t width;
    uint32_t height;
    union {
        uint8_t *bytes;
        pd_color_t *argb;
    };
    pd_color_type color_type;
    int bytes_per_pixel;
    uint32_t bytes_per_row;
    float opacity;
    size_t mem_size;
    uint8_t *palette;
} pd_canvas_t;

typedef struct pd_context_t_ {
    int top;
    int left;
    pd_bool_t is_valid;
    pd_bool_t is_writable;
    union {
        pd_canvas_t *source;
        const pd_canvas_t *source_ro;
    };
    pd_rect_t rect;    /**< 需要绘制的区域 */
    pd_bool_t with_alpha; /**< 绘制时是否需要处理 alpha 通道 */
} pd_context_t;


typedef struct pd_box_shadow_t_ {
    int x, y;
    int blur;
    int spread;
    pd_color_t color;
    int top_left_radius;
    int top_right_radius;
    int bottom_left_radius;
    int bottom_right_radius;
} pd_box_shadow_t;

typedef struct pd_border_style_t_ {
    struct {
        int style;
        float width;
        pd_color_t color;
    } top, right, bottom, left;
    float top_left_radius;
    float top_right_radius;
    float bottom_left_radius;
    float bottom_right_radius;
} pd_border_style_t;

typedef struct pd_border_line_t_ {
    int style;
    uint32_t width;
    pd_color_t color;
} pd_border_line_t;

typedef struct pd_border_t_ {
    pd_border_line_t top, right, bottom, left;
    uint32_t top_left_radius;
    uint32_t top_right_radius;
    uint32_t bottom_left_radius;
    uint32_t bottom_right_radius;
} pd_border_t;

typedef struct pd_style_rect_ {
    pd_bool_t is_valid : 2;
    pd_style_type type : 6;
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
        pd_style_value style;
        pd_style_value val_style;
        float scale;
        float val_scale;
        char *string;
        char *val_string;
        wchar_t *wstring;
        wchar_t *val_wstring;
        pd_color_t color;
        pd_color_t val_color;
        pd_canvas_t *image;
        pd_canvas_t *val_image;
        pd_bool_t val_bool;
    };
} pd_style_rect, *pd_style_ptr;

typedef struct pd_background_position_t_ {
    pd_bool_t using_value;
    union {
        struct {
            pd_style_rect x, y;
        };
        int value;
    };
} pd_background_position_t;

typedef struct pd_background_size_t_ {
    pd_bool_t using_value;
    union {
        struct {
            pd_style_rect width, height;
        };
        int value;
    };
} pd_background_size_t;

typedef struct pd_background_style_t {
    pd_canvas_t image; /**< 背景图 */
    pd_color_t color; /**< 背景色 */
    struct {
        pd_bool_t x, y;
    } repeat;                         /**< 背景图是否重复 */
    pd_background_position_t position; /**< 定位方式 */
    pd_background_size_t size;         /**< 尺寸 */
} pd_background_style_t;

typedef struct pd_background_t {
    pd_canvas_t *image; /**< 背景图 */
    pd_color_t color;  /**< 背景色 */
    struct {
        pd_bool_t x, y;
    } repeat; /**< 背景图是否重复 */
    struct {
        int x, y;
    } position;
    struct {
        int width, height;
    } size;
} pd_background_t;


#endif  // !PANDAGL_INCLUDE_PANDAGL_TYPES_H_
