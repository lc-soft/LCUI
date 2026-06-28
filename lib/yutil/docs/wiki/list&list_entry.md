## list & list_entry

list: 元素在内部维护的双向链表
list_entry: 元素在外部维护的双向循环链表

list的使用可以参考下面的的例子：
```c

void test_list(void)
{
	int arr[] = { 0, 4, 8, 16, 32, 64, 1024, 2048 };
	size_t i;
	size_t n = sizeof(arr) / sizeof(int);

	//创建一个链表结构
	list_t list;
	//创造一个结点类型的指针
	list_node_t *node;

	//初始化链表
	list_create(&list);

	// append data
	for (i = 0; i < n; ++i) {
		list_append(&list, arr + i);
	}

	// delete data
	for (i = 0; i < n; ++i) {
		list_delete(&list, 0);
	}

	// insert data
	for (i = 0; i < n; ++i) {
		list_insert(&list, i, arr + i);
	}

	// insert head
	list_insert_head(&list, 0);
	// delete head
	list_delete_head(&list);

	list_append(&list, 0);

	// delete tail
	list_delete_last(&list);

	//遍历
	i = 0;
	list_for_each(node, &list)
	{
		if (node->data != arr + i) {
			break;
		}
		++i;
	}

	// 反向遍历
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

	//销毁链表
	list_destroy(&list, NULL);
}
```

list_entry是外置式的容器，需要在外面自己定义的结构体上进行操作，例如：
```c
struct __test_list_entry_t {
	//节点结构，用于链表维护
	list_entry_t entry_node;
	//实际数据
	size_t data;
};

typedef struct __test_list_entry_t test_list_entry_t;

void test_list_entry(void)
{
	test_list_entry_t entries[8] = { { { 0 }, 0 }, { { 0 }, 1 },
					 { { 0 }, 2 }, { { 0 }, 3 },
					 { { 0 }, 4 }, { { 0 }, 5 },
					 { { 0 }, 6 }, { { 0 }, 7 } };

	// head init
	list_entry_head_t list;
	list_entry_head_init(&list, test_list_entry_t, entry_node);

	// entry init
	list_entry_t list_entry1;
	list_entry_init(&list_entry1);

	// add head
	list_entry_add_head(&list, &entries[3].entry_node);
	list_entry_add_head(&list, &entries[2].entry_node);
	list_entry_add_head(&list, &entries[1].entry_node);

	// add tail
	list_entry_add_tail(&list, &entries[4].entry_node);
	list_entry_add_tail(&list, &entries[5].entry_node);
	list_entry_add_tail(&list, &entries[6].entry_node);

	// get entry
	test_list_entry_t* node =
	    list_entry(&list, &entries[1].entry_node, test_list_entry_t);

	// list_entry_for_each
	list_entry_t* entry = list.next;
	unsigned int i = 1;
	list_entry_for_each(&list, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
		i++;
	}

	// list_entry_for_each_reverse
	i = 6;
	list_entry_for_each_reverse(&list, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
		i--;
	}

	// list_entry_for_each_by_length
	i = 0;
	list_entry_for_each_by_length(&list, i, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
	}

	// list_entry_for_each_reverse_by_length
	i = 0;
	list_entry_for_each_reverse_by_length(&list, i, entry)
	{
		node = list_entry(&list, entry, test_list_entry_t);
	}

	// list_entry_for_each_entry
	i = 0;
	list_entry_for_each_entry(&list, node, entry_node, test_list_entry_t)
	{
		i++;
	}

	// list_entry_for_each_entry_reverse
	i = 6;
	list_entry_for_each_entry_reverse(&list, node, entry_node,
					  test_list_entry_t)
	{
		i--;
	}

	// repalce entries
	list_entry_replace_head(&list, &entries[0].entry_node);
	list_entry_replace_last(&list, &entries[7].entry_node);

	// delete entries
	list_entry_delete_head(&list);
	list_entry_delete_last(&list);

	list_entry_t* head = list_entry_get_head(&list);
	list_entry_t* tail = list_entry_get_last(&list);

	list_entry_move_head(&list, tail);
	list_entry_move_tail(&list, head);

	list_rotate_left(&list);

	list_entry_head_t new_list;
	list_entry_head_init(&new_list, test_list_entry_t, entry_node);

	list_entry_splice_head(&new_list, &list);

	list_entry_splice_tail(&list, &new_list);

	// clear entries
	list_entry_clear(&list);

	list_entry_exit(&list);
}

```