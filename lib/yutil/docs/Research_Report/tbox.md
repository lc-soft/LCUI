# TBOX

TBOX是一个用c语言实现的跨平台开发库。

针对各个平台，封装了统一的接口，简化了各类开发过程中常用操作。

由于 TBOX 中库的分类较细，涉及较广，在 LCUI 中许多划分在 util 库中的类独立了出来，故暂只对以下与 LCUI 实用工具库中关系较近的**部分库的部分函数**进行调研。

## 概要

| 类型                                                         | 说明                                                         |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [实用工具库](https://tboox.io/#/zh-cn/getting_started?id=实用工具库) | 实现base64/32编解码 <br />**实现crc32、adler32、md5、sha1等常用hash算法** <br />**实现日志输出、断言等辅助调试工具** <br />实现url编解码 <br />实现位操作相关接口，支持各种数据格式的解析。 <br />实现swap16、swap32、swap64等位交换操作。 <br />实现一些高级的位处理接口<br />实现单例模块，实现全局线程安全 <br />实现option模块，对命令行参数进行解析。 |
| [字符编码库](https://tboox.io/#/zh-cn/getting_started?id=字符编码库) | **支持utf8、utf16、gbk、gb2312、uc2、uc4 之间的互相转码，并且支持大小端格式。** |
| [平台库](https://tboox.io/#/zh-cn/getting_started?id=平台库) | 提供file、**directory**、socket、thread、**time**等常用系统接口 <br />提供atomic、atomic64接口 <br />**提供高精度、低精度定时器 <br />提供高性能的线程池操作 <br />**提供event、mutex、semaphore、spinlock等事件、互斥、信号量、自旋锁操作 <br />提供获取函数堆栈信息的接口，方便调试和错误定位 <br />提供跨平台动态库加载接口（如果系统支持的话） <br />提供io轮询器，针对epoll, poll, select, kqueue进行跨平台封装 <br />提供跨平台上下文切换接口，主要用于协程实现 |
| [数学运算库](https://tboox.io/#/zh-cn/getting_started?id=数学运算库) | 提供各种精度的定点运算支持 <br />提供随机数生成器            |
| [容器库](https://tboox.io/#/zh-cn/getting_started?id=容器库) | **提供哈希、链表、数组、队列、堆栈、最小最大堆等常用容器。 <br />**支持各种常用成员类型，在原有的容器期初上，其成员类型还可以完全自定义扩展。 <br />所有容器都支持迭代器操作。 <br />大部分容器都可以支持基于stream的序列化和反序列化操作。 |
| [object库](https://tboox.io/#/zh-cn/getting_started?id=object库) | 轻量级类apple的CoreFoundation库，支持object、**dictionary、array、string、**number、date、data等常用对象，并且可以方便扩展自定义对象的序列化。 <br />支持对xml、json、binary以及apple的plist(xplist/bplist)格式序列化和反序列化。 |



## 实用工具库

由于包含函数过多，故结合分析报告，暂只调研以下内容

- "trace.h"

### trace

**使用场景：**

- 日志输出、断言等辅助调试工具
- trace 追踪信息，记录程序日志

#### tb_trace_init

```c
tb_bool_t           tb_trace_init(tb_noarg_t);

```

trace初始化

#### tb_trace_exit

```c
tb_void_t           tb_trace_exit(tb_noarg_t);

```

退出trace模式

#### tb_trace_mode

```c
tb_size_t           tb_trace_mode(tb_noarg_t);

```

进入trace模式

#### tb_trace_mode_set

```c
tb_bool_t           tb_trace_mode_set(tb_size_t mode);

```

设置trace模式

#### tb_trace_file

```c
tb_file_ref_t       tb_trace_file(tb_noarg_t);

```

追踪 文件

#### tb_trace_file_set

```c
tb_bool_t           tb_trace_file_set(tb_file_ref_t file);

```

设置追踪 文件

#### tb_trace_file_set

```c
tb_bool_t           tb_trace_file_set(tb_file_ref_t file);

```

设置追踪 文件

#### tb_trace_file_set_path

```c
tb_bool_t           tb_trace_file_set_path(tb_char_t const* path, tb_bool_t bappend);

```

设置trace文件保存路径

#### tb_trace_done_with_args

```c
tb_void_t           tb_trace_done_with_args(tb_char_t const* prefix, tb_char_t const* module, tb_char_t const* format, tb_va_list_t args);

```

用参数完成 trace

#### tb_trace_done

```c
tb_void_t           tb_trace_done(tb_char_t const* prefix, tb_char_t const* module, tb_char_t const* format, ...);

```

完成 trace

#### tb_trace_tail

```c
tb_void_t           tb_trace_tail(tb_char_t const* format, ...);

```

完成 trace 时跟着的尾巴

#### tb_trace_sync

```c
tb_void_t           tb_trace_sync(tb_noarg_t);
```

同步 trace 

## 字符编码库

由于包含函数过多，故结合分析报告，暂只调研以下内容

- charset
- ascii
- utf8

### charset

**使用场景：**

- **设置统一接口的字符集，支持utf8、utf16、gbk、gb2312、uc2、uc4 之间的互相转码，并且支持大小端格式。**

#### tb_charset_name

```c
tb_char_t const* tb_charset_name(tb_size_t type)

```

返回字符编码名字

#### tb_charset_type

```c
tb_size_t           tb_charset_type(tb_char_t const* name);

```

返回字符编码类型

#### tb_charset_find

```c
tb_charset_ref_t    tb_charset_find(tb_size_t type);

```

找到该类字符编码

#### tb_charset_conv_bst

```c
tb_long_t           tb_charset_conv_bst(tb_size_t ftype, tb_size_t ttype, tb_static_stream_ref_t fst, tb_static_stream_ref_t tst);

```

根据静态流中转换字符编码

#### tb_charset_conv_cstr

```c
tb_long_t           tb_charset_conv_cstr(tb_size_t ftype, tb_size_t ttype, tb_char_t const* cstr, tb_byte_t* data, tb_size_t size);

```

根据char*类型字符串中转换字符编码

#### tb_charset_conv_data

```c
tb_long_t           tb_charset_conv_data(tb_size_t ftype, tb_size_t ttype, tb_byte_t const* idata, tb_size_t isize, tb_byte_t* odata, tb_size_t osize);

```

根据数据中转换字符编码

#### tb_charset_find

```c
tb_charset_ref_t    tb_charset_find(tb_size_t type);

```

找到该类字符编码

### *ascii*

**使用场景：**

- **获取和设置ASCII编码。**

#### tb_charset_ascii_get

```c
tb_long_t tb_charset_ascii_get(tb_static_stream_ref_t sstream, tb_bool_t be, tb_uint32_t* ch);

```

获取ascii字符编码

#### tb_charset_ascii_set

```c
tb_long_t tb_charset_ascii_set(tb_static_stream_ref_t sstream, tb_bool_t be, tb_uint32_t ch);

```

设置环境为ascii字符编码



### **utf8**

**使用场景：**

- **获取和设置*utf8*编码。**

#### tb_charset_utf8_get

```c
tb_long_t tb_charset_utf8_get(tb_static_stream_ref_t sstream, tb_bool_t be, tb_uint32_t* ch);

```

获取utf8字符编码

#### tb_charset_utf8_set

```c
tb_long_t tb_charset_utf8_set(tb_static_stream_ref_t sstream, tb_bool_t be, tb_uint32_t ch);

```

设置环境为utf8字符编码

## [平台库](https://tboox.io/#/zh-cn/getting_started?id=平台库)

由于包含函数过多，故结合分析报告，暂只调研以下内容

- windows/charset.c
- windows/directory.c
- time.h

### windows/charset.c

**使用场景：**

- **提供windows的字符编码转换。**

#### tb_charset_conv_impl

```c
tb_long_t tb_charset_conv_impl(tb_size_t ftype, tb_size_t ttype, tb_static_stream_ref_t fst, tb_static_stream_ref_t tst)

```

封装统一的字符编码转换接口

#### tb_charset_conv_impl_char_cp_bytes

```c
static tb_size_t tb_charset_conv_impl_char_cp_bytes(tb_byte_t const* idata, tb_size_t isize, tb_uint_t cp)
```

私用接口

#### tb_charset_conv_impl_char_cp_to_utf8

```c
static tb_size_t tb_charset_conv_impl_char_cp_to_utf8(tb_byte_t const* idata, tb_size_t isize, tb_byte_t* odata, tb_size_t osize, tb_uint_t cp)

```

私用接口：将字符编码转换为utf8格式

#### tb_charset_conv_impl_char_cp_to_utf16le

```c
static tb_size_t tb_charset_conv_impl_char_cp_to_utf16le(tb_byte_t const* idata, tb_size_t isize, tb_byte_t* odata, tb_size_t osize, tb_uint_t cp)

```

私用接口：将字符编码转换为utf16le格式

#### tb_charset_conv_impl_char_utf16le_to_cp

```c
static tb_size_t tb_charset_conv_impl_char_utf16le_to_cp(tb_byte_t const* idata, tb_size_t isize, tb_byte_t* odata, tb_size_t osize, tb_uint_t cp)

```

私用接口：将utf16le字符编码转换为自定义设置的编码格式

#### tb_charset_conv_impl_cp_to_utf8

```c
static tb_long_t tb_charset_conv_impl_cp_to_utf8(tb_static_stream_ref_t fst, tb_static_stream_ref_t tst, tb_uint_t cp)

```

私用接口：将自定义设置的编码格式转换为utf8格式

#### tb_charset_conv_impl_cp_to_utf16le

```c
static tb_long_t tb_charset_conv_impl_cp_to_utf16le(tb_static_stream_ref_t fst, tb_static_stream_ref_t tst, tb_uint_t cp)

```

私用接口：将自定义设置的编码格式转换为utf16le格式

#### tb_charset_conv_impl_utf8_to_cp

```c
static tb_long_t tb_charset_conv_impl_utf8_to_cp(tb_static_stream_ref_t fst, tb_static_stream_ref_t tst, tb_uint_t cp)

```

私用接口：将utf8格式转换为自定义设置的编码格式

#### tb_charset_conv_impl_utf16le_to_cp

```c
static tb_long_t tb_charset_conv_impl_utf16le_to_cp(tb_static_stream_ref_t fst, tb_static_stream_ref_t tst, tb_uint_t cp)

```

私用接口：将utf16le字符编码转换为自定义设置的编码格式



### directory

系统接口

**使用场景：**

- **提供windows的目录操作接口。**

#### tb_directory_create

```c
tb_bool_t tb_directory_create(tb_char_t const* path)

```

创建目录

#### tb_directory_remove

```c
tb_bool_t tb_directory_remove(tb_char_t const* path)

```

删除目录

#### tb_directory_home

```c
tb_size_t tb_directory_home(tb_char_t* path, tb_size_t maxn)

```

获取 home 目录

#### tb_directory_current

```c
tb_size_t tb_directory_current(tb_char_t* path, tb_size_t maxn)

```

获取 当前 目录

#### tb_directory_current_set

```c
tb_bool_t tb_directory_current_set(tb_char_t const* path)

```

设置当前目录

#### tb_directory_temporary

```c
tb_size_t tb_directory_temporary(tb_char_t* path, tb_size_t maxn)

```

获取 临时 目录

#### tb_directory_walk

```c
tb_void_t tb_directory_walk(tb_char_t const* path, tb_long_t recursion, tb_bool_t prefix, tb_directory_walk_func_t func, tb_cpointer_t priv)

```

进入目录

#### tb_directory_copy

```c
tb_bool_t tb_directory_copy(tb_char_t const* path, tb_char_t const* dest)

```

复制目录



### time

关于时间的存储方式：tbox中使用`__int64`、`signed long (long) `存储时间

**使用场景：**

- **提供时间相关接口**

#### tb_usleep

```c
tb_void_t       tb_usleep(tb_size_t us);

```

停us秒(微秒)

#### tb_msleep

```c
tb_void_t       tb_msleep(tb_size_t ms);

```

停us秒(毫秒)

#### tb_sleep

```c
tb_void_t       tb_sleep(tb_size_t s);

```

停s秒(秒)

#### tb_msleep

```c
tb_hong_t       tb_mclock(tb_noarg_t);

```

返回当前时间（微秒）

#### tb_msleep

```c
tb_hong_t       tb_uclock(tb_noarg_t);

```

返回当前时间（毫秒）

#### tb_msleep

```c
tb_bool_t       tb_gettimeofday(tb_timeval_t* tv, tb_timezone_t* tz);

```

返回1970-01-01 00:00:000格式时间

### *timer*

**使用场景：**

- **提供高精度、低精度定时器**

#### tb_timer

```c
tb_timer_ref_t      tb_timer(tb_noarg_t);

```

全局定时器

#### tb_timer_init

```c
tb_timer_ref_t      tb_timer_init(tb_size_t grow, tb_bool_t ctime);

```

初始化定时器

#### tb_timer_exit

```c
tb_void_t           tb_timer_exit(tb_timer_ref_t timer);

```

退出定时器

#### tb_timer_kill

```c
tb_void_t           tb_timer_kill(tb_timer_ref_t timer);

```

杀死定时器

#### tb_timer_clear

```c
tb_void_t           tb_timer_clear(tb_timer_ref_t timer);

```

清理定时器

#### tb_timer_delay

```c
tb_size_t           tb_timer_delay(tb_timer_ref_t timer);

```

延长定时器

#### tb_timer_top

```c
tb_hize_t           tb_timer_top(tb_timer_ref_t timer);

```

返回定时器何时执行

#### tb_timer_spak

```c
tb_bool_t           tb_timer_spak(tb_timer_ref_t timer);

```

单线程外部循环的spak计时器

#### tb_timer_loop

```c
tb_void_t           tb_timer_loop(tb_timer_ref_t timer);

```

外部线程的循环定时器

#### tb_timer_task_post

```c
tb_void_t           tb_timer_task_post(tb_timer_ref_t timer, tb_size_t delay, tb_bool_t repeat, tb_timer_task_func_t func, tb_cpointer_t priv);

```

延迟后的后定时器任务，过期后将自动删除

#### tb_timer_task_post_at

```c
tb_void_t           tb_timer_task_post_at(tb_timer_ref_t timer, tb_hize_t when, tb_size_t period, tb_bool_t repeat, tb_timer_task_func_t func, tb_cpointer_t priv);

```

按照绝对时间删除定时器任务，并在自动删除后，过期

#### tb_timer_task_post_after

```c
tb_void_t           tb_timer_task_post_after(tb_timer_ref_t timer, tb_hize_t after, tb_size_t period, tb_bool_t repeat, tb_timer_task_func_t func, tb_cpointer_t priv);

```

按照相对时间删除定时器任务，并在自动删除后，过期

#### tb_timer_task_init

```c
tb_timer_task_ref_t tb_timer_task_init(tb_timer_ref_t timer, tb_size_t delay, tb_bool_t repeat, tb_timer_task_func_t func, tb_cpointer_t priv);

```

初始化延迟定时器任务，需要手动删除它

#### tb_timer_task_init_at

```c
tb_timer_task_ref_t tb_timer_task_init_at(tb_timer_ref_t timer, tb_hize_t when, tb_size_t period, tb_bool_t repeat, tb_timer_task_func_t func, tb_cpointer_t priv);

```

 按照绝对时间初始化定时器任务，需要手动删除它

#### tb_timer_task_init_after

```c
tb_timer_task_ref_t tb_timer_task_init_after(tb_timer_ref_t timer, tb_hize_t after, tb_size_t period, tb_bool_t repeat, tb_timer_task_func_t func, tb_cpointer_t priv);

```

 按照相对时间初始化定时器任务，需要手动删除它

#### tb_timer_task_exit

```c
tb_void_t           tb_timer_task_exit(tb_timer_ref_t timer, tb_timer_task_ref_t task);
```

 退出定时器任务，如果尚未调用该任务，则不会调用该任务

#### tb_timer_task_kill

```c
tb_void_t           tb_timer_task_kill(tb_timer_ref_t timer, tb_timer_task_ref_t task);

```

杀死定时器任务，如果尚未调用该任务，则不会调用该任务



## [数学运算库](https://tboox.io/#/zh-cn/getting_started?id=数学运算库)

鉴于LCUI中数学运算库只包含到了max、min、round函数，现只调研tbox中相关函数

- src\tbox\libm\round.h
- src\tbox\prefix\utils.h\tb_max()
- src\tbox\prefix\utils.h\tb_min()

**使用场景：**

- **提供相关数学函数**

#### tb_max

```c
#define tb_max(x, y)                    (((x) > (y))? (x) : (y))

```

返回最大值

#### tb_min

```c
#define tb_min(x, y)                    (((x) < (y))? (x) : (y))

```

返回最小值

#### tb_round

```c
#define tb_round(x)                 ((x) > 0? (tb_int32_t)((x) + 0.5) : (tb_int32_t)((x) - 0.5))

```

## [容器库](https://tboox.io/#/zh-cn/getting_started?id=容器库)

由于包含函数过多，故结合

分析报告，暂只调研以下内容

-  "array_iterator.h"

-  "hash_map.h"

-  "list.h"

-  "list_entry"

-  ~~"single_list.h"~~

-  ~~"single_list_entry.h"~~

  

### array_iterator

**存储方式：**

```c
typedef struct __tb_array_iterator_t
{
    /// the iterator base
    tb_iterator_t           base;

    /// the items
    tb_pointer_t            items;

    /// the items count
    tb_size_t               count;

}tb_array_iterator_t, *tb_array_iterator_ref_t;
```

**使用场景：**

- **数组迭代器**

#### tb_array_iterator_init_ptr

```c
tb_iterator_ref_t   tb_array_iterator_init_ptr(tb_array_iterator_ref_t iterator, tb_pointer_t* items, tb_size_t count);

```

初始化数组指针类型元素的迭代器。

#### tb_array_iterator_init_mem

```c
tb_iterator_ref_t   tb_array_iterator_init_mem(tb_array_iterator_ref_t iterator, tb_pointer_t items, tb_size_t count, tb_size_t size);

```

内存元素的初始化数组迭代器

#### tb_array_iterator_init_str

```c
tb_iterator_ref_t   tb_array_iterator_init_str(tb_array_iterator_ref_t iterator, tb_char_t** items, tb_size_t count);

```

c字符串元素的 初始化数组迭代器

#### tb_array_iterator_init_istr

```c
tb_iterator_ref_t   tb_array_iterator_init_istr(tb_array_iterator_ref_t iterator, tb_char_t** items, tb_size_t count);

```

c-string元素的 初始化数组迭代器并忽略大小写

#### tb_array_iterator_init_long

```c
tb_iterator_ref_t   tb_array_iterator_init_long(tb_array_iterator_ref_t iterator, tb_long_t* items, tb_size_t count);

```

long元素的 初始化数组迭代器

#### tb_array_iterator_init_size

```c
tb_iterator_ref_t   tb_array_iterator_init_size(tb_array_iterator_ref_t iterator, tb_size_t* items, tb_size_t count);

```

size元素的 初始化数组迭代器

#### tb_array_iterator_init_istr

```c
tb_iterator_ref_t   tb_array_iterator_init_istr(tb_array_iterator_ref_t iterator, tb_char_t** items, tb_size_t count);

```

c-string元素的 初始化数组迭代器并忽略大小写

### list

 元素在内部维护的双向链表。

> - 内部维护：就是链表容器本身回去开辟一块空间，去单独存储元素内容，这种方式对接口的操作比较简单，但是灵活性和性能不如前一种，如果不需要多个链表维护同一种元素，那么使用这种模式简单操作下，更为妥当。（而且内部元素的存储也是用内存池优化过的）。

**存储方式：**

```c
typedef struct __tb_iterator_t
{
    /// the iterator mode
    tb_size_t               mode;

    /// the iterator step
    tb_size_t               step;

    /// the iterator priv
    tb_pointer_t            priv;

    /// the iterator operation
    tb_iterator_op_ref_t    op;

}tb_iterator_t, *tb_iterator_ref_t;
```

**使用场景：**

- **链表**

#### tb_list_init

```c
tb_list_ref_t       tb_list_init(tb_size_t grow, tb_element_t element);

```

初始化双链表。

- `grow`：设置为0表示采用默认的自动元素增长大小，也可以手动设置更适合的大小
- `element`：设置双链的类型

#### tb_list_exit

```c
tb_void_t           tb_list_exit(tb_list_ref_t list);

```

销毁list

#### tb_list_clear

```c
tb_void_t           tb_list_clear(tb_list_ref_t list);

```

 清除双链表

#### tb_list_head

```c
tb_pointer_t        tb_list_head(tb_list_ref_t list);

```

返回头结点

#### tb_list_last

```c
tb_pointer_t        tb_list_last(tb_list_ref_t list);

```

返回尾结点

#### tb_list_insert_prev

```c
tb_size_t           tb_list_insert_prev(tb_list_ref_t list, tb_size_t itor, tb_cpointer_t data);

```

插入上一项

#### tb_list_insert_next

```c
tb_size_t           tb_list_insert_next(tb_list_ref_t list, tb_size_t itor, tb_cpointer_t data);

```

在之前新的元素后面插入一个新元素

#### tb_list_insert_head

```c
tb_size_t           tb_list_insert_head(tb_list_ref_t list, tb_cpointer_t data);

```

在链表头部插入元素，并返回新元素的迭代器索引。

#### tb_list_insert_tail

```c
tb_size_t           tb_list_insert_tail(tb_list_ref_t list, tb_cpointer_t data);

```

链表尾部插入元素

#### tb_list_replace

```c
tb_void_t           tb_list_replace(tb_list_ref_t list, tb_size_t itor, tb_cpointer_t data);

```

替换该项

#### tb_list_replace_head

```c
tb_void_t           tb_list_replace_head(tb_list_ref_t list, tb_cpointer_t data);

```

替换头结点

#### tb_list_replace_last

```c
tb_void_t           tb_list_replace_last(tb_list_ref_t list, tb_cpointer_t data);

```

替换尾结点

#### tb_list_remove

```c
tb_size_t           tb_list_remove(tb_list_ref_t list, tb_size_t itor);

```

移除指定的元素

#### tb_list_remove_head

```c
tb_void_t           tb_list_remove_head(tb_list_ref_t list);

```

移除头结点

#### tb_list_remove_last

```c
tb_void_t           tb_list_remove_last(tb_list_ref_t list);

```

移除尾结点

#### tb_list_moveto_prev

```c
tb_void_t           tb_list_moveto_prev(tb_list_ref_t list, tb_size_t itor, tb_size_t move);

```

移到上一项

#### tb_list_moveto_next

```c
tb_void_t           tb_list_moveto_next(tb_list_ref_t list, tb_size_t itor, tb_size_t move);

```

移到下一项

#### tb_list_moveto_head


tb_void_t           tb_list_moveto_head(tb_list_ref_t list, tb_size_t move);
```


移到头结点

#### tb_list_moveto_tail

```c
tb_void_t           tb_list_moveto_tail(tb_list_ref_t list, tb_size_t move);

```

移到尾结点

#### tb_list_size

```c
tb_size_t           tb_list_size(tb_list_ref_t list);

```

返回结点数

#### tb_list_maxn

```c
tb_size_t           tb_list_maxn(tb_list_ref_t list);

```

返回结点最大容量

#### tb_for_all

tbox的遍历函数是抽象出来的。

```c
#define tb_for_all(type, item, iterator) \
            tb_iterator_ref_t item##_iterator_all = (tb_iterator_ref_t)iterator; \
            tb_for(type, item, tb_iterator_head(item##_iterator_all), tb_iterator_tail(item##_iterator_all), item##_iterator_all)
```

### list_entry

元素在外部维护的双向链表。

> 类似Linux的链表结构，链表容器本身不存储元素，不开辟内存空间，仅仅是一个结点头，这样比较节省内存，更加灵活。（尤其是在多个链表间元素迁移的时候，或者多个链表需要统一内存池维护的时候）。

**存储方式**

```c
/*! the doubly-linked list entry type
 *
 * <pre>
 * list: list => ... => last
 *        |               |
 *        <---------------
 *
 * </pre>
 */
typedef struct __tb_list_entry_t
{
    /// the next entry
    struct __tb_list_entry_t*   next;

    /// the prev entry
    struct __tb_list_entry_t*   prev;

}tb_list_entry_t, *tb_list_entry_ref_t;

/// the list entry head type
typedef struct __tb_list_entry_head_t
{
    /// the next entry
    struct __tb_list_entry_t*   next;

    /// the prev entry
    struct __tb_list_entry_t*   prev;

    /// the list size
    tb_size_t                   size;

    /// the iterator
    tb_iterator_t               itor;

    /// the entry offset
    tb_size_t                   eoff;

    /// the entry copy func
    tb_entry_copy_t             copy;

}tb_list_entry_head_t, *tb_list_entry_head_ref_t;
```



#### tb_list_entry_init

```c
// 初始化链表，需要指定外置元素的结构体类型，链表的结点名字
tb_list_entry_head_t list;
#define tb_list_entry_init(list, type, entry, copy)     tb_list_entry_init_(list, tb_offsetof(type, entry), sizeof(type), copy)
```

初始化链表，需要指定外置元素的结构体类型，链表的结点名字

**参数说明**

- list：指定链表
- type：外置元素的结构
- entry：链表的结点名字
- copy：the entry copy func

#### tb_list_entry_clear

```c
static __tb_inline__ tb_void_t              tb_list_entry_clear(tb_list_entry_head_ref_t list)

```

 清除双链表

#### tb_list_entry_head

```c
static __tb_inline__ tb_list_entry_ref_t    tb_list_entry_head(tb_list_entry_head_ref_t list)

```

返回头结点

#### tb_list_entry_last

```c
static __tb_inline__ tb_list_entry_ref_t    tb_list_entry_last(tb_list_entry_head_ref_t list)

```

返回尾结点

#### tb_list_entry_insert_prev

```c
static __tb_inline__ tb_void_t              tb_list_entry_insert_prev(tb_list_entry_head_ref_t list, tb_list_entry_ref_t node, tb_list_entry_ref_t entry)

```

插入上一项

#### tb_list_entry_insert_next

```c
static __tb_inline__ tb_void_t              tb_list_entry_insert_next(tb_list_entry_head_ref_t list, tb_list_entry_ref_t node, tb_list_entry_ref_t entry)

```

在之前新的元素后面插入一个新元素

#### tb_list_entry_insert_head

```c
static __tb_inline__ tb_void_t              tb_list_entry_insert_head(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)

```

在链表头部插入元素，并返回新元素的迭代器索引。

#### tb_list_entry_insert_tail

```c
static __tb_inline__ tb_void_t              tb_list_entry_insert_tail(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)

```

链表尾部插入元素

注意：所有操作都是在外置结构体中的list_entry结点上操作

#### tb_list_entry_replace

```c
static __tb_inline__ tb_void_t              tb_list_entry_replace(tb_list_entry_head_ref_t list, tb_list_entry_ref_t node, tb_list_entry_ref_t entry)

```

替换该项

#### tb_list_entry_replace_head

```c
static __tb_inline__ tb_void_t              tb_list_entry_replace_head(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)

```

替换头结点

#### tb_list_entry_replace_last

```c
static __tb_inline__ tb_void_t              tb_list_entry_replace_last(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)

```

替换尾结点

#### tb_list_entry_remove

```c
static __tb_inline__ tb_void_t              tb_list_entry_remove(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)

```

移除指定的元素

#### tb_list_entry_remove_next

#### tb_list_entry_remove_prev

#### tb_list_entry_remove_head

#### tb_list_entry_remove_last

```c
static __tb_inline__ tb_void_t              tb_list_entry_remove_prev(tb_list_entry_head_ref_t list, tb_list_entry_ref_t next)

static __tb_inline__ tb_void_t              tb_list_entry_remove_head(tb_list_entry_head_ref_t list)

static __tb_inline__ tb_void_t              tb_list_entry_remove_last(tb_list_entry_head_ref_t list)
```

移除元素

#### tb_list_entry_moveto_prev

```c
static __tb_inline__ tb_void_t              tb_list_entry_moveto_prev(tb_list_entry_head_ref_t list, tb_list_entry_ref_t node, tb_list_entry_ref_t entry)

```

移到上一项

#### tb_list_entry_moveto_next

```c
static __tb_inline__ tb_void_t              tb_list_entry_moveto_next(tb_list_entry_head_ref_t list, tb_list_entry_ref_t node, tb_list_entry_ref_t entry)

```

移到下一项

#### tb_list_entry_moveto_head

```c
static __tb_inline__ tb_void_t              tb_list_entry_moveto_head(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)

```


移到头结点

#### tb_list_entry_moveto_tail

```c
static __tb_inline__ tb_void_t              tb_list_entry_moveto_tail(tb_list_entry_head_ref_t list, tb_list_entry_ref_t entry)
```

移到尾结点

#### tb_list_entry

```c
#define tb_list_entry(head, entry)      ((((tb_byte_t*)(entry)) - (head)->eoff))
```

访问具体某个结点的元素数据

#### tb_for_all_if

```c
tb_for_all_if(type, item, iterator, cond)
```

如果条件符合，则使用迭代器遍历所有元素


### *hash_map*

**存储方式：**

```c
typedef struct __tb_hash_map_item_t
{
    /// the item name
    tb_pointer_t        name;

    /// the item data
    tb_pointer_t        data;

}tb_hash_map_item_t, *tb_hash_map_item_ref_t;

```

**使用场景：**

- **哈希表**

#### tb_hash_map_init

```c
tb_hash_map_ref_t       tb_hash_map_init(tb_size_t bucket_size, tb_element_t element_name, tb_element_t element_data);

```

初始化哈希表

#### tb_hash_map_exit

```c
tb_void_t               tb_hash_map_exit(tb_hash_map_ref_t hash_map);

```

退出哈希表

#### tb_hash_map_clear

```c
tb_void_t               tb_hash_map_clear(tb_hash_map_ref_t hash_map);

```

清理哈希表

#### tb_hash_map_get

```c
tb_pointer_t            tb_hash_map_get(tb_hash_map_ref_t hash_map, tb_cpointer_t name);

```

获取哈希表值

#### tb_hash_map_find

```c
tb_size_t               tb_hash_map_find(tb_hash_map_ref_t hash_map, tb_cpointer_t name);

```

通过名称查找项目

#### tb_hash_map_insert

```c
tb_size_t               tb_hash_map_insert(tb_hash_map_ref_t hash_map, tb_cpointer_t name, tb_cpointer_t data);

```

通过名称插入项目数据

#### tb_hash_map_remove

```c
tb_void_t               tb_hash_map_remove(tb_hash_map_ref_t hash_map, tb_cpointer_t name);

```

通过名称中删除项

#### tb_hash_map_size

```c
tb_size_t               tb_hash_map_size(tb_hash_map_ref_t hash_map);

```

返回容器容量

#### tb_hash_map_init

```c
tb_size_t               tb_hash_map_maxn(tb_hash_map_ref_t hash_map);

```

返回容器最大容量

## [object库](https://tboox.io/#/zh-cn/getting_started?id=object库)

由于包含函数过多，故结合分析报告，暂只调研以下内容

-  src\tbox\object\array.h
-  src\tbox\object\dictionary.h
-  src\tbox\object\string.h

### *array*

**存储方式：**

```c
typedef struct __tb_oc_array_t
{
    // the object base
    tb_object_t         base;

    // the vector
    tb_vector_ref_t     vector;

    // is increase refn?
    tb_bool_t           incr;

}tb_oc_array_t;
```
其中：

```c
typedef struct __tb_object_t
{
    /// the object flag
    tb_uint8_t                  flag;

    /// the object type
    tb_uint16_t                 type;

    /// the object reference count
    tb_size_t                   refn;

    /// the object private data
    tb_cpointer_t               priv;

    /// the copy func
    struct __tb_object_t*    (*copy)(struct __tb_object_t* object);

    /// the clear func
    tb_void_t                   (*clear)(struct __tb_object_t* object);

    /// the exit func
    tb_void_t                   (*exit)(struct __tb_object_t* object);

}tb_object_t, *tb_object_ref_t;


```

```c
typedef struct __tb_iterator_t
{
    /// the iterator mode
    tb_size_t               mode;

    /// the iterator step
    tb_size_t               step;

    /// the iterator priv
    tb_pointer_t            priv;

    /// the iterator operation
    tb_iterator_op_ref_t    op;

}tb_iterator_t, *tb_iterator_ref_t;
```

**使用场景：**

- **数组**

#### tb_oc_array_init

```c
tb_object_ref_t     tb_oc_array_init(tb_size_t grow, tb_bool_t incr);

```

初始化数组

#### tb_oc_array_init

```c
tb_object_ref_t     tb_oc_array_init(tb_size_t grow, tb_bool_t incr);

```

初始化数组

#### tb_oc_array_size

```c
tb_size_t           tb_oc_array_size(tb_object_ref_t array);

```

返回数组大小

#### tb_oc_array_item

```c
tb_object_ref_t     tb_oc_array_item(tb_object_ref_t array, tb_size_t index);

```

索引处的数组项

#### tb_oc_array_incr

```c
tb_void_t           tb_oc_array_incr(tb_object_ref_t array, tb_bool_t incr);

```

设置数组是否开启引用

#### tb_oc_array_itor

```c
tb_iterator_ref_t   tb_oc_array_itor(tb_object_ref_t array);

```

数组迭代器

#### tb_oc_array_remove

```c
tb_void_t           tb_oc_array_remove(tb_object_ref_t array, tb_size_t index);

```

按索引删除项

#### tb_oc_array_append

```c
tb_void_t           tb_oc_array_append(tb_object_ref_t array, tb_object_ref_t item);

```

将项附加到数组

#### tb_oc_array_insert

```c
tb_void_t           tb_oc_array_insert(tb_object_ref_t array, tb_size_t index, tb_object_ref_t item);

```

将项插入数组

#### tb_oc_array_replace

```c
tb_void_t           tb_oc_array_replace(tb_object_ref_t array, tb_size_t index, tb_object_ref_t item);

```

初始化数组

#### tb_oc_array_init

```c
tb_object_ref_t     tb_oc_array_init(tb_size_t grow, tb_bool_t incr);

```

将项替换为数组

### ***dictionary***

**存储方式：**

```c
typedef struct __tb_oc_dictionary_t
{
    // the object base
    tb_object_t         base;

    // the capacity size
    tb_size_t           size;

    // the object hash
    tb_hash_map_ref_t   hash;

    // increase refn?
    tb_bool_t           incr;

}tb_oc_dictionary_t;
```

其中：

```c
typedef struct __tb_oc_dictionary_item_t
{
    /// the key
    tb_char_t const*    key;

    /// the value
    tb_object_ref_t     val;

}tb_oc_dictionary_item_t;
```

**使用场景：**

- **字典**

#### tb_oc_dictionary_init

```c
tb_object_ref_t         tb_oc_dictionary_init(tb_size_t size, tb_bool_t incr);

```

初始化字典

#### tb_oc_dictionary_size

```c
tb_size_t               tb_oc_dictionary_size(tb_object_ref_t dictionary);

```

字典大小

#### tb_oc_dictionary_incr

```c
tb_void_t               tb_oc_dictionary_incr(tb_object_ref_t dictionary, tb_bool_t incr);

```

设置字典引用

#### tb_oc_dictionary_itor

```c
tb_iterator_ref_t       tb_oc_dictionary_itor(tb_object_ref_t dictionary);

```

字典迭代器

#### tb_oc_dictionary_value

```c
tb_object_ref_t         tb_oc_dictionary_value(tb_object_ref_t dictionary, tb_char_t const* key);

```

获取字典值

#### tb_oc_dictionary_insert

```c
tb_void_t               tb_oc_dictionary_insert(tb_object_ref_t dictionary, tb_char_t const* key, tb_object_ref_t val);

```

插入词典项

#### tb_oc_dictionary_remove

```c
tb_void_t               tb_oc_dictionary_remove(tb_object_ref_t dictionary, tb_char_t const* key);

```

删除词典项

### ***string***

**存储方式：**

```c
// the string type
typedef struct __tb_oc_string_t
{
    // the object base
    tb_object_t         base;

    // the string
    tb_string_t         str;

}tb_oc_string_t;
```



#### tb_oc_string_init_from_cstr

```c
tb_object_ref_t     tb_oc_string_init_from_cstr(tb_char_t const* cstr);

```

根据c字符串初始化字符串

#### tb_oc_string_init_from_str

```c
tb_object_ref_t     tb_oc_string_init_from_str(tb_string_ref_t str);

```

根据字符串初始化字符串

#### tb_oc_string_cstr

```c
tb_char_t const*    tb_oc_string_cstr(tb_object_ref_t string);

```

c-string

#### tb_oc_string_cstr_set

```c
tb_size_t           tb_oc_string_cstr_set(tb_object_ref_t string, tb_char_t const* cstr);

```

设置c-string

#### tb_oc_string_size

```c
tb_size_t           tb_oc_string_size(tb_object_ref_t string);

```

字符串大小



## 编码风格

- [聊聊tbox的代码风格](https://tboox.org/cn/2016/07/29/code-style/)

### 命名规则

tbox的命名规则偏unix化些，主要以`小写+下划线`为主。

### 缩进问题

全部使用4个空格的缩进，

### 类型问题

为了做到跨平台，所有类型，都是全部重定义过的，例如：

```c
typedef signed int                  tb_int_t;
typedef unsigned int                tb_uint_t;
typedef signed short                tb_short_t;
typedef unsigned short              tb_ushort_t;
typedef tb_int_t                    tb_bool_t;
typedef signed char                 tb_int8_t;
typedef tb_int8_t                   tb_sint8_t;
typedef unsigned char               tb_uint8_t;
typedef tb_short_t                  tb_int16_t;
typedef tb_int16_t                  tb_sint16_t;
typedef tb_ushort_t                 tb_uint16_t;
typedef tb_int_t                    tb_int32_t;
typedef tb_int32_t                  tb_sint32_t;
typedef tb_uint_t                   tb_uint32_t;
typedef char                        tb_char_t;
typedef tb_int32_t                  tb_uchar_t;
typedef tb_uint8_t                  tb_byte_t;
typedef void                        tb_void_t;
typedef tb_void_t*                  tb_pointer_t;
typedef tb_void_t const*            tb_cpointer_t;
typedef tb_pointer_t                tb_handle_t;

#define tb_true     ((tb_bool_t)1)
#define tb_false    ((tb_bool_t)0)
```

使用的原因：

```
主要还是为了解决平台统一问题，以及代码一致性。
```

### 接口设计

tbox里面的接口，也是基于对象式，但是跟c++的那种对象还有些区别的，这里一个对象类型，对应两个文件，这里拿对象`vector`为例：

```
vector.h
vector.c
```

一个是接口文件，一个是实现文件。

#### 接口文件

也看下接口文件中的接口定义方式：

```c
// the vector ref type
typedef struct{}*   tb_vector_ref_t;

/*!  初始化 vector
 * 
 * @code
 *
    //  初始化 vector
    tb_vector_ref_t vector = tb_vector_init(0, tb_element_str(tb_true));
    if (vector)
    {
        // insert elements into head
        tb_vector_insert_head(vector, "hi!");

        // insert elements into tail
        tb_vector_insert_tail(vector, "how");
        tb_vector_insert_tail(vector, "are");
        tb_vector_insert_tail(vector, "you");

        // dump elements
        tb_for_all (tb_char_t const*, cstr, vector)
        {
            // trace
            tb_trace_d("%s", cstr);
        }

        // exit vector
        tb_vector_exit(vector);
    }
 * @endcode
 *
 * @param grow      the item grow
 * @param element   the element
 *
 * @return          the vector
 */
tb_vector_ref_t     tb_vector_init(tb_size_t grow, tb_element_t element);

/*! exist vector
 *
 * @param vector    the vector
 */
tb_void_t           tb_vector_exit(tb_vector_ref_t vector);

/*! insert the vector tail item
 *
 * @param vector    the vector
 * @param data      the item data
 */
tb_void_t           tb_vector_insert_tail(tb_vector_ref_t vector, tb_cpointer_t data);
```

**在tbox里面，每个接口，都是有注释的，注释规范用的是doxygen的格式**，并且对于一些接口的，会加上如何调用的demo例子在注释中.



尤其需要说一下的是，这里的类型定义：

```c
typedef struct{}*   tb_vector_ref_t;
```

把struct的成员暴露在头文件中，很容易导致版本更新的时候，struct定义跟库接口不一致，产生一些莫名的问题。

定义成：

```
typedef void* tb_vector_ref_t;
```

这种方式，相当于去类型化了，把强类型削弱了，会导致一个vector对象，不小心传给`tb_list_remove`的接口，编译也不会报错。

#### 实现文件

接口实现中，我们才会去真正的定义这个vector对象的具体成员结构，例如：

```c
typedef struct __tb_vector_t
{
    // the data size
    tb_size_t   size;

    // the data buffer
    tb_byte_t*  data;

    // ...

}tb_vector_t;

// 这里第一个参数名为self，相当于c++的this，表示当前对象实例的引用
tb_void_t tb_vector_exit(tb_vector_ref_t self)
{
    // tbox的每个接口开头，都会加上assert进行断言检测
    // 在debug下第一时间报告出现的问题，release下会禁用，不影响性能
    tb_vector_t* vector = (tb_vector_t*)self;
    tb_assert_and_check_return(vector);

    // exit data
    if (vector->data) tb_free(data);
    vector->data = tb_null;
    vector->size = 0;
}
```

