/*
 * bmp.c -- LCUI BMP image file processing module.
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
#include <string.h>
#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

typedef struct {
	uint16_t type;		/**< Magic identifier */
	uint32_t size;		/**< File size in bytes */
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;	/**< Offset to image data, bytes */
} HEADER;

typedef struct {
	uint32_t size;				/**< Header size in bytes */
	uint32_t width, height;			/**< Width and height of image */
	uint16_t planes;			/**< Number of colour planes */
	uint16_t bits;				/**< Bits per pixel */
	uint32_t compression;			/**< Compression type */
	uint32_t imagesize;			/**< Image size in bytes */
	int32_t xresolution, yresolution;	/**< Pixels per meter */
	uint32_t ncolours;			/**< Number of colours */
	uint32_t importantcolours;		/**< Important colours */
} INFOHEADER;

typedef struct {
	unsigned char r, g, b, junk;
} COLOURINDEX;


typedef struct LCUI_BMPReaderRec_ {
	HEADER header;
	INFOHEADER info;
} LCUI_BMPReaderRec, *LCUI_BMPReader;

static void BMPHeader_Init( HEADER *header, uint16_t buffer[8] )
{
	header->type = buffer[0];
	header->size = *(uint32_t*)(buffer + 1);
	header->reserved1 = buffer[3];
	header->reserved2 = buffer[4];
	header->offset = buffer[5];
}

int LCUI_InitBMPReader( LCUI_ImageReader reader )
{
	ASSIGN( bmp_reader, LCUI_BMPReader );
	reader->data = bmp_reader;
	reader->destructor = free;
	reader->type = LCUI_BMP_READER;
	reader->env = &reader->env_src;
	if( reader->fn_begin ) {
		reader->fn_begin( reader->stream_data );
	}
	return 0;
}

int LCUI_ReadBMPHeader( LCUI_ImageReader reader )
{
	size_t n;
	uint16_t buffer[8];
	LCUI_BMPReader bmp_reader = reader->data;
	INFOHEADER *info = &bmp_reader->info;
	reader->header.type = LCUI_UNKNOWN_IMAGE;
	if( reader->type != LCUI_BMP_READER ) {
		return -EINVAL;
	}
	n = reader->fn_read( reader->stream_data, buffer, 14 );
	/* 受到字节对齐影响，直接将读到的写到结构体变量里会让最后一个成员
	 * 变量的值不正常，因此需要手动为其成员变量赋值 */
	BMPHeader_Init( &bmp_reader->header, buffer );
	if( n < 14 || bmp_reader->header.type != 0x4D42 ) {
		return -ENODATA;
	}
	n = reader->fn_read( reader->stream_data, info, sizeof( INFOHEADER ) );
	if( n < sizeof( INFOHEADER ) ) {
		return -ENODATA;
	}
	reader->header.width = info->width;
	reader->header.height = info->height;
	reader->header.type = LCUI_BMP_IMAGE;
	if( info->bits == 24 ) {
		reader->header.color_type = COLOR_TYPE_RGB;
		reader->header.bit_depth = 24;
	}
	return 0;
}

int LCUI_ReadBMP( LCUI_ImageReader reader, LCUI_Graph *graph )
{
	size_t n, row, bytes_per_row;
	unsigned char *buffer, *dest;
	LCUI_BMPReader bmp_reader = reader->data;
	INFOHEADER *info = &bmp_reader->info;
	if( reader->type != LCUI_BMP_READER ) {
		return -EINVAL;
	}
	if( reader->header.type == LCUI_UNKNOWN_IMAGE ) {
		if( LCUI_ReadBMPHeader( reader ) != 0 ) {
			return -ENODATA;
		}
	}
	/* 信息头中的偏移位置是相对于起始处，需要减去当前已经偏移的位置 */
	n = bmp_reader->header.offset - bmp_reader->info.size - 14;
	reader->fn_skip( reader->stream_data, n );
	if( 0 != Graph_Create( graph, info->width, info->height ) ) {
		return -ENOMEM;
	}
	/* 暂时不实现其它色彩类型处理 */
	if( info->bits != 24 ) {
		return -ENOSYS;
	}
	bytes_per_row = (info->bits * info->width + 31) / 32 * 4;
	if( bytes_per_row < graph->bytes_per_row ) {
		return -EINVAL;
	}
	buffer = malloc( bytes_per_row );
	if( !buffer ) {
		return -ENOMEM;
	}
	/* 从最后一行开始保存 */
	dest = graph->bytes + graph->bytes_per_row * (graph->height - 1);
	for( row = 0; row < info->height; ++row ) {
		n = reader->fn_read( reader->stream_data,
				     buffer, bytes_per_row );
		if( n < bytes_per_row ) {
			break;
		}
		memcpy( dest, buffer, graph->bytes_per_row );
		dest -= graph->bytes_per_row;
		if( reader->fn_prog ) {
			reader->fn_prog( reader->prog_arg,
					 100.0f * row / info->height );
		}
	}
	free( buffer );
	return 0;
}

