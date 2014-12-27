/* ***************************************************************************
 * linkedlist.c -- Linked List
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
 * linkedlist.c -- 链表
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

//#define DEBUG
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

/*
 * 注意，此链表是线程不安全的。
 * 此链表支持结点内存复用，可减少内存申请次数，但链表的内存占用会只增不减，一
 * 直会保持之前最大长度时占用的内存大小。
 * 加入了游标功能，用于记录当前所处结点的位置，主要是方便访问、删除、插入、移
 * 动结点，不必每次操作都从头至尾遍历链表来定位目标结点。
 * 从头至尾遍历链表，只需要这样使用：
 *
 * LinkedList_Goto( list, 0 );
 * while( data = (datatype)LinkedList_Get(list) ) {
 *	// 使用 data 的代码块
 *	...
 *	LinkedList_ToNext( list );
 * }
 *
 * 由于部分操作的目标结点都是由游标决定的，虽然在单线程下可以这样使用，但如果
 * 有多个线程共用同一个链表，这个游标的定位可能会不正常，例如：多个线程使用上
 * 述代码同时遍历同一个链表，各个线程有可能不会完整的遍历全部结点，假设线程A
 * 先遍历完了 1 2 3 结点，那么线程B就会从 4 结点开始遍历。
 * 在不确定同一时间段内该链表仅有当前线程使用的情况下，建议使用类似于遍历数组
 * 的方法进行遍历：
 *
 * int i, n = LinkedList_GetTotal( list );
 * for( i=0; i<n; ++i ) {
 *	LinkedList_Goto( list, i );
 *	data = (datatype)LinkedList_Get( list );
 * }
 *
 * 上述代码中调用的函数都是内联函数，如果编译器能够展开内联函数的代码的话，就不
 * 会产生像调用普通函数那样的性能开销。上述两个特性看上去可能会带来一点效率上的
 * 提升，但具体还是以实际效果为准。
 */

#define MALLOC_ONE(type) (type*)malloc(sizeof(type))

/** 初始化链表 */
void LinkedList_Init( LinkedList *list, int node_data_size )
{
        list->node_data_size = node_data_size;
        list->used_node_num = 0;
        list->usable_node_num = 0;
        list->current_node_pos = 0;
        list->need_free_data = 0;
        list->need_reuse_mem = 0;
	list->usable_head_node = NULL;
	list->used_head_node = NULL;
	list->used_tail_node = NULL;
        list->current_node = NULL;
        list->destroy_func = NULL;
}

/** 销毁整个链表 */
void LinkedList_Destroy( LinkedList *list )
{
	LinkedListNode *node;
        list->current_node_pos = 0;
	list->current_node = NULL;
	list->used_tail_node = NULL;
	list->used_node_num = 0;
	list->usable_node_num = 0;
        /* 先释放未使用的结点 */
	if( list->usable_head_node ) {
		node = list->usable_head_node;
		while( node ) {
			list->usable_head_node = node->next;
			if( list->need_free_data && node->data ) {
				free( node->data );
			}
			node->data = NULL;
			node->prev = NULL;
			node->next = NULL;
			free( node );
			node = list->usable_head_node;
		}
	}
        /* 释放已使用的结点 */
	if( list->used_head_node ) {
		node = list->used_head_node;
		while( node ) {
			list->used_head_node = node->next;
			if( list->need_free_data && node->data ) {
				free( node->data );
			}
			node->data = NULL;
			node->prev = NULL;
			node->next = NULL;
			free( node );
			node = list->used_head_node;
		}
	}
}

/** 移除当前结点 */
int LinkedList_Delete( LinkedList *list )
{
        if( list->used_node_num <= 0 ) {
                return -2;
        }

        if( !list->current_node || list->current_node_pos < 0
         || list->current_node_pos >= list->used_node_num ) {
                return -1;
        }

        --list->used_node_num;
	++list->usable_node_num;

        if( list->destroy_func ) {
                list->destroy_func( list->current_node->data );
        }
        if( !list->need_reuse_mem ) {
                free( list->current_node->data );
                list->current_node->data = NULL;
        }
	/* 如果在链表尾端 */
	DEBUG_MSG("current = %p, head = %p, tail = %p, used_node_num = %d\n", list->current_node, list->used_head_node, list->used_tail_node, list->used_node_num);
	if( list->current_node == list->used_tail_node ) {
		if( list->current_node->prev ) {
			list->used_tail_node = list->current_node->prev;
			DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
		} else {
			list->used_tail_node = NULL;
			DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
		}
	}
	if( list->current_node == list->used_head_node ) {
		list->used_head_node = list->used_head_node->next;
		DEBUG_MSG("head = %p\n", list->used_head_node);
	}
	DEBUG_MSG("list->current_node->next = %p\n", list->current_node->next);
	DEBUG_MSG("list->current_node->prev = %p\n", list->current_node->prev);
        /* 分离当前结点 */
        if( list->current_node->next ) {
		if( list->current_node->prev ) {
			list->current_node->next->prev = list->current_node->prev;
			list->current_node->prev->next = list->current_node->next;
		} else {
			list->used_head_node = list->current_node->next;
			list->used_head_node->prev = NULL;
		}
        } else {
		if( list->current_node->prev ) {
			list->current_node->prev->next = list->current_node->next;
		} else {
			list->used_head_node = list->current_node->next;
		}
	}

	list->current_node->prev = NULL;
	if( list->usable_head_node ) {
		list->usable_head_node->prev = list->current_node;
		list->current_node = list->current_node->next;
		list->usable_head_node->prev->next = list->usable_head_node;
		list->usable_head_node = list->usable_head_node->prev;
	} else {
		list->usable_head_node = list->current_node;
		list->current_node = list->current_node->next;
		list->usable_head_node->next = NULL;
	}
        return 0;
}

/** 跳转至指定结点 */
int LinkedList_Goto( LinkedList *list, int pos )
{
        if( pos < 0 || pos >= list->used_node_num ) {
                return -1;
        }
        /* 如果该结点离头结点比较近，则直接从头结点开始遍历 */
        if( pos < list->current_node_pos-pos ) {
                list->current_node_pos = 0;
		list->current_node = list->used_head_node;
        }
        /* 如果该结点离边界结点比较近，则直接从边界结点开始遍历 */
        else if( pos - list->current_node_pos > list->used_node_num - pos ) {
                list->current_node_pos = list->used_node_num-1;
		list->current_node = list->used_tail_node;
        }

        while( list->current_node_pos < pos ) {
                list->current_node = list->current_node->next;
                ++list->current_node_pos;
        }
        while( list->current_node_pos > pos ) {
                list->current_node = list->current_node->prev;
                --list->current_node_pos;
        }
        return 0;
}

/** 获取指定位置上的结点 */
static LinkedListNode* LinkedList_GetNode( LinkedList *list, int pos )
{
        int i;
        LinkedListNode *node = NULL;

        if( pos < 0 || pos >= list->used_node_num ) {
                return NULL;
        }
        if( pos < list->current_node_pos-pos ) {
                i = 0;
		node = list->used_head_node;
        }
        else if( pos - list->current_node_pos > list->used_node_num - pos ) {
                i = list->used_node_num-1;
		node = list->used_tail_node;
        } else {
                i = list->current_node_pos;
        }

        while( i < pos ) {
                node = list->current_node->next;
                ++i;
        }
        while( i > pos ) {
                node = list->current_node->prev;
                --i;
        }
        return node;
}

/** 将当前结点移动至指定位置 */
int LinkedList_MoveTo( LinkedList *list, int pos )
{
        LinkedListNode *src_node, *des_node;

        des_node = LinkedList_GetNode( list, pos );
        if( !des_node ) {
                return -1;
        }
        
        src_node = list->current_node;
        src_node->prev->next = src_node->next;
        if( src_node->next ) {
                src_node->next->prev = src_node->prev;
        }

        if( list->current_node_pos < pos ) {
                src_node->prev = des_node;
                src_node->next = des_node->next;
                des_node->next->prev = src_node;
                des_node->next = src_node;
        }
        else if( list->current_node_pos > pos ) {
                if( des_node->prev ) {
                        des_node->prev->next = src_node;
                } else {
			list->used_head_node = src_node;
                }
                src_node->prev = des_node->prev;
                des_node->prev = src_node;
                src_node->next = des_node;
        }
        return 0;
}

/** 分配一个可用的结点 */
static LinkedListNode *LinkedList_AllocNode( LinkedList *list )
{
        if( !list->usable_head_node ) {
		list->usable_head_node = MALLOC_ONE(LinkedListNode);
		list->usable_head_node->next = NULL;
		list->usable_head_node->prev = NULL;
		list->usable_head_node->data = NULL;
		++list->usable_node_num;
        }

        ++list->used_node_num;
	--list->usable_node_num;

	if( !list->used_head_node ) {
		list->used_head_node = list->usable_head_node;
		list->usable_head_node = list->usable_head_node->next;
		if( list->usable_head_node ) {
			list->usable_head_node->prev = NULL;
		}
		list->used_head_node->next = NULL;
		list->used_head_node->prev = NULL;
		list->used_tail_node = list->used_head_node;
		DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
		list->current_node = list->used_head_node;
		list->current_node_pos = 0;
		return list->used_head_node;
	}
	
	DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
	list->used_tail_node->next = list->usable_head_node;
	list->used_tail_node->next->prev = list->used_tail_node;
	list->usable_head_node = list->usable_head_node->next;
	if( list->usable_head_node ) {
		list->usable_head_node->prev = NULL;
	}
	list->used_tail_node = list->used_tail_node->next;
	DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
	list->used_tail_node->next = NULL;
        return list->used_tail_node;
}

/** 在当前结点前面插入新结点，并引用数据 */
int LinkedList_Insert( LinkedList *list, void *data )
{
	LinkedListNode *node;
	
	node = LinkedList_AllocNode( list );
	node->data = data;
	
	if( !list->current_node ) {
		return -1;
	}

	if( list->current_node == list->used_head_node ) {
		/* 已经在这个位置就不用移动结点了 */
		if( node == list->used_head_node ) {
			return 0;
		}
		DEBUG_MSG("%p, %p\n", node, list->current_node);
		node->next = list->used_head_node;
		list->used_head_node = node;
		list->used_tail_node = node->prev;
		DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
		list->used_tail_node->next = NULL;
		node->prev = NULL;
	} else {
		node->next = list->current_node;
		list->used_tail_node = node->prev;
		DEBUG_MSG("list->used_tail_node = %p\n", list->used_tail_node);
		list->used_tail_node->next = NULL;
		node->prev = list->current_node->prev;
		list->current_node->prev = node;
		list->current_node = node;
	}

	return 0;
}

/** 在当前结点前面插入新结点，并将数据的副本记录到该结点上 */
void* LinkedList_InsertCopy( LinkedList *list, void *data )
{
	void *data_copy;
	data_copy = malloc( list->node_data_size );
        memcpy( data_copy, data, list->node_data_size );
	LinkedList_Insert( list, data_copy );
	return data_copy;
}

/** 将数据的副本记录至链表的相应结点上 */
void *LinkedList_AddDataCopy( LinkedList *list, void *data )
{
        LinkedListNode *node;
        node = LinkedList_AllocNode( list );
        if( !node->data ) {
                node->data = malloc( list->node_data_size );
        }
        memcpy( node->data, data, list->node_data_size );
        return node->data;
}

/** 将数据引用至链表的相应结点 */
void LinkedList_AddData( LinkedList *list, void *data_ptr )
{
        LinkedListNode *node;
        node = LinkedList_AllocNode( list );
        if( node->data ) {
                free( node->data );
        }
        node->data = data_ptr;
}
