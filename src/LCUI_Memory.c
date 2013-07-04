/** ******************************************************************************
 * @file	LCUI_Memory.c
 * @brief	memory management.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	LCUI_Memory.c
 * @brief	内存管理.
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于						\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)				\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。	\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 * 用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define RED	0
#define BLACK	1

/* 内存的分类信息 */
typedef struct mem_info_t_ {
	unsigned int class_id;	/** 所属分类ID */
	char *class_name;	/** 指向分类名的指针 */
	size_t total_size;	/** 该分类的内存块的总大小 */
} mem_info_t;

/* 内存块的信息 */
typedef struct mem_block_t_ {
	unsigned int class_id;	/** 所属分类ID */
	void *mem_addr;		/** 该内存块的地址 */
	size_t mem_size;	/** 该内存块的大小 */
} mem_block_t;

typedef unsigned char rb_color_t;
typedef unsigned char rb_data_type_t;

enum rb_data_type {
	RB_DATA_TYPE_ADDR,
	RB_DATA_TYLE_INFO
};

typedef union mem_data_t_ {
	mem_block_t mem_blk;
	mem_info_t mem_info;
} mem_data_t;

typedef struct rb_node_t_ rb_node_t;
struct rb_node_t_ {
	rb_color_t color;
	mem_data_t mem_data;
	rb_node_t *parent, *left, *right;
};

typedef struct rb_tree_t_ {
	unsigned int total_node;
	rb_node_t *root;
} rb_tree_t;

rb_tree_t global_mem_data;
rb_tree_t global_mem_class_info;

static void rb_tree_init( rb_tree_t *rbt )
{
	rbt->root = NULL;
	rbt->total_node = 0;
}

/**
树的左旋

   node          right 
   / \	  ==>	 /  \
  a  right     node  y
 / \           / \	 
b   y         a   b
*/
static rb_node_t* rb_rotate_left( rb_node_t* node, rb_node_t* root )
{  
	rb_node_t* right = node->right;
   
	if ((node->right = right->left)) {  
		right->left->parent = node;
	}  
	right->left = node;
   
	if ((right->parent = node->parent)) {  
		if (node == node->parent->right) {  
			node->parent->right = right;  
		} else {
			node->parent->left = right;  
		}  
	} else {  
		root = right;  
	}  
	node->parent = right;
   
	return root;  
}  
  
  
/**
树的右旋  

   node          left
   /  \          /  \
 left  y   ==>	a   node 
 / \           / \
a   b         b   y 
*/
static rb_node_t* rb_rotate_right( rb_node_t* node, rb_node_t* root )
{  
	rb_node_t* left = node->left;  
   
	if ((node->left = left->right)) {  
		left->right->parent = node;  
	}  
	left->right = node;  
   
	if ((left->parent = node->parent)) {
		if (node == node->parent->right) {  
			node->parent->right = left;  
		} else {  
			node->parent->left = left;  
		}  
	} else {  
		root = left;  
	}  
	node->parent = left;  
   
	return root;  
}

static rb_node_t*
rb_insert_rebalance( rb_node_t *root, rb_node_t *node )
{
	rb_node_t *parent, *gparent, *uncle, *tmp;
   
	while ((parent = node->parent) && parent->color == RED) {
		gparent = parent->parent;
	
		if (parent == gparent->left) {  
			uncle = gparent->right;
			if (uncle && uncle->color == RED) {  
				uncle->color = BLACK;
				parent->color = BLACK;
				gparent->color = RED;	
				node = gparent;
			} else {
				if (parent->right == node) {
					root = rb_rotate_left(parent, root);
					tmp = parent;
					parent = node;
					node = tmp;
				}
				parent->color = BLACK;
				gparent->color = RED;
				root = rb_rotate_right(gparent, root);
			}  
		} else {
			uncle = gparent->left;
			if (uncle && uncle->color == RED) {
				uncle->color = BLACK;  
				parent->color = BLACK;  
				gparent->color = RED;  
				node = gparent;
			} else {
				if (parent->left == node) {
					root = rb_rotate_right(parent, root);
					tmp = parent;
					parent = node;
					node = tmp;
				}
				parent->color = BLACK;  
				gparent->color = RED;  
				root = rb_rotate_left(gparent, root);
			}  
		}
	}  
   
	root->color = BLACK;
	return root;
}

/** 红黑树查找结点 */
static rb_node_t* 
rb_search_auxiliary(	rb_node_t *root, mem_data_t *data,
			rb_data_type_t type, rb_node_t** save )
{  
	rb_node_t *node = root, *parent = NULL;

	if( type == RB_DATA_TYPE_ADDR ) {
		while( node ) {
			parent = node;
			if ( node->mem_data.mem_blk.mem_addr
			 > data->mem_blk.mem_addr ) {
				node = node->left;
			} else if ( node->mem_data.mem_blk.mem_addr
				< data->mem_blk.mem_addr ) {
				node = node->right;
			} else {
				return node;  
			}
		}
	} else {
		while( node ) {
			parent = node;
			if ( node->mem_data.mem_info.class_id
			 > data->mem_info.class_id ) {
				node = node->left;
			} else if ( node->mem_data.mem_info.class_id
				< data->mem_info.class_id ) {
				node = node->right;
			} else {
				return node;  
			}
		}
	}
	if( save ) { 
		*save = parent;  
	}
	return NULL;  
}

static rb_node_t*
rb_search( rb_node_t* root, mem_data_t *data, rb_data_type_t type )
{  
	return rb_search_auxiliary(root, data, type, NULL);
}

static int
rb_insert( rb_tree_t *rbt, mem_data_t *data, rb_data_type_t type )
{
	rb_node_t *root, *node, *parent_node;
	
	parent_node = NULL;
	root = rbt->root;
	node = rb_search_auxiliary( root, data, type, &parent_node );
	if( node ) {
		return -1;
	}

	node = (rb_node_t*)malloc( sizeof(rb_node_t) );
	node->left = NULL;
	node->parent = parent_node;
	node->right = NULL;
	node->mem_data = *data;
	node->color = RED;

	if( parent_node ) {
		if( type == RB_DATA_TYPE_ADDR ) {
			if( parent_node->mem_data.mem_blk.mem_addr
				> data->mem_blk.mem_addr ) {
				parent_node->left = node;
			} else {
				parent_node->right = node;
			}
		} else {
			if( parent_node->mem_data.mem_info.class_id
				 > data->mem_info.class_id ) {
				parent_node->left = node;
			} else {
				parent_node->right = node;
			}
		}
	} else {
		root = node;
	}
	/* 调整红黑树的平衡 */
	rbt->root = rb_insert_rebalance( root, node );
	return 0;
}  
  
/** 在树中的结点被删除后调整平衡 */
static rb_node_t*
rb_erase_rebalance( rb_node_t *node, rb_node_t *parent, rb_node_t *root )
{
	rb_node_t *other, *o_left, *o_right;
   
	while ((!node || node->color == BLACK) && node != root) {  
		if (parent->left == node) {  
			other = parent->right;
			if (other->color == RED) {  
				other->color = BLACK;	
				parent->color = RED;
				root = rb_rotate_left(parent, root);
				other = parent->right;
			}
			if ((!other->left || other->left->color == BLACK) 
			 && (!other->right || other->right->color == BLACK)) {
				other->color = RED;
				node = parent;
				parent = node->parent;
			} else {
				if (!other->right || other->right->color == BLACK) {  
					if ((o_left = other->left)) {
						o_left->color = BLACK;
					}   
					other->color = RED;
					root = rb_rotate_right(other, root);
					other = parent->right;
				}
				other->color = parent->color;
				parent->color = BLACK;
				if (other->right) {
					other->right->color = BLACK;
				}  
				root = rb_rotate_left(parent, root);
				node = root;
				break;  
			}  
		} else {
			other = parent->left;  
			if (other->color == RED) {  
				other->color = BLACK;  
				parent->color = RED;  
				root = rb_rotate_right(parent, root);  
				other = parent->left;  
			}  
			if ((!other->left || other->left->color == BLACK) &&  
				(!other->right || other->right->color == BLACK)) {  
				other->color = RED;  
				node = parent;  
				parent = node->parent;  
			} else {  
				if (!other->left || other->left->color == BLACK) {  
					if ((o_right = other->right)) {  
						o_right->color = BLACK;  
					}  
					other->color = RED;  
					root = rb_rotate_left(other, root);  
					other = parent->left;  
				}  
				other->color = parent->color;  
				parent->color = BLACK;  
				if (other->left) {
					other->left->color = BLACK;  
				}  
				root = rb_rotate_right(parent, root);  
				node = root;  
				break;  
			}  
		}  
	}  
   
	if( node ) {
		node->color = BLACK;
	}
	return root;
}


/** 删除红黑树中的结点 */
static int
rb_erase( rb_tree_t *rbt, mem_data_t *data, rb_data_type_t type )  
{
	rb_color_t color;
	rb_node_t *root, *child, *parent, *old, *left, *node; 

	root = rbt->root;
	/* 查找要删除的结点 */
	node = rb_search_auxiliary( root, data, type, NULL );
	if( !node ) {
		return -1;  
	}
	old = node;
	if (node->left && node->right) {
		node = node->right;  
		while ((left = node->left) != NULL) {  
			node = left;  
		}  
		child = node->right;  
		parent = node->parent;  
		color = node->color;  
	
		if (child) {  
			child->parent = parent;  
		}  
		if (parent) {  
			if (parent->left == node) {  
				parent->left = child;  
			} else {  
				parent->right = child;  
			}  
		} else {  
			root = child;  
		}  
	
		if (node->parent == old) {  
			parent = node;  
		}  
	
		node->parent = old->parent;  
		node->color = old->color;  
		node->right = old->right;  
		node->left = old->left;  
	
		if (old->parent) {  
			if (old->parent->left == old) {  
				old->parent->left = node;  
			} else {  
				old->parent->right = node;  
			}  
		} else {  
			root = node;  
		}  
	
		old->left->parent = node;  
		if (old->right) {  
			old->right->parent = node;  
		}  
	} else {  
		if (!node->left) {  
			child = node->right;  
		} else if (!node->right) {  
			child = node->left;  
		}  
		parent = node->parent;  
		color = node->color;  
	
		if (child) {  
			child->parent = parent;  
		}  
		if (parent) {  
			if (parent->left == node) {  
				parent->left = child;  
			} else {  
				parent->right = child;  
			}  
		} else {  
			root = child;  
		}  
	}
   
	free(old);  

	if (color == BLACK) {
		/* 恢复红黑树性质 */
		root = rb_erase_rebalance( child, parent, root );
	}

	rbt->root = root;
	rbt->total_node -= 1;
	return 0;  
}

/** 初始化LCUI的内存管理模块 */
void LCUIMM_Init(void)
{
	mem_data_t first_data;

	/* 初始化默认类别的数据 */
	first_data.mem_info.class_id = 0;
	first_data.mem_info.class_name = NULL;
	first_data.mem_info.total_size = 0;

	rb_tree_init( &global_mem_class_info );
	rb_tree_init( &global_mem_data );

	/* 插入默认类别的记录 */
	rb_insert( &global_mem_class_info, &first_data, RB_DATA_TYLE_INFO );
}

unsigned int LCUIMM_NewClass( char *class_name )
{
	rb_node_t *node;
	mem_data_t mem_data;

	mem_data.mem_info.class_id = BKDRHash(class_name);
	mem_data.mem_info.class_name = class_name;
	mem_data.mem_info.total_size = 0;
	node = rb_search( global_mem_class_info.root, 
			&mem_data, RB_DATA_TYLE_INFO );
	if( node == NULL ) {
		rb_insert(	&global_mem_class_info,
				&mem_data, RB_DATA_TYLE_INFO );
		global_mem_class_info.total_node += 1;
	} else {
		printf( "%s class is exist !", class_name );
	}
	return mem_data.mem_info.class_id;
}

void *LCUIMM_Alloc( size_t size, unsigned int class_id )
{
	rb_node_t *node;
	mem_data_t mem_data;

	mem_data.mem_info.class_id = class_id;
	/* 查找该类别的结点 */
	node = rb_search( global_mem_class_info.root,
			&mem_data, RB_DATA_TYLE_INFO );
	/* 如果不存在，则用默认类别的结点 */
	if( node == NULL ) {
		mem_data.mem_info.class_id = 0;
		node = rb_search( global_mem_class_info.root,
				&mem_data, RB_DATA_TYLE_INFO );
		/* 如果默认类别的结点不存在，则说明出问题了 */
		if( node == NULL ) {
			abort();
		}
	}
	/* 分配内存空间，并记录它 */
	mem_data.mem_blk.mem_addr = malloc( size );
	/* 分配失败则返回NULL */
	if( mem_data.mem_blk.mem_addr == NULL ) {
		return NULL;
	}
	mem_data.mem_blk.mem_size = size;
	/* 累计该类别的总内存空间大小 */
	node->mem_data.mem_info.total_size += size;
	/* 插入该内存的信息 */
	rb_insert( &global_mem_data, &mem_data, RB_DATA_TYPE_ADDR );
	return mem_data.mem_blk.mem_addr;
}

int LCUIMM_Free( void *mem_ptr )
{
	int size;
	rb_node_t *node;
	mem_data_t mem_data;
	
	mem_data.mem_blk.mem_addr = mem_ptr;
	/* 先查找该地址的内存块信息 */
	node = rb_search( global_mem_data.root,
			&mem_data, RB_DATA_TYPE_ADDR );
	if( node ) {
		/* 记录该内存块的类别ID，以及内存块的大小 */
		mem_data.mem_info.class_id = node->mem_data.mem_blk.class_id;
		size = node->mem_data.mem_blk.mem_size;
		/* 找到与类别ID对应的类别结点指针 */
		node = rb_search( global_mem_class_info.root, 
				&mem_data, RB_DATA_TYLE_INFO );
		if( node ) {
			/* 更新该类别内存块的总内存用量的大小 */
			node->mem_data.mem_info.total_size -= size;
		}
	} else {
		printf("%p can not be released !\n");
		return -1;
	}
	mem_data.mem_blk.mem_addr = mem_ptr;
	if( -1 == rb_erase( &global_mem_data, 
			&mem_data, RB_DATA_TYPE_ADDR ) ) {
		printf("%p can not be released !\n");
		return -1;
	}
	return 0;
}

size_t LCUIMM_GetMemSize( unsigned int class_id )
{
	rb_node_t *node;
	mem_data_t mem_data;

	mem_data.mem_info.class_id = class_id;
	/* 查找该类别的结点 */
	node = rb_search( global_mem_class_info.root, 
				&mem_data, RB_DATA_TYLE_INFO );
	/* 如果不存在，则用默认类别的结点 */
	if( node == NULL ) {
		return 0;
	}
	return node->mem_data.mem_info.total_size;
}

size_t LCUIMM_GetMemSizeByName( char *mem_class_name )
{
	return LCUIMM_GetMemSize( BKDRHash( mem_class_name ) );
}