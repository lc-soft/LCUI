/* linkedlist.c -- Linked pd_linked_list_t
 *
 * Copyright to be defined
 *
 */

//#define DEBUG
#include <stdlib.h>
#include <PandaGL/util/linkedlist.h>

void pd_linked_list_init(pd_linked_list_t *list)
{
    list->length = 0;
    list->head.next = list->tail.next = NULL;
    list->head.data = list->tail.data = NULL;
    list->head.prev = list->tail.prev = NULL;
}

void pd_linked_list_unlink(pd_linked_list_t *list, pd_linked_list_node_t *node)
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

void pd_linked_list_clear_ex(pd_linked_list_t *list, void (*on_destroy)(void *),
            int free_node)
{
    pd_linked_list_node_t *prev, *node;
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

pd_linked_list_node_t *pd_linked_list_get_node_at_tail(const pd_linked_list_t *list, size_t pos)
{
    pd_linked_list_node_t *node;
    if (pos >= list->length) {
        return NULL;
    }
    pos += 1;
    node = list->tail.prev;
    while (--pos >= 1 && node) {
        node = node->prev;
    }
    return node;
}

pd_linked_list_node_t *pd_linked_list_get_node(const pd_linked_list_t *list, size_t pos)
{
    pd_linked_list_node_t *node;
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

void pd_linked_list_link(pd_linked_list_t *list, pd_linked_list_node_t *cur,
             pd_linked_list_node_t *node)
{
    node->prev = cur;
    node->next = cur->next;
    if (cur->next) {
        cur->next->prev = node;
    }
    cur->next = node;
    list->length += 1;
}

void pd_linked_list_insert_node(pd_linked_list_t *list, size_t pos, pd_linked_list_node_t *node)
{
    pd_linked_list_node_t *target;
    target = pd_linked_list_get_node(list, pos);
    if (target) {
        pd_linked_list_link(list, target->prev, node);
    } else {
        pd_linked_list_append_node(list, node);
    }
}

pd_linked_list_node_t *pd_linked_list_insert(pd_linked_list_t *list, size_t pos, void *data)
{
    pd_linked_list_node_t *node;
    node = malloc(sizeof(pd_linked_list_node_t));
    node->data = data;
    pd_linked_list_insert_node(list, pos, node);
    return node;
}

void pd_linked_list_delete_node(pd_linked_list_t *list, pd_linked_list_node_t *node)
{
    pd_linked_list_unlink(list, node);
    node->data = NULL;
    free(node);
    node = NULL;
}

void pd_linked_list_append_node(pd_linked_list_t *list, pd_linked_list_node_t *node)
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

void pd_linked_list_delete(pd_linked_list_t *list, size_t pos)
{
    pd_linked_list_node_t *node = pd_linked_list_get_node(list, pos);
    pd_linked_list_delete_node(list, node);
}

void *pd_linked_list_get(const pd_linked_list_t *list, size_t pos)
{
    pd_linked_list_node_t *node = pd_linked_list_get_node(list, pos);
    return node ? node->data : NULL;
}

pd_linked_list_node_t *pd_linked_list_append(pd_linked_list_t *list, void *data)
{
    pd_linked_list_node_t *node;
    node = malloc(sizeof(pd_linked_list_node_t));
    node->data = data;
    node->next = NULL;
    pd_linked_list_append_node(list, node);
    return node;
}

void pd_linked_list_node_delete(pd_linked_list_node_t *node)
{
    free(node);
}

void pd_linked_list_concat(pd_linked_list_t *list1, pd_linked_list_t *list2)
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
