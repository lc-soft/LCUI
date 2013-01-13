#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

typedef struct _LCUI_GraphLayer LCUI_GraphLayer;

struct _LCUI_GraphLayer
{
	BOOL visible;	/* 图层是否可见 */
	
	int x, y, z;	/* 
			* 表示X、Y、Z轴的坐标，XY轴就是一个平面坐标系，原点在屏幕
			* 左上角；而z表示图层的堆叠顺序，z的值越大，图层显示位置越
			* 靠前。
			* */
	LCUI_GraphLayer *parent;	/* 该图层的容器图层 */
	LCUI_Queue child;		/* 该图层中内的子图层记录 */
	LCUI_Queue invalid_area;	/* 记录无效区域 */
	LCUI_Graph graph;		/* 图层像素数据 */
};

/*
 * 一些英文单词的缩写
 * ctnr: Container 容器
 * 
 * 
 * */

/* 释放图层占用的内存资源 */
void GraphLayer_Free( LCUI_GraphLayer *glayer )
{
	int i, total;
	LCUI_Queue *queue;
	LCUI_GraphLayer *tmp_glayer;
	
	/* 从父图层中的子图层队列中移除自己 */
	if( glayer->parent ) {
		queue = &glayer->parent->child;
		total = Queue_Get_Total( queue );
		for( i=0; i<total; ++i ) {
			tmp_glayer = Queue_Get( queue, i );
			if( tmp_glayer == glayer ) {
				Queue_Delete( queue, i );
				break;
			}
		}
	}
	free( glayer );
}

/* 创建新的图层 */
LCUI_GraphLayer *
GraphLayer_New( void )
{
	LCUI_GraphLayer * glayer;
	
	glayer = malloc( sizeof( LCUI_GraphLayer ) );
	if( glayer == NULL ) {
		return NULL;
	}
	
	glayer->visible = FALSE;
	glayer->x = glayer->y = glayer->z = 0;
	glayer->parent = NULL;
	Graph_Init( &glayer->graph );
	Queue_Init( &glayer->child, 0, NULL );
	Queue_Using_Pointer( &glayer->child ); /* 队列用于存储指针 */
	RectQueue_Init( &glayer->invalid_area );
	
	return glayer;
}

/* 添加子图层至容器图层中 */
int GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
				LCUI_GraphLayer *glayer )
{
	if( !des_ctnr ) {
		return -1;
	}
	/* 根据队列中的z值，将子图层存放在队列中适当的位置 */
	// code ......
	
	return 0;
}

/* 移动子图层至新的容器图层中 */
int GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
				LCUI_GraphLayer *glayer )
{
	return 0;
}

/* 设定图层的XY轴坐标 */
int GraphLayer_SetXY( LCUI_GraphLayer *glayer, int x, int y )
{
	return 0;
}

/* 设定图层的Z轴坐标 */
int GraphLayer_SetZ( LCUI_GraphLayer *glayer, int z )
{
	return 0;
}

/* 获取指定图层中指定坐标上存在的子图层 */
LCUI_GraphLayer *
GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y )
{
	return NULL;
}

/* 获取图层自身的图形数据 */
int GraphLayer_GetSelfGraph(	LCUI_GraphLayer *glayer, 
				LCUI_Graph *graph )
{
	return 0;
}

/* 获取该图层和子图层混合后的图形数据 */
int GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
				LCUI_Graph *graph_buff )
{
	return 0;
}

/* 添加图层内的无效区域的记录 */
int GraphLayer_InvalidArea( LCUI_GraphLayer *ctnr, LCUI_Rect area )
{
	return 0;
} 
