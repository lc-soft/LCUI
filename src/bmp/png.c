/* ***************************************************************************
 * png.c -- LCUI PNG image file processing module.
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * png.c -- LCUI的PNG图像文件读写支持模块。
 *
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

#ifdef USE_LIBPNG
#include <png.h>
#endif

#define PNG_BYTES_TO_CHECK 4

int Graph_LoadPNG( const char *filepath, LCUI_Graph *graph )
{
#ifdef USE_LIBPNG
	FILE *fp;
	uchar_t *pixel_ptr;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep* row_pointers;
	char buf[PNG_BYTES_TO_CHECK];
	int w, h, x, y, ret = 0, temp, color_type;

	fp = fopen( filepath, "rb" );
	if( fp == NULL ) {
		return ENOENT;
	}

	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	info_ptr = png_create_info_struct( png_ptr );

	setjmp( png_jmpbuf( png_ptr ) );
	/* 读取PNG_BYTES_TO_CHECK个字节的数据 */
	temp = fread( buf, 1, PNG_BYTES_TO_CHECK, fp );
	/* 若读到的数据并没有PNG_BYTES_TO_CHECK个字节 */
	if( temp < PNG_BYTES_TO_CHECK ) {
		fclose( fp );
		png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
		return -1;
	}
	/* 检测数据是否为PNG的签名 */
	temp = png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK );
	/* 如果不是PNG的签名，则说明该文件不是PNG文件 */
	if( temp != 0 ) {
		fclose( fp );
		png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
		return -1;
	}
	/* 复位文件指针 */
	rewind( fp );
	/* 开始读文件 */
	png_init_io( png_ptr, fp );
	/* 读取PNG图片信息 */
	png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0 );
	/* 获取图像的色彩类型 */
	color_type = png_get_color_type( png_ptr, info_ptr );
	/* 获取图像的宽高 */
	w = png_get_image_width( png_ptr, info_ptr );
	h = png_get_image_height( png_ptr, info_ptr );
	/* 获取所有行像素数据，row_pointers里边就是rgba数据 */
	row_pointers = png_get_rows( png_ptr, info_ptr );
	/* 根据不同的色彩类型进行相应处理 */
	switch( color_type ) {
	case PNG_COLOR_TYPE_RGB_ALPHA:
		graph->color_type = COLOR_TYPE_ARGB;
		temp = Graph_Create( graph, w, h );
		if( temp != 0 ) {
			ret = -ENOMEM;
			break;
		}
		pixel_ptr = graph->bytes;
		for( y = 0; y < h; ++y ) {
			/*
			 * Graph的像素数据存储格式是BGRA，而PNG库
			 * 提供像素数据的是RGBA格式的，因此需要调整写入顺序
			 */
			for( x = 0; x < w * 4; x += 4 ) {
				*pixel_ptr++ = row_pointers[y][x + 2];
				*pixel_ptr++ = row_pointers[y][x + 1];
				*pixel_ptr++ = row_pointers[y][x];
				*pixel_ptr++ = row_pointers[y][x + 3];
			}
		}
		break;

	case PNG_COLOR_TYPE_RGB:
		graph->color_type = COLOR_TYPE_RGB;
		temp = Graph_Create( graph, w, h );
		if( temp != 0 ) {
			ret = -ENOMEM;
			break;
		}
		pixel_ptr = graph->bytes;
		for( y = 0; y < h; ++y ) {
			for( x = 0; x < w * 3; x += 3 ) {
				*pixel_ptr++ = row_pointers[y][x + 2];
				*pixel_ptr++ = row_pointers[y][x + 1];
				*pixel_ptr++ = row_pointers[y][x];
			}
		}
		break;
		/* 其它色彩类型的图像就读了 */
	default: ret = -1; break;
	}
	fclose( fp );
	png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
	return ret;
#else
	_DEBUG_MSG( "warning: not PNG support!" );
	return -1;
#endif
}

int Graph_GetPNGSize( const char *filepath, int *width, int *height )
{
#ifdef USE_LIBPNG
	int ret;
	png_infop info_ptr;
	png_structp png_ptr;
	char buf[PNG_BYTES_TO_CHECK];
	FILE *fp = fopen( filepath, "rb" );
	if( !fp ) {
		return ENOENT;
	}
	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	info_ptr = png_create_info_struct( png_ptr );
	setjmp( png_jmpbuf( png_ptr ) );
	ret = fread( buf, 1, PNG_BYTES_TO_CHECK, fp );
	if( ret < PNG_BYTES_TO_CHECK ) {
		fclose( fp );
		png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
		return -1;
	}
	ret = png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK );
	if( ret != 0 ) {
		fclose( fp );
		png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
		return -1;
	}
	rewind( fp );
	png_init_io( png_ptr, fp );
	png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0 );
	*width = png_get_image_width( png_ptr, info_ptr );
	*height = png_get_image_height( png_ptr, info_ptr );
	fclose( fp );
	png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
	return 0;
#else
	_DEBUG_MSG( "warning: not PNG support!" );
	return -1;
#endif
}

int Graph_WritePNG( const char *file_name, const LCUI_Graph *graph )
{
#ifdef USE_LIBPNG
	FILE *fp;
	LCUI_Rect rect;
	png_byte color_type;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers;
	int x, y, row_size;

	if( !Graph_IsValid( graph ) ) {
		_DEBUG_MSG( "graph is not valid\n" );
		return -1;
	}
	/* create file */
	fp = fopen( file_name, "wb" );
	if( !fp ) {
		_DEBUG_MSG( "file %s could not be opened for writing\n", file_name );
		return -1;
	}
	/* initialize stuff */
	png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if( !png_ptr ) {
		fclose( fp );
		_DEBUG_MSG( "png_create_write_struct failed\n" );
		return -1;
	}
	info_ptr = png_create_info_struct( png_ptr );
	if( !info_ptr ) {
		fclose( fp );
		_DEBUG_MSG( "png_create_info_struct failed\n" );
		png_destroy_write_struct( &png_ptr, &info_ptr );
		return -1;
	}
	if( setjmp( png_jmpbuf( png_ptr ) ) ) {
		fclose( fp );
		_DEBUG_MSG( "error during init_io\n" );
		png_destroy_write_struct( &png_ptr, &info_ptr );
		return -1;
	}
	png_init_io( png_ptr, fp );
	if( Graph_HasAlpha( graph ) ) {
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	} else {
		color_type = PNG_COLOR_TYPE_RGB;
	}
	/* write header */
	png_set_IHDR( png_ptr, info_ptr, graph->w, graph->h,
		      8, color_type, PNG_INTERLACE_NONE,
		      PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );

	png_write_info( png_ptr, info_ptr );
	/* write bytes */

	Graph_GetValidRect( graph, &rect );
	graph = Graph_GetQuote( graph );
	if( graph->color_type == COLOR_TYPE_ARGB ) {
		LCUI_ARGB *px_ptr, *px_row_ptr;

		row_size = png_get_rowbytes( png_ptr, info_ptr );
		px_row_ptr = graph->argb + rect.top * graph->width + rect.left;
		row_pointers = (png_bytep*)malloc( rect.height*sizeof( png_bytep ) );
		for( y = 0; y < rect.height; ++y ) {
			row_pointers[y] = png_malloc( png_ptr, row_size );
			px_ptr = px_row_ptr;
			for( x = 0; x < row_size; ++px_ptr ) {
				row_pointers[y][x++] = px_ptr->red;
				row_pointers[y][x++] = px_ptr->green;
				row_pointers[y][x++] = px_ptr->blue;
				row_pointers[y][x++] = px_ptr->alpha;
			}
			px_row_ptr += graph->w;
		}
	} else {
		uchar_t *px_ptr, *px_row_ptr;

		row_size = png_get_rowbytes( png_ptr, info_ptr );
		px_row_ptr = graph->bytes + rect.top * graph->bytes_per_row;
		px_row_ptr += rect.left * graph->bytes_per_pixel;
		row_pointers = (png_bytep*)malloc( rect.height*sizeof( png_bytep ) );
		for( y = 0; y < rect.height; ++y ) {
			row_pointers[y] = (png_bytep)malloc( row_size );
			px_ptr = px_row_ptr;
			for( x = 0; x < row_size; x += 3 ) {
				row_pointers[y][x + 2] = *px_ptr++; // blue
				row_pointers[y][x + 1] = *px_ptr++; // green
				row_pointers[y][x] = *px_ptr++;   // red
			}
			px_row_ptr += graph->bytes_per_row;
		}
	}
	png_write_image( png_ptr, row_pointers );
	/* cleanup heap allocation */
	for( y = 0; y < rect.height; ++y ) {
		free( row_pointers[y] );
	}
	free( row_pointers );
	/* end write */
	png_write_end( png_ptr, NULL );
	png_destroy_write_struct( &png_ptr, &info_ptr );
	fclose( fp );
	return 0;
#else
	LOG( "warning: not PNG support!" );
	return 0;
#endif
}
