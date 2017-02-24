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
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

typedef struct LCUI_ImageInterfaceRec_ {
	const char *suffix;
	int (*init)(LCUI_ImageReader);
	int (*read_header)(LCUI_ImageReader);
	int (*read)(LCUI_ImageReader, LCUI_Graph*);
} LCUI_ImageInterfaceRec, *LCUI_ImageInterface;

static const LCUI_ImageInterfaceRec interfaces[] = {
	{ ".png", LCUI_InitPNGReader, LCUI_ReadPNGHeader, LCUI_ReadPNG },
	{ ".jpeg .jpg", LCUI_InitJPEGReader, LCUI_ReadJPEGHeader, LCUI_ReadJPEG },
	{ ".bmp", LCUI_InitBMPReader, LCUI_ReadBMPHeader, LCUI_ReadBMP }
};

static int n_interfaces = sizeof(interfaces) / sizeof(LCUI_ImageInterfaceRec);

static size_t FileStream_OnRead( void *data, void *buffer, size_t size )
{
	return fread( buffer, 1, size, data );
}

static void FileStream_OnSkip( void *data, long offset )
{
	fseek( data, offset, SEEK_CUR );
}

static void FileStream_OnRewind( void *data )
{
	rewind( data );
}

void LCUI_SetImageReaderForFile( LCUI_ImageReader reader, FILE *fp )
{
	reader->stream_data = fp;
	reader->fn_skip = FileStream_OnSkip;
	reader->fn_read = FileStream_OnRead;
	reader->fn_rewind = FileStream_OnRewind;
}

static int DetectImageType( const char *filename )
{
	int i;
	for( i = 0; i < n_interfaces; ++i ) {
		if( strstr( filename, interfaces[i].suffix ) ) {
			return i;
		}
	}
	return -1;
}

static int LCUI_InitImageReaderByType( LCUI_ImageReader reader, int type )
{
	int ret;
	reader->fn_rewind( reader->stream_data );
	ret = interfaces[type].init( reader );
	if( ret != 0 ) {
		return -ENODATA;
	}
	if( LCUI_SetImageReaderJump( reader ) ) {
		return -ENODATA;
	}
	return interfaces[type].read_header( reader );
}

int LCUI_InitImageReader( LCUI_ImageReader reader )
{
	int ret, i;
	for( i = 0; i < n_interfaces; ++i ) {
		reader->fn_rewind( reader->stream_data );
		ret = LCUI_InitImageReaderByType( reader, i );
		if( ret == 0 ) {
			return 0;
		}
	}
	reader->fn_rewind( reader->stream_data );
	reader->type = LCUI_UNKNOWN_READER;
	return -ENOENT;
}

void LCUI_DestroyImageReader( LCUI_ImageReader reader )
{
	if( reader->data ) {
		reader->destructor( reader->data );
	}
	reader->data = NULL;
	reader->type = LCUI_UNKNOWN_READER;
	reader->header.type = LCUI_UNKNOWN_IMAGE;
}

int LCUI_ReadImageHeader( LCUI_ImageReader reader )
{
	int i = reader->type - 1;
	if( i < n_interfaces && i >= 0 ) {
		return interfaces[i].read_header( reader );
	}
	return -ENODATA;
}

int LCUI_ReadImage( LCUI_ImageReader reader, LCUI_Graph *out )
{
	int i = reader->type - 1;
	if( i < n_interfaces && i >= 0 ) {
		return interfaces[i].read( reader, out );
	}
	return -ENODATA;
}

int LCUI_ReadImageFile( const char *filepath, LCUI_Graph *out )
{
	int ret;
	FILE *fp;
	LCUI_ImageReaderRec reader = { 0 };

	fp = fopen( filepath, "rb" );
	if( !fp ) {
		return -ENOENT;
	}
	ret = DetectImageType( filepath );
	LCUI_SetImageReaderForFile( &reader, fp );
	if( ret >= 0 ) {
		ret = LCUI_InitImageReaderByType( &reader, ret );
	}
	if( ret < 0 ) {
		if( LCUI_InitImageReader( &reader ) != 0 ) {
			fclose( fp );
			return -ENODATA;
		}
	}
	if( LCUI_SetImageReaderJump( &reader ) ) {
		ret = -ENODATA;
		goto exit;
	}
	ret = LCUI_ReadImage( &reader, out );
exit:
	LCUI_DestroyImageReader( &reader );
	fclose( fp );
	return  ret;
}

int LCUI_GetImageSize( const char *filepath, int *width, int *height )
{
	int ret;
	FILE *fp;
	LCUI_ImageReaderRec reader = { 0 };

	fp = fopen( filepath, "rb" );
	if( !fp ) {
		return -ENOENT;
	}
	ret = DetectImageType( filepath );
	LCUI_SetImageReaderForFile( &reader, fp );
	if( ret >= 0 ) {
		ret = LCUI_InitImageReaderByType( &reader, ret );
	}
	if( ret < 0 ) {
		if( LCUI_InitImageReader( &reader ) != 0 ) {
			fclose( fp );
			return -ENODATA;
		}
	}
	*width = reader.header.width;
	*height = reader.header.height;
	LCUI_DestroyImageReader( &reader );
	fclose( fp );
	return 0;
}

