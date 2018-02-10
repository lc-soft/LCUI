/* linkedlist.c -- Linked LinkedList
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

//#define DEBUG
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/util/linkedlist.h>

void LinkedList_Init( LinkedList *list )
{
	list->length = 0;
	list->head.next = list->tail.next = NULL;
	list->head.data = list->tail.data = NULL;
	list->head.prev = list->tail.prev = NULL;
}

void LinkedList_Unlink( LinkedList *list, LinkedListNode *node )
{
	if( list->length > 0 ) {
		list->length -= 1;
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
	LinkedListNode *prev, *node;
	node = list->tail.prev;
	list->head.next = NULL;
	list->tail.prev = NULL;
	list->length = 0;
	while( node && node != &list->head ) {
		prev = node->prev;
		node->prev = NULL;
		node->next = NULL;
		if( node->data && on_destroy ) {
			on_destroy( node->data );
		}
		if( free_node ) {
			free( node );
		}
		node = prev;
	}
}

LinkedListNode *LinkedList_GetNodeAtTail( LinkedList *list, size_t pos )
{
	LinkedListNode *node;
	if( pos >= list->length ) {
		return NULL;
	}
	pos += 1;
	node = list->tail.prev;
	while( --pos >= 1 && node ) {
		node = node->prev;
	}
	return node;
}

LinkedListNode *LinkedList_GetNode( LinkedList *list, size_t pos )
{
	LinkedListNode *node;
	if( pos >= list->length ) {
		return NULL;
	}
	if( pos > list->length / 2 ) {
		pos = list->length - pos;
		node = list->tail.prev;
		while( --pos >= 1 && node ) {
			node = node->prev;
		}
	} else {
		pos += 1;
		node = list->head.next;
		while( --pos >= 1 && node ) {
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

void LinkedList_InsertNode( LinkedList *list, size_t pos, LinkedListNode *node )
{
	LinkedListNode *target;
	target = LinkedList_GetNode( list, pos );
	if( target ) {
		LinkedList_Link( list, target->prev, node );
	} else {
		LinkedList_AppendNode( list, node );
	}
}

LinkedListNode *LinkedList_Insert( LinkedList *list, size_t pos, void *data )
{
	LinkedListNode *node;
	node = malloc( sizeof( LinkedListNode ) );
	node->data = data;
	LinkedList_InsertNode( list, pos, node );
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

void LinkedList_Delete( LinkedList *list, size_t pos )
{
	LinkedListNode *node = LinkedList_GetNode( list, pos );
	LinkedList_DeleteNode( list, node );
}

void *LinkedList_Get( LinkedList *list, size_t pos )
{
	LinkedListNode *node = LinkedList_GetNode( list, pos );
	return node ? node->data:NULL;
}

LinkedListNode *LinkedList_Append( LinkedList *list, void *data )
{
	LinkedListNode *node;
	node = malloc( sizeof( LinkedListNode ) );
	node->data = data;
	node->next = NULL;
	LinkedList_AppendNode( list, node );
	return node;
}

void LinkedListNode_Delete( LinkedListNode *node )
{
	free( node );
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
	LinkedListNode *node;
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
	size_t i, j;
	LinkedListNode *node, *cur, *next;
	for( i = 0; i < list->length - 1; ++i ) {
		int no_swap = 1;
		cur = list->head.next;
		for( j = 0; j < list->length - 1 - i; ++j ) {
			next = cur->next;
			if( cmp( cur->data, next->data ) >= 0 ) {
				LinkedList_SwapNode( list, cur, next );
				node = cur;
				cur = next;
				next = node;
				no_swap = 0;
			}
			cur = cur->next;
		}
		if( no_swap ) {
			break;
		}
	}
}
