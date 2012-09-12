/* ***************************************************************************
 * LCUI_Font.h -- Font Handling
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
 * LCUI_Font.h -- 字体处理
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
#ifndef __LCUI_FONTS_H__
#define __LCUI_FONTS_H__

#define ENCODEING_TYPE_UTF8		0
#define ENCODEING_TYPE_GB2312	1

void Get_Default_Font_Bitmap(unsigned short code, LCUI_Bitmap *out_bitmap);
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
 *  */ 

int Show_Font_Bitmap(LCUI_Bitmap *in_fonts);
/* 功能：在屏幕打印以0和1表示字体位图 */ 

int Mix_Fonts_Bitmap(
		LCUI_Graph	*back_graph,  /* 背景图形 */
		int		start_x,
		int		start_y,
		LCUI_Bitmap	*in_fonts,  /* 传入的字体位图数据 */
		LCUI_RGB	color,       /* 字体的配色 */ 
		int		flag
);
/* 功能：将字体位图数据与背景图形混合 */ 

int Char_To_Wchar_T(char *in_text, wchar_t **unicode_text);
/*
 * 功能：将char型字符串转换成wchar_t字符串
 * 参数说明：
 * in_text      ：传入的char型字符串
 * unicode_text ：输出的wchar_t型字符串
 * 返回值：正常则wchar_t型字符串的长度，否则返回-1
 * */ 

int String_To_List(char *text , LCUI_WString **out_list);
/*
 * 功能：将原始的字符串分割成二维数组,字符编码为Unicode
 * 参数说明：
 * in_text  ： 传入的char型字符串
 * list     ： 输出的二维LCUI_Wchar_T指针
 * 返回值：字符串的行数
 * */ 

int Open_Fontfile(LCUI_Font *font_data, char *fontfile);
/* 功能：打开字体文件，并保存数据至LCUI_Font结构体中 */ 

int Get_WChar_Bitmap(LCUI_Font *font_data, wchar_t ch, LCUI_Bitmap *out_bitmap);
/*
 * 功能：获取单个wchar_t型字符的位图
 * 说明：LCUI_Font结构体中储存着已被打开的字体文件句柄和face对象的句柄，如果字体文件已经被
 * 成功打开一次，此函数不会再次打开字体文件。
 */ 

int Count_Contents_Size(
	LCUI_WString *contents, 
	int *out_width, int *out_height, 
	int rows, int space, int linegap);
/*
 * 功能：计算文本位图的尺寸
 * 参数说明：
 * contents： 需要计算的目标
 * out_width   ： 输出的宽度
 * out_height  ： 输出的高度
 * rows    :  文本的总行数
 * space   :  每个字之间的间隔
 * linegap :  行距
 * */ 
#endif /* __LCUI_FONTS_H__ */
