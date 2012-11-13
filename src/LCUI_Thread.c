/* ***************************************************************************
 * LCUI_Thread.c -- base thread management
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
 * LCUI_Thread.c -- 基本的线程管理
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
 * 文件名：LCUI_Thread.c
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

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include <unistd.h>

void Thread_TreeNode_Init(Thread_TreeNode *ttn)
/* 功能：初始化线程树结点 */
{
	ttn->tid = 0;
	ttn->child.queue = NULL;
	ttn->child.max_num = 0;
	ttn->child.total_num = 0;
	ttn->parent = NULL;
}


Thread_TreeNode *
Search_Thread_Tree(Thread_TreeNode *ttn, thread_t tid)
/*
 * 功能：从指定线程树的结点中搜索匹配的线程ID，并返回线程树结点的指针
 * 提示：该遍历树的算法比较简陋，有待完善。
 **/
{ 
	if(NULL == ttn) {
		return NULL;
	}
	if(ttn->tid == tid) {
		return ttn;
	}
	
	int i;
	Thread_TreeNode *new_ttn;
	for(i=0; i<ttn->child.max_num; ++i) {
		if( !ttn->child.queue[i] ) {
			continue;
		}
		/* 如果该线程树结点指针有效，就继续递归查找 */
		new_ttn = Search_Thread_Tree(ttn->child.queue[i], tid);
		if( new_ttn ) {
			return new_ttn;
		}
	}
	return NULL;/* 没有在for循环里返回正常的指针，那么就在这里返回NULL */
}

Thread_TreeNode *
Thread_TreeNode_Add_New(Thread_TreeNode *ttn, thread_t tid)
/* 功能：在线程树中添加新的结点 */
{
	int temp = 0, i;
	LCUI_ID pos = 0;
	if(ttn->child.total_num > 0) {
		/* 先遍历数据，找未使用的空位 */
		for(i = 0; i < ttn->child.max_num; ++i) {
			if(ttn->child.queue[i] == NULL) {
				if(temp == 0) {
					temp = 1;
					pos = i;
				}
			}
			else if(ttn->child.queue[i]->tid == tid) {
				 /* 如果该线程ID一致 */
				return ttn->child.queue[i];
			}
		}
		ttn->child.total_num += 1; /* 增加该程序的总窗口数量 */ 
	} else {
		pos = 0;
		ttn->child.total_num = 1;
		ttn->child.max_num = ttn->child.total_num; 
		ttn->child.queue = (Thread_TreeNode**)calloc(1, 
				sizeof( Thread_TreeNode*));
		
		if(ttn->child.queue == NULL) {/* 如果申请内存失败 */ 
		//
			return NULL;
		}
		temp = 1;
	}
	
	if(temp == 0) {/* 如果没有空位，那就扩增内存 */ 
		pos = ttn->child.max_num;
		++ttn->child.max_num; 
		ttn->child.queue = (Thread_TreeNode**)realloc(ttn->child.queue, 
				ttn->child.max_num * sizeof(Thread_TreeNode*));
		if(ttn->child.queue == NULL) {/* 如果扩增内存失败 */
			return NULL;
		}
	}
	
	ttn->child.queue[pos] = (Thread_TreeNode*)calloc(1, sizeof(Thread_TreeNode));
	/* 以下需要初始化 */
	Thread_TreeNode_Init(ttn->child.queue[pos]);
	ttn->child.queue[pos]->tid = tid; /* 保存线程ID */
	ttn->child.queue[pos]->parent = ttn; /* 保存父线程的结点指针 */
	return ttn->child.queue[pos];
}


int Thread_TreeNode_Delete(Thread_TreeNode *ttn, thread_t tid)
/* 功能：在线程树中删除一个结点 */
{
	int i;
	Thread_TreeNode *tt;
	tt = Search_Thread_Tree(&LCUI_Sys.thread_tree, tid);
	if(tt == NULL) {
		return -1;
	}
	tt = tt->parent; /* 得到父线程的结点指针 */
	if(tt == NULL) {
		return -2;
	}
	if(tt->child.total_num <= 0) {
		return -3;
	}
	for(i = 0; i < tt->child.total_num; ++i) { 
		if(tt->child.queue[i] != NULL 
		  && tt->child.queue[i]->tid == tid
		) {
			--tt->child.total_num; 
			free(tt->child.queue[i]);
			tt->child.queue[i] = NULL;
			return 0; 
		}
	}
	return -3;
}

int LCUI_Thread_Create( thread_t *tidp,
			const thread_attr_t *attr,
			void *(*start_rtn)(void*),
			void * arg )
/*
 * 功能：创建一个线程
 * 说明：主要是调用pthread_create函数来创建线程，并进行附加处理
 * */
{
	Thread_TreeNode *tt;
	/* 先获取当前所在线程的ID */
	thread_t t = thread_self();
	/* 在线程树中查找匹配的线程ID，并得到该结点的指针 */
	tt = Search_Thread_Tree(&LCUI_Sys.thread_tree, t);
	if(tt == NULL) {/* 如果没有搜索到，那就新增，并获得该结点指针 */
		tt = Thread_TreeNode_Add_New(&LCUI_Sys.thread_tree, t);
		if(tt == NULL) {
			return -1; /* 出问题的话就返回-1 */
		}
	}
	/* 调用pthread_create函数来创建线程 */
	thread_create(tidp, attr, start_rtn, arg);
	//printf("create thread:%lu, parent thread:%lu\n", *tidp, t);
	Thread_TreeNode_Add_New(tt, *tidp);/* 将得到的线程ID添加至结点 */
	return 0;
}

int LCUI_Thread_Join(thread_t thread, void **retval)
/* 功能：等待一个线程的结束 */
{
	int tmp;
	tmp = thread_join(thread, retval);
	/* 从线程树中删除这个线程的结点 */
	if(tmp == 0) {/* 如果pthread_join函数的返回值正常 */
		Thread_TreeNode_Delete(&LCUI_Sys.thread_tree, thread);
	}
	return tmp;
}

int LCUI_Thread_Cancel(thread_t thread) 
/* 功能：撤销一个线程 */
{
	//printf("cancel thread:%lu\n", thread);
	int tmp;
	tmp = thread_cancel(thread);
	/* 从线程树中删除这个线程的结点 */
	if(tmp == 0) {
		Thread_TreeNode_Delete(&LCUI_Sys.thread_tree, thread);
	}
	return tmp;
}

void LCUI_Thread_Exit(void* retval)
/*
 * 功能：终止调用它的线程并返回一个指向某个对象的指针
 * 说明：线程通过调用LCUI_Thread_Exit函数终止执行，就如同进程在结
 * 束时调用exit函数一样。
 * */
{
	thread_exit(retval);
}


int LCUI_Thread_Tree_Cancel(Thread_TreeNode *ttn)
/* 功能：撤销线程关系树的结点中的线程以及它的所有子线程 */
{
	if(ttn == NULL) {
		return 0;
	}
	
	int i;
	for(i=0; i<ttn->child.max_num; ++i) {
		if(ttn->child.queue[i] == NULL) {
			continue;
		}
		/* 如果该线程树结点指针有效，就继续递归调用，直到末尾，才开始往前撤销线程 */
		LCUI_Thread_Tree_Cancel(ttn->child.queue[i]); 
	}
	return LCUI_Thread_Cancel(ttn->tid);
}

int LCUI_App_Thread_Cancel(LCUI_ID app_id)
/* 功能：撤销指定ID的程序的全部线程 */
{
	Thread_TreeNode *ttn;
	ttn = Search_Thread_Tree(&LCUI_Sys.thread_tree, app_id);
	if(ttn == NULL) {
		return -1;
	}
	
	int i;
	for(i=0; i<ttn->child.max_num; ++i) {
		if(ttn->child.queue[i] != NULL) {
		/* 只撤销预先程序的子线程，主线程会在程序结束后自己结束 */
			LCUI_Thread_Tree_Cancel(ttn->child.queue[i]); 
		}
	}
	return 0; 
}
