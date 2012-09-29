/* ***************************************************************************
 * bitmapfont.c -- The Bitmap Font operation set.
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
 * bitmapfont.c -- 位图字体的操作集
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
 
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_FONT_H
#include LC_MEM_H
#include LC_ERROR_H
#include LC_GRAPH_H

#include <iconv.h>

#include FT_GLYPH_H

extern uchar_t const *in_core_font_8x8();

int FontBMP_Valid(LCUI_FontBMP *bitmap)
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回1，无效返回0
 */
{
	if(NULL != bitmap && bitmap->width * bitmap->height > 0) {
		return 1; 
	}
	return 0;
}

void Print_FontBMP_Info(LCUI_FontBMP *bitmap)
/* 功能：打印位图的信息 */
{
	printf("address:%p\n",bitmap);
	if(bitmap == NULL) {
		return;
	}
	printf("width:%d,height:%d,alpha:%u\n", 
	bitmap->width, bitmap->height, bitmap->alpha);
}

void FontBMP_Init(LCUI_FontBMP *bitmap)
/* 初始化字体位图 */
{
	bitmap->width = 0;
	bitmap->height = 0;
	bitmap->data = NULL;
	bitmap->alpha = 255;
}

void FontBMP_Free(LCUI_FontBMP *bitmap)
/* 释放字体位图占用的资源 */
{
	if(FontBMP_Valid(bitmap)) {
		free(bitmap->data);
		FontBMP_Init(bitmap);
	}
}

void FontBMP_Create(LCUI_FontBMP *bitmap, int width, int height)
/* 功能：为Bitmap内的数据分配内存资源，并初始化 */
{
	int i;
	if(width < 0 || height < 0) {
		return;
	}
	if(FontBMP_Valid(bitmap)) {
		FontBMP_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->height = height;
	bitmap->data = (uchar_t**)malloc(bitmap->height*sizeof(uchar_t*));
	/* 为背景图的每一行分配内存 */
	for(i = 0; i < bitmap->height; ++i) { 
		bitmap->data[i] = (uchar_t*)calloc(1, 
				sizeof(uchar_t) * bitmap->width); 
	}
	bitmap->alpha  = 255; /* 字体位图默认不透明 */
}

void FontBMP_Resize(LCUI_FontBMP *bitmap, int width, int height)
/* 功能：更改位图的尺寸 */
{
	int i, j;
	/* 如果新尺寸不大于原来的尺寸 */
	if(width < bitmap->width || height < bitmap->height) {
		return;
	} 
	bitmap->data = (uchar_t**)realloc( bitmap->data, 
				height*sizeof(uchar_t*) );
	for(i = 0; i < height; ++i) {   /* 为背景图的每一行扩增内存 */
		bitmap->data[i] = (uchar_t*)realloc( bitmap->data[i], 
					sizeof(uchar_t) * width); 
		/* 把扩增的部分置零 */
		if(i < bitmap->height) {
			for(j = bitmap->width; j < width; ++j) {
				bitmap->data[i][j] = 0; 
			}
		} else {
			memset(bitmap->data[i], 0 , bitmap->width); 
		}
	} 
}

void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap)
/* 功能：根据字符编码，获取系统自带的字体位图 */
{
	int i,j, start;
	uchar_t const *p;
	
	p = in_core_font_8x8();
	FontBMP_Create(out_bitmap, 9, 9);/* 为位图分配内存，9x9的尺寸 */
	if(code < 256) {
		start = code * 8;
		for (i=start;i<start+8;++i) {
			for (j=0;j<8;++j) {/* 将数值转换成一行位图 */
				out_bitmap->data[i-start][7-j] = 
						(p[i]&(1<<j))?255:0;  
			}
		}
	} else {
		uchar_t error_bitmap[] = {
			1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1 
		};
		for (i=0;i<8;i++) {
			for (j=0;j<8;j++) {
				out_bitmap->data[i][j]= 
					(error_bitmap[i*8+j] > 0) ?255:0; 
			}
		}
	}
}

/* 默认的字体文件路径 */
static char default_font[1024] = LCUI_DEFAULT_FONTFILE;

void Set_Default_Font(char *fontfile)
/* 
 * 功能：设定默认的字体文件路径
 * 说明：需要在LCUI初始化前使用，因为LCUI初始化时会打开默认的字体文件
 *  */
{
	if(fontfile == NULL) {
		strcpy(default_font, "");
	} else {
		strcpy(default_font, fontfile);
	}
}

void LCUI_Font_Init(LCUI_Font *font)
/* 
 * 功能：初始化LCUI的Font结构体数据 
 * 说明：本函数在LCUI初始化时调用，LCUI_Font结构体中记录着字体相关的数据
 * */
{ 
	char *p;
	printf("loading fontfile...\n");/* 无缓冲打印内容 */
	font->type = DEFAULT;
	font->size = 12;
	font->fore_color.red = 0;
	font->fore_color.green = 0;
	font->fore_color.blue = 0;
	String_Init(&font->font_file);
	String_Init(&font->family_name);
	String_Init(&font->style_name);
	font->space = 1;
	font->linegap = 0;
	font->status = KILLED;
	//font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT;
	//font->load_flags = FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
	font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT; 
	font->render_mode = FT_RENDER_MODE_MONO;
	font->ft_lib = NULL;
	font->ft_face = NULL;
	/* 如果在环境变量中定义了字体文件路径，那就使用它 */
	p = getenv("LCUI_FONTFILE");
	if(p != NULL) {
		strcpy(default_font, p); 
	}
	/* 打开默认字体文件 */
	Open_Fontfile(&LCUI_Sys.default_font, default_font); 
}

void Font_Init(LCUI_Font *in)
/* 
 * 功能：初始化Font结构体数据
 * 说明：默认是继承系统的字体数据
 * */
{
	in->type = DEFAULT;   /* 字体类型为LCUI默认的 */
	in->size = 12;
	in->fore_color.red = 0;
	in->fore_color.green = 0;
	in->fore_color.blue = 0;
	String_Init(&in->font_file);
	String_Init(&in->family_name);
	String_Init(&in->style_name);
	if(LCUI_Sys.default_font.status == ACTIVE) {
		in->status = ACTIVE;
		LCUI_Strcpy(&in->family_name, &LCUI_Sys.default_font.family_name);
		LCUI_Strcpy(&in->style_name, &LCUI_Sys.default_font.style_name);
		LCUI_Strcpy(&in->font_file, &LCUI_Sys.default_font.font_file);
		in->ft_lib = LCUI_Sys.default_font.ft_lib;
		in->ft_face = LCUI_Sys.default_font.ft_face;
	} else {
		in->status = KILLED;
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
	in->space = 1;
	in->linegap = 0;
	in->load_flags = LCUI_Sys.default_font.load_flags;
	in->render_mode = LCUI_Sys.default_font.render_mode;
}

void Font_Free(LCUI_Font *in)
/* 功能：释放Font结构体数据占用的内存资源 */
{
	String_Free(&in->font_file);
	String_Free(&in->family_name);
	String_Free(&in->style_name);
	if(in->status == ACTIVE) { 
		in->status = KILLED;
		if(in->type == CUSTOM) { 
			FT_Done_Face(in->ft_face);
			FT_Done_FreeType(in->ft_lib);
		}
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
}

void LCUI_Font_Free()
/* 功能：释放LCUI默认的Font结构体数据占用的内存资源 */
{ 
	String_Free(&LCUI_Sys.default_font.font_file);
	String_Free(&LCUI_Sys.default_font.family_name);
	String_Free(&LCUI_Sys.default_font.style_name);
	if(LCUI_Sys.default_font.status == ACTIVE) {
		LCUI_Sys.default_font.status = KILLED;
		FT_Done_Face(LCUI_Sys.default_font.ft_face);
		FT_Done_FreeType(LCUI_Sys.default_font.ft_lib); 
		LCUI_Sys.default_font.ft_lib = NULL;
		LCUI_Sys.default_font.ft_face = NULL;
	}
}


int Show_FontBMP(LCUI_FontBMP *in_fonts)
/* 功能：在屏幕打印以0和1表示字体位图 */
{
	int x,y;
	for(y = 0;y < in_fonts->height; ++y){
		for(x = 0; x < in_fonts->width; ++x){
			if(in_fonts->data[y*in_fonts->width + x] > 0)
				printf("1");
			else printf("0");
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int FontBMP_Mix(
		LCUI_Graph	*back_graph,  /* 背景图形 */
		int		start_x,
		int		start_y,
		LCUI_FontBMP	*in_fonts,  /* 传入的字体位图数据 */
		LCUI_RGB	color,       /* 字体的配色 */ 
		int		flag
)
/* 功能：将字体位图数据与背景图形混合 */
{
	if(!FontBMP_Valid(in_fonts) || !Graph_Valid(back_graph)) {
		return -1;
	}
	
	float k;
	int count = 0, m;
	uchar_t j;
	LCUI_Rect read, write;/* 用于记录区域的范围的数据 */
	int x = 0, y = 0, end_x, end_y;//右下角的坐标
	int box_width, box_height, width, height;
	
	Rect_Init(&read);
	Rect_Init(&write);
	
	/* 保存图片的尺寸 */
	width = in_fonts->width;
	height = in_fonts->height;
	read.width = width;
	read.height = height;
	/* 获取背景的区域的尺寸 */
	box_width = back_graph->width;
	box_height = back_graph->height;
	/* 得到图像右下角点的坐标 */
	end_x = start_x + width;
	end_y = start_y + height;

	/* 如果图片尺寸超出窗口限定的尺寸，更改图片显示的区域 */
	if(start_x < 0) {/* 如果起点在x轴的坐标小于0 */
		read.x = 0 - start_x; /* 改变读取的区域的起点在x轴的坐标 */
		read.width = read.width - read.x;
		start_x = 0;
	}
	if(end_x > box_width) {/* 如果超过可显示区域 */
		read.width = read.width - (end_x - box_width);
		end_x = box_width;
	}
	if(start_y < 0) {
		read.y = 0 - start_y;
		read.height = read.height - read.y;
		start_y = 0;
	}
	if(end_y > box_height) {
		read.height = read.height - (end_y - box_height);
		end_y = box_height;
	}
	
	k = in_fonts->alpha / 255.0;
	/* 获取数据输出到背景的区域范围 */
	write.x = start_x;
	write.y = start_y;
	/* 开始读取图片中的图形数组并写入窗口 */
	switch(flag) {
		case GRAPH_MIX_FLAG_OVERLAY:
			for (y = 0; y < read.height; ++y) {
				m = (write.y + y) * back_graph->width + write.x; 
				for (x = 0; x < read.width; ++x) {
					count = m + x;/* 计算需填充至窗口的各点的坐标 */
					j = in_fonts->data[read.y + y][read.x + x] * k;
					back_graph->rgba[0][count] = (color.red * j + back_graph->rgba[0][count] * (255 - j)) /255;
					back_graph->rgba[1][count] = (color.green * j + back_graph->rgba[1][count] * (255 - j)) /255;
					back_graph->rgba[2][count] = (color.blue * j + back_graph->rgba[2][count] * (255 - j)) /255;
				}
			}
			break;
		case GRAPH_MIX_FLAG_REPLACE:
			for (y = 0; y < read.height; ++y) {
				m = (write.y + y) * back_graph->width + write.x; 
				for (x = 0; x < read.width; ++x) {
					count = m + x;/* 计算需填充至窗口的各点的坐标 */
					j = in_fonts->data[read.y + y][read.x + x] * k;
					if(j != 0) {
						back_graph->rgba[0][count] = color.red;
						back_graph->rgba[1][count] = color.green;
						back_graph->rgba[2][count] = color.blue;
						back_graph->rgba[3][count] = j;
					}
				}
			}
			break;
		default : flag = GRAPH_MIX_FLAG_REPLACE;
	}
	return 0;
}

int Open_Fontfile(LCUI_Font *font_data, char *fontfile)
/* 功能：打开字体文件，并保存数据至LCUI_Font结构体中 */
{
	int type;
	FT_Library    library;
	FT_Face       face;
	FT_Error      face_error = 0, lib_error = 0;
	
	type = font_data->type;
	if(font_data->status == ACTIVE) {
		if(fontfile == NULL 
		|| Strcmp(&font_data->font_file, fontfile) == 0) {
			return 0;
		} else if( Strcmp(&font_data->font_file, 
				LCUI_Sys.default_font.font_file.string)) {
			type = CUSTOM;
		}
	}
	else if(fontfile == NULL) {
		return -1;
	}
	lib_error = FT_Init_FreeType( & library);  /* 初始化FreeType库 */
	if (lib_error) { /* 当初始化库时发生了一个错误 */
		printf("open fontfile: "FT_INIT_ERROR);
		return - 1 ;
	}
	
	face_error = FT_New_Face( library, fontfile , 0 , & face );
	if(face_error) {
		FT_Done_FreeType(library);
		if ( face_error == FT_Err_Unknown_File_Format )
			printf("open fontfile: "FT_UNKNOWN_FILE_FORMAT); /* 未知文件格式 */ 
		else 
			printf("open fontfile: "FT_OPEN_FILE_ERROR);/* 打开错误 */
		perror(fontfile);
		return -1;
	}
	printf(
		"=============== font info ==============\n" 
		"family name: %s\n"
		"style name : %s\n"
		"========================================\n" ,
		face->family_name,
		face->style_name
	);
	Font_Free(font_data);
	/* 保存字体信息 */
	Strcpy(&font_data->family_name, face->family_name);
	Strcpy(&font_data->style_name, face->style_name);
	Strcpy(&font_data->font_file, fontfile);
	font_data->type = type;
	font_data->status = ACTIVE; 
	font_data->ft_lib = library;
	font_data->ft_face = face;
	return 0;
}

int Get_FontBMP(LCUI_Font *font_data, wchar_t ch, LCUI_FontBMP *out_bitmap)
/*
 * 功能：获取单个wchar_t型字符的位图
 * 说明：LCUI_Font结构体中储存着已被打开的字体文件句柄和face对象的句柄，如果字体文件已经被
 * 成功打开一次，此函数不会再次打开字体文件。
 */
{ 
	FT_Face         p_FT_Face = NULL;   /* face对象的句柄 */
	FT_Bitmap       bitmap;         
	FT_BitmapGlyph  bitmap_glyph;    
	FT_Glyph        glyph;
	FT_GlyphSlot    slot;               /* 字形槽的句柄 */
	FT_Error        error = 0;
	int i , j = 0, value = 0;
	int k, start_y = 0, ch_height = 0, ch_width = 0;
	
	DEBUG_MSG("Get_FontBMP():char: %d\n", ch);
	if(font_data != NULL) {
	 /* 如果LCUI_Font结构体中的字体信息有效，就打开结构体中的指定的字体文件，并
	  * 将字体文件和face对象的句柄保存至结构体中。
	  * 当然，如果LCUI_Font结构体有字体文件和face对象的句柄，就直接返回0。
	  */
		if(font_data->ft_face == NULL 
		|| font_data->ft_lib == NULL ) { 
			error = Open_Fontfile(font_data, 
					font_data->font_file.string);
			if(error) {
				value = -1; 
			}
		}
		p_FT_Face = font_data->ft_face; 
	} else {
		return -1;
	}
	
	/* 不能获取字体文件，就使用内置的8x8点阵字体 */
	if(value != 0) {
		if(ch == ' ') { /* 如果是空格 */
			/* 自定义的函数，用于获取8x8点阵字体位图 */
			Get_Default_FontBMP('a', out_bitmap);
			for (i=0;i<8;i++)  
			for (j=0;j<8;j++)  
				out_bitmap->data[i][j]= 0; 
		} else { 
			FontBMP_Create(out_bitmap, 8, 8);
			Get_Default_FontBMP(ch, out_bitmap);
		}
		return 0;
	}
	/* 如果能正常打开字体文件 */
	FT_Select_Charmap(p_FT_Face, FT_ENCODING_UNICODE);   /* 设定为UNICODE，默认的也是 */
	FT_Set_Pixel_Sizes(p_FT_Face, 0, font_data->size);   /* 设定字体大小 */
	slot = p_FT_Face->glyph;
	if(ch == ' ') { /* 如果有空格,它的宽度就以字母a的宽度为准 */
		error = FT_Load_Char( p_FT_Face, 'a', font_data->load_flags); 
		if(error) {
			return error; 
		}
		
		error = FT_Get_Glyph(p_FT_Face -> glyph, &glyph);
		if(error) {
			return error; 
		}
		
		FontBMP_Free(out_bitmap);
		/* 背景图形的高度，这个高度要大于字体的高度，所以是+3 */
		/* 256级灰度字形转换成位图 */
		FT_Glyph_To_Bitmap(&glyph, font_data->render_mode, 0 ,1);
		/* FT_RENDER_MODE_NORMAL 这是默认渲染模式，它对应于8位抗锯齿位图 */
		bitmap_glyph = (FT_BitmapGlyph)glyph;
		bitmap       = bitmap_glyph -> bitmap;
		/* 字体所在的背景图的尺寸需要大一点 */
		out_bitmap->height = font_data->size + 3;
		FontBMP_Create(out_bitmap, bitmap.width, out_bitmap->height);
		/* 释放字形占用的内存 */
		FT_Done_Glyph(glyph);
		glyph = NULL; 
		return 0;
	}
	/* 这个函数只是简单地调用FT_Get_Char_Index和FT_Load_Glyph */
	error = FT_Load_Char( p_FT_Face, ch, font_data->load_flags);
	if(error) {
		return error; 
	}
	
	/* 从插槽中提取一个字形图像 
	 * 请注意，创建的FT_Glyph对象必须与FT_Done_Glyph成对使用 */
	error = FT_Get_Glyph(p_FT_Face->glyph, &glyph);
	if(error) {
		return error; 
	}
	
	int bg_height;
	FontBMP_Free(out_bitmap);
	/* 背景图形的高度，这个高度要大于字体的高度，所以是+4 */
	bg_height = font_data->size + 4; 
	/* 256级灰度字形转换成位图 */
	FT_Glyph_To_Bitmap(&glyph, font_data->render_mode, 0 ,1);
	/* FT_RENDER_MODE_NORMAL 是默认渲染模式，它对应于8位抗锯齿位图。 */
	bitmap_glyph = (FT_BitmapGlyph)glyph;
	bitmap       = bitmap_glyph -> bitmap;
	k = 0;
	/* 获取起点的y轴坐标 */
	start_y = font_data->size - slot->bitmap_top; 
	ch_width = bitmap.width;
	/* 处理字体位图在背景图中的范围 */
	if(start_y < 0) {
		start_y = 0; 
	}
	if(bitmap.rows > bg_height) {
		ch_height = font_data->size; 
	} else {
		ch_height = bitmap.rows; 
	}
		
	if(ch_height + start_y > bg_height) {
		ch_height = bg_height - start_y; 
	}
	/* 开辟内存空间,如果出现问题，会导致FT_Done_Glyph函数出现段错误 */
	FontBMP_Create(out_bitmap, ch_width, bg_height);
	/* 开始将字体位图贴到背景图形中 */
	for(i = 0; i < ch_height; ++i) 
	for(j = 0;j < ch_width; ++j) {
		switch (bitmap.pixel_mode) {
		    case FT_PIXEL_MODE_GRAY:
			/* 一个8位位图，一般用来表示反锯齿字形图像。每个像素用一个字节存储 */
			out_bitmap->data[start_y + i][j] = bitmap.buffer[k];
			break;
		    case FT_PIXEL_MODE_MONO: 
			/* 一个单色位图,每个bit对应一个点,非黑即白 */
			out_bitmap->data[start_y + i][j] = bitmap.buffer[k]?255:0;
			break;
		}
		++k;
	} 
	/* 释放字形占用的内存 */
	FT_Done_Glyph(glyph);
	glyph = NULL; 
	
	//Print_FontBMP_Info(out_bitmap);
	return 0;
}


