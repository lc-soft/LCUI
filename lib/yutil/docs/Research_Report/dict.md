# dict

字典的主要用途有以下两个：

1. 实现数据库键空间（key space）；
2. 用作 Hash 类型键的底层实现之一；

## LCUI & redis 

### dict

哈希表

实现方式：

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



**具体使用场景**见`RA/dict.md`

```c
Dict_Create //创建一个新字典
Dict_Expand //扩展字典大小
Dict_Add //将元素添加到目标哈希表中
Dict_Addcop //将元素添加到目标哈希表中， 功能与 `Dict_Add()` 相同，但必须指定 `valDup` 才能添加成功
Dict Addraw //添加元素的底层实现函数(由 `Dict_Add` 调用)
Dict_Replace //获将新元素添加到字典，如果 `key` 已经存在，那么新元素覆盖旧元素。
Dict_Replaceraw//添加元素的底层实现函数，获将新元素添加到字典，如果 `key` 已经存在，那么新元素覆盖旧元素。
Dict_Deletenofree//删除元素(不释放内存资源)
Dict_Release//删除字典，释放内存资源
Dict_Find//在字典中按指定的 key 查找
Dict_Fetchvalue//查找给定 key 在字典 d 中的值
Dict_Resize//重新调整字典的大小，缩减多余空间
Dict_Getlterator// 创建一个迭代器，用于遍历哈希表结点。
Dict_Getsafelterator// 创建一个迭代器，用于遍历哈希表结点。
Dict_Next//迭代器的推进函数。
Dict_Releaselterator//删除迭代器
Dict_Getrandomkey//从字典中随机获取一项
Dict_Printstats//打印字典的统计信息
Dict_Genhashfunction//散列函数*(a popular one from Bernstein).*
Dict_Genhashfunction//不区分大小写的版本的散列函数
Dict _nthashfunction//整形散列函数*(a popular one from Bernstein).*
Dict_dentityhashfunction//以整数为键的散列函数*(a popular one from Bernstein).*
Dict_Empty//清空字典
Dict_Enableresize//设置字典可以重新调整大小
Dict_Disable Resize//设置字典是否可以重新调整大小
Dict_Rehash//字典的 rehash 函数
Dict_Rehashmilliseconds//在指定的时间内完成 rehash 操作
Dict_SethashfunctionSeed//设置 Hash Seed 
Dict_Gethashfunctionseed//获取 Hash Seed
Stringkeydict_keyhash//获取String类型key的hash值
Stringkey Dict_KeyCompare//判断两个键是否冲突
Stringkeydict_keydup//用新建覆盖掉原key值
Stringkeydict_keydestructor//销毁该键值
Dict_initstringkeytype//初始化字典类型
Dict_initstring CopykeyType//初始化复制字典类型
```

**参考：**

- [Redis内部数据结构详解之字典(dict)_fmt.Println(-CSDN博客](https://blog.csdn.net/acceptedxukai/article/details/17484431)
## Tbox

### *dictionary*

**实现方式：**

```c
/// the dictionary item type
typedef struct __tb_oc_dictionary_item_t
{
    /// the key
    tb_char_t const*    key;

    /// the value
    tb_object_ref_t     val;

}tb_oc_dictionary_item_t;

// the dictionary type
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

**使用场景**

- 要创建 dictionary，使用 `tb_oc_dictionary_init() `。
- 返回字典大小，使用`tb_oc_dictionary_size()`
- 设置引用，使用`tb_oc_dictionary_incr()`
- 创建迭代器，使用`tb_oc_dictionary_itor();`
- 插入键和值，使用 `tb_oc_dictionary_insert()` 。
- 若要查找与给定键对应的值，使用 `tb_oc_dictionary_value ()`。
- 若要删除键和值，使用 `tb_oc_dictionary_remove () `。

## Glib

### [Hash Tables](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html)

`GHashTable` 提供键和值之间的关联，这些键和值经过优化后可以在分摊 o (1)中找到、插入或删除关联值。遍历每个元素的所有操作都需要 o (n)时间(列出所有键/值、表调整大小等)。

**实现方式：**

```c
struct _GHashTable
{
  gsize            size;
  gint             mod;
  guint            mask;
  gint             nnodes;
  gint             noccupied;  /* nnodes + tombstones */

  guint            have_big_keys : 1;
  guint            have_big_values : 1;

  gpointer         keys;
  guint           *hashes;
  gpointer         values;

  GHashFunc        hash_func;
  GEqualFunc       key_equal_func;
  gatomicrefcount  ref_count;
#ifndef G_DISABLE_ASSERT
  /*
   * Tracks the structure of the hash table, not its contents: is only
   * incremented when a node is added or removed (is not incremented
   * when the key or data of a node is modified).
   */
  int              version;
#endif
  GDestroyNotify   key_destroy_func;
  GDestroyNotify   value_destroy_func;
};

typedef struct
{
  GHashTable  *hash_table;
  gpointer     dummy1;
  gpointer     dummy2;
  gint         position;
  gboolean     dummy3;
  gint         version;
} RealIter;


typedef struct _GHashTable  GHashTable;

typedef gboolean  (*GHRFunc)  (gpointer  key,
                               gpointer  value,
                               gpointer  user_data);

typedef struct _GHashTableIter GHashTableIter;

struct _GHashTableIter
{
  /*< private >*/
  gpointer      dummy1;
  gpointer      dummy2;
  gpointer      dummy3;
  int           dummy4;
  gboolean      dummy5;
  gpointer      dummy6;
};

```

**使用场景**

- 要创建 `GHashTable`，使用 `g_hash_table_new ()` 。
- 若要向 `GHashTable` 中插入键和值，使用 `g_hash_table_insert ()` 。
- 若要查找与给定键对应的值，使用 `g_hash_table_lookup ()`和 `g_hash_table_lookup_extended ()`。
- 还可以使用 `g_ hash_table_lookup_extended ()`简单地检查哈希表中是否存在键。
- 若要删除键和值，使用 `g_hash_table_remove () `。
- 要为每个键和值对调用函数，可以使用 `g_hash_table_foreach ()`或使用迭代器迭代哈希表中的键/值对，请参见 `GHashTableIter`。没有定义哈希表的迭代顺序，您不能依赖于按照插入键/值的相同顺序对它们进行迭代。
- 使用 `ghash_table_destroy ()`来销毁一个 `GHashTable`。
- 哈希表的一个常见用例是存储关于一组键的信息，而不将任何特定值与每个键关联。`GHashTable` 优化了这样做的一种方法: 如果您只存储 `key == value` 的键值对，那么 `GHashTable` 不会分配内存来存储这些值，如果您的集合很大，这可以节省相当多的空间。函数 `g_hash_table_add ()`和 `g_hash_table_contains ()`被设计成这样使用 `GHashTable`。

## 结论

先将命名风格改为面向对象命名风格

```c
//新的接口设计

#define dictionary_hash_key(d, key) (d)->type->hashFunction(key)
#define dictionary_item_get_key(he) ((he)->key)
#define dictionary_item_get_val(he) ((he)->v.val)
#define dictionary_item_get_signed_integer_val(he) ((he)->v.s64)
#define dictionary_item_get_unsigned_integer_val(he) ((he)->v.u64)
#define dictionary_slots(d) ((d)->ht[0].size+(d)->ht[1].size)
#define dictionary_size(d) ((d)->ht[0].used+(d)->ht[1].used)
#define dictionary_is_rehashing(ht) ((ht)->rehashidx != -1)

dictionary_t *dictionary_create(dictionary_type_t *type, void *privdata);

int dictionary_expand(dictionary_t *dictionary, unsigned long size);

int dictionary_add(dictionary_t *dictionary, void *key, void *val);

int dictionary_add_copy(dictionary_t *dictionary, void *key, const void *val);

dictionary_item_t *dictionary_add_raw(dictionary_t *dictionary, void *key);

int dictionary_replace(dictionary_t *dictionary, void *key, void *val);
dictionary_item_t *dictionary_replace_raw(dictionary_t *dictionary, void *key);
int dictionary_delete(dictionary_t *dictionary, const void *key);

int dictionary_delete_no_free(dictionary_t *dictionary, const void *key);

void dictionary_release(dictionary_t *dictionary);

dictionary_item_t * dictionary_find(dictionary_t *dictionary, const void *key);

void *dictionary_fetch_value(dictionary_t *dictionary, const void *key);

int dictionary_resize(dictionary_t *dictionary);

dictionary_iterator_t *dictionary_get_iterator(dictionary_t *dictionary);

dictionary_iterator_t *dictionary_get_safe_iterator(dictionary_t *dictionary);

dictionary_item_t *dictionary_next(dictionary_iterator_t *iter);

void dictionary_release_iterator(dictionary_iterator_t *iter);

dictionary_item_t *dictionary_get_random_key(dictionary_t *dictionary);

void dictionary_print_stats(dictionary_t *dictionary);

unsigned int dictionary_gen_hash(const unsigned char *buf, int len);
unsigned int dictionary_gen_case_hash(const unsigned char *buf, int len);
unsigned int dictionary_gen_int_hash(unsigned int key);

unsigned int dictionary_identity_hash(unsigned int key);
void dictionary_empty(dictionary_t *dictionary);
void dictionary_enable_resize(void);
void dictionary_disable_resize(void);
int dictionary_rehash(dictionary_t *dictionary, int num);
int dictionary_rehash_milliseconds(dictionary_t *dictionary, int ms);
void dictionary_set_hash_seed(unsigned int initval);
unsigned int dictionary_get_hash_seed(void);

unsigned int string_key_dictionary_key_hash(const void *key);
int string_key_dictionary_key_compare(void *privdata, const void *key1,
				      const void *key2);
void *string_key_dictionary_key_dup(void *privdata, const void *key);
void string_key_dictionary_key_destructor(void *privdata, void *key);

void dictionary_init_string_key_type(dictionary_type_t *type);
void dictionary_init_string_copy_key_type(dictionary_type_t *type);
```

