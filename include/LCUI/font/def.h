
#ifndef LIB_FONT_INCLUDE_FONT_DEF_H
#define LIB_FONT_INCLUDE_FONT_DEF_H

/* clang-format off */

typedef enum font_style_t {
	FONT_STYLE_NORMAL,
	FONT_STYLE_ITALIC,
	FONT_STYLE_OBLIQUE,
	FONT_STYLE_TOTAL_NUM
} font_style_t;

typedef enum font_weight_t {
	FONT_WEIGHT_NONE,
	FONT_WEIGHT_THIN,
	FONT_WEIGHT_EXTRA_LIGHT,
	FONT_WEIGHT_LIGHT,
	FONT_WEIGHT_NORMAL,
	FONT_WEIGHT_MEDIUM,
	FONT_WEIGHT_SEMI_BOLD,
	FONT_WEIGHT_BOLD,
	FONT_WEIGHT_EXTRA_BOLD,
	FONT_WEIGHT_BLACK,
	FONT_WEIGHT_TOTAL_NUM
} font_weight_t;

typedef struct font_bitmap_t {
	int top;   /**< 与顶边框的距离 */
	int left;  /**< 与左边框的距离 */
	int width; /**< 位图宽度 */
	int rows;  /**< 位图行数 */
	int pitch;
	unsigned char *buffer; /**< 字体位图数据 */
	short num_grays;
	char pixel_mode;
	struct {
		int x, y;
	} advance;
} font_bitmap_t;

typedef struct font_engine_t font_engine_t;

typedef struct font_t {
	int id;                /**< 字体信息ID */
	char *style_name;      /**< 样式名称 */
	char *family_name;     /**< 字族名称 */
	void *data;            /**< 相关数据 */
	font_style_t style;    /**< 风格 */
	font_weight_t weight;  /**< 粗细程度 */
	font_engine_t *engine; /**< 所属的字体引擎 */
} font_t;

struct font_engine_t {
	char name[64];
	int (*open)(const char *, font_t ***);
	int (*render)(font_bitmap_t *, unsigned, int, font_t *);
	void (*close)(void *);
};

/* clang-format on */

#endif
