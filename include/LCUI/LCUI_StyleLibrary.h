#ifndef __LCUI_STYLE_LIBRARY_H__
#define __LCUI_STYLE_LIBRARY_H__

typedef struct {
	LCUI_Queue style_classes;
} LCUI_StyleLibrary;

typedef struct {
	LCUI_String class_name;	/* 类名 */
	LCUI_Queue style_attr;		/* 该类的样式属性 */
	LCUI_Queue pseudo_classes;	/* 伪类 */
} LCUI_StyleClass;

typedef struct {
	LCUI_String attr_name;
	LCUI_String attr_value;
} LCUI_StyleAttr;

LCUI_BEGIN_HEADER

/* 初始化样式库 */
void StyleLib_Init( LCUI_StyleLibrary *lib );

void StyleAttr_Init( LCUI_StyleAttr *attr );

void StyleClass_Init( LCUI_StyleClass *style_class );

/* 获取样式类的句柄 */
LCUI_StyleClass *
StyleLib_GetStyleClass(	LCUI_StyleLibrary *lib, 
			const char *class_name );

/* 添加指定名称的样式类到样式库中 */
LCUI_StyleClass *
StyleLib_AddStyleClass(	LCUI_StyleLibrary *lib, 
			const char *class_name );

LCUI_StyleAttr *
StyleLib_GetStyleAttr(	LCUI_StyleClass *style_class,
			const char *pseudo_class_name,
			const char *attr_name );

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
					char *attr_buff );

/* 为样式类添加样式属性 */
int StyleClass_SetStyleAttr(	LCUI_StyleClass *style_class,
				const char *pseudo_class_name,
				const char *attr_name,
				const char *attr_value );

/* 根据字符串的内容，往样式库里添加相应样式 */
int StyleLib_AddStyleFromString(	LCUI_StyleLibrary *lib,
					const char *style_string );

/* 根据指定文件内的数据，往样式库里添加相应样式 */
int StyleLib_AddStyleFromFile(	LCUI_StyleLibrary *lib,
				const char *filepath );

LCUI_END_HEADER

#endif
