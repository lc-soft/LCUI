/* ***************************************************************************
 * linkedlist.h -- Linked LinkedList
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
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
	int length;
	LinkedListNode head, tail;
};

#define LinkedList_ForEach(node, list) \
	for( node = (list)->head.next; node; node = node->next )

#define LinkedList_ForEachReverse(node, list) \
	for( node = (list)->tail.prev; node && node != &(list)->head; node = node->prev )

#define LinkedList_Init(list) {				\
	(list)->length = 0;				\
	(list)->head.next = (list)->tail.next = NULL;	\
	(list)->head.data = (list)->tail.data = NULL;	\
	(list)->head.prev = (list)->tail.prev = NULL;	\
}

#define LinkedList_Clear(list, func) LinkedList_ClearEx( list, func, 1 )
#define LinkedList_ClearData(list, func) LinkedList_ClearEx( list, func, 0 )

LCUI_API LinkedListNode *LinkedList_Append( LinkedList *list, void *data );
LCUI_API LinkedListNode *LinkedList_Insert( LinkedList *list, int pos, void *data );
LCUI_API LinkedListNode *LinkedList_GetNode( LinkedList *list, int pos );
LCUI_API void *LinkedList_Get( LinkedList *list, int pos );
LCUI_API void LinkedList_Unlink( LinkedList *list, LinkedListNode *node );
LCUI_API void LinkedList_Link( LinkedList *list, LinkedListNode *cur, LinkedListNode *node );
LCUI_API void LinkedList_Delete( LinkedList *list, int pos );
LCUI_API void LinkedList_DeleteNode( LinkedList *list, LinkedListNode *node );
LCUI_API void LinkedList_AppendNode( LinkedList *list, LinkedListNode *node );
LCUI_API void LinkedList_Sort( LinkedList *list, void(*on_sort)(void*, void*) );
LCUI_API void LinkedList_ClearEx( LinkedList *list, void(*on_destroy)(void*), int free_node );
LCUI_API void LinkedList_Concat( LinkedList *list1, LinkedList *list2 );

LCUI_END_HEADER

#endif
