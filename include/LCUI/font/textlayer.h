/* ***************************************************************************
 * textlayer.h -- text bitmap layer processing module.
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
 * textlayer.h -- 文本图层处理模块
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

#ifndef __LCUI_TEXTLAYER_H__
#define __LCUI_TEXTLAYER_H__

LCUI_BEGIN_HEADER

/** 文本添加类型 */
enum TextAddType {
        TEXT_ADD_TYPE_INSERT,		/**< 插入至插入点处 */
        TEXT_ADD_TYPE_APPEND		/**< 追加至文本末尾 */
};

typedef struct TextCharData {
        wchar_t char_code;		/**< 字符码 */
        LCUI_TextStyle *style;		/**< 该字符使用的样式数据 */
	const LCUI_FontBitmap *bitmap;	/**< 字体位图数据(只读) */
} TextCharData;

/** 文本行结尾符 */
typedef enum EOLChar {
	EOL_NONE,	/**< 无换行 */
	EOL_CR,		/**< Mac OS 格式换行，CF = Carriage-Return，字符：\r */
	EOL_LF,		/**< UNIX/Linux 格式换行，LF = Line-Feed，字符：\r */
	EOL_CR_LF	/**< Windows 格式换行： \r\n */
} EOLChar;

/* 文本行 */
typedef struct TextRowData {
        int width;			/**< 宽度 */
        int height;			/**< 高度 */
	int text_height;		/**< 当前行中最大字体的高度 */
        int length;			/**< 该行文本长度 */
        int max_length;			/**< 该行文本最大长度 */
        TextCharData **string;		/**< 该行文本的数据 */
	EOLChar eol;			/**< 行尾结束类型 */
} TextRowData;

/* 文本行列表 */
typedef struct TextRowList {
        int length;			/**< 当前总行数 */
        int max_length;			/**< 最大行数 */
        TextRowData **rows;		/**< 每一行文本的数据 */
} TextRowList;

typedef struct LCUI_TextLayerRec_  {
        int offset_x;			/**< X轴坐标偏移量 */
        int offset_y;			/**< Y轴坐标偏移量 */
	int new_offset_x;		/**< 新的X轴坐标偏移量 */
	int new_offset_y;		/**< 新的Y轴坐标偏移量 */
        int insert_x;			/**< 光标所在列数 */
        int insert_y;			/**< 光标所在行数 */
        int max_width;			/**< 最大文本宽度 */
        int max_height;			/**< 最大文本高度 */
	LCUI_BOOL is_mulitiline_mode;	/**< 是否启用多行文本模式 */
        LCUI_BOOL is_autowrap_mode;	/**< 是否启用自动换行模式 */
	LCUI_BOOL is_using_style_tags;	/**< 是否使用文本样式标签 */
        LCUI_BOOL is_using_buffer;	/**< 是否使用缓存空间来存储文本位图 */
	LinkedList dirty_rect;		/**< 脏矩形记录 */

        int text_align;			/**< 文本的对齐方式 */
        TextRowList row_list;		/**< 文本行列表 */
        LCUI_TextStyle text_style;	/**< 文本全局样式 */
	LinkedList style_cache;		/**< 样式缓存 */
	LCUI_Style line_height;		/**< 全局文本行高度 */
	struct {
		LCUI_BOOL update_bitmap;	/**< 更新文本的字体位图 */
		LCUI_BOOL update_typeset;	/**< 重新对文本进行排版 */
		int typeset_start_row;		/**< 排版处理的起始行 */	
		LCUI_BOOL redraw_all;		/**< 重绘所有字体位图 */
	} task;				/**< 待处理的任务 */
        LCUI_Graph graph;		/**< 文本位图缓存 */
}* LCUI_TextLayer;

/** 获取文本行总数 */
LCUI_API int TextLayer_GetRowTotal( LCUI_TextLayer layer );

/** 获取指定文本行的高度 */
LCUI_API int TextLayer_GetRowHeight( LCUI_TextLayer layer, int row );

/** 获取指定文本行的文本长度 */
LCUI_API int TextLayer_GetRowTextLength( LCUI_TextLayer layer, int row );

/** 添加 更新文本排版 的任务 */
LCUI_API void TextLayer_AddUpdateTypeset( LCUI_TextLayer layer, int start_row );

/** 设置文本颜色 */
LCUI_API void TextLayer_SetFontColor( LCUI_TextLayer layer, LCUI_Color color );

/** 设置文本所使用的字体字族 */
LCUI_API void TextLayer_SetFontFamily( LCUI_TextLayer layer, const char *family_name );

/** 设置文本的字体像素大小 */
LCUI_API void TextLayer_SetFontPixelSize( LCUI_TextLayer layer, int pixel_size );

/** 设置文本对齐方式 */
LCUI_API void TextLayer_SetTextAlign( LCUI_TextLayer layer, int align );

/** 设置坐标偏移量 */
LCUI_API void TextLayer_SetOffset( LCUI_TextLayer layer, int offset_x, int offset_y );

LCUI_API LCUI_TextLayer TextLayer_New(void);

/** 销毁TextLayer */
LCUI_API void TextLayer_Destroy( LCUI_TextLayer *layer );

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

/** 设置最大尺寸 */
LCUI_API int TextLayer_SetMaxSize( LCUI_TextLayer layer, LCUI_Size new_size );

/** 设置是否启用多行文本模式 */
LCUI_API void TextLayer_SetMultiline( LCUI_TextLayer layer, int is_true );

/** 删除文本光标的当前坐标右边的文本 */
LCUI_API int TextLayer_Delete( LCUI_TextLayer layer, int n_char );

/** 退格删除文本，即删除文本光标的当前坐标左边的文本 */
LCUI_API int TextLayer_Backspace( LCUI_TextLayer layer, int n_char );

/** 设置是否启用自动换行模式 */
LCUI_API void TextLayer_SetAutoWrap( LCUI_TextLayer layer, int is_true );

/** 设置是否使用样式标签 */
LCUI_API void TextLayer_SetUsingStyleTags( LCUI_TextLayer layer, LCUI_BOOL is_true );

/** 计算并获取文本的宽度 */
LCUI_API int TextLayer_GetWidth( LCUI_TextLayer layer );

/** 计算并获取文本的高度 */
LCUI_API int TextLayer_GetHeight( LCUI_TextLayer layer );

/** 重新载入各个文字的字体位图 */
LCUI_API void TextLayer_ReloadCharBitmap( LCUI_TextLayer layer );

/** 更新数据 */
LCUI_API void TextLayer_Update( LCUI_TextLayer layer, LinkedList *rect_list );

/** 
 * 将文本图层中的指定区域的内容绘制至目标图像中
 * @param layer 要使用的文本图层
 * @param area 文本图层中需要绘制的区域
 * @param layer_pos 文本图层在目标图像中的位置
 * @param graph 目标图像
 */
LCUI_API int TextLayer_DrawToGraph( LCUI_TextLayer layer, LCUI_Rect area,
				    LCUI_Pos layer_pos, LCUI_Graph *graph );

/** 绘制文本 */
LCUI_API int TextLayer_Draw( LCUI_TextLayer layer );

/** 清除已记录的无效矩形 */
LCUI_API void TextLayer_ClearInvalidRect( LCUI_TextLayer layer );

/** 设置全局文本样式 */
LCUI_API void TextLayer_SetTextStyle( LCUI_TextLayer layer, LCUI_TextStyle *style );

/** 设置文本行的高度 */
LCUI_API void TextLayer_SetLineHeight( LCUI_TextLayer layer, LCUI_Style *val );

LCUI_END_HEADER

#endif
