/* ***************************************************************************
 * LCUI_Thread.hpp -- C++ class of basic thread management
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
 * LCUI_Thread.hpp -- 基本的线程管理的C++类
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


#ifndef __LCUI_THREAD_HPP__
#define __LCUI_THREAD_HPP__ 

#ifdef __cplusplus

class LCUIThread
{
	public:
	LCUIThread()
	{
		tid = 0;
		thread_rwlock_init(&lock);
	}
	int create( const thread_attr_t *attr,
			void *(*start_rtn)(void*),
			void * arg )
	{
		return LCUIThread_Create(&tid, attr, start_rtn, arg);
	}
	int rdlock()
	{
		return thread_rwlock_rdlock(&lock);
	}
	int wrlock()
	{
		return thread_rwlock_wrlock(&lock);
	}
	int unlock()
	{
		return thread_rwlock_unlock(&lock);
	}
	int mutex_lock()
	{
		return thread_mutex_lock(&lock);
	}
	int mutex_unlock()
	{
		return thread_mutex_unlock(&lock);
	}
	int cancel()
	{
		return LCUIThread_Cancel(tid);
	}
	int join(void **retval)
	{
		return LCUIThread_Join(tid, retval);
	}
	void exit(void* retval) __attribute__ ((__noreturn__))
	{
		LCUIThread_Exit(retval);
	}
	thread_t getid()
	{
		return tid;
	}
	private:
	thread_t tid;
	thread_rwlock lock;
};
#endif

#endif
