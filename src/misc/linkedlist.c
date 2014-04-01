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

#include <LCUI_Build.h>
#include LC_MISC_LINKED_LIST_H

#define MALLOC_ONE(type) (type*)malloc(sizeof(type))

/** 初始化链表 */
LCUI_API void LinkedList_Init( LinkedList *list, int node_data_size )
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
LCUI_API void LinkedList_Destroy( LinkedList *list )
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
LCUI_API int LinkedList_Delete( LinkedList *list )
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
	if( list->current_node == list->used_tail_node ) {
		if( list->current_node->prev ) {
			list->used_tail_node = list->current_node->prev;
		} else {
			list->used_tail_node = NULL;
		}
	}
        /* 分离当前结点 */
        if( list->current_node->next ) {
                list->current_node->next->prev = list->current_node->prev;
        }
        if( list->current_node->prev ) {
                list->current_node->prev->next = list->current_node->next;
        } else {
                list->used_head_node = list->current_node->next;
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
LCUI_API int LinkedList_Goto( LinkedList *list, int pos )
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
LCUI_API int LinkedList_MoveTo( LinkedList *list, int pos )
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
		list->used_tail_node = list->used_head_node;
		list->current_node = list->used_head_node;
		list->current_node_pos = 0;
		return list->used_head_node;
	}

	list->used_tail_node->next = list->usable_head_node;
	list->used_tail_node->next->prev = list->used_tail_node;
	list->usable_head_node = list->usable_head_node->next;
	if( list->usable_head_node ) {
		list->usable_head_node->prev = NULL;
	}
	list->used_tail_node = list->used_tail_node->next;
	list->used_tail_node->next = NULL;
        return list->used_tail_node;
}

/** 在当前结点前面插入新结点，并引用数据 */
LCUI_API int LinkedList_Insert( LinkedList *list, void *data )
{
	LinkedListNode *node;
	
	node = LinkedList_AllocNode( list );
	node->data = data;
	
	if( !list->current_node ) {
		return -1;
	}

	if( list->current_node == list->used_head_node ) {
		node->next = list->used_head_node;
		list->used_head_node = node;
		if( node == list->used_tail_node ) {
			list->used_tail_node = node->prev;
			list->used_tail_node->next = NULL;
		}
		node->prev = NULL;
	} else {
		node->next = list->current_node;
		node->prev = list->current_node->prev;
		list->current_node->prev = node;
		list->current_node = node;
	}

	return 0;
}

/** 在当前结点前面插入新结点，并将数据的副本记录到该结点上 */
LCUI_API void* LinkedList_InsertCopy( LinkedList *list, void *data )
{
	void *data_copy;
	data_copy = malloc( list->node_data_size );
        memcpy( data_copy, data, list->node_data_size );
	LinkedList_Insert( list, data_copy );
	return data_copy;
}

/** 将数据的副本记录至链表的相应结点上 */
LCUI_API void *LinkedList_AddDataCopy( LinkedList *list, void *data )
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
LCUI_API void LinkedList_AddData( LinkedList *list, void *data_ptr )
{
        LinkedListNode *node;
        node = LinkedList_AllocNode( list );
        if( node->data ) {
                free( node->data );
        }
        node->data = data_ptr;
}
