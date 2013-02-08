#ifndef __LCUI_FONT_LIBRARY_H__
#define __LCUI_FONT_LIBRARY_H__

LCUI_BEGIN_HEADER

void FontLIB_DestroyAll( void );

/* 初始化字体数据库 */
void FontLIB_Init( void );

/* 获取字体的风格ID */
int FontLIB_GetStyleID( const char *style_name );

/* 获取字体的字族ID */
int FontLIB_GetFamliyID( const char *family_name );

/* 
 * 添加一个字体位图数据至数据库中
 * 注意：调用此函数后，作为参数fontbmp_buff的变量，不能被free掉，否则，数据库中记录
 * 的此数据会无效 
 * */
LCUI_FontBMP *
FontLIB_Add(	wchar_t char_code,	int family_id,
		int style_id,		int pixel_size,	
		LCUI_FontBMP *fontbmp_buff );

/* 获取字体位图数据 */
LCUI_FontBMP *
FontLIB_GetFontBMP(	wchar_t char_code,	int family_id,
			int style_id,		int pixel_size	);

LCUI_END_HEADER

#endif
