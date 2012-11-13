/* ***************************************************************************
 * LCUI_Queue.c -- basic queue processing
 * 
 * Copyright (C) 2012 by
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
 * 版权所有 (C) 2012 归属于 
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
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_WIDGET_H 
#include LC_ERROR_H
#include <unistd.h>

/************************ LCUI_Queue **********************************/
int Queue_Lock (LCUI_Queue *queue)
/* 功能：锁定一个队列，使之只能被一个线程使用 */
{ 
	return thread_mutex_lock(&queue->lock);
}

int Queue_UnLock (LCUI_Queue *queue)
/* 功能：解开队列锁 */
{
	return thread_mutex_unlock(&queue->lock);
}

int Queue_Using (LCUI_Queue * queue, int mode) 
/* 
 * 功能：设定队列的状态为“使用” 
 * 说明：参数mode需要是QUEUE_MODE_READ和QUEUE_MODE_WRITE这两种之一。
 * QUEUE_MODE_READ 表示“读”模式，由于是读，可以和其它以“读”模式访问本队列的线程共享。
 * QUEUE_MODE_WRITE 表示“写”模式，写只能由一个线程写，其它线程既不能读也不能写。
 * */
{ 
	if(mode == QUEUE_MODE_READ) {
	//	printf("use, queue: %p, mode: read\n", queue);
		return thread_rwlock_rdlock(&queue->lock); 
	} else {
		//printf("use, queue: %p, mode: write\n", queue);
		return thread_rwlock_wrlock(&queue->lock);  
	}
}

int Queue_End_Use (LCUI_Queue * queue) 
/* 功能：储存矩形数据的队列为空闲状态 */
{
	//switch(queue->lock.status) {
	    //case RWLOCK_WRITE:printf("end use, queue: %p, befor mode: write\n", queue);break;
	    //case RWLOCK_READ:printf("end use, queue: %p, befor mode: read\n", queue);break;
	    //case RWLOCK_FREE:printf("end use, queue: %p, befor mode: free\n", queue);break;
	//}
	return thread_rwlock_unlock(&queue->lock); 
}

void Queue_Init (LCUI_Queue * queue, size_t element_size, void (*func) ()) 
/* 功能：初始化队列 */
{
	thread_rwlock_init(&queue->lock);
	queue->member_type	= 0;
	queue->data_mode	= 0;
	queue->data_array	= NULL;
	queue->data_head_node.data = NULL;
	queue->data_head_node.prev = NULL;
	queue->data_head_node.next = NULL;
	queue->total_num	= 0;
	queue->max_num		= 0;
	queue->element_size	= element_size;
	queue->destroy_func	= func; 
}

void Queue_Using_Pointer(LCUI_Queue * queue)
/* 
 * 功能：设定队列成员类型为指针 
 * 说明：如果队列只是用于存放指针，并且不希望队列销毁后，指针指向的内存空间也被释放，可
 * 使用该函数设置。
 * */
{
	queue->member_type = 1;
}

int Queue_Get_Total(LCUI_Queue * queue)
/* 说明：获取队列当前总成员数量 */
{
	return queue->total_num;
}

int Queue_Set_DataMode(LCUI_Queue * queue, Queue_DataMode mode)
/* 
 * 功能：设定队列使用的数据储存模式
 * 说明：只能在初始化后且未加入成员时使用该函数
 * */
{
	if(queue->total_num > 0 || queue->max_num > 0) {
		return -1;
	}
	queue->data_mode = 1;
	return 0;
}


int Queue_Swap(LCUI_Queue * queue, int pos_a, int pos_b)
/* 功能：交换队列中指定位置两个成员的位置 */
{
	void *temp;
	if (pos_a < queue->total_num && pos_b < queue->total_num 
	 && pos_a != pos_b && queue->total_num > 0) {
		if(queue->data_mode == 0) {
			temp = queue->data_array[pos_a];
			queue->data_array[pos_a] = queue->data_array[pos_b];
			queue->data_array[pos_b] = temp;
		} else {
			int i, pos;
			LCUI_Node *a, *b, *temp;
			if(pos_a < pos_b) {
				pos = pos_a;
			} else {
				pos = pos_b;
			}
			a = &queue->data_head_node;
			for(i=0; i<=pos; ++i) {
				a = a->next;
			}
			if(pos_a < pos_b) {
				pos = pos_b;
			}
			b = &queue->data_head_node;
			for(i=0; i<=pos; ++i) {
				b = b->next;
			}
			/* 交换两个结点的指向上结点的指针 */
			temp = a->prev;
			a->prev = b->prev;
			b->prev = temp; 
			/* 交换两个结点的指向下结点的指针 */
			temp = a->next;
			a->next = b->next;
			b->next = temp;
		}
		return 0;
	}
	return -1;
}

void Destroy_Queue(LCUI_Queue * queue) 
/* 功能：释放队列占用的内存资源 */
{ 
	int i;
	if(queue->member_type == 0) {
	/* 如果成员是普通类型，释放队列成员占用的内存空间 */ 
		while( Queue_Delete(queue, 0) );/* 清空队列成员 */
		if(queue->data_mode == 0) {
			for(i=0; i<queue->max_num; ++i) {
				free( queue->data_array[i] ); 
			}
			free (queue->data_array);/* 释放二维指针占用的的内存空间 */ 
		} else {
			LCUI_Node *p, *obj;
			p = queue->data_head_node.next;
			/* 切换到下个结点后，把上一个结点销毁 */
			while( p ) {
				obj = p;
				p = p->next;
				free( obj );
			}
			queue->data_head_node.next = NULL;
		}
	}
	
	queue->data_array	= NULL;
	queue->total_num	= 0;
	queue->max_num		= 0;
	thread_rwlock_destroy(&queue->lock);
}


void * Queue_Get (LCUI_Queue * queue, int pos)
/* 
 * 功能：从队列中获取指定位置的成员 
 * 说明：成功返回指向该成员的指针，失败返回NULL
 * 注意：请勿对返回的指针进行free操作
 * */
{
	void  *data = NULL;
	
	if( pos < 0) {
		return NULL;
	} 
	Queue_Using (queue, QUEUE_MODE_READ);
	if (queue->total_num > 0 && pos < queue->total_num) {
		if(queue->data_mode == 0) {
			data = queue->data_array[pos]; 
		} else {
			int i;
			LCUI_Node *p;
			p = queue->data_head_node.next;
			for(i=0; i<= pos && p!=NULL; ++i) {
				data = p->data;
				p = p->next;
			}
		}
	}
	Queue_End_Use (queue);
	return data;
}

int Queue_Insert( LCUI_Queue * queue, int pos, const void *data)
/* 功能：向队列中指定位置插入成员 */
{
	int src_pos;
	src_pos = Queue_Add(queue, data);
	return Queue_Move(queue, pos, src_pos); 
}

int Queue_Move(LCUI_Queue *queue, int des_pos, int src_pos)
/* 功能：将队列中指定位置的成员移动至目的位置 */
{
	void *temp;
	int i, total;
	
	total = Queue_Get_Total(queue);
	if(des_pos < 0 || des_pos > total 
	|| src_pos < 0 || src_pos > total ) {
		return -1;
	}
	//printf("des pos: %d, src pos: %d, total: %d\n", des_pos, src_pos, queue->total_num);
	if(des_pos == src_pos ) {
		return 0;
	}
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	if(queue->data_mode == 0) {
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
	} else {  
		LCUI_Node *temp, *p_src, *p_des;
		/* 得到源位置的结点的指针 */
		p_src = queue->data_head_node.next;
		for(i=0; p_src->next && i<src_pos; ++i ) {
			p_src = p_src->next;
		} 
		/* 解除该位置的结点与前后结点的链接 */
		temp = p_src->prev;
		temp->next = p_src->next;
		if(p_src->next != NULL) {
			p_src->next->prev = temp;
		}
		/* 得到目标位置的结点的指针 */
		p_des = queue->data_head_node.next;
		if(des_pos < src_pos) {
			/* 目标位置的结点接后面 */
			for(i=0; p_des->next && i<des_pos; ++i ) {
				p_des = p_des->next;
			} 
			//printf("des: %c, pos: %d\n", *(char*)p_des->data, des_pos);
			//printf("src: %c, pos: %d\n", *(char*)p_src->data, src_pos);
			/* 新结点链接 前结点 */
			temp = p_des->prev;
			temp->next = p_src; 
			p_src->prev = temp;
			/* 新结点链接 后结点 */
			p_src->next = p_des; 
			p_des->prev = p_src;
		} else {
			des_pos -= 1; /* 数量有变动，目标位置在源位置后面，位置需向前移1个单位 */
			/* 目标位置的结点接前面 */
			for(i=0; p_des->next && i<des_pos; ++i ) {
				//printf("data: %c, pos: %d\n", *(char*)p_des->data, i);
				p_des = p_des->next;
			}
			//printf("i: %d\n", i);
			//printf("des: %c, pos: %d\n", *(char*)p_des->data, des_pos);
			//printf("src: %c, pos: %d\n", *(char*)p_src->data, src_pos);
			temp = p_des->next;
			p_des->next = p_src;
			p_src->next = temp;
			p_src->prev = p_des;
		}
	}
	Queue_End_Use(queue);
	return 0;
}


int Queue_Replace_By_Flag(LCUI_Queue * queue, int pos, const void *data, int flag)
/* 功能：覆盖队列中指定位置的成员 */
{
	int i, total;
	total = Queue_Get_Total(queue);
	if(pos >= total) {	/* 如果超出队列范围 */
		return -1;
	}
	
	if(queue->data_mode == 0) {
		/* 
		 * 考虑到队列成员有时会是结构体，并且结构体成员中可能会有指针，为了避免因重复覆盖不
		 * 对指针进行释放而导致的内存溢出，需要先调用析构函数对该成员进行销毁，因为析构函数
		 * 一般会对结构体中的指针进行释放，之后，再复制新成员的数据至该成员的内存空间。
		 *  */
		if(NULL != queue->destroy_func) {
			queue->destroy_func(queue->data_array[pos]); 
		}
		Queue_Using(queue, QUEUE_MODE_WRITE);
		if(flag == 1) {
			memcpy(queue->data_array[pos], data, queue->element_size);
		} else {
			/* 拷贝指针 */
			memcpy(&queue->data_array[pos], &data, sizeof(void*));
		}
		Queue_End_Use(queue);
	} else {
		LCUI_Node *p;
		
		p = queue->data_head_node.next;
		for(i=0; p->next && i<pos; ++i ) {
			p = p->next;
		}
		if(NULL != queue->destroy_func) {
			queue->destroy_func( p->data ); 
		}
		Queue_Using(queue, QUEUE_MODE_WRITE);
		if(flag == 1) {
			memcpy(p->data, data, queue->element_size); 
		} else { 
			memcpy(&p->data, &data, sizeof(void*));
		}
		Queue_End_Use(queue);
	}
	return 0;
}

int Queue_Replace(LCUI_Queue * queue, int pos, const void *data)
/* 功能：覆盖队列中指定位置的成员内存空间里的数据 */
{
	return Queue_Replace_By_Flag(queue, pos, data, 1);
}

int Queue_Replace_Pointer(LCUI_Queue * queue, int pos, const void *data)
/* 功能：覆盖队列中指定位置的成员指针 */
{
	return Queue_Replace_By_Flag(queue, pos, data, 0);
}

static int Queue_Add_By_Flag(LCUI_Queue * queue, const void *data, int flag)
/* 
 * 功能：将新的成员添加至队列 
 * 说明：是否为新成员重新分配内存空间，由参数flag的值决定
 * 返回值：正常则返回在队列中的位置，错误则返回非0值
 * */
{
	int i, pos;
	size_t size; 
	LCUI_Node *p, *q;
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	
	pos = queue->total_num;
	++queue->total_num;
	/* 如果数据是以数组形式储存 */
	if(queue->data_mode == QUEUE_DATA_MODE_ARRAY) {
		if(queue->total_num > queue->max_num) {
		/* 如果当前总数大于之前最大的总数 */
			queue->max_num = queue->total_num;
			size = sizeof(void*) * queue->total_num;
			/* 如果总数大于1，说明之前已经malloc过，直接realloc扩增内存 */
			if (queue->total_num > 1 && queue->data_array != NULL) { 
				queue->data_array = (void **) 
					realloc( queue->data_array, size ); 
			} else {
				queue->data_array = (void **) malloc (sizeof(void*)); 
			}
			
			if(NULL == queue->data_array) {
				printf("Queue_Add_By_Flag(): "ERROR_MALLOC_ERROR);
				Queue_End_Use(queue);
				exit(-1);
			}
			if (flag == 1) {
				queue->data_array[pos] = malloc(queue->element_size);
			}
		}
		else if (flag == 1 && queue->data_array[pos] == NULL) { 
			queue->data_array[pos] = 
					malloc(queue->element_size);
		}
		
		if(flag == 1) {
			memcpy(queue->data_array[pos], data, 
				queue->element_size);
		} else {
			memcpy(&queue->data_array[pos], &data, sizeof(void*));
		}
	} else {/* 否则，数据是以链表形式储存 */  
		DEBUG_MSG("new total_num: %d\n", queue->total_num);
		if(queue->total_num > queue->max_num) {
			p = &queue->data_head_node;
			DEBUG_MSG("head_node: %p, next: %p\n", p, p->next);
			while ( p->next ) {
				p = p->next;
			} 
			q = (LCUI_Node*) malloc (sizeof(LCUI_Node)); 
			q->prev = p;
			q->next = NULL; 
			p->next = q;
			queue->max_num = queue->total_num;
		}
		p = queue->data_head_node.next; 
		for(i=0; p->next && i<pos; ++i ) {
			p = p->next;
		} 
		if(flag == 1) { 
			p->data = malloc ( queue->element_size );
			memcpy( p->data, data, queue->element_size );
		} else {
			memcpy( &p->data, &data, sizeof(void*) );
		} 
	}
	/* 
	 * total_num自增1，但不大于max_num，那么，就有现成的内存空间可用，直接
	 * 调用memcpy函数拷贝数据进去即可。因为Queue_Delete函数并不会释放成员
	 * 占用的内存空间，最多也只是将成员里的指针指向的内存空间释放。
	 *  */ 
	Queue_End_Use(queue);
	return pos;
}


int Queue_Add(LCUI_Queue * queue, const void *data) 
/* 
 * 功能：将新的成员添加至队列 
 * 说明：这个函数只是单纯的添加成员，如果想有更多的功能，需要自己实现
 * */
{
	return Queue_Add_By_Flag(queue, data, 1); 
}


int Queue_Add_Pointer(LCUI_Queue * queue, const void *data)
/* 
 * 功能：将新的成员添加至队列 
 * 说明：与Queue_Add函数不同，该函数只是修改指定位置的成员指针指向的地址，主要用
 * 与部件队列的处理上，有的部件需要从一个队列转移到另一个队列上，不重新分配内存空间，
 * 直接使用原来的内存地址，这是为了避免部件转移所在队列后，部件指针无效的问题。
 * */
{
	return Queue_Add_By_Flag(queue, data, 0); 
}

int Queue_Empty(LCUI_Queue *queue)
/* 功能：检测队列是否为空 */
{
	if(queue->total_num > 0) {
		return 0;
	}
	
	return 1;
}

static BOOL Queue_Delete_By_Flag(LCUI_Queue * queue, int pos, int flag) 
/* 
 * 功能：从队列中删除一个成员，并重新排列队列
 * 说明：处理方式因flag的值而不同 
 * 返回值：正常返回真（1），出错返回假（0）
 * */
{
	int i;
	void *save = NULL;
	
	Queue_Using (queue, QUEUE_MODE_WRITE); 
	/* 有效性检测 */
	if (pos >=0 && pos < queue->total_num && queue->total_num > 0);
	else {
		Queue_End_Use (queue);
		return FALSE;
	} 
	if(queue->data_mode == QUEUE_DATA_MODE_ARRAY) {
		save = queue->data_array[pos];/* 备份地址 */
		/* 移动排列各个成员位置，这只是交换指针的值，把需要删除的成员移至队列末尾 */
		for (i = pos; i < queue->total_num - 1; ++i) {
			queue->data_array[i] = queue->data_array[i + 1]; 
		}

		if(flag == 1) {
			queue->data_array[i] = save;
			memset(queue->data_array[i], 0, queue->element_size);
		} else {
			queue->data_array[i] = NULL;
		}
	} else {
		LCUI_Node *temp, *p_src, *p_des;
		/* 得到源位置的结点的指针 */
		p_src = queue->data_head_node.next;
		if(p_src == NULL) {
			return FALSE;
		}
		for(i=0; p_src->next && i<pos; ++i ) {
			p_src = p_src->next;
		} 
		/* 备份指针 */
		save = p_src->data;
		/* 解除该位置的结点与前后结点的链接 */
		temp = p_src->prev;
		temp->next = p_src->next;
		if(p_src->next != NULL)  {
			p_src->next->prev = temp; 
		}
		p_des = queue->data_head_node.next;
		if(p_des == NULL) {
			return FALSE;
		}
		/* 找到链表中最后一个结点 */
		for(i=0; p_des->next; ++i ) {
			p_des = p_des->next;
		}
		/* 把需删除的结点链接到链表尾部 */
		p_des->next = p_src;
		p_src->prev = p_des;
		p_src->next = NULL;

		if(flag == 1) { 
			memset(p_src->data, 0, queue->element_size);
		} else {
			p_src->data = NULL;
		}
	} 
	/* 
	 * 如果是使用本函数转移队列成员至另一个队列，该队列成员还是在同一个内存空间，
	 * 只不过，记录该成员的内存地址的队列不同。这种操作，本函数不会在源队列中保留
	 * 该成员的地址，因为源队列可能会被销毁，销毁时也会free掉队列中每个成员，而
	 * 目标队列未被销毁，且正在使用之前转移过来的成员，这会产生错误。
	 *  */ 
	--queue->total_num; 
	
	Queue_End_Use (queue);
	if(flag == 1) { 
		/* 对该位置的成员进行析构处理 */
		if(NULL != queue->destroy_func) {
			queue->destroy_func(save);
		} 
		/* 不需要释放内存，只有在调用Destroy_Queue函数时才全部释放 */
		//free(save); 
	}
	return TRUE;
}

int Queue_Delete (LCUI_Queue * queue, int pos)
/* 功能：从队列中删除一个成员，并释放该成员占用的内存资源 */
{
	return Queue_Delete_By_Flag(queue, pos, 1);
}

int Queue_Delete_Pointer (LCUI_Queue * queue, int pos) 
/* 功能：从队列中删除一个成员指针，不对该指针指向的内存进行释放 */
{
	return Queue_Delete_By_Flag(queue, pos, 0);
}

//#define _NEED_TEST_QUEUE_
#ifdef _NEED_TEST_QUEUE_
/*
 * 下面有两个main函数，用于对本文件内的函数进行测试，你可以选择其中一个main函数，编译
 * 并运行，看看结果
 * */
#define test_1
#ifdef test_1
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
	Queue_Set_DataMode(&q1, QUEUE_DATA_MODE_LINKED_LIST);
	/* 添加0至9的字符至队列 */
	for(i=0; i<10; i++) {
		ch = '0' + i; 
		Queue_Add(&q1, &ch);
	}
	/* 获取每个成员，并保存至队列 */
	for(i=0; i<10; i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );  
	}
	str[i] = 0;
	
	printf("befor, string:%s\n", str);
	p = (char*)Queue_Get(&q1, 5);
	printf("delete char: %c\n", *p);
	Queue_Delete_Pointer(&q1, 5); 
	for(i=0; i<9; i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );
	}
	str[i] = 0;
	printf("after, string:%s\n\n", str);
	
	Queue_Add_Pointer(&q2, p);
	printf("add char: %c\n", *p);
	for(i=0; i<Queue_Get_Total(&q2); i++) {
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
	for(i=0; i<Queue_Get_Total(&q1); i++) {
		str[i] = *( (char*)Queue_Get(&q1, i) );
	}
	str[i] = 0;
	printf("after, string:%s\n\n", str);
	
	
	Destroy_Queue(&q1);
	Destroy_Queue(&q2);
	return 0;
}
#endif
#ifdef test_2
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
	
	Destroy_Queue(&bq);
	return 0;
}
#endif
#endif


/************************ LCUI_Queue End ******************************/

/************************** WidgetQueue ********************************/

static void Destroy_Widget(LCUI_Widget *widget)
/*
 * 功能：销毁一个部件
 * 说明：如果这个部件有子部件，将对它进行销毁
 * */
{
	widget->parent = NULL;
	
	/* 释放字符串 */
	String_Free(&widget->type);
	String_Free(&widget->style);
	
	/* 释放储存图形数据的结构体 */
	Graph_Free(&widget->graph);
	Graph_Free(&widget->background_image);
	
	/* 销毁部件的队列 */
	Destroy_Queue(&widget->child);
	Destroy_Queue(&widget->event);
	Destroy_Queue(&widget->data);
	Destroy_Queue(&widget->update_area);
	
	widget->visible = FALSE;
	widget->enabled = TRUE;
	
	void (*func)(LCUI_Widget*);
	func = Get_WidgetFunc_By_ID(widget->type_id, 
				FUNC_TYPE_DESTROY);
	func(widget); /* 调用之，释放private指针指向的内存空间 */
	free(widget->private_data);/* 释放这个指向部件私有数据结构体的指针 */
}

void WidgetQueue_Init(LCUI_Queue *queue)
/* 功能：初始化部件队列 */
{
	Queue_Init(queue, sizeof(LCUI_Widget), Destroy_Widget);
}

int WidgetQueue_Get_Pos(LCUI_Queue *queue, LCUI_Widget *widget)
/* 功能：从部件队列中获取指定部件的排列位置 */
{
	LCUI_Widget *temp;
	int i, result = -1, total; 
	
	total = Queue_Get_Total(queue); 
	for(i = 0; i < total; ++i) {
		temp = Queue_Get(queue, i);
		if(temp == widget) { 
			result = i; 
			break;
		}
	} 
	return result;
}


int WidgetQueue_Move(LCUI_Queue *queue, int pos, LCUI_Widget *widget)
/* 
 * 功能：移动指定部件在队列中的位置
 * 返回值：成功则返回部件的新位置，失败返回-1
 * */
{
	LCUI_Widget *temp;
	int total, i;//, j, des_pos;
	total = Queue_Get_Total(queue);
	for(i=0; i<total; ++i) {
		temp = (LCUI_Widget*)Queue_Get(queue, i);
		if(temp == widget) {/* 如果找到了部件 */
			break;
		}
	}
	return Queue_Move( queue, pos, i );
	
/* 现在暂时不需要“总是保持在前端显示” 这个功能 */
#ifdef use_this_old_code
	j = i;
	for(i=0; i<j; ++i) {/* 从头到当前位置遍历队列 */
		temp = (LCUI_Widget*)Queue_Get(queue, i);
		
		if( temp->lock_display ) {
		/* 如果该位置的部件锁定了位置 */
			if( widget->lock_display ) {
			/* 如果目标部件锁定了位置，那就可以移动至最前端 */
				des_pos = 0; 
				Queue_Using(queue, QUEUE_MODE_WRITE);
				for (i=j; i > des_pos; --i) {
					queue->data_array[i] = queue->data_array[i - 1]; 
				}
				queue->data_array[des_pos] = widget; 
				Queue_End_Use(queue);
				break;
			}
		} else {/* 否则，该位置的部件没锁定位置 */
			des_pos = i;
			Queue_Using(queue, QUEUE_MODE_WRITE);
			for (i=j; i > des_pos; --i) {
				queue->data_array[i] = queue->data_array[i - 1];  
			}
			queue->data_array[des_pos] = widget; 
			Queue_End_Use(queue); 
			break;
		}
	}
	return -1;
#endif
}
/************************ WidgetQueue End ******************************/


/************************* RectQueue **********************************/
void RectQueue_Init(LCUI_Queue *queue)
/* 功能：初始化储存矩形数据的队列 */
{
	/* 由于LCUI_Rect结构体中的成员没有指针，因此，不需要释放指针指向的内存，也就不需要析构函数 */
	Queue_Init(queue, sizeof(LCUI_Rect), NULL);
}

int RectQueue_Get( LCUI_Rect * rect, int pos, LCUI_Queue * queue)
/* 功能：从队列指定位置中获取一个矩形数据 */
{
	void *temp;
	temp = Queue_Get(queue, pos);
	if(NULL == temp) {
		return 0;
	}
	*rect = *((LCUI_Rect*)temp);
	return 1;
}

void Queue_Copy(LCUI_Queue *des, LCUI_Queue *src)
/* 功能：将源队列里的全部成员拷贝追加至目标队列里 */
{
	LCUI_Rect *rect;
	int i, total;
	total = Queue_Get_Total(src);
	for(i=0; i<total; ++i) {
		rect = (LCUI_Rect *)Queue_Get(src, i);/* 获取源队列里的成员 */
		//printf("[%d] rect: %d,%d, %d,%d\n", i, rect->x, rect->y, rect->width, rect->height);
		RectQueue_Add(des, *rect); /* 添加至目标队列里 */
	}
}
 
int RectQueue_Add (LCUI_Queue * queue, LCUI_Rect rect) 
/* 功能：将矩形数据追加至队列 */
{ 
	int i, flag = 0;
	LCUI_Rect t_rect; 
	LCUI_Queue rect_buff;
	
	//if(debug_mark)
	//	printf("New : [%d,%d] %d,%d\n", rect.x, rect.y, rect.width, rect.height);
	
	if(!Rect_Valid(rect)) {
		//printf("not valid\n");
		return -1;
	}
	
	RectQueue_Init(&rect_buff);
	
	for (i = 0; i < queue->total_num; ++i) {
		if(RectQueue_Get(&t_rect, i, queue)) {
			//if(debug_mark)
			//  printf("temp : [%d,%d] %d,%d\n", t_rect.x, t_rect.y, t_rect.width, t_rect.height);
			
			if (!Rect_Valid(t_rect)) {
			/* 删除这个矩形数据，因为它是无效的 */
				Queue_Delete (queue, i); 
			} else if (Rect_Include_Rect (rect, t_rect)) {
			/* 删除这个矩形数据，因为它已经被新增的矩形区域包含 */
				Queue_Delete (queue, i); 
			} else if (Rect_Include_Rect (t_rect, rect)) {
			/* 如果新增的矩形数据与已存在的矩形数据属于包含关系 */
				//if(debug_mark) 
				//  printf("Include 2\n");
				  
				flag = 1;
				break;
			} else if(Rect_Equal(rect, t_rect)) {
			/* 相等的就不需要了 */
				//if(debug_mark)
				//  printf("Equal || not valid\n");
				
				flag = 1;
				break;
			} else if(Rect_Is_Overlay(rect, t_rect)) {
				/* 如果新增的矩形与队列中的矩形重叠 */ 
				/* 将矩形分离成若干个不重叠的矩形，之后将它们添加进去 */
				//printf("Rect_Is_Overlay(rect, t_rect)\n");
				Cut_Overlay_Rect(t_rect, rect, &rect_buff);
				//debug_mark = 1;
				Queue_Copy(queue, &rect_buff);
				//debug_mark = 0;
				flag = 1;
				break;
			}
		} else {
			break;
		}
	}
	
	Destroy_Queue(&rect_buff);
	if (flag == 0) { /* 没有的话，就需要添加至队列 */ 
		return Queue_Add(queue, &rect);
	}
	/* 销毁队列 */
	//if(debug_mark) 
	//	printf("done\n");
	return -1;
}
/************************* RectQueue end *******************************/

