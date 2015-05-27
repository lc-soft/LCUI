/* ***************************************************************************
 * textlayer.h -- text bitmap layer processing module.
 * 
 * Copyright (C) 2012-2014 by
 * Liu Chao
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
 * textlayer.h -- 文本图层处理模块
 *
 * 版权所有 (C) 2012-2014 归属于
 * 刘超
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
#ifndef __LCUI_TEXTLAYER_H__
#define __LCUI_TEXTLAYER_H__

LCUI_BEGIN_HEADER

/* 文本对齐方式 */
typedef enum TextAlignType_ {
	TEXT_ALIGN_LEFT,	/* 向左对齐 */
	TEXT_ALIGN_CENTER,	/* 居中对齐 */
	TEXT_ALIGN_RIGHT	/* 向右对齐 */
} TextAlignType;

/* 文本添加类型 */
typedef enum TextAddType_ {
        TEXT_ADD_TYPE_INSERT,		/* 插入至插入点处 */
        TEXT_ADD_TYPE_APPEND		/* 追加至文本末尾 */
} TextAddType;

typedef struct TextCharDataRec_ {
        wchar_t char_code;		/* 字符码 */
        LCUI_TextStyle *style;		/* 该字符使用的样式数据 */
	LCUI_FontBitmap *bitmap;		/* 字体位图数据 */
} TextCharData;

/** 文本行结尾符 */
typedef enum EOLChar_ {
	EOL_NONE,	/**< 无换行 */
	EOL_CR,		/**< Mac OS 格式换行，CF = Carriage-Return，字符：\r */
	EOL_LF,		/**< UNIX/Linux 格式换行，LF = Line-Feed，字符：\r */
	EOL_CR_LF	/**< Windows 格式换行： \r\n */
} EOLChar;

/* 文本行 */
typedef struct TextRowDataRec_ {
        int max_width;			/* 文本行的最大像素宽度 */
        int max_height;			/* 文本行的最大像素高度 */
        int top_spacing;		/* 与上一行文本的间距 */
        int bottom_spacing;		/* 与下一行文本的间距 */
        int string_len;			/* 该行文本长度 */
        int string_max_len;		/* 该行文本最大长度 */
        TextCharData **string;		/* 该行文本的数据 */
	EOLChar eol;			/* 行尾结束类型 */
} TextRowData;

/* 文本行列表 */
typedef struct TextRowListRec_ {
        int rows;			/* 当前总行数 */
        int max_rows;			/* 最大行数 */
        TextRowData **rowdata;		/* 每一行文本的数据 */
} TextRowList;

/* 任务数据 */
typedef struct TaskDataRec_ {
        LCUI_BOOL update_bitmap;	/* 更新文本的字体位图 */
        LCUI_BOOL update_typeset;	/* 重新对文本进行排版 */
        int typeset_start_row;		/* 排版处理的起始行 */	
        int redraw_all;			/* 重绘所有字体位图 */
} TaskData;

typedef struct TextLayerRec_  {
        int offset_x;			/* X轴坐标偏移量 */
        int offset_y;			/* Y轴坐标偏移量 */
	int new_offset_x;		/* 新的X轴坐标偏移量 */
	int new_offset_y;		/* 新的Y轴坐标偏移量 */
        int insert_x;			/* 光标所在列数 */
        int insert_y;			/* 光标所在行数 */
        int max_width;			/* 最大文本宽度 */
        int max_height;			/* 最大文本高度 */
        
	LCUI_BOOL is_mulitiline_mode;	/* 是否启用多行文本模式 */
        LCUI_BOOL is_autowrap_mode;	/* 是否启用自动换行模式 */
	LCUI_BOOL is_using_style_tags;	/* 是否使用文本样式标签 */
        LCUI_BOOL is_using_buffer;	/* 是否使用缓存空间来存储文本位图 */
	LCUI_DirtyRectList dirty_rect;	/* 脏矩形记录 */

        TextAlignType text_align;	/* 文本的对齐方式 */
        TextRowList row_list;		/* 文本行列表 */
        LCUI_TextStyle text_style;	/* 文本全局样式 */
        TaskData task;			/* 任务 */
        LCUI_Graph graph;		/* 文本位图缓存 */
} LCUI_TextLayer;

/** 获取文本行总数 */
static inline int TextLayer_GetRowTotal( LCUI_TextLayer* layer )
{
	return layer->row_list.rows;
}

/** 获取指定文本行的高度 */
static inline int TextLayer_GetRowHeight( LCUI_TextLayer* layer, int row )
{
        if( row >= layer->row_list.rows ) {
                return 0;
        }
        return layer->row_list.rowdata[row]->max_height;
}

/** 获取指定文本行的文本长度 */
static inline int TextLayer_GetRowTextLength( LCUI_TextLayer* layer, int row )
{
        if( row >= layer->row_list.rows ) {
                return -1;
        }
        return layer->row_list.rowdata[row]->string_len;
}

/** 设置文本颜色 */
static inline void TextLayer_SetFontColor( LCUI_TextLayer* layer, LCUI_Color color )
{
	TextStyle_FontColor( &layer->text_style, color );
	layer->task.redraw_all = TRUE;
}

/** 设置文本所使用的字体字族 */
static inline void TextLayer_SetFontFamily( LCUI_TextLayer* layer, const char *family_name )
{
	TextStyle_FontFamily( &layer->text_style, family_name );
        layer->task.update_bitmap = TRUE;
}

/** 设置文本的字体像素大小 */
static inline void TextLayer_SetFontPixelSize( LCUI_TextLayer* layer, int pixel_size )
{
	TextStyle_FontSize( &layer->text_style, pixel_size );
        layer->task.update_bitmap = TRUE;
}

/** 设置文本对齐方式 */
static inline void TextLayer_SetTextAlign( LCUI_TextLayer* layer, TextAlignType align )
{
        layer->text_align = align;
        layer->task.redraw_all = TRUE;
}

/** 设置坐标偏移量 */
static inline void TextLayer_SetOffset( LCUI_TextLayer* layer, int offset_x, int offset_y )
{
        layer->new_offset_x = offset_x;
        layer->new_offset_y = offset_y;
}

LCUI_API void TextLayer_Init( LCUI_TextLayer *layer );

/** 销毁TextLayer */
LCUI_API void TextLayer_Destroy( LCUI_TextLayer *layer );

/** 设置插入点的行列坐标 */
LCUI_API void TextLayer_SetCaretPos( LCUI_TextLayer* layer, int row, int col );

/** 根据像素坐标设置文本光标的行列坐标 */
LCUI_API int TextLayer_SetCaretPosByPixelPos( LCUI_TextLayer* layer, int x, int y );

/** 获取指定行列的文字的像素坐标 */
LCUI_API int TextLayer_GetCharPixelPos( LCUI_TextLayer* layer, int row,
						int col, LCUI_Pos *pixel_pos );

/** 获取文本光标的像素坐标 */
LCUI_API int TextLayer_GetCaretPixelPos( LCUI_TextLayer *layer, LCUI_Pos *pixel_pos );

/** 清空文本 */
LCUI_API void TextLayer_ClearText( LCUI_TextLayer* layer );

/** 插入文本内容（宽字符版） */
LCUI_API int TextLayer_InsertTextW( LCUI_TextLayer *layer, const wchar_t *wstr,
						LCUI_StyleTagStack *tag_stack );

/** 插入文本内容 */
LCUI_API int TextLayer_InsertTextA( LCUI_TextLayer* layer, const char *ascii_text );

/** 插入文本内容（UTF-8版） */
LCUI_API int TextLayer_InsertText( LCUI_TextLayer* layer, const char *utf8_text );

/** 追加文本内容（宽字符版） */
LCUI_API int TextLayer_AppendTextW( LCUI_TextLayer *layer, const wchar_t *wstr, 
						LCUI_StyleTagStack *tag_stack );

/** 追加文本内容 */
LCUI_API int TextLayer_AppendTextA( LCUI_TextLayer* layer, const char *ascii_text );

/** 追加文本内容（UTF-8版） */
LCUI_API int TextLayer_AppendText( LCUI_TextLayer* layer, const char *utf8_text );

/** 设置文本内容（宽字符版） */
LCUI_API int TextLayer_SetTextW( LCUI_TextLayer *layer, const wchar_t *wstr,
						LCUI_StyleTagStack *tag_stack );

/** 设置文本内容 */
LCUI_API int TextLayer_SetTextA( LCUI_TextLayer* layer, const char *ascii_text );

/** 设置文本内容（UTF-8版） */
LCUI_API int TextLayer_SetText( LCUI_TextLayer* layer, const char *utf8_text );

/** 获取文本图层中的文本（宽字符版） */
LCUI_API int TextLayer_GetTextW( LCUI_TextLayer *layer, int start_pos,
					int max_len, wchar_t *wstr_buff );

/** 获取文本位图缓存 */
LCUI_API LCUI_Graph* TextLayer_GetGraphBuffer( LCUI_TextLayer *layer );

/** 设置最大尺寸 */
LCUI_API int TextLayer_SetMaxSize( LCUI_TextLayer *layer, LCUI_Size new_size );

/** 设置是否启用多行文本模式 */
LCUI_API void TextLayer_SetMultiline( LCUI_TextLayer* layer, int is_true );

/** 删除文本光标的当前坐标右边的文本 */
LCUI_API int TextLayer_Delete( LCUI_TextLayer *layer, int n_char );

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
LCUI_API int TextLayer_Backspace( LCUI_TextLayer* layer, int n_char );

/** 设置是否启用自动换行模式 */
LCUI_API void TextLayer_SetAutoWrap( LCUI_TextLayer* layer, int is_true );

/** 设置是否使用样式标签 */
LCUI_API void TextLayer_SetUsingStyleTags( LCUI_TextLayer *layer, LCUI_BOOL is_true );

/** 计算并获取文本的宽度 */
LCUI_API int TextLayer_GetWidth( LCUI_TextLayer* layer );

/** 计算并获取文本的高度 */
LCUI_API int TextLayer_GetHeight( LCUI_TextLayer* layer );

/** 重新载入各个文字的字体位图 */
LCUI_API void TextLayer_ReloadCharBitmap( LCUI_TextLayer* layer );

/** 更新数据 */
LCUI_API void TextLayer_Update( LCUI_TextLayer* layer, LinkedList *rect_list );

/** 
 * 将文本图层中的指定区域的内容绘制至目标图像缓存中
 * @param layer 要使用的文本图层
 * @param area 文本图层中需要绘制的区域
 * @param pos 文本图层在目标图像中的位置
 * @param need_replace 绘制时是否需要覆盖像素
 */
LCUI_API int TextLayer_DrawToGraph( LCUI_TextLayer *layer, LCUI_Rect area,
				    LCUI_Pos pos, LCUI_Graph *graph );

/** 绘制文本 */
LCUI_API int TextLayer_Draw( LCUI_TextLayer* layer );

/** 清除已记录的无效矩形 */
LCUI_API void TextLayer_ClearInvalidRect( LCUI_TextLayer *layer );

/** 设置全局文本样式 */
LCUI_API void TextLayer_SetTextStyle( LCUI_TextLayer *layer,
					LCUI_TextStyle *style );

LCUI_END_HEADER

#endif
