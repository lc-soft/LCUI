/*
 * jpeg.c -- LCUI JPEG image file processing module.
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

#define BUFFER_SIZE 4096

typedef struct LCUI_JPEGErrorRec_ {
	struct jpeg_error_mgr pub;
	LCUI_ImageReader reader;
} LCUI_JPEGErrorRec, *LCUI_JPEGError;

typedef struct LCUI_JPEGReaderRec_ {
	struct jpeg_source_mgr src;		/**< JPEG 资源管理接口 */
	LCUI_JPEGErrorRec err;			/**< JPEG 的错误处理接口 */
	LCUI_BOOL start_of_file;		/**< 是否刚开始读文件 */
	LCUI_ImageReader base;			/**< 所属的图片读取器 */
	unsigned char buffer[BUFFER_SIZE];	/**< 数据缓存 */
} LCUI_JPEGReaderRec, *LCUI_JPEGReader;

static void DestroyJPEGReader( void *data )
{
	j_decompress_ptr cinfo = data;
	jpeg_destroy_decompress( cinfo );
	free( data );
}

METHODDEF( void ) JPEGReader_OnErrorExit( j_common_ptr cinfo )
{
	char msg[JMSG_LENGTH_MAX];
	LCUI_JPEGError err = (LCUI_JPEGError)cinfo->err;
	cinfo->err->format_message( cinfo, msg );
	LOG( "%s\n", msg );
	longjmp( *err->reader->env, 1 );
}

static void JPEGReader_OnInit( j_decompress_ptr cinfo )
{
	LCUI_ImageReader reader;
	LCUI_JPEGReader jpeg_reader;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	jpeg_reader->start_of_file = TRUE;
	reader = jpeg_reader->base;
	if( reader->fn_begin ) {
		reader->fn_begin( reader->stream_data );
	}
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
	if( size <= 0 ) {
		/* 将空的输入文件视为致命错误 */
		if( jpeg_reader->start_of_file ) {
			ERREXIT( cinfo, JERR_INPUT_EMPTY );
		}
		WARNMS( cinfo, JWRN_JPEG_EOF );
		/* 插入一个无用的 EOI 标记 */
		jpeg_reader->buffer[0] = (JOCTET)0xFF;
		jpeg_reader->buffer[1] = (JOCTET)JPEG_EOI;
		size = 2;
	}
	/* 设置数据缓存地址和大小，供 jpeg 解码器使用 */
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
	jpeg_reader->src.bytes_in_buffer = size;
	jpeg_reader->start_of_file = FALSE;
	return TRUE;
}

static void JPEGReader_OnSkip( j_decompress_ptr cinfo, long num_bytes )
{
	LCUI_ImageReader reader;
	LCUI_JPEGReader jpeg_reader;
	struct jpeg_source_mgr *src = cinfo->src;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	reader = jpeg_reader->base;
	if( !reader->fn_skip ) {
		if( num_bytes <= 0 ) {
			return;
		}
		while( num_bytes > (long)src->bytes_in_buffer ) {
			num_bytes -= (long)src->bytes_in_buffer;
			src->fill_input_buffer( cinfo );
		}
		src->next_input_byte += (size_t)num_bytes;
		src->bytes_in_buffer -= (size_t)num_bytes;
		return;
	}
	/* 如果跳过的字节数小于当前缓存的数据大小，则直接移动数据读取指针 */
	if( src->bytes_in_buffer > (size_t)num_bytes ) {
		src->next_input_byte += num_bytes;
		src->bytes_in_buffer -= num_bytes;
		return;
	}
	num_bytes -= (long)src->bytes_in_buffer;
	reader->fn_skip( reader->stream_data, num_bytes );
	/* 重置当前缓存 */
	src->bytes_in_buffer = 0;
	src->next_input_byte = jpeg_reader->buffer;
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

int LCUI_ReadJPEGHeader( LCUI_ImageReader reader )
{
	size_t size;
	short int *buffer;
	j_decompress_ptr cinfo;
	LCUI_JPEGReader jpeg_reader;
	LCUI_ImageHeader header = &reader->header;
	if( reader->type != LCUI_JPEG_READER ) {
		return -EINVAL;
	}
	cinfo = reader->data;
	jpeg_reader = (LCUI_JPEGReader)cinfo->src;
	size = reader->fn_read( reader->stream_data, jpeg_reader->buffer,
				sizeof( short int ) );
	if( size < sizeof( short int ) ) {
		return -ENODATA;
	}
	jpeg_reader->src.bytes_in_buffer = sizeof( short int );
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
	buffer = (short int*)jpeg_reader->buffer;
	if( buffer[0] != -9985 ) {
		return -ENODATA;
	}
	jpeg_read_header( cinfo, TRUE );
	header->width = cinfo->image_width;
	header->height = cinfo->image_height;
	header->type = LCUI_JPEG_IMAGE;
	if( cinfo->jpeg_color_space == JCS_RGB ) {
		header->color_type = COLOR_TYPE_RGB;
		header->bit_depth = 24;
	} else {
		header->color_type = 0;
	}
	return 0;
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
	reader->header.type = LCUI_UNKNOWN_IMAGE;
	reader->destructor = DestroyJPEGReader;
	reader->env = &reader->env_src;
	cinfo->src = (struct jpeg_source_mgr*)jpeg_reader;
	cinfo->err = jpeg_std_error( &jpeg_reader->err.pub );
	jpeg_reader->err.pub.error_exit = JPEGReader_OnErrorExit;
	jpeg_reader->err.reader = reader;
	return 0;
#else
	LOG( "warning: not JPEG support!" );
#endif
	return -1;
}

int LCUI_ReadJPEG( LCUI_ImageReader reader, LCUI_Graph *graph )
{
#ifdef USE_LIBJPEG
	uchar_t *bytep;
	JSAMPARRAY buffer;
	j_decompress_ptr cinfo;
	int k, row_stride;
	size_t x;

	if( reader->type != LCUI_JPEG_READER ) {
		return -EINVAL;
	}
	if( reader->header.type == LCUI_UNKNOWN_IMAGE ) {
		if( LCUI_ReadJPEGHeader( reader ) != 0 ) {
			return -ENODATA;
		}
	}
	cinfo = reader->data;
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
	row_stride = cinfo->output_width * cinfo->output_components;
	buffer = cinfo->mem->alloc_sarray( (j_common_ptr)cinfo,
					   JPOOL_IMAGE, row_stride, 1 );
	while( cinfo->output_scanline < cinfo->output_height ) {
		bytep = graph->bytes;
		bytep += cinfo->output_scanline * graph->bytes_per_row;
		jpeg_read_scanlines( cinfo, buffer, 1 );
		for( x = 0; x < graph->width; ++x ) {
			k = x * 3;
			*bytep++ = buffer[0][k + 2];
			*bytep++ = buffer[0][k + 1];
			*bytep++ = buffer[0][k];
		}
		if( reader->fn_prog ) {
			reader->fn_prog( reader->prog_arg,
					 100.0f * cinfo->output_scanline
					 / cinfo->output_height );
		}
	}
	return 0;
#else
	LOG( "warning: not JPEG support!" );
#endif
	return -ENOSYS;
}

