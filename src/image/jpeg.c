/* ***************************************************************************
 * jpeg.c -- LCUI JPEG image file processing module.
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
 * jpeg.c -- LCUI的JPEG图像文件读写支持模块。
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

#ifdef USE_LIBJPEG
/* *********************************************************************
 * jmorecfg.h 里面的boolean是这样定义的：
 * typedef enum { FALSE = 0, TRUE = 1 } boolean;
 * 为了不使LCUI的FALSE和TRUE这两个宏与boolean枚举中的FALSE和TRUE产生冲突，在此用
 * #undef取消这两个宏。
 * ********************************************************************/
#undef TRUE
#undef FALSE

#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}
#endif

int LCUI_ReadJPEGFile( const char *filepath, LCUI_Graph *buf )
{
#ifdef USE_LIBJPEG
	FILE *fp;
	uchar_t *bytep;
	short int JPsyg;
	JSAMPARRAY buffer;
	struct my_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;
	int x, y, n, k, row_stride, jaka;

	fp = fopen( filepath, "rb" );
	if( !fp ) {
		return ENOENT;
	}
	if( fread( &JPsyg, sizeof( short int ), 1, fp ) ) {
		if( JPsyg != -9985 ) {  /* 如果不是jpg图片 */
			return  -1;
		}
	}
	rewind( fp );
	cinfo.err = jpeg_std_error( &jerr.pub );
	jerr.pub.error_exit = my_error_exit;
	if( setjmp( jerr.setjmp_buffer ) ) {
		jpeg_destroy_decompress( &cinfo );
		return 2;
	}
	jpeg_create_decompress( &cinfo );
	jpeg_stdio_src( &cinfo, fp );
	(void)jpeg_read_header( &cinfo, TRUE );
	(void)jpeg_start_decompress( &cinfo );
	jaka = cinfo.num_components;
	buf->color_type = COLOR_TYPE_RGB;
	n = Graph_Create( buf, cinfo.output_width, cinfo.output_height );
	if( n != 0 ) {
		_DEBUG_MSG( "error" );
		exit( -1 );
	}
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, 
					     JPOOL_IMAGE, row_stride, 1);
	bytep = buf->bytes;
	for( y = 0; cinfo.output_scanline < cinfo.output_height; ++y ) {
		(void)jpeg_read_scanlines( &cinfo, buffer, 1 );
		if( jaka == 3 ) {
			for( x = 0; x < buf->w; x++ ) {
				k = x * 3;
				*bytep++ = buffer[0][k + 2];
				*bytep++ = buffer[0][k + 1];
				*bytep++ = buffer[0][k];
			}
		} else {
			for( x = 0; x < buf->w; x++ ) {
				*bytep++ = buffer[0][x];
				*bytep++ = buffer[0][x];
				*bytep++ = buffer[0][x];
			}
		}
	}
	(void)jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	fclose( fp );
#else
	LOG( "warning: not JPEG support!" );
#endif
	return 0;
}

int Graph_GetJPEGSize( const char *filepath, int *width, int *height )
{
#ifdef USE_LIBJPEG
	short int JPsyg;
	struct my_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;
	FILE *fp = fopen( filepath, "rb" );
	if( !fp ) {
		return ENOENT;
	}
	if( fread( &JPsyg, sizeof( short int ), 1, fp ) ) {
		if( JPsyg != -9985 ) {
			return  -1;
		}
	}
	rewind( fp );
	cinfo.err = jpeg_std_error( &jerr.pub );
	jerr.pub.error_exit = my_error_exit;
	if( setjmp( jerr.setjmp_buffer ) ) {
		jpeg_destroy_decompress( &cinfo );
		return 2;
	}
	jpeg_create_decompress( &cinfo );
	jpeg_stdio_src( &cinfo, fp );
	(void)jpeg_read_header( &cinfo, TRUE );
	(void)jpeg_start_decompress( &cinfo );
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	jpeg_destroy_decompress( &cinfo );
	fclose( fp );
#else
	LOG( "warning: not JPEG support!" );
#endif
	return 0;
}
