# rbtree



## LCUI & Linux

### *rbtree*

LCUI 的红黑树的源码基于 Linux 的红黑树源码修改而来

**实现方式：**

```c
typedef struct RBTreeNodeRec_ RBTreeNode;

struct RBTreeNodeRec_ {
	unsigned char color;
	int key;
	union {
		void *data;
		char *str;
	};
	RBTreeNode *parent, *left, *right;
};

typedef struct RBTreeRec_ {
	int total_node;
	int(*compare)(void*, const void*);
	void(*destroy)(void*);
	RBTreeNode *root;
} RBTree;

```

**使用场景**

```c
void RBTree_Init(RBTree *rbt); //初始化红黑树  
void RBTree_Destroy(RBTree *rbt); //销毁红黑树         
RBTreeNode * RBTree_First(const RBTree *rbt); //获取第一个结点 
RBTreeNode * RBTree_Next(const RBTreeNode *node); //获取下一个结点     
RBTreeNode * RBTree_Search(RBTree* rbt, int key); //红黑树查找结点     
void*        RBTree_GetData(RBTree* rbt, int key); //红黑树查找结点并获取该 结点数据 
RBTreeNode * RBTree_Insert(RBTree *rbt, int key, void *data); //插入新的结点 
int  RBTree_Erase(RBTree *rbt, int key); //删除红黑树中的结点  
void RBTree_EraseNode(RBTree *rbt, RBTreeNode *node); //按结点删除红黑树 
int  RBTree_CustomErase(RBTree *rbt, const void *keydata); //按指定数据删除结点 
RBTreeNode * RBTree_CustomSearch(RBTree* rbt, const void *keydata); //红黑树按指定数据查找结点 
void*        RBTree_CustomGetData(RBTree* rbt, const void *keydata); //获取指定结点数据 
RBTreeNode * RBTree_CustomInsert(RBTree *rbt, const void *keydata, void *data); //按指定数据插入 
```

参考：

- [如何将 Linux 内核实现的红黑树 rbtree 运用到你的 C 程序中？ - Haippy - 博客园 (cnblogs.com)](https://www.cnblogs.com/haippy/archive/2012/09/02/2668099.html)

## Glib

### **[Balanced Binary Trees](https://developer.gnome.org/glib/stable/glib-Balanced-Binary-Trees.html)**

GTree 结构及其相关函数提供了按顺序搜索和遍历的优化键/值对的排序集合。这意味着 GTree 上的大多数操作(访问、搜索、插入、删除、 ...)的平均值为 `o(log (n))` ，最糟糕的情况是时间复杂度为` o(n)`。但是，请注意，维护一个平衡排序的 n 元素 GTree 是在时间 `o(nlog(n))`中完成的。

**实现方式：**

```c
typedef struct _GTree GTree;
typedef struct _GTreeNode GTreeNode;
/**
 * GTree:
 *
 * The GTree struct is an opaque data structure representing a
 * [balanced binary tree][glib-Balanced-Binary-Trees]. It should be
 * accessed only by using the following functions.
 */
struct _GTree
{
  GTreeNode        *root;
  GCompareDataFunc  key_compare;
  GDestroyNotify    key_destroy_func;
  GDestroyNotify    value_destroy_func;
  gpointer  key_compare_data;
  guint     nnodes;
  gint      ref_count;
};

struct _GTreeNode
{
  gpointer   key; /* key for this node */
  gpointer   value;       /* value stored at this node */
  GTreeNode *left;        /* left subtree */
  GTreeNode *right;       /* right subtree */
  gint8      balance;     /* height (right) - height (left) */
  guint8     left_child;
  guint8     right_child;
};

```



**使用场景**

- 使用 `g_tree_new ()`创建一个新的 GTree。
- 在 GTree 中插入键/值对，使用 `g_tree_insert ()`，开销未`(o (n log (n)))`,。
- 要删除一个键/值对，可以使用 `g_tree_remove ()`，(o (n log (n)))。
- 若要查找与给定键对应的值，请使用 `g_tree_lookup ()`和 `g_tree_lookup_extended ()`。
- 若要查找 GTree 中的结点数，请使用 `g_tree_nnodes ()`。若要获得 GTree 的高度，请使用 `g_tree_height ()`。
- 要遍历 GTree，请为遍历中访问过的每个结点调用一个函数，使用 `g_tree_foreach ()`。
- 要销毁一棵 GTree，使用 `g_tree_destroy ()` 。





## Ngnix 

`ngx_rbtree_t`（红黑树）是一种非常有效的数据结构，nginx中的核心模块（如定时器管理、文件缓存模块）需要进行快速检索的场合下都使用了`ngx_rbtree_t`。

### [ngx_rbtree](https://trac.nginx.org/nginx/browser/nginx/src/core/ngx_rbtree.h)

- src/core/ngx_rbtree

`ngx_rbtree`的结点`ngx_rbtree_node_t`结构跟一般的红黑树差不多，都是由键值key、左孩子left、右孩子right、父亲结点parent、颜色值color，**不同的是`ngx_rbtree_node_t`这里多了一个data，但根据官方文档记在，由于data只有一个字节，表示太少，很少使用到**。

ngx_rbtree_t的结构也与一般红黑树相同，右root结点和哨兵叶子结点（sentinel）组成，**不同的是这里多了一个 函数指针inserter，它决定了在添加结点是新加还是替换。**



**实现方式：**

```c
typedef ngx_uint_t  ngx_rbtree_key_t;
typedef ngx_int_t   ngx_rbtree_key_int_t;

/* 红黑树结点结构 */
typedef struct ngx_rbtree_node_s  ngx_rbtree_node_t;

struct ngx_rbtree_node_s {
    ngx_rbtree_key_t       key;     /* 结点的键值 */
    ngx_rbtree_node_t     *left;    /* 结点的左孩子 */
    ngx_rbtree_node_t     *right;   /* 结点的右孩子 */
    ngx_rbtree_node_t     *parent;  /* 结点的父亲 */
    u_char         color;   /* 结点的颜色 */
    u_char         data;    /* */
};

typedef struct ngx_rbtree_s  ngx_rbtree_t;

typedef void (*ngx_rbtree_insert_pt) (ngx_rbtree_node_t *root,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);

/* 红黑树结构 */
struct ngx_rbtree_s {
    ngx_rbtree_node_t     *root;    /* 指向树的根结点 */
    ngx_rbtree_node_t     *sentinel;/* 指向树的叶子结点NIL */
    ngx_rbtree_insert_pt   insert;  /* 添加元素结点的函数指针，解决具有相同键值问题，在初始化时需要指定*/

};
```

#### ngx_rbtree_init

```c
#define ngx_rbtree_init(tree, s, i) 
#define ngx_rbtree_sentinel_init(node)  ngx_rbt_black(node)
```

初始化

#### ngx_rbtree_left_rotate && ngx_rbtree_right_rotate

```c
ngx_rbtree_left_rotate(ngx_rbtree_node_t **root, ngx_rbtree_node_t *sentinel,     ngx_rbtree_node_t *node)
```

左旋  和 右旋

#### ngx_rbtree_insert

```c
//向红黑树中添加结点
void  
ngx_rbtree_insert(ngx_thread_volatile ngx_rbtree_t *tree,  
    ngx_rbtree_node_t *node) ;
//唯一值类型插入
void ngx_rbtree_insert_value(ngx_rbtree_node_t *root, ngx_rbtree_node_t *node,	    ngx_rbtree_node_t *sentinel);
//唯一时间类型的key 插入方法
void ngx_rbtree_insert_timer_value(ngx_rbtree_node_t *root,	    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);
```

ngx_rbtree添加插入结点

#### ngx_rbtree_delete

```c
void
ngx_rbtree_delete_delete(ngx_thread_volatile ngx_rbtree_t *tree,
     ngx_rbtree_node_t *node)
```

ngx_rbtree删除

#### ngx_rbtree_next

```c
	ngx_rbtree_node_t *ngx_rbtree_next(ngx_rbtree_t *tree,
61	    ngx_rbtree_node_t *node);
```

访问下一个结点

#### ngx_rbtree_min

```c
static ngx_inline ngx_rbtree_node_t *ngx_rbtree_min(ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
```

遍历：返回以node作为树根的这个红黑树中，最小的那个结点。

> ### 如何遍历整个红黑树？
>
> 先调用 ngx_rbtree_min，传入树根，就可以得到该红黑树的最小结点。
>
> 然后不断调用 ngx_rbtree_next，就可以遍历整颗红黑树。

#### 其他

```c
	#define ngx_rbt_red(node)               ((node)->color = 1)
	#define ngx_rbt_black(node)             ((node)->color = 0)
	#define ngx_rbt_is_red(node)            ((node)->color)
	#define ngx_rbt_is_black(node)          (!ngx_rbt_is_red(node))
	#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)
```



参考：

- [Nginx 红黑树结构 ngx_rbtree_t · 理解 Nginx 源码 · 看云 (kancloud.cn)](https://www.kancloud.cn/digest/understandingnginx/202594)
- [[Nginx数据结构之红黑树ngx_rbtree_t - 季末的天堂 - 博客园 (cnblogs.com)](https://www.cnblogs.com/jimodetiantang/p/8955303.html)]
- [[Nginx\] 红黑树 (mdgsf.github.io)](https://mdgsf.github.io/2017/08/08/nginx-rbtree/)

## 比较

……
