/** ******************************************************************************
 * @file	LCUI_GraphLayer.c
 * @brief	GraphLayer operation set.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	LCUI_GraphLayer.c
 * @brief	图层的操作集.
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于							\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)					\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。		\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定用途	\n
 * 的隐含担保，详情请参照GPLv2许可协议。						\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

LCUI_API int GraphLayer_PrintChildList( LCUI_GraphLayer *glayer )
{
	int i, n;
	LCUI_Queue *child_list;
	LCUI_GraphLayer *child;

	if( glayer == NULL ) {
		return -1;
	}
	child_list = &glayer->child;
	if(child_list == NULL) {
		return -1;
	}
	n = Queue_GetTotal( child_list );
	_DEBUG_MSG("total glayer: %d\n", n);
	for(i=0; i<n; ++i) {
		child = (LCUI_GraphLayer*)Queue_Get( child_list, i );
		if( child == NULL ) {
			continue;
		}
		printf("[%d] glayer: %p, z-index: %d, pos: (%d,%d), size: (%d, %d)\n",
			i, child, child->z_index, child->pos.x, child->pos.y,
			child->graph.w, child->graph.h );
	}
	return 0;
}

LCUI_API int GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer )
{
	int i, total;
	LCUI_Queue *child_list;
	LCUI_GraphLayer *tmp_glayer;
	
	if( !child_glayer ) {
		return -1;
	}
	if(  !child_glayer->parent ) {
		return 0;
	}
	/* 引用父图层的子图层列表 */
	child_list = &child_glayer->parent->child;
	total = Queue_GetTotal( child_list );
	/* 查找子图层记录 */
	for( i=0; i<total; ++i ) {
		tmp_glayer = (LCUI_GraphLayer*)Queue_Get( child_list, i );
		/* 若找到则删除该子图层记录 */
		if( tmp_glayer == child_glayer ) {
			Queue_DeletePointer( child_list, i );
			child_glayer->parent = NULL;
			return 0;
		}
	}
	child_glayer->parent = NULL;
	return 0;
}

LCUI_API void GraphLayer_Free( LCUI_GraphLayer *glayer )
{
	if( glayer == NULL ) {
		return;
	}
	/* 移除该图层在父图层中的记录 */
	GraphLayer_DeleteChild( glayer );
	/* 释放图层的图像数据 */
	Graph_Free( &glayer->graph );
	/* 销毁子图层列表 */
	Queue_Destroy( &glayer->child );
	free( glayer );
}

LCUI_API LCUI_GraphLayer* GraphLayer_New( void )
{
	LCUI_GraphLayer * glayer;
	
	glayer = (LCUI_GraphLayer*)malloc( sizeof( LCUI_GraphLayer ) );
	if( glayer == NULL ) {
		return NULL;
	}
	/* 赋初始值 */
	glayer->visible = FALSE;
	glayer->inherit_alpha = TRUE;
	glayer->pos.x = glayer->pos.y = glayer->z_index = 0;
	glayer->padding.left = 0;
	glayer->padding.top = 0;
	glayer->padding.bottom = 0;
	glayer->padding.right = 0;
	glayer->parent = NULL;
	/* 初始化图像数据 */
	Graph_Init( &glayer->graph );
	/* 初始化子图层列表 */
	Queue_Init( &glayer->child, 0, NULL );
	/* 标记该队列用于存储指针，即使销毁队列后，也不自动释放指针指向的内存空间 */ 
	Queue_UsingPointer( &glayer->child ); 
	return glayer;
}


LCUI_API int GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
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
		tmp_child = (LCUI_GraphLayer*)Queue_Get( &des_ctnr->child, i );
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

LCUI_API int GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
					LCUI_GraphLayer *glayer )
{
	int ret;
	//_DEBUG_MSG( "new_ctnr: %p, glayer: %p\n", new_ctnr, glayer );
	/* 先删除子图层记录，以解除与之前父图层的父子关系 */
	ret = GraphLayer_DeleteChild( glayer );
	if( ret != 0 ) {
		return -1;
	}
	/* 再添加至新的父图层内 */
	ret = GraphLayer_AddChild( new_ctnr, glayer );
	if( ret != 0 ) {
		return -2;
	}
	return 0;
}

LCUI_API LCUI_Rect GraphLayer_GetRect( LCUI_GraphLayer *glayer )
{
	LCUI_Rect rect;
	rect.x = glayer->pos.x;
	rect.y = glayer->pos.y;
	if( glayer->parent ) {
		rect.x += glayer->parent->padding.left;
		rect.y += glayer->parent->padding.top;
	}
	rect.width = glayer->graph.w;
	rect.height = glayer->graph.h;
	return rect;
}

LCUI_API LCUI_Size GraphLayer_GetSize( LCUI_GraphLayer *glayer )
{
	LCUI_Size size;
	size.w = glayer->graph.w;
	size.h = glayer->graph.h;
	return size;
}

LCUI_API uchar_t GraphLayer_GetAlpha( LCUI_GraphLayer *glayer )
{
	return glayer->graph.alpha;
}

LCUI_API void GraphLayer_InerntAlpha( LCUI_GraphLayer *glayer, LCUI_BOOL flag )
{
	glayer->inherit_alpha = flag;
}

LCUI_API int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y )
{
	if( !glayer ) {
		return -1;
	}
	glayer->pos.x = x;
	glayer->pos.y = y;
	return 0;
}

LCUI_API void GraphLayer_SetPadding( LCUI_GraphLayer *glayer, LCUI_Padding padding )
{
	glayer->padding = padding;
}

LCUI_API void GraphLayer_SetAlpha( LCUI_GraphLayer *glayer, uchar_t alpha )
{
	glayer->graph.alpha = alpha;
}

LCUI_API int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index )
{	
	if( !glayer ) {
		return -1;
	}
	glayer->z_index = z_index;
	return 0;
}


LCUI_API int GraphLayer_Sort( LCUI_GraphLayer *glayer )
{
	LCUI_GraphLayer *child_a, *child_b;
	int i, j, total;
	
	if( !glayer ) {
		return -1;
	}
	/* 排序前先锁上队列互斥锁 */
	Queue_Lock( &glayer->child );
	total = Queue_GetTotal( &glayer->child );
	/* 使用的是选择排序法 */
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
	/* 解开互斥锁 */
	Queue_Unlock( &glayer->child );
	return 0;
}


LCUI_API int GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h )
{
	if( !glayer ) {
		return -1;
	}
	/* 尺寸没有变化的话就返回 */
	if( glayer->graph.w == w
	 && glayer->graph.h == h) {
		return 1;
	}
	
	/* 调整图层尺寸 */
	if( 0 != Graph_Create( &glayer->graph, w, h ) ) {
			return -2;
	}
	return 0;
}


LCUI_API LCUI_GraphLayer* GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y )
{
	return NULL;
}

LCUI_API LCUI_Graph* GraphLayer_GetSelfGraph( LCUI_GraphLayer *glayer )
{
	return &glayer->graph;
}

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
	cut_rect.width = glayer->graph.w;
	cut_rect.height = glayer->graph.h;
	pos = glayer->pos;
	/* 如果根图层无效，或者没有父图层 */
	if( !root_glayer || !glayer->parent ) {
		return cut_rect;
	}
	/* 加上父图层的内边距 */
	pos.x += glayer->parent->padding.left;
	pos.y += glayer->parent->padding.top;
	/* 计算当前有效显示区域 */
	area.x = glayer->parent->padding.left;
	area.y = glayer->parent->padding.top;
	/* 区域尺寸则减去内边距 */
	area.width = glayer->parent->graph.w;
	area.width -= glayer->parent->padding.left;
	area.width -= glayer->parent->padding.right;
	area.height = glayer->parent->graph.h;
	area.height -= glayer->parent->padding.top;
	area.height -= glayer->parent->padding.bottom;
	/* 根据图层的坐标及尺寸，计算图层裁剪后的区域 */
	if(pos.x < area.x) {
		cut_rect.x = area.x - pos.x; 
		cut_rect.width -= cut_rect.x;
	}
	if(pos.x + glayer->graph.w - area.x > area.width) {
		cut_rect.width -= pos.x;
		cut_rect.width += area.x;
		cut_rect.width -= glayer->graph.w;
		cut_rect.width += area.width;
	}
	if(pos.y < area.y) {
		cut_rect.y = area.y - pos.y; 
		cut_rect.height -= cut_rect.y;
	}
	if(pos.y + glayer->graph.h - area.y > area.height) {
		cut_rect.height -= pos.y;
		cut_rect.height += area.y;
		cut_rect.height -= glayer->graph.h;
		cut_rect.height += area.height;
	}
	/* 递归调用 */
	rect = GraphLayer_GetValidRect( root_glayer, glayer->parent );
	/* 根据父图层的有效区域，调整当前图层的裁剪区域 */
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


LCUI_API LCUI_Pos
GraphLayer_GetGlobalPos(	LCUI_GraphLayer *root_glayer,
				LCUI_GraphLayer *glayer )
{
	LCUI_Pos pos;
	if( !glayer || !root_glayer || glayer == root_glayer ) {
		return Pos(0,0);
	}
	/* 递归获取父图层的全局坐标 */
	pos = GraphLayer_GetGlobalPos( root_glayer, glayer->parent );
	pos.x += glayer->pos.x;
	pos.y += glayer->pos.y;
	if( glayer->parent ) {
		/* 加上内边距 */
		pos.x += glayer->parent->padding.left;
		pos.y += glayer->parent->padding.top;
	}
	return pos;
}


static int 
__GraphLayer_GetLayers(
	LCUI_GraphLayer *root_glayer,
	LCUI_GraphLayer *glayer, 
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
		child = (LCUI_GraphLayer*)Queue_Get( child_list, i );
		/* 忽略无效或不可见的图层 */
		if( !child || !child->visible ) {
			continue;
		}
		/* 获取子图层的有效区域及全局坐标 */
		tmp = GraphLayer_GetValidRect( root_glayer, child );
		pos = GraphLayer_GetGlobalPos( root_glayer, child );
		//_DEBUG_MSG( "child: %p, pos: %d,%d, valid rect: %d,%d, %d, %d\n", 
		//	child, pos.x, pos.y, tmp.x, tmp.y, tmp.width, tmp.height);
		//Graph_PrintInfo( &child->graph );
		/* 有效区域加上子部件的全局坐标 */
		tmp.x += pos.x;
		tmp.y += pos.y;
		/* 判断区域是否有效 */
		if( !LCUIRect_IsValid(tmp) ) {
			continue;
		}
		/* 若该有效区域与目标区域重叠，则记录子部件，并进行递归 */
		if( LCUIRect_Overlay(tmp, rect) ) {
			Queue_AddPointer( queue, child );
			__GraphLayer_GetLayers(	root_glayer, 
						child, rect, queue );
		}
	}
	return 0;
}

LCUI_API int GraphLayer_GetLayers(	LCUI_GraphLayer *glayer, 
					LCUI_Rect rect,
					LCUI_Queue *queue )
{
	//_DEBUG_MSG("rect: %d,%d,%d,%d\n", 
	//rect.x, rect.y, rect.width, rect.height);
	return __GraphLayer_GetLayers( glayer, glayer, rect, queue );
}

/* 获取图层实际的全局透明度 */
static uchar_t GraphLayer_GetRealAlpha( LCUI_GraphLayer *glayer )
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


LCUI_API int GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
					LCUI_Graph *graph_buff,
					LCUI_Rect rect )
{
	int i, total; 
	uchar_t tmp_alpha, alpha;
	LCUI_Pos pos, glayer_pos;
	LCUI_GraphLayer *glayer;
	LCUI_Queue glayerQ;
	LCUI_Rect valid_area;
	LCUI_Graph tmp_graph;
	
	/* 检测这个区域是否有效 */
	if (rect.x < 0 || rect.y < 0) {
		return -1; 
	}
	if (rect.x + rect.width > ctnr->graph.w
	 || rect.y + rect.height > ctnr->graph.h ) {
		 return -1;
	}
	if (rect.width <= 0 || rect.height <= 0) {
		return -2;
	}
	
	Graph_Init( &tmp_graph );
	Queue_Init( &glayerQ, 0, NULL);
	Queue_UsingPointer( &glayerQ );
	
	graph_buff->color_type = COLOR_TYPE_RGB;
	/* 为graph_buff分配合适尺寸的内存空间 */
	Graph_Create( graph_buff, rect.width, rect.height );
	/* 获取rect区域内的图层列表 */
	GraphLayer_GetLayers( ctnr, rect, &glayerQ ); 
	total = Queue_GetTotal( &glayerQ ); 
	//_DEBUG_MSG( "total: %d\n", total );
	/* 若记录数为零，则表明该区域没有图层 */
	if( total <= 0 ) {
		/* 若没有父图层，则填充白色 */
		if( ctnr == NULL ) {
			Graph_FillColor( graph_buff, RGB(255,255,255) );
		} else { /* 否则使用父图层的图形 */
			Graph_Cut( &ctnr->graph, rect, graph_buff );
		}
		/* 销毁记录 */
		Queue_Destroy( &glayerQ );
		return 0;
	}
	/* 从顶层到底层遍历图层，排除被其它图层完全遮挡或者自身完全透明的图层 */
	for(i=total-1; i>=0; --i) {
		glayer = (LCUI_GraphLayer*)Queue_Get( &glayerQ, i );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		glayer_pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area.x += glayer_pos.x;
		valid_area.y += glayer_pos.y;
		alpha = GraphLayer_GetRealAlpha( glayer );
		/* 当前图层的透明度小于255的话，就跳过 */
		if( alpha < 255 ) {
			continue;
		}
		/* 如果色彩类型为RGB，没有alpha通道 */
		if( glayer->graph.color_type == COLOR_TYPE_RGB ) {
			/* 如果该图层的有效区域包含目标区域 */
			if( LCUIRect_IncludeRect(valid_area, rect) ) { 
				/* 移除底层的图层，因为已经被完全遮挡 */
				for(total=i-1;total>=0; --total) {
					Queue_DeletePointer( &glayerQ, 0 );
				}
				goto skip_loop;
			}
		}
	}
skip_loop:
	total = Queue_GetTotal( &glayerQ );
	//_DEBUG_MSG( "total: %d\n", total );
	if(i <= 0) {
		if( ctnr == NULL ) {
			Graph_FillColor( graph_buff, RGB(255,255,255) );
		} else {
			Graph_Cut ( &ctnr->graph, rect, graph_buff );
		}
	}
	/* 获取图层列表中的图层 */
	for(i=0; i<total; ++i) {
		glayer = (LCUI_GraphLayer*)Queue_Get( &glayerQ, i );
		//_DEBUG_MSG("%p = Queue_Get( %p, %d )\n", glayer, &glayerQ, i);
		if( !glayer ) {
			continue;
		}
		/* 锁上该图层的互斥锁 */
		Graph_Lock( &glayer->graph );
		/* 获取该图层的有效区域及全局坐标 */
		pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		/* 引用该图层的有效区域内的图像 */
		Graph_Quote( &tmp_graph, &glayer->graph, valid_area ); 
		//_DEBUG_MSG("valid area: %d,%d,%d,%d, pos: %d,%d, size: %d,%d\n", 
		//	valid_area.x, valid_area.y, valid_area.width, valid_area.height,
		//	pos.x, pos.y, glayer->graph.w, glayer->graph.h
		//	);
		/* 获取相对坐标 */
		pos.x = pos.x - rect.x + valid_area.x;
		pos.y = pos.y - rect.y + valid_area.y;
		//_DEBUG_MSG("mix pos: %d,%d\n", pos.x, pos.y);
		/* 如果该图层没有继承父图层的透明度 */
		if( !glayer->inherit_alpha ) {
			/* 直接叠加至graph_buff */
			Graph_Mix( graph_buff, &tmp_graph, pos );
		} else {
			/* 否则，计算该图层应有的透明度 */
			alpha = GraphLayer_GetRealAlpha( glayer );
			/* 备份该图层的全局透明度 */
			tmp_alpha = glayer->graph.alpha;
			/* 将实际透明度作为全局透明度，参与图像叠加 */
			glayer->graph.alpha = alpha;
			Graph_Mix( graph_buff, &tmp_graph, pos );
			/* 还原全局透明度 */
			glayer->graph.alpha = tmp_alpha;
		}
		/* 解锁图层 */
		Graph_Unlock( &glayer->graph );
	}
	Queue_Destroy( &glayerQ );
	return 0;
}

LCUI_API int GraphLayer_Front( LCUI_GraphLayer *glayer )
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
		return -2;
	}
	if( src_pos+1 == des_pos ) {
		return 1;
	}
	/* 找到的话就移动位置 */
	Queue_Move( child_list, des_pos, src_pos );
	return 0;
}

LCUI_API int GraphLayer_Show( LCUI_GraphLayer *glayer )
{
	if( !glayer ) {
		return -1;
	}
	glayer->visible = TRUE;
	return 0;
}

LCUI_API int GraphLayer_Hide( LCUI_GraphLayer *glayer )
{
	if( !glayer ) {
		return -1;
	}
	glayer->visible = FALSE;
	return 0;
}
