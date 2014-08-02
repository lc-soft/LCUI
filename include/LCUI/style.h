#ifndef __LCUI_STYLE_LIBRARY_H__
#define __LCUI_STYLE_LIBRARY_H__

LCUI_BEGIN_HEADER
	
typedef struct StyleLIB_Property_ {
	LCUI_String name;	/* 属性名 */
	LCUI_String value;	/* 属性值 */
} StyleLIB_Property;

typedef struct StyleLIB_PseudoClass_ {
	LCUI_String name;	/* 伪类的名称 */
	LCUI_Queue property;	/* 伪类的属性集 */
} StyleLIB_PseudoClass;

typedef struct StyleLIB_Element_ {
	LCUI_String name;		/* 名称 */
	LCUI_Queue property;		/* 属性集 */
	LCUI_Queue pseudo_class;	/* 拥有的伪类 */
} StyleLIB_Element, StyleLIB_Selector, StyleLIB_Class;

typedef struct StyleLIB_Library_ {
	LCUI_Queue selectors;	/* 选择器 */
	LCUI_Queue classes;	/* 类 */
} StyleLIB_Library;


/* 销毁样式库 */
LCUI_API void StyleLIB_Destroy( StyleLIB_Library *lib_ptr );

/* 初始化样式库 */
LCUI_API void StyleLIB_Init( StyleLIB_Library *lib_ptr );

/* 添加一个选择器 */
LCUI_API StyleLIB_Element *
StyleLIB_AddSelector(	StyleLIB_Library *lib_ptr,
			const char *selector_name );

/* 添加一个类 */
LCUI_API StyleLIB_Element *
StyleLIB_AddClass(	StyleLIB_Library *lib_ptr,
			const char *class_name );

/* 为 选择器/类 添加属性 */
LCUI_API int 
StyleLIB_AddProperty(	StyleLIB_Element *element_ptr,
			const char *pseudo_class_name,
			const char *property_name,
			const char *property_value );
						
/* 获取选择器的句柄 */
LCUI_API StyleLIB_Element *
StyleLIB_GetSelector(	StyleLIB_Library *lib_ptr,
			const char *selector_name );

/* 获取类的句柄 */
LCUI_API StyleLIB_Element *
StyleLIB_GetClass(	StyleLIB_Library *lib_ptr,
			const char *class_name );
					
/* 获取属性的句柄 */
LCUI_API StyleLIB_Property *
StyleLIB_GetProperty(	StyleLIB_Selector *selector_ptr,
			StyleLIB_Class *class_ptr,
			const char *pseudo_class_name,
			const char *property_name );

/* 获取属性值 */
LCUI_API LCUI_BOOL
StyleLIB_GetPropertyValue(	StyleLIB_Selector *selector_ptr,
				StyleLIB_Class *class_ptr,
				const char *pseudo_class_name,
				const char *property_name,
				char *value_buff );

/* 设置属性的值 */
LCUI_API void
StyleLIB_SetPropertyValue(	StyleLIB_Property *property_ptr,
				const char *property_value );

/* 根据字符串的内容，往样式库里添加相应样式 */
LCUI_API int
StyleLIB_AddStyleFromString(	StyleLIB_Library *lib,
				const char *style_string );


/* 根据指定文件内的数据，往样式库里添加相应样式 */
LCUI_API int
StyleLIB_AddStyleFromFile(	StyleLIB_Library *lib,
				const char *filepath );


LCUI_END_HEADER

#endif
