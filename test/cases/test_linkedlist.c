#include <stdio.h>
#include <LCUI.h>
#include "ctest.h"

void test_linkedlist(void)
{
	int arr[] = { 8, 4, 64, 16, 32, 1024 };

	size_t i;
	size_t n = sizeof(arr) / sizeof(int);

	list_t list;
	list_node_t *node;

	list_init(&list);
	for (i = 0; i < n; ++i) {
		list_append(&list, arr + i);
	}
	it_i("list_append() should work", (int)list.length, (int)n);
	i = 0;
	for (list_each(node, &list)) {
		if (node->data != arr + i) {
			break;
		}
		++i;
	}
	it_b("list_each() should work",
	     node == NULL && i == sizeof(arr) / sizeof(int), TRUE);

	i = n - 1;
	for (list_each_reverse(node, &list)) {
		if (node->data != arr + i) {
			break;
		}
		if (i == 0) {
			i = n;
			break;
		}
		--i;
	}
	it_i("list_each_reverse() should work", (int)i, (int)n);

	list_clear(&list, NULL);
	it_b("list_clear() should work",
	     list.length == 0 && !list.head.next && !list.tail.prev, TRUE);
}
