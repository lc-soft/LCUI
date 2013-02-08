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
	int family_id;			/* 字族ID */
	LCUI_Queue font_style;		/* 风格库 */
} LCUI_FontFamliyItem;

typedef struct {
	wchar_t char_code;		/* 字符码 */
	LCUI_Queue font_family;	/* 字族库 */
} LCUI_FontDataItem;

#define MAX_LEN	256

typedef struct {
	int style_id;			/* 风格ID */
	char style_name[MAX_LEN];	/* 风格名 */
} LCUI_FontStyle;

typedef struct {
	int family_id;			/* 字族ID */
	char family_name[MAX_LEN];	/* 字族名 */
} LCUI_FontFamliy;

static BOOL database_init = FALSE;
static LCUI_Queue family_table, style_table;
static LCUI_Queue font_database;

/* 初始化字体的字族信息表 */
static void FontLIB_FamliyTableInit( void )
{
	Queue_Init( &family_table, sizeof(LCUI_FontFamliy), NULL );
}

/* 初始化字体的样式信息表 */
static void FontLIB_StyleTableInit( void )
{
	Queue_Init( &style_table, sizeof(LCUI_FontStyle), NULL );
}

static void FontLIB_DestroyBMP( LCUI_FontBMPItem *bmp )
{
	if( bmp->bitmap ) {
		free( bmp->bitmap );
	}
}

static void FontLIB_DestroyStyle( LCUI_FontStyleItem *style )
{
	Destroy_Queue( &style->font_bmp );
}

static void FontLIB_DestroyFamliy( LCUI_FontFamliyItem *family )
{
	Destroy_Queue( &family->font_style );
}

static void FontLIB_Destroy( LCUI_FontDataItem *font )
{
	Destroy_Queue( &font->font_family );
}

void FontLIB_DestroyAll( void )
{
	if( !database_init ) {
		return;
	}
	Destroy_Queue( &font_database );
}

static void FontLIB_StyleInit( LCUI_FontStyleItem *item )
{
	Queue_Init(	&item->font_bmp, 
			sizeof(LCUI_FontBMPItem), 
			FontLIB_DestroyBMP );
}

static void FontLIB_FamliyInit( LCUI_FontFamliyItem *item )
{
	Queue_Init(	&item->font_style, 
			sizeof(LCUI_FontStyleItem), 
			FontLIB_DestroyStyle );
}

static void FontLIB_DataInit( LCUI_FontDataItem *item )
{
	Queue_Init(	&item->font_family, 
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
	database_init = TRUE;
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
int FontLIB_GetFamliyID( const char *family_name )
{
	int i, n;
	LCUI_FontFamliy *item;
	
	if( !database_init ) {
		return -1;
	}
	n = Queue_Get_Total( &family_table );
	for(i=0; i<n; ++i) {
		item = Queue_Get( &family_table, i );
		if( !item ) {
			continue;
		}
		if( strcmp( family_name, item->family_name ) == 0 ) {
			return item->family_id;
		}
	}
	item = malloc( sizeof(LCUI_FontFamliy) );
	if( !item ) {
		return -2;
	}
	item->family_id = rand(); /* 用随机数作为字族ID */
	strncpy( item->family_name, family_name, MAX_LEN ); /* 保存字族名 */
	Queue_Add_Pointer( &family_table, item );
	return item->family_id;
}


static LCUI_FontBMPItem *
FontLIB_GetBMPItem( LCUI_FontStyleItem *style_item, int pixel_size )
{
	int i, n;
	LCUI_FontBMPItem *item;
	
	n = Queue_Get_Total( &style_item->font_bmp );
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &style_item->font_bmp, i );
		if( !item ) {
			continue;
		}
		if( item->pixel_size == pixel_size ) {
			return item;
		}
	}
	return NULL;
}

static LCUI_FontStyleItem *
FontLIB_GetStyleItemByID( LCUI_FontFamliyItem *family_item, int style_id )
{
	int i, n;
	LCUI_FontStyleItem *item;
	
	n = Queue_Get_Total( &family_item->font_style);
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &family_item->font_style, i );
		if( !item ) {
			continue;
		}
		if( item->style_id == style_id ) {
			return item;
		}
	}
	return NULL;
}

static LCUI_FontFamliyItem *
FontLIB_GetFamliyItemByID( LCUI_FontDataItem *data_item, int family_id )
{
	int i, n;
	LCUI_FontFamliyItem *item;
	
	n = Queue_Get_Total( &data_item->font_family );
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &data_item->font_family, i );
		if( !item ) {
			continue;
		}
		if( item->family_id == family_id ) {
			return item;
		}
	}
	return NULL;
}

static LCUI_FontDataItem *
FontLIB_GetDataItem( wchar_t char_code )
{
	int i, n;
	LCUI_FontDataItem *item;
	
	n = Queue_Get_Total( &font_database );
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &font_database, i );
		if( !item ) {
			continue;
		}
		if( item->char_code == char_code ) {
			return item;
		}
	}
	return NULL;
}

/* 
 * 添加一个字体位图数据至数据库中
 * 注意：调用此函数后，作为参数fontbmp_buff的变量，不能被free掉，否则，数据库中记录
 * 的此数据会无效 
 * */
LCUI_FontBMP *
FontLIB_Add(	wchar_t char_code,	int family_id,
		int style_id,		int pixel_size,	
		LCUI_FontBMP *fontbmp_buff )
{
	LCUI_FontDataItem *font;
	LCUI_FontFamliyItem *family;
	LCUI_FontStyleItem *style;
	LCUI_FontBMPItem *bmp;
	
	if( !database_init ) {
		return NULL;
	}
	/* 获取字体数据句柄，如果获取失败，则新增 */
	font = FontLIB_GetDataItem( char_code );
	if( !font ) {
		font = malloc( sizeof(LCUI_FontDataItem) );
		if( !font ) {
			return NULL;
		}
		FontLIB_DataInit( font );
		Queue_Add_Pointer( &font_database, font );
	}
	
	/* 获取字体字族句柄，如果获取失败，则新增 */
	family = FontLIB_GetFamliyItemByID( font, family_id );
	if( !family ) {
		family = malloc( sizeof(LCUI_FontFamliyItem) );
		if( !family ) {
			return NULL;
		}
		FontLIB_FamliyInit( family );
		Queue_Add_Pointer( &font->font_family, family );
	}
	
	/* 获取字体风格句柄，如果获取失败，则新增 */
	style = FontLIB_GetStyleItemByID( family, style_id );
	if( !style ) {
		style = malloc( sizeof(LCUI_FontStyleItem) );
		if( !style ) {
			return NULL;
		}
		FontLIB_StyleInit( style );
		Queue_Add_Pointer( &family->font_style, style );
	}
	
	/* 获取字体位图句柄，如果获取失败，则新增 */
	bmp = FontLIB_GetBMPItem( style, pixel_size );
	if( !bmp ) {
		bmp = malloc( sizeof(LCUI_FontBMPItem) );
		if( !bmp ) {
			return NULL;
		}
		bmp->pixel_size = pixel_size;
		bmp->bitmap = NULL;
		Queue_Add_Pointer( &style->font_bmp, bmp );
	}
	
	/* 如果该指针为NULL，那么就申请一块空间 */
	if( NULL == bmp->bitmap ) {
		bmp->bitmap = malloc( sizeof(LCUI_FontBMP) );
		if( !bmp->bitmap ) {
			return NULL;
		}
	}
	/* 拷贝数据至该空间内 */
	memcpy( bmp->bitmap, fontbmp_buff, sizeof(LCUI_FontBMP) );
	return bmp->bitmap;
}

/* 获取字体位图数据 */
LCUI_FontBMP *
FontLIB_GetFontBMP(	wchar_t char_code,	int family_id,
			int style_id,		int pixel_size	)
{
	LCUI_FontDataItem *font;
	LCUI_FontFamliyItem *family;
	LCUI_FontStyleItem *style;
	LCUI_FontBMPItem *bmp;
	
	if( !database_init ) {
		return NULL;
	}
	
	font = FontLIB_GetDataItem( char_code );
	if( !font ) {
		return NULL;
	}
	family = FontLIB_GetFamliyItemByID( font, family_id );
	if( !family ) {
		return NULL;
	}
	style = FontLIB_GetStyleItemByID( family, style_id );
	if( !style ) {
		return NULL;
	}
	bmp = FontLIB_GetBMPItem( style, pixel_size );
	if( !bmp ) {
		return NULL;
	}
	return bmp->bitmap;
}
