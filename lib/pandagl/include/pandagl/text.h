/*
 * lib/pandagl/include/pandagl/text.h: -- Text layout and rendering module.
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_TEXT_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_TEXT_H

#include <yutil.h>
#include "common.h"
#include "types.h"
#include "font.h"

PD_BEGIN_DECLS

typedef struct pd_text_style {
	bool has_family : 1;
	bool has_style : 1;
	bool has_weight : 1;
	bool has_back_color : 1;
	bool has_fore_color : 1;
	bool has_pixel_size : 1;

	int style;
	int weight;
	int *font_ids;

	pd_color_t fore_color;
	pd_color_t back_color;

	int pixel_size;
} pd_text_style_t;

typedef enum pd_text_align_t {
	PD_TEXT_ALIGN_LEFT,
	PD_TEXT_ALIGN_RIGHT,
	PD_TEXT_ALIGN_CENTER
} pd_text_align_t;

typedef struct pd_char {
	wchar_t code;                   /**< 字符码 */
	pd_text_style_t *style;         /**< 该字符使用的样式数据 */
	const pd_font_bitmap_t *bitmap; /**< 字体位图数据(只读) */
} pd_char_t;

/** end of line character */
typedef enum pd_text_eol_t {
	/** 无换行 */
	PD_TEXT_EOL_NONE,

	/** mac os 格式换行，cf = carriage-return，字符：\r */
	PD_TEXT_EOL_CR,

	/** unix/linux 格式换行，lf = line-feed，字符：\r */
	PD_TEXT_EOL_LF,

	/** windows 格式换行： \r\n */
	PD_TEXT_EOL_CR_LF
} pd_text_eol_t;

/* 文本行 */
typedef struct pd_text_line {
	int width;          /**< 宽度 */
	int height;         /**< 高度 */
	int length;         /**< 该行文本长度 */
	pd_char_t **string; /**< 该行文本的数据 */
	pd_text_eol_t eol;  /**< 行尾结束类型 */
} pd_text_line_t;

/**
 * word-break mode
 * the word-break mode specifies whether or not the textlayer should
 * insert line breaks wherever the text would otherwise overflow its
 * content box.
 */
typedef enum pd_word_break_t {
	PD_WORD_BREAK_NORMAL,
	PD_WORD_BREAK_BREAK_ALL
} pd_word_break_t;

typedef struct pd_text {
	int offset_x;     /**< x轴坐标偏移量 */
	int offset_y;     /**< y轴坐标偏移量 */
	int new_offset_x; /**< 新的x轴坐标偏移量 */
	int new_offset_y; /**< 新的y轴坐标偏移量 */
	int insert_x;     /**< 光标所在列数 */
	int insert_y;     /**< 光标所在行数 */
	int width;        /**< 实际文本宽度 */

	/**
	 * 固定宽高
	 * 当它们等于0时，文本宽高会根据文本内容自动适应
	 * 当它们大于0时，会直接根据该值处理文本对齐
	 */
	int fixed_width, fixed_height;
	/**
	 * 最大文本宽高
	 * 当未设置固定宽度时，文字排版将按最大宽度进行
	 */
	int max_width, max_height;

	int length;
	int line_height;
	pd_text_align_t text_align;
	pd_word_break_t word_break;
	bool mulitiline_enabled;
	bool autowrap_enabled;
	bool style_tag_enabled;
	list_t dirty_rects;
	list_t styles;
	pd_text_style_t default_style;
	pd_text_line_t **lines;
	int lines_length;
	struct {
		bool update_bitmap;
		bool update_typeset;
		int typeset_start_line;
		bool redraw_all;
	} task;
} pd_text_t;

/** 获取文本行总数 */
PD_PUBLIC int pd_text_get_lines_length(pd_text_t *layer);

/** 获取指定文本行的高度 */
PD_PUBLIC int pd_text_get_line_height(pd_text_t *layer, int line_num);

/** 获取指定文本行的文本长度 */
PD_PUBLIC int pd_text_get_line_length(pd_text_t *layer, int line_num);

/** 添加 更新文本排版 的任务 */
PD_PUBLIC void pd_text_set_typeset_task(pd_text_t *layer, int start_row);

/** 设置文本对齐方式 */
PD_PUBLIC void pd_text_set_align(pd_text_t *layer, int align);

/** 设置坐标偏移量 */
PD_PUBLIC bool pd_text_set_offset(pd_text_t *layer, int offset_x,
				       int offset_y);

PD_PUBLIC pd_text_t *pd_text_create(void);

PD_PUBLIC void pd_text_destroy(pd_text_t *layer);

/** 标记指定范围内容的文本行的矩形为无效 */
PD_PUBLIC void pd_text_mark_dirty(pd_text_t *layer, int start_row, int end_row);

/** 设置插入点的行列坐标 */
PD_PUBLIC void pd_text_set_insert_position(pd_text_t *layer, int line_num,
					   int col);

/** 根据像素坐标设置文本光标的行列坐标 */
PD_PUBLIC int pd_text_set_insert_pixel_position(pd_text_t *layer, int x, int y);

/** 获取指定行列的文字的像素坐标 */
PD_PUBLIC int pd_text_get_char_pixel_position(pd_text_t *layer, int line_num,
					      int col, pd_pos_t *pixel_pos);

/** 获取文本光标的像素坐标 */
PD_PUBLIC int pd_text_get_insert_pixel_position(pd_text_t *layer,
						pd_pos_t *pixel_pos);

/** 清空文本 */
PD_PUBLIC void pd_text_empty(pd_text_t *layer);

PD_PUBLIC int pd_text_insert(pd_text_t *layer, const wchar_t *wstr,
			     list_t *tag_stack);

PD_PUBLIC int pd_text_append(pd_text_t *layer, const wchar_t *wstr,
			     list_t *tag_stack);

PD_PUBLIC int pd_text_write(pd_text_t *layer, const wchar_t *wstr,
			    list_t *tag_stack);

/** 获取文本图层中的文本（宽字符版） */
PD_PUBLIC size_t pd_text_dump(pd_text_t *text, size_t start_pos, size_t max_len,
			      wchar_t *wstr_buff);

/** 计算并获取文本的宽度 */
PD_PUBLIC int pd_text_get_width(pd_text_t *layer);

/** 计算并获取文本的高度 */
PD_PUBLIC int pd_text_get_height(pd_text_t *layer);

/** 设置固定尺寸 */
PD_PUBLIC int pd_text_set_fixed_size(pd_text_t *layer, int width, int height);

/** 设置最大尺寸 */
PD_PUBLIC int pd_text_set_max_size(pd_text_t *layer, int width, int height);

/** 设置是否启用多行文本模式 */
PD_PUBLIC void pd_text_set_multiline(pd_text_t *layer, bool enabled);

/** 删除文本光标的当前坐标右边的文本 */
PD_PUBLIC int pd_text_delete(pd_text_t *layer, int n_char);

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
PD_PUBLIC int pd_text_backspace(pd_text_t *layer, int n_char);

/** 设置是否启用自动换行模式 */
PD_PUBLIC void pd_text_set_autowrap(pd_text_t *layer, bool autowrap);

/** 设置单词内断行模式 */
PD_PUBLIC void pd_text_set_word_break(pd_text_t *layer, pd_word_break_t mode);

/** 设置是否使用样式标签 */
PD_PUBLIC void pd_text_set_style_tag(pd_text_t *layer, bool enabled);

/** 重新载入各个文字的字体位图 */
PD_PUBLIC void pd_text_reload_bitmap(pd_text_t *layer);

/** 更新数据 */
PD_PUBLIC void pd_text_update(pd_text_t *layer, list_t *rects);

/**
 * 将文本图层中的指定区域的内容绘制至目标图像中
 * @param layer 要使用的文本图层
 * @param area 文本图层中需要绘制的区域
 * @param layer_pos 文本图层在目标图像中的位置
 * @param cavans 目标画布
 */
PD_PUBLIC int pd_text_render_to(pd_text_t *layer, pd_rect_t area,
				pd_pos_t layer_pos, pd_canvas_t *canvas);

/** 设置全局文本样式 */
PD_PUBLIC void pd_text_set_style(pd_text_t *layer, pd_text_style_t *style);

/** 设置文本行的高度 */
PD_PUBLIC void pd_text_set_line_height(pd_text_t *layer, int height);

/** 初始化字体样式数据 */
PD_PUBLIC void pd_text_style_init(pd_text_style_t *data);

PD_PUBLIC int pd_text_style_copy_family(pd_text_style_t *dst,
					pd_text_style_t *src);

PD_PUBLIC int pd_text_style_copy(pd_text_style_t *dst, pd_text_style_t *src);

PD_PUBLIC void pd_text_style_destroy(pd_text_style_t *data);

PD_PUBLIC void pd_text_style_merge(pd_text_style_t *base,
				   pd_text_style_t *target);

/* 设置字体粗细程度 */
PD_PUBLIC int pd_text_style_set_weight(pd_text_style_t *ts,
				       pd_font_weight_t weight);

PD_PUBLIC int pd_text_style_set_style(pd_text_style_t *ts,
				      pd_font_style_t style);

/**
 * 设置字体
 * @param[in][out] ts 字体样式数据
 * @param[in] str 字体名称，如果有多个名称则用逗号分隔
 */
PD_PUBLIC int pd_text_style_set_font(pd_text_style_t *ts,
				     const char *const *names);

PD_PUBLIC void pd_text_style_set_size(pd_text_style_t *ts, int pixel_size);

PD_PUBLIC void pd_text_style_set_fore_color(pd_text_style_t *ts,
					    pd_color_t color);

PD_PUBLIC void pd_text_style_set_back_color(pd_text_style_t *ts,
					    pd_color_t color);

/** 设置使用默认的字体 */
PD_PUBLIC int pd_text_style_set_default_font(pd_text_style_t *ts);

/** 从字符串中获取样式标签的名字及样式属性 */
PD_PUBLIC const wchar_t *pd_scan_style_open_tag(const wchar_t *wstr,
						wchar_t *name, int max_name_len,
						wchar_t *data);

/** 在字符串中获取样式的结束标签，输出的是标签名 */
PD_PUBLIC const wchar_t *pd_scan_style_close_tag(const wchar_t *wstr,
						 wchar_t *name);

PD_PUBLIC void pd_style_tags_clear(list_t *tags);

PD_PUBLIC pd_text_style_t *pd_style_tags_get_text_style(list_t *tags);

/** 处理样式标签 */
PD_PUBLIC const wchar_t *pd_style_tags_next_open_tag(list_t *tags,
						     const wchar_t *str);

/** 处理样式结束标签 */
PD_PUBLIC const wchar_t *pd_style_tags_next_close_tag(list_t *tags,
						      const wchar_t *str);

PD_END_DECLS

#endif
