# dict

字典（哈希表）数据结构操作函数。

**实现方式：**

```c
/** 哈希表结点结构 */
typedef struct DictEntry {
	void *key;
	union {
		void *val;
		uint64_t u64;
		int64_t s64;
	} v;
	struct DictEntry *next; /**< 指向下一个哈希结点(形成链表) */
} DictEntry;

/** 字典内数据的类型 */
typedef struct DictType {
	unsigned int(*hashFunction)(const void *key);
	void *(*keyDup)(void *privdata, const void *key);
	void *(*valDup)(void *privdata, const void *obj);
	int(*keyCompare)(void *privdata, const void *key1, const void *key2);
	void(*keyDestructor)(void *privdata, void *key);
	void(*valDestructor)(void *privdata, void *obj);
} DictType;

/** 哈希表结构 */
typedef struct DictHashTable {
	DictEntry **table;	/**< 结点指针数组 */
	unsigned long size;	/**< 桶的数量 */
	unsigned long sizemask;	/**< mask 码，用于地址索引计算 */
	unsigned long used;	/**< 已有结点数量 */
} DictHashTable;

/** 字典结构 */
typedef struct Dict {
	DictType *type;		/**< 为哈希表中不同类型的值所使用的一族函数 */
	void *privdata;
	DictHashTable ht[2];	/**< 每个字典使用两个哈希表 */
	int rehashidx;		/**< rehash 进行到的索引位置，如果没有在 rehash ，就为 -1 */
	int iterators;		/**< 当前正在使用的 iterator 的数量 */
} Dict;

/** 用于遍历字典的迭代器 */
typedef struct DictIterator {
	Dict *d;		/**< 迭代器所指向的字典 */
	int table;		/**< 使用的哈希表号码 */
	int index;		/**< 迭代进行的索引 */
	int safe;		/**< 是否安全 */
	DictEntry *entry;	/**< 指向哈希表的当前结点 */
	DictEntry *next_entry;	/**< 指向哈希表的下个结点 */
} DictIterator;
```

**使用场景:**

## Dict_Create

```c
LCUI_API Dict *Dict_Create(DictType *type, void *privdata);
```

创建一个新字典

**参数说明：**

- `type`：设置字典内数据的类型
- `privdata`：数据

**返回说明：**

- 成功返回字典指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/strpool.c
- src/gui/widget_task.c
- src/gui/widget_prototype.c
- src/gui/widget_id.c
- src/gui/widget_helper.c
- src/gui/widget_event.c
- src/gui/widget_background.c
- src/gui/widget_attribute.c
- src/gui/css_parser.c
- src/gui/css_library.c
- src/font/fontlibrary.c



## Dict_Expand

```c
LCUI_API int Dict_Expand(Dict *d, unsigned long size);

```

扩展字典大小

**参数说明：**

- `d` – 字典指针
- `size` – 扩展大小

**返回说明：**

- 成功返回 0，失败返回-1

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_Add

```c
LCUI_API int Dict_Add(Dict *d, void *key, void *val);

```

将元素添加到目标哈希表中

**参数说明：**

- `d` – 字典指针
- `key` – 新元素的关键字
- `val` – 新元素的值

**返回说明：**

- 添加成功为 0，添加出错为 -1

**存在问题：**

1. 命名风格

**改进方案：**

- 修改命名风格。

**依赖它的文件：**

- src/util/strpool.c
- src/gui/widget_task.c
- src/gui/widget_prototype.c
- src/gui/widget_id.c
- src/gui/widget_helper.c
- src/gui/widget_event.c
- src/gui/widget_background.c
- src/gui/widget_attribute.c
- src/gui/css_parser.c
- src/gui/css_library.c
- src/font/fontlibrary.c

## Dict_AddCopy

```c
LCUI_API int Dict_AddCopy(Dict *d, void *key, const void *val);
```

将元素添加到目标哈希表中， 功能与 `Dict_Add()` 相同，但必须指定 `valDup` 才能添加成功

**参数说明：**

- `d` – 字典指针
- `key` – 新元素的关键字
- `val` – 新元素的值

**返回说明：**

- 成功返回指向目录下文件的开始，`LCUI_DirEntry`指针，失败返回 NULL

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_AddRaw

```c
LCUI_API DictEntry *Dict_AddRaw(Dict *d, void *key);
```

添加元素的底层实现函数(由 `Dict_Add` 调用)

**参数说明：**

- `d` – 字典指针
- `key` – 新元素的关键字

**返回说明：**

- 成功返回添加的字典入口

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_Replace

```c
LCUI_API int Dict_Replace(Dict *d, void *key, void *val);
```

获将新元素添加到字典，如果 `key` 已经存在，那么新元素覆盖旧元素。

**参数说明：**

- `d` – 字典指针

- `key` – 新元素的关键字
- `val` – 新元素的值

**返回说明：**

- 1， key 不存在，新建元素添加成功
- 0 ，key 已经存在，旧元素被新元素覆盖

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_ReplaceRaw

```c
LCUI_API LCUI_API DictEntry *Dict_ReplaceRaw(Dict *d, void *key);
```

添加元素的底层实现函数，获将新元素添加到字典，如果 `key` 已经存在，那么新元素覆盖旧元素。

**参数说明：**

- `d` – 字典指针

- `key` – 新元素的关键字

**返回说明：**

- 返回覆盖后的字典入口指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_DeleteNoFree

```c
int Dict_DeleteNoFree(Dict *ht, const void *key)

```

删除元素(不释放内存资源)

**参数说明：**

- `d` – 字典指针

- `key` – 删除元素的关键字

**返回说明：**

- 成功返回0，失败返回1.

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_Release

```c
LCUI_API void Dict_Release(Dict *d);

```

删除字典，释放内存资源

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/strpool.c
- src/gui/widget_prototype.c
- src/gui/widget_id.c
- src/gui/widget_event.c
- src/gui/widget_background.c
- src/gui/widget_base.c
- src/gui/widget_attribute.c
- src/gui/css_parser.c
- src/gui/css_library.c
- src/font/fontlibrary.c

## Dict_Find

```c
LCUI_API DictEntry * Dict_Find(Dict *d, const void *key);

```

在字典中按指定的 key 查找
查找过程是典型的 separate chaining find 操作

**参数说明：**

- `d` – 字典指针

- `key` – 元素的关键字

**返回说明：**

- 成功返回元素指针

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_FetchValue

```c
LCUI_API void *Dict_FetchValue(Dict *d, const void *key);

```

查找给定 key 在字典 d 中的值

**参数说明：**

- `d` – 字典指针

- `key` – 元素的关键字

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/strpool.c
- src/gui/widget_task.c
- src/gui/widget_prototype.c
- src/gui/widget_id.c
- src/gui/widget_event.c
- src/gui/widget_background.c
- src/gui/widget_attribute.c
- src/gui/css_parser.c
- src/gui/css_library.c
- src/font/fontlibrary.c

## Dict_Resize

```c
LCUI_API int Dict_Resize(Dict *d);
```

重新调整字典的大小，缩减多余空间

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 失败返回-1，成功返回0.

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_GetIterator

```c
LCUI_API DictIterator *Dict_GetIterator(Dict *d);

```

 创建一个迭代器，用于遍历哈希表结点。

> 迭代器是不安全的，只能执行 `Dict_Next` 操作。

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 成功返回迭代器

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_GetSafeIterator

```c
LCUI_API DictIterator *Dict_GetSafeIterator(Dict *d);

```

 创建一个迭代器，用于遍历哈希表结点。

> safe 属性指示迭代器是否安全，如果迭代器是安全的，那么它可以在遍历的过程中进行增删操作

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 成功返回迭代器

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_Next

```c
LCUI_API DictEntry *Dict_Next(DictIterator *iter);

```

 迭代器的推进函数。

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 成功返回迭代器

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_ReleaseIterator

```c
LCUI_API void Dict_ReleaseIterator(DictIterator *iter);
```

删除迭代器

**参数说明：**

- `iter` – 迭代器指针

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_GetRandomKey

```c
LCUI_API DictEntry *Dict_GetRandomKey(Dict *d);

```

从字典中随机获取一项

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 返回一项字典元素

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_PrintStats

```c
LCUI_API void Dict_PrintStats(Dict *d);
```

打印字典的统计信息

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_GenHashFunction

```c
LCUI_API unsigned int Dict_GenHashFunction(const unsigned char *buf, int len);

```

散列函数*(a popular one from Bernstein).*

**参数说明：**

- `buf` – 输入字符串
- `len` – 长度

**返回说明：**

- 返回散列值

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_GenHashFunction

```c
unsigned int Dict_GenCaseHashFunction(const unsigned char *buf, int len)

```

以及不区分大小写的版本的散列函数

**参数说明：**

- `buf` – 缓存
- `len` – 长度

**返回说明：**

- 返回hash值

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_IntHashFunction

```c
unsigned int Dict_IntHashFunction(unsigned int key)

```

整形散列函数*(a popular one from Bernstein).*

**参数说明：**

- `buf` – 缓存
- `len` – 长度

**返回说明：**

- 返回键值

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_IdentityHashFunction

```c
LCUI_API unsigned int Dict_IdentityHashFunction(unsigned int key);

```

以整数为键的散列函数*(a popular one from Bernstein).*

**参数说明：**

- `key` – 整形

**返回说明：**

- 返回键值

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_Empty

```
LCUI_API void Dict_Empty(Dict *d);c

```

清空字典

**参数说明：**

- `d` – 字典指针

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c

## Dict_EnableResize

```c
LCUI_API void Dict_EnableResize(void);
```

设置字典可以重新调整大小

**参数说明：**

- 无

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_DisableResize

```c
LCUI_API void Dict_DisableResize(void);
```

设置字典是否可以重新调整大小

**参数说明：**

- 无

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_Rehash

```c
LCUI_API int Dict_Rehash(Dict *d, int n);
```

字典的 rehash 函数

**参数说明：**

- `d` – 字典指针
- `n` – 要执行 rehash 的元素数量

**返回说明：**

-  0 所有元素 rehash 完毕
- 1 还有元素没有 rehash

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_RehashMilliseconds

```c
LCUI_API int Dict_RehashMilliseconds(Dict *d, int ms);
```

在指定的时间内完成 rehash 操作

**参数说明：**

- `d` – 字典指针
- `ms` – 进行 rehash 的时间，以毫秒为单位

**返回说明：**

- rehashes 完成 rehash 的元素的数量

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_SetHashFunctionSeed

```c
LCUI_API void Dict_SetHashFunctionSeed(unsigned int initval);
```

设置 Hash Seed 参与计算

**参数说明：**

- `initval` – 设置初始seed

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## Dict_GetHashFunctionSeed

```c
LCUI_API unsigned int Dict_GetHashFunctionSeed(void);
```

获取 Hash Seed

**参数说明：**

- 无

**返回说明：**

- 返回seed

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## StringKeyDict_KeyHash

```c
LCUI_API unsigned int StringKeyDict_KeyHash(const void *key);
```

获取String类型key的hash值

**参数说明：**

- `key` – void指针

**返回说明：**

- 返回hash值

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## StringKeyDict_KeyCompare

```c
LCUI_API int StringKeyDict_KeyCompare(void *privdata, const void *key1,	 const void *key2);
```

判断两个键是否冲突

**参数说明：**

- `privdata` – 数据
- `key1` – 键1
- `key1` – 键2

**返回说明：**

- 冲突返回1，否则返回0

**存在问题：**

1. 参数过多，privdata 未使用

**改进方案：**

- 删除该参数

**依赖它的文件：**

- 无

## StringKeyDict_KeyDup

```c
LCUI_API void *StringKeyDict_KeyDup(void *privdata, const void *key);

```

用新建覆盖掉原key值

**参数说明：**

- `privdata` – 数据
- `key` – 键1

**返回说明：**

- 成功则返回新键

**存在问题：**

1. 参数过多，privdata未使用

**改进方案：**

- 删除该参数

**依赖它的文件：**

- 无

## StringKeyDict_KeyDestructor

```c
LCUI_API void StringKeyDict_KeyDestructor(void *privdata, void *key);

```

销毁该键值

**参数说明：**

- `privdata` – 数据
- `key` – 键1

**返回说明：**

- 无

**存在问题：**

1. 参数过多，privdata未使用

**改进方案：**

- 删除该参数

**依赖它的文件：**

- 无

## Dict_InitStringKeyType

```c
void Dict_InitStringKeyType(DictType *t)

```

初始化字典类型

**参数说明：**

- `t` – 字典指针

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/strpool.c
- src/gui/widget_task.c
- src/gui/widget_prototype.c
- src/gui/widget_helper.c
- src/gui/widget_event.c
- src/gui/widget_background.c
- src/gui/widget_attribute.c
- src/gui/css_parser.c
- src/gui/css_library.c
- src/font/fontlibrary.c

## Dict_InitStringCopyKeyType

```c
void Dict_InitStringCopyKeyType(DictType *t)
```

初始化复制字典类型

**参数说明：**

- `t` – 字典指针

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_id.c
- src/gui/css_library.c



## **改进方案：**

- 请调研常见哈希表的实现方式，并决定是否更新源码。
