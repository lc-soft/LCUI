/* ***************************************************************************
 * LCUI_GraphLayer.c -- GraphLayer operation set.
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
 * LCUI_GraphLayer.c -- 图层的操作集
 *
 * 版权所有 (C) 2013 归属于
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
#include LC_GRAPH_H

/*
 * 一些英文单词的缩写
 * ctnr: Container 容器
 * 
 * 
 * */

/* 打印子图层信息列表 */
LCUI_API int
GraphLayer_PrintChildList( LCUI_GraphLayer *glayer )
{
	int i, n;
	LCUI_Queue *child_list;
	LCUI_GraphLayer *child;

	child_list = &glayer->child;
	if(child_list == NULL) {
		return -1;
	}
	n = Queue_GetTotal( child_list );
	for(i=0; i<n; ++i) {
		child = (LCUI_GraphLayer*)Queue_Get( child_list, i );
		if( child == NULL ) {
			continue;
		}
		printf("[%d] glayer: %p, z-index: %d, pos: (%d,%d), size: (%d, %d)\n",
			i, child, child->z_index, child->pos.x, child->pos.y,
			child->graph.width, child->graph.height );
	}
	return 0;
}

/* 将子图层从父图层中的子图层队列中移除 */
LCUI_API int
GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer )
{
	int i, total;
	LCUI_Queue *queue;
	LCUI_GraphLayer *tmp_glayer;
	
	if( !child_glayer || !child_glayer->parent ) {
		return -1;
	}
	
	queue = &child_glayer->parent->child;
	total = Queue_GetTotal( queue );
	for( i=0; i<total; ++i ) {
		tmp_glayer = Queue_Get( queue, i );
		if( tmp_glayer == child_glayer ) {
			Queue_DeletePointer( queue, i );
			child_glayer->parent = NULL;
			return 0;
		}
	}
	child_glayer->parent = NULL;
	return 1;
}

/* 释放图层占用的内存资源 */
LCUI_API void
GraphLayer_Free( LCUI_GraphLayer *glayer )
{
	if( glayer == NULL ) {
		return;
	}
	GraphLayer_DeleteChild( glayer );
	Graph_Free( &glayer->graph );
	Queue_Destroy( &glayer->child );
	free( glayer );
}

/* 创建新的图层 */
LCUI_API LCUI_GraphLayer*
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
	Queue_UsingPointer( &glayer->child ); /* 队列用于存储指针 */ 
	return glayer;
}

/* 添加子图层至容器图层中 */
LCUI_API int
GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
			LCUI_GraphLayer *glayer )
{
	int i, total;
	LCUI_GraphLayer *tmp_child;
	//_DEBUG_MSG( "des_ctnr: %p, glayer: %p\n", des_ctnr, glayer );
	/* 容器图层必须有效 */
	if( !des_ctnr ) {
		return -1;
	}
	/* 子图层必须有效，并且不能有父图层 */
	if( !glayer || glayer->parent ) {
		//_DEBUG_MSG( "!glayer || glayer->parent\n" );
		return -2;
	}
	/* 根据队列中的z值，将子图层存放在队列中适当的位置 */
	total = Queue_GetTotal( &des_ctnr->child );
	for( i=0; i<total; ++i ) {
		tmp_child = Queue_Get( &des_ctnr->child, i );
		/* 如果比当前位置的图层的z值小，那就对比下一个位置的图层 */
		if( glayer->z_index < tmp_child->z_index ) {
			continue;
		}
		/* 将新图层插入至该位置 */
		Queue_InsertPointer( &des_ctnr->child, i, glayer );
		break;
	}
	/* 如果没找到位置，则直接添加至末尾 */
	if( i >= total ) {
		Queue_AddPointer( &des_ctnr->child, glayer );
	}
	glayer->parent = des_ctnr;
	return 0;
}

/* 移动子图层至新的容器图层中 */
LCUI_API int
GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
			LCUI_GraphLayer *glayer )
{
	int ret;
	//_DEBUG_MSG( "new_ctnr: %p, glayer: %p\n", new_ctnr, glayer );
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

/* 获取图层矩形 */
LCUI_API LCUI_Rect
GraphLayer_GetRect( LCUI_GraphLayer *glayer )
{
	LCUI_Rect rect;
	rect.x = glayer->pos.x;
	rect.y = glayer->pos.y;
	rect.width = glayer->graph.width;
	rect.height = glayer->graph.height;
	return rect;
}

/* 获取图层尺寸 */
LCUI_API LCUI_Size
GraphLayer_GetSize( LCUI_GraphLayer *glayer )
{
	LCUI_Size size;
	size.w = glayer->graph.width;
	size.h = glayer->graph.height;
	return size;
}

/* 获取图层的全局透明度 */
LCUI_API uchar_t
GraphLayer_GetAlpha( LCUI_GraphLayer *glayer )
{
	return glayer->graph.alpha;
}

/* 图层是否继承父图层的透明度 */
LCUI_API void
GraphLayer_InerntAlpha( LCUI_GraphLayer *glayer, LCUI_BOOL flag )
{
	glayer->inherit_alpha = flag;
}

/* 设定图层的XY轴坐标 */
LCUI_API int
GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y )
{
	if( !glayer ) {
		return -1;
	}
	glayer->pos.x = x;
	glayer->pos.y = y;
	return 0;
}

/* 设定图层的全局透明度 */
LCUI_API void
GraphLayer_SetAlpha( LCUI_GraphLayer *glayer, uchar_t alpha )
{
	glayer->graph.alpha = alpha;
}

/* 设定图层的Z轴坐标，调用此函数后，需要调用GraphLayer_Sort函数对图层列表进行排序 */
LCUI_API int
GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index )
{	
	if( !glayer ) {
		return -1;
	}
	glayer->z_index = z_index;
	return 0;
}

/* 根据子图层的z-index值，对目标图层的子图层进行排序 */
LCUI_API int
GraphLayer_Sort( LCUI_GraphLayer *glayer )
{
	LCUI_GraphLayer *child_a, *child_b;
	int i, j, total;
	
	if( !glayer ) {
		return -1;
	}
	Queue_Lock( &glayer->child );
	total = Queue_GetTotal( &glayer->child );
	for(i=0; i<total; ++i) {
		child_a = (LCUI_GraphLayer*)Queue_Get( &glayer->child, i );
		if( !child_a ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			child_b = (LCUI_GraphLayer*)Queue_Get( &glayer->child, j );
			if( !child_b ) {
				continue;
			}
			if( child_b->z_index > child_a->z_index ) {
				Queue_Swap( &glayer->child, j, i);
				child_a = child_b;
			}
		}
	}
	Queue_Unlock( &glayer->child );
	return 0;
}

/* 调整图层的大小 */
LCUI_API int
GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h )
{
	if( !glayer ) {
		return -1;
	}
	/* 尺寸没有变化的话就返回 */
	if( glayer->graph.width == w
	 && glayer->graph.height == h) {
		return 1;
	}
	
	/* 调整图层尺寸 */
	if( 0 != Graph_Create( &glayer->graph, w, h ) ) {
			return -2;
	}
	return 0;
}

/* 获取指定图层中指定坐标上存在的子图层 */
LCUI_API LCUI_GraphLayer*
GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y )
{
	return NULL;
}

/* 获取指向图层自身图形数据的指针 */
LCUI_API LCUI_Graph*
GraphLayer_GetSelfGraph( LCUI_GraphLayer *glayer )
{
	return &glayer->graph;
}

/* 获取指定根图层中的子图层的有效区域 */
LCUI_API LCUI_Rect
GraphLayer_GetValidRect(	LCUI_GraphLayer *root_glayer,
				LCUI_GraphLayer *glayer )
{
	int temp; 
	LCUI_Pos pos;
	LCUI_Rect area, rect, cut_rect;
	
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
LCUI_API LCUI_Pos
GraphLayer_GetGlobalPos(	LCUI_GraphLayer *root_glayer,
				LCUI_GraphLayer *glayer )
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
		//_DEBUG_MSG("!glayer\n");
		return -1;
	}
	if( !glayer->visible ) {
		//_DEBUG_MSG("!glayer_visible\n");
		return 1;
	}
	child_list = &glayer->child;
	/* 从底到顶遍历子部件 */
	total = Queue_GetTotal( child_list );
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
		//Graph_PrintInfo( &child->graph );
		tmp.x += pos.x;
		tmp.y += pos.y;
		if( !LCUIRect_IsValid(tmp) ) {
			continue;
		}
		if( LCUIRect_Overlay(tmp, rect) ) {
			Queue_AddPointer( queue, child );
			__GraphLayer_GetLayers(	root_glayer, 
						child, rect, queue );
		}
	}
	return 0;
}

/* 获取与图层中指定区域内层叠的子图层 */
LCUI_API int
GraphLayer_GetLayers(
	LCUI_GraphLayer *glayer, 
	LCUI_Rect rect, LCUI_Queue *queue )
{
	//_DEBUG_MSG("rect: %d,%d,%d,%d\n", 
	//rect.x, rect.y, rect.width, rect.height);
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
LCUI_API int
GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
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
	Queue_UsingPointer( &glayerQ );
	
	graph_buff->have_alpha = FALSE;
	tmp_graph.have_alpha = TRUE;
	Graph_Create( graph_buff, rect.width, rect.height );
	GraphLayer_GetLayers( ctnr, rect, &glayerQ ); 
	total = Queue_GetTotal( &glayerQ ); 
	//_DEBUG_MSG( "total: %d\n", total );
	if( total <= 0 ) {
		Graph_Cut ( &ctnr->graph, rect, graph_buff );
		Queue_Destroy( &glayerQ );
		return 1;
	}
	
	for(i=total-1; i>=0; --i) {
		glayer = Queue_Get( &glayerQ, i );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		glayer_pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area.x += glayer_pos.x;
		valid_area.y += glayer_pos.y;
		switch( Graph_IsOpaque( &glayer->graph ) ) {
		    case -1:
			Queue_DeletePointer( &glayerQ, i );
			break;
		    case 0: break;
		    case 1:
			if( LCUIRect_IncludeRect(valid_area, rect) ) { 
				goto skip_loop;
			}
			break;
		    default:break;
		} 
	}
skip_loop:
	total = Queue_GetTotal( &glayerQ );
	//_DEBUG_MSG( "total: %d\n", total );
	if(i <= 0){
		Graph_Cut (&ctnr->graph, rect, graph_buff );
	}
	for(i=0; i<total; ++i) {
		glayer = Queue_Get( &glayerQ, i );
		//_DEBUG_MSG("%p = Queue_Get( %p, %d )\n", glayer, &glayerQ, i);
		if( !glayer ) {
			continue;
		}
		Graph_Lock( &glayer->graph );
		pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		Graph_Quote( &tmp_graph, &glayer->graph, valid_area ); 
		//_DEBUG_MSG("valid area: %d,%d,%d,%d, pos: %d,%d, size: %d,%d\n", 
		//	valid_area.x, valid_area.y, valid_area.width, valid_area.height,
		//	pos.x, pos.y, glayer->graph.width, glayer->graph.height
		//	);
			
		/* 获取相对坐标 */
		pos.x = pos.x - rect.x + valid_area.x;
		pos.y = pos.y - rect.y + valid_area.y;
		//_DEBUG_MSG("mix pos: %d,%d\n", pos.x, pos.y);
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
		Graph_Unlock( &glayer->graph );
	}
	Queue_Destroy( &glayerQ );
	return 0;
}

/* 将指定图层显示在同等z-index值图层的前端 */
LCUI_API int
GraphLayer_Front( LCUI_GraphLayer *glayer )
{
	int i, total, src_pos = -1, des_pos = -1;
	LCUI_GraphLayer *tmp_child;
	LCUI_Queue *child_list;
	
	if( !glayer || !glayer->parent ) {
		return -1;
	}
	child_list = &glayer->parent->child;
	total = Queue_GetTotal( child_list );
	/* 先在队列中找到自己，以及z-index值小于或等于它的第一个图层 */
	for(i=0,src_pos=des_pos=-1; i<total; ++i) {
		tmp_child = (LCUI_GraphLayer*)Queue_Get( child_list, i );
		if( !tmp_child ) {
			continue;
		}
		if( tmp_child == glayer ) {
			src_pos = i;
			continue;
		}
		if( des_pos == -1 ) {
			if( tmp_child->z_index
			  <= glayer->z_index ) {
				des_pos = i;
			}
		} else {
			if( src_pos != -1 ) {
				break;
			}
		}
	}
	/* 没有找到就退出 */
	if( des_pos == -1 || src_pos == -1 ) {
		return -1;
	}
	if( src_pos+1 == des_pos ) {
		return 1;
	}
	/* 找到的话就移动位置 */
	Queue_Move( child_list, des_pos, src_pos );
	return 0;
}

/* 显示图层 */
LCUI_API int
GraphLayer_Show( LCUI_GraphLayer *glayer )
{
	if( !glayer ) {
		return -1;
	}
	if( glayer->visible ) {
		return 1;
	}
	
	glayer->visible = TRUE;
	return 0;
}

/* 隐藏图层 */
LCUI_API int
GraphLayer_Hide( LCUI_GraphLayer *glayer )
{
	if( !glayer ) {
		return -1;
	}
	if( !glayer->visible ) {
		return 1;
	}
	
	glayer->visible = FALSE;
	return 0;
}
