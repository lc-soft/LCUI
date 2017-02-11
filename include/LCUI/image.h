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

typedef size_t( *LCUI_ImageReaderFunction )(void*, void*, size_t);

enum LCUI_ImageReaderType {
	LCUI_UNKNOWN_READER,
	LCUI_PNG_READER,
	LCUI_JPEG_READER,
	LCUI_BMP_READER
};

typedef struct LCUI_ImageReaderRec {
	void *stream_data;
	LCUI_BOOL has_error;
	LCUI_ImageReaderFunction func;

	int type;
	void *data;
	void( *destructor )(void*);
} LCUI_ImageReaderRec, *LCUI_ImageReader;

/** 创建适用于 PNG 图像的读取器 */
LCUI_API LCUI_ImageReader LCUI_CreatePNGReader( void *data,
						LCUI_ImageReaderFunction func );

/** 创建图像读取器 */
LCUI_API LCUI_ImageReader LCUI_CreateImageReader( void *data,
						  LCUI_ImageReaderFunction func );


/** 销毁图像读取器 */
LCUI_API void LCUI_DestroyImageReader( LCUI_ImageReader reader );

LCUI_API int LCUI_ReadPNG( LCUI_ImageReader reader, LCUI_Graph *graph );

LCUI_API int LCUI_ReadImage( LCUI_ImageReader reader, LCUI_Graph *out );

/** 从BMP文件中读取图像数据 */
LCUI_API int LCUI_ReadBMPFile( const char *filepath, LCUI_Graph *out );

/** 从PNG文件中读取图像数据 */
LCUI_API int LCUI_ReadPNGFile( const char *filepath, LCUI_Graph *graph );

/** 从JPEG文件中读取图像数据 */
LCUI_API int LCUI_ReadJPEGFile( const char *filepath, LCUI_Graph *buf );

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
