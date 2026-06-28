# API 设计规范文档

## **命名规则**

采用`小写+下划线`命名风格，不使用库名作为接口的前缀名。

### **数据类型**

数据类型名称以 `_t` 结尾，示例：

```
struct foo_t {
    int bar;
};
```

如果是结构体类型，则应该使用 `typedef` 为该类型定义别名：

```
typedef struct foo_t foo_t;
```

不使用 `typedef` 为数据类型的指针定义别名，例如以下写法是不推荐的。

```
typedef struct foo* foo_t;
```

我们推荐将复杂数据结构定义在源文件中，在头文件中只公开由 `typedef` 定义的别名，操作接口只接受该数据结构的指针，示例：

```
// example

typedef struct foo_t foo_t;

foo_t *foo_create(int bar);

void foo_destroy(foo_t *foo);

// example.c

#include "example"

struct foo_t {
    int bar;
};
```

### **函数命名**

推荐采用面向对象命名风格，以对象类型名称为前缀，后面接动词或动宾词组，相当于将 `object.doSomething()` 写成 `object_do_something()`。
函数命名中所使用的动词必须是常见的单词，例如：

- get/set
- add/remove
- new/free
- create/destroy
- init/shutdown
- open/close
- start/stop
- begin/end

示例：

```c
const char *value;
foo_t *foo;

foo_create(&foo);
foo_set_property(foo, "key", "value");
foo_get_property(foo, "key", &value);
foo_destroy(foo);
```

### 参数命名

无论是**函数命名**还是**参数命名**都该保证其含义清晰。

示例：

```c
//wrong
void foo_destroy(foo_t *f);
//correct
void foo_destroy(foo_t *foo);
```

### 宏

使用宏定义的常量采用`大写+下划线`命名风格；
使用宏定义的函数采用`小写+下划线`命名风格。

## 实现

### 注释

对每个接口都编写注释，注释规范使用`doxygen`格式。

### 合法性检查

在每个接口实现的开头，都应当进行输入变量的合法性检查。

如果有条件的话，增加异常处理措施可使程序更加可控。

## 需求及方案

各个模块要做什么改动、为什么改、怎么改。

### linkedlist

**改动需求**

1. 基于 linux 链表结构，实现新的链表结构。

**改动原因**

1.  LCUI 中的一些只需要记录和遍历数据的地方可以使用 linux 这种链表结构。

**实现方案**

1. 参考 linux 的链表实现结构，实现新的链表。

### arrary

**改动需求**

1. 将命名风格改为面向对象命名风格。
2. LCUI 用的 linkedlist 与常见链表数据结构不大一样，将 linkedlist 更名为 list。
3. 实现迭代器来遍历数组。*（低优先级）*

**改动原因**

1. 命名风格。
2. 由于需要基于 linux 链表结构，实现新的链表结构，但原有的 linkedlist 在 LCUI 中使用广泛，故考虑改名。

**实现方案**

1. 将命名风格改为面向对象命名风格。
2. 改名。
3. 考虑 LCUI 的 `LinkedList_Each()` 方案，靠宏生成迭代相关代码。

### rbtree

**改动需求**

1. 重新实现红黑树。

**改动原因**

1. 源码是基于 Linux 的红黑树源码修改而来的。

**实现方案**

1. 按照 ngnix rbtree 重新实现红黑树。
2. 重新实现原 rbtree.c 中的接口。

### dict

**改动需求**

1. 将命名风格改为面向对象命名风格。
2. 使用其他方式更新源代码。（低优先级）

**改动原因**

1. 命名风格。

**实现方案**

1. 将命名风格改为面向对象命名风格。

### logger

**改动需求**

1. 将命名风格改为面向对象命名风格。
2. 使用链表替换掉 mutex 。
3. 添加日志输出前缀。

**改动原因**

1. 命名风格。
2. 减少依赖。
3. 每次使用需要写`[foo]`前缀或无前缀。

**实现方案**

1. 将命名风格改为面向对象命名风格。
2. 使用缓存链表。
3. 可以通过修改宏添加日志前缀、子模块前缀。

### time

**改动需求**

1. 提供`int64`和`timeval` 两种方案保存时间。

**改动原因**

1. 同时提供int64和timeval两种，可以应对不同的需求。

**实现方案**

1. 参考 Linux 等开源库的 timeval 结构体重新实现time。

### timer

**改动需求**

1. 重新设计定时器的接口：改用面向对象模式代替单例模式。示例：

   ```c
   // 当前的接口设计
   LCUI_InitTimer();
   LCUI_FreeTimer();
   LCUITimer_Set();
   LCUITimer_Free();
   LCUI_ProcessTimers();
   // 新的接口设计
   TimerList_New();
   TimerList_Free();
   TimerList_Add();
   TimerList_Remove();
   TimerList_Process();
   ```

2. 允许开发者基于定时器接口实现一个定时器线程，以此摆脱 LCUI 主循环对定时器精度的影响。

3. 减少依赖项， 只需要依赖链表和时间相关函数。

**改动原因**

1. 全局共用同一个定时器列表。
2. 定时器都是在主线程中处理的，这意味着定时器的时间粒度受到帧率的限制，不能小于每帧的停留时间。
3. 用到了线程操作、互斥锁、链表、时间相关的函数，依赖项较多，与 util 的依赖少特性不符。

**实现方案**

1. 重新设计结构：原有的定时器结构依赖过多，重新设计的**结构要求**：

   - 尽量保留原有结构，去掉过多的依赖项。

   重新设计的数据结构使用**有序链表**

   - 具有**有序性**，可以**快速**找到**到期**任务；
   - 其**过期执行**、**插入**(添加定时任务)和**删除**(取消定时任务)的频率比较高。

2. 执行到期任务：**不依赖事件循环，**列如，用一个列表记录定时器，每当处理函数被调用时从列表取出已超时的定时器，然后调用这些定时器的回调函数。

3. 不再是全局共用同一个定时器列表。

### charset

**改动需求**

1. 判断标准库提供的函数是否满足现有使用需求。


**改动原因**

1. 主要使用场景是 unicode 和 utf-8 互转，以及 unicode 转本地字符编码，标准库提供的 mbstowcs() 和 wcstombs() 已经够用了。

**实现方案**

1. 使用标准库提供的 mbstowcs() 和 wcstombs() 替换掉原有的字符编码函数。


### math

**改动需求**

1. 更改命名风格。
2. 提供一些更多的数学运算操作。（低优先级）

**改动原因**

1. 原有命名风格；如将`iround`改为`round`。
2. 包含函数过少。

**实现方案**

1. 更改命名风格。
2. 结合其他开源库，添加常用的数学操作。

### dirent

**改动需求**

1. 将命名风格改为面向对象命名风格。

**改动原因**

1. 命名风格。

**实现方案**

1. 将命名风格改为面向对象命名风格。

### string

**改动需求**

1. 将命名风格改为面向对象命名风格。

**改动原因**

1. 命名风格。

**实现方案**

1. 将命名风格改为面向对象命名风格。

### strlist

**改动需求**

1. 将命名风格改为面向对象命名风格。

**改动原因**

1. 命名风格。

**实现方案**

1. 将命名风格改为面向对象命名风格。

### strpool

**改动需求**

1. 将命名风格改为面向对象命名风格。
2. 调研是否有更优同类实现方案。（低优先级）

**改动原因**

1. 命名风格。

**实现方案**

1. 将命名风格改为面向对象命名风格。

## 参考内容

- libgit2: https://libgit2.org/libgit2/
- LevelDB: https://github.com/google/leveldb/blob/master/include/leveldb/c
- [聊聊tbox的代码风格](https://tboox.org/cn/2016/07/29/code-style/)
- [LCUI 3.0 development plan](https://github.com/lc-soft/LCUI/issues/239)
- [API Design Principles - Qt Wiki](https://wiki.qt.io/API_Design_Principles)
- [如何实现一个定时器？看这一篇就够了-技术圈 (proginn.com)](https://jishuin.proginn.com/p/763bfbd54473)
