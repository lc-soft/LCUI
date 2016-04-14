/* ***************************************************************************
* timer.h -- timer support.
* 
* Copyright (C) 2013-2016 by Liu Chao <lc-soft@live.cn>
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
* timer.h -- 定时器支持
*
* 版权所有 (C) 2013-2016 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_TIMER_H__
#define __LCUI_TIMER_H__

LCUI_BEGIN_HEADER

/** 
 * 设置定时器
 * 定时器的作用是让一个任务在经过指定时间后才执行
 * @param n_ms
 *	等待的时间，单位为毫秒
 * @param callback_func
 *	用于响应定时器的回调函数
 * @param reuse 
 *	指示该定时器是否重复使用，如果要用于循环定时处理某些任
 *	务，可将它置为 TRUE，否则置于 FALSE。
 * @return 
 *	该定时器的标识符
 * */
LCUI_API int LCUITimer_Set( long int n_ms, void (*callback_func)(void*),
			    void *arg, LCUI_BOOL reuse );

/**
 * 释放定时器
 * 当不需要定时器时，可以使用该函数释放定时器占用的资源，并移除程序任务队列
 * 中还未处理的定时器任务
 * @param timer_id
 *	需要释放的定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Free( int timer_id );

/**
 * 暂停定时器的倒计时
 * 一般用于往复定时的定时器
 * @param timer_id
 *	目标定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Pause( int timer_id );

/**
 * 继续定时器的倒计时
 * @param timer_id
 *	目标定时器的标识符
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Continue( int timer_id );

/**
 * 重设定时器的等待时间
 * @param timer_id
 *	需要释放的定时器的标识符
 * @param n_ms
 *	等待的时间，单位为毫秒
 * @return
 *	正常返回0，指定ID的定时器不存在则返回-1.
 * */
LCUI_API int LCUITimer_Reset( int timer_id, long int n_ms ) ;

/* 初始化定时器模块 */
LCUI_API void LCUI_InitTimer( void );

/* 停用定时器模块 */
LCUI_API void LCUI_ExitTimer( void );

LCUI_END_HEADER

#endif
