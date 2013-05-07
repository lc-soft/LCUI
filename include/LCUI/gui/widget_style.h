#ifndef __LCUI_WIDGET_STYLE_H__
#define __LCUI_WIDGET_STYLE_H__

LCUI_API void WidgetStyle_LibraryInit( void );

LCUI_API void WidgetStyle_LibraryDestroy( void );

LCUI_API int WidgetStyle_LoadFromString( const char *style_str );

LCUI_API int WidgetStyle_Sync( LCUI_Widget *widget );

#endif