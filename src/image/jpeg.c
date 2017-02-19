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
#include <LCUI/image.h>

#ifdef USE_LIBJPEG

#undef TRUE
#undef FALSE

#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>

#define BUFFER_SIZE 4096

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct LCUI_JPEGErrRec_ {
	struct jpeg_error_mgr err;	/**< JPEG 的错误处理接口 */
	jmp_buf setjmp_buffer;		/**< setjump 的缓存 */
} LCUI_JPEGErrRec, *LCUI_JPEGErr;

typedef struct LCUI_JPEGReaderRec_ {
	struct jpeg_source_mgr src;		/**< JPEG 资源管理接口 */
	LCUI_JPEGErrRec err;			/**< 用于错误处理相关的数据 */
	LCUI_ImageReader base;			/**< 所属的图片读取器 */
	unsigned char buffer[BUFFER_SIZE];	/**< 数据缓存 */
} LCUI_JPEGReaderRec, *LCUI_JPEGReader;

typedef struct my_error_mgr * my_error_ptr;

METHODDEF( void ) my_error_exit( j_common_ptr cinfo )
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	cinfo->err->output_message( cinfo );
	longjmp( myerr->setjmp_buffer, 1 );
}

static void DestroyJPEGReader( void *data )
{
	j_decompress_ptr cinfo = data;
	jpeg_finish_decompress( cinfo );
	jpeg_destroy_decompress( cinfo );
	free( data );
}

METHODDEF( void ) JPEGReader_OnErrorExit( j_common_ptr cinfo )
{
	LCUI_JPEGErr err = (LCUI_JPEGErr)cinfo->err;
	cinfo->err->output_message( cinfo );
	//longjmp( err->setjmp_buffer, 1 );
}

static void JPEGReader_OnInit( j_decompress_ptr cinfo )
{
	LCUI_ImageReader reader;
	LCUI_JPEGReader jpeg_reader;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	reader = jpeg_reader->base;
	if( reader->fn_begin ) {
		reader->fn_begin( reader->stream_data );
	}
}

static size_t JPEGReader_Rewind( LCUI_JPEGReader reader )
{

}

static boolean JPEGReader_OnRead( j_decompress_ptr cinfo )
{
	size_t size;
	LCUI_ImageReader reader;
	LCUI_JPEGReader jpeg_reader;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	reader = jpeg_reader->base;
	size = reader->fn_read( reader->stream_data,
				jpeg_reader->buffer, BUFFER_SIZE );
	/* 设置数据缓存地址和大小，供 jpeg 解码器使用 */
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
	jpeg_reader->src.bytes_in_buffer = size;
	if( size < BUFFER_SIZE ) {
		return FALSE;
	}
	return TRUE;
}

static void JPEGReader_OnSkip( j_decompress_ptr cinfo, long num_bytes )
{
	size_t size;
	LCUI_ImageReader reader;
	LCUI_JPEGReader jpeg_reader;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	reader = jpeg_reader->base;
	/* 如果跳过的字节数小于当前缓存的数据大小，则直接移动数据读取指针 */
	if( jpeg_reader->src.bytes_in_buffer > (size_t)num_bytes ) {
		jpeg_reader->src.next_input_byte += num_bytes;
		jpeg_reader->src.bytes_in_buffer -= num_bytes;
		return;
	}
	size = num_bytes - jpeg_reader->src.bytes_in_buffer;
	reader->fn_skip( reader->stream_data, size );
	/* 重置当前缓存 */
	jpeg_reader->src.bytes_in_buffer = 0;
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
}

static void JPEGReader_OnTerminate( j_decompress_ptr cinfo )
{
	LCUI_ImageReader reader;
	LCUI_JPEGReader jpeg_reader;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	reader = jpeg_reader->base;
	if( reader->fn_end ) {
		reader->fn_end( reader->stream_data );
	}
}

static void *jpeg_malloc( j_decompress_ptr cinfo, size_t size )
{
	return cinfo->mem->alloc_small( (j_common_ptr)cinfo,
					JPOOL_PERMANENT, size );
}

static LCUI_BOOL LCUI_CheckImageIsJPEG( LCUI_ImageReader reader )
{
	size_t size;
	short int mark;
	j_decompress_ptr cinfo = reader->data;
	LCUI_JPEGReader jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	size = reader->fn_read( reader->stream_data, jpeg_reader->buffer,
				sizeof( short int ) );
	if( size < sizeof( short int ) ) {
		return FALSE;
	}
	jpeg_reader->src.bytes_in_buffer = sizeof( short int );
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
	mark = *((short int*)jpeg_reader->buffer);
	return mark == -9985;
}

#endif

int LCUI_InitJPEGReader( LCUI_ImageReader reader )
{
#ifdef USE_LIBJPEG
	j_decompress_ptr cinfo;
	LCUI_JPEGReader jpeg_reader;
	cinfo = malloc( sizeof( struct jpeg_decompress_struct ) );
	jpeg_create_decompress( cinfo );
	jpeg_reader = jpeg_malloc( cinfo, sizeof( LCUI_JPEGReaderRec ) );
	jpeg_reader->src.init_source = JPEGReader_OnInit;
	jpeg_reader->src.term_source = JPEGReader_OnTerminate;
	jpeg_reader->src.skip_input_data = JPEGReader_OnSkip;
	jpeg_reader->src.fill_input_buffer = JPEGReader_OnRead;
	jpeg_reader->src.resync_to_restart = jpeg_resync_to_restart;
	jpeg_reader->src.bytes_in_buffer = 0;
	jpeg_reader->src.next_input_byte = NULL;
	jpeg_reader->base = reader;
	reader->data = cinfo;
	reader->type = LCUI_JPEG_READER;
	reader->destructor = DestroyJPEGReader;
	cinfo->src = (struct jpeg_source_mgr*)jpeg_reader;
	cinfo->err = jpeg_std_error( &jpeg_reader->err.err );
	jpeg_reader->err.err.error_exit = JPEGReader_OnErrorExit;
	if( setjmp( jpeg_reader->err.setjmp_buffer ) ) {
		reader->type = LCUI_UNKNOWN_READER;
		return -1;
	}
	if( LCUI_CheckImageIsJPEG( reader ) ) {
		return 0;
	}
	if( reader->fn_rewind ) {
		reader->fn_rewind( reader->stream_data );
	}
	LCUI_DestroyImageReader( reader );
#endif
	return -1;
}

int LCUI_ReadJPEG( LCUI_ImageReader reader, LCUI_Graph *graph )
{
#ifdef USE_LIBJPEG
	uchar_t *bytep;
	JSAMPARRAY buffer;
	j_decompress_ptr cinfo;
	int x, y, k, row_stride;

	if( reader->type != LCUI_JPEG_READER ) {
		return -EINVAL;
	}
	cinfo = reader->data;
	jpeg_read_header( cinfo, TRUE );
	jpeg_start_decompress( cinfo );
	/* 暂时不处理其它色彩类型的图像 */
	if( cinfo->num_components != 3 ) {
		return -ENOSYS;
	}
	graph->color_type = COLOR_TYPE_RGB;
	if( 0 != Graph_Create( graph, cinfo->output_width,
			       cinfo->output_height ) ) {
		return -ENOMEM;
	}
	bytep = graph->bytes;
	row_stride = cinfo->output_width * cinfo->output_components;
	buffer = cinfo->mem->alloc_sarray( (j_common_ptr)cinfo,
					   JPOOL_IMAGE, row_stride, 1 );
	for( y = 0; cinfo->output_scanline < cinfo->output_height; ++y ) {
		jpeg_read_scanlines( cinfo, buffer, 1 );
		for( x = 0; x < graph->width; x++ ) {
			k = x * 3;
			*bytep++ = buffer[0][k + 2];
			*bytep++ = buffer[0][k + 1];
			*bytep++ = buffer[0][k];
		}
	}
	return 0;
#else
	LOG( "warning: not JPEG support!" );
#endif
	return -ENOSYS;
}

static size_t FileStream_OnRead( void *data, void *buffer, size_t size )
{
	return fread( buffer, 1, size, data );
}

static void FileStream_OnSkip( void *data, long offset )
{
	fseek( data, offset, SEEK_CUR );
}

static void FileStream_OnRewind( void *data )
{
	rewind( data );
}

int LCUI_ReadJPEGFile( const char *filepath, LCUI_Graph *graph )
{
	FILE *fp;
	LCUI_ImageReaderRec reader = { 0 };
	fp = fopen( filepath, "rb" );
	if( !fp ) {
		return -ENOENT;
	}
	reader.stream_data = fp;
	reader.fn_read = FileStream_OnRead;
	reader.fn_skip = FileStream_OnSkip;
	reader.fn_rewind = FileStream_OnRewind;
	if( LCUI_InitJPEGReader( &reader ) != 0 ) {
		return -ENODATA;
	}
	return LCUI_ReadJPEG( &reader, graph );
}

int LCUI_ReadJPEGFile2( const char *filepath, LCUI_Graph *buf )
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
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	jpeg_destroy_decompress( &cinfo );
	fclose( fp );
#else
	LOG( "warning: not JPEG support!" );
#endif
	return 0;
}
