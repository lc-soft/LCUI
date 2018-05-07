/*
 * freetype.c -- The FreeType font-engine support module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <LCUI_Build.h>
#ifdef LCUI_FONT_ENGINE_FREETYPE
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <stdlib.h>
#include <errno.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

#define LCUI_FONT_RENDER_MODE	FT_RENDER_MODE_NORMAL
#define LCUI_FONT_LOAD_FALGS	(FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)

static struct {
	FT_Library library;
} freetype;

static int FreeType_Open( const char *filepath, LCUI_Font **outfonts )
{
	FT_Face face;
	LCUI_Font font, *fonts;
	int i, err, num_faces;

	err = FT_New_Face( freetype.library, filepath, -1, &face );
	if( err ) {
		*outfonts = NULL;
		return -1;
	}
	num_faces = face->num_faces;
	FT_Done_Face( face );
	if( num_faces < 1 ) {
		return 0;
	}
	fonts = malloc( sizeof(LCUI_FontRec*) * num_faces );
	if( !fonts ) {
		return -ENOMEM;
	}
	for( i = 0; i < num_faces; ++i ) {
		err = FT_New_Face( freetype.library, filepath, i, &face );
		if( err ) {
			fonts[i] = NULL;
			continue;
		}
		FT_Select_Charmap( face, FT_ENCODING_UNICODE );
		font = Font( face->family_name, face->style_name );
		font->data = face;
		fonts[i] = font;
	}
	*outfonts = fonts;
	return num_faces;
}

static void FreeType_Close( void *face )
{
	FT_Done_Face( face );
}

/** 转换 FT_GlyphSlot 类型数据为 LCUI_FontBitmap */
static size_t Convert_FTGlyph( LCUI_FontBitmap *bmp, FT_GlyphSlot slot, int mode )
{
	int error;
	size_t size;
	FT_BitmapGlyph bitmap_glyph;
	FT_Glyph  glyph;

	/* 从字形槽中提取一个字形图像
	 * 请注意，创建的FT_Glyph对象必须与FT_Done_Glyph成对使用 */
	error = FT_Get_Glyph( slot, &glyph );
	if(error) {
		return -1;
	}
	/*---------------------- 打印字体信息 --------------------------
	LOG(" width= %ld,  met->height= %ld\n"
	"horiBearingX = %ld, horiBearingY = %ld, horiAdvance = %ld\n"
	"vertBearingX = %ld, vertBearingY = %ld,  vertAdvance = %ld\n",
	slot->metrics.width>>6, slot->metrics.height>>6,
	slot->metrics.horiBearingX>>6, slot->metrics.horiBearingY>>6,
	slot->metrics.horiAdvance>>6, slot->metrics.vertBearingX>>6,
	slot->metrics.vertBearingY>>6, slot->metrics.vertAdvance>>6 );
	------------------------------------------------------------*/
	if ( glyph->format != FT_GLYPH_FORMAT_BITMAP ) {
		error = FT_Glyph_To_Bitmap(&glyph, mode, 0 ,1);
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
	bmp->top = bitmap_glyph->top;
	bmp->left = slot->metrics.horiBearingX>>6;
	bmp->rows = bitmap_glyph->bitmap.rows;
	bmp->width = bitmap_glyph->bitmap.width;
	bmp->advance.x = slot->metrics.horiAdvance>>6;	/* 水平跨距 */
	bmp->advance.y = slot->metrics.vertAdvance>>6;	/* 垂直跨距 */
	/* 分配内存，用于保存字体位图 */
	size = bmp->rows * bmp->width * sizeof(uchar_t);
	bmp->buffer = (uchar_t*)malloc( size );
	if( !bmp->buffer ) {
		FT_Done_Glyph(glyph);
		return -1;
	}

	switch( bitmap_glyph->bitmap.pixel_mode ) {
	    /* 8位灰度位图，直接拷贝 */
	    case FT_PIXEL_MODE_GRAY:
		memcpy( bmp->buffer, bitmap_glyph->bitmap.buffer, size );
		break;
	    /* 单色点阵图，需要转换 */
	    case FT_PIXEL_MODE_MONO: {
		FT_Bitmap bitmap;
		FT_Int x, y;
		uchar_t *bit_ptr, *byte_ptr;

		FT_Bitmap_New( &bitmap );
		/* 转换位图bitmap_glyph->bitmap至bitmap，1个像素占1个字节 */
		FT_Bitmap_Convert( freetype.library, &bitmap_glyph->bitmap, &bitmap, 1 );
		bit_ptr = bitmap.buffer;
		byte_ptr = bmp->buffer;
		for( y=0; y<bmp->rows; ++y ) {
			for( x=0; x<bmp->width; ++x ) {
				*byte_ptr = *bit_ptr ? 255:0;
				++byte_ptr, ++bit_ptr;
			}
		}
		FT_Bitmap_Done( freetype.library, &bitmap );
		break;
	    }
	    /* 其它像素模式的位图，暂时先直接填充255，等需要时再完善 */
	    default:
		memset( bmp->buffer, 255, size );
		break;
	}
	FT_Done_Glyph( glyph );
	return size;
}

static int FreeType_Render( LCUI_FontBitmap *bmp, wchar_t ch,
			    int pixel_size, LCUI_Font font )
{
	int ret = 0;
	FT_UInt index;
	FT_Face ft_face = (FT_Face)font->data;

	/* 设定字体尺寸 */
	FT_Set_Pixel_Sizes( ft_face, 0, pixel_size );
	index = FT_Get_Char_Index( ft_face, ch );
	if( index == 0 ) {
		ret = -1;
	}
	/* 载入该字的字形数据 */
	if( FT_Load_Glyph( ft_face, index, LCUI_FONT_LOAD_FALGS ) != 0 ) {
		return -2;
	}
	Convert_FTGlyph( bmp, ft_face->glyph, LCUI_FONT_RENDER_MODE );
	return ret;
}

int LCUIFont_InitFreeType( LCUI_FontEngine *engine )
{
	if( FT_Init_FreeType(&freetype.library) ) {
		return -1;
	}
	strcpy( engine->name, "FreeType" );
	engine->render = FreeType_Render;
	engine->open = FreeType_Open;
	engine->close = FreeType_Close;
	return 0;
}

int LCUIFont_ExitFreeType( void )
{
	FT_Done_FreeType( freetype.library );
	return 0;
}

#endif
