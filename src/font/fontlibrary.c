/* ***************************************************************************
 * fontlibrary.c -- The font info and font bitmap cache module.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * fontlibrary.c -- 字体信息和字体位图缓存模块。
 *
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>

#ifdef LCUI_FONT_ENGINE_FREETYPE

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

#else

typedef void* FT_Library;
typedef void* FT_Face;

#endif

#define NAME_MAX_LEN    256     /**< 最大名字长度 */
#define PATH_MAX_LEN    1024    /**< 最大路径长度 */

/**
 * 库中缓存的字体位图是分组存放的，共有三级分组，分别为：
 * 字符->字体信息->字体大小
 * 获取字体位图时的搜索顺序为：先找到字符的记录，然后在记录中的字体数据库里找
 * 到指定字体样式标识号的字体位图库，之后在字体位图库中找到指定像素大小的字体
 * 位图。
 * 此模块的设计只考虑到支持 FreeType，且必须在编译时确定使用哪个字体引擎，以
 * 后会考虑进行调整，以同时支持多个字体引擎，并能够随时切换，不用重新编译。
 */

typedef struct LCUI_FontInfo_ {
	int id;                         /**< 字体信息ID */
	char style_name[NAME_MAX_LEN];  /**< 风格名 */
	char family_name[NAME_MAX_LEN]; /**< 字族名 */
	char filepath[PATH_MAX_LEN];    /**< 字体文件路径 */
	FT_Face face;
} LCUI_FontInfo;

static struct FontLibraryContext {
	int count;				/**, 计数器，主要用于为字体信息生成标识号 */
	LCUI_BOOL is_inited;			/**< 标记，指示数据库是否初始化 */
	FT_Library library;			/**< FreeType 库句柄 */
	LCUI_RBTree info_cache;			/**< 字体信息缓存 */
	LCUI_RBTree bitmap_cache;		/**< 字体位图缓存 */
	LCUI_FontInfo *default_font;		/**< 指针，引用的是默认字体信息 */
	LCUI_FontInfo *incore_font;		/**< 指针，引用的是内置字体的信息 */
} fontlib = {0, FALSE};

static void FontLIB_DestroyFontBitmap( void *arg )
{
	FontBMP_Free( (LCUI_FontBMP*)arg );
}

static void FontLIB_DestroyTreeNode( void *arg )
{
	RBTree_Destroy( (LCUI_RBTree*)arg );
}

LCUI_API void FontLIB_DestroyAll( void )
{
	if( !fontlib.is_inited ) {
		return;
	}
	fontlib.is_inited = FALSE;
	RBTree_Destroy( &fontlib.info_cache );
	RBTree_Destroy( &fontlib.bitmap_cache );
#ifdef LCUI_FONT_ENGINE_FREETYPE
	FT_Done_FreeType( fontlib.library );
#endif
}

void FontLIB_Init( void )
{
	if( fontlib.is_inited ) {
		return;
	}
	RBTree_Init( &fontlib.bitmap_cache );
	RBTree_Init( &fontlib.info_cache );
	RBTree_OnDestroy( &fontlib.bitmap_cache, FontLIB_DestroyTreeNode );
	RBTree_SetDataNeedFree( &fontlib.bitmap_cache, TRUE );
	RBTree_SetDataNeedFree( &fontlib.info_cache, TRUE );
#ifdef LCUI_FONT_ENGINE_FREETYPE
	/* 当初始化库时发生了一个错误 */
	if( FT_Init_FreeType(&fontlib.library) ) {
		_DEBUG_MSG("failed to initialize.\n");
		return;
	}
#else
	fontlib.library = NULL;
#endif
	fontlib.is_inited = TRUE;
}

#ifdef LCUI_FONT_ENGINE_FREETYPE
FT_Library FontLIB_GetLibrary(void)
{
	return fontlib.library;
}
#endif

LCUI_API int FontLIB_FindInfoByFilePath( const char *filepath )
{
	LCUI_FontInfo *font;
	LCUI_RBTreeNode *node;

	if( !fontlib.is_inited ) {
		return -1;
	}
	node = RBTree_First( &fontlib.info_cache );
	while( node ) {
		font = (LCUI_FontInfo*)node->data;
		if( strcmp( filepath, font->filepath ) == 0 ) {
			return node->key;
		}
		node = RBTree_Next( node );
	}
	return -2;
}

/** 获取指定字体ID的字体信息 */
static LCUI_FontInfo* FontLIB_GetFont( int font_id )
{
	if( !fontlib.is_inited ) {
		return NULL;
	}
	return (LCUI_FontInfo*)RBTree_GetData( &fontlib.info_cache, font_id );
}

LCUI_API int FontLIB_GetFontIDByFamilyName( const char *family_name )
{
	LCUI_FontInfo *font;
	LCUI_RBTreeNode *node;

	if( !fontlib.is_inited ) {
		return -1;
	}
	node = RBTree_First( &fontlib.info_cache );
	while( node ) {
		font = (LCUI_FontInfo*)node->data;
		/* 不区分大小写，进行对比 */
		if( strcmp(font->family_name, family_name) == 0 ) {
			return node->key;
		}
		node = RBTree_Next( node );
	}
	return -2;
}

FT_Face FontLIB_GetFontFace( int font_id )
{
	LCUI_FontInfo *info;
	info = FontLIB_GetFont( font_id );
	if( !info ) {
		return NULL;
	}
	return info->face;
}

/** 获取默认的字体ID */
LCUI_API int FontLIB_GetDefaultFontID( void )
{
	if( !fontlib.default_font ) {
		return -1;
	}
	return fontlib.default_font->id;
}

/** 设定默认的字体 */
LCUI_API void FontLIB_SetDefaultFont( int id )
{
	LCUI_FontInfo *p;
	p = FontLIB_GetFont( id );
	if( p ) {
		fontlib.default_font = p;
	}
}

/** 添加字体族，并返回该字族的ID */
static int FontLIB_AddFontInfo(	const char *family_name,
				const char *style_name,
				const char *filepath,
				FT_Face face )
{
	LCUI_FontInfo *info;
	info = (LCUI_FontInfo*)malloc( sizeof(LCUI_FontInfo) );
	info->id = ++fontlib.count;
	strncpy( info->family_name, family_name, NAME_MAX_LEN );
	strncpy( info->style_name, style_name, NAME_MAX_LEN );
	strncpy( info->filepath, filepath, PATH_MAX_LEN );
	info->face = face;
	RBTree_Insert( &fontlib.info_cache, info->id, info );
	return info->id;
}

LCUI_API LCUI_FontBMP*
FontLIB_AddFontBMP(	wchar_t char_code, int font_id,
			int pixel_size, LCUI_FontBMP *fontbmp )
{
	LCUI_FontBMP *bmp_cache;
	LCUI_RBTree *tree_font, *tree_bmp;

	if( !fontlib.is_inited ) {
		return NULL;
	}
	/* 获取字符的字体信息集 */
	tree_font = (LCUI_RBTree*)
	RBTree_GetData( &fontlib.bitmap_cache, char_code );
	if( !tree_font ) {
		tree_font = (LCUI_RBTree*)malloc( sizeof(LCUI_RBTree) );
		if( !tree_font ) {
			return NULL;
		}
		RBTree_Init( tree_font );
		RBTree_SetDataNeedFree( tree_font, TRUE );
		RBTree_OnDestroy( tree_font, FontLIB_DestroyTreeNode );
		RBTree_Insert( &fontlib.bitmap_cache, char_code, tree_font );
	}
	/* 当字体ID不大于0时，使用内置字体 */
	if( font_id <= 0 ) {
		font_id = fontlib.incore_font->id;
	}
	/* 获取相应字体样式标识号的字体位图库 */
	tree_bmp = (LCUI_RBTree*)RBTree_GetData( tree_font, font_id );
	if( !tree_bmp ) {
		tree_bmp = (LCUI_RBTree*)malloc( sizeof(LCUI_RBTree) );
		if( !tree_bmp ) {
			return NULL;
		}
		RBTree_Init( tree_bmp );
		RBTree_OnDestroy( tree_bmp, FontLIB_DestroyFontBitmap );
		RBTree_SetDataNeedFree( tree_bmp, TRUE );
		RBTree_Insert( tree_font, font_id, tree_bmp );
	}
	/* 在字体位图库中获取指定像素大小的字体位图 */
	bmp_cache = (LCUI_FontBMP*)RBTree_GetData( tree_bmp, pixel_size );
	if( !bmp_cache ) {
		bmp_cache = (LCUI_FontBMP*)malloc( sizeof(LCUI_FontBMP) );
		if( !bmp_cache ) {
			return NULL;
		}
		RBTree_Insert( tree_bmp, pixel_size, bmp_cache );
	}
	/* 拷贝数据至该空间内 */
	memcpy( bmp_cache, fontbmp, sizeof(LCUI_FontBMP) );
	return bmp_cache;
}

LCUI_API LCUI_FontBMP*
FontLIB_GetFontBMP( wchar_t char_code, int font_id, int pixel_size )
{
	LCUI_RBTree *tree;
	if( !fontlib.is_inited ) {
		return NULL;
	}
	tree = (LCUI_RBTree*)RBTree_GetData( &fontlib.bitmap_cache, char_code );
	if( !tree ) {
		return NULL;
	}
	if( font_id <= 0 ) {
		font_id = fontlib.incore_font->id;
	}
	tree = (LCUI_RBTree*)RBTree_GetData( tree, font_id );
	if( !tree ) {
		return NULL;
	}
	return (LCUI_FontBMP*)RBTree_GetData( tree, pixel_size );
}

LCUI_API LCUI_FontBMP*
FontLIB_GetExistFontBMP( int font_id, wchar_t ch, int pixel_size )
{
	LCUI_FontBMP *bmp, bmp_cache;
	bmp = FontLIB_GetFontBMP( ch, font_id, pixel_size );
	if( bmp ) {
		return bmp;
	}
	FontBMP_Init( &bmp_cache );
	FontBMP_Load( &bmp_cache, font_id, ch, pixel_size );
	return FontLIB_AddFontBMP( ch, font_id, pixel_size, &bmp_cache );
}

/** 载入字体值数据库中 */
LCUI_API int FontLIB_LoadFontFile( const char *filepath )
{
	int id;
#ifdef LCUI_FONT_ENGINE_FREETYPE
	FT_Face face;
	int error_code;
#endif
	if( !filepath ) {
		return -1;
	}
	/* 如果有同一文件路径的字族信息 */
	id = FontLIB_FindInfoByFilePath( filepath );
	if( id >= 0 ) {
		return id;
	}

#ifdef LCUI_FONT_ENGINE_FREETYPE
	error_code = FT_New_Face( fontlib.library, filepath, 0, &face );
	if( error_code != 0 ) {
		_DEBUG_MSG( "%s: open error\n", filepath );
		if ( error_code == FT_Err_Unknown_File_Format ) {
			// ...
		} else {
			// ...
		}
		return -2;
	}
	/* 打印字体信息 */
	printf( "=============== font info ==============\n"
		"family name: %s\n"
		"style name : %s\n"
		"========================================\n" ,
		face->family_name, face->style_name );

	/* 设定为UNICODE，默认的也是 */
	FT_Select_Charmap( face, FT_ENCODING_UNICODE );
	/* 记录字体信息至数据库中 */
	id = FontLIB_AddFontInfo( face->family_name, face->style_name, filepath, face );
#else
	_DEBUG_MSG("warning: not font engine support!");
#endif
	return id;
}

/* 移除指定ID的字体数据 */
int FontLIB_DeleteFontInfo( int id )
{
	return 0;
}

static int FontLIB_AddInCoreFontInfo( void )
{
	int font_id;
	font_id = FontLIB_AddFontInfo( "in-core.font_8x8", "default", "\0", NULL );
	fontlib.incore_font = FontLIB_GetFont( font_id );
	if( !fontlib.incore_font ) {
		return -1;
	}
	return font_id;
}

#ifdef LCUI_BUILD_IN_WIN32
#define MAX_FONTFILE_NUM        4
#else
#define MAX_FONTFILE_NUM        1
#endif

/** 初始化字体处理模块 */
void LCUI_InitFont( void )
{
	char *p;
	int font_id = -1, i;
	char *fontfilelist[MAX_FONTFILE_NUM]={
#ifdef LCUI_BUILD_IN_WIN32
		"C:/Windows/Fonts/msyh.ttf",
		"C:/Windows/Fonts/msyh.ttc",
		"C:/Windows/Fonts/simsun.ttc",
		"C:/Windows/Fonts/consola.ttf"
#else
		"../fonts/msyh.ttf"
#endif
	};

	printf("loading fontfile...\n");
	FontLIB_Init(); /* 初始化字体数据库 */
	FontLIB_AddInCoreFontInfo(); /* 添加内置的字体信息 */
	/* 如果在环境变量中定义了字体文件路径，那就使用它 */
	p = getenv("LCUI_FONTFILE");
	if( p ) {
		printf( "load fontfile: %s\n", p );
		font_id = FontLIB_LoadFontFile( p );
	}
	/* 如果载入成功，则设定该字体为默认字体 */
	if(font_id > 0) {
		FontLIB_SetDefaultFont( font_id );
		return;
	}
	/* 否则载入失败就载入其它字体文件 */
	for(i=0; i<MAX_FONTFILE_NUM; ++i) {
		font_id = FontLIB_LoadFontFile( fontfilelist[i] );
		if(font_id > 0) {
			FontLIB_SetDefaultFont( font_id );
			break;
		}
	}
}

/** 停用字体处理模块 */
void LCUIModule_Font_End( void )
{
	FontLIB_DestroyAll();
}
