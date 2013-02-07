#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_FONT_H

typedef struct {
	int pixel_size;			/* 位图像素大小 */
	LCUI_FontBMP *bitmap;		/* 位图数据 */
} LCUI_FontBMPItem;

typedef struct {
	int style_id;			/* 风格ID */
	LCUI_Queue font_bmp;		/* 位图库 */
} LCUI_FontStyleItem;

typedef struct {
	int famliy_id;			/* 字族ID */
	LCUI_Queue font_style;		/* 风格库 */
} LCUI_FontFamliyItem;

typedef struct {
	wchar_t char_code;		/* 字符码 */
	LCUI_Queue font_famliy;	/* 字族库 */
} LCUI_FontDataItem;

#define MAX_LEN	256

typedef struct {
	int style_id;			/* 风格ID */
	char style_name[MAX_LEN];	/* 风格名 */
} LCUI_FontStyle;

typedef struct {
	int famliy_id;			/* 字族ID */
	char famliy_name[MAX_LEN];	/* 字族名 */
} LCUI_FontFamliy;

static BOOL database_init = FALSE;
static LCUI_Queue famliy_table, style_table;
static LCUI_Queue font_database;

/* 初始化字体的字族信息表 */
void FontLIB_FamliyTableInit( void )
{
	Queue_Init( &famliy_table, sizeof(LCUI_FontFamliy), NULL );
}

/* 初始化字体的样式信息表 */
void FontLIB_StyleTableInit( void )
{
	Queue_Init( &style_table, sizeof(LCUI_FontStyle), NULL );
}

void FontLIB_DestroyBMP( LCUI_FontBMPItem *bmp )
{
	
}

void FontLIB_DestroyStyle( LCUI_FontStyleItem *style )
{
	
}

void FontLIB_DestroyFamliy( LCUI_FontFamliyItem *famliy )
{
	
}

void FontLIB_Destroy( LCUI_FontDataItem *font )
{
	
}

void FontLIB_FamliyInit( LCUI_FontFamliyItem *item )
{
	Queue_Init(	&item->font_style, 
			sizeof(LCUI_FontStyleItem), 
			FontLIB_DestroyStyle );
}

void FontLIB_DataInit( LCUI_FontDataItem *item )
{
	Queue_Init(	&item->font_famliy, 
			sizeof(LCUI_FontFamliyItem), 
			FontLIB_DestroyFamliy );
}

/* 初始化字体数据库 */
void FontLIB_Init( void )
{
	if( database_init ) {
		return;
	}
	Queue_Init(	&font_database, 
			sizeof(LCUI_FontDataItem), 
			FontLIB_Destroy );
	
	FontLIB_FamliyTableInit();
	FontLIB_StyleTableInit();
}

LCUI_FontBMPItem *
FontLIB_GetBMPItem( LCUI_FontStyleItem *item, int pixel_size )
{
	return NULL;
}

LCUI_FontStyleItem *
FontLIB_GetStyleItemByID( LCUI_FontFamliyItem *item, int style_id )
{
	return NULL;
}

LCUI_FontStyleItem *
FontLIB_GetStyleItem( LCUI_FontFamliyItem *item, const char *style_name )
{
	return NULL;
}

LCUI_FontFamliyItem *
FontLIB_GetFamliyItemByID( LCUI_FontDataItem *item, int famliy_id )
{
	return NULL;
}

LCUI_FontFamliyItem *
FontLIB_GetFamliyItem( LCUI_FontDataItem *item, const char *famliy_name )
{
	return NULL;
}

LCUI_FontDataItem *
FontLIB_GetDataItem( wchar_t char_code )
{
	return NULL;
}

/* 获取字体的风格ID */
int FontLIB_GetStyleID( const char *style_name )
{
	int i, n;
	LCUI_FontStyle *item;
	
	if( !database_init ) {
		return -1;
	}
	n = Queue_Get_Total( &style_table );
	for(i=0; i<n; ++i) {
		item = Queue_Get( &style_table, i );
		if( !item ) {
			continue;
		}
		if( strcmp( style_name, item->style_name ) == 0 ) {
			return item->style_id;
		}
	}
	item = malloc( sizeof(LCUI_FontStyle) );
	if( !item ) {
		return -2;
	}
	item->style_id = rand();
	strncpy( item->style_name, style_name, MAX_LEN );
	Queue_Add_Pointer( &style_table, item );
	return item->style_id;
}

/* 获取字体的字族ID */
int FontLIB_GetFamliyID( const char *famliy_name )
{
	int i, n;
	LCUI_FontFamliy *item;
	
	if( !database_init ) {
		return -1;
	}
	n = Queue_Get_Total( &famliy_table );
	for(i=0; i<n; ++i) {
		item = Queue_Get( &famliy_table, i );
		if( !item ) {
			continue;
		}
		if( strcmp( famliy_name, item->famliy_name ) == 0 ) {
			return item->famliy_id;
		}
	}
	item = malloc( sizeof(LCUI_FontFamliy) );
	if( !item ) {
		return -2;
	}
	item->famliy_id = rand(); /* 用随机数作为字族ID */
	strncpy( item->famliy_name, famliy_name, MAX_LEN ); /* 保存字族名 */
	Queue_Add_Pointer( &famliy_table, item );
	return item->famliy_id;
}

/* 添加一个字体位图数据至数据库中 */
LCUI_FontBMP *
FontLIB_Add(	wchar_t char_code,	int famliy_id,
		int style_id,		int pixel_size,	
		LCUI_FontBMP *font_bmp )
{
	return NULL;
}

/* 获取字体位图数据 */
LCUI_FontBMP *
FontLIB_GetFontBMP(	wchar_t char_code,	int famliy_id,
			int style_id,		int pixel_size	)
{
	return NULL;
}
