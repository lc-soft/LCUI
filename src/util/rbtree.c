/* rbtree.c -- Red Black Trees
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <LCUI_Build.h>
#include <LCUI/util/rbtree.h>

#define RED     0
#define BLACK   1

/** 初始化红黑树 */
void RBTree_Init( RBTree *rbt )
{
	rbt->root = NULL;
	rbt->compare = NULL;
	rbt->destroy = NULL;
	rbt->total_node = 0;
}

static void DestroyRBTreeNode( RBTree *rbt, RBTreeNode *node )
{
	if( !node ) {
		return;
	}
	DestroyRBTreeNode( rbt, node->left );
	DestroyRBTreeNode( rbt, node->right );
	if( node->parent ) {
		if( node->parent->left == node ) {
			node->parent->left = NULL;
		} else {
			node->parent->right = NULL;
		}
	}
	if( rbt->destroy && node->data ) {
		rbt->destroy( node->data );
	}
	node->data = NULL;
	free( node );
}

/** 销毁红黑树 */
void RBTree_Destroy( RBTree *rbt )
{
	rbt->total_node = 0;
	DestroyRBTreeNode( rbt, rbt->root );
	rbt->root = NULL;
}

/** 获取第一个结点 */
RBTreeNode *RBTree_First( const RBTree *rbt )
{
	RBTreeNode *node;

	node = rbt->root;
	if( !node ) {
		return NULL;
	}
	while( node->left ) {
		node = node->left;
	}
	return node;
}

/** 获取下一个结点 */
RBTreeNode *RBTree_Next( const RBTreeNode *node )
{
	RBTreeNode *parent;

	if( node->parent == node ) {
		return NULL;
	}
	if( node->right ) {
		node = node->right;
		while( node->left ) {
			node = node->left;
		}
		return (RBTreeNode*)node;
	}
	while ((parent = node->parent) && node == parent->right ) {
		node = parent;
	}
	return parent;
}

/**
 * 树的左旋
 *
 *      node              right
 *      / \       ==>     /  \
 *     a  right         node  y
 *    / \              / \
 *   b   y            a   b
 */
static RBTreeNode*
rb_rotate_left( RBTreeNode* node, RBTreeNode* root )
{
	RBTreeNode* right = node->right;

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
 * 树的右旋
 *
 *        node           left
 *        /  \           /  \
 *      left  y   ==>   a   node
 *      / \            / \
 *     a   b          b   y
 */
static RBTreeNode*
rb_rotate_right( RBTreeNode* node, RBTreeNode* root )
{
	RBTreeNode* left = node->left;

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

static RBTreeNode*
rb_insert_rebalance( RBTreeNode *root, RBTreeNode *node )
{
	RBTreeNode *parent, *gparent, *uncle, *tmp;

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
static RBTreeNode*
rb_search_auxiliary( RBTreeNode *root, int key, const void *keydata,
		int (*cmp)(void*,const void*), RBTreeNode **save )
{
	int ret;
	RBTreeNode *node = root, *parent = NULL;

	while( node ) {
		parent = node;
		if( cmp && keydata ) {
			ret = cmp( node->data, keydata );
			if( ret > 0 ) {
				node = node->left;
			}
			else if( ret < 0 ) {
				node = node->right;
			} else {
				return node;
			}
			continue;
		}
		if ( node->key > key ) {
			node = node->left;
		} else if ( node->key < key ) {
			node = node->right;
		} else {
			return node;
		}
	}
	if( save ) {
		*save = parent;
	}
	return NULL;
}

RBTreeNode* RBTree_CustomSearch( RBTree* rbt, const void *keydata )
{
	return rb_search_auxiliary( rbt->root, 0, keydata, rbt->compare, NULL );
}

RBTreeNode* RBTree_Search( RBTree* rbt, int key )
{
	return rb_search_auxiliary( rbt->root, key, NULL, rbt->compare, NULL );
}

void* RBTree_CustomGetData( RBTree* rbt, const void *keydata )
{
	RBTreeNode *node;
	node = rb_search_auxiliary( rbt->root, 0, keydata, rbt->compare, NULL );
	if( node ) {
		return node->data;
	}
	return NULL;
}

void* RBTree_GetData( RBTree* rbt, int key )
{
	RBTreeNode *node;
	node = rb_search_auxiliary( rbt->root, key, NULL, rbt->compare, NULL );
	if( node ) {
		return node->data;
	}
	return NULL;
}

static RBTreeNode*
rb_insert( RBTree *rbt, int key, const void *keydata, void *data )
{
	RBTreeNode *root, *node, *parent_node;

	parent_node = NULL;
	root = rbt->root;
	node = rb_search_auxiliary( root, key, keydata,
				    rbt->compare, &parent_node );
	if( node ) {
		return NULL;
	}

	node = (RBTreeNode*)malloc( sizeof(RBTreeNode) );
	node->left = NULL;
	node->parent = parent_node;
	node->right = NULL;
	node->data = data;
	node->key = key;
	node->color = RED;

	if( parent_node ) {
		if( rbt->compare && keydata ) {
			if( rbt->compare( parent_node->data, keydata ) > 0 ) {
				parent_node->left = node;
			} else {
				parent_node->right = node;
			}
		} else {
			if( parent_node->key > key ) {
				parent_node->left = node;
			} else {
				parent_node->right = node;
			}
		}
	} else {
		root = node;
	}
	rbt->total_node += 1;
	/* 调整红黑树的平衡 */
	rbt->root = rb_insert_rebalance( root, node );
	return node;
}

RBTreeNode* RBTree_Insert( RBTree *rbt, int key, void *data )
{
	return rb_insert( rbt, key, NULL, data );
}

RBTreeNode* RBTree_CustomInsert( RBTree *rbt, const void *keydata, void *data )
{
	return rb_insert( rbt, 0, keydata, data );
}

/** 在树中的结点被删除后调整平衡 */
static RBTreeNode*
rb_erase_rebalance( RBTreeNode *node, RBTreeNode *parent,
		    RBTreeNode *root )
{
	RBTreeNode *other, *o_left, *o_right;

	while( (!node || node->color == BLACK) && node != root ) {
		if( parent->left == node ) {
			other = parent->right;
			if( other->color == RED ) {
				other->color = BLACK;
				parent->color = RED;
				root = rb_rotate_left( parent, root );
				other = parent->right;
			}
			if( (!other->left || other->left->color == BLACK)
			    && (!other->right || other->right->color == BLACK) ) {
				other->color = RED;
				node = parent;
				parent = node->parent;
				continue;
			}
			if( !other->right || other->right->color == BLACK ) {
				if( (o_left = other->left) ) {
					o_left->color = BLACK;
				}
				other->color = RED;
				root = rb_rotate_right( other, root );
				other = parent->right;
			}
			other->color = parent->color;
			parent->color = BLACK;
			if( other->right ) {
				other->right->color = BLACK;
			}
			root = rb_rotate_left( parent, root );
			node = root;
			break;
		}

		other = parent->left;
		if( other->color == RED ) {
			other->color = BLACK;
			parent->color = RED;
			root = rb_rotate_right( parent, root );
			other = parent->left;
		}
		if( (!other->left || other->left->color == BLACK)
		    && (!other->right || other->right->color == BLACK) ) {
			other->color = RED;
			node = parent;
			parent = node->parent;
			continue;
		}

		if( !other->left || other->left->color == BLACK ) {
			if( (o_right = other->right) ) {
				o_right->color = BLACK;
			}
			other->color = RED;
			root = rb_rotate_left( other, root );
			other = parent->left;
		}
		other->color = parent->color;
		parent->color = BLACK;
		if( other->left ) {
			other->left->color = BLACK;
		}
		root = rb_rotate_right( parent, root );
		node = root;
		break;
	}

	if( node ) {
		node->color = BLACK;
	}
	return root;
}

static void rb_erase_by_node( RBTree *rbt, RBTreeNode *node )
{
	unsigned char color;
	RBTreeNode *parent;
	RBTreeNode *old = node;
	RBTreeNode *child = NULL;
	RBTreeNode *root = rbt->root;
	if( node->left && node->right ) {
		RBTreeNode *left;
		node = node->right;
		while( (left = node->left) != NULL ) {
			node = left;
		}
		child = node->right;
		parent = node->parent;
		color = node->color;

		if( child ) {
			child->parent = parent;
		}
		if( parent ) {
			if( parent->left == node ) {
				parent->left = child;
			} else {
				parent->right = child;
			}
		} else {
			root = child;
		}

		if( node->parent == old ) {
			parent = node;
		}

		node->parent = old->parent;
		node->color = old->color;
		node->right = old->right;
		node->left = old->left;

		if( old->parent ) {
			if( old->parent->left == old ) {
				old->parent->left = node;
			} else {
				old->parent->right = node;
			}
		} else {
			root = node;
		}

		old->left->parent = node;
		if( old->right ) {
			old->right->parent = node;
		}
	} else {
		if( !node->left ) {
			child = node->right;
		} else if( !node->right ) {
			child = node->left;
		}
		parent = node->parent;
		color = node->color;

		if( child ) {
			child->parent = parent;
		}
		if( parent ) {
			if( parent->left == node ) {
				parent->left = child;
			} else {
				parent->right = child;
			}
		} else {
			root = child;
		}
	}

	if( rbt->destroy && old->data ) {
		rbt->destroy( old->data );
	}
	free( old );
	if( color == BLACK ) {
		/* 恢复红黑树性质 */
		root = rb_erase_rebalance( child, parent, root );
	}
	rbt->root = root;
	rbt->total_node -= 1;
}

/** 删除红黑树中的结点 */
static int rb_erase( RBTree *rbt, int key, const void *keydata )
{
	RBTreeNode *node;
	/* 查找要删除的结点 */
	node = rb_search_auxiliary( rbt->root, key, keydata, 
				    rbt->compare, NULL );
	if( !node ) {
		return -1;
	}
	rb_erase_by_node( rbt, node );
	return 0;
}

int RBTree_Erase( RBTree *rbt, int key )
{
	return rb_erase( rbt, key, NULL );
}

void RBTree_EraseNode( RBTree *rbt, RBTreeNode *node )
{
	rb_erase_by_node( rbt, node );
}

int RBTree_CustomErase( RBTree *rbt, const void *keydata )
{
	return rb_erase( rbt, 0, keydata );
}
