#ifndef __LCUI_WIDGET_STYLE_H__
#define __LCUI_WIDGET_STYLE_H__

LCUI_API void LCUIWidgetStyleLibrary_Init( void );

LCUI_API void LCUIWidgetStyleLibrary_Destroy( void );

LCUI_API int WidgetStyle_LoadFromString( const char *style_str );

LCUI_API int WidgetStyle_LoadFromFile( const char *filepath );

LCUI_API int WidgetStyle_Sync( LCUI_Widget widget );

#endif