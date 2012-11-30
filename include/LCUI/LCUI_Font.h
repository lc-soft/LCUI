/* ***************************************************************************
 * LCUI_Font.h -- The font handling module of LCUI
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Font.h -- LCUI的字体处理模块
 *
 * 版权所有 (C) 2012 归属于 
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
#ifndef __LCUI_FONT_H__
#define __LCUI_FONT_H__

#include <stdint.h>

#define ENCODEING_TYPE_UTF8	0
#define ENCODEING_TYPE_GB2312	1

LCUI_BEGIN_HEADER

int Using_GB2312();
/* 
 * 说明：如果你的系统只能使用GB2312编码，不能使用UTF-8编码，可以使用这
 * 个函数进行设置，让相关函数正常转换字符编码 
 * */

int Char_To_Wchar_T(char *in_text, wchar_t **unicode_text);
/*
 * 功能：将char型字符串转换成wchar_t字符串
 * 参数说明：
 * in_text      ：传入的char型字符串
 * unicode_text ：输出的wchar_t型字符串
 * 返回值：正常则wchar_t型字符串的长度，否则返回-1
 * */

int FontBMP_Valid(LCUI_FontBMP *bitmap);
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回1，无效返回0
 */ 

void Print_FontBMP_Info(LCUI_FontBMP *bitmap);
/* 功能：打印位图的信息 */ 

void FontBMP_Init(LCUI_FontBMP *bitmap);
/* 初始化字体位图 */ 

void FontBMP_Free(LCUI_FontBMP *bitmap);
/* 释放字体位图占用的资源 */ 

int FontBMP_Create(LCUI_FontBMP *bitmap, int width, int height);
/* 功能：为Bitmap内的数据分配内存资源，并初始化 */ 

void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap);
/* 功能：根据字符编码，获取系统自带的字体位图 */ 

void Set_Default_Font(char *fontfile);
/* 
 * 功能：设定默认的字体文件路径
 * 说明：需要在LCUI初始化前使用，因为LCUI初始化时会打开默认的字体文件
 *  */ 

void LCUI_Font_Init(LCUI_Font *font);
/* 
 * 功能：初始化LCUI的Font结构体数据 
 * 说明：本函数在LCUI初始化时调用，LCUI_Font结构体中记录着字体相关的数据
 * */

void Font_Init(LCUI_Font *in);
/* 
 * 功能：初始化Font结构体数据
 * 说明：默认是继承系统的字体数据
 * */ 

void Font_Free(LCUI_Font *in);
/* 功能：释放Font结构体数据占用的内存资源 */ 

void LCUI_Font_Free();
/* 功能：释放LCUI默认的Font结构体数据占用的内存资源 */ 

int Show_FontBMP(LCUI_FontBMP *in_fonts);
/* 功能：在屏幕打印以0和1表示字体位图 */ 

int FontBMP_Mix( LCUI_Graph	*graph, LCUI_Pos	des_pos,
		LCUI_FontBMP	*bitmap, LCUI_RGB	color,
		int flag );
/* 功能：将字体位图数据与背景图形混合 */

int Open_Fontfile(LCUI_Font *font_data, char *fontfile);
/* 功能：打开字体文件，并保存数据至LCUI_Font结构体中 */ 

int Get_FontBMP(LCUI_Font *font_data, wchar_t ch, int pixel_size, LCUI_FontBMP *out_bitmap);
/*
 * 功能：获取单个wchar_t型字符的位图
 * 说明：LCUI_Font结构体中储存着已被打开的字体文件句柄和face对象的句柄，如果字体文件已经被
 * 成功打开一次，此函数不会再次打开字体文件。
 */


/*************************** LCUI_TextLayer ****************************/
typedef struct _LCUI_TextStyle	LCUI_TextStyle; 
typedef struct _LCUI_TextLayer 	LCUI_TextLayer; 
typedef struct _LCUI_CharData	LCUI_CharData;
typedef struct _Text_RowData		Text_RowData; 

typedef enum _font_style		enum_font_style;
typedef enum _font_weight		enum_font_weight;
typedef enum _font_decoration	enum_font_decoration; 

/********* 保存字体相关数据以及位图 ********/
struct _LCUI_CharData
{
	BOOL display:1;		/* 标志，是否需要显示该字 */
	BOOL need_update:1;	/* 标志，表示是否需要刷新该字的字体位图数据 */
	//BOOL using_quote:2;	/* 标志，表示是否引用了现成的文本样式 */
	
	wchar_t char_code;	/* 字符码 */
	LCUI_FontBMP bitmap;	/* 字体位图 */
	LCUI_TextStyle *data;	/* 文本样式数据 */
};
/***************************************/

/********* 保存一行的文本数据 *************/
struct _Text_RowData
{
	LCUI_Size max_size;	/* 记录最大尺寸 */
	LCUI_Pos pos;		/* 当前行所在的位置 */
	LCUI_Queue string;	/* 这个队列中的成员用于引用源文本的字体数据 */
	LCUI_CharData *last_char;	/* 保存最后一个字符数据的地址，通常是换行符 */
};
/***************************************/

enum _font_style
{ 
	FONT_STYLE_NORMAL = 0, 
	FONT_STYLE_ITALIC = 1, 
	FONT_STYLE_OBIQUE = 2 
};

enum _font_weight
{
	FONT_WEIGHT_NORMAL	= 0,
	FONT_WEIGHT_BOLD	= 1 
}; 

enum _font_decoration
{
	FONT_DECORATION_NONE		= 0,	/* 无装饰 */
	FONT_DECORATION_BLINK		= 1,	/* 闪烁 */
	FONT_DECORATION_UNDERLINE	= 2,	/* 下划线 */
	FONT_DECORATION_LINE_THROUGH	= 3,	/* 贯穿线 */
	FONT_DECORATION_OVERLINE	= 4	/* 上划线 */
};

/******************* 字体相关数据 **********************/
struct _LCUI_TextStyle
{
	BOOL _family:1;
	BOOL _style:1;
	BOOL _weight:1;
	BOOL _decoration:1;
	BOOL _back_color:1;
	BOOL _fore_color:1;
	BOOL _pixel_size:1;
	
	char family[256];
	enum_font_style	style		:3;
	enum_font_weight	weight		:3;
	enum_font_decoration	decoration	:4;
	
	LCUI_RGB fore_color;
	LCUI_RGB back_color;
	
	int pixel_size;	
};
/******************************************************/

struct _LCUI_TextLayer
{
	BOOL read_only		:1;	/* 指示文本内容是否为只读 */
	BOOL using_code_mode	:1;	/* 指示是否开启代码模式 */
	BOOL using_style_tags	:1;	/* 指示是否处理样式标签 */
	BOOL enable_word_wrap	:1;	/* 指示是否自动换行 */
	BOOL enable_multiline	:1;	/* 指示是否为多行文本图层部件 */ 
	BOOL need_proc_buff	:1;	/* 指示是否处理缓冲区内的文本 */
	BOOL need_scroll_layer	:1;	/* 指示是否需要滚动图层 */
	BOOL have_select : 1;	/* 标记，指示是否在文本图层中选择了文本 */
	uint32_t start, end;	/* 被选中的文本的范围 */ 
	
	LCUI_Queue color_keyword;	/* 记录需要使用指定风格的关键字 */
	LCUI_Queue text_source_data;	/* 储存文本相关数据 */
	LCUI_Queue rows_data;		/* 储存每一行文本的数据 */
	LCUI_Queue tag_buff;		/* 保存样式标签中表达的属性数据 */
	LCUI_Queue style_data;		/* 保存样式数据 */
	LCUI_Queue clear_area;		/* 记录需刷新的区域 */
	
	LCUI_Pos offset_pos;		/* 偏移位置 */
	uint32_t current_src_pos;	/* 当前光标在源文本中位置 */
	LCUI_Pos current_des_pos;	/* 当前光标在分段后的文本中的位置 */
	uint32_t max_text_len;		/* 最大文本长度 */
	
	BOOL show_cursor;	/* 指定是否需要显示文本光标 */
	int timer_id;		/* 定时器的ID */
	
	LCUI_String text_buff;
	LCUI_TextStyle default_data;	/* 缺省状态下使用的文本样式数据 */
};

/**********************************************************************/

/**************************** TextStyle *******************************/
void 
TextStyle_Init ( LCUI_TextStyle *data );
/* 初始化字体样式数据 */ 

void
TextStyle_FontFamily( LCUI_TextStyle *style, const char *fontfamily );
/* 设置字体族 */ 

void
TextStyle_FontSize( LCUI_TextStyle *style, int fontsize );
/* 设置字体大小 */ 

void
TextStyle_FontColor( LCUI_TextStyle *style, LCUI_RGB color );
/* 设置字体颜色 */ 

void
TextStyle_FontBackColor( LCUI_TextStyle *style, LCUI_RGB color );
/* 设置字体背景颜色 */ 

void
TextStyle_FontStyle( LCUI_TextStyle *style, enum_font_style fontstyle );
/* 设置字体样式 */ 

void
TextStyle_FontWeight( LCUI_TextStyle *style, enum_font_weight fontweight );

void
TextStyle_FontDecoration( LCUI_TextStyle *style, enum_font_decoration decoration );
/* 设置字体下划线 */ 

int 
TextStyle_Cmp( LCUI_TextStyle *a, LCUI_TextStyle *b );

/************************** End TextStyle *****************************/


/*************************** TextLayer *********************************/
void 
TextLayer_Init( LCUI_TextLayer *layer );
/* 初始化文本图层相关数据 */ 

void 
Destroy_TextLayer( LCUI_TextLayer *layer );
/* 销毁文本图层占用的资源 */ 

void 
TextLayer_Draw( LCUI_Widget *widget, LCUI_TextLayer *layer, int mode );
/* 绘制文本图层 */

void
TextLayer_Refresh( LCUI_TextLayer *layer );
/* 标记文本图层中每个字的位图，等待绘制文本图层时进行更新 */

void
TextLayer_Set_Offset( LCUI_TextLayer *layer, LCUI_Pos offset_pos );
/* 设定文本图层的偏移位置 */

LCUI_Size 
TextLayer_Get_Size ( LCUI_TextLayer *layer );
/* 获取文本图层的实际尺寸 */

wchar_t *
TextLayer_Get_Text( LCUI_TextLayer *layer );

void
TextLayer_Text_Set_Default_Style( LCUI_TextLayer *layer, LCUI_TextStyle style );
/* 设定默认的文本样式，需要调用TextLayer_Draw函数进行文本位图更新 */

void 
TextLayer_ReadOnly( LCUI_TextLayer *layer, BOOL flag );
/* 指定文本图层中的文本是否为只读 */

void
TextLayer_Text_Clear( LCUI_TextLayer *layer );
/* 清空文本内容 */

void
TextLayer_Text( LCUI_TextLayer *layer, char *new_text );
/* 设定整个文本图层中需显示的文本，光标复位，原有选中文本被删除 */ 

int 
TextLayer_Append( LCUI_TextLayer *layer, char *new_text );
/* 在文本末尾追加文本，不移动光标，不删除原有选中文本 */ 

int 
TextLayer_Text_Add( LCUI_TextLayer *layer, char *new_text );
/* 在光标处添加文本，如有选中文本，将被删除 */

int 
TextLayer_Text_Paste( LCUI_TextLayer *layer );
/* 将剪切板的内容粘贴至文本图层 */ 

int 
TextLayer_Text_Backspace( LCUI_TextLayer *layer, int n );
/* 删除光标左边处n个字符 */ 

int 
TextLayer_Text_Delete( LCUI_TextLayer *layer, int n );
/* 删除光标右边处n个字符 */ 

LCUI_Pos 
TextLayer_Get_Char_PixelPos( LCUI_TextLayer *layer, LCUI_Pos char_pos );
/* 获取显示出来的文字相对于文本图层的坐标，单位为像素 */

LCUI_Pos 
TextLayer_Set_Cursor_PixelPos( LCUI_TextLayer *layer, LCUI_Pos pixel_pos );
/* 
 * 功能：根据传入的二维坐标，设定光标在的文本图层中的位置
 * 说明：该位置会根据当前位置中的字体位图来调整，确保光标显示在字体位图边上，而不
 * 会遮挡字体位图；光标在文本图层中的位置改变后，在字符串中的位置也要做相应改变，
 * 因为文本的添加，删减，都需要以光标当前所在位置对应的字符为基础。
 * 返回值：文本图层中对应字体位图的坐标
 *  */

LCUI_Pos
TextLayer_Get_Cursor_Pos( LCUI_TextLayer *layer );
/* 获取光标在文本框中的位置，也就是光标在哪一行的哪个字后面 */ 


LCUI_Pos
TextLayer_Get_Cursor_FixedPixelPos( LCUI_TextLayer *layer );
/* 获取文本图层的光标位置，单位为像素 */

LCUI_Pos
TextLayer_Get_Cursor_PixelPos( LCUI_TextLayer *layer );
/* 获取文本图层的光标相对于容器位置，单位为像素 */

LCUI_Pos
TextLayer_Set_Cursor_Pos( LCUI_TextLayer *layer, LCUI_Pos pos );
/* 设定光标在文本框中的位置，并返回该光标的坐标，单位为像素 */

int
TextLayer_Get_RowLen( LCUI_TextLayer *layer, int row );
/* 获取指定行显式文字数 */

int 
TextLayer_CurRow_Get_MaxHeight( LCUI_TextLayer *layer );
/* 获取当前行的最大高度 */

int 
TextLayer_Get_Rows( LCUI_TextLayer *layer );
/* 获取文本行数 */

int 
TextLayer_Get_Select_Text( LCUI_TextLayer *layer, char *out_text );
/* 获取文本图层内被选中的文本 */ 

int 
TextLayer_Copy_Select_Text( LCUI_TextLayer *layer );
/* 复制文本图层内被选中的文本 */ 

int 
TextLayer_Cut_Select_Text( LCUI_TextLayer *layer );
/* 剪切文本图层内被选中的文本 */ 

void 
TextLayer_Using_StyleTags( LCUI_TextLayer *layer, BOOL flag );
/* 指定文本图层是否处理样式标签 */ 

/*************************** End TextLayer ****************************/


LCUI_END_HEADER


#endif /* __LCUI_FONTS_H__ */
