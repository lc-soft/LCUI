/* ***************************************************************************
 * cond.c -- condition variables, for the mechanism of thread synchronization
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * cond.c -- 条件变量，用于进行线程同步的机制
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

#ifdef LCUI_BUILD_IN_LINUX
#include <errno.h>
#include <sys/time.h>

/** 初始化一个条件变量 */
int LCUICond_Init( LCUI_Cond *cond )
{
	return pthread_cond_init( cond, NULL );
}

/** 销毁一个条件变量 */
int LCUICond_Destroy( LCUI_Cond *cond )
{
	return pthread_cond_destroy( cond );
}

/** 阻塞当前线程，等待条件成立 */
int LCUICond_Wait( LCUI_Cond *cond, LCUI_Mutex *mutex )
{
	return pthread_cond_wait( cond, mutex );
}

/** 计时阻塞当前线程，等待条件成立 */
int LCUICond_TimedWait( LCUI_Cond *cond, LCUI_Mutex *mutex, unsigned int ms )
{
	int ret;
	long int out_usec;
	struct timeval now;
	struct timespec outtime;

	gettimeofday(&now, NULL);
	out_usec = now.tv_usec + ms*1000;
	outtime.tv_sec = now.tv_sec + out_usec / 1000000;
	outtime.tv_nsec = (out_usec % 1000000) *1000;
	DEBUG_MSG("wait, ms = %u, outtime.tv_nsec: %ld\n", ms, outtime.tv_nsec);
	ret = pthread_cond_timedwait( cond, mutex, &outtime );
	DEBUG_MSG("ret: %d, ETIMEDOUT = %d, EINVAL = %d\n", ret, ETIMEDOUT, EINVAL);
	switch( ret ) {
	case 0: return 0;
	case ETIMEDOUT: return 1;
	default: break;
	}
	return -1;
}

/** 唤醒一个阻塞等待条件成立的线程 */
int LCUICond_Signal( LCUI_Cond *cond )
{
	return pthread_cond_signal( cond );
}

/** 唤醒所有阻塞等待条件成立的线程 */
int LCUICond_Broadcast( LCUI_Cond *cond )
{
	return pthread_cond_broadcast( cond );
}

#endif
