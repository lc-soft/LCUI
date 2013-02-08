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
#include "config.h"
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_FONT_H
#include LC_MEM_H
#include LC_ERROR_H
#include LC_GRAPH_H

#include <iconv.h>

#ifdef USE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#endif

BOOL FontBMP_Valid(LCUI_FontBMP *bitmap)
/*
 * 功能：检测位图数据是否有效
 * 返回值：有效返回帧，无效返回假
 */
{
	if( bitmap && bitmap->width > 0 && bitmap->rows > 0) {
		return TRUE; 
	}
	return FALSE;
}

void Print_FontBMP_Info(LCUI_FontBMP *bitmap)
/* 功能：打印位图的信息 */
{
	printf("address:%p\n",bitmap);
	if( !bitmap ) {
		return;
	}
	printf("top: %d, left: %d, width:%d, rows:%d\n", 
	bitmap->top, bitmap->left, bitmap->width, bitmap->rows);
}

void FontBMP_Init(LCUI_FontBMP *bitmap)
/* 初始化字体位图 */
{
	bitmap->rows = 0;
	bitmap->width = 0;
	bitmap->top = 0;
	bitmap->left = 0; 
	bitmap->num_grays = 0;
	bitmap->pixel_mode = 0;
	bitmap->buffer = NULL; 
}

void FontBMP_Free(LCUI_FontBMP *bitmap)
/* 释放字体位图占用的资源 */
{
	if(FontBMP_Valid(bitmap)) {
		free(bitmap->buffer);
		FontBMP_Init(bitmap);
	}
}

int FontBMP_Create(LCUI_FontBMP *bitmap, int width, int rows)
/* 功能：创建字体位图 */
{ 
	size_t size;
	if(width < 0 || rows < 0) {
		FontBMP_Free(bitmap);
		return -1;
	}
	if(FontBMP_Valid(bitmap)) {
		FontBMP_Free(bitmap);
	}
	bitmap->width = width;
	bitmap->rows = rows;
	size = width*rows*sizeof(uchar_t);
	bitmap->buffer = (uchar_t*)malloc( size );
	if( bitmap->buffer == NULL ) {
		return -2;
	}
	return 0;
}

void Get_Default_FontBMP(unsigned short code, LCUI_FontBMP *out_bitmap)
/* 功能：根据字符编码，获取已内置的字体位图 */
{
	int i,j, start, m;
	uchar_t const *p;
	
	p = in_core_font_8x8(); /* 获取指向内置字体位图数组的指针 */
	FontBMP_Create(out_bitmap, 8, 8); /* 为位图分配内存，8x8的尺寸 */
	out_bitmap->left = 0;
	out_bitmap->top = 0;
	if(code < 256) { /* 如果在ASCII编码范围内 */
		if(code == ' ') { /* 空格就直接置0 */
			memset(out_bitmap->buffer, 0, sizeof(uchar_t)*64); 
		} else {
			start = code * 8;
			for (i=start;i<start+8;++i) {
				m = (i-start) * 8 + 7;
				/* 将数值转换成一行位图 */
				for (j=0;j<8;++j,--m) {
					out_bitmap->buffer[m] 
					 = (p[i]&(1<<j))?255:0;  
				}
			}
		}
	} else { /* 否则用“口”表示未知编码字符 */
		static uchar_t null_bmp[] = {
			1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1 };
		for (i=0;i<8;i++) {
			m = i*8;
			for (j=0;j<8;j++,++m) {
				out_bitmap->buffer[m] = (null_bmp[m]>0)?255:0; 
			}
		}
	}
	/* 修改相关属性 */
	out_bitmap->top = 8;
	out_bitmap->left = 0;
	out_bitmap->rows = 8;
	out_bitmap->width = 8;
	out_bitmap->pixel_mode = 0;
	out_bitmap->num_grays = 1;
	out_bitmap->advance.x = 8;
	out_bitmap->advance.y = 8;
}

/* 默认的字体文件路径 */
static char default_font[1024] = LCUI_DEFAULT_FONTFILE;

void Set_Default_Font(char *fontfile)
/* 
 * 功能：设定默认的字体文件路径
 * 说明：需要在LCUI初始化前使用，因为LCUI初始化时会打开默认的字体文件
 *  */
{
	if( !fontfile ) {
		strcpy(default_font, "");
	} else {
		strcpy(default_font, fontfile);
	}
}

/* 初始化字体处理模块 */
void LCUIModule_Font_Init( void )
{
	char *p;
	LCUI_Font *font;
	
	font = &LCUI_Sys.default_font;
	printf("loading fontfile...\n");/* 无缓冲打印内容 */
	font->type = DEFAULT;
	String_Init( &font->font_file );
	String_Init( &font->family_name );
	String_Init( &font->style_name );
	font->state = KILLED;
#ifdef USE_FREETYPE
	//font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT;
	//font->load_flags = FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
	font->load_flags = FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT; 
	font->render_mode = FT_RENDER_MODE_MONO;
#else
	font->load_flags = 0;
	font->render_mode = 0;
#endif
	font->ft_lib = NULL;
	font->ft_face = NULL;
	/* 如果在环境变量中定义了字体文件路径，那就使用它 */
	p = getenv("LCUI_FONTFILE");
	if( p ) {
		strcpy(default_font, p); 
	}
	/* 打开默认字体文件 */
	Open_Fontfile(&LCUI_Sys.default_font, default_font);
	FontLIB_Init();
}

/* 停用字体处理模块 */
void LCUIModule_Font_End( void )
{
	/* 释放字符串 */
	String_Free( &LCUI_Sys.default_font.font_file );
	String_Free( &LCUI_Sys.default_font.family_name );
	String_Free( &LCUI_Sys.default_font.style_name );
	/* 如果缺省字体的状态是活动的，那就拷贝 */
	if(LCUI_Sys.default_font.state == ACTIVE) {
		LCUI_Sys.default_font.state = KILLED;
#ifdef USE_FREETYPE
		FT_Done_Face(LCUI_Sys.default_font.ft_face);
		FT_Done_FreeType(LCUI_Sys.default_font.ft_lib); 
#endif
		LCUI_Sys.default_font.ft_lib = NULL;
		LCUI_Sys.default_font.ft_face = NULL;
	}
	FontLIB_DestroyAll();
}

void Font_Init(LCUI_Font *in)
/* 
 * 功能：初始化Font结构体数据
 * 说明：默认是继承系统的字体数据
 * */
{
	/* 字体类型为LCUI默认的 */
	in->type = DEFAULT;
	/* 初始化字符串 */
	String_Init(&in->font_file);
	String_Init(&in->family_name);
	String_Init(&in->style_name);
	/* 如果缺省字体的状态是活动的，那就拷贝 */
	if(LCUI_Sys.default_font.state == ACTIVE) {
		in->state = ACTIVE;
		LCUI_Strcpy(&in->family_name, &LCUI_Sys.default_font.family_name);
		LCUI_Strcpy(&in->style_name, &LCUI_Sys.default_font.style_name);
		LCUI_Strcpy(&in->font_file, &LCUI_Sys.default_font.font_file);
		in->ft_lib = LCUI_Sys.default_font.ft_lib;
		in->ft_face = LCUI_Sys.default_font.ft_face;
	} else {
		in->state = KILLED;
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
	in->load_flags = LCUI_Sys.default_font.load_flags;
	in->render_mode = LCUI_Sys.default_font.render_mode;
}

void Font_Free(LCUI_Font *in)
/* 功能：释放Font结构体数据占用的内存资源 */
{
	String_Free(&in->font_file);
	String_Free(&in->family_name);
	String_Free(&in->style_name);
	if(in->state == ACTIVE) { 
		in->state = KILLED;
		if(in->type == CUSTOM) { 
#ifdef USE_FREETYPE
			FT_Done_Face(in->ft_face);
			FT_Done_FreeType(in->ft_lib);
#endif
		}
		in->ft_lib = NULL;
		in->ft_face = NULL;
	}
}

int Show_FontBMP(LCUI_FontBMP *fontbmp)
/* 功能：在屏幕打印以0和1表示字体位图 */
{
	int x,y,m;
	for(y = 0;y < fontbmp->rows; ++y){
		m = y*fontbmp->width;
		for(x = 0; x < fontbmp->width; ++x,++m){ 
			if(fontbmp->buffer[m] > 128) {
				printf("#");
			} else if(fontbmp->buffer[m] > 64) {
				printf("-");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int FontBMP_Mix( LCUI_Graph	*graph, LCUI_Pos	des_pos,
		LCUI_FontBMP	*bitmap, LCUI_RGB	color,
		int flag )
/* 功能：将字体位图绘制到背景图形上 */
{
	LCUI_Graph *des;
	LCUI_Rect des_rect, cut;
	uint_t total, m, n, y;
	uint_t src_start_pos, des_start_pos;
	/* 获取背景图形的有效区域 */
	des_rect = Get_Graph_Valid_Rect( graph );
	/* 获取背景图引用的源图形 */
	des = Get_Quote_Graph( graph ); 
	/* 数据有效性检测 */
	if( !FontBMP_Valid( bitmap ) || !Graph_Valid( graph ) ) {
		return -1;
	}
	/* 起点位置的有效性检测 */
	if(des_pos.x > des->width || des_pos.y > des->height) {
		return -2;
	}
	/* 获取需要裁剪的区域 */
	if( Get_Cut_Area( Size( des_rect.width, des_rect.height ),
		Rect( des_pos.x, des_pos.y, bitmap->width, bitmap->rows ),
		&cut
	)) {
		des_pos.x += cut.x;
		des_pos.y += cut.y;
	}
	
	/* 如果是以叠加模式绘制字体位图 */
	if( flag == GRAPH_MIX_FLAG_OVERLAY ) {
		/* 预先计算起点位置 */
		src_start_pos = cut.y * bitmap->width + cut.x;
		des_start_pos = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
		for (y = 0; y < cut.height; ++y) {
			m = src_start_pos;
			n = des_start_pos;
			total = n + cut.width;
			for (; n < total; ++n,++m) { 
				/* 获取通过ALPHA混合后的像素点的数据 */
				des->rgba[0][n] = ALPHA_BLENDING(color.red, des->rgba[0][n], bitmap->buffer[m]);
				des->rgba[1][n] = ALPHA_BLENDING(color.green, des->rgba[1][n], bitmap->buffer[m]);
				des->rgba[2][n] = ALPHA_BLENDING(color.blue, des->rgba[2][n], bitmap->buffer[m]);
			}
			/* 切换至下一行像素点 */
			des_start_pos += des->width;
			src_start_pos += bitmap->width;
		}
	} else { /* 否则是以覆盖模式 */
		src_start_pos = cut.y * bitmap->width + cut.x;
		des_start_pos = (des_pos.y + des_rect.y) * des->width + des_pos.x + des_rect.x;
		for (y = 0; y < cut.height; ++y) {
			m = src_start_pos;
			n = des_start_pos;
			total = n + cut.width;
			for (; n < total; ++n,++m) {
				des->rgba[0][n] = color.red;
				des->rgba[1][n] = color.green;
				des->rgba[2][n] = color.blue;
				des->rgba[3][n] = bitmap->buffer[m]; 
			} 
			des_start_pos += des->width;
			src_start_pos += bitmap->width;
		}
	}
	return 0;
}

int Open_Fontfile(LCUI_Font *font_data, char *fontfile)
/* 打开指定路径中的字体文件，并保存数据至LCUI_Font结构体中 */
{
#ifdef USE_FREETYPE
	int		type;
	FT_Library	library;
	FT_Face		face;
	FT_Error	face_error = 0, lib_error = 0;
	
	type = font_data->type;
	if(font_data->state == ACTIVE) {
		/* 如果字体文件路径无效，或该路径和默认的字体文件路径一样，则退出函数 */
		if( !fontfile || !Strcmp(&font_data->font_file, fontfile) ) {
			return 0;
		}
		else if( Strcmp(&font_data->font_file, 
				LCUI_Sys.default_font.font_file.string)) {
			/* 否则，如果不一样，就将type赋值为CUSTOM，表示自定义 */
			type = CUSTOM;
		}
	}
	else if( !fontfile ) {
		return -1;
	}
	/* 初始化FreeType库 */
	lib_error = FT_Init_FreeType( & library);
	/* 当初始化库时发生了一个错误 */
	if (lib_error) {
		printf("open fontfile: "FT_INIT_ERROR);
		return - 1 ;
	}
	
	face_error = FT_New_Face( library, fontfile , 0 , &face );
	if(face_error) {
		FT_Done_FreeType(library);
		if ( face_error == FT_Err_Unknown_File_Format ) {
			/* 未知文件格式 */ 
			printf("open fontfile: "FT_UNKNOWN_FILE_FORMAT);
		} else  {
			/* 打开错误 */
			printf("open fontfile: "FT_OPEN_FILE_ERROR);
		}
		/* 打印错误信息 */
		perror(fontfile);
		return -1;
	}
	/* 打印字体信息 */
	printf(	"=============== font info ==============\n" 
		"family name: %s\n"
		"style name : %s\n"
		"========================================\n" ,
		face->family_name,
		face->style_name );
	/* 先处理掉之前保存的字体信息 */
	Font_Free( font_data );
	/* 保存新的字体信息 */
	Strcpy(&font_data->family_name, face->family_name);
	Strcpy(&font_data->style_name, face->style_name);
	Strcpy(&font_data->font_file, fontfile);
	font_data->type = type;
	font_data->state = ACTIVE;
	font_data->ft_lib = library;
	font_data->ft_face = face;
	return 0;
#else
	printf("warning: not font engine support!\n");
	return -1;
#endif
}

/* 如果定义了USE_FREETYPE宏定义，则使用FreeType字体引擎处理字体数据 */
#ifdef USE_FREETYPE

static int 
Convert_FTGlyph( LCUI_FontBMP *des, FT_GlyphSlot slot, int render_mode )
/* 转换FT_GlyphSlot类型数据为LCUI_FontBMP */
{
	static FT_Error	error;
	static size_t		size;
	static FT_BitmapGlyph  bitmap_glyph;
	static FT_Glyph        glyph;
	
	/* 从字形槽中提取一个字形图像 
	 * 请注意，创建的FT_Glyph对象必须与FT_Done_Glyph成对使用 */
	error = FT_Get_Glyph( slot, &glyph );
	if(error) {
		return -1; 
	}
	/*---------------------- 打印字体信息 --------------------------
	printf(" width= %ld,  met->height= %ld\n"
	"horiBearingX = %ld, horiBearingY = %ld, horiAdvance = %ld\n"
	"vertBearingX = %ld, vertBearingY = %ld,  vertAdvance = %ld\n", 
	slot->metrics.width>>6, slot->metrics.height>>6,
	slot->metrics.horiBearingX>>6, slot->metrics.horiBearingY>>6, 
	slot->metrics.horiAdvance>>6, slot->metrics.vertBearingX>>6, 
	slot->metrics.vertBearingY>>6, slot->metrics.vertAdvance>>6 ); 
	------------------------------------------------------------*/
	if ( glyph->format != FT_GLYPH_FORMAT_BITMAP ) {
		error = FT_Glyph_To_Bitmap(&glyph, render_mode, 0 ,1);
		if(error) {
			return -1;
		}
	}
	bitmap_glyph = (FT_BitmapGlyph)glyph;
	/*
	 * FT_Glyph_Metrics结构体中保存字形度量，通过face->glyph->metrics结
	 * 构访问，可得到字形的宽、高、左边界距、上边界距、水平跨距等等。
	 * 注意：因为不是所有的字体都包含垂直度量，当FT_HAS_VERTICAL为假时，
	 * vertBearingX，vertBearingY和vertAdvance的值是不可靠的，目前暂不考虑
	 * 此情况的处理。
	 * */
	des->top = bitmap_glyph->top;
	des->left = slot->metrics.horiBearingX>>6;
	des->rows = bitmap_glyph->bitmap.rows;
	des->width = bitmap_glyph->bitmap.width;
	des->pixel_mode = bitmap_glyph->bitmap.pixel_mode;
	des->num_grays = bitmap_glyph->bitmap.num_grays;
	des->advance.x = slot->metrics.horiAdvance>>6;	/* 水平跨距 */
	des->advance.y = slot->metrics.vertAdvance>>6;	/* 垂直跨距 */
	/* 分配内存，用于保存字体位图 */
	size = des->rows * des->width * sizeof(uchar_t);
	des->buffer = malloc( size );
	if( !des->buffer ) {
		FT_Done_Glyph(glyph);
		return -1;
	}
	/* 拷贝至该内存空间内 */
	memcpy( des->buffer, bitmap_glyph->bitmap.buffer, size ); 
	FT_Done_Glyph(glyph);
	return size;
}

#endif


static int 
Get_NewFontBMP(	LCUI_Font *font_data, wchar_t ch, 
		int pixel_size, LCUI_FontBMP *out_bitmap)
/*
 * 功能：获取单个wchar_t型字符的字体位图数据
 * 说明：LCUI_Font结构体中储存着已被打开的字体文件句柄和face对象的句柄，如果字体文件
 * 已经被成功打开一次，此函数不会再次打开字体文件。
 */
{
#ifdef USE_FREETYPE
	size_t size;
	BOOL have_space = FALSE;
	
	FT_Face		p_FT_Face = NULL;   /* face对象的句柄 */ 
	FT_Error	error;
	
	if( font_data && font_data->ft_face ) {
		p_FT_Face = font_data->ft_face; 
	} else {
		Get_Default_FontBMP( ch, out_bitmap );
		return -1;
	}
	
	/* 设定为UNICODE，默认的也是 */
	FT_Select_Charmap( p_FT_Face, FT_ENCODING_UNICODE ); 
	/* 设定字体尺寸 */
	FT_Set_Pixel_Sizes( p_FT_Face, 0, pixel_size );
	/* 如果是空格 */
	if( ch == ' ' ) {
		ch = 'a';
		have_space = TRUE;
	}
	/* 这个函数只是简单地调用FT_Get_Char_Index和FT_Load_Glyph */
	error = FT_Load_Char( p_FT_Face, ch, font_data->load_flags);
	if(error) {
		return error; 
	}
	size = Convert_FTGlyph( out_bitmap, p_FT_Face->glyph, font_data->render_mode );
	/* 如果是空格则将位图内容清空 */
	if( have_space ) {
		memset( out_bitmap->buffer, 0, size );
	}
	return 0;
#else
	Get_Default_FontBMP( ch, out_bitmap );
	return -1;
#endif
}

/* 获取现有的字体位图数据 */
LCUI_FontBMP *
Get_ExistFontBMP( LCUI_Font *font_data, wchar_t ch, int pixel_size )
{
	LCUI_FontBMP *font_bmp, bmp_buff;
	int error, family_id, style_id;

#ifdef USE_FREETYPE
	if( !font_data ) {
		goto get_default;
	} else {
		if( !font_data->ft_face || !font_data->ft_lib ) { 
			error = Open_Fontfile( font_data, 
					font_data->font_file.string );
					
			if( error ) {
				goto get_default;
			}
		}
	}
	family_id = FontLIB_GetFamliyID( font_data->family_name.string );
	style_id = FontLIB_GetStyleID( font_data->style_name.string );
	goto skip_get_default;
	
get_default:;
	family_id = FontLIB_GetFamliyID( "default" );
	style_id = FontLIB_GetStyleID( "default" );
	
skip_get_default:;
#else
	family_id = FontLIB_GetFamliyID( "defalut" );
	style_id = FontLIB_GetStyleID( "defalut" );
#endif
	font_bmp = FontLIB_GetFontBMP( ch, family_id, style_id, pixel_size );
	if( !font_bmp ) {
		Get_NewFontBMP( font_data, ch, pixel_size, &bmp_buff );
		font_bmp = FontLIB_Add( ch, family_id, style_id, 
					pixel_size, &bmp_buff );
	}
	return font_bmp;
}
