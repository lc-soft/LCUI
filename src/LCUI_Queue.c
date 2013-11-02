/* ***************************************************************************
 * LCUI_Queue.c -- basic queue processing
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * LCUI_Queue.c -- 基本的队列处理
 *
 * 版权所有 (C) 2012-2013 归属于
 * 刘超
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
 
/*
 * 队列设计方案：
 * 队列有“已用”和“未用”这两种空间，element_size保存队列成员占用空间大小，总成员个数
 * 由max_num表示，包含有效数据的成员个数由total_num表示。
 * 当删除一个队列成员后，total_num会减1，max_num不变，该队列成员占用的空间不会被释放，
 * 以备后续使用，而该队列成员会移动至队列末尾，成为“未用”空间。
 * 当往队列里添加数据时，如果total_num小于max_num，则使用“未用”空间里的队列成员来保存
 * 该数据。
 * member_type保存队列的成员数据类型：指针 或者 变量；如果队列主要用于存放指针，引用
 * 其它位置的数据，那么，在删除，销毁队列时，不会释放数据指针指向的内存空间；否则，会
 * 释放。
 * 队列的数据存储方式有两种：数组 和 链表；
 * 前者的读写速度较快，但数据插入效率较慢，因为在指定位置插入数据后，该位置后面的所有
 * 数据的位置需要向后移动，后面的数据元素越多，耗时越多。
 * 后者适合频繁的进行数据插入和删除，因为只需要找到目标位置的结点，然后插入新结点即可。
 * LCUI的文本位图层的处理就用到了后者，因为编辑文本主要是添加和删除文字。
 * 此模块代码可以改进，比如可以选择相应算法，以提高队列的 查找和插入 效率。
 * */

//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_WIDGET_H 
#include LC_ERROR_H

#define MEMBER_TYPE_ADDR	0
#define MEMBER_TYPE_DATA	1

/** 为队列设定互斥锁，使之只能被一个线程使用 */
LCUI_API int Queue_Lock( LCUI_Queue *queue )
{ 
	return LCUIMutex_Lock( &queue->mutex );
}

/** 解开队列的互斥锁 */
LCUI_API int Queue_Unlock( LCUI_Queue *queue )
{
	return LCUIMutex_Unlock( &queue->mutex );
}

/** 初始化队列 */
LCUI_API void Queue_Init(	LCUI_Queue *queue,
				size_t element_size,
				void (*func) (void*)) 
{
	LCUIMutex_Init( &queue->mutex );
	queue->member_type	= MEMBER_TYPE_DATA;
	queue->data_mode	= QUEUE_DATA_MODE_ARRAY;
	queue->data_array	= NULL;
	queue->data_head_node.data = NULL;
	queue->data_head_node.prev = NULL;
	queue->data_head_node.next = NULL;
	queue->total_num	= 0;
	queue->max_num		= 0;
	queue->element_size	= element_size;
	queue->destroy_func	= func; 
}

/**
 * 功能：设定队列成员类型为指针 
 * 说明：如果队列只是用于存放指针，并且不希望队列销毁后，指针指向的内存空间也被释放，可
 * 使用该函数设置。
 * */
LCUI_API void Queue_UsingPointer( LCUI_Queue *queue )
{
	queue->member_type = MEMBER_TYPE_ADDR;
}

/** 获取队列当前总成员数量 */
LCUI_API int Queue_GetTotal( LCUI_Queue *queue )
{
	return queue->total_num;
}

/**
 * 功能：设定队列使用的数据储存模式
 * 说明：只能在初始化后且未加入成员时使用该函数
 * */
LCUI_API int Queue_SetDataMode( LCUI_Queue *queue, Queue_DataMode mode )
{
	if(queue->total_num > 0 || queue->max_num > 0) {
		return -1;
	}
	queue->data_mode = 1;
	return 0;
}

/** 交换队列中指定位置两个成员的位置 */
LCUI_API int Queue_Swap( LCUI_Queue *queue, int pos_a, int pos_b )
{
	void *temp;
	
	if (pos_a >= queue->total_num || pos_b >= queue->total_num 
	 || pos_a == pos_b || queue->total_num <= 0) {
		return -1;
	}
	
	if(queue->data_mode == QUEUE_DATA_MODE_ARRAY) {
		temp = queue->data_array[pos_a];
		queue->data_array[pos_a] = queue->data_array[pos_b];
		queue->data_array[pos_b] = temp;
	} else {
		int i;
		LCUI_Node *a, *b;
		/* 找到两个位置的结点 */
		a = &queue->data_head_node;
		for(i=0; i<=pos_a; ++i) {
			a = a->next;
		}
		b = &queue->data_head_node;
		for(i=0; i<=pos_b; ++i) {
			b = b->next;
		}
		/* 交换指针 */
		temp = a->data;
		a->data = b->data;
		b->data = temp;
	}
	return 0;
}

/** 释放队列占用的内存资源 */
LCUI_API void Queue_Destroy( LCUI_Queue *queue )
{
	/* 有效性判断 */
	if( !queue ) { 
		return;
	}
	
	if( queue->data_mode == QUEUE_DATA_MODE_LINKED_LIST ) {
		LCUI_Node *p, *obj;
		p = queue->data_head_node.next;
		/* 切换到下个结点后，把上一个结点销毁 */
		while( p ) {
			/* 记录当前结点 */
			obj = p;
			/* 切换到下个结点 */
			p = p->next;
			/* 如果队列成员保存的是数据 */
			if( queue->member_type == MEMBER_TYPE_DATA ) {
				/* 若有析构函数，则调用它进行清理工作 */
				if( queue->destroy_func ) {
					queue->destroy_func( obj->data );
				}
				/* 释放数据占用的内存空间 */
				free( obj->data );
			}
			/* 释放当前结点占用的内存空间 */
			free( obj );
		}
		/* 用goto只是为了避免if-else带来的多余的缩进 */
		goto final_work;
	}
	/* 如果队列成员保存的是数据 */
	if( queue->member_type == MEMBER_TYPE_DATA ) {
		int i;
		for (i=0; i<queue->total_num; ++i) {
			/* 若有析构函数，则先调用析构函数进行相关清理工作 */
			if( queue->destroy_func ) {
				queue->destroy_func( queue->data_array[i] );
			}
			/* 然后释放该队列成员的数据占用的内存空间 */
			free( queue->data_array[i] );
		}
		/* 如果有未使用的队列成员，则把它们占用的内存空间也释放掉 */
		if( queue->max_num > queue->total_num ) {
			if( queue->total_num > 0 ) {
				i = queue->total_num;
			} else {
				i = 0;
			}
			for(; i<queue->max_num; ++i) {
				free( queue->data_array[i] );
			}
		}
	}
	/* 将用于存储各个队列成员地址的空间释放掉 */ 
	free( queue->data_array );

final_work:
	/* 重置各种数据 */
	queue->data_array = NULL;
	queue->data_head_node.next = NULL;
	queue->total_num = 0;
	queue->max_num = 0;
	/* 销毁互斥锁 */
	LCUIMutex_Destroy( &queue->mutex );
}

/**
 * 功能：从队列中获取指定位置的成员 
 * 说明：成功返回指向该成员的指针，失败返回NULL
 * 注意：请勿对返回的指针进行free操作
 * */
LCUI_API void* Queue_Get( LCUI_Queue *queue, int pos )
{
	int i;
	LCUI_Node *p;
	
	if (queue->total_num <= 0 || pos < 0 || pos >= queue->total_num) {
		return NULL;
	}
	
	if(queue->data_mode == QUEUE_DATA_MODE_ARRAY) {
		return queue->data_array[pos]; 
	}

	p = queue->data_head_node.next;
	for(i=0; i< pos && p->next; ++i) {
		p = p->next;
	}
	if( p != NULL ) {
		return p->data;
	}

	return NULL;
}

/** 向队列中指定位置插入成员 */
LCUI_API int Queue_Insert(	LCUI_Queue *queue,
				int pos,
				const void *data )
{
	int src_pos;
	src_pos = queue->total_num;
	if( Queue_Add(queue, data) ) {
		return Queue_Move(queue, pos, src_pos); 
	}
	return -1;
}

/** 向队列中指定位置插入成员的指针 */
LCUI_API int Queue_InsertPointer(	LCUI_Queue *queue,
					int pos,
					const void *data )
{
	int src_pos;
	src_pos = queue->total_num;
	if( Queue_AddPointer(queue, data) ) {
		return Queue_Move(queue, pos, src_pos); 
	}
	return -1;
}

/** 将队列中指定位置的成员移动至目的位置 */
LCUI_API int Queue_Move( LCUI_Queue *queue, int des_pos, int src_pos )
{
	void *temp;
	int i, total;
	LCUI_Node *temp_node, *p_src, *p_des;
	
	total = Queue_GetTotal(queue);
	if(des_pos < 0 || des_pos > total 
	|| src_pos < 0 || src_pos > total ) {
		return -1;
	}
	//printf("Queue_Move(): queue: %p, des pos: %d, src pos: %d, total: %d\n", 
	//queue, des_pos, src_pos, queue->total_num);
	if(des_pos == src_pos ) {
		return 0;
	}
	
	if(queue->data_mode == QUEUE_DATA_MODE_ARRAY) {
		temp = queue->data_array[src_pos];
		if (src_pos > des_pos) {
		/* 如果新位置在原位置的前面，把两位置之间的成员向右移动 */
			for (i = src_pos; i > des_pos; --i) {
				queue->data_array[i] = queue->data_array[i - 1];  
			}
		} else if (src_pos < des_pos) {
		/* 如果新位置在原位置的后面，把两位置之间的成员向左移动 */
			for (i = src_pos; i < des_pos; ++i) {
				queue->data_array[i] = queue->data_array[i + 1];  
			}
		}
		queue->data_array[des_pos] = temp;
		return 0;
	}

	/* 得到源位置的结点的指针 */
	p_src = queue->data_head_node.next;
	for(i=0; p_src->next && i<src_pos; ++i ) {
		p_src = p_src->next;
	}
	/* 获取前个结点 */
	temp_node = p_src->prev;
	/* 前个结点的next指针指向当前结点的下个结点 */
	temp_node->next = p_src->next;
	/* 若该结点不是在末尾 */
	if( p_src->next ) {
		/* 当前结点的下个结点的prev指针指向前个结点 */
		p_src->next->prev = temp_node;
	}
	/* 得到目标位置的结点的指针 */
	p_des = queue->data_head_node.next;
	if(des_pos < src_pos) {
		for(i=0; p_des->next && i<des_pos; ++i ) {
			p_des = p_des->next;
		}
		/* 插入至目标结点的前面，并修改prev指针 */
		temp_node = p_des->prev;
		temp_node->next = p_src;
		p_src->prev = temp_node;
		/* 修改该结点的后结点 */
		p_src->next = p_des; 
		p_des->prev = p_src;
	} else {
		/* 数量有变动，目标位置在源位置后面，位置需向前移1个单位 */
		des_pos -= 1;
		/* 找到目标位置的结点 */
		for(i=0; p_des->next && i<des_pos; ++i ) {
			p_des = p_des->next;
		}
		/* 将结点接到目标位置的结点的后面 */
		p_src->next = p_des->next;
		if( p_des->next ) {
			p_des->next->prev = p_src;
		}
		p_des->next = p_src;
		p_src->prev = p_des;
	}
	return 0;
}

/** 根据给定的标志，覆盖队列中指定位置的成员 */
static int __Queue_Replace(	LCUI_Queue *queue,
				int pos,
				const void *data,
				LCUI_BOOL is_pointer )
{
	int i;
	LCUI_Node *p;

	/* 如果超出队列范围 */
	if( pos >= queue->total_num || pos < 0 ) {
		return -1;
	}
	
	if(queue->data_mode == QUEUE_DATA_MODE_ARRAY) {
		/* 
		 * 考虑到队列成员有时会是结构体，并且结构体成员中可能会有指针，为了避免因重复覆盖不
		 * 对指针进行释放而导致的内存溢出，需要先调用析构函数对该成员进行销毁，因为析构函数
		 * 一般会对结构体中的指针进行释放，之后，再复制新成员的数据至该成员的内存空间。
		 *  */
		if( queue->destroy_func ) {
			queue->destroy_func(queue->data_array[pos]); 
		}
		if( is_pointer ) {
			/* 直接更改当前成员记录的地址 */
			memcpy( &queue->data_array[pos], &data, sizeof(void*) );
		} else {
			/* 将数据覆盖至当前成员的内存空间里 */
			memcpy( queue->data_array[pos], data, queue->element_size );
		}
		return 0;
	} 
		
	p = queue->data_head_node.next;
	for(i=0; p->next && i<pos; ++i ) {
		p = p->next;
	}
	if( queue->destroy_func ) {
		queue->destroy_func( p->data ); 
	}
	if( is_pointer ) {
		memcpy( &p->data, &data, sizeof(void*) );
	} else {
		memcpy( p->data, data, queue->element_size ); 
	}
	return 0;
}

/** 覆盖队列中指定位置的成员内存空间里的数据 */
LCUI_API int Queue_Replace(	LCUI_Queue *queue,
				int pos,
				const void *data )
{
	return __Queue_Replace( queue, pos, data, FALSE );
}

/** 覆盖队列中指定位置的成员指针 */
LCUI_API int Queue_ReplacePointer(	LCUI_Queue *queue,
					int pos,
					const void *data )
{
	return __Queue_Replace( queue, pos, data, TRUE );
}

/** 
 * 功能：将新的成员添加至队列 
 * 说明：是否为新成员重新分配内存空间，由参数flag的值决定
 * 返回值：正常则返回在队列中的位置，错误则返回非0值
 * */
static void* __Queue_Add(	LCUI_Queue *queue, 
				const void *data,
				LCUI_BOOL is_pointer )
{
	int i, pos;
	size_t size; 
	LCUI_Node *p, *q;

	pos = queue->total_num;
	++queue->total_num;
	/* 如果数据是以数组形式储存 */
	if( queue->data_mode == QUEUE_DATA_MODE_LINKED_LIST ) {
		DEBUG_MSG("new total_num: %d\n", queue->total_num);
		if(queue->total_num > queue->max_num) {
			p = &queue->data_head_node;
			DEBUG_MSG("head_node: %p, next: %p\n", p, p->next);
			for( i=0; i<pos && p->next; ++i ) {
				p = p->next;
			}
			q = (LCUI_Node*)malloc(sizeof(LCUI_Node)); 
			q->prev = p;
			q->next = NULL;
			p->next = q;
			p = q;
			queue->max_num = queue->total_num;
			if( !is_pointer ) { 
				p->data = malloc( queue->element_size );
				memcpy( p->data, data, queue->element_size );
			} else {
				memcpy( &p->data, &data, sizeof(void*) );
			}
			return p->data;
		}
		p = queue->data_head_node.next; 
		for(i=0; p->next && i<pos; ++i ) {
			p = p->next;
		} 
		if( !is_pointer ) {
			memcpy( p->data, data, queue->element_size );
		} else {
			memcpy( &p->data, &data, sizeof(void*) );
		}
		return p->data;
	}
	/* 如果当前成员使用量不大于最大成员数量，则说明有未使用的队列成员空间 */
	if( queue->total_num <= queue->max_num ) {
		/* 如果不保存指针 */
		if( !is_pointer ) {
			/* 如果当前位置的成员空间的指针无效 */
			if( !queue->data_array[pos] ) {
				queue->data_array[pos] = malloc( queue->element_size );
			}
			/* 复制数据至当前位置的成员空间里 */
			memcpy(queue->data_array[pos], data, queue->element_size);
		} else {
			/* 指针间的赋值 */
			memcpy(&queue->data_array[pos], &data, sizeof(void*));
		}
		return queue->data_array[pos];
	}
	
	queue->max_num = queue->total_num;
	size = sizeof(void*) * queue->total_num;
	/* 如果总数大于1，说明之前已经malloc过，直接realloc扩增内存 */
	if (queue->total_num > 1 && queue->data_array ) {
		queue->data_array = (void **)realloc( queue->data_array, size );
	} else {
		queue->data_array = (void **)malloc( sizeof(void*) ); 
	}
	if( !queue->data_array ) {
		printf("__Queue_Add(): "ERROR_MALLOC_ERROR);
		exit(-1);
	}
	if( !is_pointer ) {
		queue->data_array[pos] = malloc( queue->element_size );
		memcpy(queue->data_array[pos], data, queue->element_size);
	} else {
		memcpy(&queue->data_array[pos], &data, sizeof(void*));
	}
	return queue->data_array[pos];
}

/** 打印队列信息，一般用于调试 */
LCUI_API void Queue_PrintInfo( LCUI_Queue *queue )
{
	printf(
		"queue: %p, total: %d, max: %d, data_mode: %d\n"
		"data_array: %p, node: %p\n",
		queue, queue->total_num, queue->max_num, queue->data_mode,
		queue->data_array, queue->data_head_node.data
	);
}

/**
 * 功能：将新的成员添加至队列 
 * 说明：这个函数只是单纯的添加成员，如果想有更多的功能，需要自己实现
 * */
LCUI_API void* Queue_Add( LCUI_Queue *queue, const void *data )
{
	return __Queue_Add( queue, data, FALSE ); 
}

/**
 * 功能：将新的成员添加至队列 
 * 说明：与Queue_Add函数不同，该函数只是修改指定位置的成员指针指向的地址，主要用
 * 与部件队列的处理上，有的部件需要从一个队列转移到另一个队列上，不重新分配内存空间，
 * 直接使用原来的内存地址，这是为了避免部件转移所在队列后，部件指针无效的问题。
 * */
LCUI_API void* Queue_AddPointer( LCUI_Queue *queue, const void *data )
{
	return __Queue_Add( queue, data, TRUE ); 
}

/** 将一个队列拼接至另一个队列的末尾 */
LCUI_API int Queue_Cat( LCUI_Queue *des, LCUI_Queue *src )
{
	int i,total;
	
	if( !des || !src ) {
		return -1;
	}
	
	total = Queue_GetTotal( src );
	for( i=0; i<total; ++i ) {
		Queue_AddPointer( des, Queue_Get( src, i ) );
	}
	return 0;
}

/* 检测队列是否为空 */
LCUI_API LCUI_BOOL
Queue_Empty( LCUI_Queue *queue )
{
	if(queue->total_num > 0) {
		return FALSE;
	}
	
	return TRUE;
}

/* 查找指定成员指针所在队列中的位置 */
LCUI_API int
Queue_Find( LCUI_Queue *queue, const void *p )
{
	void *tmp;
	int i, total; 
	
	total = Queue_GetTotal( queue );
	for(i=0; i<total; ++i) {
		tmp = Queue_Get( queue, i );
		if( tmp == p ) { 
			return i;
		}
	} 
	return -1;
}

/* 
 * 功能：从队列中删除一个成员，并重新排列队列
 * 说明：处理方式因flag的值而不同 
 * 返回值：正常返回真（1），出错返回假（0）
 * */
static LCUI_BOOL 
__Queue_Delete( LCUI_Queue *queue, int pos, LCUI_BOOL is_pointer )
{
	int i;
	void *save = NULL;
	LCUI_Node *temp, *p_src, *p_des;
	
	/* 有效性检测 */
	if( pos < 0 || pos >= queue->total_num || queue->total_num <= 0 ) {
		return FALSE;
	} 
	if( queue->data_mode == QUEUE_DATA_MODE_ARRAY ) {
		/* 需删除的成员的备份地址 */
		save = queue->data_array[pos];
		/* 减少当前队列总成员数量 */
		--queue->total_num;
		/* 移动排列各个成员位置，这只是交换指针的值，*/
		for (i=pos; i<queue->total_num; ++i) {
			queue->data_array[i] = queue->data_array[i+1]; 
		}
		/* 如果只要移除指针 */
		if( is_pointer ) {
			queue->data_array[i] = NULL;
			return TRUE;
		}
		/* 把需要删除的成员移至队列末尾 */
		queue->data_array[i] = save;
		/* 对该位置的成员进行析构处理 */
		if( queue->destroy_func ) {
			queue->destroy_func(save);
		}
		return TRUE;
	}
	/* 得到源位置的结点的指针 */
	p_src = queue->data_head_node.next;
	if( !p_src ) {
		return FALSE;
	}
	for(i=0; p_src->next && i<pos; ++i ) {
		p_src = p_src->next;
	} 
	/* 备份指针 */
	save = p_src->data;
	/* 如果后面还有结点 */
	if( p_src->next ) {
		/* 解除该位置的结点与前后结点的链接 */
		temp = p_src->prev;
		temp->next = p_src->next;
		p_src->next->prev = temp; 
		p_des = queue->data_head_node.next;
		if( p_des ) {
			/* 找到链表中最后一个结点 */
			while( p_des->next ) {
				p_des = p_des->next;
			}
			/* 把需删除的结点链接到链表尾部 */
			p_des->next = p_src;
			p_src->prev = p_des;
			p_src->next = NULL;
		}
	}
	--queue->total_num;
	if( is_pointer ) {
		p_src->data = NULL;
		return TRUE;
	} 
	/* 对该位置的成员进行析构处理 */
	if( queue->destroy_func ) {
		queue->destroy_func(save);
	}
	return TRUE;
}

/* 从队列中删除一个成员，并释放该成员占用的内存资源 */
LCUI_API int
Queue_Delete( LCUI_Queue *queue, int pos )
{
	return __Queue_Delete( queue, pos, FALSE );
}

/* 从队列中删除一个成员指针，不对该指针指向的内存进行释放 */
LCUI_API int
Queue_DeletePointer( LCUI_Queue *queue, int pos )
{
	return __Queue_Delete( queue, pos, TRUE );
}

//#define _NEED_TEST_QUEUE_
#ifdef _NEED_TEST_QUEUE_
/*
 * 下面有几个main函数，用于对本文件内的函数进行测试，你可以选择其中一个main函数，编译
 * 并运行，看看结果
 * */
#define test_4
#ifdef test_1
/* 测试Queue_Cat函数 */
int main()
{
	int i, total;
	char ch, str[20];
	LCUI_Queue q1, q2;
	/* 初始化 */
	Queue_Init(&q1, sizeof(char), NULL);
	Queue_Init(&q2, sizeof(char), NULL);
	Queue_SetDataMode(&q1, QUEUE_DATA_MODE_LINKED_LIST);
	Queue_SetDataMode(&q2, QUEUE_DATA_MODE_LINKED_LIST);
	/* 添加0至9的字符至队列 */
	for(i=0; i<10; i++) {
		ch = '0' + i; 
		Queue_Add(&q1, &ch);
		Queue_Add(&q2, &ch);
	}
	total = Queue_GetTotal( &q1 );
	for(i=0; i<total; i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );  
	}
	str[i] = 0;
	
	printf("before, string:%s\n", str);
	Queue_Cat( &q1, &q2 ); /* 拼接队列 */
	total = Queue_GetTotal( &q1 );
	for(i=0; i<total; i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );  
	}
	str[i] = 0;
	printf("after, string:%s\n\n", str);
	
	Queue_Destroy(&q1);
	Queue_Destroy(&q2);
	return 0;
}
#endif
#ifdef test_2
/* 
 * 功能：测试通用队列的成员指针增删功能
 * 说明：先从队列1中获取指定位置的成员指针，之后删除该位置的成员指针，把成员指针添加至
 * 队列2中。
 *  */
int main()
{
	int i;
	char *p, ch, str[11];
	LCUI_Queue q1, q2;
	/* 初始化 */
	Queue_Init(&q1, sizeof(char), NULL);
	Queue_Init(&q2, sizeof(char), NULL);
	Queue_SetDataMode(&q1, QUEUE_DATA_MODE_LINKED_LIST);
	/* 添加0至9的字符至队列 */
	for(i=0; i<10; i++) {
		ch = '0' + i; 
		Queue_Add(&q1, &ch);
	}
	/* 获取每个成员，并保存至字符串 */
	for(i=0; i<10; i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );  
	}
	str[i] = 0;
	
	printf("befor, string:%s\n", str);
	p = (char*)Queue_Get(&q1, 5);
	printf("delete char: %c\n", *p);
	Queue_DeletePointer(&q1, 5); 
	for(i=0; i<9; i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );
	}
	str[i] = 0;
	printf("after, string:%s\n\n", str);
	
	//Queue_AddPointer(&q2, p);
	Queue_InsertPointer( &q2, 0, p );
	printf("add char: %c\n", *p);
	for(i=0; i<Queue_GetTotal(&q2); i++) {
		str[i] = *( (char*)Queue_Get(&q2, i) );
	}
	str[i] = 0;
	printf("after, string:%s\n\n", str);
	
	int des_pos, src_pos;
	src_pos = 3;
	des_pos = 8;
	for(i=0; i<5; ++i) {
		p = (char*)Queue_Get(&q1, src_pos);
		printf("move char: %c, src pos: %d, des pos: %d\n", *p, src_pos, des_pos);
		Queue_Move(&q1, des_pos, src_pos);
	}
	for(i=0; i<Queue_GetTotal(&q1); i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );
	}
	str[i] = 0;
	printf("after, string:%s\n\n", str);
	
	
	Queue_Destroy(&q1);
	Queue_Destroy(&q2);
	return 0;
}
#endif
#ifdef test_3
/* 
 * 功能：测试通用队列的基本功能
 * 说明：此函数是将‘0’-’9‘的字符存入通用队列中，之后再取出保存至字符串中，最后打印字符串
 *  */
int main()
{
	int i;
	char ch, str[11];
	LCUI_Queue bq;
	/* 初始化 */
	Queue_Init(&bq, sizeof(char), NULL);
	/* 添加0至9的字符至队列 */
	for(i=0; i<10; i++) {
		ch = '0' + i;
		Queue_Add(&bq, &ch);
	}
	/* 获取每个成员，并保存至队列 */
	for(i=0; i<10; i++) {
		str[i] = *( (char*)Queue_Get(&bq, i) );  
	}
	str[i] = 0;
	
	printf("string:%s\n", str);
	
	Queue_Destroy(&bq);
	return 0;
}
#endif
#ifdef test_4
int main()
{
	clock_t start;
	int i, total;
	LCUI_Queue q1;
	/* 初始化 */
	Queue_Init(&q1, sizeof(int), NULL);
	//Queue_SetDataMode(&q1, QUEUE_DATA_MODE_LINKED_LIST);
	nobuff_printf( "queue add......" );
	start = clock();
	/* 添加0至9的字符至队列 */
	for(i=0; i<20000; i++) {
		Queue_Add(&q1, &i);
	}
	nobuff_printf( "%ld us\n", clock()-start );
	total = Queue_GetTotal( &q1 );
	nobuff_printf( "queue get......" );
	start = clock();
	for(i=0; i<total; i++) {
		Queue_Get(&q1, i);  
	}
	nobuff_printf( "%ld us\n", clock()-start );
	Queue_Destroy(&q1);
	return 0;
}
#endif
#endif
