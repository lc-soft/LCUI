/* ***************************************************************************
 * steptimer.h -- step timer, Mainly used to control the count of frames per
 * second for the rendering loop.
 *
 * Copyright (C) 2014-2017 by Liu Chao <lc-soft@live.cn>
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
 * steptimer.h -- 步进计时器，主要用于控制渲染循环每秒更新的帧数。
 *
 * 版权所有 (C) 2014-2017 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_UTIL_STEPTIMER_H
#define LCUI_UTIL_STEPTIMER_H

LCUI_BEGIN_HEADER

#ifdef LCUI_UTIL_STEPTIMER_C
typedef struct StepTimerRec_* StepTimer;
#else
typedef void* StepTimer;
#endif

/** 新建帧数控制实例 */
LCUI_API StepTimer StepTimer_Create( void );

/** 销毁帧数控制相关资源 */
LCUI_API void StepTimer_Destroy( StepTimer timer );

/** 设置最大FPS（帧数/秒） */
LCUI_API void StepTimer_SetFrameLimit( StepTimer timer, unsigned int max );

/** 获取当前FPS */
LCUI_API int StepTimer_GetFrameCount( StepTimer timer );

/** 让当前帧停留一定时间 */
LCUI_API void StepTimer_Remain( StepTimer timer );

/** 暂停数据帧的更新 */
LCUI_API void StepTimer_Pause( StepTimer timer, LCUI_BOOL need_pause );

LCUI_END_HEADER

#endif

