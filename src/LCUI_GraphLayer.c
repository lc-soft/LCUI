#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

typedef struct _LCUI_GraphLayer LCUI_GraphLayer;

struct _LCUI_GraphLayer
{
	BOOL		visible;	/* 图层是否可见 */
	BOOL		inherit_alpha;	/* 是否继承父图层的透明度 */
	int		z_index;	/* 图层的堆叠顺序，值越大，图层显示位置越靠前 */
	LCUI_Pos	pos;		/* 图层的xy轴坐标 */
	
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
	Graph_Free( &glayer->graph );
	Destroy_Queue( &glayer->child );
	Destroy_Queue( &glayer->invalid_area );
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
	glayer->inherit_alpha = TRUE;
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
		break;
	}
	/* 如果没找到位置，则直接添加至末尾 */
	if( i >= total ) {
		Queue_Add_Pointer( &des_ctnr->child, glayer );
	}
	glayer->parent = des_ctnr;
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

/* 添加图层内的无效区域的记录 */
int GraphLayer_InvalidArea( LCUI_GraphLayer *ctnr, LCUI_Rect area )
{
	if( !ctnr ) {
		return -1;
	}
	return RectQueue_Add( &ctnr->invalid_area, area );
} 

/* 获取图层矩形 */
LCUI_Rect GraphLayer_GetRect( LCUI_GraphLayer *glayer )
{
	LCUI_Rect rect;
	rect.x = glayer->pos.x;
	rect.y = glayer->pos.y;
	rect.width = glayer->graph.width;
	rect.height = glayer->graph.height;
	return rect;
}

/* 设定图层的XY轴坐标 */
int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y )
{
	LCUI_Rect rect;
	
	if( !glayer ) {
		return -1;
	}
	
	/* 如果图层是显示的，那就需要添加无效区域 */
	if( glayer->visible ) {
		rect = GraphLayer_GetRect( glayer );
		GraphLayer_InvalidArea( glayer->parent, rect );
		glayer->pos.x = x;
		glayer->pos.y = y;
		rect.x = x;
		rect.y = y;
		GraphLayer_InvalidArea( glayer->parent, rect );
	} else {
		/* 否则，直接改坐标 */
		glayer->pos.x = x;
		glayer->pos.y = y;
	}
	
	return 0;
}

/* 设定图层的Z轴坐标 */
int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index )
{
	int i, total, src_pos = -1, des_pos = -1;
	LCUI_GraphLayer *tmp_child;
	LCUI_Queue *queue;
	
	if( !glayer ) {
		return -1;
	}
	
	glayer->z_index = z_index;
	if( !glayer->parent ) {
		return 1;
	}
	queue = &glayer->parent->child;
	total = Queue_Get_Total( queue );
	for( i=0; i<total; ++i ) {
		tmp_child = Queue_Get( queue, i );
		if( glayer == tmp_child ) {
			/* 找到自己的位置 */
			src_pos = i;
			continue;
		} else if( z_index < tmp_child->z_index ) {
			continue;
		}
		/* 找到需要移动至的位置 */
		des_pos = i;
		/* 如果已经找到自己的位置 */
		if( src_pos != -1 ) {
			break;
		}
	}
	/* 如果没有找到自己的位置或者目标位置 */
	if( -1 == src_pos || -1 == des_pos ) {
		return 2;
	}
	Queue_Move( queue, des_pos, src_pos );
	return 0;
}

/* 调整图层的大小 */
int GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h )
{
	LCUI_Rect rect;
	
	if( !glayer ) {
		return -1;
	}
	/* 尺寸没有变化的话就返回 */
	if( glayer->graph.width == w && glayer->graph.height == h) {
		return 1;
	}
	
	rect = GraphLayer_GetRect( glayer );
	/* 设置改变尺寸前的图层区域为无效区域 */
	GraphLayer_InvalidArea( glayer->parent, rect );
	/* 调整图层尺寸 */
	if( 0 != Graph_Create( &glayer->graph, w, h ) ) {
			return -2;
	}
	rect.width = w;
	rect.height = h;
	/* 设置改变尺寸后的图层区域为无效区域 */
	GraphLayer_InvalidArea( glayer->parent, rect );
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
	LCUI_Rect rect;
	rect = Rect( 0, 0, glayer->graph.width, glayer->graph.height );
	/* 引用图层图形 */
	return Quote_Graph( graph, &glayer->graph, rect );
}

/* 获取指定根图层中的子图层的有效区域 */
LCUI_Rect GraphLayer_GetValidRect(
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer )
{
	int temp; 
	LCUI_Pos pos;
	LCUI_Rect area;
	LCUI_Rect cut_rect;
	
	if( !glayer ) {
		return Rect(0,0,0,0);
	}
	cut_rect.x = 0;
	cut_rect.y = 0;
	cut_rect.width = glayer->graph.width;
	cut_rect.height = glayer->graph.height;
	pos = glayer->pos;
	area.x = area.y = 0;
	if( !root_glayer || !glayer->parent ) {
		return cut_rect;
	} 
	else if( glayer->parent == root_glayer ) { 
		area.width = root_glayer->graph.width;
		area.height = root_glayer->graph.height;
	} else {
		area.width = glayer->parent->graph.width;
		area.height = glayer->parent->graph.height;
	}
	
	if(pos.x < area.x) {
		cut_rect.x = area.x - pos.x; 
		cut_rect.width -= cut_rect.x;
	}
	if(pos.x + glayer->graph.width - area.x > area.width) {
		cut_rect.width -= pos.x;
		cut_rect.width += area.x;
		cut_rect.width -= glayer->graph.width;
		cut_rect.width += area.width;
	}
	if(pos.y < area.y) {
		cut_rect.y = area.y - pos.y; 
		cut_rect.height -= cut_rect.y;
	}
	if(pos.y + glayer->graph.height - area.y > area.height) {
		cut_rect.height -= pos.y;
		cut_rect.height += area.y;
		cut_rect.height -= glayer->graph.height;
		cut_rect.height += area.height;
	}
	
	LCUI_Rect rect;
	rect = GraphLayer_GetValidRect( root_glayer, glayer->parent );
	if(rect.x > area.x) {
		temp = pos.x + cut_rect.x;
		if(temp < rect.x) {
			temp = rect.x - pos.x;
			cut_rect.width -= (temp - cut_rect.x);
			cut_rect.x = temp;
		}
	}
	if(rect.y > area.y) {
		temp = pos.y + cut_rect.y;
		if(pos.y < rect.y) {
			temp = rect.y - pos.y;
			cut_rect.height -= (temp - cut_rect.y);
			cut_rect.y = temp;
		}
	}
	if(rect.x+rect.width < area.x+area.width) {
		temp = pos.x+cut_rect.x+cut_rect.width;
		if(temp > rect.x+rect.width) {
			cut_rect.width -= (temp-(rect.x+rect.width));
		}
	}
	if(rect.y+rect.height < area.y+area.height) {
		temp = pos.y+cut_rect.y+cut_rect.height;
		if(temp > rect.y+rect.height) {
			cut_rect.height -= (temp-(rect.y+rect.height));
		}
	}
	return cut_rect;
}

/* 指定根容器图层，获取当前子图层相对于根容器图层的全局坐标 */
LCUI_Pos GraphLayer_GetGlobalPos( 
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer )
{
	LCUI_Pos pos;
	if( !glayer || !root_glayer || glayer == root_glayer ) {
		return Pos(0,0);
	}
	pos = GraphLayer_GetGlobalPos( root_glayer, glayer->parent );
	pos = Pos_Add( pos, glayer->pos );
	return pos;
}


static int 
__GraphLayer_GetLayers(
	LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer, 
	LCUI_Rect rect, LCUI_Queue *queue )
{
	int i, total;
	LCUI_Pos pos;
	LCUI_Rect tmp;
	LCUI_GraphLayer *child; 
	LCUI_Queue *child_list;

	if( !glayer ) {
		return -1;
	}
	if( !glayer->visible ) {
		return 1;
	}
	child_list = &glayer->child;
	/* 从底到顶遍历子部件 */
	total = Queue_Get_Total( child_list );
	//_DEBUG_MSG( "root: %p, cur: %p, child total: %d\n",
	//		root_glayer, glayer, total );
	/* 从尾到首，从底到顶，遍历图层 */
	for( i=total-1; i>=0; --i ) {
		child = Queue_Get( child_list, i );
		if( !child || !child->visible ) {
			continue;
		}
		
		tmp = GraphLayer_GetValidRect( root_glayer, child );
		pos = GraphLayer_GetGlobalPos( root_glayer, child );
		//_DEBUG_MSG( "child: %p, pos: %d,%d, valid rect: %d,%d, %d, %d\n", 
		//	child, pos.x, pos.y, tmp.x, tmp.y, tmp.width, tmp.height);
		tmp.x += pos.x;
		tmp.y += pos.y;
		if( !Rect_Valid(tmp) ) {
			continue;
		}
		if( Rect_Is_Overlay(tmp, rect) ) {
			Queue_Add_Pointer( queue, child );
			__GraphLayer_GetLayers(	root_glayer, 
						child, rect, queue );
		}
	}
	return 0;
}

/* 获取与图层中指定区域内层叠的子图层 */
int GraphLayer_GetLayers(
	LCUI_GraphLayer *glayer, 
	LCUI_Rect rect, LCUI_Queue *queue )
{
	return __GraphLayer_GetLayers( glayer, glayer, rect, queue );
}

/* 获取图层实际的全局透明度 */
static uchar_t 
GraphLayer_GetRealAlpha( LCUI_GraphLayer *glayer )
{
	if( !glayer->inherit_alpha || !glayer->parent );
	else if( glayer->parent ) {
		uchar_t tmp, alpha;
		tmp = GraphLayer_GetRealAlpha( glayer->parent );
		alpha = tmp*1.0 / 255 * glayer->graph.alpha;
		return alpha;
	}
	return glayer->graph.alpha;
}


/* 获取该图层和子图层混合后的图形数据 */
int GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
				LCUI_Graph *graph_buff,
				LCUI_Rect rect )
{
	static int i, total; 
	static uchar_t tmp_alpha, alpha;
	static LCUI_Pos pos, glayer_pos;
	static LCUI_GraphLayer *glayer;
	static LCUI_Queue glayerQ;
	static LCUI_Rect valid_area;
	static LCUI_Graph tmp_graph;
	
	/* 检测这个区域是否有效 */
	if (rect.x < 0 || rect.y < 0) {
		return -1; 
	}
	if (rect.x + rect.width > ctnr->graph.width
	 || rect.y + rect.height > ctnr->graph.height ) {
		 return -1;
	}
	if (rect.width <= 0 && rect.height <= 0) {
		return -2;
	}
	
	Graph_Init( &tmp_graph );
	Queue_Init( &glayerQ, 0, NULL);
	Queue_Using_Pointer( &glayerQ );
	
	graph_buff->have_alpha = FALSE;
	tmp_graph.have_alpha = TRUE;
	Graph_Create( graph_buff, rect.width, rect.height );
	GraphLayer_GetLayers( ctnr, rect, &glayerQ ); 
	total = Queue_Get_Total( &glayerQ ); 
	//_DEBUG_MSG( "total: %d\n", total );
	if( total <= 0 ) {
		Graph_Cut ( &ctnr->graph, rect, graph_buff );
		Destroy_Queue( &glayerQ );
		return 1;
	}
	
	for(i=total-1; i>=0; --i) {
		glayer = Queue_Get( &glayerQ, i );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		glayer_pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area.x += glayer_pos.x;
		valid_area.y += glayer_pos.y;
		switch( Graph_Is_Opaque( &glayer->graph ) ) {
		    case -1:
			Queue_Delete_Pointer( &glayerQ, i );
			break;
		    case 0: break;
		    case 1:
			if( Rect_Include_Rect(valid_area, rect) ) { 
				goto skip_loop;
			}
			break;
		    default:break;
		} 
	}
skip_loop:
	total = Queue_Get_Total( &glayerQ );
	//_DEBUG_MSG( "total: %d\n", total );
	if(i <= 0){
		Graph_Cut (&ctnr->graph, rect, graph_buff );
	}
	for(i=0; i<total; ++i) {
		glayer = Queue_Get( &glayerQ, i ); 
		pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		Quote_Graph( &tmp_graph, &glayer->graph, valid_area ); 
		/* 获取相对坐标 */
		pos.x = pos.x - rect.x + valid_area.x;
		pos.y = pos.y - rect.y + valid_area.y;
		/* 如果该图层没有继承父图层的透明度 */
		if( !glayer->inherit_alpha ) {
			Graph_Mix( graph_buff, &tmp_graph, pos );
		} else {
			/* 否则，计算该图层应有的透明度 */
			alpha = GraphLayer_GetRealAlpha( glayer );
			tmp_alpha = glayer->graph.alpha;
			glayer->graph.alpha = alpha;
			Graph_Mix( graph_buff, &tmp_graph, pos );
			glayer->graph.alpha = tmp_alpha;
		}
	}
	Destroy_Queue( &glayerQ );
	return 0;
}

/* 显示图层 */
int GraphLayer_Show( LCUI_GraphLayer *glayer )
{
	LCUI_Rect rect;
	
	if( !glayer ) {
		return -1;
	}
	if( glayer->visible ) {
		return 1;
	}
	
	glayer->visible = TRUE;
	rect = GraphLayer_GetRect( glayer );
	GraphLayer_InvalidArea( glayer->parent, rect );
	return 0;
}

/* 隐藏图层 */
int GraphLayer_Hide( LCUI_GraphLayer *glayer )
{
	LCUI_Rect rect;
	
	if( !glayer ) {
		return -1;
	}
	if( !glayer->visible ) {
		return 1;
	}
	
	glayer->visible = FALSE;
	rect = GraphLayer_GetRect( glayer );
	GraphLayer_InvalidArea( glayer->parent, rect );
	return 0;
}
