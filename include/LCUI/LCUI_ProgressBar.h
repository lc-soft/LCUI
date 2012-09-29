/* ***************************************************************************
 * LCUI_ProgressBar.h -- LCUI's ProgressBar widget
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
 * LCUI_ProgressBar.h -- LCUI 的进度条部件
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


#ifndef __LCUI_PROGBAR_H__
#define __LCUI_PROGBAR_H__

typedef struct _LCUI_ProgressBar		LCUI_ProgressBar;

/*************** 进度条 ********************/
struct _LCUI_ProgressBar
{
	pthread_t thread;	/* 线程 */
	LCUI_Graph fore_graph;		/* 前景图形 */ 
	LCUI_Graph flash_image;	/* 附加的图形，实现进度条上移动的的闪光 */
	
	LCUI_Widget *fore_pic_box;	/* 前景图片的图片盒子 */ 
	LCUI_Widget *img_pic_box;	/* 闪光图片的图片盒子 */
	
	int img_move_speed; /* 表示进度条上的闪光的移动速度，单位为：像素/秒 */
	int sleep_time;	/* 指定在闪光移动至进度条尾部后，等待多久进行下一次移动，单位为：毫秒 */
	int max_value;	/* 总进度的值 */
	int value;	/* 当前进度的值 */
};


LCUI_BEGIN_HEADER

int Draw_Empty_Slot(LCUI_Graph *graph, int width, int height);
/* 功能：绘制进度条的空槽 */ 

LCUI_Widget *Get_ProgressBar_Flash_Img_Widget(LCUI_Widget *widget);
/* 功能：获取进度条上闪光图形所在的PictureBox部件 */ 

void Set_ProgressBar_Max_Value(LCUI_Widget *widget, int max_value);
/* 功能：设定进度条最大值 */ 

int Get_ProgressBar_Max_Value(LCUI_Widget *widget);
/* 功能：获取进度条最大值 */ 

void Set_ProgressBar_Value(LCUI_Widget *widget, int value);
/* 功能：设定进度条当前值 */ 

int Get_ProgressBar_Value(LCUI_Widget *widget);
/* 功能：获取进度条当前值 */ 

LCUI_END_HEADER

#endif
