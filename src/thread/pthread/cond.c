/* cond.c -- Condition variables, for the mechanism of thread synchronization
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
