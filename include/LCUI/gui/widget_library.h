#ifndef __LCUI_WIDGET_LIBRARY_H__
#define __LCUI_WIDGET_LIBRARY_H__

LCUI_BEGIN_HEADER

/** 添加一个部件类型 */
LCUI_API int LCUIWidget_AddType( const char *widget_type );

/** 移除一个部件类型 */
LCUI_API int LCUIWidget_RemoveType( const char *widget_type );

/** 设置指定类型部件的函数 */
LCUI_API int LCUIWidget_SetFunc( const char *widget_type, const char *func_type,
			void(*func)(LCUI_Widget*) );

/** 调用指定类型部件的函数 */
LCUI_API void LCUIWidget_Call( const char *widget_type, const char *func_type );

LCUI_END_HEADER

#endif
