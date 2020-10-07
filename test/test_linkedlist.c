#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"
#include "libtest.h"

void test_linkedlist(void)
{
	int arr[] = { 8, 4, 64, 16, 32, 1024 };

	size_t i;
	size_t n = sizeof(arr) / sizeof(int);

	LinkedList list;
	LinkedListNode *node;

	LinkedList_Init(&list);
	for (i = 0; i < n; ++i) {
		LinkedList_Append(&list, arr + i);
	}
	it_i("LinkedList_Append() should work", (int)list.length, (int)n);
	i = 0;
	for (LinkedList_Each(node, &list)) {
		if (node->data != arr + i) {
			break;
		}
		++i;
	}
	it_b("LinkedList_Each() should work",
	     node == NULL && i == sizeof(arr) / sizeof(int), TRUE);

	i = n - 1;
	for (LinkedList_EachReverse(node, &list)) {
		if (node->data != arr + i) {
			break;
		}
		if (i == 0) {
			i = n;
			break;
		}
		--i;
	}
	it_i("LinkedList_EachReverse() should work", (int)i, (int)n);

	LinkedList_Clear(&list, NULL);
	it_b("LinkedList_ClearData() should work",
	     list.length == 0 && !list.head.next && !list.tail.prev, TRUE);
}
