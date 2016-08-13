/* ***************************************************************************
 * linkedlist.c -- Linked LinkedList
 *
 * Copyright (C) 2014-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2014-2016 归属于 刘超 <lc-soft@live.cn>
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
#include <time.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

void LinkedList_Init( LinkedList *list )
{
	list->length = 0;
	list->head.next = list->tail.next = NULL;
	list->head.data = list->tail.data = NULL;
	list->head.prev = list->tail.prev = NULL;
}

void LinkedList_Unlink( LinkedList *list, LinkedListNode *node )
{
	list->length -= 1;
	if( list->length < 0 ) {
		abort();
	}
	if( node->next ) {
		node->next->prev = node->prev;
	}
	if( node == list->tail.prev ) {
		list->tail.prev = node->prev;
	}
	if( node->prev ) {
		node->prev->next = node->next;
	}
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
	if( pos >= list->length ) {
		return NULL;
	}
	if( pos > list->length / 2 ) {
		pos = list->length - pos - 1;
		node = list->tail.prev;
		while( --pos >= 0 && node ) {
			node = node->prev;
		}
	} else {
		node = list->head.next;
		while( --pos >= 0 && node ) {
			node = node->next;
		}
	}
	return node;
}

void LinkedList_Link( LinkedList *list, LinkedListNode *cur,
		      LinkedListNode *node )
{
	node->prev = cur;
	node->next = cur->next;
	if( cur->next ) {
		cur->next->prev = node;
	}
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
		LinkedList_Link( list, target->prev, node );
	} else {
		LinkedList_AppendNode( list, node );
	}
	return node;
}

void LinkedList_DeleteNode( LinkedList *list, LinkedListNode *node )
{
	LinkedList_Unlink( list, node );
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
	return node ? node->data:NULL;
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


typedef struct SortRange_ {
	int start, end;
	LinkedListNode *snode, *enode;
} SortRange;

static SortRange NewSortRange( int s, LinkedListNode *snode, 
			       int e, LinkedListNode *enode )
{
	SortRange r;
	r.end = e;
	r.start = s;
	r.enode = enode;
	r.snode = snode;
	return r;
}

void LinkedList_SwapNode( LinkedList *list, LinkedListNode *a, LinkedListNode *b )
{
	LinkedListNode *node;/*
	void *data;
	data = a->data;
	a->data = b->data;
	b->data = data;
	return;*/
	if( list->tail.prev == a ) {
		list->tail.prev = b;
	} else if( list->tail.prev == b ) {
		list->tail.prev = a;
	}
	if( a->prev == b ) {
		a->prev = b->prev;
		a->prev->next = a;
		b->prev = a;
		b->next = a->next;
		if( b->next ) {
			b->next->prev = b;
		}
		a->next = b;
	} else if( b->prev == a ) {
		b->prev = a->prev;
		b->prev->next = b;
		a->prev = b;
		a->next = b->next;
		if( a->next ) {
			a->next->prev = a;
		}
		b->next = a;
	} else {
		node = a->prev;
		a->prev = b->prev;
		a->prev->next = a;
		b->prev = node;
		b->prev->next = b;
		node = a->next;
		a->next = b->next;
		if( a->next ) {
			a->next->prev = a;
		}
		b->next = node;
		if( b->next ) {
			b->next->prev = b;
		}
	}
}

void LinkedList_QuickSort( LinkedList *list, int (*cmp)(void*, void*) )
{
	int p = 0;
	SortRange *r;
	if( list->length <= 0 ) {
		return;
	}
	r = malloc( list->length * sizeof(SortRange) );
	r[p++] = NewSortRange( 0, list->head.next, 
			       list->length - 1, list->tail.prev );
	while( p ) {
		int left, right;
		SortRange range = r[--p];
		LinkedListNode *mnode, *lnode, *rnode, *node;
		if( range.start >= range.end ) {
			continue;
		}
		mnode = range.enode;
		left = range.start, right = range.end - 1;
		lnode = range.snode, rnode = range.enode->prev;
		while( left < right ) {
			while( left < right && 
			       cmp( lnode->data, mnode->data ) < 0 ) {
				left++, lnode = lnode->next;
			}
			while( left < right &&
			       cmp( rnode->data, mnode->data ) >= 0 ) {
				right--, rnode = rnode->prev;
			}
			LinkedList_SwapNode( list, lnode, rnode );
			if( lnode == range.snode ) {
				range.snode = rnode;
			}
			node = lnode;
			lnode = rnode;
			rnode = node;
		}
		if( cmp( lnode->data, range.enode->data) >= 0 ) {
			LinkedList_SwapNode( list, lnode, range.enode );
			if( mnode == range.enode ) {
				mnode = lnode;
			}
			node = lnode;
			lnode = range.enode;
			range.enode = node;
		} else {
			left++, lnode = lnode->next;
		}
		r[p++] = NewSortRange( range.start, range.snode, 
				       left - 1, lnode->prev );
		r[p++] = NewSortRange( left + 1, lnode->next, 
				       range.end, range.enode );
	}
	free( r );
}

void LinkedList_BubbleSort( LinkedList *list, int( *cmp )(void*, void*) )
{
	int i, j;
	LinkedListNode *node, *cur, *next;
	for( i = 0; i < list->length - 1; ++i ) {
		LCUI_BOOL no_swap = TRUE;
		cur = list->head.next;
		for( j = 0; j < list->length - 1 - i; ++j ) {
			next = cur->next;
			if( cmp( cur->data, next->data ) >= 0 ) {
				LinkedList_SwapNode( list, cur, next );
				node = cur;
				cur = next;
				next = node;
				no_swap = FALSE;
			}
			cur = cur->next;
		}
		if( no_swap ) {
			break;
		}
	}
}
