## 红黑树 rbtree

数据结构-红黑树

直接看例子：
```c
static const int count = 1 << 20;

void test_rbtree(void)
{
	int i;
	int c = 0;
	const int delete_count = 1024;
	const int search_count = 1024;

	//新建 红黑树
	rbtree_t tree = { 0 };
	//新建并 红黑树结点
	rbtree_node_t *s, *node;
	//初始化红黑树
	rbtree_init(&tree);

	char* v[] = {
		"apple", "banana", "cherry",    "grape",      "lemon",
		"mango", "pear",   "pineapple", "strawberry", "watermelon"
	};

	//插入结点
	for (i = 0; i < count; i++) {
		rbtree_insert(&tree, i + 1, NULL, v[i % 10]);
	}

	//查询结点
	for (i = 0; i < search_count; i++) {
		s = rbtree_search_by_key(&tree, (rand() % count) + 1);
		if (s) {
			c++;
		}
	}

	//删除节点
	for (i = 1; i <= delete_count; i++) {
		rbtree_delete(&tree, i, NULL);
	}

	//遍历红黑树
	for (node = rbtree_get_min(tree.root); node; node = rbtree_next(node)) {
		c++;
	}
	//销毁红黑树
	rbtree_destroy(&tree);
}
```