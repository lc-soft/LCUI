/*
 * linkedlist.h -- Linked list
 *
 * Copyright to be defined
 *
 */

#ifndef PANDAGL_INCLUDE_PANDAGL_UTIL_LINKEDLIST_H_
#define PANDAGL_INCLUDE_PANDAGL_UTIL_LINKEDLIST_H_

#include <PandaGL/build.h>

typedef struct pd_linked_list_node_t_ pd_linked_list_node_t;
typedef struct pd_linked_list_t_ pd_linked_list_t;

struct pd_linked_list_node_t_ {
    void *data;
    pd_linked_list_node_t *prev, *next;
};

struct pd_linked_list_t_ {
    size_t length;
    pd_linked_list_node_t head, tail;
};

#define LinkedList_Each(node, list) \
    node = (list)->head.next; node; node = node->next

#define LinkedList_EachReverse(node, list) \
    node = (list)->tail.prev; node && node != &(list)->head; node = node->prev

#define LinkedList_ForEach(node, list) for( LinkedList_Each(node, list) )
#define LinkedList_ForEachReverse(node, list) for( LinkedList_EachReverse(node, list) )

PD_API pd_linked_list_node_t *pd_linked_list_append(pd_linked_list_t *list, void *data);
PD_API pd_linked_list_node_t *pd_linked_list_insert(pd_linked_list_t *list, size_t pos, void *data);
PD_API pd_linked_list_node_t *pd_linked_list_get_node(const pd_linked_list_t *list, size_t pos);
PD_API pd_linked_list_node_t *pd_linked_list_get_node_at_tail(const pd_linked_list_t *list, size_t pos);
PD_API void pd_linked_list_init(pd_linked_list_t *list);
PD_API void *pd_linked_list_get(const pd_linked_list_t *list, size_t pos);
PD_API void pd_linked_list_unlink(pd_linked_list_t *list, pd_linked_list_node_t *node);
PD_API void pd_linked_list_link(pd_linked_list_t *list, pd_linked_list_node_t *cur, pd_linked_list_node_t *node);
PD_API void pd_linked_list_delete(pd_linked_list_t *list, size_t pos);
PD_API void pd_linked_list_delete_node(pd_linked_list_t *list, pd_linked_list_node_t *node);
PD_API void pd_linked_list_append_node(pd_linked_list_t *list, pd_linked_list_node_t *node);
PD_API void pd_linked_list_insert_node(pd_linked_list_t *list, size_t pos, pd_linked_list_node_t *node);
PD_API void pd_linked_list_clear_ex(pd_linked_list_t *list, void(*on_destroy)(void*), int free_node);
PD_API void pd_linked_list_concat(pd_linked_list_t *list1, pd_linked_list_t *list2);
PD_API void pd_linked_list_node_delete(pd_linked_list_node_t *node);

#define pd_linked_list_clear(list, func) pd_linked_list_clear_ex( list, func, 1 )
#define pd_linked_list_clear_data(list, func) pd_linked_list_clear_ex( list, func, 0 )

#endif
