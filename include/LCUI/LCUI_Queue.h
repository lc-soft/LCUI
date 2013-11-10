/* ***************************************************************************
 * LCUI_Queue.h -- basic queue processing
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
 * LCUI_Queue.h -- 基本的队列处理
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
#ifndef __LCUI_QUEUE_H__
#define __LCUI_QUEUE_H__

/********* 队列的使用模式 *******/
#define QUEUE_MODE_FREE		0
#define QUEUE_MODE_READ		1
#define QUEUE_MODE_WRITE	1<<1
#define QUEUE_MODE_LOCK		1<<2
/*****************************/

#define MAX_TID		50

LCUI_BEGIN_HEADER

typedef struct _LCUI_Queue	LCUI_Queue;
typedef struct _LCUI_Node	LCUI_Node;

typedef enum _Queue_DataMode
{
	QUEUE_DATA_MODE_ARRAY = 0,
	QUEUE_DATA_MODE_LINKED_LIST = 1
}
Queue_DataMode;

/*************** 链表的结点 ******************/
struct _LCUI_Node
{
	void *data;
	LCUI_Node *next;
	LCUI_Node *prev;
};
/*******************************************/

/****************************** 队列 ***********************************/
struct _LCUI_Queue {
	LCUI_Mutex mutex;	/**< 队列互斥锁 */
	int member_type:2;	/**< 成员类型 */ 
	int data_mode:2;	/**< 数据储存方式（数组/链表） */
	
	void **data_array;		/**< 记录队列成员(数组模式) */
	LCUI_Node data_head_node;	/**< 记录队列成员(链表模式) */
	
	size_t element_size;	/**< 成员的占用的内存空间大小，单位为字节 */
	int total_num;		/**< 记录队列成员数量 */
	int max_num;		/**< 最大的数量 */
	void (*destroy_func) (void*); /**< 析构函数，当销毁队列成员时，会调用它进行销毁 */
};
/**********************************************************************/

/* 为队列设定互斥锁，使之只能被一个线程使用 */
LCUI_API int
Queue_Lock( LCUI_Queue *queue );

/* 解开队列的互斥锁 */
LCUI_API int
Queue_Unlock( LCUI_Queue *queue );

/* 初始化队列 */
LCUI_API void
Queue_Init (LCUI_Queue * queue, size_t element_size, void (*func) (void*));

/* 
 * 功能：设定队列成员类型为指针 
 * 说明：如果队列只是用于存放指针，并且不希望队列销毁后，指针指向的内存空间也被释放，可
 * 使用该函数设置。
 * */
LCUI_API void
Queue_UsingPointer( LCUI_Queue * queue );

/* 获取队列当前总成员数量 */
LCUI_API int
Queue_GetTotal( LCUI_Queue * queue );

/* 
 * 功能：设定队列使用的数据储存模式
 * 说明：只能在初始化后且未加入成员时使用该函数
 * */
LCUI_API int
Queue_SetDataMode( LCUI_Queue * queue, Queue_DataMode mode );

/* 交换队列中指定位置两个成员的位置 */
LCUI_API int
Queue_Swap( LCUI_Queue * queue, int pos_a, int pos_b );

/* 释放队列占用的内存资源 */
LCUI_API void
Queue_Destroy( LCUI_Queue * queue );

/* 
 * 功能：从队列中获取指定位置的成员 
 * 说明：成功返回指向该成员的指针，失败返回NULL
 * 注意：请勿对返回的指针进行free操作
 * */
LCUI_API void*
Queue_Get( LCUI_Queue * queue, int pos );

/** 向队列中指定位置插入成员 */
LCUI_API void* Queue_Insert(	LCUI_Queue *queue,
				int pos,
				const void *data );

/** 向队列中指定位置插入成员的指针 */
LCUI_API void* Queue_InsertPointer(	LCUI_Queue *queue,
					int pos,
					const void *data );

/* 将队列中指定位置的成员移动至目的位置 */
LCUI_API int
Queue_Move( LCUI_Queue *queue, int des_pos, int src_pos );

/* 覆盖队列中指定位置的成员内存空间里的数据 */
LCUI_API int
Queue_Replace( LCUI_Queue * queue, int pos, const void *data );

/* 覆盖队列中指定位置的成员指针 */
LCUI_API int
Queue_ReplacePointer( LCUI_Queue * queue, int pos, const void *data );

/* 打印队列信息，一般用于调试 */
LCUI_API void
Queue_PrintInfo( LCUI_Queue *queue );

/* 
 * 功能：将新的成员添加至队列 
 * 说明：这个函数只是单纯的添加成员，如果想有更多的功能，需要自己实现
 * */
LCUI_API void* Queue_Add( LCUI_Queue *queue, const void *data );

/* 
 * 功能：将新的成员添加至队列 
 * 说明：与Queue_Add函数不同，该函数只是修改指定位置的成员指针指向的地址，主要用
 * 与部件队列的处理上，有的部件需要从一个队列转移到另一个队列上，不重新分配内存空间，
 * 直接使用原来的内存地址，这是为了避免部件转移所在队列后，部件指针无效的问题。
 * */
LCUI_API void* Queue_AddPointer( LCUI_Queue *queue, const void *data );

/* 将一个队列拼接至另一个队列的末尾 */
LCUI_API int
Queue_Cat( LCUI_Queue *des, LCUI_Queue *src );

/* 检测队列是否为空 */
LCUI_API LCUI_BOOL
Queue_Empty( LCUI_Queue *queue );

/* 查找指定成员指针所在队列中的位置 */
LCUI_API int
Queue_Find( LCUI_Queue *queue, const void *p );

/* 从队列中删除一个成员，并释放该成员占用的内存资源 */
LCUI_API int
Queue_Delete( LCUI_Queue * queue, int pos );

/* 从队列中删除一个成员指针，不对该指针指向的内存进行释放 */
LCUI_API int
Queue_DeletePointer( LCUI_Queue * queue, int pos );
LCUI_END_HEADER

#endif


