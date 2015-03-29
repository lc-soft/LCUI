#ifndef __LCUI_MISC_PARSE_H__
#define __LCUI_MISC_PARSE_H__

LCUI_BEGIN_HEADER

/** 从字符串中解析出数字，支持的单位：点(pt)、像素(px)、百分比(%) */
LCUI_API LCUI_BOOL ParseNumer( LCUI_StyleVar *var, const char *str );

LCUI_END_HEADER

#endif
