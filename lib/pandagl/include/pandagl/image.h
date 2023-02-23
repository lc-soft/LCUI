/*
 * image.h -- Image read and write operations set
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

#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_IMAGE_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_IMAGE_H

#include <setjmp.h>
#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

typedef void (*pd_image_progress_func_t)(void *, float);
typedef size_t (*pd_image_read_func_t)(void *, void *, size_t);
typedef void (*pd_image_skip_func_t)(void *, long);
typedef void (*pd_image_func_t)(void *);

/** 图像读取器的类型 */
typedef enum pd_image_reader_type_t {
	PD_UNKNOWN_READER,
	PD_PNG_READER,
	PD_JPEG_READER,
	PD_BMP_READER
} pd_image_reader_type_t;

#define PD_UNKNOWN_IMAGE PD_UNKNOWN_READER
#define PD_PNG_IMAGE PD_PNG_READER
#define PD_JPEG_IMAGE PD_JPEG_READER
#define PD_BMP_IMAGE PD_BMP_READER

#define pd_image_reader_set_jump(READER) \
	(READER)->env &&setjmp(*((READER)->env))

typedef struct pd_image_header_t {
	int type;
	int bit_depth;
	int color_type;
	unsigned int width, height;
} pd_image_header_t;

/** 图像读取器 */
typedef struct pd_image_reader_t {
	/** 自定义的输入流数据 */
	void *stream_data;

	/** 是否有错误 */
	unsigned char has_error;

	/** 图像头部信息 */
	pd_image_header_t header;

	/** 在开始读取前调用的函数 */
	pd_image_func_t fn_begin;

	/** 在结束读取时调用的函数 */
	pd_image_func_t fn_end;

	/** 游标重置函数，用于重置当前读取位置到数据流的开头处 */
	pd_image_func_t fn_rewind;

	/** 数据读取函数，用于从数据流中读取数据 */
	pd_image_read_func_t fn_read;

	/** 游标移动函数，用于跳过一段数据 */
	pd_image_skip_func_t fn_skip;

	/** 用于接收图像读取进度的函数 */
	pd_image_progress_func_t fn_prog;

	/** 接收图像读取进度时的附加参数 */
	void *prog_arg;

	/** 图片读取器类型 */
	int type;

	/** 私有数据 */
	void *data;

	/** 私有数据的析构函数 */
	void (*destructor)(void *);

	/** 堆栈环境缓存的指针，用于 setjump() */
	jmp_buf *env;

	/** 默认堆栈环境缓存 */
	jmp_buf env_src;
} pd_image_reader_t;

/** 初始化适用于 PNG 图像的读取器 */
PD_PUBLIC int pd_png_reader_init(pd_image_reader_t *reader);

/** 初始化适用于 JPEG 图像的读取器 */
PD_PUBLIC int pd_jpeg_reader_init(pd_image_reader_t *reader);

/** 初始化适用于 BMP 图像的读取器 */
PD_PUBLIC int pd_bmp_reader_init(pd_image_reader_t *reader);

PD_PUBLIC void pd_image_reader_set_file(pd_image_reader_t *reader, FILE *fp);

/** 创建图像读取器 */
PD_PUBLIC int pd_image_reader_init(pd_image_reader_t *reader);

/** 销毁图像读取器 */
PD_PUBLIC void pd_image_reader_destroy(pd_image_reader_t *reader);

PD_PUBLIC int pd_png_reader_read_header(pd_image_reader_t *reader);

PD_PUBLIC int pd_jpeg_reader_read_header(pd_image_reader_t *reader);

PD_PUBLIC int pd_bmp_reader_read_header(pd_image_reader_t *reader);

PD_PUBLIC int pd_image_reader_read_header(pd_image_reader_t *reader);

PD_PUBLIC int pd_png_reader_read_data(pd_image_reader_t *reader,
				     pd_canvas_t *graph);

PD_PUBLIC int pd_bmp_reader_read_data(pd_image_reader_t *reader,
				     pd_canvas_t *graph);

PD_PUBLIC int pd_image_reader_read_data(pd_image_reader_t *reader,
				       pd_canvas_t *graph);

/** 将图像数据写入至png文件 */
PD_PUBLIC int pd_write_png_file(const char *file_name, const pd_canvas_t *graph);

/** 载入指定图片文件的图像数据 */
PD_PUBLIC int pd_read_image_from_file(const char *filepath, pd_canvas_t *out);

/** 从文件中获取图像尺寸 */
PD_PUBLIC int pd_read_image_size_from_file(const char *filepath, int *width,
					  int *height);

PD_END_DECLS

#endif
