#ifndef __LCUI_CHARSET_H__
#define __LCUI_CHARSET_H__

#define ENCODEING_TYPE_UTF8	0
#define ENCODEING_TYPE_GB2312	1

LCUI_BEGIN_HEADER

/* UTF-8转Unicode */
LCUI_EXPORT(int)
LCUICharset_UTF8ToUnicode( const char *src_utf8, wchar_t **des_unicode );

/* ASCII转Unicode */
LCUI_EXPORT(int)
LCUICharset_ASCIIToUnicode( const char *src_ascii, wchar_t **des_unicode );

/* GB2312转Unicode */
LCUI_EXPORT(int)
LCUICharset_GB2312ToUnicode( const char *src_gb2312, wchar_t **des_unicode );

LCUI_END_HEADER

#endif
