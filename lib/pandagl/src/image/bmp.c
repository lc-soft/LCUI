/*
 * lib/pandagl/src/image/bmp.c: -- LCUI BMP image file processing module.
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include "bmp_private.h"

/* clang-format off */

typedef struct {
	uint16_t type;		/**< Magic identifier */
	uint32_t size;		/**< File size in bytes */
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;	/**< Offset to image data, bytes */
} pd_bmp_header_t;

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
} pd_bmp_info_header_t;

typedef struct pd_bmp_reader {
	pd_bmp_header_t header;
	pd_bmp_info_header_t info;
        jmp_buf buf;
} pd_bmp_reader_t;

/* clang-format on */

static void pd_bmp_header_init(pd_bmp_header_t *header, uint16_t buffer[8])
{
        header->type = buffer[0];
        header->size = *(uint32_t *)(buffer + 1);
        header->reserved1 = buffer[3];
        header->reserved2 = buffer[4];
        header->offset = buffer[5];
}

void pd_bmp_reader_create(pd_image_reader_t *reader)
{
        reader->reader_data = malloc(sizeof(pd_bmp_reader_t));
}

void pd_bmp_reader_destroy(pd_image_reader_t *reader)
{
        free(reader->reader_data);
}

pd_error_t pd_bmp_reader_read_header(pd_image_reader_t *reader)
{
        size_t n;
        uint16_t buffer[8];
        pd_bmp_reader_t *bmp_reader = reader->reader_data;
        pd_bmp_info_header_t *info = &bmp_reader->info;

        if (reader->type != PD_BMP_READER) {
                return -EINVAL;
        }
        n = pd_file_reader_read(reader->file_reader, buffer, 14);
        /* 受到字节对齐影响，直接将读到的写到结构体变量里会让最后一个成员
         * 变量的值不正常，因此需要手动为其成员变量赋值 */
        pd_bmp_header_init(&bmp_reader->header, buffer);
        if (n < 14 || bmp_reader->header.type != 0x4D42) {
                return PD_ERROR_IMAGE_HEADER_INVALID;
        }
        n = pd_file_reader_read(reader->file_reader, info,
                                sizeof(pd_bmp_info_header_t));
        if (n < sizeof(pd_bmp_info_header_t)) {
                return PD_ERROR_IMAGE_HEADER_INVALID;
        }
        reader->header.width = info->width;
        reader->header.height = info->height;
        reader->header.type = PD_BMP_IMAGE;
        if (info->bits == 24) {
                reader->header.color_type = PD_COLOR_TYPE_RGB;
                reader->header.bit_depth = 24;
        }
        return PD_OK;
}

jmp_buf *pd_bmp_reader_jmpbuf(pd_image_reader_t *reader)
{
        pd_bmp_reader_t *bmp_reader = reader->reader_data;
        return &bmp_reader->buf;
}

void pd_bmp_reader_finish(pd_image_reader_t *reader)
{
}

void pd_bmp_reader_start(pd_image_reader_t *reader)
{
        pd_bmp_reader_t *bmp_reader = reader->reader_data;
        /* 信息头中的偏移位置是相对于起始处，需要减去当前已经偏移的位置 */
        long offset = bmp_reader->header.offset - bmp_reader->info.size - 14;
        pd_file_reader_skip(reader->file_reader, offset);
}

void pd_bmp_reader_read_row(pd_image_reader_t *reader, pd_canvas_t *graph)
{
        unsigned char *buffer, *dest;
        size_t n, row, bytes_per_row;
        pd_bmp_reader_t *bmp_reader = reader->reader_data;
        pd_bmp_info_header_t *info = &bmp_reader->info;

        /* 暂时不实现其它色彩类型处理 */
        if (info->bits != 24) {
                return;
        }
        bytes_per_row = (info->bits * info->width + 31) / 32 * 4;
        if (bytes_per_row < graph->bytes_per_row) {
                return;
        }
        buffer = malloc(bytes_per_row);
        /* 从最后一行开始保存 */
        dest = graph->bytes + graph->bytes_per_row * (graph->height - 1);
        for (row = 0; row < info->height; ++row) {
                n = pd_file_reader_read(reader->file_reader, buffer,
                                        bytes_per_row);
                if (n < bytes_per_row) {
                        break;
                }
                memcpy(dest, buffer, graph->bytes_per_row);
                dest -= graph->bytes_per_row;
        }
        free(buffer);
}
