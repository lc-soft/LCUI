/*
 * lib/pandagl/src/image/reader.c: -- Image read operations set
 *
 * Copyright (c) 2018-2023-2023, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pandagl.h>
#include "bmp.h"
#include "png.h"
#include "jpeg.h"

typedef struct pd_image_interface_t {
	const char *suffix;
	int (*init)(pd_image_reader_t *);
	int (*read_header)(pd_image_reader_t *);
	int (*read)(pd_image_reader_t *, pd_canvas_t *);
} pd_image_interface_t;

static const pd_image_interface_t pd_image_interfaces[] = {
#ifdef PANDAGL_HAS_LIBPNG
	{ ".png", pd_png_reader_init, pd_png_reader_read_header,
	  pd_png_reader_read_data },
#endif
#ifdef PANDAGL_HAS_LIBJPEG
	{ ".jpeg .jpg", pd_jpeg_reader_init, pd_jpeg_reader_read_header,
	  pd_jpeg_reader_read_data },
#endif
	{ ".bmp", pd_bmp_reader_init, pd_bmp_reader_read_header,
	  pd_bmp_reader_read_data }
};

#define INTERFACES_COUNT \
	(sizeof(pd_image_interfaces) / sizeof(pd_image_interface_t))

static size_t pd_file_stream_on_read(void *data, void *buffer, size_t size)
{
	return fread(buffer, 1, size, data);
}

static void pd_file_stream_on_skip(void *data, long offset)
{
	fseek(data, offset, SEEK_CUR);
}

static void pd_file_stream_on_rewind(void *data)
{
	rewind(data);
}

void pd_image_reader_set_file(pd_image_reader_t *reader, FILE *fp)
{
	reader->stream_data = fp;
	reader->fn_skip = pd_file_stream_on_skip;
	reader->fn_read = pd_file_stream_on_read;
	reader->fn_rewind = pd_file_stream_on_rewind;
}

static int pd_detech_image_type(const char *filename)
{
	int i;
	const char *suffix = NULL;

	for (i = 0; filename[i]; ++i) {
		if (filename[i] == '.') {
			suffix = filename + i;
		}
	}
	if (!suffix) {
		return -1;
	}
	for (i = 0; i < INTERFACES_COUNT; ++i) {
		if (strstr(pd_image_interfaces[i].suffix, suffix)) {
			return i;
		}
	}
	return -1;
}

static int pd_image_reader_init_for_type(pd_image_reader_t *reader, int type)
{
	int ret;
	reader->fn_rewind(reader->stream_data);
	ret = pd_image_interfaces[type].init(reader);
	if (ret != 0) {
		return -2;
	}
	if (pd_image_reader_set_jump(reader)) {
		return -2;
	}
	return pd_image_interfaces[type].read_header(reader);
}

int pd_image_reader_init(pd_image_reader_t *reader)
{
	int ret, i;
	for (i = 0; i < INTERFACES_COUNT; ++i) {
		reader->fn_rewind(reader->stream_data);
		ret = pd_image_reader_init_for_type(reader, i);
		if (ret == 0) {
			return 0;
		}
		pd_image_reader_destroy(reader);
	}
	reader->fn_rewind(reader->stream_data);
	reader->type = PD_UNKNOWN_READER;
	return -ENOENT;
}

void pd_image_reader_destroy(pd_image_reader_t *reader)
{
	if (reader->data) {
		reader->destructor(reader->data);
	}
	reader->data = NULL;
	reader->type = PD_UNKNOWN_READER;
	reader->header.type = PD_UNKNOWN_IMAGE;
}

int pd_image_reader_read_header(pd_image_reader_t *reader)
{
	int i = reader->type - 1;
	if (reader->header.type != PD_UNKNOWN_IMAGE) {
		return 0;
	}
	if (i < INTERFACES_COUNT && i >= 0) {
		return pd_image_interfaces[i].read_header(reader);
	}
	return -2;
}

int pd_image_reader_read_data(pd_image_reader_t *reader, pd_canvas_t *out)
{
	int i = reader->type - 1;
	if (i < INTERFACES_COUNT && i >= 0) {
		return pd_image_interfaces[i].read(reader, out);
	}
	return -2;
}

int pd_read_image_from_file(const char *filepath, pd_canvas_t *out)
{
	int ret;
	FILE *fp;
	pd_image_reader_t reader = { 0 };

	fp = fopen(filepath, "rb");
	if (!fp) {
		return -ENOENT;
	}
	ret = pd_detech_image_type(filepath);
	pd_image_reader_set_file(&reader, fp);
	if (ret >= 0) {
		ret = pd_image_reader_init_for_type(&reader, ret);
	}
	if (ret < 0) {
		if (pd_image_reader_init(&reader) != 0) {
			fclose(fp);
			return -2;
		}
	}
	if (pd_image_reader_set_jump(&reader)) {
		ret = -2;
	} else {
		ret = pd_image_reader_read_data(&reader, out);
	}
	pd_image_reader_destroy(&reader);
	fclose(fp);
	return ret;
}

int pd_read_image_size_from_file(const char *filepath, int *width, int *height)
{
	int ret;
	FILE *fp;
	pd_image_reader_t reader = { 0 };

	fp = fopen(filepath, "rb");
	if (!fp) {
		return -ENOENT;
	}
	ret = pd_detech_image_type(filepath);
	pd_image_reader_set_file(&reader, fp);
	if (ret >= 0) {
		ret = pd_image_reader_init_for_type(&reader, ret);
	}
	if (ret < 0) {
		if (pd_image_reader_init(&reader) != 0) {
			fclose(fp);
			return -2;
		}
	}
	*width = reader.header.width;
	*height = reader.header.height;
	pd_image_reader_destroy(&reader);
	fclose(fp);
	return 0;
}
