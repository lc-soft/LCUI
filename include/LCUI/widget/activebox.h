/* ***************************************************************************
 * LCUI_ActiveBox.h -- ActiveBox widget, play simple dynamic picture
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
 * LCUI_ActiveBox.h -- ActiveBox部件, 播放简单的动态图像
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

#ifndef __LCUI_ACTIVEBOX_H__
#define __LCUI_ACTIVEBOX_H__

typedef struct _LCUI_Frames		LCUI_Frames;
typedef struct _LCUI_Frame		LCUI_Frame;
typedef struct _LCUI_ActiveBox	LCUI_ActiveBox;

struct _LCUI_ActiveBox{
	LCUI_Frames *frames;
}; 

/************************** 保存动画每一帧的信息 **********************/
struct _LCUI_Frame
{
	LCUI_Pos offset;	/* 该帧的偏移位置 */
	LCUI_Graph *pic;	/* 当前帧的图形 */
	long int sleep_time;	/* 该帧显示的时长（单位：毫秒） */
	long int current_time;	/* 当前剩下的等待时间 */
};
/******************************************************************/

/************************** 保存动画的信息 **********************/
struct _LCUI_Frames
{
	int state;		/* 状态，播放还是暂停 */
	LCUI_Queue pic;	/* 用于记录该动画的所有帧 */
	int current;		/* 记录当前显示的帧序号 */
	LCUI_Size size;	/* 动画的尺寸 */
	LCUI_Graph slot;	/* 动画槽，记录当前帧显示的图像 */
	LCUI_Queue func_data;	/* 记录被关联的函数及它的参数 */
};
/*************************************************************/

LCUI_BEGIN_HEADER

/*********************** Frames Process *******************************/
LCUI_Frames* Create_Frames(LCUI_Size size);
/* 
 * 功能：创建一个能存放动画数据的容器
 * 说明：该容器用于记录动画的每一帧的信息，需要指定该容器的尺寸。
 *  */ 

LCUI_Pos Frame_Get_Pos(LCUI_Frames *stream, LCUI_Frame *frame);
/* 功能：获取指定帧在整个动画容器中的位置 */ 

int Resize_Frames(LCUI_Frames *p, LCUI_Size new_size);
/* 功能：调整动画的容器尺寸 */ 

int Add_Frame(	LCUI_Frames *des, LCUI_Graph *pic, 
		LCUI_Pos offset, int sleep_time );
/* 
 * 功能：为动画添加帧 
 * 说明：
 * pic指的是该帧的图像；
 * pos代表该帧图像在整个动画画面中的位置；
 * sleep_time表示该帧的显示时长，单位为：10毫秒
 * */ 

int Frames_Add_Func(	LCUI_Frames *des, 
			void (*func)(LCUI_Graph*, void*), 
			void *arg );
/* 
 * 功能：为动画关联回调函数 
 * 说明：关联回调函数后，动画每更新一帧都会调用这个函数
 * */ 

LCUI_Frame *Frames_Get_Frame(LCUI_Frames *src);
/* 功能：获取当前帧 */ 

LCUI_Graph *Frames_Get_Slot(LCUI_Frames *src);
/* 功能：获取当前帧的图像 */ 

int Frames_Play(LCUI_Frames *frames);
/* 功能：播放动画 */ 

int Frames_Pause(LCUI_Frames *frames);
/* 功能：暂停动画 */ 
/*********************** End Frames Process ***************************/

/************************** ActiveBox *********************************/
LCUI_Frames *ActiveBox_Get_Frames(LCUI_Widget *widget);

int ActiveBox_Set_Frames_Size(LCUI_Widget *widget, LCUI_Size new_size);
/* 功能：设定动画尺寸 */ 

int ActiveBox_Play(LCUI_Widget *widget);
/* 功能：播放动画 */ 

int ActiveBox_Pause(LCUI_Widget *widget);
/* 功能：暂停动画 */ 

int ActiveBox_Add_Frame(	LCUI_Widget *widget, LCUI_Graph *pic, 
				LCUI_Pos offset, int sleep_time );
/* 功能：为ActiveBox部件内的动画添加一帧图像 */ 
/************************** End ActiveBox *****************************/

LCUI_END_HEADER

#endif
