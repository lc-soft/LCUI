/*
 * rbtree.h -- Red Black Trees
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
	int(*compare)(void*, const void*);
	void(*destroy)(void*);
	RBTreeNode *root;
} RBTree;

#define RBTree_GetTotal(rbt) (rbt)->total_node;
#define RBTree_OnCompare(rbt, func) (rbt)->compare = func
#define RBTree_OnDestroy(rbt, func) (rbt)->destroy = func

LCUI_API void RBTree_Init(RBTree *rbt);
LCUI_API void RBTree_Destroy(RBTree *rbt);
LCUI_API RBTreeNode *RBTree_First(const RBTree *rbt);
LCUI_API RBTreeNode *RBTree_Next(const RBTreeNode *node);
LCUI_API RBTreeNode* RBTree_Search(RBTree* rbt, int key);
LCUI_API void* RBTree_GetData(RBTree* rbt, int key);
LCUI_API RBTreeNode* RBTree_Insert(RBTree *rbt, int key, void *data);
LCUI_API int RBTree_Erase(RBTree *rbt, int key);
LCUI_API void RBTree_EraseNode(RBTree *rbt, RBTreeNode *node);
LCUI_API int RBTree_CustomErase(RBTree *rbt, const void *keydata);
LCUI_API RBTreeNode* RBTree_CustomSearch(RBTree* rbt, const void *keydata);
LCUI_API void* RBTree_CustomGetData(RBTree* rbt, const void *keydata);
LCUI_API RBTreeNode* RBTree_CustomInsert(RBTree *rbt, const void *keydata, void *data);

LCUI_END_HEADER

#endif
