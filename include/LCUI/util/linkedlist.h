/*
 * linkedlist.h -- Linked list
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LCUI_UTIL_LINKEDLIST_H
#define LCUI_UTIL_LINKEDLIST_H

LCUI_BEGIN_HEADER

typedef struct LinkedListNodeRec_ LinkedListNode;
typedef struct LinkedListRec_ LinkedList;

struct LinkedListNodeRec_ {
	void *data;
	LinkedListNode *prev, *next;
};

struct LinkedListRec_ {
	size_t length;
	LinkedListNode head, tail;
};

#define LinkedList_Each(node, list) \
	node = (list)->head.next; node; node = node->next

#define LinkedList_EachReverse(node, list) \
	node = (list)->tail.prev; node && node != &(list)->head; node = node->prev

#define LinkedList_ForEach(node, list) for( LinkedList_Each(node, list) )
#define LinkedList_ForEachReverse(node, list) for( LinkedList_EachReverse(node, list) )

LCUI_API LinkedListNode *LinkedList_Append(LinkedList *list, void *data);
LCUI_API LinkedListNode *LinkedList_Insert(LinkedList *list, size_t pos, void *data);
LCUI_API LinkedListNode *LinkedList_GetNode(const LinkedList *list, size_t pos);
LCUI_API LinkedListNode *LinkedList_GetNodeAtTail(const LinkedList *list, size_t pos);
LCUI_API void LinkedList_Init(LinkedList *list);
LCUI_API void *LinkedList_Get(const LinkedList *list, size_t pos);
LCUI_API void LinkedList_Unlink(LinkedList *list, LinkedListNode *node);
LCUI_API void LinkedList_Link(LinkedList *list, LinkedListNode *cur, LinkedListNode *node);
LCUI_API void LinkedList_Delete(LinkedList *list, size_t pos);
LCUI_API void LinkedList_DeleteNode(LinkedList *list, LinkedListNode *node);
LCUI_API void LinkedList_AppendNode(LinkedList *list, LinkedListNode *node);
LCUI_API void LinkedList_InsertNode(LinkedList *list, size_t pos, LinkedListNode *node);
LCUI_API void LinkedList_ClearEx(LinkedList *list, void(*on_destroy)(void*), int free_node);
LCUI_API void LinkedList_Concat(LinkedList *list1, LinkedList *list2);
LCUI_API void LinkedListNode_Delete(LinkedListNode *node);

#define LinkedList_Clear(list, func) LinkedList_ClearEx( list, func, 1 )
#define LinkedList_ClearData(list, func) LinkedList_ClearEx( list, func, 0 )

LCUI_END_HEADER

#endif
