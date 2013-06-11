//#define DEBUG1

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_STYLE_LIBRARY_H

static void StyleLIB_DestroyElement( void *arg )
{
	StyleLIB_Element *element;
	element = (StyleLIB_Element*)arg;
	LCUIString_Free( &element->name );
	Queue_Destroy( &element->property );
	Queue_Destroy( &element->pseudo_class );
}

static void StyleLIB_DestroyProperty( void *arg )
{
	StyleLIB_Property *prop;
	prop = (StyleLIB_Property*)arg;
	LCUIString_Free( &prop->name );
	LCUIString_Free( &prop->value );
}

static void StyleLIB_DestroyPseudoClass( void *arg )
{
	StyleLIB_PseudoClass *pclass;
	pclass = (StyleLIB_PseudoClass*)arg;
	Queue_Destroy( &pclass->property );
	LCUIString_Free( &pclass->name );
}

/* 销毁样式库 */
LCUI_API void StyleLIB_Destroy( StyleLIB_Library *lib_ptr )
{
	Queue_Destroy( &lib_ptr->selectors );
	Queue_Destroy( &lib_ptr->classes );
}

/* 初始化样式库 */
LCUI_API void StyleLIB_Init( StyleLIB_Library *lib_ptr )
{
	Queue_Init( &lib_ptr->selectors, sizeof(StyleLIB_Element),
					StyleLIB_DestroyElement );
	Queue_Init( &lib_ptr->classes, sizeof(StyleLIB_Element),
					StyleLIB_DestroyElement );
}

static StyleLIB_Element *
StyleLIB_GetElement(	StyleLIB_Library *lib_ptr,
			const char *name,
			int type )
{
	int i, total;
	LCUI_Queue *list;
	StyleLIB_Element *p;

	if( lib_ptr == NULL || name == NULL ) {
		return NULL;
	}
	if( type == 0 ) {
		list = &lib_ptr->selectors;
	} else {
		list = &lib_ptr->classes;
	}
	total = Queue_GetTotal( list );
	for(i=0; i<total; ++i) {
		p = (StyleLIB_Element*)Queue_Get( list, i );
		if( !p ) {
			continue;
		}
		if( lcui_strcasecmp(p->name.string, name) == 0 ) {
			return p;
		}
	}
	return NULL;
}

static StyleLIB_Element *
StyleLIB_AddElement(	StyleLIB_Library *lib_ptr,
			const char *name,
			int type )
{
	LCUI_Queue *list;
	StyleLIB_Element *element;
	element = StyleLIB_GetElement( lib_ptr, name, type );
	if( element != NULL ) {
		return element;
	}
	element = (StyleLIB_Element*)malloc( sizeof(StyleLIB_Element) );
	if( element == NULL ) {
		return NULL;
	}
	LCUIString_Init( &element->name );
	_LCUIString_Copy( &element->name, name );
	Queue_Init( &element->property, sizeof(StyleLIB_Property),
					StyleLIB_DestroyProperty );
	Queue_Init( &element->pseudo_class, sizeof(StyleLIB_PseudoClass),
					StyleLIB_DestroyPseudoClass );
	if( type == 0 ) {
		list = &lib_ptr->selectors;
	} else {
		list = &lib_ptr->classes;
	}
	Queue_AddPointer( list, element );
	return element;
}

/* 添加一个选择器 */
LCUI_API StyleLIB_Element *
StyleLIB_AddSelector(	StyleLIB_Library *lib_ptr,
			const char *selector_name )
{
	return StyleLIB_AddElement( lib_ptr, selector_name, 0 );
}

/* 添加一个类 */
LCUI_API StyleLIB_Element *
StyleLIB_AddClass(	StyleLIB_Library *lib_ptr,
			const char *class_name )
{
	return StyleLIB_AddElement( lib_ptr, class_name, 1 );
}

static StyleLIB_Property *
StyleLIB_FindProperty(	LCUI_Queue *property_list,
			const char *property_name )
{
	int i, n;
	StyleLIB_Property *property_ptr;

	if( property_list == NULL
	 || property_name == NULL ) {
		return NULL;
	}
	n = Queue_GetTotal( property_list );
	for(i=0; i<n; ++i) {
		property_ptr = (StyleLIB_Property*)
				Queue_Get( property_list, i );
		if( property_ptr == NULL ) {
			continue;
		}
		if( !lcui_strcasecmp(
			property_ptr->name.string,
			property_name)
		) {
			return property_ptr;
		}
	}
	return NULL;
}

static StyleLIB_Property *
StyleLIB_GetExistProperty(	LCUI_Queue *property_list,
				const char *property_name )
{
	StyleLIB_Property *property_ptr;

	if( property_list == NULL
	 || property_name == NULL ) {
		return NULL;
	}
	property_ptr = StyleLIB_FindProperty( property_list, property_name );
	if( property_ptr != NULL ) {
		return property_ptr;
	}

	property_ptr = (StyleLIB_Property*)
			malloc(sizeof(StyleLIB_Property));
	if( property_ptr == NULL ) {
		return NULL;
	}
	LCUIString_Init( &property_ptr->name );
	LCUIString_Init( &property_ptr->value );
	Queue_AddPointer( property_list, property_ptr );
	return property_ptr;
}

static StyleLIB_PseudoClass *
StyleLIB_FindPseudoClass(	StyleLIB_Element *element_ptr,
				const char *pseudo_class_name )
{
	int n, i;
	StyleLIB_PseudoClass *pclass;

	if( element_ptr == NULL
	 || pseudo_class_name == NULL ) {
		return NULL;
	}
	n = Queue_GetTotal( &element_ptr->pseudo_class );
	for(i=0; i<n; ++i) {
		pclass = (StyleLIB_PseudoClass*)
			 Queue_Get( &element_ptr->pseudo_class, i );
		if( pclass == NULL ) {
			continue;
		}
		if( !lcui_strcasecmp(
			pclass->name.string,
			pseudo_class_name)
		) {
			return pclass;
		}
	}
	return NULL;
}

static StyleLIB_PseudoClass *
StyleLIB_GetExistPseudoClass(	StyleLIB_Element *element_ptr,
				const char *pseudo_class_name )
{
	int n, i;
	StyleLIB_PseudoClass *pclass;

	if( element_ptr == NULL
	 || pseudo_class_name == NULL ) {
		return NULL;
	}
	n = Queue_GetTotal( &element_ptr->pseudo_class );
	for(i=0; i<n; ++i) {
		pclass = (StyleLIB_PseudoClass*)
			 Queue_Get( &element_ptr->pseudo_class, i );
		if( pclass == NULL ) {
			continue;
		}
		if( !lcui_strcasecmp(
			pclass->name.string,
			pseudo_class_name)
		) {
			return pclass;
		}
	}
	pclass = (StyleLIB_PseudoClass*)
		 malloc(sizeof(StyleLIB_PseudoClass));
	if( pclass == NULL ) {
		return NULL;
	}
	LCUIString_Init( &pclass->name );
	_LCUIString_Copy( &pclass->name, pseudo_class_name );
	Queue_Init( &pclass->property, sizeof(StyleLIB_Property),
					StyleLIB_DestroyProperty );
	Queue_AddPointer( &element_ptr->pseudo_class, pclass );
	return pclass;
}

/* 为 选择器/类 添加属性 */
LCUI_API int
StyleLIB_AddProperty(	StyleLIB_Element *element_ptr,
			const char *pseudo_class_name,
			const char *property_name,
			const char *property_value )
{
	LCUI_Queue *target_queue;
	StyleLIB_PseudoClass *pclass;
	StyleLIB_Property *property_ptr;
	if( element_ptr == NULL ) {
		return -1;
	}
	pclass = StyleLIB_GetExistPseudoClass(
			element_ptr, pseudo_class_name );
	if( pclass == NULL ) {
		target_queue = &element_ptr->property;
	} else {
		target_queue = &pclass->property;
	}
	property_ptr = StyleLIB_GetExistProperty(
			target_queue, property_name );
	if( property_ptr == NULL ) {
		return -2;
	}
	_LCUIString_Copy( &property_ptr->name, property_name );
	_LCUIString_Copy( &property_ptr->value, property_value );
	return 0;
}

/* 获取选择器的句柄 */
LCUI_API StyleLIB_Element *
StyleLIB_GetSelector(	StyleLIB_Library *lib_ptr,
			const char *selector_name )
{
	return StyleLIB_GetElement( lib_ptr, selector_name, 0 );
}

/* 获取类的句柄 */
LCUI_API StyleLIB_Element *
StyleLIB_GetClass(	StyleLIB_Library *lib_ptr,
			const char *class_name )
{
	return StyleLIB_GetElement( lib_ptr, class_name, 1 );
}

/* 获取属性的句柄 */
LCUI_API StyleLIB_Property *
StyleLIB_GetProperty(	StyleLIB_Selector *selector_ptr,
			StyleLIB_Class *class_ptr,
			const char *pseudo_class_name,
			const char *property_name )
{
	StyleLIB_PseudoClass *pclass;
	LCUI_Queue *property_list;
	StyleLIB_Property *first_prop, *second_prop;

	if( selector_ptr != NULL ) {
		/* 先从 选择器 查找第一个属性 */
		pclass = StyleLIB_FindPseudoClass( selector_ptr, pseudo_class_name );
		if( pclass == NULL ) {
			property_list = &selector_ptr->property;
		} else {
			property_list = &pclass->property;
		}
		first_prop = StyleLIB_FindProperty( property_list, property_name );
	} else {
		first_prop = NULL;
	}
	if( class_ptr != NULL ) {
		/* 再从 类 中查找第二个属性 */
		pclass = StyleLIB_FindPseudoClass( class_ptr, pseudo_class_name );
		if( pclass == NULL ) {
			property_list = &class_ptr->property;
		} else {
			property_list = &pclass->property;
		}
		second_prop = StyleLIB_FindProperty( property_list, property_name );
	} else {
		second_prop = NULL;
	}
	/* 如果没找到一个属性，则返回NULL */
	if( first_prop == NULL
	 && second_prop == NULL ) {
		return NULL;
	}
	/* 如果第一个属性为NULL，则返回第二个属性 */
	if( first_prop == NULL ) {
		return second_prop;
	}
	/* 直接返回第一个属性 */
	return first_prop;
}

/* 获取属性值 */
LCUI_API LCUI_BOOL
StyleLIB_GetPropertyValue(	StyleLIB_Selector *selector_ptr,
				StyleLIB_Class *class_ptr,
				const char *pseudo_class_name,
				const char *property_name,
				char *value_buff )
{
	StyleLIB_Property *prop;
	prop = StyleLIB_GetProperty( selector_ptr, class_ptr,
		pseudo_class_name, property_name );
	if( prop == NULL ) {
		return FALSE;
	}
	strcpy( value_buff, prop->value.string );
	return TRUE;
}

/* 设置属性的值 */
LCUI_API void
StyleLIB_SetPropertyValue(	StyleLIB_Property *property_ptr,
				const char *property_value )
{
	_LCUIString_Copy( &property_ptr->value, property_value );
}

/* 记录样式扫描状态 */
typedef struct style_scan_status {
	int ch_pos;
	char name_buff[256];
	char value_buff[256];
	LCUI_BOOL save_class_name;
	LCUI_BOOL save_attr_name;
	LCUI_BOOL save_attr_value;
	LCUI_BOOL is_selector;
	StyleLIB_Element *cur_style;
} style_scan_status;

static void style_scan_status_init( style_scan_status *status )
{
	status->save_attr_name = FALSE;
	status->save_attr_value = FALSE;
	status->save_class_name = FALSE;
	status->is_selector = TRUE;
	status->cur_style = NULL;
	memset( status->name_buff, 0, sizeof(status->name_buff) );
	memset( status->value_buff, 0, sizeof(status->value_buff) );
}

static void StyleLIB_ScanStyle(
		StyleLIB_Library *lib,
		style_scan_status *status,
		const char *style_string,
		int max_len )
{
	const char *cur, *max;

	cur = style_string;
	max = cur + max_len;
	DEBUG_MSG1("len: %d\n", strlen(style_string));
	for( status->ch_pos=0; cur<max && *cur!='\0'; ++cur ) {
		if( status->save_attr_name ) {
			switch(*cur) {
			case '}':
				status->save_class_name = FALSE;
				status->save_attr_name = FALSE;
				status->save_attr_value = FALSE;
				status->cur_style = NULL;
				status->name_buff[status->ch_pos] = 0;
				status->ch_pos = 0;
				goto proc_class_name;
			case '\r':
			case '\t':
			case '\n':
			case ' ': break;
			case ':':
				status->name_buff[status->ch_pos] = 0;
				status->ch_pos = 0;
				status->save_attr_name = FALSE;
				status->save_attr_value = TRUE;
				DEBUG_MSG1("end save, attr name: %s\n", status->name_buff);
				break;
			default:
				DEBUG_MSG1("name_buff[%d]: %c\n", status->ch_pos, *cur);
				status->name_buff[status->ch_pos] = *cur;
				++status->ch_pos;
				break;
			}
			continue;
		} else if( status->save_attr_value ) {
			switch(*cur) {
			case '\r':
			case '\t':
			case '\n':
				break;
			case '}':
			case ';':
				DEBUG_MSG1("i==%d\n", status->ch_pos);
				status->value_buff[status->ch_pos] = 0;
				status->ch_pos = 0;
				StyleLIB_AddProperty( status->cur_style, NULL,
					status->name_buff, status->value_buff );
				status->save_attr_name = TRUE;
				status->save_attr_value = FALSE;
				DEBUG_MSG1("add attr: %s = %s\n\n",
					status->name_buff, status->value_buff);
				if( *cur == '}' ) {
					goto proc_class_name;
				}
				break;
			case ' ':
				if(status->ch_pos == 0) {
					break;
				}
			default:
				DEBUG_MSG1("value_buff[%d]: %c\n", status->ch_pos, *cur);
				status->value_buff[status->ch_pos] = *cur;
				++status->ch_pos;
			}
			continue;
		} else if( status->save_class_name ) {
			switch(*cur) {
			case '\r':
			case '\t':
			case '\n':
			case ' ':
				break;
			case '{':
				status->name_buff[status->ch_pos] = 0;
				status->ch_pos = 0;
				status->save_class_name = FALSE;
				status->save_attr_name = TRUE;
				if( status->is_selector ) {
					status->cur_style = StyleLIB_AddSelector( lib, status->name_buff );
				} else {
					status->cur_style = StyleLIB_AddClass( lib, status->name_buff );
				}
				DEBUG_MSG1("add class: %s\n", status->name_buff);
				break;
			default:
				DEBUG_MSG1("name_buff[%d]: %c\n", status->ch_pos, *cur);
				status->name_buff[status->ch_pos] = *cur;
				++status->ch_pos;
				break;
			}
			continue;
		}
proc_class_name:
		DEBUG_MSG1("check: %c, %d\n", *cur, *cur);
		/* 如果是以字母下划线开头 */
		if( (*cur >= 'a' && *cur <= 'z')
		 || (*cur >= 'A' && *cur <= 'Z')
		 || (*cur == '_') ) {
			status->is_selector = TRUE;
			status->save_class_name = TRUE;
			status->ch_pos = 0;
			--cur; /* 下个循环中需要将当前字符记录进去 */
		} else if( *cur == '.' ) {
			status->is_selector = FALSE;
			status->save_class_name = TRUE;
			status->ch_pos = 0;
		}
	}
	DEBUG_MSG1("quit\n");
}

/* 根据字符串的内容，往样式库里添加相应样式 */
LCUI_API int
StyleLIB_AddStyleFromString(	StyleLIB_Library *lib,
				const char *style_string )
{
	style_scan_status status;
	style_scan_status_init( &status );
	StyleLIB_ScanStyle( lib, &status, style_string, strlen(style_string) );
	return 0;
}

#define BUFFER_SIZE 512

/* 根据指定文件内的数据，往样式库里添加相应样式 */
LCUI_API int
StyleLIB_AddStyleFromFile(	StyleLIB_Library *lib,
				const char *filepath )
{
	char *buff;
	FILE *fp;
	style_scan_status status;
	buff = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	if( buff == NULL ) {
		return -1;
	}
	DEBUG_MSG1("open file: %s\n", filepath);
	/* 先初始化样式扫描状态 */
	style_scan_status_init( &status );
	/* 打开文件 */
	fp = fopen( filepath, "rb" );
	if(fp == NULL) {
		DEBUG_MSG1("open file fail\n");
		return -2;
	}
	DEBUG_MSG1("open file success\n");
	/* 分段读取文件内容，并解析样式数据 */
	while( fread(buff, sizeof(char), BUFFER_SIZE, fp) ) {
		StyleLIB_ScanStyle( lib, &status, buff, BUFFER_SIZE );
	}
	fclose( fp );
	return 0;
}
