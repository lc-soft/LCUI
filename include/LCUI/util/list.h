/*
 * list.h -- Linked list
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn>
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com>
 * All rights reserved.
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

#ifndef UTIL_LIST_H
#define UTIL_LIST_H

Y_BEGIN_DECLS

typedef struct list_node_t list_node_t;
typedef struct list_t list_t;

struct list_node_t {
	void *data;
	list_node_t *prev, *next;
};

struct list_t {
	size_t length;
	list_node_t head, tail;
};

#define list_each(node, list)     \
	node = (list)->head.next; \
	node;                     \
	node = node->next

#define list_each_reverse(node, list)    \
	node = (list)->tail.prev;        \
	node && (node != &(list)->head); \
	node = node->prev

#define list_for_each(node, list) for (list_each(node, list))
#define list_for_each_reverse(node, list) for (list_each_reverse(node, list))

void list_init(list_t *list);

// append data at tail
list_node_t *list_append(list_t *list, void *data);
// append node at tail
void list_append_node(list_t *list, list_node_t *node);

// insert data by pos
list_node_t *list_insert(list_t *list, size_t pos, void *data);
// insert node after cur
void list_link(list_t *list, list_node_t *cur, list_node_t *node);
// insert node by pos
void list_insert_node(list_t *list, size_t pos, list_node_t *node);

#define list_insert_head(list, data) list_insert(list, 0, data)

void *list_get(const list_t *list, size_t pos);
list_node_t *list_get_node_by_pos(const list_t *list, size_t pos);

void list_unlink(list_t *list, list_node_t *node);
void list_delete_by_pos(list_t *list, size_t pos);
void list_delete_node(list_t *list, list_node_t *node);

#define list_delete_head(list) list_delete_node(list, (list)->head.next)
#define list_delete_last(list) list_delete_node(list, (list)->tail.prev)

void list_node_free(list_node_t *node);
void list_clear_ex(list_t *list, void (*on_destroy)(void *),
			 int free_node);
void list_concat(list_t *list1, list_t *list2);

#define list_clear(list, func) list_clear_ex(list, func, 1)
#define list_clear_data(list, func) list_clear_ex(list, func, 0)

static inline list_node_t *list_get_first_node(const list_t *list)
{
	if (0 >= list->length) {
		return NULL;
	}
	return list->head.next;
}
static inline list_node_t *list_get_last_node(const list_t *list)
{
	if (0 >= list->length) {
		return NULL;
	}
	return list->tail.prev;
}
Y_END_DECLS

#endif
