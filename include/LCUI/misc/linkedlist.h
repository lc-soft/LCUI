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

#include <stdlib.h>
#include <memory.h>

LCUI_BEGIN_HEADER

typedef struct LinkedListNodeRec_ LinkedListNode;
typedef struct LinkedListRec_ LinkedList;

struct LinkedListNodeRec_ {
        void *data;
        LinkedListNode *prev, *next;
};

struct LinkedListRec_ {
        int node_data_size;
        int used_node_num;
        int max_node_num;
        int current_node_pos;
        int need_free_data;
        int need_reuse_mem;
        LinkedListNode *head, *tail;
        LinkedListNode *current, *boundary;
        void (*destroy_func)(void*);
};

/** 设置是否需要重复使用结点的数据内存空间 */
__inline void LinkedList_SetDataMemReuse( LinkedList *list, int is_true )
{
        list->need_reuse_mem = is_true;
}

/** 设置结点中的数据是否需要释放 */
__inline void LinkedList_SetFreeData( LinkedList *list, int is_true )
{
        list->need_free_data = is_true;
}

/** 设置结点中的数据的销毁函数 */
__inline void LinkedList_SetDestroyFunc( LinkedList *list, void (*func)(void*) )
{
        list->destroy_func = func;
}

/** 获取当前结点中的数据 */
__inline void* LinkedList_Get( LinkedList *list )
{
        return list->current->data;
}

/** 获取当前结点的上个结点中的数据 */
__inline void* LinkedList_GetPrev( LinkedList *list )
{
        return list->current->prev->data;
}

/** 获取当前结点的下个结点中的数据 */
__inline void* LinkedList_GetNext( LinkedList *list )
{
        return list->current->next->data;
}

/** 切换至下个结点 */
__inline void LinkedList_ToNext( LinkedList *list )
{
        ++list->current_node_pos;
        list->current = list->current->next;
}

/** 判断是否处于链表末尾 */
__inline int LinkedList_IsAtTail( LinkedList *list )
{
        return (list->current_node_pos == list->max_node_num-1
         || list->current == list->tail);
}

/** 判断当前结点是否在链表可用结点区域内 */
__inline int LinkedList_IsInUsedArea( LinkedList *list )
{
        return (list->current_node_pos <= list->used_node_num-1);
}

/** 初始化链表 */
void LinkedList_Init( LinkedList *list, int node_data_size );

/** 销毁整个链表 */
void LinkedList_Destroy( LinkedList *list );

/** 移除当前结点 */
int LinkedList_Delete( LinkedList *list );

/** 将当前结点移动至指定位置 */
int LinkedList_MoveTo( LinkedList *list, int pos );

/** 在当前结点前面插入新结点，并记录数据 */
void LinkedList_Insert( LinkedList *list, void *data );

/** 将数据复制至链表的相应结点上的数据内存中 */
void *LinkedList_AddData( LinkedList *list, void *data );

/** 将数据的地址记录至链表的相应结点 */
void *LinkedList_AddDataByAddress( LinkedList *list, void *data_ptr );

/** 跳转至指定结点 */
int LinkedList_Goto( LinkedList *list, int pos );

LCUI_END_HEADER

#endif
