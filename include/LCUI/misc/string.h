#ifndef __LCUI_MISC_STRING_H__
#define __LCUI_MISC_STRING_H__

LCUI_BEGIN_HEADER

/* windows系统里并没有strcasecmp函数，因此，使用自定义函数的代替 */
#ifdef LCUI_BUILD_IN_WIN32
#define strcasecmp(str1, str2)	lcui_strcasecmp(str1, str2)
#endif

/* 不区分大小写，对比两个字符串 */
LCUI_EXPORT(int)
lcui_strcasecmp( const char *str1, const char *str2 );

/* 初始化字符串 */
LCUI_EXPORT(void)
LCUIString_Init( LCUI_String *in );

/* 初始化宽字符串 */
LCUI_EXPORT(void)
LCUIWString_Init( LCUI_WString *in );

/* 拷贝源字符串至目标字符串中 */
LCUI_EXPORT(void)
_LCUIString_Copy( LCUI_String * des, const char *src );

/* 宽字符串拷贝 */
LCUI_EXPORT(void)
_LCUIWString_Copy( LCUI_WString *des, const wchar_t *src );

LCUI_EXPORT(void)
LCUIWString_Copy( LCUI_WString *des_str, LCUI_WString *src_str );

/* 字符串对比 */
LCUI_EXPORT(int)
_LCUIString_Cmp( LCUI_String *str1, const char *str2 );

LCUI_EXPORT(int)
LCUIString_Cmp( LCUI_String *str1, LCUI_String *str2 );

/* 宽字符串对比 */
LCUI_EXPORT(int)
_LCUIWString_Cmp( LCUI_WString *str1, const wchar_t *str2 );

LCUI_EXPORT(int)
LCUIWString_Cmp( LCUI_WString *str1, LCUI_WString *str2 );

/* 字符串拷贝 */
LCUI_EXPORT(int)
LCUIString_Copy( LCUI_String *str1, LCUI_String *str2 );

LCUI_EXPORT(void)
LCUIString_Free( LCUI_String *in );

/* 释放宽字符占用的资源 */
LCUI_EXPORT(void)
LCUIWchar_Free( LCUI_WChar *ch );

/* 释放宽字符串占用的资源 */
LCUI_EXPORT(void)
LCUIWString_Free( LCUI_WString *str );

LCUI_END_HEADER

#endif
