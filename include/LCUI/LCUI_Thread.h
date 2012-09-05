/* ***************************************************************************
 * LCUI_Thread.h -- basic thread management
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
 * LCUI_Thread.h -- 基本的线程管理
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

/* 
 * 说明：用于实现对线程的管理，由于LCUI还未实现多进程通信，只能靠线程ID来区分哪个操作是
 * 哪个程序的，目前打算是将程序以线程的形式运行，而不是以进程的形式运行，主线程为运行平台，
 * 其它程序就在该平台上运行，虽然能实现数据的共享，但是，一旦某个程序出现错误，整个进程就
 * 会终止。
 * 在使用本文件内的函数时，会对线程关系树（我觉得叫这个名字好一些）进行相应操作，比如：
 * 创建线程，就会先得到创建时的父线程ID，搜索这个树中的每个结点，如果有匹配的线程ID，就
 * 将线这个子程ID加入这个父线程的子线程队列中；如果没有匹配的，那就新增一个至主队列中。
 * 而撤销线程，也类似，先查找，后移除。
 * 为确保LCUI在结束后，不会因为还有其它线程在使用LCUI已经释放的资源，而导致段错误，LCUI
 * 会在退出前，撤销所有已记录的线程，然后进行资源释放。
 * */

#ifndef __LCUI_THREAD_H__
#define __LCUI_THREAD_H__ 

#define RWLOCK_READ	1
#define RWLOCK_WRITE	2

typedef struct _thread_rwlock{
	pthread_t host;
	pthread_rwlock_t lock;
	//pthread_cond_t cond;
	pthread_mutex_t mutex; 
}thread_rwlock;

/************************ thread R/W lock *****************************/ 

void thread_perror(char *msg, int err);

void thread_rwlock_init(thread_rwlock *rwlock);
/* 功能：初始化读写锁 */ 

void thread_rwlock_destroy(thread_rwlock *rwlock);
/* 功能：销毁读写锁 */ 
	
int thread_wait_mutex(thread_rwlock *rwlock);
/* 功能：等待互斥锁为解锁状态 */ 

int thread_rwlock_rdlock(thread_rwlock *rwlock);
/* 功能：设定“读”锁 */ 

int thread_rwlock_wrlock(thread_rwlock *rwlock);
/* 功能：设定“写”锁 */ 

int thread_rwlock_unlock(thread_rwlock *rwlock);
/* 功能：解开读写锁 */ 

int thread_mutex_lock(thread_rwlock *rwlock);
/* 功能：设定互斥锁 */ 

int thread_mutex_unlock(thread_rwlock *rwlock);
/* 功能：解开互斥锁 */ 

/*********************** End thread R/W lock **************************/

void Thread_TreeNode_Init(Thread_TreeNode *ttn);
/* 功能：初始化线程树结点 */ 

Thread_TreeNode *
Search_Thread_Tree(Thread_TreeNode *ttn, pthread_t tid);
/*
 * 功能：从指定线程树的结点中搜索匹配的线程ID，并返回线程树结点的指针
 * 提示：该遍历树的算法比较简陋，有待完善。
 **/ 

Thread_TreeNode *
Thread_TreeNode_Add_New(Thread_TreeNode *ttn, pthread_t tid);
/* 功能：在线程树中添加新的结点 */

int Thread_TreeNode_Delete(Thread_TreeNode *ttn, pthread_t tid);
/* 功能：在线程树中删除一个结点 */ 

int LCUI_Thread_Create( pthread_t *tidp,
						const pthread_attr_t *restrict_attr,
						void *(*start_rtn)(void*),
						void * arg);
/*
 * 功能：创建一个线程
 * 说明：主要是调用pthread_create函数来创建线程，并进行附加处理
 * */ 

int LCUI_Thread_Join(pthread_t thread, void **retval);
/* 功能：等待一个线程的结束 */ 

int LCUI_Thread_Cancel(pthread_t thread) ;
/* 功能：撤销一个线程 */ 

void LCUI_Thread_Exit(void* retval)  __attribute__ ((__noreturn__));
/*
 * 功能：终止调用它的线程并返回一个指向某个对象的指针
 * 说明：线程通过调用LCUI_Thread_Exit函数终止执行，就如同进程在结
 * 束时调用exit函数一样。
 * */ 
 
int LCUI_Thread_Tree_Cancel(Thread_TreeNode *ttn);
/* 功能：撤销线程关系树的结点中的线程以及它的所有子线程 */ 

int LCUI_App_Thread_Cancel(LCUI_ID app_id);
/* 功能：撤销指定ID的程序的全部线程 */ 
#endif

