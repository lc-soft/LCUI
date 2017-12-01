/* ***************************************************************************
 * fontlibrary.c -- The font info and font bitmap cache module.
 *
 * Copyright (C) 2012-2017 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2017 归属于 刘超 <lc-soft@live.cn>
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/font.h>

#define FONT_CACHE_SIZE	32

/**
 * 库中缓存的字体位图是分组存放的，共有三级分组，分别为：
 * 字符->字体信息->字体大小
 * 获取字体位图时的搜索顺序为：先找到字符的记录，然后在记录中的字体数据库里找
 * 到指定字体样式标识号的字体位图库，之后在字体位图库中找到指定像素大小的字体
 * 位图。
 */

/** 字体字族索引结点 */
typedef struct LCUI_FontFamilyNodeRec_ {
	char *family_name;			/**< 字体的字族名称  */
	LCUI_Font styles[FONT_STYLE_TOTAL_NUM];	/**< 字体的风格记录 */
} LCUI_FontFamilyNodeRec, *LCUI_FontFamilyNode;

/** 字体路径索引结点 */
typedef struct LCUI_FontPathNode {
	char *path;		/**< 路径  */
	LCUI_Font font;		/**< 被索引的字体信息 */
} LCUI_FontPathNode;

static struct LCUI_FontLibraryModule {
	int count;			/**< 计数器，主要用于为字体信息生成标识号 */
	int font_cache_num;		/**< 字体信息缓存区的数量 */
	LCUI_BOOL is_inited;		/**< 标记，指示数据库是否初始化 */
	Dict *font_families;		/**< 字族信息库，以字族名称索引字体信息 */
	DictType font_families_type;	/**< 字族信息库的字典类型数据 */
	RBTree bitmap_cache;		/**< 字体位图缓存区 */
	LCUI_Font **font_cache;		/**< 字体信息缓存区 */
	LCUI_Font default_font;		/**< 默认字体的信息 */
	LCUI_Font incore_font;		/**< 内置字体的信息 */
	LCUI_FontEngine engines[2];	/**< 当前可用字体引擎列表 */
	LCUI_FontEngine *engine;	/**< 当前选择的字体引擎 */
} fontlib;

#define FontBitmap_IsValid(fbmp) (fbmp && fbmp->width>0 && fbmp->rows>0)
#define SelectChar(ch) (RBTree*)RBTree_GetData( &fontlib.bitmap_cache, ch )
#define SelectFont(ch, font_id) (RBTree*)RBTree_GetData( ch, font_id )
#define SelectBitmap(font, size) (LCUI_FontBitmap*)RBTree_GetData( font, size )
#define SelectFontFamliy(family_name) (LCUI_FontFamilyNode)\
	Dict_FetchValue( fontlib.font_families, family_name );
#define SelectFontCache(id) \
	fontlib.font_cache[fontlib.font_cache_num-1][id % FONT_CACHE_SIZE]

#define SetFontCache(font) do {\
	SelectFontCache(font->id) = font;\
} while( 0 );

static void DestroyFont( LCUI_Font font )
{
	free( font->family_name );
	free( font->style_name );
	font->engine->close( font->data );
	font->data = NULL;
	font->engine = NULL;
	free( font );
}

static void DestroyFontFamilyNode( void *privdata, void *data )
{
	LCUI_FontFamilyNode node = data;
	if( node->family_name ) {
		free( node->family_name );
	}
	node->family_name = NULL;
	memset( node->styles, 0, sizeof( node->styles ) );
	free( node );
}

static void DestroyFontBitmap( void *arg )
{
	FontBitmap_Free( arg );
	free( arg );
}

static void DestroyTreeNode( void *arg )
{
	RBTree_Destroy( arg );
	free( arg );
}

static LCUI_FontStyle GetFontStyle( const char *str )
{
	char name[64];
	if( !str ) {
		return FONT_STYLE_NORMAL;
	}
	strntolower( name, 64, str );
	if( strcmp( name, "italic" ) == 0 ) {
		return FONT_STYLE_ITALIC;
	}
	if( strcmp( name, "oblique" ) == 0 ) {
		return FONT_STYLE_OBLIQUE;
	}
	return FONT_STYLE_NORMAL;
}

int LCUIFont_Add( LCUI_Font font )
{
	LCUI_FontStyle style;
	LCUI_FontFamilyNode node;
	style = GetFontStyle( font->style_name );
	node = SelectFontFamliy( font->family_name );
	if( !node ) {
		node = NEW( LCUI_FontFamilyNodeRec, 1 );
		node->family_name = strdup2( font->family_name );
		memset( node->styles, 0, sizeof( node->styles ) );
		Dict_Add( fontlib.font_families, node->family_name, node );
	}
	if( node->styles[style] ) {
		font->id = node->styles[style]->id;
		DestroyFont( node->styles[style] );
	} else {
		font->id = ++fontlib.count;
	}
	if( font->id >= fontlib.font_cache_num * FONT_CACHE_SIZE ) {
		size_t size;
		LCUI_Font **caches, *cache;
		fontlib.font_cache_num += 1;
		size = fontlib.font_cache_num * sizeof( LCUI_Font* );
		caches = realloc( fontlib.font_cache, size );
		if( !caches ) {
			fontlib.font_cache_num -= 1;
			return -ENOMEM;
		}
		cache = NEW( LCUI_Font, FONT_CACHE_SIZE );
		if( !cache ) {
			return -ENOMEM;
		}
		caches[fontlib.font_cache_num - 1] = cache;
		fontlib.font_cache = caches;
	}
	node->styles[style] = font;
	SetFontCache( font );
	return font->id;
}

/** 获取指定字体ID的字体信息 */
static LCUI_Font LCUIFont_GetById( int id )
{
	if( !fontlib.is_inited ) {
		return NULL;
	}
	if( id < 0 || id >= fontlib.font_cache_num * FONT_CACHE_SIZE ) {
		return NULL;
	}
	return SelectFontCache( id );
}

size_t LCUIFont_GetIdByNames( int **font_ids, LCUI_FontStyle style,
			      const char *names )
{
	int *ids;
	char name[256];
	const char *p;
	size_t count, i;

	*font_ids = NULL;
	if( !names ) {
		return 0;
	}
	for( p = names, count = 1; *p; ++p ) {
		if( *p == ',' ) {
			++count;
		}
	}
	if( p - names == 0 ) {
		return 0;
	}
	ids = NEW( int, count + 1 );
	if( !ids ) {
		return 0;
	}
	ids[count] = -1;
	for( p = names, count = 0, i = 0; ; ++p ) {
		if( *p != ',' && *p ) {
			name[i++] = *p;
			continue;
		}
		name[i] = 0;
		strtrim( name, name, "'\"\n\r\t " );
		ids[count] = LCUIFont_GetId( name, style );
		if( ids[count] > 0 ) {
			++count;
		}
		i = 0;
		if( !*p ) {
			break;
		}
	}
	if( count < 1 ) {
		free( ids );
		ids = NULL;
	}
	*font_ids = ids;
	return count;
}

int LCUIFont_GetId( const char *family_name, LCUI_FontStyle style )
{
	LCUI_Font font = NULL;
	LCUI_FontFamilyNode fnode;
	if( !fontlib.is_inited ) {
		return -1;
	}
	fnode = SelectFontFamliy( family_name );
	if( !fnode ) {
		return -2;
	}
	while( style >= FONT_STYLE_NORMAL ) {
		font = fnode->styles[style];
		if( font ) {
			return font->id;
		}
		style -= 1;
	}
	return -3;
}

int LCUIFont_GetDefault( void )
{
	if( !fontlib.default_font ) {
		return -1;
	}
	return fontlib.default_font->id;
}

void LCUIFont_SetDefault( int id )
{
	LCUI_Font p;
	p = LCUIFont_GetById( id );
	if( p ) {
		fontlib.default_font = p;
		LOG("[font] select: %s\n", p->family_name);
	}
}

LCUI_FontBitmap* LCUIFont_AddBitmap( wchar_t ch, int font_id,
				     int size, const LCUI_FontBitmap *bmp )
{
	LCUI_FontBitmap *bmp_cache;
	RBTree *tree_font, *tree_bmp;

	if( !fontlib.is_inited ) {
		return NULL;
	}
	/* 获取字符的字体信息集 */
	tree_font = SelectChar( ch );
	if( !tree_font ) {
		tree_font = NEW( RBTree, 1 );
		if( !tree_font ) {
			return NULL;
		}
		RBTree_Init( tree_font );
		RBTree_OnDestroy( tree_font, DestroyTreeNode );
		RBTree_Insert( &fontlib.bitmap_cache, ch, tree_font );
	}
	/* 当字体ID不大于0时，使用内置字体 */
	if( font_id <= 0 ) {
		font_id = fontlib.incore_font->id;
	}
	/* 获取相应字体样式标识号的字体位图库 */
	tree_bmp = SelectFont( tree_font, font_id );
	if( !tree_bmp ) {
		tree_bmp = NEW( RBTree, 1 );
		if( !tree_bmp ) {
			return NULL;
		}
		RBTree_Init( tree_bmp );
		RBTree_OnDestroy( tree_bmp, DestroyFontBitmap );
		RBTree_Insert( tree_font, font_id, tree_bmp );
	}
	/* 在字体位图库中获取指定像素大小的字体位图 */
	bmp_cache = SelectBitmap( tree_bmp, size );
	if( !bmp_cache ) {
		bmp_cache = NEW( LCUI_FontBitmap, 1 );
		if( !bmp_cache ) {
			return NULL;
		}
		RBTree_Insert( tree_bmp, size, bmp_cache );
	}
	/* 拷贝数据至该空间内 */
	memcpy( bmp_cache, bmp, sizeof( LCUI_FontBitmap ) );
	return bmp_cache;
}

int LCUIFont_GetBitmap( wchar_t ch, int font_id, int size,
			const LCUI_FontBitmap **bmp )
{
	int ret;
	RBTree *ctx;
	LCUI_FontBitmap bmp_cache;

	*bmp = NULL;
	if( !fontlib.is_inited ) {
		return -2;
	}
	if( font_id <= 0 ) {
		if( fontlib.default_font ) {
			font_id = fontlib.default_font->id;
		} else {
			font_id = fontlib.incore_font->id;
		}
	}
	do {
		if( !(ctx = SelectChar( ch )) ) {
			break;
		}
		ctx = SelectFont( ctx, font_id );
		if( !ctx ) {
			break;
		}
		*bmp = SelectBitmap( ctx, size );
		if( *bmp ) {
			return 0;
		}
		break;
	} while( 0 );
	if( ch == 0 ) {
		return -1;
	}
	FontBitmap_Init( &bmp_cache );
	ret = LCUIFont_RenderBitmap( &bmp_cache, ch, font_id, size );
	if( ret == 0 ) {
		*bmp = LCUIFont_AddBitmap( ch, font_id, size, &bmp_cache );
		return 0;
	}
	ret = LCUIFont_GetBitmap( 0, font_id, size, bmp );
	if( ret != 0 ) {
		*bmp = LCUIFont_AddBitmap( 0, font_id, size, &bmp_cache );
	}
	return -1;
}

static int LCUIFont_LoadFileEx( LCUI_FontEngine *engine, const char *file )
{
	LCUI_Font *fonts;
	int i, num_fonts, id;

	LOG( "[font] load file: %s\n", file );
	if( !engine ) {
		return -1;
	}
	num_fonts = fontlib.engine->open( file, &fonts );
	if( num_fonts < 1 ) {
		LOG( "[font] failed to load file: %s\n", file );
		return -2;
	}
	for( i = 0; i < num_fonts; ++i ) {
		fonts[i]->engine = engine;
		id = LCUIFont_Add( fonts[i] );
		LOG( "[font] add family: %s, style name: %s, id: %d\n",
		     fonts[i]->family_name, fonts[i]->style_name, id );
	}
	free( fonts );
	return 0;
}

int LCUIFont_LoadFile( const char *filepath )
{
	return LCUIFont_LoadFileEx( fontlib.engine, filepath );
}

/** 打印字体位图的信息 */
void FontBitmap_PrintInfo( LCUI_FontBitmap *bitmap )
{
	LOG("address:%p\n",bitmap);
	if( !bitmap ) {
		return;
	}
	LOG("top: %d, left: %d, width:%d, rows:%d\n",
	bitmap->top, bitmap->left, bitmap->width, bitmap->rows);
}

/** 初始化字体位图 */
void FontBitmap_Init( LCUI_FontBitmap *bitmap )
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0;
	bitmap->buffer = NULL;
}

/** 释放字体位图占用的资源 */
void FontBitmap_Free( LCUI_FontBitmap *bitmap )
{
	if( FontBitmap_IsValid( bitmap ) ) {
		free( bitmap->buffer );
		FontBitmap_Init( bitmap );
	}
}

/** 创建字体位图 */
int FontBitmap_Create( LCUI_FontBitmap *bitmap, int width, int rows )
{
	size_t size;
	if( width < 0 || rows < 0 ) {
		FontBitmap_Free( bitmap );
		return -1;
	}
	if( FontBitmap_IsValid( bitmap ) ) {
		FontBitmap_Free( bitmap );
	}
	bitmap->width = width;
	bitmap->rows = rows;
	size = width*rows * sizeof( uchar_t );
	bitmap->buffer = (uchar_t*)malloc( size );
	if( bitmap->buffer == NULL ) {
		return -2;
	}
	return 0;
}

/** 在屏幕打印以0和1表示字体位图 */
int FontBitmap_Print( LCUI_FontBitmap *fontbmp )
{
	int x, y, m;
	for( y = 0; y < fontbmp->rows; ++y ) {
		m = y*fontbmp->width;
		for( x = 0; x < fontbmp->width; ++x, ++m ) {
			if( fontbmp->buffer[m] > 128 ) {
				LOG( "#" );
			} else if( fontbmp->buffer[m] > 64 ) {
				LOG( "-" );
			} else {
				LOG( " " );
			}
		}
		LOG( "\n" );
	}
	LOG( "\n" );
	return 0;
}

static void FontBitmap_MixARGB( LCUI_Graph *graph, LCUI_Rect *write_rect,
				const LCUI_FontBitmap *bmp, LCUI_Color color,
				LCUI_Rect *read_rect )
{
	int x, y;
	LCUI_ARGB *px, *px_row_des;
	uchar_t *byte_ptr, *byte_row_ptr;
	double a, ca, out_a, out_r, out_g, out_b, src_a;
	byte_row_ptr = bmp->buffer + read_rect->y * bmp->width;
	px_row_des = graph->argb + write_rect->y * graph->width;
	byte_row_ptr += read_rect->x;
	px_row_des += write_rect->x;
	ca = color.alpha / 255.0;
	for( y = 0; y < read_rect->height; ++y ) {
		px = px_row_des;
		byte_ptr = byte_row_ptr;
		for( x = 0; x < read_rect->width; ++x, ++byte_ptr, ++px ) {
			src_a = *byte_ptr / 255.0 * ca;
			a = (1.0 - src_a) * px->a / 255.0;
			out_r = px->r * a + color.r * src_a;
			out_g = px->g * a + color.g * src_a;
			out_b = px->b * a + color.b * src_a;
			out_a = src_a + a;
			if( out_a > 0 ) {
				out_r /= out_a;
				out_g /= out_a;
				out_b /= out_a;
			}
			px->r = (uchar_t)(out_r + 0.5);
			px->g = (uchar_t)(out_g + 0.5);
			px->b = (uchar_t)(out_b + 0.5);
			px->a = (uchar_t)(255.0 * out_a + 0.5);
		}
		px_row_des += graph->width;
		byte_row_ptr += bmp->width;
	}
}

static void FontBitmap_MixRGB( LCUI_Graph *graph, LCUI_Rect *write_rect,
			       const LCUI_FontBitmap *bmp, LCUI_Color color,
			       LCUI_Rect *read_rect )
{
	int x, y;
	uchar_t *byte_src, *byte_row_src, *byte_row_des, *byte_des, alpha;
	byte_row_src = bmp->buffer + read_rect->y*bmp->width + read_rect->x;
	byte_row_des = graph->bytes + write_rect->y * graph->bytes_per_row;
	byte_row_des += write_rect->x*graph->bytes_per_pixel;
	for( y = 0; y < read_rect->height; ++y ) {
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for( x = 0; x < read_rect->width; ++x ) {
			alpha = (uchar_t)(*byte_src * color.alpha / 255);
			ALPHA_BLEND( *byte_des, color.b, alpha );
			++byte_des;
			ALPHA_BLEND( *byte_des, color.g, alpha );
			++byte_des;
			ALPHA_BLEND( *byte_des, color.r, alpha );
			++byte_des;
			++byte_src;
		}
		byte_row_des += graph->bytes_per_row;
		byte_row_src += bmp->width;
	}
}

int FontBitmap_Mix( LCUI_Graph *graph, LCUI_Pos pos,
		    const LCUI_FontBitmap *bmp, LCUI_Color color )
{
	LCUI_Graph write_slot;
	LCUI_Rect r_rect, w_rect;
	if( pos.x > (int)graph->width || pos.y > (int)graph->height ) {
		return -2;
	}
	/* 获取写入区域 */
	w_rect.x = pos.x;
	w_rect.y = pos.y;
	w_rect.width = bmp->width;
	w_rect.height = bmp->rows;
	/* 获取需要裁剪的区域 */
	LCUIRect_GetCutArea( graph->width, graph->height, w_rect, &r_rect );
	w_rect.x += r_rect.x;
	w_rect.y += r_rect.y;
	w_rect.width = r_rect.width;
	w_rect.height = r_rect.height;
	Graph_Quote( &write_slot, graph, &w_rect );
	Graph_GetValidRect( &write_slot, &w_rect );
	/* 获取背景图引用的源图形 */
	graph = Graph_GetQuote( graph );
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		FontBitmap_MixARGB( graph, &w_rect, bmp, color, &r_rect );
	} else {
		FontBitmap_MixRGB( graph, &w_rect, bmp, color, &r_rect );
	}
	return 0;
}

int LCUIFont_RenderBitmap( LCUI_FontBitmap *buff, wchar_t ch,
			   int font_id, int pixel_size )
{
	LCUI_Font font = fontlib.default_font;
	do {
		if( font_id < 0 || !fontlib.engine ) {
			break;
		}
		font = LCUIFont_GetById( font_id );
		if( font ) {
			break;
		}
		if( fontlib.default_font ) {
			font = fontlib.default_font;
		} else {
			font = fontlib.incore_font;
		}
		break;
	} while( 0 );
	if( !font ) {
		return -1;
	}
	return font->engine->render( buff, ch, pixel_size, font );
}

void LCUI_InitFontLibrary( void )
{
	int i, fid;
#ifdef LCUI_BUILD_IN_WIN32
#define FONTDIR "C:/Windows/Fonts/"
#define MAX_FONTFILE_NUM 4
	struct {
		const char *path;
		const char *family;
		const char *style;
	} fonts[MAX_FONTFILE_NUM] = {
		{ FONTDIR"consola.ttf", "Consola", NULL },
		{ FONTDIR"simsun.ttc", "SimSun", NULL },
		{ FONTDIR"msyh.ttf", "Microsoft YaHei", NULL },
		{ FONTDIR"msyh.ttc", "Microsoft YaHei", NULL }
	};
#else
#define FONTDIR "/usr/share/fonts/"
#define MAX_FONTFILE_NUM 4
	struct {
		const char *path;
		const char *family;
		const char *style;
	} fonts[MAX_FONTFILE_NUM] = {
		{
			FONTDIR"truetype/ubuntu-font-family/Ubuntu-R.ttf",
			"Ubuntu", NULL
		}, {
			FONTDIR"opentype/noto/NotoSansCJK-Regular.ttc",
			"Noto Sans CJK SC", NULL
		}, {
			FONTDIR"opentype/noto/NotoSansCJK.ttc",
			"Noto Sans CJK SC", NULL
		}, {
			FONTDIR"truetype/wqy/wqy-microhei.ttc",
			"WenQuanYi Micro Hei", NULL
		}
	};
#endif

	fontlib.font_cache_num = 1;
	fontlib.font_cache = NEW( LCUI_Font*, 1 );
	fontlib.font_cache[0] = NEW( LCUI_Font, FONT_CACHE_SIZE );
	RBTree_Init( &fontlib.bitmap_cache );
	fontlib.font_families_type = DictType_StringKey;
	fontlib.font_families_type.valDestructor = DestroyFontFamilyNode;
	fontlib.font_families = Dict_Create( &fontlib.font_families_type, NULL );
	RBTree_OnDestroy( &fontlib.bitmap_cache, DestroyTreeNode );
	fontlib.is_inited = TRUE;

	/* 先初始化内置的字体引擎 */
	fontlib.engine = &fontlib.engines[0];
	LCUIFont_InitInCoreFont( fontlib.engine );
	LCUIFont_LoadFile( "in-core.inconsolata" );
	fid = LCUIFont_GetId( "inconsolata", FONT_STYLE_NORMAL );
	fontlib.incore_font = LCUIFont_GetById( fid );
	fontlib.default_font = fontlib.incore_font;
	/* 然后看情况启用其它字体引擎 */
#ifdef LCUI_FONT_ENGINE_FREETYPE
	if( LCUIFont_InitFreeType( &fontlib.engines[1] ) == 0 ) {
		fontlib.engine = &fontlib.engines[1];
	}
#endif
	if( fontlib.engine && fontlib.engine != &fontlib.engines[0] ) {
		LOG( "[font] current font engine is: %s\n", 
			fontlib.engine->name );
	} else {
		LOG( "[font] warning: not font engine support!\n" );
	}
	for( i = 0; i < MAX_FONTFILE_NUM; ++i ) {
		LCUIFont_LoadFile( fonts[i].path );
	}
	for( i = MAX_FONTFILE_NUM - 1; i >= 0; --i ) {
		LCUI_FontStyle style = GetFontStyle( fonts[i].style );
		fid = LCUIFont_GetId( fonts[i].family, style );
		if( fid > 0 ) {
			LCUIFont_SetDefault( fid );
			break;
		}
	}
}

void LCUI_FreeFontLibrary( void )
{
	int i;
	LCUI_Font font;

	if( !fontlib.is_inited ) {
		return;
	}
	fontlib.is_inited = FALSE;
	while( fontlib.font_cache_num > 0 ) {
		--fontlib.font_cache_num;
		for( i = 0; i < FONT_CACHE_SIZE; ++i ) {
			font = fontlib.font_cache[fontlib.font_cache_num][i];
			if( font ) {
				DestroyFont( font );
			}
		}
		free( fontlib.font_cache[fontlib.font_cache_num] );
	}
	Dict_Release( fontlib.font_families );
	RBTree_Destroy( &fontlib.bitmap_cache );
	free( fontlib.font_cache );
	fontlib.font_cache = NULL;
	LCUIFont_ExitInCoreFont();
#ifdef LCUI_FONT_ENGINE_FREETYPE
	LCUIFont_ExitFreeType();
#endif
}
