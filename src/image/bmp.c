/* ***************************************************************************
 * bmp.c -- LCUI BMP image file processing module.
 * 
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
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
 * bmp.c -- LCUI的BMP位图文件读写支持模块。
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
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
					 100.0 * row / info->height );
		}
	}
	free( buffer );
	return 0;
}

