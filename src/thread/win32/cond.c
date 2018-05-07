/* ***************************************************************************
 * cond.c -- Condition variables, for the mechanism of thread synchronization
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

#include <errno.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>

#ifdef LCUI_BUILD_IN_WIN32

/** 初始化一个条件变量 */
int LCUICond_Init( LCUI_Cond *cond )
{
	/* 创建一个事件对象：
	 * 使用默认安全符
	 * 在事件被释放后，自动复原为无信号状态
	 * 初始状态为无信号状态
	 * 不指定名字，无名
	 */
	*cond = CreateEvent( NULL, FALSE, FALSE, NULL );
	if( *cond == 0 ) {
		return -1;
	}
	return 0;
}

/** 销毁一个条件变量 */
int LCUICond_Destroy( LCUI_Cond *cond )
{
	CloseHandle( *cond );
	return 0;
}

/** 阻塞当前线程，等待条件成立 */
int LCUICond_Wait( LCUI_Cond *cond, LCUI_Mutex *mutex )
{
	int ret;
	LCUIMutex_Unlock( mutex );
	ret = WaitForSingleObject( *cond, INFINITE );
	switch( ret ) {
	case WAIT_TIMEOUT:
		ret = ETIMEDOUT;
		break;
	case WAIT_OBJECT_0:
		ret = 0;
		break;
	case WAIT_FAILED: 
	default: 
		ret = GetLastError();
		break;
	}
	LCUIMutex_Lock( mutex );
	return ret;
}

/** 计时阻塞当前线程，等待条件成立 */
int LCUICond_TimedWait( LCUI_Cond *cond, LCUI_Mutex *mutex, unsigned int ms )
{
	int ret;
	LCUIMutex_Unlock( mutex );
	ret = WaitForSingleObject( *cond, ms );
	switch( ret ) {
	case WAIT_TIMEOUT:
		ret = ETIMEDOUT;
		break;
	case WAIT_OBJECT_0:
		ret = 0;
		break;
	case WAIT_FAILED: 
	default: 
		ret = GetLastError();
		break;
	}
	LCUIMutex_Lock( mutex );
	return ret;
}

/** 唤醒一个阻塞等待条件成立的线程 */
int LCUICond_Signal( LCUI_Cond *cond )
{
	/* 当编译为 Windows 运行时组件时，直接改为广播 */
#ifdef WINAPI_FAMILY_APP
	return LCUICond_Broadcast( cond );
#else
	if( PulseEvent( *cond ) ) {
		return 0;
	}
	return -1;
#endif
}

/** 唤醒所有阻塞等待条件成立的线程 */
int LCUICond_Broadcast( LCUI_Cond *cond )
{
	if( SetEvent( *cond ) ) {
		return 0;
	}
	return -1;
}

#endif
