#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_STYLE_LIBRARY_H

#include <math.h>

typedef struct image_data_ {
	LCUI_String path;	/* 图片文件的文件路径 */
	LCUI_Graph img;		/* 图片文件的图像数据 */
} image_data;

static LCUI_StyleLibrary style_library;
static LCUI_Queue imagefile_library;

static void destroy_imagefile_library( void *arg )
{
	image_data *data = (image_data*)arg;
	LCUIString_Free( &data->path );
	Graph_Free( &data->img );
}

/* 添加指定路径下的图片文件，并获取该图片文件的图像数据 */
static int add_imagefile( const char *filepath, LCUI_Graph *buff )
{
	int i, n;
	image_data imgdata, *data_ptr;

	if( filepath == NULL ) {
		return -1;
	}
	n = Queue_GetTotal( &imagefile_library );
	for(i=0; i<n; ++i) {
		data_ptr = (image_data*)Queue_Get( &imagefile_library , i );
		if( data_ptr == NULL ) {
			continue;
		}
		/* 如果已经存在该图片文件的记录，则直接取出图像数据 */
		if( _LCUIString_Cmp( &data_ptr->path, filepath ) == 0 ) {
			*buff = data_ptr->img;
			return 1;
		}
	}
	/* 该图片文件记录不存在，则从图片文件中载入图像数据 */
	if( Load_Image( filepath, &imgdata.img ) != 0 ) {
		return -1;
	}
	/* 载入成功后，就添加该图片文件记录 */
	LCUIString_Init( &imgdata.path );
	_LCUIString_Copy( &imgdata.path, filepath );
	Queue_Add( &imagefile_library, &imgdata );
	*buff = imgdata.img;
	return 0;
}

LCUI_API void WidgetStyle_LibraryInit( void )
{
	StyleLib_Init( &style_library );
	Queue_Init( &imagefile_library, sizeof(image_data), destroy_imagefile_library );
}

LCUI_API void WidgetStyle_LibraryDestroy( void )
{
	StyleLib_Free( &style_library );
	Queue_Destroy( &imagefile_library );
}

LCUI_API int WidgetStyle_LoadFromString( const char *style_str )
{
	return StyleLib_AddStyleFromString( &style_library, style_str );
}

/* 从字符串中指定段内获取出16进制的数 */
static int str_scan_hex_number( const char *str, int start, int end )
{
	int i, n, k;
	n = 0;
	k = end - start - 1;
	for(i=start; i<end; ++i) {
		if(str[i] >= '0' && str[i] <= '9') {
			n += (str[i]-'0')*pow(16,k);
			--k;
		} else if((str[i] >= 'a' && str[i] <= 'f')
		|| (str[i] >= 'A' && str[i] <= 'F')) {
			n += (str[i]-'a'+10)*k*pow(16,k);
			--k;
		}
	}
	return n;
}

/* 从字符串中获取RGB值，获取失败则返回0 */
static int style_color_covernt( const char *style_str, LCUI_RGB *rgb )
{
	int len;
	uchar_t r, g, b;

	len = strlen(style_str);
	if(style_str[0] == '#') {
		switch(len) {
		case 4: 
			r = str_scan_hex_number( style_str, 1, 2 );
			g = str_scan_hex_number( style_str, 2, 3 );
			b = str_scan_hex_number( style_str, 3, 4 );
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
	DEBUG_MSG("%d,%d,%d\n", r,g,b);
	return 3;
}

static int WidgetStyle_SyncBackground(
		LCUI_Widget *widget,
		LCUI_StyleClass *style_class,
		const char *pseudo_class_name )
{
	int ret;
	char *attr_value;
	LCUI_RGB back_color;
	LCUI_Graph img_bg;
	LCUI_StyleAttr *widget_attr;

	/* 根据样式类句柄，获取样式属性句柄 */
	widget_attr = StyleLib_GetStyleAttr( style_class,
			pseudo_class_name, "background-color");
	if( widget_attr != NULL ) {
		/* 引用属性值 */
		attr_value = widget_attr->attr_value.string;
		/* 将属性值转换成颜色数据 */
		ret = style_color_covernt( attr_value, &back_color );
		if( ret > 0 ) { /* 如果转换成功，则设置部件背景色 */
			Widget_SetBackgroundColor( widget, back_color );
		}
	}

	widget_attr = StyleLib_GetStyleAttr( style_class, 
			pseudo_class_name, "background-image");
	if( widget_attr != NULL ) {
		attr_value = widget_attr->attr_value.string;
		Graph_Init( &img_bg );
		/* 添加该路径的图片文件，并载入它 */
		ret = add_imagefile( attr_value, &img_bg );
		if( ret != -1 ) { /* 若正确获得图像数据，则设置为部件背景图 */
			Widget_SetBackgroundImage( widget, &img_bg );
		}
	}

	widget_attr = StyleLib_GetStyleAttr( style_class, 
			pseudo_class_name, "background-transparent");
	if( widget_attr != NULL ) {
		attr_value = widget_attr->attr_value.string;
		/* 判断属性值是"true"还是"false" */
		if( strcmp("1", attr_value) == 0
		 || lcui_strcasecmp("true",attr_value) == 0) {
			Widget_SetBackgroundTransparent( widget, TRUE );
		 } 
		else if( strcmp("0", attr_value) == 0
		 || lcui_strcasecmp("false",attr_value) == 0 ) {
			Widget_SetBackgroundTransparent( widget, FALSE );
		 }
	}
	
	widget_attr = StyleLib_GetStyleAttr( style_class, 
			pseudo_class_name, "background-layout");
	if( widget_attr != NULL ) {
		attr_value = widget_attr->attr_value.string;
		/* 判断背景图的布局方式 */
		if( lcui_strcasecmp("center",attr_value) == 0 ) {
			Widget_SetBackgroundLayout( widget, LAYOUT_CENTER );
		} else if( lcui_strcasecmp("tile",attr_value) == 0 ) {
			Widget_SetBackgroundLayout( widget, LAYOUT_TILE );
		} else if( lcui_strcasecmp("stretch",attr_value) == 0 ) {
			Widget_SetBackgroundLayout( widget, LAYOUT_STRETCH );
		} else if( lcui_strcasecmp("zoom",attr_value) == 0  ) {
			Widget_SetBackgroundLayout( widget, LAYOUT_ZOOM );
		} else if( lcui_strcasecmp("none",attr_value) == 0  ) {
			Widget_SetBackgroundLayout( widget, LAYOUT_NONE );
		} else if( lcui_strcasecmp("normal",attr_value) == 0  ) {
			Widget_SetBackgroundLayout( widget, LAYOUT_NORMAL );
		}
	}
	return 0;
}

static int WidgetStyle_SyncPostion(
		LCUI_Widget *widget,
		LCUI_StyleClass *style_class,
		const char *pseudo_class_name )
{
	return 0;
}

static int WidgetStyle_SyncBorder(
		LCUI_Widget *widget,
		LCUI_StyleClass *style_class,
		const char *pseudo_class_name )
{
	return 0;
}

static int WidgetStyle_SyncSize(
		LCUI_Widget *widget,
		LCUI_StyleClass *style_class,
		const char *pseudo_class_name )
{
	return 0;
}

LCUI_API int WidgetStyle_Sync( LCUI_Widget *widget )
{
	char type_name[256], *pseudo_class_name;
	LCUI_StyleClass *style_class;

	if( widget == NULL ) {
		return -1;
	}
	/* 如果部件指定了类型名，则直接用该类型名作为样式类名 */
	if( widget->type_name.string != NULL
	 && widget->type_name.length > 0 ) {
		strcpy( type_name, widget->type_name.string );
	} else {/* 否则，用void-widget作为缺省样式名 */
		strcpy( type_name, "void-widget" );
	}
	/* 从样式库中获取指定名字的样式类句柄 */
	style_class = StyleLib_GetStyleClass( &style_library, type_name );
	if( style_class == NULL ) {
		return 1;
	}
	/* 根据当前部件状态，选定伪类名 */
	switch(widget->state) {
	case WIDGET_STATE_ACTIVE:pseudo_class_name="active"; break;
	case WIDGET_STATE_OVERLAY:pseudo_class_name="overlay"; break;
	case WIDGET_STATE_DISABLE:pseudo_class_name="disable"; break;
	case WIDGET_STATE_NORMAL:
	default: pseudo_class_name=NULL; break;
	}
	/* 从样式库中同步部件属性 */
	WidgetStyle_SyncPostion( widget, style_class, pseudo_class_name );
	WidgetStyle_SyncSize( widget, style_class, pseudo_class_name );
	WidgetStyle_SyncBackground( widget, style_class, pseudo_class_name );
	WidgetStyle_SyncBorder( widget, style_class, pseudo_class_name );
	return 0;
}