//#define DEBUG1

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_STYLE_LIBRARY_H

LCUI_API void
StyleLib_Free( LCUI_StyleLibrary *lib )
{
	Queue_Destroy( &lib->style_classes );
}

/* 初始化样式库 */
LCUI_API void
StyleLib_Init( LCUI_StyleLibrary *lib )
{
	Queue_Init(	&lib->style_classes, 
			sizeof(LCUI_StyleClass), 
			NULL );
}

LCUI_API void
StyleAttr_Init( LCUI_StyleAttr *attr )
{
	LCUIString_Init( &attr->attr_name );
	LCUIString_Init( &attr->attr_value );
}

LCUI_API void
StyleClass_Init( LCUI_StyleClass *style_class )
{
	LCUIString_Init( &style_class->class_name );
	Queue_Init(	&style_class->style_attr,
			sizeof(LCUI_StyleAttr),
			NULL );
	Queue_Init(	&style_class->pseudo_classes, 
			sizeof(LCUI_StyleClass), 
			NULL );
}

/* 获取样式类的句柄 */
LCUI_API LCUI_StyleClass*
StyleLib_GetStyleClass(	LCUI_StyleLibrary *lib, 
			const char *class_name )
{
	int i, total;
	LCUI_StyleClass *p;
	
	total = Queue_GetTotal( &lib->style_classes );
	for(i=0; i<total; ++i) {
		p = (LCUI_StyleClass*)Queue_Get( &lib->style_classes, i );
		if( !p ) {
			continue;
		}
		if( _LCUIString_Cmp( &p->class_name, class_name) == 0 ) {
			return p;
		}
	}
	return NULL;
}

/* 添加指定名称的样式类到样式库中 */
LCUI_API LCUI_StyleClass*
StyleLib_AddStyleClass(	LCUI_StyleLibrary *lib, 
			const char *class_name )
{
	LCUI_StyleClass *style_class;
	
	/* 如果已存在同名类 */
	if( StyleLib_GetStyleClass( lib, class_name ) ) {
		return NULL;
	}

	style_class = (LCUI_StyleClass*)malloc( sizeof(LCUI_StyleClass) );
	if( !style_class ) {
		return NULL;
	}
	StyleClass_Init( style_class );
	/* 保存类名 */
	_LCUIString_Copy( &style_class->class_name, class_name );
	Queue_AddPointer( &lib->style_classes, style_class );
	return style_class;
}

LCUI_API LCUI_StyleAttr*
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
	total = Queue_GetTotal( &style_class->style_attr );
	for( i=0; i<total; ++i ) {
		p = (LCUI_StyleAttr*)Queue_Get( &style_class->style_attr, i );
		if( !p ) {
			continue;
		}
		if( _LCUIString_Cmp( &p->attr_name, attr_name ) == 0 ) {
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
LCUI_API int
StyleClass_GetStyleAttrValue(	LCUI_StyleClass *style_class,
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

/* 为样式类添加样式属性 */
LCUI_API int
StyleClass_SetStyleAttr(	LCUI_StyleClass *style_class,
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
		_LCUIString_Copy( &style_attr->attr_value, attr_value );
		return 0;
	}
	/* 否则，就需要新增属性项了 */
	style_attr = malloc( sizeof(LCUI_StyleAttr) );
	if( !style_attr ) {
		return -2;
	}
	StyleAttr_Init( style_attr );
	/* 保存属性名和属性值 */
	_LCUIString_Copy( &style_attr->attr_name, attr_name );
	_LCUIString_Copy( &style_attr->attr_value, attr_value );
	Queue_AddPointer( &style_class->style_attr, style_attr );
	return 0;
}

/* 记录样式扫描状态 */
typedef struct style_scan_status {
	int ch_pos;
	char name_buff[256];
	char value_buff[256];
	LCUI_BOOL save_class_name;
	LCUI_BOOL save_attr_name; 
	LCUI_BOOL save_attr_value;
	LCUI_StyleClass *cur_style;
} style_scan_status;

static void style_scan_status_init( style_scan_status *status )
{
	status->save_attr_name = FALSE;
	status->save_attr_value = FALSE;
	status->save_class_name = FALSE;
	status->cur_style = NULL;
	memset( status->name_buff, 0, sizeof(status->name_buff) );
	memset( status->value_buff, 0, sizeof(status->value_buff) );
}

static void StyleLib_ScanStyle(
		LCUI_StyleLibrary *lib,
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
				StyleClass_SetStyleAttr( status->cur_style, NULL, status->name_buff, status->value_buff );
				status->save_attr_name = TRUE;
				status->save_attr_value = FALSE;
				DEBUG_MSG1("add attr: %s = %s\n\n", status->name_buff, status->value_buff);
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
				status->cur_style = StyleLib_AddStyleClass( lib, status->name_buff );
				DEBUG_MSG1("add class: %s\n", status->name_buff);
				break;
			default:
				DEBUG_MSG1("name_buff[%d]: %c\n", status->ch_pos, *cur);
				status->name_buff[status->ch_pos] = *cur;
				++status->ch_pos;
			}
			continue;
		}
		if( *cur == '.' ) {
			status->save_class_name = TRUE;
			status->ch_pos = 0;
		}
	}
	DEBUG_MSG1("quit\n");
}

/* 根据字符串的内容，往样式库里添加相应样式 */
LCUI_API int
StyleLib_AddStyleFromString(	LCUI_StyleLibrary *lib,
				const char *style_string )
{
	style_scan_status status;
	style_scan_status_init( &status );
	StyleLib_ScanStyle( lib, &status, style_string, strlen(style_string) );
	return 0;
}

#define BUFFER_SIZE 512

/* 根据指定文件内的数据，往样式库里添加相应样式 */
LCUI_API int
StyleLib_AddStyleFromFile(	LCUI_StyleLibrary *lib,
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
		StyleLib_ScanStyle( lib, &status, buff, BUFFER_SIZE );
	}
	fclose( fp );
	return 0;
}
