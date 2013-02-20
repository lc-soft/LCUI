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

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include <unistd.h>

typedef struct _Thread_TreeNode Thread_TreeNode;
struct _Thread_TreeNode
{
	Thread_TreeNode *parent;	/* 父线程结点指针 */
	LCUI_Thread tid;		/* 父线程ID */
	LCUI_Queue child;		/* 子线程结点队列 */
};

static Thread_TreeNode thread_tree;

static void Destroy_TreeNode( Thread_TreeNode *ttn )
{
	Destroy_Queue( &ttn->child );
}

/* 初始化线程树结点 */
static void Thread_TreeNode_Init( Thread_TreeNode *ttn )
{
	ttn->tid = 0;
	ttn->parent = NULL;
	Queue_Init( &ttn->child, sizeof(Thread_TreeNode), Destroy_TreeNode );
}

/* 从指定线程树的结点中搜索匹配的线程ID，并返回线程树结点的指针 */
static Thread_TreeNode *
ThreadTree_Find( Thread_TreeNode *ttn, LCUI_Thread tid )
{ 
	if(NULL == ttn) {
		return NULL;
	}
	if(ttn->tid == tid) {
		return ttn;
	}
	
	int i, n;
	Thread_TreeNode *new_ttn;
	
	n = Queue_Get_Total( &ttn->child );
	for(i=0; i<n; ++i) {
		new_ttn = Queue_Get( &ttn->child, i );
		if( !new_ttn ) {
			continue;
		}
		/* 如果该线程树结点指针有效，就继续递归查找 */
		new_ttn = ThreadTree_Find(new_ttn, tid);
		if( new_ttn ) {
			return new_ttn;
		}
	}
	return NULL;/* 没有在for循环里返回正常的指针，那么就在这里返回NULL */
}

/* 获取指定线程的根线程ID */
LCUI_Thread LCUIThread_GetRootThreadID( LCUI_Thread tid )
{
	Thread_TreeNode *ttn;
	ttn = ThreadTree_Find(&thread_tree, tid);
	if( !ttn ) {
		return tid;
	}
	/* 
	 * 往父级遍历，直至父级指针为NULL，因为根线程是没
	 * 有父线程结点指针的，程序的线程ID都在根线程里的
	 * 子线程ID队列中 
	 * */
	while( ttn->parent ) { 
		ttn = ttn->parent;
		if( ttn && !ttn->parent ) { 
			break;
		}
	}
	return ttn->tid;
	
}

static Thread_TreeNode *
ThreadTreeNode_AddNew( Thread_TreeNode *ttn, LCUI_Thread tid )
/* 功能：在线程树中添加新的结点 */
{
	Thread_TreeNode *new_ttn;
	
	new_ttn = malloc(sizeof(Thread_TreeNode));
	Thread_TreeNode_Init( new_ttn );
	new_ttn->tid = tid;
	new_ttn->parent = ttn;
	Queue_Add_Pointer(& ttn->child, new_ttn );
	return new_ttn;
}


/* 在线程树中删除一个结点 */
static int 
ThreadTreeNode_Delete( Thread_TreeNode *ttn, LCUI_Thread tid )
{
	int i, n;
	Thread_TreeNode *tt, *child;
	
	tt = ThreadTree_Find(&thread_tree, tid);
	if(tt == NULL) {
		return -1;
	}
	tt = tt->parent; /* 得到父线程的结点指针 */
	if(tt == NULL) {
		return -2;
	}
	n = Queue_Get_Total( &tt->child );
	if(n <= 0) {
		return -3;
	}
	for(i=0; i<n; ++i) { 
		child = Queue_Get( &tt->child, i );
		if( !child || child->tid != tid ) {
			continue;
		}
		Queue_Delete( &tt->child, i );
		return 0;
	}
	return -3;
}

/* 创建并运行一个线程 */
int LCUIThread_Create( LCUI_Thread *tidp, void (*start_rtn)(void*), void * arg )
{
	Thread_TreeNode *tt;
	LCUI_Thread cur_tid;
	
	/* 先获取当前所在线程的ID */
	cur_tid = LCUIThread_SelfID();
	/* 在线程树中查找匹配的线程ID，并得到该结点的指针 */
	tt = ThreadTree_Find( &thread_tree, cur_tid );
	if(tt == NULL) {/* 如果没有搜索到，那就新增，并获得该结点指针 */
		tt = ThreadTreeNode_AddNew( &thread_tree, cur_tid );
		if(tt == NULL) {
			return -1; /* 出问题的话就返回-1 */
		}
	}
	_LCUIThread_Create( tidp, start_rtn, arg );
	ThreadTreeNode_AddNew( tt, *tidp );/* 将得到的线程ID添加至结点 */
	return 0;
}

/* 等待一个线程的结束，并释放该线程的资源 */
int LCUIThread_Join( LCUI_Thread thread, void **retval )
{
	int ret;
	ret = _LCUIThread_Join( thread, retval );
	/* 如果线程已正常退出，则从线程树中删除这个线程的结点 */
	if(ret == 0) {
		ThreadTreeNode_Delete( &thread_tree, thread );
	}
	return ret;
}

/* 撤销一个线程 */
void LCUIThread_Cancel( LCUI_Thread thread )
{
	_LCUIThread_Cancel( thread );
	ThreadTreeNode_Delete( &thread_tree, thread );
}

/* 记录指针作为返回值，并退出线程 */
void LCUIThread_Exit( void* retval )
{
	_LCUIThread_Exit( retval );
}

/* 撤销线程关系树的结点中的线程以及它的所有子线程 */
static void LCUIThreadTree_Cancel( Thread_TreeNode *ttn )
{
	int i, n;
	Thread_TreeNode *child;

	if(ttn == NULL) {
		return;
	}
	n = Queue_Get_Total( &ttn->child );
	for(i=0; i<n; ++i) {
		child = Queue_Get( &ttn->child, i );
		if( !child ) {
			continue;
		}
		/* 如果该线程树结点指针有效，就继续递归调用，直到末尾，才开始往前撤销线程 */
		LCUIThreadTree_Cancel( child );
	}
	LCUIThread_Cancel( ttn->tid );
}

/* 撤销指定ID的程序的全部子线程 */
int LCUIApp_CancelAllThreads( LCUI_ID app_id )
{
	int i, n;
	Thread_TreeNode *ttn, *child;
	ttn = ThreadTree_Find(&thread_tree, (LCUI_Thread)app_id);
	if(ttn == NULL) {
		return -1;
	}
	
	n = Queue_Get_Total( &ttn->child );
	for(i=0; i<n; ++i) {
		child = Queue_Get( &ttn->child, i );
		if( !child ) {
			continue;
		}
		/* 只撤销程序的子线程，主线程会在程序结束后自己结束 */
		LCUIThreadTree_Cancel( child );
	}
	return 0;
}

/* 初始化线程模块 */
void LCUIModule_Thread_Init( void )
{
	Thread_TreeNode_Init( &thread_tree ); /* 初始化根线程结点 */
	thread_tree.tid = LCUIThread_SelfID(); /* 当前线程ID作为根结点 */
	LCUI_Sys.self_id = thread_tree.tid; /* 保存线程ID */
}

/* 停用线程模块 */
void LCUIModule_Thread_End( void )
{
	Destroy_Queue( &thread_tree.child );
}

