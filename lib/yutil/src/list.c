/* list.c -- Linked list
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
#include <stdlib.h>
#include "yutil/keywords.h"
#include "yutil/list.h"

void list_create(list_t *list)
{
	list->length = 0;
	list->head.next = list->tail.next = NULL;
	list->head.data = list->tail.data = NULL;
	list->head.prev = list->tail.prev = NULL;
}

void list_unlink(list_t *list, list_node_t *node)
{
	if (list->length > 0) {
		list->length -= 1;
	}
	if (node->next) {
		node->next->prev = node->prev;
	}
	if (node == list->tail.prev) {
		list->tail.prev = node->prev;
	}
	if (node->prev) {
		node->prev->next = node->next;
	}
	node->prev = NULL;
	node->next = NULL;
}

void list_destroy_ex(list_t *list, void (*on_destroy)(void *), int free_node)
{
	list_node_t *prev, *node;
	node = list->tail.prev;
	list->head.next = NULL;
	list->tail.prev = NULL;
	list->length = 0;
	while (node && node != &list->head) {
		prev = node->prev;
		node->prev = NULL;
		node->next = NULL;
		if (node->data && on_destroy) {
			on_destroy(node->data);
		}
		if (free_node) {
			free(node);
		}
		node = prev;
	}
}

list_node_t *list_get_node(const list_t *list, size_t pos)
{
	list_node_t *node;
	if (pos >= list->length) {
		return NULL;
	}
	if (pos > list->length / 2) {
		pos = list->length - pos;
		node = list->tail.prev;
		while (--pos >= 1 && node) {
			node = node->prev;
		}
	} else {
		pos += 1;
		node = list->head.next;
		while (--pos >= 1 && node) {
			node = node->next;
		}
	}
	return node;
}

void list_link(list_t *list, list_node_t *cur, list_node_t *node)
{
	node->prev = cur;
	node->next = cur->next;
	if (cur->next) {
		cur->next->prev = node;
	}
	cur->next = node;
	list->length += 1;
}

void list_insert_node(list_t *list, size_t pos, list_node_t *node)
{
	list_node_t *target;
	target = list_get_node(list, pos);
	if (target) {
		list_link(list, target->prev, node);
	} else {
		list_append_node(list, node);
	}
}

list_node_t *list_insert(list_t *list, size_t pos, void *data)
{
	list_node_t *node;
	node = (list_node_t *)malloc(sizeof(list_node_t));
	node->data = data;
	list_insert_node(list, pos, node);
	return node;
}

void list_delete_node(list_t *list, list_node_t *node)
{
	list_unlink(list, node);
	node->data = NULL;
	free(node);
	node = NULL;
}

void list_append_node(list_t *list, list_node_t *node)
{
	if (list->head.next) {
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

void list_delete(list_t *list, size_t pos)
{
	list_node_t *node = list_get_node(list, pos);
	list_delete_node(list, node);
}

void *list_get(const list_t *list, size_t pos)
{
	list_node_t *node = list_get_node(list, pos);
	return node ? node->data : NULL;
}

list_node_t *list_append(list_t *list, void *data)
{
	list_node_t *node;
	node = (list_node_t *)malloc(sizeof(list_node_t));
	node->data = data;
	node->next = NULL;
	list_append_node(list, node);
	return node;
}

void list_node_free(list_node_t *node)
{
	free(node);
}

void list_concat(list_t *list1, list_t *list2)
{
	if (!list2->head.next) {
		return;
	}
	if (list1->head.next) {
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
