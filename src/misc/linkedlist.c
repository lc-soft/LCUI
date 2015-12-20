/* ***************************************************************************
 * linkedlist.c -- Linked LinkedList
 * 
 * Copyright (C) 2014-2015 by Liu Chao <lc-soft@live.cn>
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
 * linkedlist.c -- 链表
 *
 * 版权所有 (C) 2014-2015 归属于 刘超 <lc-soft@live.cn>
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

//#define DEBUG
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

void LinkedList_Unlink( LinkedList *list, LinkedListNode *node )
{
	(list)->length -= 1;
	if( node->next ) {
		node->next->prev = node->prev;
	}
	if( node == list->tail.prev ) {
		list->tail.prev = node->prev;
	}
	node->prev->next = node->next;
	node->prev = NULL;
	node->next = NULL;
}

void LinkedList_ClearEx( LinkedList *list, void(*on_destroy)(void*),
			 int free_node )
{
	LinkedListNode *prev, *node = list->tail.prev;
	list->head.next = NULL;
	list->tail.prev = NULL;
	while( node && node != &list->head ) {
		prev = node->prev;
		LinkedList_Unlink( list, node );
		if( node->data && on_destroy ) {
			on_destroy( node->data );
		}
		node->data = NULL;
		if( free_node ) {
			free( node );
		}
		node = prev;
	}
	list->length = 0;
}

LinkedListNode *LinkedList_GetNode( LinkedList *list, int pos )
{
	LinkedListNode *node;
	if( pos < 0 ) {
		pos += list->length;
	}
	if( pos > list->length / 2 ) {
		pos = list->length - pos;
		node = list->tail.prev;
		while( --pos > 0 && node ) {
			node = node->prev;
		}
	} else {
		node = list->head.next;
		while( --pos > 0 && node ) {
			node = node->next;
		}
	}
	return node;
}

void LinkedList_LinkNode( LinkedList *list, LinkedListNode *cur,
			  LinkedListNode *node )
{
	node->prev = cur;
	node->next = cur->next;
	node->next->prev = node;
	cur->next = node;
	list->length += 1;
}

LinkedListNode *LinkedList_Insert( LinkedList *list, int pos, void *data )
{
	LinkedListNode *node, *target;
	node = NEW( LinkedListNode, 1 );
	node->data = data;
	target = LinkedList_GetNode( list, pos );
	if( target ) {
		LinkedList_LinkNode( list, target->prev, node );
	} else {
		LinkedList_AppendNode( list, node );
	}
	return node;
}

void LinkedList_DeleteNode( LinkedList *list, LinkedListNode *node )
{
	LinkedList_Unlink(list, node);
	node->data = NULL;
	free( node );
	node = NULL;
}

void LinkedList_AppendNode( LinkedList *list, LinkedListNode *node )
{
	if( list->head.next ) {
		node->prev = list->tail.prev;
		list->tail.prev->next = node;
		list->tail.prev = node;
	} else {
		list->head.next = node;
		list->tail.prev = node;
		node->prev = &list->head;
	}
	node->next = NULL;
	list->length += 1;
}

void LinkedList_Delete( LinkedList *list, int pos )
{
	LinkedListNode *node = LinkedList_GetNode( list, pos );
	LinkedList_DeleteNode( list, node );
}

void *LinkedList_Get( LinkedList *list, int pos )
{
	LinkedListNode *node = LinkedList_GetNode( list, pos );
	return node->data;
}

LinkedListNode *LinkedList_Append( LinkedList *list, void *data )
{
	LinkedListNode *node;
	node = NEW(LinkedListNode, 1);
	node->data = data;
	node->next = NULL;
	LinkedList_AppendNode( list, node );
	return node;
}

void LinkedList_Concat( LinkedList *list1, LinkedList *list2 )
{
	if( !list2->head.next ) {
		return;
	}
	if( list1->head.next ) {
		list1->tail.prev->next = list2->head.next;
		list2->head.next->prev = list1->tail.prev;
	} else {
		list1->head.next = list2->head.next;
		list1->head.next->prev = &list1->head;
	}
	list1->tail.prev = list2->tail.prev;
	list2->head.next = list2->tail.prev = NULL;
	list1->length += list2->length;
	list2->length = 0;
}
