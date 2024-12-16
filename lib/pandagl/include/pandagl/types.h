﻿/*
 * lib/pandagl/include/pandagl/types.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_TPYES_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_TPYES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum pd_color_type_t {
	PD_COLOR_TYPE_UNSUPPORTED,
	PD_COLOR_TYPE_INDEX8,   /**< 8位索引 */
	PD_COLOR_TYPE_GRAY8,    /**< 8位灰度 */
	PD_COLOR_TYPE_RGB323,   /**< RGB323 */
	PD_COLOR_TYPE_ARGB2222, /**< ARGB2222 */
	PD_COLOR_TYPE_RGB555,   /**< RGB555 */
	PD_COLOR_TYPE_RGB565,   /**< RGB565 */
	PD_COLOR_TYPE_RGB888,   /**< RGB888 */
	PD_COLOR_TYPE_ARGB8888,  /**< RGB8888 */
} pd_color_type_t;

#define PD_COLOR_TYPE_RGB PD_COLOR_TYPE_RGB888
#define PD_COLOR_TYPE_ARGB PD_COLOR_TYPE_ARGB8888

typedef union pd_color_t {
	uint32_t value;
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
} pd_color_t;

/** Position in plane coordinate system */
typedef struct pd_pos {
	int x, y;
} pd_pos_t;

typedef struct pd_rect {
	int x, y, width, height;
} pd_rect_t;

typedef struct pd_canvas pd_canvas_t;

typedef struct pd_canvas_quote {
	int top;
	int left;
	bool is_valid;
	pd_canvas_t *source;
} pd_canvas_quote_t;

struct pd_canvas {
	unsigned width;
	unsigned height;
	float opacity;
	pd_canvas_quote_t quote;
	union {
		uint8_t *bytes;
		pd_color_t *argb;
	};
	pd_color_type_t color_type;
	unsigned bytes_per_pixel;
	unsigned bytes_per_row;
	size_t mem_size;
};

/** 进行绘制时所需的上下文 */
typedef struct pd_context {
	pd_rect_t rect;    /**< 需要绘制的区域 */
	pd_canvas_t canvas; /**< 绘制后的位图缓存（可称为：画布） */
	bool with_alpha; /**< 绘制时是否需要处理 alpha 通道 */
} pd_context_t;

typedef struct pd_background {
	pd_canvas_t *image;
	pd_color_t color;
	bool repeat_x;
	bool repeat_y;
	int x;
	int y;
	int width;
	int height;
} pd_background_t;

typedef struct pd_boxshadow {
	int x, y;
	int blur;
	int spread;
	pd_color_t color;
	int top_left_radius;
	int top_right_radius;
	int bottom_left_radius;
	int bottom_right_radius;
} pd_boxshadow_t;

typedef struct pd_border_line {
	int style;
	unsigned int width;
	pd_color_t color;
} pd_border_line_t;

typedef struct pd_border {
	pd_border_line_t top, right, bottom, left;
	unsigned int top_left_radius;
	unsigned int top_right_radius;
	unsigned int bottom_left_radius;
	unsigned int bottom_right_radius;
} pd_border_t;

typedef enum pd_error_t {
	PD_OK = 0,
	PD_ERROR_NOT_FOUND,
	PD_ERROR_NOT_IMPLEMENTED,
	PD_ERROR_IMAGE_HEADER_INVALID,
	PD_ERROR_IMAGE_TYPE_INCORRECT,
	PD_ERROR_IMAGE_DATA_NOT_SUPPORTED,
	PD_ERROR_IMAGE_READING
} pd_error_t;

#endif
