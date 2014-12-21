/* ***************************************************************************
 * picturebox.h -- LCUI's PictureBox widget
 * 
 * Copyright (C) 2012-2013 by
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
 * picturebox.h -- LCUI 的图像框部件
 *
 * 版权所有 (C) 2012-2013 归属于
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

#ifndef __LCUI_PICTURE_BOX_H_
#define __LCUI_PICTURE_BOX_H_

LCUI_BEGIN_HEADER
	
/****************** 图像的处理方式 *****************/
/* 缩放，缩放比例随着PictureBox部件的尺寸的改变而改变 */
#define SIZE_MODE_ZOOM		LAYOUT_ZOOM

/* 固定缩放，用于照片查看器，PictureBox部件的尺寸的改变不影响缩放比列 */
#define SIZE_MODE_BLOCK_ZOOM	5

#define SIZE_MODE_STRETCH	LAYOUT_STRETCH	/* 拉伸 */
#define SIZE_MODE_CENTER	LAYOUT_CENTER	/* 居中 */
#define SIZE_MODE_TILE		LAYOUT_TILE	/* 平铺 */
#define SIZE_MODE_NONE		LAYOUT_NONE	/* 无 */
#define SIZE_MODE_NORMAL	LAYOUT_NORMAL
/*************************************************/

/* 获取图片框的图片显示的区域 */
LCUI_API void PictureBox_GetViewArea( LCUI_Widget widget, LCUI_Rect *rect );

/** 获取缩放比例 */
LCUI_API float PictureBox_GetScale( LCUI_Widget widget );

/* 设定PictureBox部件显示的图像 */
LCUI_API void
PictureBox_SetImage( LCUI_Widget widget, LCUI_Graph *image );

/* 设定图片文件中的图像为PictureBox部件显示的图像 */
LCUI_API int
PictureBox_SetImageFile( LCUI_Widget widget, char *image_file );

/* 设定当加载图像失败时显示的图像 */
LCUI_API int
PictureBox_SetErrorImage( LCUI_Widget widget, LCUI_Graph *pic );

/* 设定正在加载另一图像时显示的图像 */
LCUI_API int
PictureBox_SetInitImage( LCUI_Widget widget, LCUI_Graph *pic );

/* 设定图像显示模式 */
LCUI_API void
PictureBox_SetSizeMode( LCUI_Widget widget, int mode );

/* 设定PictureBox部件的图片显示区域的大小 */
LCUI_API void
PictureBox_ResizeViewArea( LCUI_Widget widget, int width, int height );

/* 获取PictureBox部件内的图像 */
LCUI_API LCUI_Graph*
PictureBox_GetImage( LCUI_Widget widget );

/* 移动PictureBox部件内的图片的显示区域的位置 */
LCUI_API int
PictureBox_MoveViewArea( LCUI_Widget widget, LCUI_Pos des_pos );

/* 缩放PictureBox部件的图片浏览区域 */
LCUI_API int
PictureBox_ZoomViewArea( LCUI_Widget widget, double scale );

LCUI_END_HEADER

#endif

