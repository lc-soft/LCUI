#include <LCUI_Build.h>
#include LC_LCUI_H

typedef struct {
	
} LCUI_StyleLibrary;


typedef struct {
	
} LCUI_StyleClass;

/* 初始化样式库 */
int StypeLib_Init( LCUI_StyleLibrary *lib )
{
	return 0;
}

/* 根据字符串的内容，往样式库里添加相应样式 */
int StyleLib_AddStyle(	LCUI_StyleLibrary *lib,
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

/* 获取样式类 */
LCUI_StyleClass *
StyleLib_GetStyleClass(	LCUI_StyleLibrary *lib, 
			const char *style_name )
{
	return NULL;
}

/* 获取指定样式类中的属性的值 */
int StyleClass_GetAttr(	LCUI_StyleClass *class,
			const char *attr_name,
			char *attr_buff )
{
	return 0;
}
