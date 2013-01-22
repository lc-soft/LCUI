#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_STYLE_LIBRARY_H
/* 初始化样式库 */
void StypeLib_Init( LCUI_StyleLibrary *lib )
{
	Queue_Init(	&lib->style_classes, 
			sizeof(LCUI_StyleClass), 
			NULL );
}

void StyleAttr_Init( LCUI_StyleAttr *attr )
{
	String_Init( &attr->attr_name );
	String_Init( &attr->attr_value );
}

void StyleClass_Init( LCUI_StyleClass *style_class )
{
	String_Init( &style_class->class_name );
	Queue_Init(	&style_class->style_attr,
			sizeof(LCUI_StyleAttr),
			NULL );
	Queue_Init(	&style_class->pseudo_classes, 
			sizeof(LCUI_StyleClass), 
			NULL );
}

/* 获取样式类的句柄 */
LCUI_StyleClass *
StyleLib_GetStyleClass(	LCUI_StyleLibrary *lib, 
			const char *class_name )
{
	int i, total;
	LCUI_StyleClass *p;
	
	total = Queue_Get_Total( &lib->style_classes );
	for(i=0; i<total; ++i) {
		p = Queue_Get( &lib->style_classes, i );
		if( !p ) {
			continue;
		}
		if( Strcmp( &p->class_name, class_name) == 0 ) {
			return p;
		}
	}
	return NULL;
}

/* 添加指定名称的样式类到样式库中 */
LCUI_StyleClass *
StyleLib_AddStyleClass(	LCUI_StyleLibrary *lib, 
			const char *class_name )
{
	LCUI_StyleClass *style_class;
	
	/* 如果已存在同名类 */
	if( StyleLib_GetStyleClass( lib, class_name ) ) {
		return NULL;
	}

	style_class = malloc( sizeof(LCUI_StyleClass) );
	if( !style_class ) {
		return NULL;
	}
	StyleClass_Init( style_class );
	/* 保存类名 */
	Strcpy( &style_class->class_name, class_name );
	Queue_Add_Pointer( &lib->style_classes, style_class );
	return style_class;
}

LCUI_StyleAttr *
StyleLib_GetStyleAttr(	LCUI_StyleClass *style_class,
			const char *pseudo_class_name,
			const char *attr_name )
{
	int i, total;
	LCUI_StyleAttr *p;
	
	if( !style_class ) {
		return NULL;
	}
	/* 先在记录中查找是否有已存在的同名属性 */
	total = Queue_Get_Total( &style_class->style_attr );
	for( i=0; i<total; ++i ) {
		p = Queue_Get( &style_class->style_attr, i );
		if( !p ) {
			continue;
		}
		if( Strcmp( &p->attr_name, attr_name ) == 0 ) {
			return p;
		}
	}
	return NULL;
}

/* 
 * 功能：获取指定样式类中的属性的值 
 * 说明：
 * class		是类句柄
 * pseudo_class_name	伪类名，为NULL时只用主类里的样式属性。
 * attr_name		属性名
 * attr_buff		储存属性值的缓冲区
 * */
int StyleClass_GetStyleAttrValue(	LCUI_StyleClass *style_class,
					const char *pseudo_class_name,
					const char *attr_name,
					char *attr_buff )
{
	LCUI_StyleAttr *style_attr;
	
	if( !style_class ) {
		return -1;
	}
	
	style_attr = StyleLib_GetStyleAttr( 
			style_class, pseudo_class_name, attr_name );
	if( !style_attr ) {
		return -1;
	}
	strcpy( attr_buff, style_attr->attr_value.string );
	return 0;
}

/* 为样式类添加样式属性值 */
int StyleLib_SetStyleAttrValue(	LCUI_StyleClass *style_class,
				const char *pseudo_class_name,
				const char *attr_name,
				const char *attr_value )
{
	LCUI_StyleAttr *style_attr;
	
	if( !style_class ) {
		return -1;
	}
	
	style_attr = StyleLib_GetStyleAttr( 
			style_class, pseudo_class_name, attr_name );
	/* 如果已存在该属性，则覆盖属性值 */
	if( style_attr ) {
		Strcpy( &style_attr->attr_value, attr_value );
		return 0;
	}
	/* 否则，就需要新增属性项了 */
	style_attr = malloc( sizeof(LCUI_StyleAttr) );
	if( !style_attr ) {
		return -2;
	}
	StyleAttr_Init( style_attr );
	/* 保存属性名和属性值 */
	Strcpy( &style_attr->attr_name, attr_name );
	Strcpy( &style_attr->attr_value, attr_value );
	Queue_Add_Pointer( &style_class->style_attr, style_attr );
	return 0;
}

/* 根据字符串的内容，往样式库里添加相应样式 */
int StyleLib_AddStyleFromString(	LCUI_StyleLibrary *lib,
					const char *style_string )
{
	return 0;
}

/* 根据指定文件内的数据，往样式库里添加相应样式 */
int StyleLib_AddStyleFromFile(	LCUI_StyleLibrary *lib,
				const char *filepath )
{
	return 0;
}
