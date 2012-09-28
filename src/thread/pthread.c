/* ***************************************************************************
 * phread.c -- pthread threading support
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
 * phread.c -- pthread的线程支持
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

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include <unistd.h>

/************************ thread management ***************************/ 
thread_t thread_self()
{
	return pthread_self();
}

int thread_create(	thread_t *restrict __newthread,
			const thread_attr_t *restrict __attr,
			void *(*__start_routine) (void *),
			void *restrict __arg )
{
	return pthread_create ( __newthread, __attr, __start_routine, __arg);
}

int thread_cancel(thread_t __th)
{
	return pthread_cancel (__th);
}

int thread_join(thread_t __th, void **__thread_return)
{
	return pthread_join (__th, __thread_return);
}

void thread_exit(void *__retval)
{
	pthread_exit(__retval);
}
/********************* end thread management ***************************/ 


/************************ thread R/W lock *****************************/ 
void thread_perror(char *msg, int err)
{ 
	pthread_t tid;
	tid = pthread_self();
	printf("thread %lu: %s : %s\n", tid, msg, strerror(err)); 
}

void thread_rwlock_init(thread_rwlock *rwlock)
/* 功能：初始化读写锁 */
{
	int error;

	//error = pthread_cond_init(&rwlock->cond, NULL);
	//if(error) thread_perror("cond init", error);
	
	error = pthread_mutex_init(&rwlock->mutex, NULL);
	if(error) {
		thread_perror("mutex init", error); 
	}
	error = pthread_rwlock_init(&rwlock->lock, NULL);
	if(error) {
		thread_perror("rwlock init", error); 
	}
	rwlock->host = 0;
	rwlock->status = RWLOCK_FREE;
}

void thread_rwlock_destroy(thread_rwlock *rwlock)
/* 功能：销毁读写锁 */
{
	int error;
	//error = pthread_cond_destroy(&rwlock->cond);
	//if(error) thread_perror("cond destroy", error);
	
	error = pthread_mutex_destroy(&rwlock->mutex);
	if(error) {
		thread_perror("mutex destroy", error);
	}
	
	error = pthread_rwlock_destroy(&rwlock->lock);
	if(error) {
		thread_perror("rwlock destroy", error);
	}
	rwlock->host = 0; 
	rwlock->status = RWLOCK_FREE;
}
	
int thread_wait_mutex(thread_rwlock *rwlock)
/* 功能：等待互斥锁为解锁状态 */
{
	pthread_t tid;  
	tid = pthread_self();
	/* 检测队列互斥锁是否被锁住 */
	while(1) {
		//printf("thread %lu: mutex try lock\n", tid);
		/* 如果锁住失败 */
		if(pthread_mutex_trylock(&rwlock->mutex)) {
			/* 如果锁住互斥锁的是自己，那就中断循环 */
			if(rwlock->host == tid) {
				//printf("thread %lu: is self, finish\n", tid);
				return 0;
			} else {
			/* 否则，暂时移除自己的记录，之后阻塞等待互斥锁被解锁，并恢复记录 */
				//printf("thread %lu: wrlock lock\n", tid);
				//pthread_rwlock_wrlock(&rwlock->lock);
				//printf("thread %lu: wrlock lock end\n", tid);
				//printf("thread %lu: wait, mutex lock\n", tid);
				pthread_mutex_lock(&rwlock->mutex);
				//printf("thread %lu: wait, mutex lock end\n", tid);
				//pthread_rwlock_unlock(&rwlock->lock);
				//printf("thread %lu: wrlock unlock\n", tid);
				break;
			}
		} else {
			break;
		}
	}
	//printf("thread %lu: mutex unlock\n", tid);
	return pthread_mutex_unlock(&rwlock->mutex);/* 解开互斥锁 */
}

int thread_rwlock_rdlock(thread_rwlock *rwlock)
/* 功能：设定“读”锁 */
{ 
	thread_wait_mutex(rwlock);/* 等待互斥锁可用 */ 
	rwlock->status = RWLOCK_READ;
	return pthread_rwlock_rdlock(&rwlock->lock); 
}

int thread_rwlock_wrlock(thread_rwlock *rwlock)
/* 功能：设定“写”锁 */
{
	thread_wait_mutex(rwlock);
	rwlock->status = RWLOCK_WRITE;
	return pthread_rwlock_wrlock(&rwlock->lock);
}

int thread_rwlock_unlock(thread_rwlock *rwlock)
/* 功能：解开读写锁 */
{
	rwlock->status = RWLOCK_FREE;
	return pthread_rwlock_unlock(&rwlock->lock);
}

rwlock_status thread_rwlock_get_status(thread_rwlock *rwlock)
/* 功能：获取读写锁的状态 */
{
	return rwlock->status;
}

int thread_mutex_lock(thread_rwlock *rwlock)
/* 功能：设定互斥锁 */
{
	pthread_t tid;
	tid = pthread_self();
	//printf("thread %lu: mutex lock, wait mutex\n", tid);
	thread_wait_mutex(rwlock); 
	//printf("thread %lu: mutex lock, wait mutex end\n", tid);
	/* 保存设置互斥锁的线程的ID */
	rwlock->host = tid; 
	return pthread_mutex_lock(&rwlock->mutex);
}

int thread_mutex_unlock(thread_rwlock *rwlock)
/* 功能：解开互斥锁 */
{
	rwlock->host = 0;
	//pthread_cond_signal(&rwlock->cond);
	return pthread_mutex_unlock(&rwlock->mutex);
}

/*********************** End thread R/W lock **************************/
