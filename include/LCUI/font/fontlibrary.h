/* ***************************************************************************
 * fontlibrary.h -- The font info and font bitmap cache module.
 *
 * Copyright (C) 2012-2016by Liu Chao <lc-soft@live.cn>
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
 * fontlibrary.h -- 字体信息和字体位图缓存模块。
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

#ifndef LCUI_FONT_LIBRARY_H
#define LCUI_FONT_LIBRARY_H

LCUI_BEGIN_HEADER

/** 字体位图数据 */
typedef struct LCUI_FontBitmap_ {
	int top;		/**< 与顶边框的距离 */
	int left;		/**< 与左边框的距离 */
	int width;		/**< 位图宽度 */
	int rows;		/**< 位图行数 */
	int pitch;
	uchar_t *buffer;	/**< 字体位图数据 */
	short num_grays;
	char pixel_mode;
	LCUI_Pos advance;	/**< XY轴的跨距 */
} LCUI_FontBitmap;

typedef struct LCUI_FontEngine	LCUI_FontEngine;

typedef struct LCUI_Font {
	int id;                         /**< 字体信息ID */
	char *style_name;		/**< 样式名称 */
	char *family_name;		/**< 字族名称 */
	void *data;			/**< 相关数据 */
	LCUI_FontEngine *engine;	/**< 所属的字体引擎 */
} LCUI_Font;

struct LCUI_FontEngine {
	char name[64];
	int (*open)(const char*, LCUI_Font***);
	int (*render)(LCUI_FontBitmap*, wchar_t, int, LCUI_Font*);
	void (*close)(void*);
};


int LCUIFont_InitInCoreFont( LCUI_FontEngine *engine );

int LCUIFont_ExitInCoreFont( void );

#ifdef LCUI_FONT_ENGINE_FREETYPE

int LCUIFont_InitFreeType( LCUI_FontEngine *engine );

int LCUIFont_ExitFreeType( void );

#endif

/** 获取内置的 Inconsolata 字体位图 */
LCUI_API int FontInconsolata_GetBitmap( LCUI_FontBitmap *bmp, wchar_t ch, int size );

/** 打印字体位图的信息 */
LCUI_API void FontBitmap_PrintInfo( LCUI_FontBitmap *bitmap );

/** 初始化字体位图 */
LCUI_API void FontBitmap_Init( LCUI_FontBitmap *bitmap );

/** 释放字体位图占用的资源 */
LCUI_API void FontBitmap_Free( LCUI_FontBitmap *bitmap );

/** 创建字体位图 */
LCUI_API int FontBitmap_Create( LCUI_FontBitmap *bitmap, int width, int rows );

/** 在屏幕打印以0和1表示字体位图 */
LCUI_API int FontBitmap_Print( LCUI_FontBitmap *fontbmp );

/** 将字体位图绘制到目标图像上 */
LCUI_API int FontBitmap_Mix( LCUI_Graph *graph, LCUI_Pos pos,
			     const LCUI_FontBitmap *bmp, LCUI_Color color );

/** 载入字体位图 */
LCUI_API int FontBitmap_Load( LCUI_FontBitmap *buff, wchar_t ch,
			   int font_id, int pixel_size );

/** 初始化字体数据库 */
LCUI_API void FontLIB_Init( void );

/** 添加字体族，并返回该字族的ID */
LCUI_API int LCUIFont_Add( LCUI_Font *font );

/**
 * 获取字体的ID
 * @param[in] family_name 字族名称
 * @param[in] style_name 样式名称，若设为 NULL，则默认获取 regular 样式或
 * 最后一个样式的字体
 */
LCUI_API int LCUIFont_GetId( const char *family_name, const char *style_name );

/** 获取默认的字体ID */
LCUI_API int LCUIFont_GetDefault( void );

/** 设定默认的字体 */
LCUI_API void LCUIFont_SetDefault( int id );

/**
 * 向字体缓存中添加字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 要添加的字体位图
 * @warning 此函数仅仅是将 bmp 复制进缓存中，并未重新分配新的空间储存位图数
 * 据，因此，请勿在调用此函数后手动释放 bmp。
 */
LCUI_API LCUI_FontBitmap* LCUIFont_AddBitmap( wchar_t ch, int font_id,
				int size, const LCUI_FontBitmap *bmp );

/**
 * 从缓存中获取字体位图
 * @param[in] ch 字符码
 * @param[in] font_id 使用的字体ID
 * @param[in] size 字体大小（单位为像素）
 * @param[out] bmp 输出的字体位图的引用
 * @warning 请勿释放 bmp，bmp 仅仅是引用缓存中的字体位图，并未建分配新
 * 空间存储字体位图的拷贝。
 */
LCUI_API int LCUIFont_GetBitmap( wchar_t ch, int font_id, int size,
				 const LCUI_FontBitmap **bmp );

/** 载入字体至数据库中 */
LCUI_API int LCUIFont_LoadFile( const char *filepath );

/** 初始化字体处理模块 */
void LCUI_InitFont( void );

/** 停用字体处理模块 */
void LCUI_ExitFont( void );

LCUI_END_HEADER

#endif
