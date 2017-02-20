/* ***************************************************************************
 * image.h -- Image read and write operations set
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
 * ****************************************************************************/

/* ****************************************************************************
 * image.h -- 图像读写操作集
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
#ifndef LCUI_IMAGE_H
#define LCUI_IMAGE_H

#include <LCUI/graph.h>

LCUI_BEGIN_HEADER

typedef size_t( *LCUI_ImageReadFunction )(void*, void*, size_t);
typedef void( *LCUI_ImageSkipFunction )(void*, long);
typedef void( *LCUI_ImageFunction )(void*);

/** 图像读取器的类型 */
enum LCUI_ImageReaderType {
	LCUI_UNKNOWN_READER,
	LCUI_PNG_READER,
	LCUI_JPEG_READER,
	LCUI_BMP_READER
};

/** 图像读取器 */
typedef struct LCUI_ImageReaderRec_ {
	void *stream_data;		/**< 自定义的输入流数据 */
	LCUI_BOOL has_error;		/**< 是否有错误 */
	LCUI_ImageFunction fn_begin;	/**< 在开始读取前调用的函数 */
	LCUI_ImageFunction fn_end;	/**< 在结束读取时调用的函数 */
	LCUI_ImageFunction fn_rewind;	/**< 游标重置函数，用于重置当前读取位置到数据流的开头处 */
	LCUI_ImageReadFunction fn_read;	/**< 数据读取函数，用于从数据流中读取数据 */
	LCUI_ImageSkipFunction fn_skip;	/**< 游标移动函数，用于跳过一段数据 */

	int type;			/**< 图片读取器类型 */
	void *data;			/**< 私有数据 */
	void( *destructor )(void*);	/**< 私有数据的析构函数 */
} LCUI_ImageReaderRec, *LCUI_ImageReader;

/** 初始化适用于 PNG 图像的读取器 */
LCUI_API int LCUI_InitPNGReader( LCUI_ImageReader reader );

/** 初始化适用于 JPEG 图像的读取器 */
LCUI_API int LCUI_InitJPEGReader( LCUI_ImageReader reader );

/** 初始化适用于 BMP 图像的读取器 */
LCUI_API int LCUI_InitBMPReader( LCUI_ImageReader reader );

/** 创建图像读取器 */
LCUI_API int LCUI_InitImageReader( LCUI_ImageReader reader );

/** 销毁图像读取器 */
LCUI_API void LCUI_DestroyImageReader( LCUI_ImageReader reader );

LCUI_API int LCUI_ReadPNG( LCUI_ImageReader reader, LCUI_Graph *graph );

LCUI_API int LCUI_ReadJPEG( LCUI_ImageReader reader, LCUI_Graph *graph );

LCUI_API int LCUI_ReadBMP( LCUI_ImageReader reader, LCUI_Graph *graph );

LCUI_API int LCUI_ReadImage( LCUI_ImageReader reader, LCUI_Graph *graph );

/** 从BMP文件中读取图像数据 */
LCUI_API int LCUI_ReadBMPFile( const char *filepath, LCUI_Graph *out );

/** 从PNG文件中读取图像数据 */
LCUI_API int LCUI_ReadPNGFile( const char *filepath, LCUI_Graph *graph );

/** 从JPEG文件中读取图像数据 */
LCUI_API int LCUI_ReadJPEGFile( const char *filepath, LCUI_Graph *graph );

/** 将图像数据写入至png文件 */
LCUI_API int LCUI_WritePNGFile( const char *file_name, const LCUI_Graph *graph );

/** 从BMP文件中获取图像尺寸 */
LCUI_API int Graph_GetBMPSize( const char *filepath, int *width, int *height );

/** 从PNG文件中获取图像尺寸 */
LCUI_API int Graph_GetPNGSize( const char *filepath, int *width, int *height );

/** 从JPEG文件中读取图像尺寸 */
LCUI_API int Graph_GetJPEGSize( const char *filepath, int *width, int *height );

/** 载入指定图片文件的图像数据 */
LCUI_API int LCUI_ReadImageFile( const char *filepath, LCUI_Graph *out );

/** 从文件中获取图像尺寸 */
LCUI_API int LCUI_GetImageSize( const char *filepath, int *width, int *height );

LCUI_END_HEADER

#endif
