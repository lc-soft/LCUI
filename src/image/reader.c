/*
 * reader.c -- Image read operations set
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
		LCUI_DestroyImageReader( reader );
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
	if( reader->header.type != LCUI_UNKNOWN_IMAGE ) {
		return 0;
	}
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

