/* ***************************************************************************
 * linkedlist.h -- Linked List
 *
 * Copyright (C) 2014 by Liu Chao <lc-soft@live.cn>
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
 * linkedlist.h -- 链表
 *
 * 版权所有 (C) 2014 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

LCUI_BEGIN_HEADER

typedef struct LinkedListNodeRec_ LinkedListNode;
typedef struct LinkedListRec_ LinkedList;

struct LinkedListNodeRec_ {
	void *data;
	LinkedListNode *prev, *next;
};

struct LinkedListRec_ {
	int node_data_size;			/** 结点数据域的大小 */
	int used_node_num;			/**< 已使用的结点数量 */
	int usable_node_num;			/**< 可使用的结点数量 */
	int current_node_pos;			/**< 当前结点位置 */
	int need_free_data;			/**< 是否需要在释放结点时释放数据 */
	int need_reuse_mem;			/**< 是否需要复用结点的内存 */
	LinkedListNode *used_head_node;		/**< 已用的链表的头结点 */
	LinkedListNode *current_node;		/**< 当前结点 */
	LinkedListNode *used_tail_node;		/**< 已用的链表的尾结点 */
	LinkedListNode *usable_head_node;	/**< 可用的链表的头结点 */
	void (*destroy_func)(void*);		/**< 数据销毁函数 */
};

/** 获取当前数据元素的总数量 */
#define LinkedList_GetTotal(list) (list)->used_node_num

/** 设置是否需要重复使用结点的数据内存空间 */
#define LinkedList_SetDataMemReuse(list, val) (list)->need_reuse_mem = val

/** 设置结点中的数据是否需要释放 */
#define LinkedList_SetDataNeedFree(list, val) (list)->need_free_data = val

/** 设置结点中的数据的销毁函数 */
#define LinkedList_SetDestroyFunc(list, fn) (list)->destroy_func = fn

/** 获取当前结点中的数据 */
#define LinkedList_Get(list) ((list)->current_node ? (list)->current_node->data:NULL)

/** 判断是否处于链表末尾 */
#define LinkedList_IsAtEnd(list) ((list)->current_node_pos >= (list)->used_node_num\
				 || !(list)->current_node ? 1:0)

/** 切换至下个结点 */
#define LinkedList_ToNext(list) ((list)->current_node ? (++(list)->current_node_pos,\
				(list)->current_node = (list)->current_node->next):NULL)

#define LinkedList_ForEach(elem, pos, list) \
	for( LinkedList_Goto(list, pos) == 0 ? elem = LinkedList_Get(list):NULL; \
		!LinkedList_IsAtEnd(list); LinkedList_ToNext(list) ? elem = LinkedList_Get(list):0)

/** 初始化链表 */
LCUI_API void LinkedList_Init( LinkedList *list, int node_data_size );

/** 销毁整个链表 */
LCUI_API void LinkedList_Destroy( LinkedList *list );

/** 移除当前结点 */
LCUI_API int LinkedList_Delete( LinkedList *list );

/** 将当前结点移动至指定位置 */
LCUI_API int LinkedList_MoveTo( LinkedList *list, int pos );

/** 在当前结点前面插入新结点，并引用数据 */
LCUI_API int LinkedList_Insert( LinkedList *list, void *data );

/** 在当前结点前面插入新结点，并将数据的副本记录到该结点上 */
LCUI_API void* LinkedList_InsertCopy( LinkedList *list, void *data );

/** 将数据的副本记录至链表的相应结点上 */
LCUI_API void *LinkedList_AppendCopy( LinkedList *list, void *data );

/** 分配一个节点的数据空间 */
void *LinkedList_Alloc( LinkedList *list );

/** 将数据引用至链表的相应结点 */
LCUI_API void LinkedList_Append( LinkedList *list, void *data_ptr );

/** 跳转至指定结点 */
LCUI_API int LinkedList_Goto( LinkedList *list, int pos );

LCUI_END_HEADER

#endif
