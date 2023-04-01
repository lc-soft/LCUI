# 实用工具库

- 开始日期：2021-09-28
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：[#264](https://github.com/lc-soft/LCUI/pull/264)

# 概括

添加实用工具库。

# 基本示例

实用工具库的构建命令：

```sh
# 构建库
xmake build yutil

# 构建测试用例
xmake build yutil_test

# 运行测试
xmake run yutil_test
```

list 的使用例子：

```c
#include <yutil.h>

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

# 动机

# 详细设计

参考 glib、tbox 设计 API，将一些工具类函数整理进独立的子库中，子库命名为易工具（yutil），提供以下功能模块：

- **list：** 列表。基于双向链表实现，由列表元素在内部维护的链表节点。
- **list_entry:** 双向循环链表。链表节点不包含数据，使用时需将链表节点添加为元素数据结构成员。
- **rbtree：** 红黑树。
- **dict：** 字典（哈希表）。基于 2021年 8 月为止最新的 redis 中的 dict 代码做修改。
- **string：** 提供一些额外的字符串操作函数，包括：转换成小写、生成哈希值、分割字符串、清除字符串首尾的字符。
- **strpool：** 字符串池。用于解决大量相同字符串的内存占用问题。
- **strlist：** 字符串列表。针对部件和 CSS 选择器的多字符串存储需求而设计，配合 strpool 使用。
- **logger：** 日志输出和管理。提供日志输出和按级别过滤。
- **timer：** 定时器。用法是维护一个定时器列表，每次调用处理函数时获取已过期的定时器，并调用相应的回调函数。
- **dirent：** 目录操作相关函数。
- **charset：** UTF-8 编码和解码。
- **time：** 提供高精度时间相关接口。
- **math：** 提供一些额外的数学运算操作，包括最大、最小和取整函数。

删除原 src/util 目录中的以下文件：

- parse.c：字符串解析功能只有 CSS 库在用，应该由 CSS 库内部包含它。
- object.c：主要用于实现 UI 和数据的双向绑定，属于实验性的功能，3.x 版本暂时不需要考虑它。
- task.c：用于与工作线程的任务处理，应该包含到工作线程库中。
- event.c：UI 和主循环都用到了事件机制，这块应该由它们内部实现。
- rect.c：矩形数据结构有浮点型和整型这两个版本，分别用于 UI 库和图形库，它应该由这两个库内部实现。
- steptimer.c：依赖线程库的互斥锁和条件变量操作函数，不适合放入工具库内，应该考虑由平台库提供。
- uri.c/uri.cpp：URI 的打开函数属于操作系统接口，应该由平台库提供。

子库是贡献者 [@yidianyiko](https://gitee.com/yidianyiko/yutil) 开发的一个独立的开源项目，因此以 git 子模块的方式引入，而不是直接将它的源码整合进 LCUI 项目内。

具体细节可查阅以下文档：

- 需求文档：[lib/yutil/docs/RA](../../../lib/yutil/docs/RA/)
- 调研文档：[lib/yutil/docs/Research_Report](../../../lib/yutil/docs/RA/)
- API 设计文档：[lib/yutil/docs/API](../../../lib/yutil/docs/RA/)

# 缺点

无。

# 备选方案

直接使用 glib、tbox。缺点是改动量比 yutil 要大，而且会增加 LCUI 项目的体积和构建配置复杂度。

# 采用策略

这是个破坏性改动，需要对所有用到工具函数的代码进行修改。由于主要改动都在命名上，这些文件通常只需要全局替换名称即可。

# 未解决的问题

标准库已经提供字符编解码函数 wcstombs() 和 mbstowcs()，charset 模块存在意义不大，应该考虑移除。

math 提供的最大、最小和取整函数用途不大，应该考虑移除它们。

rbtree 在 LCUI 内的用途很少，应该考虑移除它，用 dict 代替。

string 模块中的一些函数的通用性较差，应该考虑移出去，作为内部函数。

time 模块中的 `sleep()` 函数与 Linux 提供的 `sleep()` 存在名称冲突，应该考虑调整命名。

移除 bool_t？
