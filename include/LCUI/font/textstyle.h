/*
 * textstyle.h -- Text style processing module.
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

#ifndef LCUI_TEXTSTYLE_H
#define LCUI_TEXTSTYLE_H

LCUI_BEGIN_HEADER

typedef struct LCUI_TextStyleRec_ {
	LCUI_BOOL has_family:1;
	LCUI_BOOL has_style:1;
	LCUI_BOOL has_weight:1;
	LCUI_BOOL has_back_color:1;
	LCUI_BOOL has_fore_color:1;
	LCUI_BOOL has_pixel_size:1;

	int style;
	int weight;
	int *font_ids;

	LCUI_Color fore_color;
	LCUI_Color back_color;

	int pixel_size;
} LCUI_TextStyleRec, *LCUI_TextStyle;

/** 初始化字体样式数据 */
LCUI_API void TextStyle_Init( LCUI_TextStyle data );

LCUI_API int TextStyle_CopyFamily( LCUI_TextStyle dst, LCUI_TextStyle src );

LCUI_API int TextStyle_Copy( LCUI_TextStyle dst, LCUI_TextStyle src );

LCUI_API void TextStyle_Destroy( LCUI_TextStyle data );

LCUI_API void TextStyle_Merge( LCUI_TextStyle base, LCUI_TextStyle target );

/* 设置字体粗细程度 */
LCUI_API int TextStyle_SetWeight( LCUI_TextStyle ts, LCUI_FontWeight weight );

LCUI_API int TextStyle_SetStyle( LCUI_TextStyle ts, LCUI_FontStyle style );

/**
 * 设置字体
 * @param[in][out] ts 字体样式数据
 * @param[in] str 字体名称，如果有多个名称则用逗号分隔
 */
LCUI_API int TextStyle_SetFont( LCUI_TextStyle ts, const char *str );

/** 设置使用默认的字体 */
int TextStyle_SetDefaultFont( LCUI_TextStyle ts );

/*-------------------------- StyleTag --------------------------------*/

#define StyleTags_Init LinkedList_Init

/** 从字符串中获取样式标签的名字及样式属性 */
LCUI_API const wchar_t *ScanStyleTag( const wchar_t *wstr, wchar_t *name,
				      int max_name_len, wchar_t *data );

/** 在字符串中获取样式的结束标签，输出的是标签名 */
LCUI_API const wchar_t *ScanStyleEndingTag( const wchar_t *wstr, wchar_t *name );

LCUI_API void StyleTags_Clear( LinkedList *tags );

LCUI_API LCUI_TextStyle StyleTags_GetTextStyle( LinkedList *tags );

/** 处理样式标签 */
LCUI_API const wchar_t* StyleTags_GetStart( LinkedList *tags,
						const wchar_t *str );

/** 处理样式结束标签 */
LCUI_API const wchar_t* StyleTags_GetEnd( LinkedList *tags,
						 const wchar_t *str );

/*------------------------- End StyleTag -----------------------------*/

LCUI_END_HEADER

#endif
