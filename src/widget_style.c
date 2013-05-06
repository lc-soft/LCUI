#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_STYLE_LIBRARY_H

static LCUI_BOOL library_is_init = FALSE;
static LCUI_StyleLibrary library;

LCUI_API void WidgetStyle_LibraryInit( void )
{
	StyleLib_Init(&library);
	library_is_init = TRUE;
}

LCUI_API int WidgetStyle_LoadFromString( const char *style_str )
{
	_DEBUG_MSG("enter\n");
	if( !library_is_init ) {
		_DEBUG_MSG("!library_is_init\n");
		return -1;
	}
	return StyleLib_AddStyleFromString( &library, style_str );
}

/* 从字符串中指定段内获取出16进制的数 */
static int str_scan_hex_number( const char *str, int start, int end )
{
	int i, n, k;
	n = 0;
	k = end - start - 1;
	for(i=start; i<end; ++i) {
		if(str[i] >= '0' && str[i] <= '9') {
			n += (str[i]-'0')*k*16;
			--k;
		}
		if((str[i] >= 'a' && str[i] <= 'f')
		|| (str[i] >= 'A' && str[i] <= 'F')) {
			n += (str[i]-'a')*k*16;
			--k;
		}
	}
	return n;
}

static int style_color_covernt( const char *style_str, LCUI_RGB *rgb )
{
	int len;
	uchar_t r, g, b;

	len = strlen(style_str);
	_DEBUG_MSG("len: %d\n", len);
	if(style_str[0] == '#') {
		switch(len) {
		case 4: 
			r = style_str[1]-'0';
			g = style_str[2]-'0';
			b = style_str[3]-'0';
			r<<=4; g<<=4; b<<=4;
			break;
		case 7:
			r = str_scan_hex_number( style_str, 1, 3 );
			g = str_scan_hex_number( style_str, 3, 5 );
			b = str_scan_hex_number( style_str, 5, 7 );
			break;
		default:
			return 0;
		}
	} else {
		return 0;
	}
	rgb->red = r;
	rgb->green = g;
	rgb->blue = b;
	_DEBUG_MSG("%d,%d,%d\n", r, g, b);
	return 3;
}


static int WidgetStyle_SyncBackground(
		LCUI_Widget *widget,
		LCUI_StyleClass *style_class,
		const char *pseudo_class_name )
{
	int ret;
	LCUI_RGB back_color;
	LCUI_StyleAttr *widget_attr;
	widget_attr = StyleLib_GetStyleAttr( style_class, pseudo_class_name, "background-color");
	if( widget_attr == NULL ) {
		_DEBUG_MSG("widget_attr == NULL\n");
		return 1;
	}
	_DEBUG_MSG("attr name: %s, attr value: %s\n",
	widget_attr->attr_name.string, widget_attr->attr_value.string);
	ret = style_color_covernt( widget_attr->attr_value.string, &back_color );
	if( ret > 0 ) {
		Widget_SetBackgroundColor( widget, back_color );
	}
	return 0;
}

static int WidgetStyle_SyncPostion(
		LCUI_Widget *widget,
		LCUI_StyleClass *style_class,
		const char *pseudo_class_name )
{
	//StyleLib_GetStyleAttr( style_class, pseudo_class_name, "background-color");
	return 0;
}

LCUI_API int WidgetStyle_Sync( LCUI_Widget *widget )
{
	char type_name[256], *pseudo_class_name;
	LCUI_StyleClass *style_class;
	_DEBUG_MSG("enter\n");
	if( !library_is_init ) {
		_DEBUG_MSG("!library_is_init\n");
		return -1;
	}
	if( widget == NULL ) {
		_DEBUG_MSG("widget == NULL\n");
		return -1;
	}
	if( widget->type_name.string != NULL
	 && widget->type_name.length > 0 ) {
		strcpy( type_name, widget->type_name.string );
	} else {
		strcpy( type_name, "void-widget" );
	}
	_DEBUG_MSG("type_name: %s\n", type_name);
	style_class = StyleLib_GetStyleClass( &library, type_name );
	if( style_class == NULL ) {
		_DEBUG_MSG("style_class == NULL\n");
		return 1;
	}
	switch(widget->state) {
	case WIDGET_STATE_ACTIVE:pseudo_class_name="active"; break;
	case WIDGET_STATE_OVERLAY:pseudo_class_name="overlay"; break;
	case WIDGET_STATE_DISABLE:pseudo_class_name="disable"; break;
	case WIDGET_STATE_NORMAL:
	default: pseudo_class_name=NULL; break;
	}
	WidgetStyle_SyncBackground( widget, style_class, pseudo_class_name );
	WidgetStyle_SyncPostion( widget, style_class, pseudo_class_name );
	return 0;
}