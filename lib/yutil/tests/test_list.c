#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/list.h"

void test_list_concat(void)
{
	list_t list1;
	list_t list2;
	list_create(&list1);
	list_create(&list2);
	list_append(&list1, (void *)0);
	// list_concat(&list1, &list2);
	list_append(&list2, (void *)10);
	list_concat(&list1, &list2);

	it_b("list_concat() should work",
	     (int)(&list1)->length == 2 && (int)(&list2)->length == 0 &&
		 !list2.head.next && !list2.tail.prev,
	     true);
	list_destroy(&list1, NULL);
	list_destroy(&list2, NULL);
}

void test_list(void)
{
	int arr[] = { 0, 4, 8, 16, 32, 64, 1024, 2048 };
	size_t i;
	size_t n = sizeof(arr) / sizeof(int);

	list_t list;
	list_node_t *node;

	list_create(&list);
	it_b("list_create() should work",
	     (int)(&list)->length == 0 && !list.head.next && !list.tail.prev,
	     true);

	// append data
	for (i = 0; i < n; ++i) {
		list_append(&list, arr + i);
	}
	it_i("list_append() should work", (int)(&list)->length, (int)n);
	it_i("list_append_node() should work", (int)(&list)->length, (int)n);

	// delete data
	for (i = 0; i < n; ++i) {
		list_delete(&list, 0);
	}
	it_i("list_delete() should work", (int)(&list)->length, (int)0);
	it_i("list_delete_node() should work", (int)(&list)->length, (int)0);
	it_i("list_unlink() should work", (int)(&list)->length, (int)0);
	it_i("list_node_free() should work", (int)(&list)->length, (int)0);
	// insert data
	for (i = 0; i < n; ++i) {
		list_insert(&list, i, arr + i);
	}
	it_i("list_insert() should work", (int)(&list)->length, (int)n);
	it_i("list_insert_node() should work", (int)(&list)->length, (int)n);
	it_i("list_link() should work", (int)(&list)->length, (int)n);

	// insert head
	list_insert_head(&list, 0);
	it_i("list_insert_head() should work", (int)(&list)->length,
	     (int)n + 1);
	// delete head
	list_delete_head(&list);
	it_i("list_delete_head() should work", (int)(&list)->length, (int)n);

	list_append(&list, 0);

	// delete tail
	list_delete_last(&list);
	it_i("list_delete_last() should work", (int)(&list)->length, (int)n);

	// list_for_each
	i = 0;
	list_for_each(node, &list)
	{
		if (node->data != arr + i) {
			break;
		}
		++i;
	}
	it_b("list_for_each() should work",
	     node == NULL && i == sizeof(arr) / sizeof(int), true);

	// list_for_each_reverse
	i = n - 1;
	list_for_each_reverse(node, &list)
	{
		if (node->data != arr + i) {
			break;
		}
		if (i == 0) {
			i = n;
			break;
		}
		--i;
	}
	it_i("list_for_each_reverse() should work", (int)i, (int)n);

	it_i("list_get_size() should work", (int)(&list)->length, (int)n);

	it_b("list_get() should work", list_get(&list, 3) == (arr + 3), true);

	it_b("list_get_first_node() should work",
	     list_get_first_node(&list)->data == (arr), true);
	it_b("list_get_last_node() should work",
	     list_get_last_node(&list)->data == (arr + n - 1), true);

	test_list_concat();

	list_destroy(&list, NULL);
	it_b("list_destroy() should work",
	     (int)(&list)->length == 0 && !list.head.next && !list.tail.prev,
	     true);
}
