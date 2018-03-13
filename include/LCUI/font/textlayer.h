/*
 * textlayer.h -- Text layout and rendering module.
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

#ifndef LCUI_TEXTLAYER_H
#define LCUI_TEXTLAYER_H

LCUI_BEGIN_HEADER

typedef struct LCUI_TextCharRec_ {
        wchar_t code;			/**< 字符码 */
        LCUI_TextStyle style;		/**< 该字符使用的样式数据 */
	const LCUI_FontBitmap *bitmap;	/**< 字体位图数据(只读) */
} LCUI_TextCharRec, *LCUI_TextChar;

/** End Of Line character */
typedef enum LCUI_EOLChar {
	LCUI_EOL_NONE,		/**< 无换行 */
	LCUI_EOL_CR,		/**< Mac OS 格式换行，CF = Carriage-Return，字符：\r */
	LCUI_EOL_LF,		/**< UNIX/Linux 格式换行，LF = Line-Feed，字符：\r */
	LCUI_EOL_CR_LF		/**< Windows 格式换行： \r\n */
} LCUI_EOLChar;

/* 文本行 */
typedef struct TextRowRec_ {
        int width;			/**< 宽度 */
        int height;			/**< 高度 */
	int text_height;		/**< 当前行中最大字体的高度 */
        int length;			/**< 该行文本长度 */
        LCUI_TextChar *string;		/**< 该行文本的数据 */
	LCUI_EOLChar eol;		/**< 行尾结束类型 */
} LCUI_TextRowRec, *LCUI_TextRow;

/* 文本行列表 */
typedef struct LCUI_TextRowListRec_ {
        int length;			/**< 当前总行数 */
        LCUI_TextRow *rows;		/**< 每一行文本的数据 */
} LCUI_TextRowListRec, *LCUI_TextRowList;

/**
 * word-break mode
 * The word-break mode specifies whether or not the textlayer should
 * insert line breaks wherever the text would otherwise overflow its
 * content box.
 */
typedef enum LCUI_WordBreakMode {
	LCUI_WORD_BREAK_NORMAL,		/**< 默认的断行规则，将宽度溢出的单词放到下一行 */
	LCUI_WORD_BREAK_BREAK_ALL	/**< 任意字符间断行 */
} LCUI_WordBreakMode;

typedef struct LCUI_TextLayerRec_  {
        int offset_x;			/**< X轴坐标偏移量 */
        int offset_y;			/**< Y轴坐标偏移量 */
	int new_offset_x;		/**< 新的X轴坐标偏移量 */
	int new_offset_y;		/**< 新的Y轴坐标偏移量 */
        int insert_x;			/**< 光标所在列数 */
        int insert_y;			/**< 光标所在行数 */
	int width;			/**< 实际文本宽度 */

	/**
	 * 固定宽高
	 * 当它们等于0时，文本宽高会根据文本内容自动适应
	 * 当它们大于0时，会直接根据该值处理文本对齐
	 */
	int fixed_width, fixed_height;
	/**
	 * 最大文本宽高
	 * 当未设置固定宽度时，文字排版将按最大宽度进行
	 */
	int max_width, max_height;

	int length;				/**< 文本长度 */
	int line_height;			/**< 全局文本行高度 */
	int text_align;				/**< 文本的对齐方式 */
	LCUI_WordBreakMode word_break;		/**< 单词内断行模式 */
	LCUI_BOOL enable_mulitiline;		/**< 是否启用多行文本模式 */
        LCUI_BOOL enable_autowrap;		/**< 是否启用自动换行模式 */
	LCUI_BOOL enable_style_tags;		/**< 是否使用文本样式标签 */
        LCUI_BOOL enable_canvas;		/**< 是否使用缓存空间来存储文本位图 */
	LinkedList dirty_rects;			/**< 脏矩形记录 */
	LinkedList text_styles;			/**< 样式缓存 */
	LCUI_TextStyleRec text_default_style;	/**< 文本全局样式 */
        LCUI_TextRowListRec text_rows;		/**< 文本行列表 */
	struct {
		LCUI_BOOL update_bitmap;	/**< 更新文本的字体位图 */
		LCUI_BOOL update_typeset;	/**< 重新对文本进行排版 */
		int typeset_start_row;		/**< 排版处理的起始行 */	
		LCUI_BOOL redraw_all;		/**< 重绘所有字体位图 */
	} task;					/**< 待处理的任务 */
        LCUI_Graph canvas;			/**< 文本位图缓存 */
} LCUI_TextLayerRec, *LCUI_TextLayer;

/** 获取文本行总数 */
LCUI_API int TextLayer_GetRowTotal( LCUI_TextLayer layer );

/** 获取指定文本行的高度 */
LCUI_API int TextLayer_GetRowHeight( LCUI_TextLayer layer, int row );

/** 获取指定文本行的文本长度 */
LCUI_API int TextLayer_GetRowTextLength( LCUI_TextLayer layer, int row );

/** 添加 更新文本排版 的任务 */
LCUI_API void TextLayer_AddUpdateTypeset( LCUI_TextLayer layer, int start_row );

/** 设置文本对齐方式 */
LCUI_API void TextLayer_SetTextAlign( LCUI_TextLayer layer, int align );

/** 设置坐标偏移量 */
LCUI_API void TextLayer_SetOffset( LCUI_TextLayer layer, int offset_x, int offset_y );

LCUI_API LCUI_TextLayer TextLayer_New(void);

/** 销毁TextLayer */
LCUI_API void TextLayer_Destroy( LCUI_TextLayer layer );

/** 标记指定范围内容的文本行的矩形为无效 */
LCUI_API void TextLayer_InvalidateRowsRect( LCUI_TextLayer layer, 
					    int start_row, int end_row );

/** 设置插入点的行列坐标 */
LCUI_API void TextLayer_SetCaretPos( LCUI_TextLayer layer, int row, int col );

/** 根据像素坐标设置文本光标的行列坐标 */
LCUI_API int TextLayer_SetCaretPosByPixelPos( LCUI_TextLayer layer, int x, int y );

/** 获取指定行列的文字的像素坐标 */
LCUI_API int TextLayer_GetCharPixelPos( LCUI_TextLayer layer, int row,
					int col, LCUI_Pos *pixel_pos );

/** 获取文本光标的像素坐标 */
LCUI_API int TextLayer_GetCaretPixelPos( LCUI_TextLayer layer, LCUI_Pos *pixel_pos );

/** 清空文本 */
LCUI_API void TextLayer_ClearText( LCUI_TextLayer layer );

/** 插入文本内容（宽字符版） */
LCUI_API int TextLayer_InsertTextW( LCUI_TextLayer layer, const wchar_t *wstr,
				    LinkedList *tag_stack );

/** 插入文本内容 */
LCUI_API int TextLayer_InsertTextA( LCUI_TextLayer layer, const char *str );

/** 插入文本内容（UTF-8版） */
LCUI_API int TextLayer_InsertText( LCUI_TextLayer layer, const char *utf8_str );

/** 追加文本内容（宽字符版） */
LCUI_API int TextLayer_AppendTextW( LCUI_TextLayer layer, const wchar_t *wstr, 
				    LinkedList *tag_stack );

/** 追加文本内容 */
LCUI_API int TextLayer_AppendTextA( LCUI_TextLayer layer, const char *ascii_text );

/** 追加文本内容（UTF-8版） */
LCUI_API int TextLayer_AppendText( LCUI_TextLayer layer, const char *utf8_text );

/** 设置文本内容（宽字符版） */
LCUI_API int TextLayer_SetTextW( LCUI_TextLayer layer, const wchar_t *wstr,
				 LinkedList *tag_stack );

/** 设置文本内容 */
LCUI_API int TextLayer_SetTextA( LCUI_TextLayer layer, const char *ascii_text );

/** 设置文本内容（UTF-8版） */
LCUI_API int TextLayer_SetText( LCUI_TextLayer layer, const char *utf8_text );

/** 获取文本图层中的文本（宽字符版） */
LCUI_API int TextLayer_GetTextW( LCUI_TextLayer layer, int start_pos,
				 int max_len, wchar_t *wstr_buff );

/** 获取文本位图缓存 */
LCUI_API LCUI_Graph* TextLayer_GetGraphBuffer( LCUI_TextLayer layer );

/** 计算并获取文本的宽度 */
LCUI_API int TextLayer_GetWidth( LCUI_TextLayer layer );

/** 计算并获取文本的高度 */
LCUI_API int TextLayer_GetHeight( LCUI_TextLayer layer );

/** 设置固定尺寸 */
LCUI_API int TextLayer_SetFixedSize( LCUI_TextLayer layer, int width, int height );

/** 设置最大尺寸 */
LCUI_API int TextLayer_SetMaxSize( LCUI_TextLayer layer, int width, int height );

/** 设置是否启用多行文本模式 */
LCUI_API void TextLayer_SetMultiline( LCUI_TextLayer layer, int is_true );

/** 删除文本光标的当前坐标右边的文本 */
LCUI_API int TextLayer_TextDelete( LCUI_TextLayer layer, int n_char );

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
LCUI_API int TextLayer_TextBackspace( LCUI_TextLayer layer, int n_char );

/** 设置是否启用自动换行模式 */
LCUI_API void TextLayer_SetAutoWrap( LCUI_TextLayer layer, LCUI_BOOL autowrap );

/** 设置单词内断行模式 */
LCUI_API void TextLayer_SetWordBreak( LCUI_TextLayer layer, LCUI_WordBreakMode mode );

/** 设置是否使用样式标签 */
LCUI_API void TextLayer_SetUsingStyleTags( LCUI_TextLayer layer, LCUI_BOOL is_true );

/** 重新载入各个文字的字体位图 */
LCUI_API void TextLayer_ReloadCharBitmap( LCUI_TextLayer layer );

/** 更新数据 */
LCUI_API void TextLayer_Update( LCUI_TextLayer layer, LinkedList *rects );

/** 
 * 将文本图层中的指定区域的内容绘制至目标图像中
 * @param layer 要使用的文本图层
 * @param area 文本图层中需要绘制的区域
 * @param layer_pos 文本图层在目标图像中的位置
 * @param cavans 目标画布
 */
LCUI_API int TextLayer_RenderTo( LCUI_TextLayer layer, LCUI_Rect area,
				 LCUI_Pos layer_pos, LCUI_Graph *canvas );

/** 绘制文本 */
LCUI_API int TextLayer_RenderAll( LCUI_TextLayer layer );

/** 清除已记录的无效矩形 */
LCUI_API void TextLayer_ClearInvalidRect( LCUI_TextLayer layer );

/** 设置全局文本样式 */
LCUI_API void TextLayer_SetTextStyle( LCUI_TextLayer layer, LCUI_TextStyle style );

/** 设置文本行的高度 */
LCUI_API void TextLayer_SetLineHeight( LCUI_TextLayer layer, int height );

LCUI_END_HEADER

#endif
