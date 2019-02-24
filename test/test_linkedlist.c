#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"

int test_linkedlist(void)
{
	int ret = 0;
	int arr[] = { 8, 4, 64, 16, 32, 1024 };

	size_t i;
	size_t n = sizeof(arr) / sizeof(int);

	LinkedList list;
	LinkedListNode *node;

	LinkedList_Init(&list);
	for (i = 0; i < n; ++i) {
		LinkedList_Append(&list, arr + i);
	}
	CHECK_WITH_TEXT("LinkedList_Append() should work", list.length == n);
	i = 0;
	for (LinkedList_Each(node, &list)) {
		if (node->data != arr + i) {
			break;
		}
		++i;
	}
	CHECK_WITH_TEXT("LinkedList_Each() should work",
			node == NULL && i == sizeof(arr) / sizeof(int));

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
	CHECK_WITH_TEXT("LinkedList_EachReverse() should work", i == n);

	LinkedList_Clear(&list, NULL);
	CHECK_WITH_TEXT("LinkedList_ClearData() should work",
			list.length == 0 && !list.head.next && !list.tail.prev);
	return ret;
}
