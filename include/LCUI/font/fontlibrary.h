/*
 * fontlibrary.h -- The font info and font bitmap cache module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LCUI_FONT_LIBRARY_H
#define LCUI_FONT_LIBRARY_H

LCUI_BEGIN_HEADER

typedef enum LCUI_FontStyle {
	FONT_STYLE_NORMAL,
	FONT_STYLE_ITALIC,
	FONT_STYLE_OBLIQUE,
	FONT_STYLE_TOTAL_NUM
} LCUI_FontStyle;

typedef enum LCUI_FontWeight {
	FONT_WEIGHT_NONE = 0,
	FONT_WEIGHT_THIN = 100,
	FONT_WEIGHT_EXTRA_LIGHT = 200,
	FONT_WEIGHT_LIGHT = 300,
	FONT_WEIGHT_NORMAL = 400,
	FONT_WEIGHT_MEDIUM = 500,
	FONT_WEIGHT_SEMI_BOLD = 600,
	FONT_WEIGHT_BOLD = 700,
	FONT_WEIGHT_EXTRA_BOLD = 800,
	FONT_WEIGHT_BLACK = 900,
	FONT_WEIGHT_TOTAL_NUM = 9
} LCUI_FontWeight;

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

typedef struct LCUI_FontEngine LCUI_FontEngine;

typedef struct LCUI_FontRec_ {
	int id;                         /**< 字体信息ID */
	char *style_name;		/**< 样式名称 */
	char *family_name;		/**< 字族名称 */
	void *data;			/**< 相关数据 */
	LCUI_FontStyle style;		/**< 风格 */
	LCUI_FontWeight weight;		/**< 粗细程度 */
	LCUI_FontEngine *engine;	/**< 所属的字体引擎 */
} LCUI_FontRec, *LCUI_Font;

struct LCUI_FontEngine {
	char name[64];
	int (*open)(const char*, LCUI_Font**);
	int (*render)(LCUI_FontBitmap*, wchar_t, int, LCUI_Font);
	void (*close)(void*);
};

/**
 * 根据字符串内容猜测字体粗细程度
 * 文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-weight
 */
LCUI_API LCUI_FontWeight LCUIFont_DetectWeight( const char *str );

/**
 * 根据字符串内容猜测字体风格
 * 文档：https://developer.mozilla.org/en-US/docs/Web/CSS/font-style
 */
LCUI_API LCUI_FontStyle LCUIFont_DetectStyle( const char *str );

#ifndef _XTYPEDEF_FONT
LCUI_API LCUI_Font Font( const char *family_name, const char *style_name );
#endif

LCUI_API void DeleteFont( LCUI_Font font );

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
LCUI_API int LCUIFont_RenderBitmap( LCUI_FontBitmap *buff, wchar_t ch,
			   int font_id, int pixel_size );

/** 添加字体族，并返回该字族的ID */
LCUI_API int LCUIFont_Add( LCUI_Font font );

/**
 * 获取字体的ID
 * @param[in] family_name 字族名称
 * @param[in] style 字体风格
 * @param[in] weight 字体粗细程度，若为值 0，则默认为 FONT_WEIGHT_NORMAL
 */
LCUI_API int LCUIFont_GetId( const char *family_name,
			     LCUI_FontStyle style,
			     LCUI_FontWeight weight );

/**
 * 更新当前字体的粗细程度
 * @param[in] font_ids 当前的字体 id 列表
 * @params[in] weight 字体粗细程度
 * @params[out] new_font_ids 更新字体粗细程度后的字体 id 列表
 */
LCUI_API size_t LCUIFont_UpdateWeight( const int *font_ids,
				       LCUI_FontWeight weight,
				       int **new_font_ids );

/**
 * 更新当前字体的风格
 * @param[in] font_ids 当前的字体 id 列表
 * @params[in] style 字体风格
 * @params[out] new_font_ids 更新字体粗细程度后的字体 id 列表
 */
LCUI_API size_t LCUIFont_UpdateStyle( const int *font_ids,
				      LCUI_FontStyle style,
				      int **new_font_ids );

/**
 * 根据字族名称获取对应的字体 ID 列表
 * @param[out] ids 输出的字体 ID 列表
 * @param[in] style 字体风格
 * @param[in] weight 字体粗细程度，若为值 0，则默认为 FONT_WEIGHT_NORMAL
 * @param[in] names 字族名称，多个名字用逗号隔开
 * @return 获取到的字体 ID 的数量
 */
LCUI_API size_t LCUIFont_GetIdByNames( int **font_ids,
				       LCUI_FontStyle style,
				       LCUI_FontWeight weight,
				       const char *names );

/** 获取指定字体ID的字体信息 */
LCUI_API LCUI_Font LCUIFont_GetById( int id );

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
LCUI_API void LCUI_InitFontLibrary( void );

/** 停用字体处理模块 */
LCUI_API void LCUI_FreeFontLibrary( void );

LCUI_END_HEADER

#endif
