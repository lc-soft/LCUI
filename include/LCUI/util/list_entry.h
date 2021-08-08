/*
 * list_entry.h -- the doubly-linked list entry
 *
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com> All rights reserved.
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
#ifndef UTIL_LIST_ENTRY_H
#define UTIL_LIST_ENTRY_H

/* ------------------------------- includes --------------------------------*/
#include <assert.h>
#include <stddef.h>
#include "keywords.h"
#include "types.h"

Y_BEGIN_DECLS

/* ------------------------------- types -----------------------------------*/
// the doubly-linked list entry type
struct __list_entry_t {
	struct __list_entry_t* next;
	struct __list_entry_t* prev;
};

// the list entry head type
struct __list_entry_head_t {
	struct __list_entry_t* next;
	struct __list_entry_t* prev;
	size_t length;
	size_t entry_offset;
};

typedef struct __list_entry_t list_entry_t;
typedef struct __list_entry_head_t list_entry_head_t;

/* ------------------------------- macros ------------------------------------*/

#define list_entry_offsetof(type, member) ((size_t) & ((type*)0)->member)

#define list_entry_head_init(list, type, member) \
	list_entry_head_init_(list, list_entry_offsetof(type, member))

#define list_entry_clear(list) list_entry_head_init_(list, (list)->entry_offset)

// get the struct for this entry
#define list_entry(head, entry, type) \
	((type*)((unsigned char*)(entry) - (head)->entry_offset))

#define list_entry_get_first_entry(head, type) \
	list_entry(head, (head)->next, type)

#define list_entry_get_last_entry(head, type) \
	list_entry(head, (head)->prev, type)

#define list_entry_next_entry_(head, node, member, type) \
	list_entry(head, (node)->member.next, type)

#define list_entry_prev_entry_(head, node, member, type) \
	list_entry(head, (node)->member.prev, type)

#define list_entry_for_each(head, entry)                           \
	for (entry = (head)->next; entry != (list_entry_t*)(head); \
	     entry = entry->next)

#define list_entry_for_each_reverse(head, entry)                   \
	for (entry = (head)->prev; entry != (list_entry_t*)(head); \
	     entry = entry->prev)

#define list_entry_for_each_by_length(head, size, entry)            \
	for (entry = (head)->next, size = 0; size < (head)->length; \
	     entry = entry->next, size++)

#define list_entry_for_each_reverse_by_length(head, size, entry)             \
	for (entry = (head)->prev, size = 0; entry != (list_entry_t*)(head); \
	     entry = entry->prev, size++)

#define list_entry_for_each_entry(head, node, member, type) \
	for (node = list_entry_get_first_entry(head, type); \
	     &node->member != (list_entry_t*)(head);        \
	     node = list_entry_next_entry_(head, node, member, type))

#define list_entry_for_each_entry_reverse(head, node, member, type) \
	for (node = list_entry_get_last_entry(head, type);          \
	     &node->member != (list_entry_t*)(head);                \
	     node = list_entry_prev_entry_(head, node, member, type))

// init list entry
static inline void list_entry_init(list_entry_t* entry)
{
	entry->next = entry;
	entry->prev = entry;
}

// init list entry head
static inline void list_entry_head_init_(list_entry_head_t* list,
					 size_t entry_offset)
{
	list->next = (list_entry_t*)list;
	list->prev = (list_entry_t*)list;
	list->length = 0;
	list->entry_offset = entry_offset;
}

// destroy
static inline void list_entry_exit(list_entry_head_t* head)
{
	head->next = NULL;
	head->prev = NULL;
	head->length = 0;
	head->entry_offset = 0;
}

// the list entry length
static inline size_t list_entry_size(list_entry_head_t* head)
{
	return head->length;
}

// the list head entry
static inline list_entry_t* list_entry_get_head(list_entry_head_t* head)
{
	return head->next;
}

// the list last entry
static inline list_entry_t* list_entry_get_last(list_entry_head_t* head)
{
	return head->prev;
}

static inline bool_t list_entry_is_empty(list_entry_head_t* head)
{
	return !(head->length);
}

static inline bool_t list_entry_is_head(list_entry_head_t* head,
					list_entry_t* entry)
{
	return head->next == entry;
}

static inline bool_t list_entry_is_last(list_entry_head_t* head,
					list_entry_t* entry)
{
	return head->prev == entry;
}

// a list has just one entry?
static inline int list_is_singular(list_entry_head_t* head)
{
	return !list_entry_is_empty(head) && (head->length == 1);
}

static inline bool_t list_entry_is_valid(list_entry_head_t* list)
{
	return (list->next && list->next->prev == (list_entry_t*)list) &&
	       (list->prev && list->prev->next == (list_entry_t*)list);
}

// insert a new entry after node.
static inline void list_entry_add_next(list_entry_head_t* head,
				       list_entry_t* node, list_entry_t* entry)
{
	assert(head && node && node->next && entry);
	assert(node != entry);
	assert(list_entry_is_valid(head));

	node->next->prev = entry;
	entry->next = node->next;
	entry->prev = node;
	node->next = entry;

	head->length++;
}

// insert entry to the prev
#define list_entry_add_prev(head, node, entry) \
	list_entry_add_next(head, ((node)->prev), entry)

// insert entry to the head
static inline void list_entry_add_head(list_entry_head_t* head,
				       list_entry_t* entry)
{
	assert(entry);
	list_entry_add_next(head, (list_entry_t*)head, entry);
}

// insert entry to the tail
static inline void list_entry_add_tail(list_entry_head_t* head,
				       list_entry_t* entry)
{
	assert(head && head->prev && head->prev->next && entry);
	assert(head->prev != entry);
	assert(list_entry_is_valid(head));

	head->prev->next = entry;
	entry->prev = head->prev;
	entry->next = (list_entry_t*)head;
	head->prev = entry;

	head->length++;
}

// delete the entry (private interface)
static inline void list_entry_delete_(list_entry_head_t* head,
				      list_entry_t* prev, list_entry_t* next)
{
	assert(head && head->length && prev && next);

	assert(list_entry_is_valid(head));

	prev->next = next;
	next->prev = prev;

	head->length--;
}

// delete the entry
static inline void list_entry_delete(list_entry_head_t* head,
				     list_entry_t* entry)
{
	assert(entry);

	list_entry_delete_(head, entry->prev, entry->next);
}

// delete the entry and reinitialize it.
static inline void list_entry_delete_init(list_entry_head_t* head,
					  list_entry_t* entry)
{
	assert(entry);

	list_entry_delete_(head, entry->prev, entry->next);
	list_entry_init(entry);
}

// delete the head entry
static inline void list_entry_delete_head(list_entry_head_t* head)
{
	assert(head && head->next);

	list_entry_delete_(head, (list_entry_t*)head, head->next->next);
}

// delete the last entry
static inline void list_entry_delete_last(list_entry_head_t* head)
{
	assert(head && head->prev);

	list_entry_delete_(head, head->prev->prev, (list_entry_t*)head);
}

// delete from one list and add as another's head
static inline void list_entry_move_next(list_entry_head_t* head,
					list_entry_t* node, list_entry_t* entry)
{
	list_entry_delete(head, entry);
	list_entry_add_next(head, node, entry);
}

// move the prev entry
#define list_entry_move_prev(head, node, entry) \
	list_entry_move_next(head, (node)->prev, entry);

// move the head entry
static inline void list_entry_move_head(list_entry_head_t* head,
					list_entry_t* entry)
{
	list_entry_move_next(head, (list_entry_t*)head, entry);
}

// move the tail entry
static inline void list_entry_move_tail(list_entry_head_t* head,
					list_entry_t* entry)
{
	assert(head);

	list_entry_move_next(head, head->prev, entry);
}

// replace old entry by new one
static inline void list_entry_replace(list_entry_t* old_entry,
				      list_entry_t* new_entry)
{
	assert(old_entry && old_entry->next && old_entry->prev && new_entry);
	assert(old_entry != new_entry);

	new_entry->next = old_entry->next;
	new_entry->next->prev = new_entry;
	new_entry->prev = old_entry->prev;
	new_entry->prev->next = new_entry;
}

// replace the head entry
static inline void list_entry_replace_head(list_entry_head_t* head,
					   list_entry_t* entry)
{
	assert(head);

	list_entry_replace(head->next, entry);
}

// replace the last entry
static inline void list_entry_replace_last(list_entry_head_t* head,
					   list_entry_t* entry)
{
	assert(head);

	list_entry_replace(head->prev, entry);
}

// rotate the list to the left. move head to tail
static inline void list_rotate_left(list_entry_head_t* head)
{
	list_entry_t* entry;
	if (!list_entry_is_empty(head)) {
		entry = head->next;
		list_entry_move_tail(head, entry);
	}
}

// splice the spliced_list to list[prev, next]
static inline void list_entry_splice(list_entry_head_t* head,
				     list_entry_t* prev, list_entry_t* next,
				     list_entry_head_t* spliced_list)
{
	if (list_entry_is_empty(spliced_list)) {
		return;
	}
	spliced_list->next->prev = prev;
	prev->next = spliced_list->next;
	spliced_list->prev->next = next;
	next->prev = spliced_list->prev;

	head->length += spliced_list->length;

	list_entry_clear(spliced_list);
}

// splice the spliced_list at the list head
static inline void list_entry_splice_head(list_entry_head_t* head,
					  list_entry_head_t* spliced_list)
{
	assert(head);

	list_entry_splice(head, (list_entry_t*)head, head->next, spliced_list);
}

// splice the spliced_list at the list tail
static inline void list_entry_splice_tail(list_entry_head_t* head,
					  list_entry_head_t* spliced_list)
{
	assert(head);

	list_entry_splice(head, head->prev, (list_entry_t*)head, spliced_list);
}

Y_END_DECLS

#endif