
#ifndef __LCUI_GRAPHLAYER_H__
#define __LCUI_GRAPHLAYER_H__

LCUI_BEGIN_HEADER

typedef struct _LCUI_GraphLayer LCUI_GraphLayer;

struct _LCUI_GraphLayer
{
	BOOL		visible;	/* 图层是否可见 */
	BOOL		inherit_alpha;	/* 是否继承父图层的透明度 */
	int		z_index;	/* 图层的堆叠顺序，值越大，图层显示位置越靠前 */
	LCUI_Pos	pos;	/* 图层的xy轴坐标 */
	
	LCUI_GraphLayer	*parent;	/* 该图层的容器图层 */
	LCUI_Queue		child;		/* 该图层中内的子图层记录 */
	LCUI_Queue		invalid_area;	/* 记录无效区域 */
	LCUI_Graph		graph;		/* 图层像素数据 */
};

/* 将子图层从父图层中的子图层队列中移除 */
int GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer );

/* 释放图层占用的内存资源 */
void GraphLayer_Free( LCUI_GraphLayer *glayer );

/* 创建新的图层 */
LCUI_GraphLayer *
GraphLayer_New( void );

/* 添加子图层至容器图层中 */
int GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
				LCUI_GraphLayer *glayer );

/* 移动子图层至新的容器图层中 */
int GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
				LCUI_GraphLayer *glayer );

/* 添加图层内的无效区域的记录 */
int GraphLayer_InvalidArea( LCUI_GraphLayer *ctnr, LCUI_Rect area );

/* 获取图层矩形 */
LCUI_Rect GraphLayer_GetRect( LCUI_GraphLayer *glayer );

/* 设定图层的XY轴坐标 */
int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y );

/* 设定图层的Z轴坐标 */
int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index );

/* 调整图层的大小 */
int GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h );

/* 获取指定图层中指定坐标上存在的子图层 */
LCUI_GraphLayer *
GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y );

/* 获取图层自身的图形数据 */
int GraphLayer_GetSelfGraph(	LCUI_GraphLayer *glayer, 
				LCUI_Graph *graph );

/* 获取指定根图层中的子图层的有效区域 */
LCUI_Rect GraphLayer_GetValidRect(
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer );

/* 指定根容器图层，获取当前子图层相对于根容器图层的全局坐标 */
LCUI_Pos GraphLayer_GetGlobalPos( 
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer );

/* 获取与图层中指定区域内层叠的子图层 */
int GraphLayer_GetLayers(
	LCUI_GraphLayer *glayer, 
	LCUI_Rect rect, LCUI_Queue *queue );

/* 获取该图层和子图层混合后的图形数据 */
int GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
				LCUI_Graph *graph_buff,
				LCUI_Rect rect );

/* 显示图层 */
int GraphLayer_Show( LCUI_GraphLayer *glayer );

/* 隐藏图层 */
int GraphLayer_Hide( LCUI_GraphLayer *glayer );

LCUI_END_HEADER

#endif
