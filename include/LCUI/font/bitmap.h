#ifndef LIB_FONT_INCLUDE_FONT_BITMAP_H
#define LIB_FONT_INCLUDE_FONT_BITMAP_H

#include "def.h"
#include <LCUI/header.h>
#include <LCUI/types.h>

/** 打印字体位图的信息 */
LCUI_API void FontBitmap_PrintInfo(font_bitmap_t *bitmap);

/** 初始化字体位图 */
LCUI_API void font_bitmap_init(font_bitmap_t *bitmap);

/** 释放字体位图占用的资源 */
LCUI_API void font_bitmap_destroy(font_bitmap_t *bitmap);

/** 在屏幕打印以0和1表示字体位图 */
LCUI_API int font_bitmap_print(font_bitmap_t *font_bitmap);

/** 将字体位图绘制到目标图像上 */
LCUI_API int font_bitmap_mix(pd_canvas_t *graph, pd_pos_t pos,
			    const font_bitmap_t *bmp, pd_color_t color);

#endif
