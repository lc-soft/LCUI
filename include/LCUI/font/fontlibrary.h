#ifndef __LCUI_FONT_LIBRARY_H__
#define __LCUI_FONT_LIBRARY_H__

LCUI_BEGIN_HEADER

LCUI_EXPORT(void) FontLIB_DestroyAll( void );

/* 初始化字体数据库 */
LCUI_EXPORT(void) FontLIB_Init( void );

/* 通过字体文件路径来查找字体信息，并获取字体ID */
LCUI_EXPORT(int) FontLIB_FindInfoByFilePath( const char *filepath );

/* 获取指定字族名的字体ID */
LCUI_EXPORT(int) FontLIB_GetFontIDByFamilyName( const char *family_name );

/* 获取指定字体ID的字体face对象句柄 */
LCUI_EXPORT(FT_Face) FontLIB_GetFontFace( int font_id );

/* 获取默认的字体ID */
LCUI_EXPORT(int) FontLIB_GetDefaultFontID( void );

/* 设定默认的字体 */
LCUI_EXPORT(void) FontLIB_SetDefaultFont( int id );

/* 
 * 添加一个字体位图数据至数据库中
 * 注意：调用此函数后，作为参数fontbmp_buff的变量，不能被free掉，否则，数据库中记录
 * 的此数据会无效 
 * */
LCUI_EXPORT(LCUI_FontBMP *)
FontLIB_AddFontBMP(	wchar_t char_code, int font_id,
			int pixel_size,	 LCUI_FontBMP *fontbmp_buff );

/* 获取字体位图数据 */
LCUI_EXPORT(LCUI_FontBMP *)
FontLIB_GetFontBMP( wchar_t char_code, int font_id, int pixel_size );

/* 载入字体值数据库中 */
LCUI_EXPORT(int) FontLIB_LoadFontFile( const char *filepath );

LCUI_END_HEADER

#endif
