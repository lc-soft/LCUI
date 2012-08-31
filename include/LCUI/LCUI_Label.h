/* ***************************************************************************
 * LCUI_Label.h -- LCUI's Label widget
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
 * LCUI_Label.h -- LCUI 的文本标签部件
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
#ifndef __LCUI_LABEL_H__
#define __LCUI_LABEL_H__
typedef struct _LCUI_Label			LCUI_Label;
/**************************** lable部件 ********************************/
struct _LCUI_Label
{
	int				auto_size;           /* 自动调整大小(IS_TURE/IS_FALSE) */

	LCUI_Font		font;                /* 保存着字体信息 */
	LCUI_String	text;                /* 与控件关联的文本，这个是原始字符串 */
	LCUI_WString	*contents;           /* 内容，记录着每个字的信息以及位图 */
	int				rows;                /* 内容的行数 */
	LCUI_Align		text_align;          /* 文本的对齐方式 */
};/* 可用于显示运行时的信息或者说明性文字 */
/******************************* END ************************************/

int Get_Label_Row_Len(LCUI_Widget *widget, int row);
/*
 * 功能：获取label部件中指定行的字符串长度
 * 参数说明：
 * widget ：需要进行操作的部件
 * row   : 第几行
 * 返回值：失败则返回-2，成功返回长度，类型不符返回-1
 * */ 

void Set_Label_Text(LCUI_Widget *widget, const char *fmt, ...);
/* 功能：设定与标签关联的文本内容 */

int Set_Label_Font(LCUI_Widget *widget, int font_size, char *font_file);
/*
 * 功能：为标签文字设定字体大小、字体类型、字体颜色
 * 参数说明：
 * widget    ：需要进行操作的部件
 * font_file ：字体文件的位置，最好为绝对路径
 * color     ：字体的配色，也就是最终显示的颜色
 * */ 

void Set_Label_Font_Default_Color(LCUI_Widget *widget, LCUI_RGB color);
/*
 * 功能：设置label部件关联的文本的字体颜色，字体颜色为RGB三色的混合色
 * 参数说明：
 * label ：需要进行操作的label部件
 * color : 配色
 * */ 
#endif
