/*
 * rbtree.c -- red-black tree
 *
 * Copyright (c) 2021, Li Zihao <yidianyiko@foxmail.com> All rights reserved.
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
#include "yutil/keywords.h"
#include "yutil/rbtree.h"

#define rbtree_red(node) ((node)->color = 1)
#define rbtree_black(node) ((node)->color = 0)

/** 红黑树查找结点 */
static rbtree_node_t *rbtree_search_(rbtree_t *tree, int key,
				     const void *keydata, rbtree_node_t **save)
{
	int ret = 0;
	rbtree_node_t *node = tree->root;
	rbtree_node_t *temp = NULL;
	while (node) {
		temp = node;
		ret = (keydata && tree->compare)
			  ? tree->compare(node->data, keydata)
			  : (node->key - key);
		if (ret == 0) {
			return node;
		}
		node = (ret < 0) ? node->right : node->left;
	}
	if (save) {
		*save = temp;
	}
	return NULL;
}

// repalce old_node in the tree by new_node
static inline void rbtree_replace_(rbtree_t *tree, rbtree_node_t *old_node,
				   rbtree_node_t *new_node)
{
	new_node->parent = old_node->parent;

	if (old_node == tree->root) {
		tree->root = new_node;
	} else if (old_node == old_node->parent->left) {
		old_node->parent->left = new_node;
	} else {
		old_node->parent->right = new_node;
	}

	old_node->parent = new_node;
}

/**
 * left rotate
 *
 *      node              temp
 *      / \       ==>     /  \
 *     a  temp         node  y
 *    / \              / \
 *   b   y            a   b
 */
static inline void rbtree_left_rotate(rbtree_t *tree, rbtree_node_t *node)
{
	rbtree_node_t *temp = node->right;

	node->right = temp->left;
	if (temp->left) {
		temp->left->parent = node;
	}

	rbtree_replace_(tree, node, temp);
	temp->left = node;
}

/**
 * right rotate
 *
 *        node           temp
 *        /  \           /  \
 *      temp  y   ==>   a   node
 *      / \            / \
 *     a   b          b   y
 */
static inline void rbtree_right_rotate(rbtree_t *tree, rbtree_node_t *node)
{
	rbtree_node_t *temp = node->left;

	node->left = temp->right;
	if (temp->right) {
		temp->right->parent = node;
	}

	rbtree_replace_(tree, node, temp);
	temp->right = node;
}

static void rbtree_insert_rebalance(rbtree_t *tree, rbtree_node_t *node)
{
	rbtree_node_t *gparent;
	rbtree_node_t *uncle;
	while (node != tree->root &&
	       (node->parent && rbtree_is_red(node->parent))) {
		gparent = node->parent->parent;

		if (node->parent == gparent->left) {
			uncle = gparent->right;
			// case 1: uncle is red, then update node to gparent
			// case 2: uncle is balck or NIL
			if (uncle && rbtree_is_red(uncle)) {
				rbtree_black(node->parent);
				rbtree_black(uncle);
				rbtree_red(gparent);
				node = gparent;

			} else {
				if (node->parent && node->parent->left &&
				    node == node->parent->right) {
					node = node->parent;
					rbtree_left_rotate(tree, node);
					gparent = node->parent->parent;
				}
				// case 2 -> case 1
				rbtree_black(node->parent);
				rbtree_red(gparent);
				rbtree_right_rotate(tree, gparent);
			}
		} else {
			uncle = node->parent->parent->left;

			if (uncle && rbtree_is_red(uncle)) {
				rbtree_black(node->parent);
				rbtree_black(uncle);
				rbtree_red(gparent);
				node = gparent;
			} else {
				if (node->parent && node->parent->left &&
				    node == node->parent->left) {
					node = node->parent;
					rbtree_right_rotate(tree, node);
					gparent = node->parent->parent;
				}
				if (node->parent) {
					rbtree_black(node->parent);
				}
				rbtree_red(gparent);
				rbtree_left_rotate(tree, gparent);
			}
		}
	}

	rbtree_black(tree->root);
}

static void rbtree_delete_rebalance(rbtree_t *tree, rbtree_node_t *node,
				    rbtree_node_t *parent)
{
	rbtree_node_t *brother, *brother_left, *brother_right;

	while ((!node || rbtree_is_black(node)) && node != tree->root) {
		if (parent->left == node) {
			brother = parent->right;
			if (rbtree_is_red(brother)) {
				rbtree_black(brother);
				rbtree_red(parent);
				rbtree_left_rotate(tree, parent);
				brother = parent->right;
			}
			if ((!brother->left ||
			     rbtree_is_black(brother->left)) &&
			    (!brother->right ||
			     rbtree_is_black(brother->right))) {
				rbtree_red(brother);
				node = parent;
				parent = node->parent;
				continue;
			}
			if (!brother->right ||
			    rbtree_is_black(brother->right)) {
				if ((brother_left = brother->left)) {
					rbtree_black(brother_left);
				}
				rbtree_red(brother);
				rbtree_right_rotate(tree, brother);
				brother = parent->right;
			}
			brother->color = parent->color;
			rbtree_black(parent);
			if (brother->right) {
				rbtree_black(brother->right);
			}
			rbtree_left_rotate(tree, parent);
			node = tree->root;
			break;
		}

		brother = parent->left;
		if (rbtree_is_red(brother)) {
			rbtree_black(brother);
			rbtree_red(parent);
			rbtree_right_rotate(tree, parent);
			brother = parent->left;
		}
		if ((!brother->left || rbtree_is_black(brother->left)) &&
		    (!brother->right || rbtree_is_black(brother->right))) {
			rbtree_red(brother);
			node = parent;
			parent = node->parent;
			continue;
		}

		if (!brother->left || rbtree_is_black(brother->left)) {
			if ((brother_right = brother->right)) {
				rbtree_black(brother_right);
			}
			rbtree_red(brother);
			rbtree_left_rotate(tree, brother);
			brother = parent->left;
		}
		brother->color = parent->color;
		rbtree_black(parent);
		if (brother->left) {
			rbtree_black(brother->left);
		}
		rbtree_right_rotate(tree, parent);
		node = tree->root;
		break;
	}

	if (node) {
		rbtree_black(node);
	}
	return;
}

static void rbtree_node_destroy(rbtree_t *tree, rbtree_node_t *node)
{
	if (!node) {
		return;
	}
	rbtree_node_destroy(tree, node->left);
	rbtree_node_destroy(tree, node->right);
	if (node->parent) {
		if (node == node->parent->left) {
			node->parent->left = NULL;
		} else {
			node->parent->right = NULL;
		}
	}
	if (tree->destroy && node->data) {
		tree->destroy(node->data);
	}
	node->data = NULL;
	free(node);
}

// init red-black tree
void rbtree_init(rbtree_t *tree)
{
	tree->root = NULL;
	tree->compare = NULL;
	tree->destroy = NULL;
	tree->total_node = 0;
}

void rbtree_destroy(rbtree_t *tree)
{
	rbtree_node_destroy(tree, tree->root);
	tree->root = NULL;
	tree->total_node = 0;
}

void rbtree_insert(rbtree_t *tree, int key, const void *keydata, void *data)
{
	int ret = 0;
	rbtree_node_t *node_parent = NULL;
	rbtree_node_t *node = rbtree_search_(tree, key, keydata, &node_parent);
	if (node) {
		return;
	}
	node = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
	if (!node) {
		return;
	}
	node->left = NULL;
	node->right = NULL;
	node->parent = node_parent;
	node->data = data;
	node->key = key;

	if (!node_parent) {
		rbtree_black(node);
		tree->root = node;
		tree->total_node++;
		return;
	} else {
		tree->total_node++;
		rbtree_red(node);
		ret = (tree->compare && keydata)
			  ? tree->compare(node_parent->data, keydata)
			  : node_parent->key - key;
		if (ret > 0) {
			node_parent->left = node;
		} else {
			node_parent->right = node;
		}
	}

	/* re-balance tree */
	rbtree_insert_rebalance(tree, node);
}

void rbtree_insert_by_key(rbtree_t *tree, int key, void *data)
{
	rbtree_insert(tree, key, NULL, data);
}

void rbtree_insert_by_keydata(rbtree_t *tree, const void *keydata, void *data)
{
	if (!keydata) {
		return;
	}
	rbtree_insert(tree, 0, keydata, data);
}

void rbtree_delete_by_node(rbtree_t *tree, rbtree_node_t *node)
{
	unsigned char is_red;
	rbtree_node_t *parent = NULL;
	rbtree_node_t *temp = NULL;
	rbtree_node_t *old = node;

	if (node->left && node->right) {
		node = rbtree_get_min(node->right);
		temp = node->right;
		parent = node->parent;
		is_red = node->color;

		if (temp) {
			temp->parent = parent;
		}
		if (parent) {
			if (parent->left == node) {
				parent->left = temp;
			} else {
				parent->right = temp;
			}
		} else {
			tree->root = temp;
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
			tree->root = node;
		}

		old->left->parent = node;
		if (old->right) {
			old->right->parent = node;
		}
	} else {
		if (!node->left) {
			temp = node->right;
		} else if (!node->right) {
			temp = node->left;
		}
		parent = node->parent;
		is_red = node->color;

		if (temp) {
			temp->parent = parent;
		}
		if (parent) {
			if (parent->left == node) {
				parent->left = temp;
			} else {
				parent->right = temp;
			}
		} else {
			tree->root = temp;
		}
	}

	if (tree->destroy && old->data) {
		tree->destroy(old->data);
	}
	free(old);
	tree->total_node--;
	if (is_red) {
		return;
	}

	rbtree_delete_rebalance(tree, temp, parent);
}

/** 删除红黑树中的结点 */
int rbtree_delete(rbtree_t *tree, int key, const void *keydata)
{
	rbtree_node_t *node;
	node = rbtree_search_(tree, key, keydata, NULL);
	if (!node) {
		return -1;
	}
	rbtree_delete_by_node(tree, node);
	return 0;
}

int rbtree_delete_by_key(rbtree_t *tree, int key)
{
	return rbtree_delete(tree, key, NULL);
}

int rbtree_delete_by_keydata(rbtree_t *tree, const void *keydata)
{
	return rbtree_delete(tree, 0, keydata);
}

rbtree_node_t *rbtree_search_by_key(rbtree_t *tree, int key)
{
	return rbtree_search_(tree, key, NULL, NULL);
}
rbtree_node_t *rbtree_search_by_data(rbtree_t *tree, const void *keydata)
{
	return rbtree_search_(tree, 0, keydata, NULL);
}

void *rbtree_get_data_by_keydata(rbtree_t *tree, const void *keydata)
{
	rbtree_node_t *node;
	node = rbtree_search_(tree, 0, keydata, NULL);
	return (node) ? node->data : NULL;
}

void *rbtree_get_data_by_key(rbtree_t *tree, int key)
{
	rbtree_node_t *node;
	node = rbtree_search_(tree, key, NULL, NULL);
	return (node) ? node->data : NULL;
}

rbtree_node_t *rbtree_next(const rbtree_node_t *node)
{
	rbtree_node_t *parent = node->parent;

	if (parent == node) {
		return NULL;
	}
	if (node->right) {
		return rbtree_get_min(node->right);
	}
	while ((parent = node->parent) != NULL && node == parent->right) {
		node = parent;
	}
	return parent;
}