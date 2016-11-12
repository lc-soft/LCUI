/* ***************************************************************************
 * rbtree.h -- Red Black Trees
 *
 * Copyright (C) 2014-2016 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * rbtree.h-- 红黑树
 *
 * 版权所有 (C) 2014-2016 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#ifndef LCUI_UTIL_RBTREE_H
#define LCUI_UTIL_RBTREE_H

LCUI_BEGIN_HEADER

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
	int (*compare)(void*, const void*);
	void (*destroy)(void*);
        RBTreeNode *root;
} RBTree;

#define RBTree_GetTotal(rbt) (rbt)->total_node;
#define RBTree_OnCompare(rbt, func) (rbt)->compare = func
#define RBTree_OnDestroy(rbt, func) (rbt)->destroy = func

LCUI_API void RBTree_Init( RBTree *rbt );
LCUI_API void RBTree_Destroy( RBTree *rbt );
LCUI_API RBTreeNode *RBTree_First( const RBTree *rbt );
LCUI_API RBTreeNode *RBTree_Next( const RBTreeNode *node );
LCUI_API RBTreeNode* RBTree_Search( RBTree* rbt, int key );
LCUI_API void* RBTree_GetData( RBTree* rbt, int key );
LCUI_API RBTreeNode* RBTree_Insert( RBTree *rbt, int key, void *data );
LCUI_API int RBTree_Erase( RBTree *rbt, int key );
LCUI_API void RBTree_EraseNode( RBTree *rbt, RBTreeNode *node );
LCUI_API int RBTree_CustomErase( RBTree *rbt, const void *keydata );
LCUI_API RBTreeNode* RBTree_CustomSearch( RBTree* rbt, const void *keydata );
LCUI_API void* RBTree_CustomGetData( RBTree* rbt, const void *keydata );
LCUI_API RBTreeNode* RBTree_CustomInsert( RBTree *rbt, const void *keydata, void *data );

LCUI_END_HEADER

#endif
