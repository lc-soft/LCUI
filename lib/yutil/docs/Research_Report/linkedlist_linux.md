# linkedlist

Linux 中的链表采用了数据与结点分离的实现方式。这样在进行遍历等操作的时候，就可以大大的提升效率。

```c
struct list_head {
	struct list_head *next, *prev;
};
```

在这个结构体类型定义中，只包含两个指向`list_head`结构的的指针`prev`和`next`，并没有数据成员,其主要作用就是**嵌套在其它结构体类型的定义中**起到链接作用。

而在需要链表组织的数据，只需要包含一个`struct list_head`成员就可以把数据链接起来。这种通用的链表结构避免了为每个数据项类型定义自己的链表的操作。

## 操作接口

### 初始化

```c
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)
```

`LIST_HEAD(name)`在声明的时候完成了链表的初始化，将链表的前后指针指向了自己，得到一个空链表。

`INIT_LIST_HEAD(ptr)`在运行时初始化链表。

### 插入

```c
static inline void list_add(struct list_head *new, struct list_head *head);
static inline void list_add_tail(struct list_head *new, struct list_head *head);
```

`list_add`在表头插入结点。

`list_add_tail`在表尾插入结点。

### 删除

```c
static inline void list_del(struct list_head *entry) {
  __list_del_entry(entry);
  entry->next = LIST_POISON1;
  entry->prev = LIST_POISON2;
}
static inline void list_del_init(struct list_head *entry) {
  __list_del_entry(entry);
  INIT_LIST_HEAD(entry);
}
```

`list_del`用于删除结点。被删除的结点的`next`和`prev`被重新指向`LIST_POISON1`、`LIST_POISON2`

> ```c
> #define LIST_POISON1  ((void *) 0x100 + POISON_POINTER_DELTA)
> #define LIST_POISON2  ((void *) 0x122 + POISON_POINTER_DELTA)
> ```
>
> 非法访问已删除的结点将引起页故障。 0x00100100 是出错信息提示的地址。

`list_del_init`用于删除结点。被删除的结点的`next`和`prev`重新指向自己。

### 移动

```c
static inline void list_move(struct list_head *list, struct list_head *head);
static inline void list_move_tail(struct list_head *list, struct list_head *head);
```

`list_move`将一个链表的结点移动到另一个链表的首部。

`list_move_tail`将一个链表的结点移动到另一个链表的尾部。

### 合并

```c
static inline void list_splice(const struct list_head *list, struct list_head *head);
static inline void list_splice_init(struct list_head *list, struct list_head *head);
```

`list_splice`将list链表合并到head链表，原list头结点仍然指向原来的结点。

`list_move_tail`将list链表合并到head链表，原list头结点重新初始化。

### 遍历

```c
#define list_entry(ptr, type, member) container_of(ptr, type, member)

#define list_first_entry(ptr, type, member)                                    \
  list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) list_entry((ptr)->prev, type, member)

```

`list_entry`通过`list_head`成员访问一个数据项结点。

- `ptr`：指向`list_head`成员的指针
- `type`：数据项的类型
- `member`：数据项类型定义中的`list_head`成员的变量名。

> 其中:
>
> ```c
> #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
> 
> #define container_of(ptr, type, member)                     \
> 	({                                                  \
> 		void* __mptr = (void*)(ptr);                \
> 		((type*)(__mptr - offsetof(type, member))); \
> 	})
> ```
>
> 这里使用的是一个利用编译器技术的小技巧，即先求得结构成员在与结构中的偏移量，然后根据成员变量的地址反过来得出属主结构变量的地址。
>
> 对于给定一个结构，`offsetof(type,member)`是一个常量，`list_entry()`正是利用这个不变的偏移量来求得链表数据项的变量地址。

```c
#define list_for_each(pos, head)                                               \
  for (pos = (head)->next; pos != (head); pos = pos->next)
```

`list_for_each`遍历整个链表。

```c
#define list_for_each_entry(pos, head, member)                                 \
  for (pos = list_first_entry(head, typeof(*pos), member);                     \
       !list_entry_is_head(pos, head, member);                                 \
       pos = list_next_entry(pos, member))
```

`list_for_each_entry`遍历链表的时候获得链表结点数据项.

```c
#define list_for_each_prev(pos, head);
#define list_for_each_entry_reverse(pos, head, member);

```

`list_for_each_entry_reverse`是遍历的反向操作。

```c
#define list_for_each_entry_continue(pos, head, member);
```

`list_for_each_entry_continue`用于从结点pos开始遍历。

```c
#define list_prepare_entry(pos, head, member) ;
```

`list_prepare_entry`如果pos有值，则从pos开始遍历，如果没有，则从链表头开始。

> 由于上面的宏都是通过移动pos指针来达到遍历的目的。所以如果遍历的操作中包含删除pos指针所指向的结点，pos指针的移动就会被中断，
>
> Linux链表提供了两个对应于基本遍历操作的"_safe"接口：`list_for_each_safe(pos, n, head)`、`list_for_each_entry_safe(pos, n, head, member)`，它们要求调用者另外提供一个与pos同类型的指针n，在for循环中暂存pos下一个结点的地址，避免因pos结点被释放而造成的断链。

### 安全性

```c
static inline int list_empty(const struct list_head *head)
{
        return head->next == head;
}
static inline int list_empty_careful(const struct list_head *head) {
  struct list_head *next = smp_load_acquire(&head->next);
  return (next == head) && (next == head->prev);
}
```

`list_empty`仅以头指针的next是否指向自己来判断链表是否为空，判断链表是否为空。

`list_empty_careful`以头指针的next和prev是否同时指向自己来判断链表是否为空，判断链表是否为空。

> 注意：除非其他cpu的链表操作只有ist_del_init()，否则仍然不能保证安全。例如，如果另一个 CPU 可以重新 list_add() ，它就不能被使用。

```c
#define list_for_each_safe(pos, n, head)                                       \
  for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

```

`list_for_each_safe`用于从结点pos开始遍历。



## 优势：

> Linux的内核链表的一个突出优点是：由于可以方便地将其标准实现（即“小结构体”）镶嵌到任意结点当中，因此任何数据组成的链表的所有操作都被完全统一。另外，即使在代码维护过程中要对结点成员进行升级修改，也完全不影响该结点原有的链表结构。
>
> 内核链表的整体结构分为数据域和指针域两部分。指针域分为头指针和尾指针，头指针指向前一个数据，尾指针指向下一个数据；而头结点则让整个链表产生循环关系。信息存储在堆空间里的存储形式如图3所示。

![图3 数据存储状态示意图](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/5b7160be899b47f39259c3316165e9ff~tplv-k3u1fbpfcp-zoom-1.image)[数据存储状态示意图](https://link.juejin.cn/?target=https%3A%2F%2Fkns.cnki.net%2FKXReader%2FDetail%2FGetImg%3Ffilename%3Dimages%2FELEW202102075_01800.jpg%26uid%3DWEEvREcwSlJHSldTTEYzVTFPU25OeDA5Nnhqck1sYUg4YWpTNFhubDdoTT0%3D%249A4hF_YAuvQ5obgVAqNKPCYcEjKensW4IQMovwHtwkF4VYPoHbKxJw!!)

## 注意

1.  使用`list_for_each`遍历链表结点时不可销毁结点。
2. 访问结点的数据域时，如果结点类型的第一个成员不是`list_head`类型的变量时需要使用`list_entry()`获取结点类型变量的起始地址，进而访问结点的数据域，否则可以通过强制类型转换(考虑两个结构体变量的内存布局)。

## 使用场景

1. 在C语言程序中若想根据需要动态地分配和释放内存单元, 通常可以使用链表结构来组织数据；
2. 可以在用户程序自定义的结构体类型中定义一个或多个list_head指针, 用于链接不同的链表。

## 参阅

- [Linux 内核里的数据结构——双向链表 (qq.com)](https://mp.weixin.qq.com/s/AUHiQlYfKigiAXpYyMEMBw)
- [深入理解Linux内核链表](https://mp.weixin.qq.com/s/VavjKQmfsXR-0dFccBuf9Q)
- [内核双链表 (biscuitos.github.io)](https://biscuitos.github.io/blog/LIST/)

