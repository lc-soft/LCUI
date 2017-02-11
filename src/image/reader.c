/* ***************************************************************************
 * reader.c -- Image read operations set
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
 * reader.c -- 图像的读操作集
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h> 

/** 检测图片格式，并解码图片 */
static int LCUI_DetectImage( const char *filepath, LCUI_Graph *out )
{
	int ret = -1;
	if( ret == -1 ) {
		ret = LCUI_ReadPNGFile( filepath, out );
	}
	if( ret == -1 ) {
		ret = LCUI_ReadJPEGFile( filepath, out );
	}
	if( ret == -1 ) {
		ret = LCUI_ReadBMPFile( filepath, out );
	}
	return ret;
}

LCUI_ImageReader LCUI_CreateImageReader( void *data,
					 LCUI_ImageReaderFunction func )
{
	LCUI_ImageReader reader;
	reader = LCUI_CreatePNGReader( data, func );
	if( reader ) {
		return reader;
	}
	return reader;
}

void LCUI_DestroyImageReader( LCUI_ImageReader reader )
{
	reader->destructor( reader->data );
	free( reader );
}

int LCUI_ReadImage( LCUI_ImageReader reader, LCUI_Graph *out )
{
	switch( reader->type ) {
	case LCUI_PNG_READER:
		return LCUI_ReadPNG( reader, out );
	case LCUI_JPEG_READER:
	case LCUI_BMP_READER:
	default: break;
	}
	return -ENODATA;
}

int LCUI_ReadImageFile( const char *filepath, LCUI_Graph *out )
{
	FILE *fp;
	int ret = 0;
	Graph_Init( out );
	out->color_type = COLOR_TYPE_RGB;
	fp = fopen( filepath, "rb" );
	if( fp == NULL ) {
		return ENOENT;
	}
	fgetc( fp );
	if( !ferror( fp ) ) {
		fseek( fp, 0, SEEK_END );
		if( ftell( fp ) > 4 ) {
			fclose( fp );
			ret = LCUI_DetectImage( filepath, out );
		} else {
			ret = -1;
			fclose( fp );
		}
	}
	return ret;
}

int LCUI_GetImageSize( const char *filepath, int *width, int *height )
{
	int ret = Graph_GetPNGSize( filepath, width, height );
	if( ret != 0 ) {
		ret = Graph_GetJPEGSize( filepath, width, height );
	}
	if( ret != 0 ) {
		ret = Graph_GetBMPSize( filepath, width, height );
	}
	return ret;
}
