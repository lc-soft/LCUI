## 红黑树 rbtree

数据结构-红黑树

## 函数概要

```c
void         RBTree_Init(RBTree *rbt);  //初始化红黑树                  
void         RBTree_Destroy(RBTree *rbt);  //销毁红黑树                 
RBTreeNode * RBTree_First(const RBTree *rbt);  //获取第一个结点         
RBTreeNode * RBTree_Next(const RBTreeNode *node);  //获取下一个结点     
RBTreeNode * RBTree_Search(RBTree* rbt, int key);  //红黑树查找结点     
void*        RBTree_GetData(RBTree* rbt, int key);  //红黑树查找结点并获取该 结点数据 
RBTreeNode * RBTree_Insert(RBTree *rbt, int key, void *data);  //插入新的结点 
int          RBTree_Erase(RBTree *rbt, int key);  //删除红黑树中的结点  
void         RBTree_EraseNode(RBTree *rbt, RBTreeNode *node);  //按结点删除红黑树 
int          RBTree_CustomErase(RBTree *rbt, const void *keydata);  //按指定数据删除结点 
RBTreeNode * RBTree_CustomSearch(RBTree* rbt, const void *keydata);  //红黑树按指定数据查找结点 
void*        RBTree_CustomGetData(RBTree* rbt, const void *keydata);  //获取指定结点数据 
RBTreeNode * RBTree_CustomInsert(RBTree *rbt, const void *keydata, void *data);  //按指定数据插入 
```

## RBTree_Init

```c
void RBTree_Init(RBTree *rbt);
```

初始化红黑树

**参数说明：**

- `rbt` -目标红黑树 

**返回说明：**

- 无

**依赖它的文件：**

- src/keyboard.c
- src/font/fontlibrary.c
- src/gui/builder.c
- src/gui/widget_background.c
- src/gui/widget_event.c
- src/gui/widget_paint.c
- src/util/event.c

## RBTree_Destroy

```c
void RBTree_Destroy(RBTree *rbt);
```

销毁红黑树

**参数说明：**

- `rbt` -目标红黑树 

**返回说明：**

- 无

**依赖它的文件：**

- src/keyboard.c
- src/font/fontlibrary.c
- src/gui/builder.c
- src/gui/widget_background.c
- src/gui/widget_event.c
- src/gui/widget_paint.c
- src/util/event.c

## RBTree_First

```c
 RBTreeNode *RBTree_First(const RBTree *rbt);
```

返回红黑树的第一个结点

**参数说明：**

- `rbt` -目标红黑树 

**返回说明：**

- 返回第一红黑树结点



## RBTree_Next

```c
RBTreeNode *RBTree_Next(const RBTreeNode *node);
```

返回红黑树的下一个结点

**参数说明：**

- `node` -红黑树结点

**返回说明：**

- 返回输入结点的下个结点

## RBTree_Search

```c
RBTreeNode* RBTree_Search(RBTree* rbt, int key);
```

调用`rb_search_auxiliary()`搜索红黑树

**参数说明：**

- `rbt` -目标红黑树 
- `key` -搜索的序号

**返回说明：**

- 返回找到的结点

**依赖它的文件：**

- src/keyboard.c
- src/font/fontlibrary.c
- src/gui/widget_background.c
- src/gui/widget_event.c
- src/util/event.c

## RBTree_GetData

```c
void* RBTree_GetData(RBTree* rbt, int key);
```

调用`rb_search_auxiliary()`搜索红黑树找到指定 结点并返回其数据。

**参数说明：**

- `rbt` -目标红黑树 
- `key` -搜索的序号

**返回说明：**

- 返回找到的结点的(void*) 的数据

**依赖它的文件：**

- src/keyboard.c
- src/font/fontlibrary.c
- src/gui/widget_background.c
- src/gui/widget_event.c
- src/util/event.c

## RBTree_Insert

```c
RBTreeNode* RBTree_Insert(RBTree *rbt, int key, void *data);
```

插入新结点

**参数说明：**

- `rbt` -目标红黑树 
- `key` -搜索的序号
- `rbt` -新 结点的值

**返回说明：**

- 返回插入的结点

**依赖它的文件：**

- src/keyboard.c
- src/font/fontlibrary.c
- src/gui/widget_event.c
- src/util/event.c

## RBTree_Erase

```c
int RBTree_Erase(RBTree *rbt, int key);
```

按序号删除结点

**参数说明：**

- `rbt` -目标红黑树 
- `key` -搜索的结点序号

**返回说明：**

- 成功返回0，失败返回-1

**依赖它的文件：**

- src/util/event.c

## RBTree_EraseNode

```c
void RBTree_EraseNode(RBTree *rbt, RBTreeNode *node);
```

按结点删除结点

**参数说明：**

- `rbt` -目标红黑树 
- `node` -目标结点

**返回说明：**

- 

## RBTree_CustomErase

```c
  int RBTree_CustomErase(RBTree *rbt, const void *keydata);
```

自定义操作-按关键数据删除结点

**参数说明：**

- `rbt` -目标红黑树 
- `keydata` -目标数据

**返回说明：**

- 成功返回0，失败返回-1

**依赖它的文件：**

- src/gui/widget_background.c

## RBTree_CustomSearch

```c
  RBTreeNode* RBTree_CustomSearch(RBTree* rbt, const void *keydata);
```

自定义操作-按关键数据搜索结点

**参数说明：**

- `rbt` -目标红黑树 
- `keydata` -目标数据

**返回说明：**

- 返回找到的结点，未找到返回 null

## RBTree_CustomGetData

```c
  void* RBTree_CustomGetData(RBTree* rbt, const void *keydata);
```

自定义操作-按关键数据搜索结点返回其值

**参数说明：**

- `rbt` -目标红黑树 
- `keydata` -目标数据

**返回说明：**

- 返回找到的结点返回其值

**依赖它的文件：**

- src/gui/builder.c
- src/gui/widget_background.c
- src/gui/widget_event.c

## RBTree_CustomInsert

```
  RBTreeNode* RBTree_CustomInsert(RBTree *rbt, const void *keydata, void *data);
```

自定义操作-按关键数据插入结点

**参数说明：**

- `rbt` -目标红黑树 
- `keydata` -目标数据
- `data` -插入数据

**返回说明：**

- 返回新插入的结点

**依赖它的文件：**

- src/gui/builder.c
- src/gui/widget_background.c
- src/gui/widget_event.c



## **存在问题：**

1. 红黑树的源码基于 Linux 的红黑树源码修改而来的，可以选择更好的实现方式。

## 改进方案

1. 调研常见的红黑树实现分析
  - Linux rbtree
  - ngnix rbtree
  - ...
2. 比较并选择较好的红黑树实现
3. 实现
