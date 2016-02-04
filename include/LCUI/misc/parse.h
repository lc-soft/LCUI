#ifndef __LCUI_MISC_PARSE_H__
#define __LCUI_MISC_PARSE_H__

LCUI_BEGIN_HEADER

/** 从字符串中解析出数字，支持的单位：点(pt)、像素(px)、百分比(%) */
LCUI_API LCUI_BOOL ParseNumber( LCUI_Style var, const char *str );

/** 从字符串中解析出色彩值，支持格式：#fff、#ffffff, rgba(R,G,B,A)、rgb(R,G,B) */
LCUI_API LCUI_BOOL ParseColor( LCUI_Style var, const char *str );

LCUI_END_HEADER

#endif
