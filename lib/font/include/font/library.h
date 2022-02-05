#ifndef LIB_FONT_INCLUDE_FONT_LIBRARY_H
#define LIB_FONT_INCLUDE_FONT_LIBRARY_H

#include <LCUI/header.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API char *fontlib_get_font_path(const char *name);

/**
 * 根据字符串内容猜测字体粗细程度
 * 文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight
 */
LCUI_API font_weight_t fontlib_detect_weight(const char *str);

/**
 * 根据字符串内容猜测字体风格
 * 文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-style
 */
LCUI_API font_style_t fontlib_detect_style(const char *str);

/** 载入字体位图 */
LCUI_API int fontlib_render_bitmap(font_bitmap_t *buff, unsigned ch, int font_id,
				   int pixel_size);

/** 添加字体族，并返回该字族的ID */
LCUI_API int fontlib_add_font(font_t *font);

/**
 * 获取字体的ID
 * @param[in] family_name 字族名称
 * @param[in] style 字体风格
 * @param[in] weight 字体粗细程度，若为值 0，则默认为 FONT_WEIGHT_NORMAL
 */
LCUI_API int fontlib_get_font_id(const char *family_name, font_style_t style,
				 font_weight_t weight);

/**
 * 更新当前字体的粗细程度
 * @param[in] font_ids 当前的字体 id 列表
 * @params[in] weight 字体粗细程度
 * @params[out] new_font_ids 更新字体粗细程度后的字体 id 列表
 */
LCUI_API size_t fontlib_update_font_weight(const int *font_ids,
					   font_weight_t weight,
					   int **new_font_ids);

/**
 * 更新当前字体的风格
 * @param[in] font_ids 当前的字体 id 列表
 * @params[in] style 字体风格
 * @params[out] new_font_ids 更新字体粗细程度后的字体 id 列表
 */
LCUI_API size_t fontlib_update_font_style(const int *font_ids,
					  font_style_t style,
					  int **new_font_ids);

/**
 * 根据字族名称获取对应的字体 ID 列表
 * @param[out] ids 输出的字体 ID 列表
 * @param[in] style 风格
 * @param[in] weight 字重，若为值 0，则默认为 FONT_WEIGHT_NORMAL
 * @param[in] names 字族名称，多个名字用逗号隔开
 * @return 获取到的字体 ID 的数量
 */
LCUI_API size_t fontlib_query(int **font_ids, font_style_t style,
			      font_weight_t weight, const char *names);

/** 获取指定字体ID的字体信息 */
LCUI_API font_t *fontlib_get_font(int id);

/** 获取默认的字体ID */
LCUI_API int fontlib_get_default_font(void);

/** 设定默认的字体 */
LCUI_API void fontlib_set_default_font(int id);

/**
 * 向字体缓存中添加字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 要添加的字体位图
 * @warning 此函数仅仅是将 bmp 复制进缓存中，并未重新分配新的空间储存位图数
 * 据，因此，请勿在调用此函数后手动释放 bmp。
 */
LCUI_API font_bitmap_t *fontlib_add_bitmap(wchar_t ch, int font_id, int size,
					   const font_bitmap_t *bmp);

/**
 * 从缓存中获取字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 输出的字体位图的引用
 * @warning 请勿释放 bmp，bmp 仅仅是引用缓存中的字体位图，并未建分配新
 * 空间存储字体位图的拷贝。
 */
LCUI_API int fontlib_get_bitmap(unsigned ch, int font_id, int size,
				const font_bitmap_t **bmp);

/** 载入字体至数据库中 */
LCUI_API int fontlib_load_file(const char *filepath);

/** 初始化字体处理模块 */
LCUI_API void fontlib_init(void);

/** 停用字体处理模块 */
LCUI_API void fontlib_destroy(void);

LCUI_END_HEADER

#endif
