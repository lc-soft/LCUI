/*
 * lib/pandagl/include/pandagl/font.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_PANDAGL_INCLUDE_FONT_H
#define LIB_PANDAGL_INCLUDE_FONT_H

#include "common.h"

typedef enum pd_font_style_t {
	PD_FONT_STYLE_NORMAL,
	PD_FONT_STYLE_ITALIC,
	PD_FONT_STYLE_OBLIQUE,
	PD_FONT_STYLE_TOTAL_NUM
} pd_font_style_t;

typedef enum pd_font_weight_t {
	PD_FONT_WEIGHT_NONE,
	PD_FONT_WEIGHT_THIN,
	PD_FONT_WEIGHT_EXTRA_LIGHT,
	PD_FONT_WEIGHT_LIGHT,
	PD_FONT_WEIGHT_NORMAL,
	PD_FONT_WEIGHT_MEDIUM,
	PD_FONT_WEIGHT_SEMI_BOLD,
	PD_FONT_WEIGHT_BOLD,
	PD_FONT_WEIGHT_EXTRA_BOLD,
	PD_FONT_WEIGHT_BLACK,
	PD_FONT_WEIGHT_TOTAL_NUM
} pd_font_weight_t;

typedef struct pd_font_bitmap_t {
	int top;   /**< 与顶边框的距离 */
	int left;  /**< 与左边框的距离 */
	int width; /**< 位图宽度 */
	int rows;  /**< 位图行数 */
	int pitch;
	uint8_t *buffer; /**< 字体位图数据 */
	struct {
		int hori_advance, vert_advance;
		int bbox_width, bbox_height;
		int ascender;
	} metrics;
} pd_font_bitmap_t;

typedef struct font_engine_t font_engine_t;

typedef struct pd_font_t {
	int id;                  /**< 字体信息ID */
	char *style_name;        /**< 样式名称 */
	char *family_name;       /**< 字族名称 */
	void *data;              /**< 相关数据 */
	pd_font_style_t style;   /**< 风格 */
	pd_font_weight_t weight; /**< 粗细程度 */
	font_engine_t *engine;   /**< 所属的字体引擎 */
} pd_font_t;

struct font_engine_t {
	char name[64];
	int (*open)(const char *, pd_font_t ***);
	int (*render)(pd_font_bitmap_t *, unsigned, int, pd_font_t *);
	void (*close)(void *);
};

PD_BEGIN_DECLS

/** 将字体位图绘制到目标图像上 */
PD_PUBLIC int pd_canvas_mix_font_bitmap(pd_canvas_t *graph, pd_pos_t pos,
					const pd_font_bitmap_t *bmp,
					pd_color_t color);

PD_PUBLIC char *pd_font_library_get_font_path(const char *name);

/**
 * 根据字符串内容猜测字体粗细程度
 * 文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight
 */
PD_PUBLIC pd_font_weight_t pd_font_library_detect_weight(const char *str);

/**
 * 根据字符串内容猜测字体风格
 * 文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-style
 */
PD_PUBLIC pd_font_style_t pd_font_library_detect_style(const char *str);

/** 载入字体位图 */
PD_PUBLIC int pd_font_library_render_bitmap(pd_font_bitmap_t *buff, unsigned ch,
					    int font_id, int pixel_size);

/** 添加字体族，并返回该字族的ID */
PD_PUBLIC int pd_font_library_add_font(pd_font_t *font);

/**
 * 获取字体的ID
 * @param[in] family_name 字族名称
 * @param[in] style 字体风格
 * @param[in] weight 字体粗细程度，若为值 0，则默认为 PD_FONT_WEIGHT_NORMAL
 */
PD_PUBLIC int pd_font_library_get_font_id(const char *family_name,
					  pd_font_style_t style,
					  pd_font_weight_t weight);

/**
 * 更新当前字体的粗细程度
 * @param[in] font_ids 当前的字体 id 列表
 * @params[in] weight 字体粗细程度
 * @params[out] new_font_ids 更新字体粗细程度后的字体 id 列表
 */
PD_PUBLIC size_t pd_font_library_update_font_weight(const int *font_ids,
						    pd_font_weight_t weight,
						    int **new_font_ids);

/**
 * 更新当前字体的风格
 * @param[in] font_ids 当前的字体 id 列表
 * @params[in] style 字体风格
 * @params[out] new_font_ids 更新字体粗细程度后的字体 id 列表
 */
PD_PUBLIC size_t pd_font_library_update_font_style(const int *font_ids,
						   pd_font_style_t style,
						   int **new_font_ids);

/**
 * 根据字族名称获取对应的字体 ID 列表
 * @param[out] ids 输出的字体 ID 列表
 * @param[in] style 风格
 * @param[in] weight 字重，若为值 0，则默认为 PD_FONT_WEIGHT_NORMAL
 * @param[in] names 字族名称，多个名字用逗号隔开
 * @return 获取到的字体 ID 的数量
 */
PD_PUBLIC unsigned pd_font_library_query(int **font_ids, pd_font_style_t style,
					 pd_font_weight_t weight,
					 const char *const *names);

/** 获取指定字体ID的字体信息 */
PD_PUBLIC pd_font_t *pd_font_library_get_font(int id);

/** 获取默认的字体ID */
PD_PUBLIC int pd_font_library_get_default_font(void);

/** 设定默认的字体 */
PD_PUBLIC void pd_font_library_set_default_font(int id);

/**
 * 向字体缓存中添加字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 要添加的字体位图
 * @warning 此函数仅仅是将 bmp 复制进缓存中，并未重新分配新的空间储存位图数
 * 据，因此，请勿在调用此函数后手动释放 bmp。
 */
PD_PUBLIC pd_font_bitmap_t *pd_font_library_add_bitmap(
    wchar_t ch, int font_id, int size, const pd_font_bitmap_t *bmp);

/**
 * 从缓存中获取字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 输出的字体位图的引用
 * @warning 请勿释放 bmp，bmp 仅仅是引用缓存中的字体位图，并未建分配新
 * 空间存储字体位图的拷贝。
 */
PD_PUBLIC int pd_font_library_get_bitmap(unsigned ch, int font_id, int size,
					 const pd_font_bitmap_t **bmp);

/** 载入字体至数据库中 */
PD_PUBLIC int pd_font_library_load_file(const char *filepath);

/** 初始化字体处理模块 */
PD_PUBLIC void pd_font_library_init(void);

/** 停用字体处理模块 */
PD_PUBLIC void pd_font_library_destroy(void);

PD_END_DECLS

#endif
