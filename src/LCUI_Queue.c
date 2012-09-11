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
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_MEM_H
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
	if(mode == QUEUE_MODE_READ) 
		return thread_rwlock_rdlock(&queue->lock); 
	else 
		return thread_rwlock_wrlock(&queue->lock);  
}

int Queue_End_Use (LCUI_Queue * queue) 
/* 功能：储存矩形数据的队列为空闲状态 */
{
	return thread_rwlock_unlock(&queue->lock); 
}


void Queue_Init (LCUI_Queue * queue, size_t element_size, void (*func) ()) 
/* 功能：初始化队列 */
{
	thread_rwlock_init(&queue->lock);
	queue->member_type  = 0;   
	queue->queue		= NULL;
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

int Queue_Swap(LCUI_Queue * queue, int pos_a, int pos_b)
/* 功能：交换队列中指定位置两个成员的值 */
{
	void *temp;
	if (pos_a < queue->total_num 
		&& pos_b < queue->total_num 
		&& queue->total_num > 0) {
		temp = queue->queue[pos_a];
		queue->queue[pos_a] = queue->queue[pos_b];
		queue->queue[pos_b] = temp;
		return 0;
	}
	return -1;
}

int Queue_Delete_By_Flag (LCUI_Queue * queue, int pos, int flag) 
/* 
 * 功能：从队列中删除一个成员，并重新排列队列
 * 说明：处理方式因flag的值而不同 
 * 返回值：正常返回真（1），出错返回假（0）
 * */
{
	int i, value = 0;
	void *save = NULL;
	Queue_Using (queue, QUEUE_MODE_WRITE);
	if (pos >=0 && pos < queue->total_num 
		&& queue->total_num > 0) {
		save = queue->queue[pos];/* 备份地址 */
		for (i = pos; i < queue->total_num - 1; ++i)
		/* 移动排列各个成员位置，这只是交换指针的值，把需要删除的成员移至队列末尾 */
			queue->queue[i] = queue->queue[i + 1]; 
			
		if(flag == 1) {/* 被删除的队列成员的内存地址移动到队列末尾 */
			queue->queue[i] = save;
			memset(queue->queue[i], 0, queue->element_size);
		}
		else queue->queue[i] = NULL;
		/* 
		 * 如果是使用本函数转移队列成员至另一个队列，该队列成员还是在同一个内存空间，
		 * 只不过，记录该成员的内存地址的队列不同。这种操作，本函数不会在源队列中保留
		 * 该成员的地址，因为源队列可能会被销毁，销毁时也会free掉队列中每个成员，而
		 * 目标队列未被销毁，且正在使用之前转移过来的成员，这会产生错误。
		 *  */
		/* 减少成员总数 */
		--queue->total_num;
		value = 1;
	} 
	Queue_End_Use (queue);
	if(flag == 1) { 
		/* 对该位置的成员进行析构处理 */
		if(NULL != queue->destroy_func) 
			queue->destroy_func(save); 
		/* 不需要释放内存，只有在调用Destroy_Queue函数时才全部释放 */
		//free(save); 
	}
	return value;
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


void Destroy_Queue(LCUI_Queue * queue) 
/*
 * 功能：释放队列占用的内存资源 
 */
{ 
	int i;
	if(queue->member_type == 0) {
	/* 如果成员是普通类型，释放队列成员占用的内存空间 */
		while(Queue_Delete(queue, 0));/* 清空队列成员 */ 
		for(i=0; i<queue->max_num; i++)
			free(queue->queue[i]); 
	}
	free (queue->queue);/* 释放二维指针占用的的内存空间 */ 
	
	queue->queue		= NULL;
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
	Queue_Using (queue, QUEUE_MODE_READ);
	if (queue->total_num > 0 && pos < queue->total_num) 
		data = queue->queue[pos]; 
	Queue_End_Use (queue);
	return data;
}

int Queue_Insert(const void *data, int pos, LCUI_Queue * queue)
/* 功能：向队列中指定位置插入成员 */
{
	int i, total;
	size_t size;
	total = Queue_Get_Total(queue);
	if(pos > total) 
		return -1; 
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	/* 没有的话，就需要添加至队列 */
	++queue->total_num;
	if(queue->total_num > queue->max_num) {
	/* 如果如果当前数量大于当前容量，就扩增用于储存数据的内存空间 */
		size =  sizeof(void*) * queue->total_num;
		queue->queue = (void**)realloc( queue->queue, size );
		if(queue->queue == NULL) {
		/* 如果重新分配内存失败了 */
			Queue_End_Use(queue);
			printf("Queue_Insert(): "ERROR_MALLOC_ERROR);
			exit(-1);
		}
		/* 为成员分配内存空间 */
		queue->queue[total] = malloc(queue->element_size);
		queue->max_num = queue->total_num;
	}
	/* 平移队列中的成员 */
	for(i = queue->total_num - 1; i > pos; --i)
		memcpy(queue->queue[i], queue->queue[i-1], queue->element_size);
	/* 将值储存至队列 */
	memcpy(queue->queue[pos], data, queue->element_size);
	
	Queue_End_Use(queue);
	return 0;
}

int Queue_Move(LCUI_Queue *queue, int des_pos, int src_pos)
/* 功能：将队列中指定位置的成员移动至目的位置 */
{
	void *temp;
	int i, total;
	
	total = Queue_Get_Total(queue);
	if(des_pos < 0 || des_pos > total 
	|| src_pos < 0 || src_pos > total )
		return -1;
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	temp = queue->queue[src_pos];
	if (src_pos > des_pos) {
	/* 如果新位置在原位置的前面，把两位置之间的成员向右移动 */
		for (i = src_pos; i > des_pos; --i) 
			queue->queue[i] = queue->queue[i - 1];  
	} else if (src_pos < des_pos) {
	/* 如果新位置在原位置的后面，把两位置之间的成员向左移动 */
		for (i = src_pos; i < des_pos; ++i) 
			queue->queue[i] = queue->queue[i + 1];  
	} 
	
	queue->queue[des_pos] = temp;
	Queue_End_Use(queue);
	return 0;
}


int Queue_Replace(LCUI_Queue * queue, int pos, const void *data)
/* 功能：覆盖队列中指定位置的成员 */
{
	int total;
	total = Queue_Get_Total(queue);
	if(pos >= total)	/* 如果超出队列范围 */
		return -1;
		
	Queue_Using(queue, QUEUE_MODE_WRITE);
	
	/* 
	 * 考虑到队列成员有时会是结构体，并且结构体成员中可能会有指针，为了避免因重复覆盖不
	 * 对指针进行释放而导致的内存溢出，需要先调用析构函数对该成员进行销毁，因为析构函数
	 * 一般会对结构体中的指针进行释放，之后，再复制新成员的数据至该成员的内存空间。
	 *  */
	if(NULL != queue->destroy_func) 
		queue->destroy_func(queue->queue[pos]); 
		
	memcpy(queue->queue[pos], data, queue->element_size);
	Queue_End_Use(queue);
	return 0;
}

int Queue_Add_By_Flag(LCUI_Queue * queue, void *data, int flag)
/* 
 * 功能：将新的成员添加至队列 
 * 说明：是否为新成员重新分配内存空间，由参数flag的值决定
 * 返回值：正常则返回在队列中的位置，错误则返回非0值
 * */
{
	int pos;
	
	Queue_Using(queue, QUEUE_MODE_WRITE);
	
	pos = queue->total_num;
	queue->total_num += 1;					/* 总数+1 */
	if(queue->total_num > queue->max_num) {
	/* 如果当前总数大于之前最大的总数 */
		queue->max_num = queue->total_num;
		/* 如果总数大于1，说明之前已经malloc过，直接realloc扩增内存 */
		if (queue->total_num > 1 && queue->queue != NULL) 
			queue->queue =	(void **) realloc( queue->queue, 
					sizeof(void*) * queue->total_num ); 
		else queue->queue = (void **) malloc (sizeof(void*)); 
		
		if(NULL == queue->queue) {
			printf("Queue_Add_By_Flag(): "ERROR_MALLOC_ERROR);
			Queue_End_Use(queue);
			exit(-1);
		}
		/* 为该位置的成员分配内存空间 */
		if (flag == 1)
			queue->queue[pos] = malloc(queue->element_size);
	} else if(flag == 1) {
		/* 
		 * 转移队列成员的地址至另一个队列后，源队列中的多余指针会赋为NULL，所以需要
		 * 重新分配内存。
		 *  */ 
		if(queue->queue[pos] == NULL)
			queue->queue[pos] = malloc(queue->element_size);
	}
	/* 
	 * total_num自增1，但不大于max_num，那么，就有现成的内存空间可用，直接
	 * 调用memcpy函数拷贝数据进去即可。因为Queue_Delete函数并不会释放成员
	 * 占用的内存空间，最多也只是将成员里的指针指向的内存空间释放。
	 *  */
	
	if(flag == 1)  
		/* 直接用等号赋值的话，编译器会有警告信息，用memcpy函数即可 */
		memcpy(queue->queue[pos], data, queue->element_size);  
	else
		queue->queue[pos] = data;
		
	Queue_End_Use (queue);
	return pos;
}

int Queue_Add(LCUI_Queue * queue, void *data) 
/* 
 * 功能：将新的成员添加至队列 
 * 说明：这个函数只是单纯的添加成员，如果想有更多的功能，需要自己实现
 * */
{
	return Queue_Add_By_Flag(queue, data, 1); 
}


int Queue_Add_Pointer(LCUI_Queue * queue, void *data)
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
	if(queue->total_num > 0)
		return 0;
		
	return 1;
}

//#define _NEED_TEST_QUEUE_
#ifdef _NEED_TEST_QUEUE_
/*
 * 下面有两个main函数，用于对本文件内的函数进行测试，你可以选择其中一个main函数，编译
 * 并运行，看看结果
 * */


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
	/* 添加0至9的字符至队列 */
	for(i=0; i<10; i++) {
		ch = '0' + i;
		Queue_Add(&q1, &ch);
	}
	/* 获取每个成员，并保存至队列 */
	for(i=0; i<10; i++) str[i] = *( (char*)Queue_Get(&q1, i) );  
	str[i] = 0;
	
	printf("q1, string:%s\n", str);
	p = (char*)Queue_Get(&q1, 5);
	printf("char: %c\n", *p);
	Queue_Delete_Pointer(&q1, 5); 
	for(i=0; i<9; i++)
		str[i] = *( (char*)Queue_Get(&q1, i) );
	str[i] = 0;
	printf("q1, string:%s\n", str);
	Queue_Add_Pointer(&q2, p);
	for(i=0; i<Queue_Get_Total(&q2); i++)
		str[i] = *( (char*)Queue_Get(&q2, i) );
	str[i] = 0;
	printf("q2, string:%s\n", str);
	
	Destroy_Queue(&q1);
	Destroy_Queue(&q2);
	return 0;
}

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
	for(i=0; i<10; i++)
	{
		ch = '0' + i;
		Queue_Add(&bq, &ch);
	}
	/* 获取每个成员，并保存至队列 */
	for(i=0; i<10; i++) str[i] = *( (char*)Queue_Get(&bq, i) );  
		
	str[i] = 0;
	
	printf("string:%s\n", str);
	
	Destroy_Queue(&bq);
	return 0;
}
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
	widget->focus = NULL;
	
	/* 释放字符串 */
	Free_String(&widget->type);
	Free_String(&widget->style);
	
	/* 释放储存图形数据的结构体 */
	Free_Graph(&widget->graph);
	Free_Graph(&widget->background_image);
	
	/* 销毁部件的队列 */
	Destroy_Queue(&widget->child);
	Destroy_Queue(&widget->event);
	Destroy_Queue(&widget->data);
	Destroy_Queue(&widget->update_area);
	
	widget->focus = NULL;
	widget->visible = IS_FALSE;
	widget->enabled = IS_TRUE;
	
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
	int total, i, j, des_pos;
	total = Queue_Get_Total(queue);
	for(i=0; i<total; ++i) {
		temp = (LCUI_Widget*)Queue_Get(queue, i);
		if(temp == widget) {/* 如果找到部件 */
			j = i;
			for(i=0; i<j; ++i) {/* 从头到当前位置遍历队列 */
				temp = (LCUI_Widget*)Queue_Get(queue, i);
				
				if( temp->lock_display == IS_TRUE ) {
				/* 如果该位置的部件锁定了位置 */
					if(widget->lock_display == IS_TRUE) {
					/* 如果目标部件锁定了位置，那就可以移动至最前端 */
						des_pos = 0; 
						Queue_Using(queue, QUEUE_MODE_WRITE);
						for (i=j; i > des_pos; --i) 
							queue->queue[i] = queue->queue[i - 1]; 
							
						queue->queue[des_pos] = widget; 
						Queue_End_Use(queue);
						break;
					}
				} else {/* 否则，该位置的部件没锁定位置 */
					des_pos = i;
					Queue_Using(queue, QUEUE_MODE_WRITE);
					for (i=j; i > des_pos; --i) 
						queue->queue[i] = queue->queue[i - 1];  
						
					queue->queue[des_pos] = widget; 
					Queue_End_Use(queue);
					break;
				}
			} 
		}
	}
	return -1;
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
	if(NULL == temp)
		return 0;
	
	memcpy(rect, temp, sizeof(LCUI_Rect));
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
		}
		else break;
	}
	
	Destroy_Queue(&rect_buff);
	if (flag == 0)
	/* 没有的话，就需要添加至队列 */ 
		return Queue_Add(queue, &rect);
	/* 销毁队列 */
	//if(debug_mark) 
	//	printf("done\n");
	return -1;
}
/************************* RectQueue end *******************************/

