/* ***************************************************************************
 * LCUI_Label.c -- LCUI's Label widget
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
 * LCUI_Label.c -- LCUI 的文本标签部件
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
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_MEM_H
#include LC_MISC_H
#include LC_FONT_H
#include LC_GRAPHICS_H
#include LC_LABEL_H

#include <stdarg.h>

static void Label_Init(LCUI_Widget *widget)
/* 功能：初始化label部件数据 */
{ 
	LCUI_Label *label;
	label = (LCUI_Label*)Malloc_Widget_Private(widget, sizeof(LCUI_Label));

	label->auto_size	= IS_TRUE; 				/* 开启自动调整尺寸 */
	label->image		= NULL;
	label->image_align	= ALIGN_MIDDLE_CENTER;	/* 对齐方式为居中 */
	label->contents		= NULL; 
	label->rows			= 0;					/* 行数归0 */
	label->text_align	= ALIGN_TOP_LEFT;		/* 文本对齐方式为左上角对齐 */  
	
	Font_Init(&label->font); /* 初始化字体信息 */
	String_Init(&label->text);/* 初始化字符串 */
}

static void Destroy_Label(LCUI_Widget *widget)
/* 功能：释放label部件占用的资源 */
{
	LCUI_Label *label;
	label = (LCUI_Label*)Get_Widget_Private_Data(widget);
	Free_WString(label->contents);
	label->contents = NULL;
	Free_String(&label->text);
}

static void Refresh_Label_FontBitmap(LCUI_Widget *widget)
/* 功能：刷新label部件内的字体位图 */
{ 
	int i, k;
	
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);  
	
	if(label->text.size <= 0)
		return;
		
	for(i = 0; i < label->rows; ++i)
	{/* 遍历每一行字符串中的每个字符 */ 
		for(k = 0; k < label->contents[i].size; ++k)
		{/* 获取字符位图 */ 
			Get_WChar_Bitmap(&label->font, 
					label->contents[i].string[k].char_code , 
					&label->contents[i].string[k].bitmap ); 
		} 
		label->contents[i].update = IS_TRUE;
	}
	Draw_Widget(widget);
}

static void Exec_Update_Label(LCUI_Widget *widget)
/* 功能：执行更新label部件的操作 */
{
    int len;
	int i, k;
	int max_height = 0, max_width = 0;
	
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget); 
	
	len = label->text.size; 
	
	if(len <= 0)
	{/* 如果内容为空 */
		if(label->auto_size == IS_TRUE) 
			/*如果开启了自动调整大小*/
			Resize_Widget(widget, Size(0, 0)); 
			
		widget->graph.flag = HAVE_ALPHA; 
	}
	else
	{
		/* 声明一个用于保存分割后的wchar_t型字符串的LCUI_Wchar_T型指针 */
		LCUI_WString *new_list; 
		/* 声明两个用于保存字符串行数的整型变量 */
		int old_rows, new_rows; 
		/* 分割成wchar_t型数据 */ 
		new_rows = String_To_List(label->text.string , &new_list); 
		/* 获取之前文本的行数 */
		old_rows = label->rows;
		
		if(old_rows > 0) /* 改变内存空间大小 */
            label->contents = (LCUI_WString *)realloc(label->contents, 
										sizeof(LCUI_WString) * new_rows); 
		else 
			label->contents = (LCUI_WString *)calloc(new_rows, 
												sizeof(LCUI_WString));
		
		for(i = 0; i < new_rows; ++i)
		{/* 遍历每一行 */
			/* 改变这一行内容占的内存空间大小 */
			if(old_rows > i)
				label->contents[i].string = (LCUI_WChar_T*)realloc(
								label->contents[i].string, 
								sizeof(LCUI_WChar_T) * 
								(new_list[i].size + 1));
			else
			{
				label->contents[i].string = (LCUI_WChar_T*)malloc(
							sizeof(LCUI_WChar_T) * 
							(new_list[i].size + 1));
				label->contents[i].size = 0;
			}
			
			for(k = 0; k < new_list[i].size; ++k)
			{/* 遍历这行的字符串的每个元素 */
				if(new_list[i].string[k].color_type == DEFAULT)
				/* 使用默认的颜色 */
					label->contents[i].string[k].color = label->font.fore_color;
				else  /* 否则使用自定义颜色 */
					label->contents[i].string[k].color = new_list[i].string[k].color; 
				
				if(k < label->contents[i].size)
				{/* 如果字符的位置还在label->contents[i]的范围内 */
					if(label->contents[i].string[k].char_code != new_list[i].string[k].char_code)
					{/* 如果文字不一样，就需要刷新该字 */
						label->contents[i].string[k].char_code = new_list[i].string[k].char_code; 
						/* 获取字符位图 */ 
						Get_WChar_Bitmap(&label->font, 
								label->contents[i].string[k].char_code , 
								&label->contents[i].string[k].bitmap );
                        label->contents[i].update = IS_TRUE;/* 这一行文本需要刷新 */ 
					}
				}
				else
				{/* 否则，说明new_list[i]的元素个数比label->contents[i]的多 */
					label->contents[i].string[k].char_code = new_list[i].string[k].char_code; 
					/* 先初始化数据结构，然后获取字符位图 */
					Bitmap_Init(&label->contents[i].string[k].bitmap);
					Get_WChar_Bitmap(&label->font, 
							label->contents[i].string[k].char_code , 
							&label->contents[i].string[k].bitmap );
					label->contents[i].update = IS_TRUE; 
				} 
			}
			/* 改成新的字符串长度 */
			label->contents[i].size = new_list[i].size;
			/* 释放内存，这行的数据没用了 */
			free(new_list[i].string);
		}
		/* 释放内存，new_list已经不需要用了 */
		free(new_list);
		new_list = NULL; 
			/* 计算文本内容的尺寸 */
		label->rows = new_rows;
		Count_Contents_Size(label->contents, &max_width, &max_height, 
				label->rows, label->font.space, label->font.linegap);
		if(label->auto_size == IS_TRUE 
		&& (max_width != widget->size.w 
		|| max_height != widget->size.h))
		{/* 如果开启了自动调整大小,并且尺寸有改变 */
			Resize_Widget(widget, Size(max_width, max_height + 2));
			Refresh_Widget(widget);
		} 
	}
	
	int flag;
    if(Valid_Graph(label->image))
	{ /* 如果有标签图像需要显示 */
        if(Valid_Graph(&widget->background_image))
            flag = label->image_align | GRAPH_MIX_FLAG_OVERLAY; /* 叠加模式 */ 
        else 
			flag = label->image_align | GRAPH_MIX_FLAG_REPLACE; /* 替换模式 */
        /* 根据对齐方式以及处理模式来处理图形 */
		Align_Image(&widget->graph, label->image, flag); 
	}
    else
    {
        if(!Valid_Graph(&widget->background_image)) 
			flag = GRAPH_MIX_FLAG_REPLACE; /* 替换模式 */
        else 
			flag = GRAPH_MIX_FLAG_OVERLAY; /* 叠加模式 */
    }
	/* 合成字体位图 */
	Mix_Widget_FontBitmap(widget, 1, 1, label->contents, label->rows, 
							label->font.space, label->font.linegap, flag);
}

void Set_Label_Image(LCUI_Widget *widget, LCUI_Graph *img, LCUI_Align align)
/* 
 * 功能：设置label部件的背景图像
 * 参数说明：
 * widget ：需要进行操作的部件
 * img    ：需要设为背景图的图像数据
 * align  ：背景图像的布局
 * */
{
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
    
	if(img != NULL)
	{
		if(Valid_Graph(img))
		{
			label->image = img;
			label->image_align = align;
			Draw_Widget(widget);
		}
		else label->image = NULL;
	}
}

int Get_Label_Row_Len(LCUI_Widget *widget, int row)
/*
 * 功能：获取label部件中指定行的字符串长度
 * 参数说明：
 * widget ：需要进行操作的部件
 * row   : 第几行
 * 返回值：失败则返回-2，成功返回长度，类型不符返回-1
 * */
{
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
	
	if(row > 0 && row <= label->rows) 
		return label->contents[row - 1].size; 
	return -2;
}

void Set_Label_Text(LCUI_Widget *widget, const char *fmt, ...)
/* 功能：设定与标签关联的文本内容 */
{
	char text[LABEL_TEXT_MAX_SIZE];
    memset(text, 0, sizeof(text)); 
    
	LCUI_Label *label;
	
	label = (LCUI_Label*)Get_Widget_Private_Data(widget);
	
    va_list ap;
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE, fmt, ap);
	va_end(ap); 
	 
	Strcpy(&label->text, text);/* 拷贝字符串 */ 
	Draw_Widget(widget);/* 更新部件 */ 
}

int Set_Label_Font(LCUI_Widget *widget, int font_size, char *font_file)
/*
 * 功能：为标签文字设定字体大小、字体类型、字体颜色
 * 参数说明：
 * widget    ：需要进行操作的部件
 * font_file ：字体文件的位置，最好为绝对路径
 * color     ：字体的配色，也就是最终显示的颜色
 * */
{
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
    
	int refresh_flag = 0;
	if(label->font.size != font_size)
	{
		label->font.size = font_size;
		refresh_flag = 1;
	}
	if(Strcmp(&label->font.font_file, font_file) != 0
	&& 0 == Open_Fontfile(&label->font, font_file) )
	{
		Strcpy(&label->font.font_file, font_file);
		refresh_flag = 1;
	} 
	if(refresh_flag == 1)
    {/* 刷新标签内的字体位图数据 */ 
		Refresh_Label_FontBitmap(widget); 
	}
	return 0;
}

void Set_Label_Font_Default_Color(LCUI_Widget *widget, LCUI_RGB color)
/*
 * 功能：设置label部件关联的文本的字体颜色，字体颜色为RGB三色的混合色
 * 参数说明：
 * label ：需要进行操作的label部件
 * color : 配色
 * */
{
	int i, k; 
    
	LCUI_Label *label = (LCUI_Label*)Get_Widget_Private_Data(widget);
    
    label->font.fore_color = color;

	if(label->contents != NULL && label->rows > 0)
	{
		for(i = 0; i < label->rows; ++i)
		{/* 遍历每一行 */
			for(k = 0; k < label->contents[i].size; ++k)
			{/* 遍历这行的字符串的每个元素 */
                if(label->contents[i].string[k].color_type == DEFAULT)
                {/* 如果改变了label部件的默认的字体颜色，那么这些使用缺省颜色的文字将刷新 */
                    label->contents[i].string[k].color = label->font.fore_color; 
                }
			}
            label->contents[i].update = IS_TRUE;
		}
		Draw_Widget(widget);/* 更新部件 */
	} 
}

void Register_Label()
/* 功能：注册部件类型-文本标签至部件库 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("label");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("label",	Label_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("label",	Exec_Update_Label,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("label", Destroy_Label,		FUNC_TYPE_DESTROY);
}
