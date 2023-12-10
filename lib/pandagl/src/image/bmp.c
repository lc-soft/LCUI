/*
 * lib/pandagl/src/image/bmp.c: -- LCUI BMP image file processing module.
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pandagl.h>
#include "bmp.h"

/* clang-format off */

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


typedef struct pd_bmp_reader_t {
	HEADER header;
	INFOHEADER info;
} pd_bmp_reader_t;

/* clang-format on */

static void BMPHeader_Init(HEADER *header, uint16_t buffer[8])
{
	header->type = buffer[0];
	header->size = *(uint32_t *)(buffer + 1);
	header->reserved1 = buffer[3];
	header->reserved2 = buffer[4];
	header->offset = buffer[5];
}

int pd_bmp_reader_init(pd_image_reader_t* reader)
{
	pd_bmp_reader_t * bmp_reader;

	bmp_reader = malloc(sizeof(pd_bmp_reader_t));
	reader->data = bmp_reader;
	reader->destructor = free;
	reader->type = PD_BMP_READER;
	reader->env = &reader->env_src;
	if (reader->fn_begin) {
		reader->fn_begin(reader->stream_data);
	}
	return 0;
}

int pd_bmp_reader_read_header(pd_image_reader_t* reader)
{
	size_t n;
	uint16_t buffer[8];
	pd_bmp_reader_t * bmp_reader = reader->data;
	INFOHEADER *info = &bmp_reader->info;
	reader->header.type = PD_UNKNOWN_IMAGE;
	if (reader->type != PD_BMP_READER) {
		return -EINVAL;
	}
	n = reader->fn_read(reader->stream_data, buffer, 14);
	/* 受到字节对齐影响，直接将读到的写到结构体变量里会让最后一个成员
	 * 变量的值不正常，因此需要手动为其成员变量赋值 */
	BMPHeader_Init(&bmp_reader->header, buffer);
	if (n < 14 || bmp_reader->header.type != 0x4D42) {
		return -2;
	}
	n = reader->fn_read(reader->stream_data, info, sizeof(INFOHEADER));
	if (n < sizeof(INFOHEADER)) {
		return -2;
	}
	reader->header.width = info->width;
	reader->header.height = info->height;
	reader->header.type = PD_BMP_IMAGE;
	if (info->bits == 24) {
		reader->header.color_type = PD_COLOR_TYPE_RGB;
		reader->header.bit_depth = 24;
	}
	return 0;
}

int pd_bmp_reader_read_data(pd_image_reader_t* reader, pd_canvas_t *graph)
{
	long offset;
	unsigned char *buffer, *dest;
	size_t n, row, bytes_per_row;
	pd_bmp_reader_t * bmp_reader = reader->data;
	INFOHEADER *info = &bmp_reader->info;

	if (reader->type != PD_BMP_READER) {
		return -EINVAL;
	}
	if (reader->header.type == PD_UNKNOWN_IMAGE) {
		if (pd_bmp_reader_read_header(reader) != 0) {
			return -2;
		}
	}
	/* 信息头中的偏移位置是相对于起始处，需要减去当前已经偏移的位置 */
	offset = bmp_reader->header.offset - bmp_reader->info.size - 14;
	reader->fn_skip(reader->stream_data, offset);
	if (0 != pd_canvas_create(graph, info->width, info->height)) {
		return -ENOMEM;
	}
	/* 暂时不实现其它色彩类型处理 */
	if (info->bits != 24) {
		return -ENOSYS;
	}
	bytes_per_row = (info->bits * info->width + 31) / 32 * 4;
	if (bytes_per_row < graph->bytes_per_row) {
		return -EINVAL;
	}
	buffer = malloc(bytes_per_row);
	if (!buffer) {
		return -ENOMEM;
	}
	/* 从最后一行开始保存 */
	dest = graph->bytes + graph->bytes_per_row * (graph->height - 1);
	for (row = 0; row < info->height; ++row) {
		n = reader->fn_read(reader->stream_data, buffer, bytes_per_row);
		if (n < bytes_per_row) {
			break;
		}
		memcpy(dest, buffer, graph->bytes_per_row);
		dest -= graph->bytes_per_row;
		if (reader->fn_prog) {
			reader->fn_prog(reader->prog_arg,
					100.0f * row / info->height);
		}
	}
	free(buffer);
	return 0;
}
