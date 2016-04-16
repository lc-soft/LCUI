/* ***************************************************************************
 * fontlibrary.c -- The font info and font bitmap cache module.
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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
typedef struct LCUI_FontFamilyNode {
	char *family_name;	/**< 字族名称  */
	LinkedList styles;	/**< 该字族下的各种样式的字体信息 */
} LCUI_FontFamilyNode;

/** 字体路径索引结点 */
typedef struct LCUI_FontPathNode {
	char *path;		/**< 路径  */
	LCUI_Font *font;	/**< 被索引的字体信息 */
} LCUI_FontPathNode;

static struct LCUI_FontLibraryContext {
	int count;				/**< 计数器，主要用于为字体信息生成标识号 */
	int font_cache_num;			/**< 字体信息缓存区的数量 */
	LCUI_BOOL is_inited;			/**< 标记，指示数据库是否初始化 */
	LCUI_RBTree path_map;			/**< 字体路径映射表，用于实现按路径获取字体信息 */
	LCUI_RBTree family_tree;		/**< 字族信息树，按字族名称记录着各个字体的信息 */
	LCUI_RBTree bitmap_cache;		/**< 字体位图缓存区 */
	LCUI_Font ***font_cache;		/**< 字体信息缓存区 */
	LCUI_Font *default_font;		/**< 指针，引用的是默认字体信息 */
	LCUI_Font *incore_font;			/**< 指针，引用的是内置字体的信息 */
	LCUI_FontEngine engines[2];		/**< 当前可用字体引擎列表 */
	LCUI_FontEngine *engine;		/**< 当前选择的字体引擎 */
} fontlib = {0, FALSE};

/** 检测位图数据是否有效 */
#define FontBitmap_IsValid(fbmp) (fbmp && fbmp->width>0 && fbmp->rows>0)
#define SelectChar(ch) (LCUI_RBTree*)RBTree_GetData( &fontlib.bitmap_cache, ch )
#define SelectFont(ch, font_id) (LCUI_RBTree*)RBTree_GetData( ch, font_id )
#define SelectBitmap(font, size) (LCUI_FontBitmap*)RBTree_GetData( font, size )
#define SelectFontFamliy(family_name) (LCUI_FontFamilyNode*)\
	RBTree_CustomGetData( &fontlib.family_tree, family_name );
#define SelectFontCache(id) \
	fontlib.font_cache[fontlib.font_cache_num-1][id % FONT_CACHE_SIZE]

static int OnComparePath( void *data, const void *keydata )
{
	return strcmp(((LCUI_FontPathNode*)data)->path, keydata);
}

static int OnCompareFamily( void *data, const void *keydata )
{
	return strcmp(((LCUI_FontFamilyNode*)data)->family_name, keydata);
}

static void DestroyFontPathNode( void *arg )
{
	LCUI_FontPathNode *node = (LCUI_FontPathNode*)arg;
	if( node->path ) {
		free( node->path );
	}
	node->path = NULL;
	node->font = NULL;
}

static void DestroyFontFamilyNode( void *arg )
{
	LCUI_FontFamilyNode *node = (LCUI_FontFamilyNode*)arg;
	if( node->family_name ) {
		free( node->family_name );
	}
	node->family_name = NULL;
	LinkedList_Clear( &node->styles, NULL );
}

static void DestroyFontBitmap( void *arg )
{
	FontBitmap_Free( (LCUI_FontBitmap*)arg );
}

static void DestroyTreeNode( void *arg )
{
	RBTree_Destroy( (LCUI_RBTree*)arg );
}

/**
 * 添加字体信息记录，并返回该字体的ID
 * @warning 该函数仅仅是将 font 参数的引用添加至记录中，为保证记录的有效性，
 * 传入的 font 参数必须是动态分配的。
 */
int LCUIFont_Add( LCUI_Font *font, const char *filepath )
{
	LCUI_Font *f;
	LinkedListNode *node;
	LCUI_FontFamilyNode *fn;

	font->id = ++fontlib.count;
	if( font->id >= fontlib.font_cache_num * FONT_CACHE_SIZE ) {
		LCUI_Font ***caches, **cache;
		fontlib.font_cache_num += 1;
		caches = (LCUI_Font***)realloc( fontlib.font_cache,
			fontlib.font_cache_num * sizeof(LCUI_Font**) );
		if( caches == NULL ) {
			fontlib.font_cache_num -= 1;
			return -1;
		}
		cache = NEW(LCUI_Font*, FONT_CACHE_SIZE);
		if( cache == NULL ) {
			return -2;
		}
		caches[fontlib.font_cache_num-1] = cache;
		fontlib.font_cache = caches;
	}
	SelectFontCache(font->id) = font;
	fn = SelectFontFamliy(font->family_name);
	if( !fn ) {
		fn = NEW(LCUI_FontFamilyNode, 1);
		fn->family_name = strdup(font->family_name);
		LinkedList_Init( &fn->styles );
		RBTree_CustomInsert( &fontlib.family_tree,
				     font->family_name, fn );
	}
	LinkedList_ForEach( node, &fn->styles ) {
		f = (LCUI_Font*)node->data;
		if( strcmp( f->style_name, font->style_name ) == 0 ) {
			return -3;
		}
	}
	LinkedList_Append( &fn->styles, font );
	if( filepath ) {
		LCUI_FontPathNode *fp;
		if( LCUIFont_GetIdByPath( filepath ) >= 0 ) {
			return -4;
		}
		fp = NEW(LCUI_FontPathNode, 1);
		fp->path = strdup( filepath );
		fp->font = font;
		RBTree_CustomInsert( &fontlib.path_map, filepath, fp );
	}
	return font->id;
}

/** 获取指定字体ID的字体信息 */
static LCUI_Font* LCUIFont_GetById( int id )
{
	if( !fontlib.is_inited ) {
		return NULL;
	}
	if( id < 0 || id >= fontlib.font_cache_num * FONT_CACHE_SIZE ) {
		return NULL;
	}

	return SelectFontCache(id);
}

/** 根据字体文件路径，获取字体的ID */
int LCUIFont_GetIdByPath( const char *filepath )
{
	LCUI_Font *font;
	if( !fontlib.is_inited ) {
		return -1;
	}
	font = RBTree_CustomGetData( &fontlib.path_map, filepath );
	if( !font ) {
		return -2;
	}
	return font->id;
}

/**
 * 获取字体的ID
 * @param[in] family_name 字族名称
 * @param[in] style_name 样式名称，若设为 NULL，则默认获取 regular 样式或
 * 最后一个样式的字体
 */
int LCUIFont_GetId( const char *family_name, const char *style_name )
{
	LCUI_Font *font = NULL;
	LinkedListNode *node;
	LCUI_FontFamilyNode *fnode;

	if( !fontlib.is_inited ) {
		return -1;
	}
	fnode = SelectFontFamliy( family_name );
	if( !fnode ) {
		return -2;
	}
	LinkedList_ForEach( node, &fnode->styles ) {
		font = (LCUI_Font*)node->data;
		if( style_name ) {
			if( strcasecmp( font->style_name, style_name ) ) {
				continue;
			}
		} else {
			if( strcasecmp( font->style_name, "regular" ) ) {
				continue;
			}
		}
		return font->id;
	}
	if( !style_name && font ) {
		return font->id;
	}
	return -3;
}

/** 获取默认的字体ID */
int LCUIFont_GetDefault( void )
{
	if( !fontlib.default_font ) {
		return -1;
	}
	return fontlib.default_font->id;
}

/** 设定默认的字体 */
void LCUIFont_SetDefault( int id )
{
	LCUI_Font *p;
	p = LCUIFont_GetById( id );
	if( p ) {
		fontlib.default_font = p;
		printf("[font] select: %s\n", p->family_name);
	}
}

/**
 * 向字体缓存中添加字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 要添加的字体位图
 * @warning 此函数仅仅是将 bmp 复制进缓存中，并未重新分配新的空间储存位图数
 * 据，因此，请勿在调用此函数后手动释放 bmp。
 */
LCUI_FontBitmap* LCUIFont_AddBitmap( wchar_t ch, int font_id,
				     int size, const LCUI_FontBitmap *bmp )
{
	LCUI_FontBitmap *bmp_cache;
	LCUI_RBTree *tree_font, *tree_bmp;

	if( !fontlib.is_inited ) {
		return NULL;
	}
	/* 获取字符的字体信息集 */
	tree_font = SelectChar( ch );
	if( !tree_font ) {
		tree_font = NEW(LCUI_RBTree, 1);
		if( !tree_font ) {
			return NULL;
		}
		RBTree_Init( tree_font );
		RBTree_SetDataNeedFree( tree_font, TRUE );
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
		tree_bmp = NEW(LCUI_RBTree, 1);
		if( !tree_bmp ) {
			return NULL;
		}
		RBTree_Init( tree_bmp );
		RBTree_OnDestroy( tree_bmp, DestroyFontBitmap );
		RBTree_SetDataNeedFree( tree_bmp, TRUE );
		RBTree_Insert( tree_font, font_id, tree_bmp );
	}
	/* 在字体位图库中获取指定像素大小的字体位图 */
	bmp_cache = SelectBitmap( tree_bmp, size );
	if( !bmp_cache ) {
		bmp_cache = NEW(LCUI_FontBitmap, 1);
		if( !bmp_cache ) {
			return NULL;
		}
		RBTree_Insert( tree_bmp, size, bmp_cache );
	}
	/* 拷贝数据至该空间内 */
	memcpy( bmp_cache, bmp, sizeof(LCUI_FontBitmap) );
	return bmp_cache;
}

/**
 * 从缓存中获取字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 输出的字体位图的引用
 * @warning 请勿释放 bmp，bmp 仅仅是引用缓存中的字体位图，并未建分配新
 * 空间存储字体位图的拷贝。
 */
int LCUIFont_GetBitmap( wchar_t ch, int font_id, int size,
			const LCUI_FontBitmap **bmp )
{
	int ret;
	LCUI_RBTree *ctx;
	LCUI_FontBitmap bmp_cache;

	/* 这里的 while 只是为了减少缩进 */
	while( TRUE ) {
		if( !fontlib.is_inited ) {
			*bmp = NULL;
			return -2;
		}
		if( !(ctx = SelectChar(ch)) ) {
			break;
		}
		if( font_id <= 0 ) {
			if( !fontlib.default_font ) {
				return -3;
			}
			font_id = fontlib.default_font->id;
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
	}
	FontBitmap_Init( &bmp_cache );
	ret = FontBitmap_Load( &bmp_cache, ch, font_id, size );
	*bmp = LCUIFont_AddBitmap( ch, font_id, size, &bmp_cache );
	if( *bmp && ret == 0 ) {
		return 0;
	}
	return -1;
}

/** 载入字体值数据库中 */
int LCUIFont_LoadFile( const char *filepath )
{
	int ret, id;
	LCUI_Font *font;

	printf( "[font] load file: %s\n", filepath );
	/* 如果有同一文件路径的字族信息 */
	id = LCUIFont_GetIdByPath( filepath );
	if( id >= 0 ) {
		return id;
	}
	if( !fontlib.engine ) {
		return -1;
	}
	ret = fontlib.engine->open( filepath, &font );
	if( ret != 0 ) {
		return -2;
	}
	font->engine = fontlib.engine;
	id = LCUIFont_Add( font, filepath );
	printf("[font] add family: %s, style name: %s, id: %d\n",
		font->family_name, font->family_name, id);
	return id;
}

/** 打印字体位图的信息 */
void FontBitmap_PrintInfo( LCUI_FontBitmap *bitmap )
{
	printf("address:%p\n",bitmap);
	if( !bitmap ) {
		return;
	}
	printf("top: %d, left: %d, width:%d, rows:%d\n",
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
	if( FontBitmap_IsValid(bitmap) ) {
		free( bitmap->buffer );
		FontBitmap_Init( bitmap );
	}
}

/** 创建字体位图 */
int FontBitmap_Create( LCUI_FontBitmap *bitmap, int width, int rows )
{
	size_t size;
	if(width < 0 || rows < 0) {
		FontBitmap_Free(bitmap);
		return -1;
	}
	if(FontBitmap_IsValid(bitmap)) {
		FontBitmap_Free(bitmap);
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

/** 在屏幕打印以0和1表示字体位图 */
int FontBitmap_Print( LCUI_FontBitmap *fontbmp )
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

static void FontBitmap_MixARGB( LCUI_Graph *graph, LCUI_Rect *write_rect,
				const LCUI_FontBitmap *bmp, LCUI_Color color,
				LCUI_Rect *read_rect )
{
	int x, y;
	LCUI_ARGB *px, *px_row_des;
	uchar_t *byte_ptr, *byte_row_ptr;
	double a, out_a, out_r, out_g, out_b, src_a;
	byte_row_ptr = bmp->buffer + read_rect->y*bmp->width;
	byte_row_ptr += read_rect->x;
	px_row_des = graph->argb + write_rect->y * graph->w;
	px_row_des += write_rect->x;
	for( y = 0; y < read_rect->h; ++y ) {
		px = px_row_des;
		byte_ptr = byte_row_ptr;
		for( x = 0; x < read_rect->w; ++x, ++byte_ptr, ++px ) {
			src_a = *byte_ptr / 255.0;
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
		px_row_des += graph->w;
		byte_row_ptr += bmp->width;
	}
}

static void FontBitmap_MixRGB( LCUI_Graph *graph, LCUI_Rect *write_rect,
			       const LCUI_FontBitmap *bmp, LCUI_Color color,
			       LCUI_Rect *read_rect )
{
	int x, y;
	uchar_t *byte_src, *byte_row_src, *byte_row_des, *byte_des;
	byte_row_src = bmp->buffer + read_rect->y*bmp->width + read_rect->x;
	byte_row_des = graph->bytes + write_rect->y * graph->bytes_per_row;
	byte_row_des += write_rect->x*graph->bytes_per_pixel;
	for( y=0; y<read_rect->h; ++y ) {
		byte_src = byte_row_src;
		byte_des = byte_row_des;
		for( x=0; x<read_rect->w; ++x ) {
			ALPHA_BLEND( *byte_des, color.b, *byte_src );
			byte_des++;
			ALPHA_BLEND( *byte_des, color.g, *byte_src );
			byte_des++;
			ALPHA_BLEND( *byte_des, color.r, *byte_src );
			byte_des++;
			++byte_src;
		}
		byte_row_des += graph->bytes_per_row;
		byte_row_src += bmp->width;
	}
}

/** 将字体位图绘制到目标图像上 */
int FontBitmap_Mix( LCUI_Graph *graph, LCUI_Pos pos,
		    const LCUI_FontBitmap *bmp, LCUI_Color color )
{
	LCUI_Graph write_slot;
	LCUI_Rect r_rect, w_rect;
	if( pos.x > graph->w || pos.y > graph->h ) {
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

/** 载入字体位图 */
int FontBitmap_Load( LCUI_FontBitmap *buff, wchar_t ch,
		     int font_id, int pixel_size )
{
	LCUI_Font *info = fontlib.default_font;
	while( 1 ) {
		if( font_id < 0 || !fontlib.engine ) {
			break;
		}
		info = LCUIFont_GetById( font_id );
		if( info ) {
			break;
		}
		if( fontlib.default_font ) {
			info = fontlib.default_font;
		} else {
			info = fontlib.incore_font;
		}
	}
	if( !info ) {
		return -1;
	}
	return info->engine->render( buff, ch, pixel_size, info );
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

	fontlib.font_cache_num = 1;
	fontlib.font_cache = NEW(LCUI_Font**, 1);
	fontlib.font_cache[0] = NEW(LCUI_Font*, FONT_CACHE_SIZE);
	RBTree_Init( &fontlib.bitmap_cache );
	RBTree_Init( &fontlib.path_map );
	RBTree_Init( &fontlib.family_tree );
	RBTree_OnJudge( &fontlib.path_map, OnComparePath );
	RBTree_OnJudge( &fontlib.family_tree, OnCompareFamily );
	RBTree_OnDestroy( &fontlib.path_map, DestroyFontPathNode );
	RBTree_OnDestroy( &fontlib.family_tree, DestroyFontFamilyNode );
	RBTree_OnDestroy( &fontlib.bitmap_cache, DestroyTreeNode );
	RBTree_SetDataNeedFree( &fontlib.bitmap_cache, TRUE );
	fontlib.is_inited = TRUE;

	/* 先初始化内置的字体引擎 */
	LCUIFont_InitInCoreFont( &fontlib.engines[0] );
	font_id = LCUIFont_GetId( "inconsolata", NULL );
	fontlib.incore_font = LCUIFont_GetById( font_id );
	fontlib.engine = &fontlib.engines[0];
	/* 然后看情况启用其它字体引擎 */
#ifdef LCUI_FONT_ENGINE_FREETYPE
	if( LCUIFont_InitFreeType( &fontlib.engines[1] ) == 0 ) {
		fontlib.engine = &fontlib.engines[1];
	}
#endif
	if( fontlib.engine && fontlib.engine != &fontlib.engines[0] ) {
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
		/* 如果载入成功，则设定该字体为默认字体 */
		font_id = LCUIFont_LoadFile( target_font );
		if( font_id <= 0 ) {
			continue;
		}
		LCUIFont_SetDefault( font_id );
		break;
	} while( ++i, target_font = font_files[i], i<MAX_FONTFILE_NUM );
}

/** 停用字体处理模块 */
void LCUI_ExitFont( void )
{
	int i;
	LCUI_Font *font;

	if( !fontlib.is_inited ) {
		return;
	}
	fontlib.is_inited = FALSE;
	RBTree_Destroy( &fontlib.path_map );
	RBTree_Destroy( &fontlib.bitmap_cache );
	while( fontlib.font_cache_num > 0 ) {
		--fontlib.font_cache_num;
		for( i=0; i<FONT_CACHE_SIZE; ++i ) {
			font = fontlib.font_cache[fontlib.font_cache_num][i];
			if( !font ) {
				continue;
			}
			free( font->family_name );
			free( font->style_name );
			if( font->data ) {
				free( font->data );
			}
			font->data = NULL;
			font->engine = NULL;
		}
		free( fontlib.font_cache[fontlib.font_cache_num] );
	}
	free( fontlib.font_cache );
	fontlib.font_cache = NULL;
}
