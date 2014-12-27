/* ***************************************************************************
 * fontlibrary.c -- The font info and font bitmap cache module.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2013 归属于 刘超 <lc-soft@live.cn>
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
 */

typedef struct LCUI_FontInfo_ {
        int id;                         /**< 字体信息ID */
        char style_name[NAME_MAX_LEN];  /**< 风格名 */
        char family_name[NAME_MAX_LEN]; /**< 字族名 */
        char filepath[PATH_MAX_LEN];    /**< 字体文件路径 */
        FT_Face face;
} LCUI_FontInfo;

static int font_count = 0;
static FT_Library library = NULL;               /**< FreeType 库句柄 */
static LCUI_BOOL database_init = FALSE;         /**< 标记，指示数据库是否初始化 */
static LCUI_RBTree font_info_buffer;            /**< 字体信息缓存 */
static LCUI_RBTree fontbmp_buffer;              /**< 字体位图缓存 */
static LCUI_FontInfo *default_font = NULL;      /**< 指向默认字体信息的指针 */
static LCUI_FontInfo *in_core_font = NULL;      /**< 指向内置字体的信息的指针 */

static void FontLIB_DestroyFontBitmap( void *arg )
{
	FontBMP_Free( (LCUI_FontBMP*)arg );
}

static void FontLIB_DestroyTreeNode( void *arg )
{
        RBTree_Destroy( (LCUI_RBTree*)arg );
}

static void FontLIB_DestroyFontInfo( void *arg )
{
        
}

LCUI_API void FontLIB_DestroyAll( void )
{
        if( !database_init ) {
                return;
        }
        database_init = FALSE;
        RBTree_Destroy( &font_info_buffer );
        RBTree_Destroy( &fontbmp_buffer );
#ifdef LCUI_FONT_ENGINE_FREETYPE
        FT_Done_FreeType( library );
#endif
}

LCUI_API void FontLIB_Init( void )
{
        if( database_init ) {
                return;
        }
        
        RBTree_Init( &fontbmp_buffer );
        RBTree_Init( &font_info_buffer );
	RBTree_OnDestroy( &fontbmp_buffer, FontLIB_DestroyTreeNode );
	RBTree_SetDataNeedFree( &fontbmp_buffer, TRUE );
	RBTree_SetDataNeedFree( &font_info_buffer, TRUE );
#ifdef LCUI_FONT_ENGINE_FREETYPE
        /* 当初始化库时发生了一个错误 */
        if ( FT_Init_FreeType( &library ) ) {
                _DEBUG_MSG("failed to initialize.\n");
                return;
        }
#else
        library = NULL;
#endif
        database_init = TRUE;
}

#ifdef LCUI_FONT_ENGINE_FREETYPE
FT_Library FontLIB_GetLibrary(void)
{
        return library;
}
#endif

LCUI_API int FontLIB_FindInfoByFilePath( const char *filepath )
{
        LCUI_FontInfo *font;
        LCUI_RBTreeNode *node;

        if( !database_init ) {
                return -1;
        }
        node = RBTree_First( &font_info_buffer );
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
        if( !database_init ) {
                return NULL;
        }
        return (LCUI_FontInfo*)RBTree_GetData( &font_info_buffer, font_id );
}

LCUI_API int FontLIB_GetFontIDByFamilyName( const char *family_name )
{
        LCUI_FontInfo *font;
        LCUI_RBTreeNode *node;

        if( !database_init ) {
                return -1;
        }
        node = RBTree_First( &font_info_buffer );
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
        if( !default_font ) {
                return -1;
        }
        return default_font->id;
}

/** 设定默认的字体 */
LCUI_API void FontLIB_SetDefaultFont( int id )
{
        LCUI_FontInfo *p;
        p = FontLIB_GetFont( id );
        if( p ) {
                default_font = p;
        }
}

/** 添加字体族，并返回该字族的ID */
static int 
FontLIB_AddFontInfo(    const char *family_name, const char *style_name, 
                        const char *filepath, FT_Face face )
{
        LCUI_FontInfo *info;
        info = (LCUI_FontInfo*)malloc( sizeof(LCUI_FontInfo) );
        info->id = ++font_count;
        strncpy( info->family_name, family_name, NAME_MAX_LEN );
        strncpy( info->style_name, style_name, NAME_MAX_LEN );
        strncpy( info->filepath, filepath, PATH_MAX_LEN );
        info->face = face;
        RBTree_Insert( &font_info_buffer, info->id, info );
        return info->id;
}

LCUI_API LCUI_FontBMP* 
FontLIB_AddFontBMP( wchar_t char_code, int font_id, int pixel_size,
                                        LCUI_FontBMP *fontbmp_buff )
{
        LCUI_FontBMP *bmp;
        LCUI_RBTree *tree_font, *tree_bmp;

        if( !database_init ) {
                return NULL;
        }
        /* 获取字符的字体信息集 */
        tree_font = (LCUI_RBTree*)RBTree_GetData( &fontbmp_buffer, char_code );
        if( !tree_font ) {
                tree_font = (LCUI_RBTree*)malloc( sizeof(LCUI_RBTree) );
                if( !tree_font ) {
                        return NULL;
                }
                RBTree_Init( tree_font );
		RBTree_SetDataNeedFree( tree_font, TRUE );
		RBTree_OnDestroy( tree_font, FontLIB_DestroyTreeNode );
                RBTree_Insert( &fontbmp_buffer, char_code, tree_font );
        }
        /* 当字体ID不大于0时，使用内置字体 */
        if( font_id <= 0 ) {
                font_id = in_core_font->id;
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
        bmp = (LCUI_FontBMP*)RBTree_GetData( tree_bmp, pixel_size );
        if( !bmp ) {
                bmp = (LCUI_FontBMP*)malloc( sizeof(LCUI_FontBMP) );
                if( !bmp ) {
                        return NULL;
                }
                RBTree_Insert( tree_bmp, pixel_size, bmp );
        }
        
        /* 拷贝数据至该空间内 */
        memcpy( bmp, fontbmp_buff, sizeof(LCUI_FontBMP) );
        return bmp;
}

LCUI_API LCUI_FontBMP* 
FontLIB_GetFontBMP( wchar_t char_code, int font_id, int pixel_size )
{
        LCUI_RBTree *tree;

        if( !database_init ) {
                return NULL;
        }
        tree = (LCUI_RBTree*)RBTree_GetData( &fontbmp_buffer, char_code );
        if( !tree ) {
                return NULL;
        }
        if( font_id <= 0 ) {
                font_id = in_core_font->id;
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
        LCUI_FontBMP *bitmap_library, bmp_buff;

        bitmap_library = FontLIB_GetFontBMP( ch, font_id, pixel_size );
        if( bitmap_library ) {
                return bitmap_library;
        }
        FontBMP_Init( &bmp_buff );
        FontBMP_Load( &bmp_buff, font_id, ch, pixel_size );
        bitmap_library = FontLIB_AddFontBMP( ch, font_id, pixel_size, &bmp_buff );
        return bitmap_library;
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
        error_code = FT_New_Face( library, filepath , 0 , &face );
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
        in_core_font = FontLIB_GetFont( font_id );
        if( !in_core_font ) {
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
void LCUIModule_Font_Init( void )
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
