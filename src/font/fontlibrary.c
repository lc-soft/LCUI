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
#include <LCUI/graph.h>
#include <LCUI/font.h>

#define NAME_MAX_LEN    64
#define PATH_MAX_LEN    1024

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
	char filepath[PATH_MAX_LEN];    /**< 字体文件路径 */
	LCUI_FontFace *face;
} LCUI_FontInfo;

static struct FontLibraryContext {
	int count;				/**, 计数器，主要用于为字体信息生成标识号 */
	LCUI_BOOL is_inited;			/**< 标记，指示数据库是否初始化 */
	LCUI_RBTree info_cache;			/**< 字体信息缓存 */
	LCUI_RBTree bitmap_cache;		/**< 字体位图缓存 */
	LCUI_FontInfo *default_font;		/**< 指针，引用的是默认字体信息 */
	LCUI_FontInfo *incore_font;		/**< 指针，引用的是内置字体的信息 */
	LCUI_FontEngine engines[2];		/**< 当前可用字体引擎列表 */
	LCUI_FontEngine *engine;		/**< 当前选择的字体引擎 */
} fontlib = {0, FALSE};

static void FontLIB_DestroyFontBitmap( void *arg )
{
	FontBMP_Free( (LCUI_FontBMP*)arg );
}

static void FontLIB_DestroyTreeNode( void *arg )
{
	RBTree_Destroy( (LCUI_RBTree*)arg );
}

/** 添加字体族，并返回该字族的ID */
static int FontLIB_AddFontInfo(	LCUI_FontFace *face, const char *filepath )
{
	LCUI_FontInfo *info;
	info = (LCUI_FontInfo*)malloc( sizeof(LCUI_FontInfo) );
	info->id = ++fontlib.count;
	strncpy( info->filepath, filepath, PATH_MAX_LEN );
	info->face = face;
	RBTree_Insert( &fontlib.info_cache, info->id, info );
	return info->id;
}

/** 获取指定字体ID的字体信息 */
static LCUI_FontInfo* FontLIB_GetFont( int font_id )
{
	if( !fontlib.is_inited ) {
		return NULL;
	}
	return (LCUI_FontInfo*)RBTree_GetData( &fontlib.info_cache, font_id );
}

void FontLIB_Init( void )
{
	int font_id;
	LCUI_FontFace *face;

	if( fontlib.is_inited ) {
		return;
	}
	RBTree_Init( &fontlib.bitmap_cache );
	RBTree_Init( &fontlib.info_cache );
	RBTree_OnDestroy( &fontlib.bitmap_cache, FontLIB_DestroyTreeNode );
	RBTree_SetDataNeedFree( &fontlib.bitmap_cache, TRUE );
	RBTree_SetDataNeedFree( &fontlib.info_cache, TRUE );
	fontlib.is_inited = TRUE;

	face = (LCUI_FontFace*)malloc(sizeof(LCUI_FontFace));
	strcpy( face->style_name, "default" );
	strcpy( face->family_name, "in-core.font_8x8" );
	face->data = NULL;
	font_id = FontLIB_AddFontInfo( face, "<in-core>" );
	fontlib.incore_font = FontLIB_GetFont( font_id );
}

int FontLIB_FindInfoByFilePath( const char *filepath )
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

int FontLIB_GetFontIDByFamilyName( const char *family_name )
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
		if( strcmp(font->face->family_name, family_name) == 0 ) {
			return node->key;
		}
		node = RBTree_Next( node );
	}
	return -2;
}

/** 获取默认的字体ID */
int FontLIB_GetDefaultFontID( void )
{
	if( !fontlib.default_font ) {
		return -1;
	}
	return fontlib.default_font->id;
}

/** 设定默认的字体 */
void FontLIB_SetDefaultFont( int id )
{
	LCUI_FontInfo *p;
	p = FontLIB_GetFont( id );
	if( p ) {
		fontlib.default_font = p;
	}
}

LCUI_FontBMP* FontLIB_AddFontBMP( wchar_t ch, int font_id,
				  int pixel_size, LCUI_FontBMP *bmp )
{
	LCUI_FontBMP *bmp_cache;
	LCUI_RBTree *tree_font, *tree_bmp;

	if( !fontlib.is_inited ) {
		return NULL;
	}
	/* 获取字符的字体信息集 */
	tree_font = (LCUI_RBTree*)RBTree_GetData( &fontlib.bitmap_cache, ch );
	if( !tree_font ) {
		tree_font = (LCUI_RBTree*)malloc( sizeof(LCUI_RBTree) );
		if( !tree_font ) {
			return NULL;
		}
		RBTree_Init( tree_font );
		RBTree_SetDataNeedFree( tree_font, TRUE );
		RBTree_OnDestroy( tree_font, FontLIB_DestroyTreeNode );
		RBTree_Insert( &fontlib.bitmap_cache, ch, tree_font );
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
	memcpy( bmp_cache, bmp, sizeof(LCUI_FontBMP) );
	return bmp_cache;
}

LCUI_FontBMP* FontLIB_GeFontBMP( int font_id, wchar_t ch, int pixel_size )
{
	LCUI_RBTree *tree;
	LCUI_FontBMP *bmp, bmp_cache;

	if( !fontlib.is_inited ) {
		return NULL;
	}
	tree = (LCUI_RBTree*)RBTree_GetData( &fontlib.bitmap_cache, ch );
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
	bmp = (LCUI_FontBMP*)RBTree_GetData( tree, pixel_size );
	if( bmp ) {
		return bmp;
	}
	FontBMP_Init( &bmp_cache );
	FontBMP_Load( &bmp_cache, font_id, ch, pixel_size );
	return FontLIB_AddFontBMP( ch, font_id, pixel_size, &bmp_cache );
}

/** 载入字体值数据库中 */
int FontLIB_LoadFontFile( const char *filepath )
{
	int ret, id;
	LCUI_FontFace *face;

	/* 如果有同一文件路径的字族信息 */
	id = FontLIB_FindInfoByFilePath( filepath );
	if( id >= 0 ) {
		return id;
	}
	if( !fontlib.engine ) {
		return -1;
	}
	ret = fontlib.engine->open( filepath, &face );
	if( ret != 0 ) {
		return -2;
	}
	id = FontLIB_AddFontInfo( face, filepath );
	return id;
}

/* 移除指定ID的字体数据 */
int FontLIB_DeleteFontInfo( int id )
{
	return 0;
}


/** 检测位图数据是否有效 */
#define FontBMP_IsValid(fbmp) (fbmp && fbmp->width>0 && fbmp->rows>0)

/** 打印字体位图的信息 */
void FontBMP_PrintInfo( LCUI_FontBMP *bitmap )
{
	printf("address:%p\n",bitmap);
	if( !bitmap ) {
		return;
	}
	printf("top: %d, left: %d, width:%d, rows:%d\n",
	bitmap->top, bitmap->left, bitmap->width, bitmap->rows);
}

/** 初始化字体位图 */
void FontBMP_Init( LCUI_FontBMP *bitmap )
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0;
	bitmap->buffer = NULL;
}

/** 释放字体位图占用的资源 */
void FontBMP_Free( LCUI_FontBMP *bitmap )
{
	if( FontBMP_IsValid(bitmap) ) {
		free( bitmap->buffer );
		FontBMP_Init( bitmap );
	}
}

/** 创建字体位图 */
int FontBMP_Create( LCUI_FontBMP *bitmap, int width, int rows )
{
	size_t size;
	if(width < 0 || rows < 0) {
		FontBMP_Free(bitmap);
		return -1;
	}
	if(FontBMP_IsValid(bitmap)) {
		FontBMP_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->rows = rows;
	size = width*rows*sizeof(uchar_t);
	bitmap->buffer = (uchar_t*)malloc( size );
	if( bitmap->buffer == NULL ) {
		return -2;
	}
	return 0;
}

/** 根据字符编码，获取内置的字体位图 */
static void GetDefaultFontBMP( unsigned short code, LCUI_FontBMP *out_bitmap )
{
	int i,j, start, m;
	uchar_t const *p;

	p = in_core_font_8x8(); /* 获取指向内置字体位图数组的指针 */
	FontBMP_Create(out_bitmap, 8, 8); /* 为位图分配内存，8x8的尺寸 */
	out_bitmap->left = 0;
	out_bitmap->top = 0;
	if(code < 256) { /* 如果在ASCII编码范围内 */
		if(code == ' ') { /* 空格就直接置0 */
			memset(out_bitmap->buffer, 0, sizeof(uchar_t)*64);
		} else {
			start = code * 8;
			for (i=start;i<start+8;++i) {
				m = (i-start) * 8 + 7;
				/* 将数值转换成一行位图 */
				for (j=0;j<8;++j,--m) {
					out_bitmap->buffer[m]
					 = (p[i]&(1<<j))?255:0;
				}
			}
		}
	} else { /* 否则用“口”表示未知编码字符 */
		static uchar_t null_bmp[] = {
			1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1 };
		for (i=0;i<8;i++) {
			m = i*8;
			for (j=0;j<8;j++,++m) {
				out_bitmap->buffer[m] = (null_bmp[m]>0)?255:0;
			}
		}
	}
	/* 修改相关属性 */
	out_bitmap->top = 8;
	out_bitmap->left = 0;
	out_bitmap->rows = 8;
	out_bitmap->width = 8;
	out_bitmap->advance.x = 8;
	out_bitmap->advance.y = 8;
}

/** 在屏幕打印以0和1表示字体位图 */
int FontBMP_Print( LCUI_FontBMP *fontbmp )
{
	int x,y,m;
	for(y = 0;y < fontbmp->rows; ++y){
		m = y*fontbmp->width;
		for(x = 0; x < fontbmp->width; ++x,++m){
			if(fontbmp->buffer[m] > 128) {
				printf("#");
			} else if(fontbmp->buffer[m] > 64) {
				printf("-");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

/** 将字体位图绘制到目标图像上 */
int FontBMP_Mix( LCUI_Graph *graph, LCUI_Pos pos,
		 LCUI_FontBMP *bmp, LCUI_Color color )
{
	int val, x, y;
	LCUI_Graph *des;
	LCUI_Rect des_rect, cut;
	LCUI_ARGB *px_des, *px_row_des;
	uchar_t *bmp_src, *bmp_row_src, *byte_row_des, *byte_des;

	/* 数据有效性检测 */
	if( !FontBMP_IsValid( bmp ) || !Graph_IsValid( graph ) ) {
		return -1;
	}
	/* 获取背景图形的有效区域 */
	Graph_GetValidRect( graph, &des_rect );
	/* 获取背景图引用的源图形 */
	des = Graph_GetQuote( graph );
	/* 起点位置的有效性检测 */
	if(pos.x > des->w || pos.y > des->h) {
		return -2;
	}
	/* 获取需要裁剪的区域 */
	LCUIRect_GetCutArea( Size( des_rect.width, des_rect.height ),
			Rect( pos.x, pos.y, bmp->width, bmp->rows ), &cut );
	pos.x += cut.x;
	pos.y += cut.y;
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		bmp_row_src = bmp->buffer + cut.y*bmp->width + cut.x;
		px_row_des = des->argb + (pos.y + des_rect.y) * des->w;
		px_row_des += pos.x + des_rect.x;
		for( y=0; y<cut.h; ++y ) {
			bmp_src = bmp_row_src;
			px_des = px_row_des;
			for( x=0; x<cut.w; ++x ) {
				ALPHA_BLEND( px_des->r, color.r, *bmp_src );
				ALPHA_BLEND( px_des->g, color.g, *bmp_src );
				ALPHA_BLEND( px_des->b, color.b, *bmp_src );
				if( px_des->alpha == 255 || *bmp_src == 255 ) {
					px_des->alpha = 255;
				} else {
					val = (255 - *bmp_src)*(255 - px_des->alpha);
					px_des->alpha = (uchar_t)(255 - val / 65025);
				}
				++bmp_src;
				++px_des;
			}
			px_row_des += des->w;
			bmp_row_src += bmp->width;
		}
		return 0;
	}

	bmp_row_src = bmp->buffer + cut.y*bmp->width + cut.x;
	byte_row_des = des->bytes + (pos.y + des_rect.y) * des->bytes_per_row;
	byte_row_des += (pos.x + des_rect.x)*des->bytes_per_pixel;
	for( y=0; y<cut.h; ++y ) {
		bmp_src = bmp_row_src;
		byte_des = byte_row_des;
		for( x=0; x<cut.w; ++x ) {
			ALPHA_BLEND( *byte_des, color.b, *bmp_src );
			byte_des++;
			ALPHA_BLEND( *byte_des, color.g, *bmp_src );
			byte_des++;
			ALPHA_BLEND( *byte_des, color.r, *bmp_src );
			byte_des++;
			++bmp_src;
		}
		byte_row_des += des->w*3;
		bmp_row_src += bmp->width;
	}
	return 0;
}


/** 载入字体位图 */
int FontBMP_Load( LCUI_FontBMP *buff, int font_id,
		  wchar_t ch, int pixel_size )
{
	LCUI_FontInfo *info;

	if( font_id <= 0 || !fontlib.engine ) {
		GetDefaultFontBMP( ch, buff );
		return -1;
	}
	info = FontLIB_GetFont( font_id );
	if( !info ) {
		GetDefaultFontBMP( ch, buff );
		return -1;
	}
	return fontlib.engine->render( buff, ch, pixel_size, info->face );
}



#ifdef LCUI_BUILD_IN_WIN32
#define MAX_FONTFILE_NUM        4
#else
#define MAX_FONTFILE_NUM        1
#endif

/** 初始化字体处理模块 */
void LCUI_InitFont( void )
{
	char *target_font;
	int font_id = -1, i = -1;
	char *font_files[MAX_FONTFILE_NUM]={
#ifdef LCUI_BUILD_IN_WIN32
		"C:/Windows/Fonts/msyh.ttf",
		"C:/Windows/Fonts/msyh.ttc",
		"C:/Windows/Fonts/simsun.ttc",
		"C:/Windows/Fonts/consola.ttf"
#else
		"../fonts/msyh.ttf"
#endif
	};
	FontLIB_Init();
	fontlib.engine = NULL;
#ifdef LCUI_FONT_ENGINE_FREETYPE
	if( LCUIFont_InitFreeType( &fontlib.engines[0] ) == 0 ) {
		fontlib.engine = &fontlib.engines[0];
	}
#endif
	if( fontlib.engine ) {
		printf("[font] current font engine is: %s\n", fontlib.engine->name);
	} else {
		printf("[font] warning: not font engine support!\n");
	}
	/* 如果在环境变量中定义了字体文件路径，那就使用它 */
	target_font = getenv("LCUI_FONTFILE");
	do {
		if( !target_font ) {
			continue;
		}
		printf( "[font] load font: %s\n", target_font );
		/* 如果载入成功，则设定该字体为默认字体 */
		font_id = FontLIB_LoadFontFile( target_font );
		if( font_id <= 0 ) {
			continue;
		}
		FontLIB_SetDefaultFont( font_id );
		if( fontlib.default_font ) {
			printf("[font] select font: %s\n",
				fontlib.default_font->face->family_name);
		}
		break;
	} while( ++i, target_font = font_files[i], i<MAX_FONTFILE_NUM );
}

/** 停用字体处理模块 */
void LCUI_ExitFont( void )
{
	if( !fontlib.is_inited ) {
		return;
	}
	fontlib.is_inited = FALSE;
	RBTree_Destroy( &fontlib.info_cache );
	RBTree_Destroy( &fontlib.bitmap_cache );
}
