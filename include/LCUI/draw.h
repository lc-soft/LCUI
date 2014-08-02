/* ****************************************************************************
 * LCUI_Draw.h -- The graphics draw module of LCUI.
 * 
 * Copyright (C) 2012 by
 * Liu Chao
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
 * LCUI_Draw.h -- LCUI的图形绘制模块
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
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
 * ****************************************************************************/

#ifndef __LCUI_DRAW_H__
#define __LCUI_DRAW_H__

LCUI_BEGIN_HEADER
	
/* 打开文件时的错误 */
#define FILE_ERROR_OPEN_ERROR		-1
#define FILE_ERROR_SHORT_FILE		-2
#define FILE_ERROR_BIG_FILE		-3
#define FILE_ERROR_UNKNOWN_FORMAT	-4
#define FILE_ERROR_MALLOC_ERROR		-5

#include LC_DRAW_BORDER_H
#include LC_DRAW_LINE_H
#include LC_DRAW_BOXSHADOW_H

LCUI_API int
Graph_Rotate(LCUI_Graph *src, int rotate_angle, LCUI_Graph *des);
/* 
 * 功能：旋转图形
 * 说明：指定旋转中心点坐标以及旋转角度，即可得到旋转后的图形
 * 本源代码参考自互联网相关代码
 * 算法有待优化完善。
 */

/* 载入BMP图片文件 */
LCUI_API int Graph_LoadBMP( const char *filepath, LCUI_Graph *out );

/* 载入jpeg图片文件 */
LCUI_API int Graph_LoadJPEG( const char *filepath, LCUI_Graph *out );

/* 载入png图片文件 */
LCUI_API int Graph_LoadPNG( const char *filepath, LCUI_Graph *out );

/* 将图像数据写入至png文件 */
LCUI_API int Graph_WritePNG( const char *file_name, LCUI_Graph *graph );

/* 载入指定图片文件的图像数据 */
LCUI_API int Graph_LoadImage( const char *filepath, LCUI_Graph *out );

LCUI_API int
GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma );
/* 对图像进行高斯模糊处理 */ 

LCUI_API int
Graph_Smooth( LCUI_Graph *src, LCUI_Graph *des, double sigma );
/* 对图像进行模糊处理 */

LCUI_END_HEADER

#endif
