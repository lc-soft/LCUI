#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

typedef struct _LCUI_GraphLayer LCUI_GraphLayer;

struct _LCUI_GraphLayer
{
	BOOL visible;	/* 图层是否可见 */
	int z_index;	/* 图层的堆叠顺序，值越大，图层显示位置越靠前 */
	LCUI_Pos pos;	/* 图层的xy轴坐标 */
	
	LCUI_GraphLayer	*parent;	/* 该图层的容器图层 */
	LCUI_Queue		child;		/* 该图层中内的子图层记录 */
	LCUI_Queue		invalid_area;	/* 记录无效区域 */
	LCUI_Graph		graph;		/* 图层像素数据 */
};

/*
 * 一些英文单词的缩写
 * ctnr: Container 容器
 * 
 * 
 * */

/* 将子图层从父图层中的子图层队列中移除 */
int GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer )
{
	int i, total;
	LCUI_Queue *queue;
	LCUI_GraphLayer *tmp_glayer;
	
	if( !child_glayer || !child_glayer->parent ) {
		return -1;
	}
	
	queue = &child_glayer->parent->child;
	total = Queue_Get_Total( queue );
	for( i=0; i<total; ++i ) {
		tmp_glayer = Queue_Get( queue, i );
		if( tmp_glayer == child_glayer ) {
			Queue_Delete_Pointer( queue, i );
			return 0;
		}
	}
	return 1;
}

/* 释放图层占用的内存资源 */
void GraphLayer_Free( LCUI_GraphLayer *glayer )
{
	GraphLayer_DeleteChild( glayer );
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
	glayer->pos.x = glayer->pos.y = glayer->z_index = 0;
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
	int i, total;
	LCUI_GraphLayer *tmp_child;
	/* 容器图层必须有效 */
	if( !des_ctnr ) {
		return -1;
	}
	/* 子图层必须有效，并且不能有父图层 */
	if( !glayer || glayer->parent ) {
		return -2;
	}
	/* 根据队列中的z值，将子图层存放在队列中适当的位置 */
	total = Queue_Get_Total( &des_ctnr->child );
	for( i=0; i<total; ++i ) {
		tmp_child = Queue_Get( &des_ctnr->child, i );
		/* 如果比当前位置的图层的z值小，那就对比下一个位置的图层 */
		if( glayer->z_index < tmp_child->z_index ) {
			continue;
		}
		/* 将新图层插入至该位置 */
		Queue_Insert_Pointer( &des_ctnr->child, i, glayer );
	}
	/* 如果没找到位置，则直接添加至末尾 */
	if( i >= total ) {
		Queue_Add_Pointer( &des_ctnr->child, glayer );
	}
	return 0;
}

/* 移动子图层至新的容器图层中 */
int GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
				LCUI_GraphLayer *glayer )
{
	int ret;
	ret = GraphLayer_DeleteChild( glayer );
	if( ret != 0 ) {
		return -1;
	}
	ret = GraphLayer_AddChild( new_ctnr, glayer );
	if( ret != 0 ) {
		return -2;
	}
	return 0;
}

/* 设定图层的XY轴坐标 */
int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y )
{
	if( !glayer ) {
		return -1;
	}
	
	if( glayer->visible ) {
		
	}
	
	return 0;
}

/* 设定图层的Z轴坐标 */
int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z )
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
	if( !ctnr ) {
		return -1;
	}
	return RectQueue_Add( &ctnr->invalid_area, area );
} 

/* 显示图层 */
int GraphLayer_Show( LCUI_GraphLayer *glayer )
{
	return 0;
}

/* 隐藏图层 */
int GraphLayer_Hide( LCUI_GraphLayer *glayer )
{
	return 0;
}
