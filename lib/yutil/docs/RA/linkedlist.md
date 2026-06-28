# LinkedList

数据结构-链表的相关操作函数。

**存储结构：**

```c
struct LinkedListNodeRec_ {
	void *data;
	LinkedListNode *prev, *next;
};

struct LinkedListRec_ {
	size_t length;
	LinkedListNode head, tail;
};
```



## LinkedList_Each

```c
#define LinkedList_Each(node, list) 
```

遍历下一个结点

**参数说明：**

- `node`：缓存结点
- `list`：目标链表

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/rect.c
- src/util/object.c
- src/util/event.c
- src/thread/win32/thread.c
- src/thread/pthread/thread.c
- src/platform/windows/windows_display.c
- src/platform/windows/uwp_input.cpp
- src/platform/linux/linux_x11display.c
- src/platform/linux/linux_fbdisplay.c
- src/gui/widget/textview.c
- src/gui/widget/textedit.c
- src/gui/widget/canvas.c
- src/gui/layout/flexbox.c
- src/gui/layout/block.c
- src/gui/widget_tree.c
- src/gui/widget_task.c
- src/gui/widget_style.c
- src/gui/widget_status.c
- src/gui/widget_paint.c
- src/gui/widget_id.c
- src/gui/widget_hash.c
- src/gui/widget_event.c
- src/gui/widget_diff.c
- src/gui/widget_class.c
- src/gui/widget_base.c
- src/gui/css_library.c
- src/gui/css_parser.c
- src/gui/widget_background.c
- src/display.c
- src/ime.c
- src/main.c
- src/draw/boxshadow.c
- src/timer.c
- src/font/textlayer.c
- src/font/textstyle.c

## LinkedList_EachReverse

```c
#define LinkedList_EachReverse(node, list) 
```

遍历上一个结点

**参数说明：**

- `node`：缓存结点
- `list`：目标链表

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_style.c
- src/gui/widget_paint.c
- src/font/textstyle.c

## LinkedList_ForEach

```
#define LinkedList_ForEach(node, list) /
```

遍历所有结点

**参数说明：**

- `node`：缓存结点
- `list`：目标链表

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/platform/linux/linux_x11display.c

  

## LinkedList_ForEachReverse

```
#define LinkedList_ForEachReverse(node, list) /
```

反向遍历所有结点

**参数说明：**

- `node`：缓存结点
- `list`：目标链表

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- 无

## LinkedList_Append

```c
LinkedListNode *LinkedList_Append(LinkedList *list, void *data);

```

在链表的尾部添加新的数据结点。

**参数说明：**

- `data`：目标数据结点
- `list`：目标链表

**返回说明：**

- 返回新添加的结点

**存在问题：**

1. 无

**改进方案：

- 无

**依赖它的文件：**

- src/util/event.c
- src/platform/windows/uwp_input.cpp
- src/platform/linux/linux_x11display.c
- src/gui/widget/textedit.c
- src/gui/widget/sidebar.c
- src/gui/layout/flexbox.c
- src/gui/layout/block.c
- src/gui/widget_style.c
- src/gui/widget_paint.c
- src/gui/widget_id.c
- src/gui/widget_event.c
- src/gui/widget_background.c
- src/gui/css_parser.c
- src/font/textlayer.c
- src/gui/css_library.c
- src/worker.c
- src/display.c

## LinkedList_Insert

```c
LinkedListNode *LinkedList_Insert(LinkedList *list, size_t pos, void *data);

```

插入数据到目标为子

**参数说明：**

- `data`：数据
- `pos`：插入位置
- `list`：目标链表

**返回说明：**

- 返回新插入的结点

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/css_library.c
- src/font/textstyle.c
- src/main.c

## LinkedList_GetNode

```c
 LinkedListNode *LinkedList_GetNode(const LinkedList *list, size_t pos);

```

获取目标位置结点

**参数说明：**

- `pos`：目标位置
- `list`：目标链表

**返回说明：**

- 返回目标位置结点

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/event.c
- src/gui/widget_tree.c
- src/gui/widget_background.c
- src/worker.c
- include/LCUI/gui/widget_base.h

## LinkedList_GetNodeAtTail

```c
 LinkedListNode *LinkedList_GetNodeAtTail(const LinkedList *list, size_t pos);

```

获取尾结点

**参数说明：**

- `list`：目标链表
- `pos`：目标位置

**返回说明：**

- 返回尾结点

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_tree.c

## LinkedList_Init

```c
 void LinkedList_Init(LinkedList *list);
```

链表初始化

**参数说明：**

- `list`：目标链表

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/rect.c
- src/util/object.c
- src/util/event.c
- src/thread/win32/thread.c
- src/thread/pthread/thread.c
- src/platform/windows/windows_display.c
- src/platform/windows/uwp_input.cpp
- src/platform/linux/linux_x11display.c
- src/platform/linux/linux_fbdisplay.c
- src/gui/widget/textview.c
- src/gui/widget/textedit.c
- src/gui/widget/canvas.c
- src/gui/layout/flexbox.c
- src/gui/layout/block.c
- src/gui/widget_tree.c
- src/gui/widget_task.c
- src/gui/widget_style.c
- src/gui/widget_status.c
- src/gui/widget_paint.c
- src/gui/widget_id.c
- src/gui/widget_hash.c
- src/gui/widget_event.c
- src/gui/widget_diff.c
- src/gui/widget_class.c
- src/gui/widget_base.c
- src/gui/css_library.c
- src/gui/css_parser.c
- src/gui/widget_background.c
- **src/display.c**
- **src/ime.c**
- **src/main.c**
- src/draw/boxshadow.c
- **src/timer.c**
- src/font/textlayer.c
- src/font/textstyle.c

## LinkedList_Get

```c
 void *LinkedList_Get(const LinkedList *list, size_t pos);

```

获取目标位置结点

**参数说明：**

- `list`：目标链表
- `pos`：目标位置

**返回说明：**

- 返回无类型的数据

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_tree.c
- src/gui/widget_id.c
- src/gui/css_library.c
- src/main.c

## LinkedList_Unlink

```c
 void LinkedList_Unlink(LinkedList *list, LinkedListNode *node);

```

删除结点，但不销毁。

**参数说明：**

- `list`：目标链表
- `node`：目标结点

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/object.c
- src/util/event.c
- src/thread/win32/thread.c
- src/thread/pthread/thread.c
- src/platform/windows/windows_display.c
- src/platform/linux/linux_x11display.c
- src/gui/widget/textview.c
- src/gui/widget/canvas.c
- src/gui/widget_tree.c
- src/gui/widget_id.c
- src/gui/widget_event.c
- src/gui/widget_base.c
- src/gui/css_library.c
- src/worker.c
- **src/timer.c**

## LinkedList_Link

```c
 void LinkedList_Link(LinkedList *list, LinkedListNode *cur, LinkedListNode *node);

```

将结点插入当前结点后面

**参数说明：**

- `list`：目标链表
- `node`：目标结点
- `cur`：当前结点

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_tree.c
- src/gui/widget_base.c
- src/timer.c

## LinkedList_Delete

```c
 void LinkedList_Delete(LinkedList *list, size_t pos);

```

按序号删除结点

**参数说明：**

- `list`：目标链表
- `pos`：目标位置

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/main.c

## LinkedList_DeleteNode

```c
 void LinkedList_DeleteNode(LinkedList *list, LinkedListNode *node);

```

按结点删除结点

**参数说明：**

- `list`：目标链表
- `node`：目标结点

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/display.c
- src/font/textstyle.c
- src/gui/widget_background.c
- src/gui/widget_event.c
- src/gui/widget/textedit.c
- src/platform/windows/uwp_input.cpp
- src/util/event.c
- src/util/rect.c

## LinkedList_AppendNode

```c
 void LinkedList_AppendNode(LinkedList *list, LinkedListNode *node);

```

添加结点到链表末尾

**参数说明：**

- `list`：目标链表
- `node`：目标结点

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/object.c
- src/util/event.c
- src/thread/win32/thread.c
- src/thread/pthread/thread.c
- src/platform/windows/windows_display.c
- src/platform/linux/linux_x11display.c
- src/gui/widget/textview.c
- src/gui/widget/canvas.c
- src/gui/widget_tree.c
- src/gui/widget_event.c
- src/gui/widget_base.c
- src/gui/css_library.c
- src/timer.c
- src/ime.c

## LinkedList_InsertNode

```c
 void LinkedList_InsertNode(LinkedList *list, size_t pos, LinkedListNode *node);

```

插入结点到到链表的目标位置

**参数说明：**

- `list`：目标链表
- `node`：目标结点
- `pos`：目标位置

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_tree.c



## LinkedList_ClearEx

```c
 void LinkedList_ClearEx(LinkedList *list, void(*on_destroy)(void*), int free_node);

```

自定义清除链表相关数据

**参数说明：**

- `list`：目标链表
- `node`：目标结点
- `free_node`：是否清除结点

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

```c
#define LinkedList_Clear(list, func) LinkedList_ClearEx( list, func, 1 )
#define LinkedList_ClearData(list, func) LinkedList_ClearEx( list, func, 0 )

```



## LinkedList_Concat

```c
void LinkedList_Concat(LinkedList *list1, LinkedList *list2)

```

链接链表2到链表1后

**参数说明：**

- `list1`：链表1
- `list2`：链表2

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/util/rect.c
- src/gui/widget/textedit.c
- src/gui/widget_base.c
- src/font/textlayer.c
- src/display.c

## LinkedListNode_Delete

```c
 void LinkedListNode_Delete(LinkedListNode *node);

```

销毁结点

**参数说明：**

- `node`：目标结点

**返回说明：**

- 无

**存在问题：**

1. 无

**改进方案：**

- 无

**依赖它的文件：**

- src/gui/widget_id.c



## 使用场景

1. `src/font/textstyle.c`、``使用 LinkedList 作 *StyleTag*文本样式。涉及到以下操作
   - LinkedList_Clear
   - LinkedList_EachReverse
   - LinkedList_Each
   - LinkedList_DeleteNode
   - LinkedList_Insert
2. `include/LCUI/font/textlayer`使用 LinkedList 作` 脏矩形记录`、`样式缓存` 。涉及到以下操作
   - LinkedList_Init
   - LinkedList_Clear
   - LinkedList_Concat
   - LinkedList_Each
   - LinkedList_Append
3. `src/gui/widget/textedit.c`使用 LinkedList 作` *文本块缓冲区*`、`当前处理的标签列表` 。涉及到以下操作
   - LinkedList_Init
   - LinkedList_Clear
   - LinkedList_Concat
   - LinkedList_Each
   - LinkedList_Append
   - LinkedList_DeleteNode
4. ……

## **改进方案**

- 请调研 LCUI 中的所有 LinkedList 使用场景，确认哪些场景可以改用这种数据结构，哪些适合使用 array。
- 调研与其他库的 array 实现方式，验证是否可以将 linkedlist 更名为 array。
