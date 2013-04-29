/* ***************************************************************************
 * textlayer.h -- text bitmap layer processing module.
 * 
 * Copyright (C) 2013 by
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
 * 版权所有 (C) 2013 归属于
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

#define	AT_TEXT_LAST	0
#define	AT_CURSOR_POS	1

LCUI_BEGIN_HEADER

/********* 保存字体相关数据以及位图 ********/
typedef struct _LCUI_CharData
{
	LCUI_BOOL display:1;		/* 标志，是否需要显示该字 */
	LCUI_BOOL need_update:1;	/* 标志，表示是否需要刷新该字的字体位图数据 */
	//LCUI_BOOL using_quote:2;	/* 标志，表示是否引用了现成的文本样式 */
	
	wchar_t char_code;	/* 字符码 */
	LCUI_FontBMP *bitmap;	/* 字体位图数据 */
	LCUI_TextStyle *data;	/* 文本样式数据 */
}
LCUI_CharData;
/***************************************/

/********* 保存一行的文本数据 *************/
typedef struct _Text_RowData
{
	LCUI_Size max_size;	/* 记录最大尺寸 */
	LCUI_Pos pos;		/* 当前行所在的位置 */
	LCUI_Queue string;	/* 这个队列中的成员用于引用源文本的字体数据 */
	LCUI_CharData *last_char;	/* 保存最后一个字符数据的地址，通常是换行符 */
}
Text_RowData;
/***************************************/

typedef struct _LCUI_TextLayer
{
	LCUI_BOOL read_only		:1;	/* 指示文本内容是否为只读 */
	LCUI_BOOL using_code_mode	:1;	/* 指示是否开启代码模式 */
	LCUI_BOOL using_style_tags	:1;	/* 指示是否处理样式标签 */
	LCUI_BOOL enable_word_wrap	:1;	/* 指示是否自动换行 */
	LCUI_BOOL enable_multiline	:1;	/* 指示是否为多行文本图层部件 */ 
	LCUI_BOOL need_proc_buff	:1;	/* 指示是否处理缓冲区内的文本 */
	LCUI_BOOL need_scroll_layer	:1;	/* 指示是否需要滚动图层 */
	LCUI_BOOL have_select		:1;	/* 标记，指示是否在文本图层中选择了文本 */
	uint32_t start, end;	/* 被选中的文本的范围 */ 
	
	LCUI_Queue color_keyword;	/* 记录需要使用指定风格的关键字 */
	LCUI_Queue text_source_data;	/* 储存文本相关数据 */
	LCUI_Queue rows_data;		/* 储存每一行文本的数据 */
	LCUI_Queue tag_buff;		/* 保存样式标签中表达的属性数据 */
	LCUI_Queue style_data;		/* 保存样式数据 */
	LCUI_RectQueue clear_area;	/* 记录需刷新的区域 */
	
	LCUI_Pos offset_pos;		/* 偏移位置 */
	LCUI_Pos old_offset_pos;	/* 未滚动前的偏移位置 */
	uint32_t current_src_pos;	/* 当前光标在源文本中位置 */
	LCUI_Pos current_des_pos;	/* 当前光标在分段后的文本中的位置 */
	uint32_t max_text_len;		/* 最大文本长度 */
	LCUI_CharData password_char;	/* 该字符用于屏蔽单行，为0时则不屏蔽 */
	
	LCUI_BOOL show_cursor;	/* 指定是否需要显示文本光标 */
	
	LCUI_WString text_buff;
	LCUI_TextStyle default_data;	/* 缺省状态下使用的文本样式数据 */
}
LCUI_TextLayer;

LCUI_EXPORT(void)
TextLayer_Init( LCUI_TextLayer *layer );
/* 初始化文本图层相关数据 */

LCUI_EXPORT(void)
Destroy_TextLayer( LCUI_TextLayer *layer );
/* 销毁文本图层占用的资源 */

LCUI_EXPORT(void)
TextLayer_Draw( LCUI_Widget *widget, LCUI_TextLayer *layer, int mode );
/* 将文本图层绘制到目标部件的图层上 */

LCUI_EXPORT(void)
TextLayer_Refresh( LCUI_TextLayer *layer );
/* 标记文本图层中每个字的位图，等待绘制文本图层时进行更新 */

/* 
 * 功能：设定文本图层的偏移位置
 * 返回值：需要对图层进行重绘时返回0，否则返回1
 *  */
LCUI_EXPORT(int) TextLayer_SetOffset( LCUI_TextLayer *layer, LCUI_Pos offset_pos );

LCUI_EXPORT(LCUI_Size)
TextLayer_GetSize ( LCUI_TextLayer *layer );
/* 获取文本图层的实际尺寸 */

/* 获取文本图层中的文本内容 */
LCUI_EXPORT(size_t)
TextLayer_GetText( LCUI_TextLayer *layer, wchar_t *buff, size_t max_len );

LCUI_EXPORT(void)
TextLayer_CharLater_Refresh( LCUI_TextLayer *layer, LCUI_Pos char_pos );
/* 刷新指定行中指定字以及后面的字的区域 */

LCUI_EXPORT(void)
TextLayer_Text_SetDefaultStyle( LCUI_TextLayer *layer, LCUI_TextStyle style );
/* 设定默认的文本样式，需要调用TextLayer_Draw函数进行文本位图更新 */

LCUI_EXPORT(void)
TextLayer_ReadOnly( LCUI_TextLayer *layer, LCUI_BOOL flag );
/* 指定文本图层中的文本是否为只读 */

LCUI_EXPORT(void)
TextLayer_Text_Clear( LCUI_TextLayer *layer );
/* 清空文本内容 */

LCUI_EXPORT(void)
TextLayer_SetRowEnd( LCUI_TextLayer *layer, uint_t row, uint_t start_cols );
/* 为指定行设定结束点，结束点及后面的数据将被删除，但不记录残余文本位图区域 */

LCUI_EXPORT(int)
TextLayer_Text_GetTotalLength( LCUI_TextLayer *layer );
/* 获取文本位图中的文本长度 */

LCUI_EXPORT(void)
TextLayer_Text_Set_MaxLength( LCUI_TextLayer *layer, int max );
/* 设定文本位图中的文本长度 */

LCUI_EXPORT(void)
TextLayer_Text_SetPasswordChar( LCUI_TextLayer *layer, wchar_t ch );
/* 
 * 设置屏蔽字符，设置后，文本框内的文本都会显示成该字符
 * 如果ch的值为0，则不对文本框里的文本进行屏蔽 
 * */

/* 对文本进行预处理，处理后的数据保存至layer里 */ 
LCUI_EXPORT(void)
TextLayer_Text_Process(	LCUI_TextLayer *layer,
				int pos_type,
				wchar_t *new_text );

LCUI_EXPORT(void)
TextLayer_Text_GenerateBMP( LCUI_TextLayer *layer );
/* 为文本图层中的文本生成位图，已存在位图的文字将不重新生成 */

LCUI_EXPORT(void)
TextLayer_PrintInfo( LCUI_TextLayer *layer );
/* 打印文本图层信息 */

/* 
 * 功能：设定指定的宽字符串作为文本图层中显示的文本
 * 说明：文本将被储存至缓冲区，等待绘制文本位图时再处理缓冲区内的文本
 *  */
LCUI_EXPORT(void)
TextLayer_TextW( LCUI_TextLayer *layer, const wchar_t *wchar_text );

LCUI_EXPORT(void)
TextLayer_Text( LCUI_TextLayer *layer, const char *utf8_text );

LCUI_EXPORT(void)
TextLayer_TextA( LCUI_TextLayer *layer, const char *ascii_text );

/* 在文本末尾追加文本，不移动光标，不删除原有选中文本 */
LCUI_EXPORT(int)
TextLayer_Text_AppendW( LCUI_TextLayer *layer, wchar_t *new_text );

LCUI_EXPORT(int)
TextLayer_Text_AppendA( LCUI_TextLayer *layer, char *new_text );

LCUI_EXPORT(int)
TextLayer_Text_Append( LCUI_TextLayer *layer, char *new_text );

LCUI_EXPORT(int)
TextLayer_Text_AddW( LCUI_TextLayer *layer, wchar_t *unicode_text );

LCUI_EXPORT(int)
TextLayer_Text_AddA( LCUI_TextLayer *layer, char *ascii_text );

/* 在光标处添加文本，如有选中文本，将被删除 */
LCUI_EXPORT(int)
TextLayer_Text_Add( LCUI_TextLayer *layer, char *utf8_text );

LCUI_EXPORT(LCUI_Pos)
TextLayer_Cursor_SetPixelPos( LCUI_TextLayer *layer, LCUI_Pos pixel_pos );
/* 
 * 功能：根据传入的二维坐标，设定光标在的文本图层中的位置
 * 说明：该位置会根据当前位置中的字体位图来调整，确保光标显示在字体位图边上，而不
 * 会遮挡字体位图；光标在文本图层中的位置改变后，在字符串中的位置也要做相应改变，
 * 因为文本的添加，删减，都需要以光标当前所在位置对应的字符为基础。
 * 返回值：文本图层中对应字体位图的坐标，单位为像素
 *  */

LCUI_EXPORT(LCUI_Pos)
TextLayer_Cursor_SetPos( LCUI_TextLayer *layer, LCUI_Pos pos );
/* 设定光标在文本框中的位置，并返回该光标的坐标，单位为像素 */

LCUI_EXPORT(int)
TextLayer_Text_Backspace( LCUI_TextLayer *layer, int n );
/* 删除光标左边处n个字符 */

LCUI_EXPORT(LCUI_Pos)
TextLayer_Char_GetPixelPos( LCUI_TextLayer *layer, LCUI_Pos char_pos );
/* 获取显示出来的文字相对于文本图层的坐标，单位为像素 */

LCUI_EXPORT(LCUI_Pos)
TextLayer_Cursor_GetPos( LCUI_TextLayer *layer );
/* 获取光标在文本框中的位置，也就是光标在哪一行的哪个字后面 */

LCUI_EXPORT(LCUI_Pos)
TextLayer_Cursor_GetFixedPixelPos( LCUI_TextLayer *layer );
/* 获取文本图层的光标位置，单位为像素 */

LCUI_EXPORT(LCUI_Pos)
TextLayer_Cursor_GetPixelPos( LCUI_TextLayer *layer );
/* 获取文本图层的光标相对于容器位置，单位为像素 */

LCUI_EXPORT(int)
TextLayer_GetRowLen( LCUI_TextLayer *layer, int row );
/* 获取指定行显式文字数 */

LCUI_EXPORT(int)
TextLayer_CurRow_GetMaxHeight( LCUI_TextLayer *layer );
/* 获取当前行的最大高度 */

LCUI_EXPORT(int)
TextLayer_GetRows( LCUI_TextLayer *layer );
/* 获取文本行数 */

LCUI_EXPORT(int)
TextLayer_GetSelectedText( LCUI_TextLayer *layer, char *out_text );
/* 获取文本图层内被选中的文本 */ 

LCUI_EXPORT(int)
TextLayer_CopySelectedText( LCUI_TextLayer *layer );
/* 复制文本图层内被选中的文本 */

LCUI_EXPORT(int)
TextLayer_CutSelectedText( LCUI_TextLayer *layer );
/* 剪切文本图层内被选中的文本 */

LCUI_EXPORT(void)
TextLayer_UsingStyleTags( LCUI_TextLayer *layer, LCUI_BOOL flag );
/* 指定文本图层是否处理样式标签 */

LCUI_EXPORT(void)
TextLayer_Multiline( LCUI_TextLayer *layer, LCUI_BOOL flag );
/* 指定文本图层是否启用多行文本显示 */

LCUI_END_HEADER

#endif
