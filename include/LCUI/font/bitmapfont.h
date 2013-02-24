#ifndef __LCUI_BITMAPFONT_H__
#define __LCUI_BITMAPFONT_H__

LCUI_BEGIN_HEADER

LCUI_EXPORT(uchar_t const *) in_core_font_8x8( void );

LCUI_EXPORT(LCUI_BOOL) FontBMP_Valid(LCUI_FontBMP *bitmap);
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回帧，无效返回假
 */

LCUI_EXPORT(void) Print_FontBMP_Info(LCUI_FontBMP *bitmap);
/* 功能：打印位图的信息 */

LCUI_EXPORT(void) FontBMP_Init(LCUI_FontBMP *bitmap);
/* 初始化字体位图 */

LCUI_EXPORT(void) FontBMP_Free(LCUI_FontBMP *bitmap);
/* 释放字体位图占用的资源 */

LCUI_EXPORT(int) FontBMP_Create(LCUI_FontBMP *bitmap, int width, int rows);
/* 功能：创建字体位图 */

LCUI_EXPORT(void) Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap);
/* 功能：根据字符编码，获取已内置的字体位图 */

LCUI_EXPORT(int) Show_FontBMP(LCUI_FontBMP *fontbmp);
/* 功能：在屏幕打印以0和1表示字体位图 */

LCUI_EXPORT(int)
FontBMP_Mix(	LCUI_Graph	*graph, LCUI_Pos	des_pos,
		LCUI_FontBMP	*bitmap, LCUI_RGB	color,
		int flag );
/* 功能：将字体位图绘制到背景图形上 */

/* 获取现有的字体位图数据 */
LCUI_EXPORT(LCUI_FontBMP *)
Get_ExistFontBMP( int font_id, wchar_t ch, int pixel_size );

LCUI_END_HEADER

#endif
