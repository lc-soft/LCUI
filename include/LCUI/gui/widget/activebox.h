/* ***************************************************************************
 * activebox.h -- ActiveBox widget, play simple dynamic picture
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
 * activebox.h -- ActiveBox部件, 播放简单的动态图像
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

#ifndef __LCUI_ACTIVEBOX_H__
#define __LCUI_ACTIVEBOX_H__

LCUI_BEGIN_HEADER

/** 动画每一帧的信息 */
typedef struct AnimationFrameData_ {
	LCUI_Pos offset;	/**< 该帧的偏移位置 */
	LCUI_Graph graph;	/**< 当前帧的图形 */
	long int sleep_time;	/**< 该帧显示的时长（单位：毫秒） */
	long int current_time;	/**< 当前剩下的等待时间 */
} AnimationFrameData;

/** 动画的信息 */
typedef struct AnimationData_ {
	LCUI_Queue frame;	/**< 用于记录该动画的所有帧 */
	LCUI_Size size;		/**< 动画的尺寸 */
} AnimationData;


/** 获取当前帧的图像 */
LCUI_API LCUI_Graph* Animation_GetGraphSlot( AnimationData *animation, int play_id );

/* 获取指定帧在整个动画容器中的位置 */
LCUI_API LCUI_Pos Animation_GetFrameMixPos(	AnimationData *animation,
						AnimationFrameData *frame );

/** 调整动画的容器尺寸 */
LCUI_API int Animation_Resize( AnimationData *p, LCUI_Size new_size );

/**
 * 创建一个动画
 * 创建的动画将记录至动画库中
 * @param size
 *	动画的尺寸
 * @returns
 *	正常则返回指向动画库中的该动画的指针，失败则返回NULL
 */
LCUI_API AnimationData* Animation_Create( LCUI_Size size );

/**
 * 删除一个动画
 * 从动画库中删除指定的动画
 * @param animation
 *	需删除的动画
 * @returns
 *	正常则返回0，失败则返回-1
 */
LCUI_API int Animation_Delete( AnimationData* animation );

/**
 * 为动画添加一帧图像
 * @param des
 *	目标动画
 * @param pic
 *	新增帧的图像
 * @param offset
 *	该帧图像的坐标偏移量，用于条该帧图像的显示位置
 * @param sleep_time
 *	该帧的显示时长（单位：毫秒）
 * */
LCUI_API int Animation_AddFrame(	AnimationData *des,
					LCUI_Graph *pic,
					LCUI_Pos offset,
					int sleep_time );

/**
 * 为动画关联回调函数
 * 关联回调函数后，动画每更新一帧都会调用该函数
 * @param animation
 *	目标动画
 * @param func
 *	指向回调函数的函数指针
 * @param arg
 *	需传递给回调函数的第二个参数
 * @returns
 *	正常则返回0，失败则返回-1
 * @warning
 *	必须在动画被第一次播放后调用此函数，否则将因找不到该动画的播放实例而导致关联失败
 * */
LCUI_API int Animation_Connect(	AnimationData *animation,
				int play_id,
				void (*func)(AnimationData*, void*),
				void *arg );

/**
/* 播放指定播放实例中的动画
 * @param animation
 *	要播放的动画
 * @param play_id
 *	与该动画的播放实例对应的标识号，若不大于0，则会为该动画创建一个新的播放实例
 * @returns
 *	正常则返回该动画的播放标识号，失败则返回-1
 */
LCUI_API int Animation_Play( AnimationData *animation, int play_id );

/**
 * 暂停指定播放实例中的动画
 * @param animation
 *	要暂停的动画
 * @param play_id
 *	与该动画的播放实例对应的标识号
 * @returns
 *	正常则返回该动画的播放标识号，失败则返回-1
 */
LCUI_API int Animation_Pause( AnimationData *animation, int play_id );

/**
 * 向ActiveBox部件添加一个动画
 * @param widget
 *	目标ActiveBox部件
 * @param animation
 *	要添加的动画
 * @return
 *	正常返回0，失败返回-1
 * @note
 *	添加的动画需要手动释放，ActiveBox部件只负责记录、引用动画
 */
LCUI_API int ActiveBox_AddAnimation(	LCUI_Widget *widget,
					AnimationData *animation );

/**
 * 切换ActiveBox部件播放的动画
 * @param widget
 *	目标ActiveBox部件
 * @param animation
 *	切换至的新动画
 * @return
 *	切换成功则返回0，未找到指定ID的动画记录，则返回-1
 */
LCUI_API int ActiveBox_SwitchAnimation(	LCUI_Widget *widget,
					AnimationData *animation );

/* 播放动画 */
LCUI_API int ActiveBox_Play( LCUI_Widget *widget );

/* 暂停动画 */
LCUI_API int ActiveBox_Pause( LCUI_Widget *widget );

LCUI_END_HEADER

#endif
