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
#include LC_GRAPH_H
#include LC_LABEL_H

#include <stdarg.h>

/***************************** color text *****************************/

typedef struct
{
	LCUI_RGB *color;
	int total_num;
}
LCUI_color_array;

static void color_array_init(LCUI_color_array *in)
/* 功能：初始化结构体 */
{
	in->color = NULL;
	in->total_num = 0;
}

static void add_color(LCUI_color_array *in, LCUI_RGB color)
/* 功能：将颜色数据追加至颜色数组末尾 */
{
	in->total_num += 1;
	if(in->total_num > 1) 
		in->color = (LCUI_RGB*)realloc(in->color, 
			sizeof(LCUI_RGB) * in->total_num); 
	else 
		in->color = (LCUI_RGB*)malloc(sizeof(LCUI_RGB)); 
	in->color[in->total_num - 1] = color;
}

static void delete_color(LCUI_color_array *in)
/* 功能：从颜色数组中删除最后一个颜色数据 */
{ 
	if(in->total_num > 1) {
		/* 减少成员总数 */
		--in->total_num;
		/* 缩小数据占用的内存空间 */
		in->color = (LCUI_RGB *)realloc(in->color, 
				in->total_num * sizeof(LCUI_RGB));
	} else if(in->total_num == 1) {
		free(in->color);
		in->color = NULL;
		in->total_num = 0;
	} 
}

static int get_color(LCUI_color_array *in, LCUI_RGB *out)
/* 功能：获取颜色数组末尾的颜色数据 */
{
	if(in->total_num > 0) {
		*out = in->color[in->total_num-1];
		return 0;
	}
	return -1;
}

static int wstrcmp(wchar_t *str1, int num, char *str2)
/* 功能：从str1中前num个元素判断是否与str2中的内容全等 */
{
	int i;
	if(str1 == NULL) return -1;
	for(i=0; i<num; ++i)
		if(str1[i] != str2[i]) 
			return -1; 
	return 0;
}

static int cut_wstr_convert_to_color(wchar_t *str1, char mark_char, LCUI_RGB *color)
/* 说明：从str1中开头处起，到第一个与mark_char相同的字符为止，截取这段字符串，并返回截止处的位置 */
{
	char str[105];
	int r, g, b, i, flag = 0;
	/* 简单的转换成char型 */
	for(i=0; i<100; ++i) {
		if(str1[i] == mark_char) {
			flag = 1;
			break;
		}
		str[i] = str1[i];
	} 
	str[i] = 0;
	if(flag == 1) { 
		/* 获取RGB颜色值 */
		sscanf(str, "%d,%d,%d", &r, &g, &b);
		color->red = r;
		color->green = g;
		color->blue = b;;
		return i + 1;
	}
	return -1;
}

int Count_Contents_Size(
	LCUI_WString *contents, 
	int *out_width, int *out_height, 
	int rows, int space, int linegap)
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
{
	if(rows <= 0) return -1;
	int i, j, default_height = 0, max_width = 0, width = 0, height = 0;
	for(i = 0; i < rows; ++i) {
		for(j=0; j < contents[i].size; ++j) {
			if(contents[i].string[j].bitmap.height > 0) {
				default_height = contents[i].string[j].bitmap.height;
				break;
			}
		}
		if(default_height > 0) break;
	}
	for(i = 0; i < rows; ++i) { 
		if(contents[i].size > 0) 
			height += contents[i].string[0].bitmap.height;
		else height += default_height;
		for(j = 0; j < contents[i].size; ++j)
			width += contents[i].string[j].bitmap.width; 
		if(contents[i].size > 0)
			width += (contents[i].size) * space; /* 每个字之间的间隔 */ 
		if(width > max_width) max_width = width;
		width = 0;/* 每一行要归零，重新开始计算长度 */
	}
	
	height += (rows - 1) * linegap; /* 每两行之间的行距 */ 
	*out_width = max_width;
	*out_height = height; 
	return 0;
}

int String_To_List(char *text , LCUI_WString **out_list)
/*
 * 功能：将原始的字符串分割成二维数组,字符编码为Unicode
 * 参数说明：
 * in_text  ： 传入的char型字符串
 * list     ： 输出的二维LCUI_Wchar_T指针
 * 返回值：字符串的行数
 * */
{ 
	LCUI_color_array colors;
	LCUI_RGB color = RGB(0,0,0);
	LCUI_WString *list = NULL;
	
	char *p = NULL, *str = NULL;
	
	wchar_t *wstr = NULL, *temp_wp = NULL;
	
	int w_len, tmp, len , j, i, z, k = 0 , start_x = 0;
	
	color_array_init(&colors);
	
	/* 如果文本为NULL */
	if(text == NULL) return 0;
	len = strlen(text);
	if(len <= 0) return 0;
	
	list = (LCUI_WString *)calloc(1, sizeof(LCUI_WString));
	/* i<=len,因为可能会读取字符串末尾的结束符 */
	for(i = 0; i <= len; ++i) {
		/* 如果遇到换行或者字符串结尾 */
		if(text[i] != '\n' && text[i] != 0)
			continue;
		p = text + start_x;
		str = (char*)calloc(strlen(p)+1,sizeof(char));
		/* 扩增内存 */
		list = (LCUI_WString *)realloc(list, 
				(k + 1) * sizeof(LCUI_WString));
		
		strcpy(str, p);
		str[i-start_x] = 0; /* 加上结束符 */
		/* 转换字符编码，及字符串类型，并保存 */
		w_len = Char_To_Wchar_T(str, &wstr); 
		/* 释放 */
		free(str);
		str = NULL;
		/* 分配内存 */
		list[k].string = (LCUI_WChar_T*)calloc((w_len+1), 
					sizeof(LCUI_WChar_T)); 
		/* j记录的是实际字符数，z记录的是除去<color=R,G,B></color>后的字符数 */
		for(j=0,z=0; j < w_len; ++j,++z) { 
			temp_wp = wstr + j; 
			if(wstrcmp(temp_wp, 7, "<color=") == 0) {
				temp_wp = temp_wp + 7; 
				/* 将“<color=” 和 “>” 之间的内容裁剪出来，并转换成RGB配色数据 */
				tmp = cut_wstr_convert_to_color(temp_wp, '>', &color);
				if(tmp > 0) {/* 如果成功 */
					j += (tmp + 7); 
					add_color(&colors, color);/* 添加颜色数据 */
				}
			} else if(get_color(&colors, &color) == 0 
			 && wstrcmp(temp_wp, 8, "</color>") == 0) {
				/* 如果能获取已储存的配色，并且，包含 </color> */
				j += 8; /* 位置向右移动8个字 */
				delete_color(&colors);/* 删除这个配色 */
				/* 如果超过这行的字数 */
				if(j >= w_len) break; 
				/* 
				 * temp_wp指针已经指向</color>后面的第一个字符，
				 * 继续下次循环，并判断这个字符及后面的字符是否包含
				 * 有<color=>或者</color>，这是为了能正常处理一
				 * 连串的<color=>或者</color>。
				 **/
				--z; --j;
				continue;
			}
			if(get_color(&colors, &color) == 0) 
			/* 如果有颜色数据 */
				list[k].string[z].color_type = CUSTOM; 
			else {/* 否则颜色为缺省 */
				color = RGB(0,0,0);
				list[k].string[z].color_type = DEFAULT;
			}
			/* 保存字符码 */
			list[k].string[z].char_code = wstr[j];
			/* 初始化字体位图数据 */
			FontBMP_Init(&list[k].string[z].bitmap);
			/* 保存配色 */
			list[k].string[z].color = color;
		}
		/* 保存字符串长度 */
		list[k].size = z;
		++k;
		start_x = i+1; /* 记录上个'\n'所在的位置 */
		free(wstr); /* 释放内存 */
		wstr = NULL;
		if(text[i] == 0) break;/* 遇到结束符果断退出！ */ 
	}
	*out_list = list; 
	free(colors.color);
	return k; 
}
/*************************** end color text ***************************/

static void Label_Init(LCUI_Widget *widget)
/* 功能：初始化label部件数据 */
{
	LCUI_Label *label;
	label = (LCUI_Label*)Widget_Create_PrivData(widget, sizeof(LCUI_Label));

	label->auto_size	= IS_TRUE; 				/* 开启自动调整尺寸 */
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
	label = (LCUI_Label*)Get_Widget_PrivData(widget);
	WString_Free(label->contents);
	label->contents = NULL;
	String_Free(&label->text);
}

static void Refresh_Label_FontBitmap(LCUI_Widget *widget)
/* 功能：刷新label部件内的字体位图 */
{ 
	int i, k;
	
	LCUI_Label *label = (LCUI_Label*)Get_Widget_PrivData(widget);  
	
	if(label->text.size <= 0)
		return;
		
	for(i = 0; i < label->rows; ++i)
	{/* 遍历每一行字符串中的每个字符 */ 
		for(k = 0; k < label->contents[i].size; ++k)
		{/* 获取字符位图 */ 
			Get_FontBMP(&label->font, 
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
	
	LCUI_Label *label = (LCUI_Label*)Get_Widget_PrivData(widget); 
	
	len = label->text.size; 
	
	if(len <= 0) {/* 如果内容为空 */
		/*如果开启了自动调整大小*/
		if(label->auto_size == IS_TRUE) {
			Resize_Widget(widget, Size(0, 0)); 
		}
		widget->graph.have_alpha = IS_TRUE;
		goto skip_fontbitmap_mix;
	} 
	
	LCUI_WString *new_list;
	int old_rows, new_rows; 
	wchar_t *new_code, *old_code;
	/* 分割成wchar_t型数据 */ 
	new_rows = String_To_List(label->text.string , &new_list); 
	/* 获取之前文本的行数 */
	old_rows = label->rows;
	
	if(old_rows > 0) {/* 改变内存空间大小 */
		label->contents = (LCUI_WString *)
				realloc(label->contents, 
				sizeof(LCUI_WString) * new_rows); 
	} else {
		label->contents = (LCUI_WString *)calloc(new_rows, 
				sizeof(LCUI_WString));
	}
	for(i = 0; i < new_rows; ++i) {/* 遍历每一行 */
		/* 改变这一行内容占的内存空间大小 */
		if(old_rows > i)
			label->contents[i].string = 
				(LCUI_WChar_T*)realloc(
				label->contents[i].string, 
				sizeof(LCUI_WChar_T) * 
				(new_list[i].size + 1));
		else {
			label->contents[i].string = 
				(LCUI_WChar_T*)malloc(
				sizeof(LCUI_WChar_T) * 
				(new_list[i].size + 1));
			label->contents[i].size = 0;
		}
		/* 遍历这行的字符串的每个元素 */
		for(k = 0; k < new_list[i].size; ++k) {
			if(new_list[i].string[k].color_type == DEFAULT) {
			/* 使用默认的颜色 */
				label->contents[i].string[k].color = label->font.fore_color;
			} else {  /* 否则使用自定义颜色 */
				label->contents[i].string[k].color = new_list[i].string[k].color; 
			}
			
			new_code = &new_list[i].string[k].char_code;
			old_code = &label->contents[i].string[k].char_code;
			/* 如果字符的位置还在label->contents[i]的范围内 */
			if(k < label->contents[i].size) {
				/* 如果文字不一样，就需要刷新该字 */
				if( old_code != new_code) {
					*old_code = *new_code; 
					/* 获取字符位图 */ 
					Get_FontBMP(&label->font, *old_code , 
						&label->contents[i].string[k].bitmap );
					label->contents[i].update = IS_TRUE;/* 这一行文本需要刷新 */ 
				}
			} else { 
				*old_code = *new_code; 
				/* 先初始化数据结构，然后获取字符位图 */
				FontBMP_Init(&label->contents[i].string[k].bitmap);
				Get_FontBMP(&label->font, *old_code , 
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
	|| max_height != widget->size.h)) {
		/* 如果开启了自动调整大小,并且尺寸有改变 */
		Resize_Widget(widget, Size(max_width, max_height + 2));
		Refresh_Widget(widget);
	}
		
skip_fontbitmap_mix:;
	
	int flag; 
	if(!Graph_Valid(&widget->background_image)) {
		flag = GRAPH_MIX_FLAG_REPLACE; /* 替换模式 */
	} else {
		flag = GRAPH_MIX_FLAG_OVERLAY; /* 叠加模式 */ 
	}
	/* 合成字体位图 */
	Mix_Widget_FontBitmap(widget, 1, 1, label->contents, label->rows, 
			label->font.space, label->font.linegap, flag);
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
	LCUI_Label *label = (LCUI_Label*)Get_Widget_PrivData(widget);
	
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
	
	label = (LCUI_Label*)Get_Widget_PrivData(widget);
	
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
	LCUI_Label *label = (LCUI_Label*)Get_Widget_PrivData(widget);
    
	int refresh_flag = 0;
	if(label->font.size != font_size) {
		label->font.size = font_size;
		refresh_flag = 1;
	}
	if(Strcmp(&label->font.font_file, font_file) != 0
	&& 0 == Open_Fontfile(&label->font, font_file) ) {
		Strcpy(&label->font.font_file, font_file);
		refresh_flag = 1;
	} 
	if(refresh_flag == 1) {/* 刷新标签内的字体位图数据 */ 
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
    
	LCUI_Label *label = (LCUI_Label*)Get_Widget_PrivData(widget);
    
	label->font.fore_color = color;

	if(label->contents == NULL || label->rows <= 0) return;
	
	for(i = 0; i < label->rows; ++i) {/* 遍历每一行 */
		for(k = 0; k < label->contents[i].size; ++k) {/* 遍历这行的字符串的每个元素 */
			/* 如果改变了label部件的默认的字体颜色，那么这些使用缺省颜色的文字将刷新 */
			if(label->contents[i].string[k].color_type == DEFAULT) 
				label->contents[i].string[k].color = label->font.fore_color;  
		}
		label->contents[i].update = IS_TRUE;
	}
	Draw_Widget(widget);/* 更新部件 */ 
}

void Register_Label()
/* 功能：注册部件类型-文本标签至部件库 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("label");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("label",	Label_Init,		FUNC_TYPE_INIT);
	WidgetFunc_Add("label",	Exec_Update_Label,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("label", Destroy_Label,		FUNC_TYPE_DESTROY);
}
