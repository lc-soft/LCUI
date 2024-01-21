/*
 * lib/pandagl/src/image/png.c: -- LCUI PNG image file processing module.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pandagl.h>
#include "png-private.h"

#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(X) \
	if (!(X)) \
		goto error;

#ifdef PANDAGL_HAS_LIBPNG
#include <png.h>
#include <pandagl.h>

#define PNG_BYTES_TO_CHECK 4

typedef struct pd_png_reader_t {
	png_structp png_ptr;
	png_infop info_ptr;
} pd_png_reader_t;

static void DestroyPNGReader(void *data)
{
	pd_png_reader_t *reader = data;
	if (reader->png_ptr) {
		png_destroy_read_struct(&reader->png_ptr, &reader->info_ptr,
					NULL);
	}
	free(reader);
}

static void PNGReader_OnRead(png_structp png_ptr, png_bytep buffer,
			     png_size_t size)
{
	size_t read_size;
	pd_image_reader_t *reader = png_get_io_ptr(png_ptr);
	if (!reader || reader->has_error) {
		return;
	}
	read_size = reader->fn_read(reader->stream_data, buffer, size);
	if (read_size != size) {
		reader->has_error = PD_TRUE;
		if (reader->fn_end) {
			reader->fn_end(reader->stream_data);
		}
	}
}
#endif

int pd_png_reader_init(pd_image_reader_t *reader)
{
#ifdef PANDAGL_HAS_LIBPNG
	pd_png_reader_t *png_reader;

	png_reader = malloc(sizeof(pd_png_reader_t));
	png_reader->png_ptr =
	    png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	ASSERT(png_reader->png_ptr);
	png_reader->info_ptr = png_create_info_struct(png_reader->png_ptr);
	ASSERT(png_reader->info_ptr);
	png_set_read_fn(png_reader->png_ptr, reader, PNGReader_OnRead);
	reader->destructor = DestroyPNGReader;
	reader->has_error = PD_FALSE;
	reader->data = png_reader;
	reader->type = PD_PNG_READER;
	reader->header.type = PD_UNKNOWN_IMAGE;
	reader->env = &png_jmpbuf(png_reader->png_ptr);
	return 0;

error:
	pd_image_reader_destroy(reader);
#else
	logger_warning("warning: not PNG support!");
#endif
	return -1;
}

int pd_png_reader_read_header(pd_image_reader_t *reader)
{
#ifdef PANDAGL_HAS_LIBPNG
	size_t n;
	png_infop info_ptr;
	png_structp png_ptr;
	png_byte buf[PNG_BYTES_TO_CHECK];
	pd_png_reader_t *png_reader;
	if (reader->type != PD_PNG_READER) {
		return -EINVAL;
	}
	png_reader = reader->data;
	png_ptr = png_reader->png_ptr;
	info_ptr = png_reader->info_ptr;
	n = reader->fn_read(reader->stream_data, buf, PNG_BYTES_TO_CHECK);
	if (n < PNG_BYTES_TO_CHECK) {
		return -2;
	}
	/* 检测数据是否为PNG的签名 */
	if (!png_check_sig(buf, PNG_BYTES_TO_CHECK)) {
		return -2;
	}
	png_set_sig_bytes(png_reader->png_ptr, PNG_BYTES_TO_CHECK);
	/* 读取PNG图片信息 */
	png_read_info(png_ptr, info_ptr);
	reader->header.width = png_get_image_width(png_ptr, info_ptr);
	reader->header.height = png_get_image_height(png_ptr, info_ptr);
	reader->header.bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	reader->header.color_type = png_get_color_type(png_ptr, info_ptr);
	reader->header.type = PD_PNG_IMAGE;
	switch (reader->header.color_type) {
	case PNG_COLOR_TYPE_RGB_ALPHA:
		reader->header.color_type = PD_COLOR_TYPE_ARGB;
		break;
	case PNG_COLOR_TYPE_RGB:
		reader->header.color_type = PD_COLOR_TYPE_RGB;
		break;
	default:
		reader->header.color_type = 0;
		break;
	}
	return 0;
#else
	logger_warning("warning: not PNG support!");
	return -ENOSYS;
#endif
}

int pd_png_reader_read_data(pd_image_reader_t *reader, pd_canvas_t *graph)
{
#ifdef PANDAGL_HAS_LIBPNG
	png_uint_32 i;
	png_bytep row;
	png_infop info_ptr;
	png_structp png_ptr;
	pd_image_header_t *header;
	pd_png_reader_t *png_reader;
	int pass, number_passes, ret = 0;
	float progress;

	if (reader->type != PD_PNG_READER) {
		return -EINVAL;
	}
	header = &reader->header;
	png_reader = reader->data;
	png_ptr = png_reader->png_ptr;
	info_ptr = png_reader->info_ptr;
	if (header->type == PD_UNKNOWN_IMAGE) {
		if (pd_png_reader_read_header(reader) != 0) {
			return -2;
		}
	}
	/* 根据不同的色彩类型进行相应处理 */
	switch (header->color_type) {
	case PD_COLOR_TYPE_ARGB:
		graph->color_type = PD_COLOR_TYPE_ARGB;
		ret = pd_canvas_create(graph, header->width, header->height);
		if (ret != 0) {
			ret = -ENOMEM;
			break;
		}
		break;
	case PD_COLOR_TYPE_RGB:
		graph->color_type = PD_COLOR_TYPE_RGB;
		ret = pd_canvas_create(graph, header->width, header->height);
		if (ret != 0) {
			ret = -ENOMEM;
			break;
		}
		break;
	default:
		/* 其它色彩类型的图像就不处理了 */
		return -2;
	}
	png_set_bgr(png_ptr);
	png_set_expand(png_ptr);
	number_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	for (pass = 0; pass < number_passes; ++pass) {
		for (i = 0; i < graph->height; ++i) {
			row = graph->bytes + i * graph->bytes_per_row;
			png_read_row(png_ptr, row, NULL);
			if (reader->fn_prog) {
				progress = 100.0f * i / graph->height;
				reader->fn_prog(reader->prog_arg, progress);
			}
		}
	}
	return ret;
#else
	logger_warning("warning: not PNG support!");
	return -ENOSYS;
#endif
}

int pd_write_png_file(const char *file_name, const pd_canvas_t *graph)
{
#ifdef PANDAGL_HAS_LIBPNG

	png_byte color_type;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers;

	FILE *fp;
	unsigned char *p;
	int y, x;
	size_t row_size;
	pd_rect_t rect;

	if (!pd_canvas_is_valid(graph)) {
		logger_error("canvas is not valid\n");
		return -1;
	}
	/* create file */
	fp = fopen(file_name, "wb");
	if (!fp) {
		logger_error("file %s could not be opened for writing\n",
			     file_name);
		return -1;
	}
	/* initialize stuff */
	png_ptr =
	    png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		logger_error("png_create_write_struct failed\n");
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		fclose(fp);
		logger_error("png_create_info_struct failed\n");
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return -1;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		logger_error("error during init_io\n");
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return -1;
	}
	png_init_io(png_ptr, fp);
	if (graph->color_type == PD_COLOR_TYPE_ARGB8888) {
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	} else {
		color_type = PNG_COLOR_TYPE_RGB;
	}
	/* write header */
	png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height, 8,
		     color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		     PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);
	/* write bytes */

	pd_canvas_get_quote_rect(graph, &rect);
	graph = pd_canvas_get_quote_source_readonly(graph);
	row_size = png_get_rowbytes(png_ptr, info_ptr);
	row_pointers = (png_bytep *)malloc(rect.height * sizeof(png_bytep));

	for (y = 0; y < rect.height; ++y) {
		p = pd_canvas_pixel_at(graph, rect.x, rect.y + y);
		row_pointers[y] = png_malloc(png_ptr, row_size);
		if (graph->color_type == PD_COLOR_TYPE_ARGB) {
			for (x = 0; x < row_size; p += 4) {
				row_pointers[y][x++] = p[2];    // r
				row_pointers[y][x++] = p[1];    // g
				row_pointers[y][x++] = p[0];    // b
				row_pointers[y][x++] = p[3];    // a
			}
		} else {
			for (x = 0; x < row_size; p += 3) {
				row_pointers[y][x++] = p[2];    // r
				row_pointers[y][x++] = p[1];    // g
				row_pointers[y][x++] = p[0];    // b
			}
		}
	}
	png_write_image(png_ptr, row_pointers);
	/* cleanup heap allocation */
	for (y = 0; y < rect.height; ++y) {
		free(row_pointers[y]);
	}
	free(row_pointers);
	/* end write */
	png_write_end(png_ptr, NULL);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
#else
	logger_warning("warning: not PNG support!");
	return -ENOSYS;
#endif
}
