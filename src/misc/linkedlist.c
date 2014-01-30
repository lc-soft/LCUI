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
#include <LCUI/misc/linkedlist.h>

/** 初始化链表 */
void LinkedList_Init( LinkedList *list, int node_data_size )
{
        list->node_data_size = node_data_size;
        list->used_node_num = 0;
        list->max_node_num = 0;
        list->current_node_pos = -1;
        list->need_free_data = 0;
        list->need_reuse_mem = 0;
        list->head = NULL;
        list->tail = NULL;
        list->current = NULL;
        list->boundary = NULL;
        list->destroy_func = NULL;
}

/** 销毁整个链表 */
void LinkedList_Destroy( LinkedList *list )
{
        list->current_node_pos = -1;
        /* 先释放未使用的结点 */
        while( list->tail != list->boundary ) {
                list->tail = list->tail->prev;
                if( list->need_free_data && list->tail->next->data ) {
                        free( list->tail->next->data );
                }
                list->tail->next->data = NULL;
                list->tail->next->prev = NULL;
                list->tail->next->next = NULL;
                free( list->tail->next );
                list->tail->next = NULL;
        }
        /* 然后释放已使用的结点 */
        while( list->tail != list->head ) {
                list->tail = list->tail->prev;
                if( list->destroy_func ) {
                        list->destroy_func( list->head->data );
                }
                if( list->need_free_data && list->tail->next->data ) {
                        free( list->tail->next->data );
                }
                list->tail->next->data = NULL;
                list->tail->next->prev = NULL;
                list->tail->next->next = NULL;
                free( list->tail->next );
                list->tail->next = NULL;
        }
        /* 单独释放头结点 */
        if( list->destroy_func && list->head->data ) {
                list->destroy_func( list->head->data );
        }
        if( list->need_free_data ) {
                free( list->head->data );
        }
        list->head->data = NULL;
        list->head->next = NULL;
        free( list->head );
        list->head = NULL;
}

/** 移除当前结点 */
int LinkedList_Delete( LinkedList *list )
{
        if( !list->current || list->current_node_pos < 0
         || list->current_node_pos >= list->used_node_num ) {
                return -1;
        }
        if( list->used_node_num <= 0 ) {
                return -2;
        }

        --list->used_node_num;
        if( list->destroy_func ) {
                list->destroy_func( list->current->data );
        }
        if( !list->need_reuse_mem ) {
                free( list->current->data );
                list->current->data = NULL;
        }
        /* 分离当前结点 */
        if( list->current->next ) {
                list->current->next->prev = list->current->prev;
        }
        if( list->current->prev ) {
                list->current->prev->next = list->current->next;
        } else {
                if( list->current->next ) {
                        list->boundary = NULL;
                        return 0;
                }
                list->head = list->current->next;
        }

        if( !list->boundary->next ) {
                list->boundary->next = list->current;
                /* 把当前结点的next结点作为当前结点 */
                list->current = list->current->next;
                list->boundary->next->next = NULL;
                list->boundary->next->prev = list->boundary;
                return 0;
        }

        /* 修改 当前结点 和 边界结点的下个结点 的prev结点记录 */
        list->current->prev = list->boundary;
        list->boundary->next->prev = list->current;
        /* 把当前结点的next结点作为当前结点 */
        list->current = list->current->next;
        /* 修改结点的next结点记录，list->boundary->next->prev记录的是之前分离出来的结点 */
        list->boundary->next->prev->next = list->boundary->next;
        list->boundary->next = list->boundary->next->prev;
        return 0;
}

/** 跳转至指定结点 */
int LinkedList_Goto( LinkedList *list, int pos )
{
        if( pos < 0 || pos >= list->max_node_num ) {
                return -1;
        }
        /* 如果该结点离头结点比较近，则直接从头结点开始遍历 */
        if( pos < list->current_node_pos-pos ) {
                list->current_node_pos = 0;
                list->current = list->head;
        }
        /* 如果该结点离边界结点比较近，则直接从边界结点开始遍历 */
        else if( pos - list->current_node_pos > list->used_node_num - pos ) {
                list->current_node_pos = list->used_node_num-1;
                list->current = list->boundary;
        }

        while( list->current_node_pos < pos ) {
                list->current = list->current->next;
                ++list->current_node_pos;
        }
        while( list->current_node_pos > pos ) {
                list->current = list->current->prev;
                --list->current_node_pos;
        }
        return 0;
}

/** 获取指定位置上的结点 */
static LinkedListNode* LinkedList_GetNode( LinkedList *list, int pos )
{
        int i;
        LinkedListNode *node = NULL;

        if( pos < 0 || pos >= list->max_node_num ) {
                return NULL;
        }
        if( pos < list->current_node_pos-pos ) {
                i = 0;
                node = list->head;
        }
        else if( pos - list->current_node_pos > list->used_node_num - pos ) {
                i = list->used_node_num-1;
                node = list->boundary;
        }

        while( i < pos ) {
                node = list->current->next;
                ++i;
        }
        while( i > pos ) {
                node = list->current->prev;
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
        
        src_node = list->current;
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
                        list->head = src_node;
                }
                src_node->prev = des_node->prev;
                des_node->prev = src_node;
                src_node->next = des_node;
        }
        return 0;
}

/** 在当前结点前面插入新结点，并记录数据 */
void LinkedList_Insert( LinkedList *list, void *data )
{
        if( !list->head ) {
                list->head = (LinkedListNode*)malloc( sizeof(LinkedListNode) );
                list->head->prev = NULL;
                list->head->next = NULL;
                list->head->data = data;
                list->tail = list->head;
                list->boundary = list->head;
                list->current = list->head;
                list->current_node_pos = 0;
                list->max_node_num = 1;
                list->used_node_num = 1;
                return;
        }

        ++list->used_node_num;
        if( list->used_node_num > list->max_node_num ) {
                list->max_node_num = list->used_node_num;
        }
        /* 若当前结点没有上个结点，则插入新结点作为头结点 */
        if( !list->current->prev ) {
                list->head = (LinkedListNode*)malloc( sizeof(LinkedListNode) );
                list->head->prev = NULL;
                list->head->next = list->current;
                list->current->prev = list->head;
                list->current = list->head;
                list->current->data = data;
                return;
        }
        /* 在当前结点的前面插入一个新结点 */
        list->current->prev->next = (LinkedListNode*)malloc( sizeof(LinkedListNode) );
        /* 记录新结点的前后结点 */
        list->current->prev->next->prev = list->current->prev;
        list->current->prev->next->next = list->current;
        /* 将新结点作为当前结点的前结点 */
        list->current->prev = list->current->prev->next;
        /* 更新当前结点 */
        list->current = list->current->prev;
        list->current->data = data;
}

/** 分配一个可用的结点 */
static LinkedListNode *LinkedList_AllocNode( LinkedList *list )
{
        if( !list->head ) {
                list->head = (LinkedListNode*)malloc( sizeof(LinkedListNode) );
                list->head->prev = NULL;
                list->head->next = NULL;
                list->head->data = NULL;
                list->tail = list->head;
                list->boundary = list->head;
                list->current = list->head;
                list->current_node_pos = 0;
                list->max_node_num = 1;
                list->used_node_num = 1;
                return list->head;
        }

        ++list->used_node_num;
        if( list->used_node_num > list->max_node_num ) {
                list->boundary = list->tail;
                list->tail->next = (LinkedListNode*)malloc( sizeof(LinkedListNode) );
                list->tail->next->prev = list->tail;
                list->tail->next->next = NULL;
                list->tail->next->data = NULL;
                list->tail = list->tail->next;
                list->max_node_num = list->used_node_num;
        }
        if( list->boundary ) {
                list->boundary = list->boundary->next;
        } else {
                list->boundary = list->head;
        }
        return list->boundary;
}

/** 将数据复制至链表的相应结点上的数据内存中 */
void *LinkedList_AddData( LinkedList *list, void *data )
{
        LinkedListNode *node;
        node = LinkedList_AllocNode( list );
        if( !node->data ) {
                node->data = malloc( list->node_data_size );
        }
        memcpy( node->data, data, list->node_data_size );
        return node->data;
}

/** 将数据的地址记录至链表的相应结点 */
void *LinkedList_AddDataByAddress( LinkedList *list, void *data_ptr )
{
        LinkedListNode *node;
        node = LinkedList_AllocNode( list );
        if( node->data ) {
                free( node->data );
        }
        node->data = data_ptr;
        return node->data;
}
