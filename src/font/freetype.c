/* ***************************************************************************
 * freetype.c -- The FreeType font-engine support module.
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * freetype.c -- FreeType 字体引擎的支持模块。
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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
#ifdef LCUI_FONT_ENGINE_FREETYPE
#include <LCUI/LCUI.h>
#include <LCUI/font.h>

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

static int FreeType_Open( const char *filepath, LCUI_FontFace **out_face )
{
	int err;
	FT_Face ft_face;
	LCUI_FontFace *my_face;

	err = FT_New_Face( freetype.library, filepath, 0, &ft_face );
	if( err ) {
		*out_face = NULL;
		return err;
	}
	my_face = (LCUI_FontFace*)malloc(sizeof(LCUI_FontFace));
	FT_Select_Charmap( ft_face, FT_ENCODING_UNICODE );
	strcpy( my_face->family_name, ft_face->family_name );
	strcpy( my_face->style_name, ft_face->style_name );
	my_face->data = ft_face;
	*out_face = my_face;
	return 0;
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
	printf(" width= %ld,  met->height= %ld\n"
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
	    case FT_PIXEL_MODE_BGRA:
	    default:
		memset( bmp->buffer, 255, size );
		break;
	}
	FT_Done_Glyph( glyph );
	return size;
}

static int FreeType_Render( LCUI_FontBitmap *bmp, wchar_t ch, 
			    int pixel_size, LCUI_FontFace *face )
{
	int error;
	size_t size;
	LCUI_BOOL has_space = FALSE;
	FT_Face ft_face = (FT_Face)face->data;

	/* 设定字体尺寸 */
	FT_Set_Pixel_Sizes( ft_face, 0, pixel_size );
	/* 如果是空格 */
	if( ch == ' ' ) {
		ch = 'a';
		has_space = TRUE;
	}
	/* 载入该字的字形数据 */
	error = FT_Load_Char( ft_face, ch, LCUI_FONT_LOAD_FALGS );
	if(error) {
		return error;
	}
	size = Convert_FTGlyph( bmp, ft_face->glyph, LCUI_FONT_RENDER_MODE );
	/* 如果是空格则将位图内容清空 */
	if( has_space ) {
		memset( bmp->buffer, 0, size );
	}
	return 0;
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
