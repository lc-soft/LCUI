# array



## LCUI

### *LinkedList*

**实现方式：**

```c
struct LinkedListNodeRec_{
	void *data;
	LinkedListNode *prev, *next;
};

struct LinkedListRec_{
	size_t length;
	LinkedListNode head, tail;
};
```



## tbox

### *array*

**实现方式：**

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

索引处的数组元素

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

按索引删除元素

#### tb_oc_array_append

```c
tb_void_t           tb_oc_array_append(tb_object_ref_t array, tb_object_ref_t item);

```

将元素附加到数组

#### tb_oc_array_insert

```c
tb_void_t           tb_oc_array_insert(tb_object_ref_t array, tb_size_t index, tb_object_ref_t item);

```

将元素插入数组

#### tb_oc_array_replace

```c
tb_void_t           tb_oc_array_replace(tb_object_ref_t array, tb_size_t index, tb_object_ref_t item);

```

初始化数组

#### tb_oc_array_init

```c
tb_object_ref_t     tb_oc_array_init(tb_size_t grow, tb_bool_t incr);

```

将元素替换为数组

## Glib

数组类似于标准的 c 数组，只是它们会随着元素的添加而自动增长。

数组元素可以是任意大小(尽管一个数组的所有元素的大小相同) ，并且数组可以自动清除为’0’和零终止。

### Arrays

任意类型元素的可变长数组

**实现方式：**

```c
struct GArray {
  gchar *data;
  guint len;
};

struct _GRealArray
{
  guint8 *data;
  guint   len;
  guint   alloc;
  guint   elt_size;
  guint   zero_terminated : 1;
  guint   clear : 1;
  gatomicrefcount ref_count;
  GDestroyNotify clear_func;
};

```

其中：

| gchar * | 指向元素数据的指针。当元素被添加到 GArray 中时，数据可以被移动。 |      |
| ------- | ------------------------------------------------------------ | ---- |
| guint   | GArray 中的元素数，不包括可终止的零元素。                    |      |

**使用场景**

- 使用 `g_array_new ()`创建新数组。
- 为了在最坏的情况下向数组添加元素，成本为 `o(n)`，可以使用 `garray_append_val ()`、 `garray_append_vals()`、 `garray_prepend_val()`、 `garray_prepend_vals()`、 `garray_insert_val()`和 `garray_insert_vals()`。
- 若要访问中的数组元素(读或写)，成本为 `o(1) `请使用 `g_array_index()`。
- 若要设置数组的大小，请使用 `g_array_set_size()` 。
- 若要释放数组，请使用 `g_array_unref()`或 `g_array_free()` 。
- 所有的排序函数都在内部调用一个快速排序(或类似的)函数，平均开销为 `o(nlog(n))` ，最坏情况下开销为 `o(n^2)`。

## 比较和结论

> 调研与其他库的 array 实现方式，验证是否可以将 linkedlist 更名为 array。

不适合改名为 array 。

原因：

- array 一般使用变长数组实现，而 LCUI 中使用双链表实现。
- 一般而言，数组要求内存分配是连续的。

**结论：**
> 参照 tbox 中的 list 和 list_entry。
改名为 list 。