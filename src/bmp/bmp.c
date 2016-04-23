/* ***************************************************************************
 * bmp.c -- LCUI BMP image file processing module.
 * 
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * bmp.c -- LCUI的BMP位图文件读写支持模块。
 *
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

/* 这个结构体用于存储bmp文件的文件头的信息 */
typedef struct bmp_head {
	short int BMPsyg;
	short int nic[8];
	short int ix;
	short int nic2;
	short int iy;
	short int nic3[2];
	short int depth;
	short int rle;
	short int nic4[11];
} bmp_head;

int Graph_LoadBMP( const char *filepath, LCUI_Graph *out )
{
	bmp_head bmp;
	uchar_t *bytep;
	int  x, y, tempi, pocz, omin;
	FILE *fp = fopen( filepath, "rb" );
	if( !fp ) {
		return FILE_ERROR_OPEN_ERROR;
	}
	/* 检测是否为bmp图片 */
	tempi = fread( &bmp, 1, sizeof( bmp_head ), fp );
	if( tempi < sizeof( bmp_head ) || bmp.BMPsyg != 19778 ) {
		return FILE_ERROR_UNKNOWN_FORMAT;
	}
	pocz = bmp.nic[4];
	if( (bmp.depth != 32) && (bmp.depth != 24) ) {
		_DEBUG_MSG( "can not support  %i bit-depth !\n", bmp.depth );
		return  FILE_ERROR_UNKNOWN_FORMAT;
	}
	out->color_type = COLOR_TYPE_RGB;
	tempi = Graph_Create( out, bmp.ix, bmp.iy );
	if( tempi != 0 ) {
		_DEBUG_MSG( "can not alloc memory\n" );
		return 1;
	}
	fseek( fp, 0, SEEK_END );
	omin = ftell( fp );
	omin = omin - pocz;
	omin = omin - ((out->w*out->h)*(bmp.depth / 8));
	omin = omin / (out->h);
	fseek( fp, pocz, SEEK_SET );
	switch( bmp.depth ) {
	case 32:
		for( y = 0; y < out->h; ++y ) {
			/* 从最后一行开始写入像素数据 */
			bytep = out->bytes + ((out->h - y - 1)*out->w) * 3;
			for( x = 0; x < out->w; ++x ) {
				*bytep++ = fgetc( fp );
				*bytep++ = fgetc( fp );
				*bytep++ = fgetc( fp );
				tempi = fgetc( fp );
			}
			/* 略过填充字符 */
			if( omin > 0 ) {
				for( tempi = 0; tempi < omin; tempi++ ) {
					fgetc( fp );
				}
			}
		}
		break;
	case 24:
		for( y = 0; y < out->h; ++y ) {
			bytep = out->bytes + ((out->h - y - 1)*out->w) * 3;
			for( x = 0; x < out->w; ++x ) {
				*bytep++ = fgetc( fp );
				*bytep++ = fgetc( fp );
				*bytep++ = fgetc( fp );
			}
			if( omin > 0 ) {
				for( tempi = 0; tempi < omin; tempi++ ) {
					fgetc( fp );
				}
			}
		}
		break;
	}
	fclose( fp );
	return 0;
}

int Graph_GetBMPSize( const char *filepath, int *width, int *height )
{
	int n;
	bmp_head bmp;
	FILE *fp = fopen( filepath, "rb" );
	if( !fp ) {
		return FILE_ERROR_OPEN_ERROR;
	}
	/* 检测是否为bmp图片 */
	n = fread( &bmp, 1, sizeof( bmp_head ), fp );
	if( n < sizeof( bmp_head ) || bmp.BMPsyg != 19778 ) {
		return FILE_ERROR_UNKNOWN_FORMAT;
	}
	*width = bmp.ix;
	*height = bmp.iy;
	fclose( fp );
	return 0;
}
