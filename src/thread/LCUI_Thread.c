/* ***************************************************************************
 * LCUI_Thread.c -- base thread management
 * 
 * Copyright (C) 2012-2013 by
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
 * 版权所有 (C) 2012-2013 归属于
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

/**
 * 本模块主要用于记录LCUI应用程序所创建的线程，在LCUI退出前，会撤销掉所有线程，
 * 避免LCUI的资源在释放时还有其它线程在使用。
 * 在LCUI的进程版中，Server进程可能需要为每个LCUI应用程序进程（Client进程）准备
 * 一个线程。
 * 以后可能会为LCUI添加线程版，据说MiniGUI的线程版可以让各个MiniGUI程序运行在线
 * 程上。
 * 本模块目前暂时保留。
 */

#include <LCUI_Build.h>
#include LC_LCUI_H 

typedef struct _Thread_TreeNode Thread_TreeNode;
struct _Thread_TreeNode {
	Thread_TreeNode *parent;	/* 父线程结点指针 */
	LCUI_Thread tid;		/* 线程ID */
	LCUI_Queue child;		/* 子线程列表 */
};

static Thread_TreeNode thread_tree;

static void Destroy_TreeNode( void *arg )
{
	Thread_TreeNode *ttn;
	ttn = (Thread_TreeNode *)arg;
	Queue_Destroy( &ttn->child );
}

/** 初始化线程树结点 */
static void Thread_TreeNode_Init( Thread_TreeNode *ttn )
{
	ttn->tid = 0;
	ttn->parent = NULL;
	Queue_Init( &ttn->child, sizeof(Thread_TreeNode), Destroy_TreeNode );
}

/** 从指定线程树的结点中搜索匹配的线程ID，并返回线程树结点的指针 */
static Thread_TreeNode * ThreadTree_Find(	Thread_TreeNode *ttn, 
						LCUI_Thread tid )
{
	int i, n;
	Thread_TreeNode *new_ttn;

	if( !ttn ) {
		return NULL;
	}
	/* 如果是LCUI的一个线程的ID */
	if( tid == LCUI_Sys.display_thread
	 || tid == LCUI_Sys.dev_thread
	 || tid == LCUI_Sys.timer_thread ) {
		tid = thread_tree.tid;
	}
	if(ttn != &thread_tree && ttn->tid == tid) {
		return ttn;
	}
	
	Queue_Lock( &ttn->child );
	n = Queue_GetTotal( &ttn->child );
	for(i=0; i<n; ++i) {
		new_ttn = (Thread_TreeNode*)Queue_Get( &ttn->child, i );
		if( !new_ttn ) {
			continue;
		}
		/* 如果该线程树结点指针有效，就继续递归查找 */
		new_ttn = ThreadTree_Find( new_ttn, tid );
		if( new_ttn ) {
			Queue_Unlock( &ttn->child );
			return new_ttn;
		}
	}
	Queue_Unlock( &ttn->child );
	/* 没有在for循环里返回正常的指针，那么就在这里返回NULL */
	return NULL;
}

static void __LCUIThread_PrintInfo( Thread_TreeNode *ttn )
{
	static int level=0;
	int i, n;
	Thread_TreeNode *new_ttn;

	if(ttn == NULL) {
		ttn = &thread_tree;
	}
	_DEBUG_MSG("%*s node: %p, tid: %lu %s\n", level*2, "+", ttn, ttn->tid,
		(ttn == &thread_tree) ? "(root)":"");
	n = Queue_GetTotal( &ttn->child );
	for(i=0; i<n; ++i) {
		new_ttn = Queue_Get( &ttn->child, i );
		if( !new_ttn ) {
			continue;
		}
		++level;
		__LCUIThread_PrintInfo(new_ttn);
		--level;
	}
}

/** 打印各个线程的信息 */
LCUI_API void LCUIThread_PrintInfo( void )
{
	__LCUIThread_PrintInfo(NULL);
}

/** 获取指定线程的根线程ID */
LCUI_API LCUI_Thread LCUIThread_GetRootThreadID( LCUI_Thread tid )
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

/** 在线程树中添加新的结点 */
static Thread_TreeNode *
ThreadTreeNode_AddNew( Thread_TreeNode *ttn, LCUI_Thread tid )
{
	Thread_TreeNode *p_ttn, new_ttn;
	
	Queue_Lock( &ttn->child );
	p_ttn = (Thread_TreeNode*)Queue_Add(& ttn->child, &new_ttn );
	Thread_TreeNode_Init( p_ttn );
	p_ttn->tid = tid;
	p_ttn->parent = ttn;
	Queue_Unlock( &ttn->child );
	return p_ttn;
}

/** 在线程树中删除一个结点 */
static int ThreadTreeNode_Delete( LCUI_Thread tid )
{
	int i, n, m;
	Thread_TreeNode *tt, *child, *tmp_child;
	
	tt = ThreadTree_Find(&thread_tree, tid);
	if( !tt ) {
		return -1;
	}
	 /* 得到父线程的结点指针 */
	tt = tt->parent;
	if( !tt ) {
		return -2;
	}
	Queue_Lock( &tt->child );
	n = Queue_GetTotal( &tt->child );
	if( n <= 0 ) {
		Queue_Unlock( &tt->child );
		return -3;
	}
	for(i=0; i<n; ++i) { 
		child = (Thread_TreeNode*)Queue_Get( &tt->child, i );
		if( !child || child->tid != tid ) {
			continue;
		}
		Queue_Lock( &child->child );
		m = Queue_GetTotal( &child->child );
		/** 先把此节点里的子节点列表转移至本列表里 */
		while( m-- ) {
			tmp_child = (Thread_TreeNode*)
			Queue_Get( &child->child, 0 );
			Queue_AddPointer( &tt->child, tmp_child );
			Queue_DeletePointer( &child->child, 0 );
		}
		Queue_Unlock( &child->child );
		/* 然后移除本节点 */
		Queue_Delete( &tt->child, i );
		Queue_Unlock( &tt->child );
		return 0;
	}
	Queue_Unlock( &tt->child );
	return -4;
}

/** 创建并运行一个线程 */
LCUI_API int LCUIThread_Create(	LCUI_Thread *tidp,
				void (*start_rtn)(void*),
				void *arg )
{
	Thread_TreeNode *tt;
	LCUI_Thread cur_tid;
	
	/* 先获取当前所在线程的ID */
	cur_tid = LCUIThread_SelfID();
	/* 在线程树中查找匹配的线程ID，并得到该结点的指针 */
	tt = ThreadTree_Find( &thread_tree, cur_tid );
	/* 如果没有搜索到，那就新增，并获得该结点指针 */
	if( !tt ) {
		tt = ThreadTreeNode_AddNew( &thread_tree, cur_tid );
		if( !tt ) {
			return -1; 
		}
	}
	_LCUIThread_Create( tidp, start_rtn, arg );
	/* 将得到的线程ID添加至结点 */
	ThreadTreeNode_AddNew( tt, *tidp );
	return 0;
}

/** 等待一个线程的结束，并释放该线程的资源 */
LCUI_API int LCUIThread_Join( LCUI_Thread thread, void **retval )
{
	int ret;
	ret = _LCUIThread_Join( thread, retval );
	/* 如果线程已正常退出，则从线程树中删除这个线程的结点 */
	if(ret == 0) {
		ThreadTreeNode_Delete( thread );
	}
	return ret;
}

/** 撤销一个线程 */
LCUI_API void LCUIThread_Cancel( LCUI_Thread thread )
{
	_LCUIThread_Cancel( thread );
	/* 移除记录该线程的结点 */
	ThreadTreeNode_Delete( thread );
}

/** 记录指针作为返回值，并退出线程 */
LCUI_API void LCUIThread_Exit( void* retval )
{
	_LCUIThread_Exit( retval );
}

/** 撤销线程关系树的结点中的线程以及它的所有子线程 */
static void LCUIThreadTree_Cancel( Thread_TreeNode *ttn )
{
	int i, n;
	Thread_TreeNode *child;

	if( !ttn ) {
		return;
	}
	n = Queue_GetTotal( &ttn->child );
	for(i=0; i<n; ++i) {
		child = (Thread_TreeNode*)Queue_Get( &ttn->child, 0 );
		if( !child ) {
			continue;
		}
		/* 如果该线程树结点指针有效，就继续递归调用，直到末尾，才开始往前撤销线程 */
		LCUIThreadTree_Cancel( child );
	}
	LCUIThread_Cancel( ttn->tid );
}

/** 撤销指定ID的程序的全部子线程 */
LCUI_API int LCUIApp_CancelAllThreads( LCUI_ID app_id )
{
	int i, n;
	Thread_TreeNode *ttn, *child;
	
	ttn = ThreadTree_Find(&thread_tree, (LCUI_Thread)app_id);
	if( !ttn ) {
		return -1;
	}
	n = Queue_GetTotal( &ttn->child );
	for(i=0; i<n; ++i) {
		child = (Thread_TreeNode*)Queue_Get( &ttn->child, 0 );
		if( !child ) {
			continue;
		}
		/* 只撤销程序的子线程，主线程会在程序结束后自己结束 */
		LCUIThreadTree_Cancel( child );
	}
	return 0;
}

/** 注册程序主线程 */
LCUI_API void LCUIApp_RegisterMainThread( LCUI_ID app_id )
{
	ThreadTreeNode_AddNew( &thread_tree, app_id );
}

/** 初始化线程模块 */
LCUI_API void LCUIModule_Thread_Init( void )
{
	Thread_TreeNode_Init( &thread_tree ); /* 初始化根线程结点 */
	thread_tree.tid = LCUIThread_SelfID(); /* 当前线程ID作为根结点 */
	LCUI_Sys.self_id = thread_tree.tid; /* 保存线程ID */
}

/** 停用线程模块 */
LCUI_API void LCUIModule_Thread_End( void )
{
	Queue_Destroy( &thread_tree.child );
}
