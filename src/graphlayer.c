/* ***************************************************************************
 * graphlayer.c -- graphics layer operation set.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * graphlayer.c -- 图层的操作集.
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

int GraphLayer_PrintChildList( LCUI_GraphLayer *glayer )
{
	int i, n;
	LinkedList *children;
	LCUI_GraphLayer *child;

	if( glayer == NULL ) {
		return -1;
	}
	children = &glayer->children;
	if(children == NULL) {
		return -1;
	}
	n = LinkedList_GetTotal( children );
	_DEBUG_MSG("n glayer: %d\n", n);
	for(i=0; i<n; ++i) {
		LinkedList_Goto( children, i );
		child = (LCUI_GraphLayer*)LinkedList_Get( children );
		if( child == NULL ) {
			continue;
		}
		printf("[%d] glayer: %p, z-index: %d, pos: (%d,%d), size: (%d, %d)\n",
			i, child, child->z_index, child->pos.x, child->pos.y,
			child->graph.w, child->graph.h );
	}
	return 0;
}

int GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer )
{
	int i, n;
	LinkedList *children;
	LCUI_GraphLayer *tmp_glayer;
	
	if( !child_glayer ) {
		return -1;
	}
	if(  !child_glayer->parent ) {
		return 0;
	}
	/* 引用父图层的子图层列表 */
	children = &child_glayer->parent->children;
	n = LinkedList_GetTotal( children );
	/* 查找子图层记录 */
	for(i=0; i<n; ++i) {
		LinkedList_Goto( children, i );
		tmp_glayer = (LCUI_GraphLayer*)LinkedList_Get( children );
		/* 若找到则删除该子图层记录 */
		if( tmp_glayer == child_glayer ) {
			LinkedList_Delete( children );
			child_glayer->parent = NULL;
			return 0;
		}
	}
	child_glayer->parent = NULL;
	return 0;
}

void GraphLayer_Free( LCUI_GraphLayer *glayer )
{
	if( glayer == NULL ) {
		return;
	}
	/* 移除该图层在父图层中的记录 */
	GraphLayer_DeleteChild( glayer );
	/* 释放图层的图像数据 */
	Graph_Free( &glayer->graph );
	/* 销毁子图层列表 */
	LinkedList_Destroy( &glayer->children );
	free( glayer );
}

LCUI_GraphLayer* GraphLayer_New( void )
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
	glayer->content_box.left = 0;
	glayer->content_box.top = 0;
	glayer->content_box.bottom = 0;
	glayer->content_box.right = 0;
	glayer->parent = NULL;
	/* 初始化图像数据 */
	Graph_Init( &glayer->graph );
	/* 初始化子图层列表 */
	LinkedList_Init( &glayer->children, 0 );
	LinkedList_SetDataNeedFree( &glayer->children, FALSE );
	return glayer;
}

int GraphLayer_AddChild( LCUI_GraphLayer *des_ctnr, LCUI_GraphLayer *glayer )
{
	int i, n;
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
	n = LinkedList_GetTotal( &des_ctnr->children );
	for( i=0; i<n; ++i ) {
		LinkedList_Goto( &des_ctnr->children, i );
		tmp_child = (LCUI_GraphLayer*)LinkedList_Get( &des_ctnr->children );
		/* 如果比当前位置的图层的z值小，那就对比下一个位置的图层 */
		if( glayer->z_index < tmp_child->z_index ) {
			continue;
		}
		/* 将新图层插入至该位置 */
		LinkedList_Insert( &des_ctnr->children, glayer );
		break;
	}
	/* 如果没找到位置，则直接添加至末尾 */
	if( i >= n ) {
		LinkedList_AddData( &des_ctnr->children, glayer );
	}
	glayer->parent = des_ctnr;
	return 0;
}

int GraphLayer_MoveChild( LCUI_GraphLayer *new_ctnr, LCUI_GraphLayer *glayer )
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

LCUI_Rect GraphLayer_GetRect( LCUI_GraphLayer *glayer )
{
	LCUI_Rect rect;
	rect.x = glayer->pos.x;
	rect.y = glayer->pos.y;
	if( glayer->parent ) {
		rect.x += glayer->parent->content_box.left;
		rect.y += glayer->parent->content_box.top;
	}
	rect.width = glayer->graph.w;
	rect.height = glayer->graph.h;
	return rect;
}

LCUI_Size GraphLayer_GetSize( LCUI_GraphLayer *glayer )
{
	LCUI_Size size;
	size.w = glayer->graph.w;
	size.h = glayer->graph.h;
	return size;
}

uchar_t GraphLayer_GetAlpha( LCUI_GraphLayer *glayer )
{
	return glayer->graph.alpha;
}

void GraphLayer_InerntAlpha( LCUI_GraphLayer *glayer, LCUI_BOOL flag )
{
	glayer->inherit_alpha = flag;
}

int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y )
{
	if( !glayer ) {
		return -1;
	}
	glayer->pos.x = x;
	glayer->pos.y = y;
	return 0;
}

void GraphLayer_SetPadding( LCUI_GraphLayer *glayer, int top, int right, int bottom, int left )
{
	glayer->content_box.t = top;
	glayer->content_box.r = right;
	glayer->content_box.b = bottom;
	glayer->content_box.l = left;
}

void GraphLayer_SetAlpha( LCUI_GraphLayer *glayer, uchar_t alpha )
{
	glayer->graph.alpha = alpha;
}

int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index )
{	
	if( !glayer ) {
		return -1;
	}
	glayer->z_index = z_index;
	return 0;
}

int GraphLayer_Sort( LCUI_GraphLayer *glayer )
{
	LCUI_GraphLayer *child_a, *child_b;
	int i, j, n;
	
	if( !glayer ) {
		return -1;
	}
	/* 排序前先锁上队列互斥锁 */
	//LinkedList_Lock( &glayer->children );
	n = LinkedList_GetTotal( &glayer->children );
	/* 使用的是冒泡排序法 */
	for(j=0; j<n; ++j)
	for(i=n-1; i>=1; --i) {
		LinkedList_Goto( &glayer->children, i );
		child_a = (LCUI_GraphLayer*)LinkedList_Get( &glayer->children );
		if( !child_a ) {
			continue;
		}
		child_b = (LCUI_GraphLayer*)LinkedList_GetPrev( &glayer->children );
		if( !child_b ) {
			continue;
		}
		if( child_a->z_index > child_b->z_index ) {
			LinkedList_MoveTo( &glayer->children, i-1);
		}
	}
	/* 解开互斥锁 */
	//LinkedList_Unlock( &glayer->children );
	return 0;
}


int GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h )
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


LCUI_GraphLayer* GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y )
{
	return NULL;
}

LCUI_Graph* GraphLayer_GetSelfGraph( LCUI_GraphLayer *glayer )
{
	return &glayer->graph;
}

LCUI_Rect 
GraphLayer_GetValidRect( LCUI_GraphLayer *root_glayer, 
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
	pos.x += glayer->parent->content_box.left;
	pos.y += glayer->parent->content_box.top;
	/* 计算当前有效显示区域 */
	area.x = glayer->parent->content_box.left;
	area.y = glayer->parent->content_box.top;
	/* 区域尺寸则减去内边距 */
	area.width = glayer->parent->graph.w;
	area.width -= glayer->parent->content_box.left;
	area.width -= glayer->parent->content_box.right;
	area.height = glayer->parent->graph.h;
	area.height -= glayer->parent->content_box.top;
	area.height -= glayer->parent->content_box.bottom;
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


LCUI_Pos
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
		pos.x += glayer->parent->content_box.left;
		pos.y += glayer->parent->content_box.top;
	}
	return pos;
}


static int __GraphLayer_GetLayers( LCUI_GraphLayer *root_glayer,
		LCUI_GraphLayer *glayer, LCUI_Rect rect, LinkedList *list )
{
	int i, n;
	LCUI_Pos pos;
	LCUI_Rect tmp;
	LCUI_GraphLayer *child; 
	LinkedList *children;

	if( !glayer ) {
		//_DEBUG_MSG("!glayer\n");
		return -1;
	}
	if( !glayer->visible ) {
		//_DEBUG_MSG("!glayer_visible\n");
		return 1;
	}
	children = &glayer->children;
	/* 从底到顶遍历子部件 */
	n = LinkedList_GetTotal( children );
	//_DEBUG_MSG( "root: %p, cur: %p, child n: %d\n",
	//		root_glayer, glayer, n );
	/* 从尾到首，从底到顶，遍历图层 */
	for( i=n-1; i>=0; --i ) {
		LinkedList_Goto( children, i );
		child = (LCUI_GraphLayer*)LinkedList_Get( children );
		/* 忽略无效或不可见的图层 */
		if( !child || !child->visible ) {
			continue;
		}
		/* 获取子图层的有效区域及全局坐标 */
		tmp = GraphLayer_GetValidRect( root_glayer, child );
		pos = GraphLayer_GetGlobalPos( root_glayer, child );
		DEBUG_MSG( "child: %p, pos: %d,%d, valid rect: %d,%d, %d, %d, rect: %d,%d,%d,%d\n", 
			child, pos.x, pos.y, tmp.x, tmp.y, tmp.width, tmp.height,
			rect.x, rect.y, rect.w, rect.h);
		//Graph_PrintInfo( &child->graph );
		/* 有效区域加上子部件的全局坐标 */
		tmp.x += pos.x;
		tmp.y += pos.y;
		/* 判断区域是否有效 */
		if( tmp.w <= 0 || tmp.h <= 0 ) {
			continue;
		}
		/* 若该有效区域与目标区域重叠，则记录子部件，并进行递归 */
		if( LCUIRect_IsCoverRect(tmp, rect) ) {
			LinkedList_AddData( list, child );
			__GraphLayer_GetLayers(	root_glayer, child, rect, list );
		}
	}
	return 0;
}

int GraphLayer_GetLayers(	LCUI_GraphLayer *glayer, 
				LCUI_Rect rect, LinkedList *list )
{
	//_DEBUG_MSG("rect: %d,%d,%d,%d\n", 
	//rect.x, rect.y, rect.width, rect.height);
	return __GraphLayer_GetLayers( glayer, glayer, rect, list );
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


int GraphLayer_GetGraph( LCUI_GraphLayer *ctnr, LCUI_Graph *graph_buff, LCUI_Rect rect )
{
	int i, n; 
	uchar_t tmp_alpha, alpha;
	LCUI_Pos pos, glayer_pos;
	LCUI_GraphLayer *glayer;
	LinkedList glayer_list;
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
	if( !Graph_IsValid(graph_buff) ) {
	graph_buff->color_type = COLOR_TYPE_ARGB;
		Graph_Create( graph_buff, rect.width, rect.height );
	}

	Graph_Init( &tmp_graph );
	LinkedList_Init( &glayer_list, 0 );
	LinkedList_SetDataNeedFree( &glayer_list, FALSE );
	
	/* 获取rect区域内的图层列表 */
	GraphLayer_GetLayers( ctnr, rect, &glayer_list ); 
	n = LinkedList_GetTotal( &glayer_list ); 
	DEBUG_MSG( "n: %d\n", n );
	/* 若记录数为零，则表明该区域没有图层 */
	if( n <= 0 ) {
		/* 若没有父图层，则填充白色 */
		if( ctnr == NULL ) {
			Graph_FillColor( graph_buff, RGB(255,255,255) );
		} else { /* 否则使用父图层的图形 */
			Graph_Cut( &ctnr->graph, rect, graph_buff );
		}
		/* 销毁记录 */
		LinkedList_Destroy( &glayer_list );
		return 0;
	}
	/* 从顶层到底层遍历图层，排除被其它图层完全遮挡或者自身完全透明的图层 */
	for(i=n-1; i>=0; --i) {
		LinkedList_Goto( &glayer_list, i );
		glayer = (LCUI_GraphLayer*)LinkedList_Get( &glayer_list );
		valid_area = GraphLayer_GetValidRect( ctnr, glayer );
		glayer_pos = GraphLayer_GetGlobalPos( ctnr, glayer );
		valid_area.x += glayer_pos.x;
		valid_area.y += glayer_pos.y;
		alpha = GraphLayer_GetRealAlpha( glayer );
		/* 当前图层的透明度小于255的话，就跳过 */
		if( alpha < 255 ) {
			continue;
		}
		/* 跳过有alpha通道的图层 */
		if( glayer->graph.color_type == COLOR_TYPE_ARGB ) {
			continue;
		}
		/* 如果该图层的有效区域包含目标区域 */
		if( rect.x >= valid_area.x && rect.y >= valid_area.y
		 && rect.x + rect.w <= valid_area.x + valid_area.w
		 && rect.y + rect.h <= valid_area.y + valid_area.h ) {
			/* 移除底层的图层，因为已经被完全遮挡 */
			for(n=i-1;n>=0; --n) {
				LinkedList_Delete( &glayer_list );
			}
			goto skip_loop;
		}
	}
skip_loop:
	n = LinkedList_GetTotal( &glayer_list );
	DEBUG_MSG( "n: %d\n", n );
	if(i <= 0 && ctnr ) {
		Graph_Cut( &ctnr->graph, rect, graph_buff );
	}
	/* 获取图层列表中的图层 */
	for(i=0; i<n; ++i) {
		LinkedList_Goto( &glayer_list, i );
		glayer = (LCUI_GraphLayer*)LinkedList_Get( &glayer_list );
		//_DEBUG_MSG("%p = LinkedList_Get( %p, %d )\n", glayer, &glayer_list, i);
		if( !glayer ) {
			continue;
		}
		DEBUG_MSG("%d,%d,%d,%d\n", glayer->pos.x, glayer->pos.y, glayer->graph.w, glayer->graph.h);
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
	}
	LinkedList_Destroy( &glayer_list );
	return 0;
}

int GraphLayer_Front( LCUI_GraphLayer *glayer )
{
	int i, n, src_pos = -1, des_pos = -1;
	LCUI_GraphLayer *tmp_child;
	LinkedList *children;

	if( !glayer || !glayer->parent ) {
		return -1;
	}

	children = &glayer->parent->children;
	n = LinkedList_GetTotal( children );
	/* 先在队列中找到自己，以及z-index值小于或等于它的第一个图层 */
	for(i=0,src_pos=des_pos=-1; i<n; ++i) {
		LinkedList_Goto( children, i );
		tmp_child = (LCUI_GraphLayer*)LinkedList_Get( children );
		if( !tmp_child ) {
			continue;
		}
		if( tmp_child == glayer ) {
			src_pos = i;
			continue;
		}
		if( des_pos == -1 ) {
			/* 如果该位置的图层的z-index值不大于自己 */
			if( tmp_child->z_index <= glayer->z_index ) {
				/* 如果未找到自己的源位置 */
				if( src_pos == -1 ) {
					des_pos = i;
					continue;
				}
				/* 否则，退出循环，因为已经在前排了 */
				break;
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
	/* 找到的话就移动位置 */
	LinkedList_Goto( children, src_pos );
	LinkedList_MoveTo( children, des_pos );
	return 0;
}

int GraphLayer_Show( LCUI_GraphLayer *glayer )
{
	if( !glayer ) {
		return -1;
	}
	glayer->visible = TRUE;
	return 0;
}

int GraphLayer_Hide( LCUI_GraphLayer *glayer )
{
	if( !glayer ) {
		return -1;
	}
	glayer->visible = FALSE;
	return 0;
}
