/* ***************************************************************************
* rbtree.c -- Red Black Trees
 * 
 * Copyright (C) 2014 by Liu Chao <lc-soft@live.cn>
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
* rbtree.c -- 红黑树
 *
 * 版权所有 (C) 2014 归属于 刘超 <lc-soft@live.cn>
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <LCUI_Build.h>
#include LC_MISC_RB_TREE_H

#define RED     0
#define BLACK   1

LCUI_API void RBTree_Init( LCUI_RBTree *rbt )
{
        rbt->root = NULL;
        rbt->total_node = 0;
}

LCUI_API void RBTree_Destroy( LCUI_RBTree *rbt )
{

}

/** 获取第一个结点 */
LCUI_API LCUI_RBTreeNode *RBTree_First( const LCUI_RBTree *rbt )
{
        LCUI_RBTreeNode *node;

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
LCUI_API LCUI_RBTreeNode *RBTree_Next( const LCUI_RBTreeNode *node )
{
        LCUI_RBTreeNode *parent;  

        if( node->parent == node ) {
                return NULL;  
        }
        if( node->right ) {
                node = node->right;   
                while( node->left ) {
                        node = node->left;
                }
                return (LCUI_RBTreeNode*)node;
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
static LCUI_RBTreeNode* 
rb_rotate_left( LCUI_RBTreeNode* node, LCUI_RBTreeNode* root )
{
        LCUI_RBTreeNode* right = node->right;

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
static LCUI_RBTreeNode* 
rb_rotate_right( LCUI_RBTreeNode* node, LCUI_RBTreeNode* root )
{
        LCUI_RBTreeNode* left = node->left;

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

static LCUI_RBTreeNode* 
rb_insert_rebalance( LCUI_RBTreeNode *root, LCUI_RBTreeNode *node )
{
        LCUI_RBTreeNode *parent, *gparent, *uncle, *tmp;

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
static LCUI_RBTreeNode* 
rb_search_auxiliary( LCUI_RBTreeNode *root, int key, LCUI_RBTreeNode** save )
{
        LCUI_RBTreeNode *node = root, *parent = NULL;

        while( node ) {
                parent = node;
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

LCUI_API LCUI_RBTreeNode* RBTree_Search( LCUI_RBTree* rbt, int key )
{
        return rb_search_auxiliary( rbt->root, key, NULL );
}

LCUI_API void* RBTree_GetData( LCUI_RBTree* rbt, int key )
{
        LCUI_RBTreeNode *node;
        node = rb_search_auxiliary( rbt->root, key, NULL );
        if( node ) {
                return node->data;
        }
        return NULL;
}

LCUI_API int RBTree_Insert( LCUI_RBTree *rbt, int key, void *data )
{
        LCUI_RBTreeNode *root, *node, *parent_node;
        
        parent_node = NULL;
        root = rbt->root;
        node = rb_search_auxiliary( root, key, &parent_node );
        if( node ) {
                return -1;
        }

        node = (LCUI_RBTreeNode*)malloc( sizeof(LCUI_RBTreeNode) );
        node->left = NULL;
        node->parent = parent_node;
        node->right = NULL;
        node->data = data;
        node->key = key;
        node->color = RED;

        if( parent_node ) {
                if( parent_node->key > key ) {
                        parent_node->left = node;
                } else {
                        parent_node->right = node;
                }
        } else {
                root = node;
        }
        rbt->total_node += 1;
        /* 调整红黑树的平衡 */
        rbt->root = rb_insert_rebalance( root, node );
        return 0;
}

/** 在树中的结点被删除后调整平衡 */
static LCUI_RBTreeNode*
rb_erase_rebalance( LCUI_RBTreeNode *node, LCUI_RBTreeNode *parent,
                                                LCUI_RBTreeNode *root )
{
        LCUI_RBTreeNode *other, *o_left, *o_right;

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
                                continue;
                        }
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

                other = parent->left;
                if (other->color == RED) {
                        other->color = BLACK;
                        parent->color = RED;
                        root = rb_rotate_right(parent, root);
                        other = parent->left;
                }
                if ((!other->left || other->left->color == BLACK)
                                && (!other->right || other->right->color == BLACK)) {
                        other->color = RED;
                        node = parent;
                        parent = node->parent;
                        continue;
                }

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

        if( node ) {
                node->color = BLACK;
        }
        return root;
}

/** 删除红黑树中的结点 */
int RBTree_Erase( LCUI_RBTree *rbt, int key )  
{
        unsigned char color;
        LCUI_RBTreeNode *root, *child, *parent, *old, *left, *node; 

        root = rbt->root;
        /* 查找要删除的结点 */
        node = rb_search_auxiliary( root, key, NULL );
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
