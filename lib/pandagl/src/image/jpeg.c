/*
 * lib/pandagl/src/image/jpeg.c: -- LCUI JPEG image file processing module.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pandagl.h>
#include <setjmp.h>
#include "jpeg_private.h"

#ifdef PANDAGL_HAS_LIBJPEG

#include <jpeglib.h>
#include <jerror.h>

#define BUFFER_SIZE 4096

typedef struct pd_jpeg_error_t {
	struct jpeg_error_mgr pub;
	pd_image_reader_t *reader;
} pd_jpeg_error_t;

typedef struct pd_jpeg_reader_t {
	struct jpeg_source_mgr src;
	struct jpeg_decompress_struct cinfo;
	jmp_buf env;
	pd_jpeg_error_t err;
	bool start_of_file;
	pd_image_reader_t *base;
	unsigned char buffer[BUFFER_SIZE];
} pd_jpeg_reader_t;

METHODDEF(void) pd_jpeg_reader_on_error_exit(j_common_ptr cinfo)
{
	pd_jpeg_error_t *err = (pd_jpeg_error_t *)cinfo->err;
	pd_jpeg_reader_t *reader = err->reader->reader_data;
	cinfo->err->format_message(cinfo, err->reader->error_message);
	longjmp(reader->env, PD_ERROR_IMAGE_READING);
}

static void pd_jpeg_reader_on_init(j_decompress_ptr cinfo)
{
	pd_jpeg_reader_t *jpeg_reader;
	jpeg_reader = (pd_jpeg_reader_t *)cinfo->src;
	jpeg_reader->start_of_file = PD_TRUE;
}

static boolean pd_jpeg_reader_on_read(j_decompress_ptr cinfo)
{
	size_t size;
	pd_jpeg_reader_t *jpeg_reader = (pd_jpeg_reader_t *)cinfo->src;
	pd_image_reader_t *reader = jpeg_reader->base;

	size = pd_file_reader_read(reader->file_reader, jpeg_reader->buffer,
			       BUFFER_SIZE);
	if (size <= 0) {
		/* 将空的输入文件视为致命错误 */
		if (jpeg_reader->start_of_file) {
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		}
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* 插入一个无用的 EOI 标记 */
		jpeg_reader->buffer[0] = (JOCTET)0xFF;
		jpeg_reader->buffer[1] = (JOCTET)JPEG_EOI;
		size = 2;
	}
	/* 设置数据缓存地址和大小，供 jpeg 解码器使用 */
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
	jpeg_reader->src.bytes_in_buffer = size;
	jpeg_reader->start_of_file = PD_FALSE;
	return true;
}

static void pd_jpeg_reader_on_skip(j_decompress_ptr cinfo, long num_bytes)
{
	struct jpeg_source_mgr *src = cinfo->src;
	pd_jpeg_reader_t * jpeg_reader = (pd_jpeg_reader_t *)cinfo->src;
	pd_image_reader_t *reader = jpeg_reader->base;

	/* 如果跳过的字节数小于当前缓存的数据大小，则直接移动数据读取指针 */
	if (src->bytes_in_buffer > (size_t)num_bytes) {
		src->next_input_byte += num_bytes;
		src->bytes_in_buffer -= num_bytes;
		return;
	}
	num_bytes -= (long)src->bytes_in_buffer;
	pd_file_reader_skip(reader->file_reader, num_bytes);
	/* 重置当前缓存 */
	src->bytes_in_buffer = 0;
	src->next_input_byte = jpeg_reader->buffer;
}

static void pd_jpeg_reader_on_terminate(j_decompress_ptr cinfo)
{
}


void pd_jpeg_reader_create(pd_image_reader_t *reader)
{
	pd_jpeg_reader_t *jpeg_reader;

	jpeg_reader = malloc(sizeof(pd_jpeg_reader_t));
	jpeg_reader->src.init_source = pd_jpeg_reader_on_init;
	jpeg_reader->src.term_source = pd_jpeg_reader_on_terminate;
	jpeg_reader->src.skip_input_data = pd_jpeg_reader_on_skip;
	jpeg_reader->src.fill_input_buffer = pd_jpeg_reader_on_read;
	jpeg_reader->src.resync_to_restart = jpeg_resync_to_restart;
	jpeg_reader->src.bytes_in_buffer = 0;
	jpeg_reader->src.next_input_byte = NULL;
	jpeg_reader->err.pub.error_exit = pd_jpeg_reader_on_error_exit;
	jpeg_reader->err.reader = reader;
	jpeg_reader->base = reader;
	jpeg_create_decompress(&jpeg_reader->cinfo);
	jpeg_reader->cinfo.src = &jpeg_reader->src;
	jpeg_reader->cinfo.err = jpeg_std_error(&jpeg_reader->err.pub);
	reader->reader_data = jpeg_reader;
}

void pd_jpeg_reader_start(pd_image_reader_t *reader)
{
	pd_jpeg_reader_t *jpeg_reader = reader->reader_data;
	jpeg_start_decompress(&jpeg_reader->cinfo);
}

void pd_jpeg_reader_finish(pd_image_reader_t *reader)
{
	pd_jpeg_reader_t *jpeg_reader = reader->reader_data;

	jpeg_finish_decompress(&jpeg_reader->cinfo);
}

void pd_jpeg_reader_destroy(pd_image_reader_t *reader)
{
	pd_jpeg_reader_t *jpeg_reader = reader->reader_data;
	jpeg_destroy_decompress(&jpeg_reader->cinfo);
	free(jpeg_reader);
}

jmp_buf *pd_jpeg_reader_jmpbuf(pd_image_reader_t *reader)
{
	pd_jpeg_reader_t *jpeg_reader = reader->reader_data;
	return &jpeg_reader->env;
}

pd_error_t pd_jpeg_reader_read_header(pd_image_reader_t *reader)
{
	size_t size;
	short int *buffer;
	pd_image_header_t *header = &reader->header;
	pd_jpeg_reader_t *jpeg_reader = reader->reader_data;
	j_decompress_ptr cinfo = &jpeg_reader->cinfo;

	size = pd_file_reader_read(reader->file_reader, jpeg_reader->buffer,
			       sizeof(short int));
	if (size < sizeof(short int)) {
		return PD_ERROR_IMAGE_HEADER_INVALID;
	}
	jpeg_reader->src.bytes_in_buffer = sizeof(short int);
	jpeg_reader->src.next_input_byte = jpeg_reader->buffer;
	buffer = (short int *)jpeg_reader->buffer;
	if (buffer[0] != -9985) {
		return PD_ERROR_IMAGE_HEADER_INVALID;
	}
	if (setjmp(jpeg_reader->env)) {
		return PD_ERROR_IMAGE_HEADER_INVALID;
	}
	jpeg_read_header(cinfo, PD_TRUE);
	header->width = cinfo->image_width;
	header->height = cinfo->image_height;
	header->type = PD_JPEG_IMAGE;
	header->color_type = PD_COLOR_TYPE_RGB;
	if (cinfo->jpeg_color_space == JCS_RGB) {
		header->bit_depth = 24;
	}
	return PD_OK;
}

void pd_jpeg_reader_read_row(pd_image_reader_t *reader, pd_canvas_t *out)
{
	uint8_t *bytep;
	JSAMPARRAY buffer;
	pd_jpeg_reader_t *jpeg_reader = reader->reader_data;
	j_decompress_ptr cinfo = &jpeg_reader->cinfo;
	int row_stride = cinfo->output_width * cinfo->output_components;
	int k;
	unsigned x;

	buffer = cinfo->mem->alloc_sarray((j_common_ptr)cinfo, JPOOL_IMAGE,
					  row_stride, 1);
	bytep = out->bytes + cinfo->output_scanline * out->bytes_per_row;
	jpeg_read_scanlines(cinfo, buffer, 1);
	for (x = 0; x < out->width; ++x) {
		k = x * 3;
		*bytep++ = buffer[0][k + 2];
		*bytep++ = buffer[0][k + 1];
		*bytep++ = buffer[0][k];
	}
}

#endif
