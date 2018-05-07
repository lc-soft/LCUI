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

#ifndef LCUI_IMAGE_H
#define LCUI_IMAGE_H

#include <setjmp.h>
#include <LCUI/graph.h>

LCUI_BEGIN_HEADER

typedef void(*LCUI_ImageProgressFunc)(void*, float);
typedef size_t(*LCUI_ImageReadFunc)(void*, void*, size_t);
typedef void(*LCUI_ImageSkipFunc)(void*, long);
typedef void(*LCUI_ImageFunc)(void*);

/** 图像读取器的类型 */
enum LCUI_ImageReaderType {
	LCUI_UNKNOWN_READER,
	LCUI_PNG_READER,
	LCUI_JPEG_READER,
	LCUI_BMP_READER
};

#define LCUI_UNKNOWN_IMAGE	LCUI_UNKNOWN_READER
#define LCUI_PNG_IMAGE		LCUI_PNG_READER
#define LCUI_JPEG_IMAGE		LCUI_JPEG_READER
#define LCUI_BMP_IMAGE		LCUI_BMP_READER

#define LCUI_SetImageReaderJump(READER) (READER)->env && setjmp(*((READER)->env))

typedef struct LCUI_ImageHeaderRec_ {
	int type;
	int bit_depth;
	int color_type;
	unsigned int width, height;
} LCUI_ImageHeaderRec, *LCUI_ImageHeader;

/** 图像读取器 */
typedef struct LCUI_ImageReaderRec_ {
	void *stream_data;			/**< 自定义的输入流数据 */
	LCUI_BOOL has_error;			/**< 是否有错误 */
	LCUI_ImageHeaderRec header;		/**< 图像头部信息 */
	LCUI_ImageFunc fn_begin;		/**< 在开始读取前调用的函数 */
	LCUI_ImageFunc fn_end;			/**< 在结束读取时调用的函数 */
	LCUI_ImageFunc fn_rewind;		/**< 游标重置函数，用于重置当前读取位置到数据流的开头处 */
	LCUI_ImageReadFunc fn_read;		/**< 数据读取函数，用于从数据流中读取数据 */
	LCUI_ImageSkipFunc fn_skip;		/**< 游标移动函数，用于跳过一段数据 */
	LCUI_ImageProgressFunc fn_prog;		/**< 用于接收图像读取进度的函数 */
	void *prog_arg;				/**< 接收图像读取进度时的附加参数 */

	int type;				/**< 图片读取器类型 */
	void *data;				/**< 私有数据 */
	void(*destructor)(void*);		/**< 私有数据的析构函数 */
	jmp_buf *env;				/**< 堆栈环境缓存的指针，用于 setjump() */
	jmp_buf env_src;			/**< 默认堆栈环境缓存 */
} LCUI_ImageReaderRec, *LCUI_ImageReader;

/** 初始化适用于 PNG 图像的读取器 */
LCUI_API int LCUI_InitPNGReader(LCUI_ImageReader reader);

/** 初始化适用于 JPEG 图像的读取器 */
LCUI_API int LCUI_InitJPEGReader(LCUI_ImageReader reader);

/** 初始化适用于 BMP 图像的读取器 */
LCUI_API int LCUI_InitBMPReader(LCUI_ImageReader reader);

LCUI_API void LCUI_SetImageReaderForFile(LCUI_ImageReader reader, FILE *fp);

/** 创建图像读取器 */
LCUI_API int LCUI_InitImageReader(LCUI_ImageReader reader);

/** 销毁图像读取器 */
LCUI_API void LCUI_DestroyImageReader(LCUI_ImageReader reader);

LCUI_API int LCUI_ReadPNGHeader(LCUI_ImageReader reader);

LCUI_API int LCUI_ReadJPEGHeader(LCUI_ImageReader reader);

LCUI_API int LCUI_ReadBMPHeader(LCUI_ImageReader reader);

LCUI_API int LCUI_ReadImageHeader(LCUI_ImageReader reader);

LCUI_API int LCUI_ReadPNG(LCUI_ImageReader reader, LCUI_Graph *graph);

LCUI_API int LCUI_ReadJPEG(LCUI_ImageReader reader, LCUI_Graph *graph);

LCUI_API int LCUI_ReadBMP(LCUI_ImageReader reader, LCUI_Graph *graph);

LCUI_API int LCUI_ReadImage(LCUI_ImageReader reader, LCUI_Graph *graph);

/** 将图像数据写入至png文件 */
LCUI_API int LCUI_WritePNGFile(const char *file_name, const LCUI_Graph *graph);

/** 载入指定图片文件的图像数据 */
LCUI_API int LCUI_ReadImageFile(const char *filepath, LCUI_Graph *out);

/** 从文件中获取图像尺寸 */
LCUI_API int LCUI_GetImageSize(const char *filepath, int *width, int *height);

LCUI_END_HEADER

#endif
