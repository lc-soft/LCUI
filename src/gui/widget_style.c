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

static StyleLIB_Library style_library;
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
	StyleLIB_Init( &style_library );
	Queue_Init( &imagefile_library, sizeof(image_data), destroy_imagefile_library );
}

LCUI_API void WidgetStyle_LibraryDestroy( void )
{
	StyleLIB_Destroy( &style_library );
	Queue_Destroy( &imagefile_library );
}

LCUI_API int WidgetStyle_LoadFromString( const char *style_str )
{
	return StyleLib_AddStyleFromString( &style_library, style_str );
}

LCUI_API int WidgetStyle_LoadFromFile( const char *filepath )
{
	return StyleLib_AddStyleFromFile( &style_library, filepath );
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
static int style_color_convert( const char *style_str, LCUI_RGB *rgb )
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
			return -1;
		}
	} else {
		return -1;
	}
	rgb->red = r;
	rgb->green = g;
	rgb->blue = b;
	DEBUG_MSG("%d,%d,%d\n", r,g,b);
	return 0;
}

/* 根据部件的样式，同步设置部件的背景 */
static int WidgetStyle_SyncBackground(
		LCUI_Widget *widget,
		StyleLIB_Selector *selector,
		StyleLIB_Class *style_class,
		const char *pseudo_class_name )
{
	int ret;
	char *attr_value;
	LCUI_RGB back_color;
	LCUI_Graph img_bg;
	StyleLIB_Property *widget_attr;

	/* 根据样式类句柄，获取样式属性句柄 */
	widget_attr = StyleLIB_GetProperty( selector, style_class,
			pseudo_class_name, "background-color" );
	if( widget_attr != NULL ) {
		/* 引用属性值 */
		attr_value = widget_attr->value.string;
		/* 将属性值转换成颜色数据 */
		ret = style_color_convert( attr_value, &back_color );
		if( ret != -1 ) { /* 如果转换成功，则设置部件背景色 */
			Widget_SetBackgroundColor( widget, back_color );
		}
	}

	widget_attr = StyleLIB_GetProperty( selector, style_class,
			pseudo_class_name, "background-image" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		Graph_Init( &img_bg );
		/* 添加该路径的图片文件，并载入它 */
		ret = add_imagefile( attr_value, &img_bg );
		if( ret != -1 ) { /* 若正确获得图像数据，则设置为部件背景图 */
			Widget_SetBackgroundImage( widget, &img_bg );
		}
	}
	
	widget_attr = StyleLIB_GetProperty( selector, style_class,
			pseudo_class_name, "background-transparent" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
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
	
	widget_attr = StyleLIB_GetProperty( selector, style_class,
			pseudo_class_name, "background-layout" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
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

/* 根据样式，同步部件的定位 */
static int WidgetStyle_SyncPostion(
		LCUI_Widget *widget,
		StyleLIB_Selector *selector,
		StyleLIB_Class *style_class,
		const char *pseudo_class_name )
{
	int ret;
	LCUI_Pos offset;
	char *attr_value;
	IntOrFloat_t num;
	ALIGN_TYPE align;
	StyleLIB_Property *widget_attr;
	
	offset.x = widget->offset.x;
	offset.y = widget->offset.y;
	align = widget->align;
	IntOrFloat_Init( &num );
	/* 获取align属性的值 */
	widget_attr = StyleLIB_GetProperty( selector, style_class,
					pseudo_class_name, "align" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		if( lcui_strcasecmp("none",attr_value) == 0 ) {
			align = ALIGN_NONE;
		} else if( lcui_strcasecmp("top-left",attr_value) == 0 ) {
			align = ALIGN_TOP_LEFT;
		} else if( lcui_strcasecmp("top-center",attr_value) == 0 ) {
			align = ALIGN_TOP_CENTER;
		} else if( lcui_strcasecmp("top-right",attr_value) == 0 ) {
			align = ALIGN_TOP_RIGHT;
		} else if( lcui_strcasecmp("middle-left",attr_value) == 0 ) {
			align = ALIGN_MIDDLE_LEFT;
		} else if( lcui_strcasecmp("middle-center",attr_value) == 0 ) {
			align = ALIGN_MIDDLE_CENTER;
		} else if( lcui_strcasecmp("middle-right",attr_value) == 0 ) {
			align = ALIGN_MIDDLE_RIGHT;
		} else if( lcui_strcasecmp("bottom-left",attr_value) == 0 ) {
			align = ALIGN_BOTTOM_LEFT;
		} else if( lcui_strcasecmp("bottom-center",attr_value) == 0 ) {
			align = ALIGN_BOTTOM_CENTER;
		} else if( lcui_strcasecmp("bottom-right",attr_value) == 0 ) {
			align = ALIGN_BOTTOM_RIGHT;
		}
	}
	/* 获取left属性的值 */
	widget_attr = StyleLIB_GetProperty( selector, style_class,
					pseudo_class_name, "left" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		ret = GetIntOrFloat( attr_value, &num );
		if( ret == 0 ) {
			if( num.which_one == 1 ) {
				offset.x = Widget_GetContainerWidth( widget->parent );
				offset.x = (int)(offset.x * num.scale);
			} else {
				offset.x = num.px;
			}
		}
	}
	/* 获取top属性的值 */
	widget_attr = StyleLIB_GetProperty( selector, style_class,
					pseudo_class_name, "top" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		GetIntOrFloat( attr_value, &num );
		ret = GetIntOrFloat( attr_value, &num );
		if( ret == 0 ) {
			if( num.which_one == 1 ) {
				offset.y = Widget_GetContainerHeight( widget->parent );
				offset.y = (int)(offset.y * num.scale);
			} else {
				offset.y = num.px;
			}
		}
	}

	Widget_SetAlign( widget, align, offset );
	return 0;
}


/* 从字符串中获取边框数据，获取失败则返回-1 */
static int style_border_convert( const char *style_str, LCUI_Border *border )
{
	int i;
	char buff[64];
	const char *str_ptr;
	LCUI_RGB color;
	IntOrFloat_t num;
	BORDER_STYLE style_type;
	LCUI_BOOL have_color=FALSE, have_size=FALSE, have_type=FALSE;

	IntOrFloat_Init( &num );
	str_ptr = style_str;
	while(1) {
		for(i=0; i<64; ++i) {
			if( *str_ptr == ' ' ) {
				++str_ptr;
				break;
			}
			if( *str_ptr == 0 ) {
				break;
			}
			buff[i] = *str_ptr;
			++str_ptr;
		}
		buff[i] = 0;
		if( style_color_convert(buff,&color) != -1 ) {
			/* 如果已经有了边框颜色 */
			if( have_color ) {
				return -1;
			}
			have_color = TRUE;
		} else if( GetIntOrFloat(buff, &num) != -1 ) {
			/* 如果已经有了边框大小 */
			if( have_size ) {
				return -1;
			}
			/* 不支持百分比 */
			if( num.which_one == 1 ) {
				return -1;
			}
			have_size = TRUE;
		} else if( lcui_strcasecmp(buff,"solid") == 0 ) {
			if( have_type ) {
				return -1;
			}
			style_type = BORDER_STYLE_SOLID;
			have_type = TRUE;
		} else if( lcui_strcasecmp(buff,"none") == 0 ) {
			if( have_type ) {
				return -1;
			}
			style_type = BORDER_STYLE_NONE;
			have_type = TRUE;
		} else if( lcui_strcasecmp(buff,"dotted") == 0 ) {
			if( have_type ) {
				return -1;
			}
			style_type = BORDER_STYLE_DOTTED;
			have_type = TRUE;
		} else if( lcui_strcasecmp(buff,"double") == 0 ) {
			if( have_type ) {
				return -1;
			}
			style_type = BORDER_STYLE_DOUBLE;
			have_type = TRUE;
		} else if( lcui_strcasecmp(buff,"dashed") == 0 ) {
			if( have_type ) {
				return -1;
			}
			style_type = BORDER_STYLE_DASHED;
			have_type = TRUE;
		}
		if( *str_ptr == 0 ) {
			break;
		}
	}
	if( have_color && have_size && have_type ) {
		*border = Border( num.px, style_type, color );
		return 0;
	}
	return -1;
}

static int WidgetStyle_SyncBorder(
		LCUI_Widget *widget,
		StyleLIB_Selector *selector,
		StyleLIB_Class *style_class,
		const char *pseudo_class_name )
{
	int ret;
	LCUI_Border border;
	char *attr_value;
	IntOrFloat_t num;
	StyleLIB_Property *widget_attr;

	IntOrFloat_Init( &num );
	/* 获取border属性的值 */
	widget_attr = StyleLIB_GetProperty( selector, style_class,
					pseudo_class_name, "border" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		ret = style_border_convert( attr_value, &border );
		if( ret != -1 ) {
			Widget_SetBorder( widget, border );
		}
	}
	return 0;
}

/* 根据部件样式，同步部件的尺寸 */
static int WidgetStyle_SyncSize(
		LCUI_Widget *widget,
		StyleLIB_Selector *selector,
		StyleLIB_Class *style_class,
		const char *pseudo_class_name )
{
	int ret;
	char *attr_value;
	IntOrFloat_t num;
	StyleLIB_Property *widget_attr;

	widget_attr = StyleLIB_GetProperty( selector, style_class,
					pseudo_class_name, "width" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		ret = GetIntOrFloat( attr_value, &num );
		if( ret == 0 ) {
			widget->w = num;
			Widget_UpdateSize( widget );
		}
	}
	
	widget_attr = StyleLIB_GetProperty( selector, style_class,
					pseudo_class_name, "height" );
	if( widget_attr != NULL ) {
		attr_value = widget_attr->value.string;
		ret = GetIntOrFloat( attr_value, &num );
		if( ret == 0 ) {
			widget->h = num;
			Widget_UpdateSize( widget );
		}
	}
	return 0;
}

LCUI_API int WidgetStyle_Sync( LCUI_Widget *widget )
{
	StyleLIB_Class *style_class;
	StyleLIB_Selector *style_selector;
	char type_name[256], *class_name_ptr, class_name[256], *pseudo_class_name;
	
	if( widget == NULL ) {
		return -1;
	}
	/* 如果部件指定了类型名，则直接用该类型名作为样式选择器名 */
	if( widget->type_name.string != NULL
	 && widget->type_name.length > 0 ) {
		strcpy( type_name, widget->type_name.string );
	} else {/* 否则，用void-widget作为缺省选择器名 */
		strcpy( type_name, "void-widget" );
	}
	/* 从样式库中获取指定名称的 选择器 的句柄 */
	style_selector = StyleLIB_GetSelector( &style_library, type_name );

	/* 如果部件指定了样式类名，则直接用该名称作为样式类名 */
	if( widget->style_name.string != NULL
	 && widget->style_name.length > 0 ) {
		strcpy( class_name, widget->style_name.string );
		class_name_ptr = class_name;
	} else {
		class_name_ptr = NULL;
	}
	/* 从样式库中获取指定名称的 样式类 的句柄 */
	style_class = StyleLIB_GetClass( &style_library, class_name_ptr );

	/* 根据当前部件状态，选定伪类名 */
	switch(widget->state) {
	case WIDGET_STATE_ACTIVE:pseudo_class_name="active"; break;
	case WIDGET_STATE_OVERLAY:pseudo_class_name="overlay"; break;
	case WIDGET_STATE_DISABLE:pseudo_class_name="disable"; break;
	case WIDGET_STATE_NORMAL:
	default: pseudo_class_name=NULL; break;
	}
	DEBUG_MSG1("selector: %p, class: %p\n", style_selector, style_class);
	/* 从样式库中同步部件属性 */
	WidgetStyle_SyncPostion( widget, style_selector, style_class, pseudo_class_name );
	WidgetStyle_SyncSize( widget, style_selector, style_class, pseudo_class_name );
	WidgetStyle_SyncBackground( widget, style_selector, style_class, pseudo_class_name );
	WidgetStyle_SyncBorder( widget, style_selector, style_class, pseudo_class_name );
	return 0;
}
