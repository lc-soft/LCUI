/* ***************************************************************************
 * picturebox.h -- LCUI's PictureBox widget
 * 
 * Copyright (C) 2013 by
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
 * 版权所有 (C) 2013 归属于
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

typedef struct _LCUI_PictureBox	LCUI_PictureBox;
/******************* 图像状态 *******************/
#define IMAGE_STATE_LOADING   1
#define IMAGE_STATE_FAIL     -1
#define IMAGE_STATE_SUCCESS   0
/**********************************************/


/**************************** 图片盒子 *********************************/
struct _LCUI_PictureBox
{
	LCUI_Graph	*image;		/* 在PictureBox中显示的图像 */
	LCUI_Rect	read_box;	/* 显示图像的区域范围 */ 
	LCUI_Graph	buff_graph;	/* 用于储存缩放后的图形 */
	float		scale;		/* 缩放比例 */
	int		size_mode;	/* 控制PictureBox该如何处理图像位置和部件大小 */
	
	LCUI_Graph	error_image;	/* 当加载图像失败时显示的图像 */
	LCUI_Graph	initial_image;	/* 正在加载另一图像时显示的图像 */ 
	
	int		image_state;	/* 图片状态(LOADING / FAIL / SUCCESS) */
};
/****************************** END ***********************************/

/* 获取图片盒子的图片显示的区域 */
LCUI_EXPORT(LCUI_Rect)
PictureBox_GetViewArea( LCUI_Widget *widget );

/* 获取获取图片盒子的图片显示的区域的坐标 */
LCUI_EXPORT(LCUI_Pos)
PictureBox_GetViewAreaPos( LCUI_Widget *widget );

/* 获取缩放比例 */
LCUI_EXPORT(float)
PictureBox_GetScale( LCUI_Widget *widget );

/* 设定PictureBox部件显示的图像 */
LCUI_EXPORT(void)
PictureBox_SetImage( LCUI_Widget *widget, LCUI_Graph *image );

/* 设定图片文件中的图像为PictureBox部件显示的图像 */
LCUI_EXPORT(int)
PictureBox_SetImageFile( LCUI_Widget *widget, char *image_file );

/* 设定当加载图像失败时显示的图像 */
LCUI_EXPORT(int)
PictureBox_SetErrorImage( LCUI_Widget *widget, LCUI_Graph *pic );

/* 设定正在加载另一图像时显示的图像 */
LCUI_EXPORT(int)
PictureBox_SetInitImage( LCUI_Widget *widget, LCUI_Graph *pic );

/* 设定图像显示模式 */
LCUI_EXPORT(void)
PictureBox_SetSizeMode( LCUI_Widget *widget, int mode );

/* 设定PictureBox部件的图片显示区域的大小 */
LCUI_EXPORT(void)
PictureBox_ResizeViewArea( LCUI_Widget *widget, int width, int height );

/* 获取PictureBox部件内的图像 */
LCUI_EXPORT(LCUI_Graph*)
PictureBox_GetImage( LCUI_Widget *widget );

/* 移动PictureBox部件内的图片的显示区域的位置 */
LCUI_EXPORT(int)
PictureBox_MoveViewArea( LCUI_Widget *widget, LCUI_Pos des_pos );

/* 缩放PictureBox部件的图片浏览区域 */
LCUI_EXPORT(int)
PictureBox_ZoomViewArea( LCUI_Widget *widget, double scale );

LCUI_END_HEADER

#endif

