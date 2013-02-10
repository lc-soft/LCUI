
#define USE_FREETYPE

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_FONT_H

#define NAME_MAX_LEN	256
#define PATH_MAX_LEN	1024

typedef struct {
	int pixel_size;			/* 位图像素大小 */
	LCUI_FontBMP *bitmap;		/* 位图数据 */
} LCUI_FontBMPItem;

typedef struct {
	int font_id;			/* 字体信息ID */
	LCUI_Queue font_bmp;		/* 位图库 */
} LCUI_FontDataItem;

typedef struct {
	wchar_t char_code;		/* 字符码 */
	LCUI_Queue data;		/* 与该字符关联的数据 */
} LCUI_FontCharItem;

typedef struct {
	int id;
	char style_name[NAME_MAX_LEN];	/* 风格名 */
	char family_name[NAME_MAX_LEN];	/* 字族名 */
	char filepath[PATH_MAX_LEN];	/* 字体文件路径 */
	FT_Face face;
} LCUI_FontInfo;

static int font_count = 0;
static FT_Library library = NULL;
static BOOL database_init = FALSE;
static LCUI_Queue font_database, fontbitmap_database;
static LCUI_FontInfo *default_font = NULL;

static void FontLIB_DestroyBMP( LCUI_FontBMPItem *bmp )
{
	if( bmp->bitmap ) {
		free( bmp->bitmap );
	}
}

static void FontLIB_DestroyData( LCUI_FontDataItem *data )
{
	Destroy_Queue( &data->font_bmp );
}

static void FontLIB_Destroy( LCUI_FontCharItem *font )
{
	Destroy_Queue( &font->data );
}

static void FontLIB_DestroyFontInfo( LCUI_FontInfo *info )
{
	
}

void FontLIB_DestroyAll( void )
{
	if( !database_init ) {
		return;
	}
	database_init = FALSE;
	Destroy_Queue( &font_database );
	Destroy_Queue( &fontbitmap_database );
#ifdef USE_FREETYPE
	FT_Done_FreeType( library );
#endif
}


static void FontLIB_DataInit( LCUI_FontDataItem *item )
{
	Queue_Init(	&item->font_bmp, 
			sizeof(LCUI_FontBMPItem), 
			FontLIB_DestroyBMP );
}

static void FontLIB_CharInit( LCUI_FontCharItem *item )
{
	Queue_Init(	&item->data, 
			sizeof(LCUI_FontDataItem), 
			FontLIB_DestroyData );
}

/* 初始化字体数据库 */
void FontLIB_Init( void )
{
	if( database_init ) {
		return;
	}
	Queue_Init(	&fontbitmap_database, 
			sizeof(LCUI_FontCharItem), 
			FontLIB_Destroy );
	Queue_Init(	&font_database, 
			sizeof(LCUI_FontInfo), 
			FontLIB_DestroyFontInfo );
#ifdef USE_FREETYPE
	/* 当初始化库时发生了一个错误 */
	if ( FT_Init_FreeType( &library ) ) {
		printf("%s: %s", __FUNCTION__, FT_INIT_ERROR);
		return;
	}
#else
	library = NULL;
#endif
	database_init = TRUE;
}

/* 通过字体文件路径来查找字体信息，并获取字体ID */
int FontLIB_FindInfoByFilePath( const char *filepath )
{
	int i, n;
	LCUI_FontInfo *item;
	
	if( !database_init ) {
		return -1;
	}
	n = Queue_Get_Total( &font_database );
	for(i=0; i<n; ++i) {
		item = Queue_Get( &font_database, i );
		if( !item ) {
			continue;
		}
		if( strcmp( filepath, item->filepath ) == 0 ) {
			return item->id;
		}
	}
	return -2;
}

/* 获取指定字体ID的字体face对象句柄 */
LCUI_FontInfo *FontLIB_GetFont( int font_id )
{
	int i, n;
	LCUI_FontInfo *item;
	
	if( !database_init ) {
		return NULL;
	}
	n = Queue_Get_Total( &font_database );
	for(i=0; i<n; ++i) {
		item = Queue_Get( &font_database, i );
		if( !item ) {
			continue;
		}
		if( item->id == font_id ) {
			return item;
		}
	}
	return NULL;
}

/* 获取指定字族名的字体ID */
int FontLIB_GetFontIDByFamilyName( const char *family_name )
{
	int i, n;
	LCUI_FontInfo *item;
	
	if( !database_init ) {
		return -1;
	}
	n = Queue_Get_Total( &font_database );
	for(i=0; i<n; ++i) {
		item = Queue_Get( &font_database, i );
		if( !item ) {
			continue;
		}
		/* 不区分大小写，进行对比 */
		if( strcasecmp( item->family_name, family_name) == 0 ) {
			return item->id;
		}
	}
	return -1;
}

/* 获取指定字体ID的字体face对象句柄 */
FT_Face FontLIB_GetFontFace( int font_id )
{
	LCUI_FontInfo *info;
	info = FontLIB_GetFont( font_id );
	if( !info ) {
		return NULL;
	}
	return info->face;
}

/* 获取默认的字体ID */
int FontLIB_GetDefaultFontID( void )
{
	if( !default_font ) {
		return -1;
	}
	return default_font->id;
}

/* 设定默认的字体 */
void FontLIB_SetDefaultFont( int id )
{
	LCUI_FontInfo *p;
	p = FontLIB_GetFont( id );
	if( p ) {
		default_font = p;
	}
}

/* 添加字体族，并返回该字族的ID */
int
FontLIB_AddFontInfo(	const char *family_name, const char *style_name, 
			const char *filepath, FT_Face face )
{
	LCUI_FontInfo *info;
	info = malloc( sizeof(LCUI_FontInfo) );
	info->id = ++font_count;
	strncpy( info->family_name, family_name, NAME_MAX_LEN );
	strncpy( info->style_name, style_name, NAME_MAX_LEN );
	strncpy( info->filepath, style_name,PATH_MAX_LEN );
	info->face = face;
	Queue_Add_Pointer( &font_database, info );
	return info->id;
}

static LCUI_FontBMPItem *
FontLIB_GetBMPItem( LCUI_FontDataItem *data_item, int pixel_size )
{
	int i, n;
	LCUI_FontBMPItem *item;
	
	n = Queue_Get_Total( &data_item->font_bmp );
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &data_item->font_bmp, i );
		if( !item ) {
			continue;
		}
		if( item->pixel_size == pixel_size ) {
			return item;
		}
	}
	return NULL;
}

static LCUI_FontDataItem *
FontLIB_GetDataItem( LCUI_FontCharItem *char_item, int id )
{
	int i, n;
	LCUI_FontDataItem *item;
	
	n = Queue_Get_Total( &char_item->data );
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &char_item->data, i );
		if( !item ) {
			continue;
		}
		if( item->font_id == id ) {
			return item;
		}
	}
	return NULL;
}

static LCUI_FontCharItem *
FontLIB_GetCharItem( wchar_t char_code )
{
	int i, n;
	LCUI_FontCharItem *item;
	
	n = Queue_Get_Total( &fontbitmap_database );
	for( i=0; i<n; ++i ) {
		item = Queue_Get( &fontbitmap_database, i );
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
FontLIB_AddFontBMP(	wchar_t char_code, int font_id,
			int pixel_size,	 LCUI_FontBMP *fontbmp_buff )
{
	LCUI_FontCharItem *font;
	LCUI_FontDataItem *data;
	LCUI_FontBMPItem *bmp;
	
	if( !database_init ) {
		return NULL;
	}
	/* 获取字体数据句柄，如果获取失败，则新增 */
	font = FontLIB_GetCharItem( char_code );
	if( !font ) {
		font = malloc( sizeof(LCUI_FontCharItem) );
		if( !font ) {
			return NULL;
		}
		FontLIB_CharInit( font );
		Queue_Add_Pointer( &fontbitmap_database, font );
	}
	
	/* 获取字体字族句柄，如果获取失败，则新增 */
	data = FontLIB_GetDataItem( font, font_id );
	if( !data ) {
		data = malloc( sizeof(LCUI_FontDataItem) );
		if( !data ) {
			return NULL;
		}
		FontLIB_DataInit( data );
		data->font_id = font_id;
		Queue_Add_Pointer( &font->data, data );
	}
	
	/* 获取字体位图句柄，如果获取失败，则新增 */
	bmp = FontLIB_GetBMPItem( data, pixel_size );
	if( !bmp ) {
		bmp = malloc( sizeof(LCUI_FontBMPItem) );
		if( !bmp ) {
			return NULL;
		}
		bmp->pixel_size = pixel_size;
		bmp->bitmap = NULL;
		Queue_Add_Pointer( &data->font_bmp, bmp );
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
FontLIB_GetFontBMP( wchar_t char_code, int font_id, int pixel_size )
{
	LCUI_FontCharItem *font;
	LCUI_FontDataItem *data;
	LCUI_FontBMPItem *bmp;
	
	if( !database_init ) {
		return NULL;
	}
	
	font = FontLIB_GetCharItem( char_code );
	if( !font ) {
		return NULL;
	}
	data = FontLIB_GetDataItem( font, font_id );
	if( !data ) {
		return NULL;
	}
	bmp = FontLIB_GetBMPItem( data, pixel_size );
	if( !bmp ) {
		return NULL;
	}
	return bmp->bitmap;
}

/* 载入字体值数据库中 */
int FontLIB_LoadFontFile( const char *filepath )
{
	int id, error_code;
	
	if( !filepath ) {
		return -1;
	}
	/* 如果有同一文件路径的字族信息 */
	id = FontLIB_FindInfoByFilePath( filepath );
	if( id >= 0 ) {
		return -1;
	}
	
#ifdef USE_FREETYPE
	FT_Face face;
	error_code = FT_New_Face( library, filepath , 0 , &face );
	if( error_code ) {
		FT_Done_FreeType( library );
		printf("%s: ", __FUNCTION__);
		if ( error_code == FT_Err_Unknown_File_Format ) {
			printf( "%s", FT_UNKNOWN_FILE_FORMAT );
		} else {
			printf("%s", FT_OPEN_FILE_ERROR);
		}
		perror( filepath );
		return -1;
	}
	/* 打印字体信息 */
	printf(	"=============== font info ==============\n" 
		"family name: %s\n"
		"style name : %s\n"
		"========================================\n" ,
		face->family_name, face->style_name );

	/* 设定为UNICODE，默认的也是 */
	FT_Select_Charmap( face, FT_ENCODING_UNICODE );
	/* 记录字体信息至数据库中 */
	id = FontLIB_AddFontInfo( face->family_name, face->style_name, filepath, face );
#else
	printf("%s: %s\n", __FUNCTION__, "warning: not font engine support!");
#endif
	return id;
}

/* 移除指定ID的字体数据 */
int FontLIB_DeleteFontInfo( int id )
{
	return 0;
}
