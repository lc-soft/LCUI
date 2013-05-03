/* ***************************************************************************
 * LCUI_GraphLayer.h -- GraphLayer operation set.
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
 * LCUI_GraphLayer.h -- 图层的操作集
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
#ifndef __LCUI_GRAPHLAYER_H__
#define __LCUI_GRAPHLAYER_H__

LCUI_BEGIN_HEADER

typedef struct _LCUI_GraphLayer LCUI_GraphLayer;

struct _LCUI_GraphLayer
{
	LCUI_BOOL visible;		/* 图层是否可见 */
	LCUI_BOOL inherit_alpha;	/* 是否继承父图层的透明度 */
	int z_index;			/* 图层的堆叠顺序，值越大，图层显示位置越靠前 */
	LCUI_Pos pos;			/* 图层的xy轴坐标 */
	
	LCUI_GraphLayer *parent;	/* 该图层的容器图层 */
	LCUI_Queue child;		/* 该图层中内的子图层记录 */
	LCUI_Graph graph;		/* 图层像素数据 */
};

/* 打印子图层信息列表 */
LCUI_API int
GraphLayer_PrintChildList( LCUI_GraphLayer *glayer );

/* 将子图层从父图层中的子图层队列中移除 */
LCUI_API int
GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer );

/* 释放图层占用的内存资源 */
LCUI_API void
GraphLayer_Free( LCUI_GraphLayer *glayer );

/* 创建新的图层 */
LCUI_API LCUI_GraphLayer*
GraphLayer_New( void );

/* 添加子图层至容器图层中 */
LCUI_API int
GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
			LCUI_GraphLayer *glayer );

/* 移动子图层至新的容器图层中 */
LCUI_API int
GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
			LCUI_GraphLayer *glayer );

/* 获取图层矩形 */
LCUI_API LCUI_Rect
GraphLayer_GetRect( LCUI_GraphLayer *glayer );

/* 获取图层尺寸 */
LCUI_API LCUI_Size
GraphLayer_GetSize( LCUI_GraphLayer *glayer );

/* 获取图层的全局透明度 */
LCUI_API uchar_t
GraphLayer_GetAlpha( LCUI_GraphLayer *glayer );

/* 图层是否继承父图层的透明度 */
LCUI_API void
GraphLayer_InerntAlpha( LCUI_GraphLayer *glayer, LCUI_BOOL flag );

/* 设定图层的XY轴坐标 */
LCUI_API int
GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y );

/* 设定图层的全局透明度 */
LCUI_API void
GraphLayer_SetAlpha( LCUI_GraphLayer *glayer, uchar_t alpha );

/* 设定图层的Z轴坐标，调用此函数后，需要调用GraphLayer_Sort函数对图层列表进行排序 */
LCUI_API int
GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index );

/* 根据子图层的z-index值，对目标图层的子图层进行排序 */
LCUI_API int
GraphLayer_Sort( LCUI_GraphLayer *glayer );

/* 调整图层的大小 */
LCUI_API int
GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h );

/* 获取指定图层中指定坐标上存在的子图层 */
LCUI_API LCUI_GraphLayer*
GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y );

/* 获取指向图层自身图形数据的指针 */
LCUI_API LCUI_Graph*
GraphLayer_GetSelfGraph( LCUI_GraphLayer *glayer );

/* 获取指定根图层中的子图层的有效区域 */
LCUI_API LCUI_Rect
GraphLayer_GetValidRect( LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer );

/* 指定根容器图层，获取当前子图层相对于根容器图层的全局坐标 */
LCUI_API LCUI_Pos
GraphLayer_GetGlobalPos( LCUI_GraphLayer *root_glayer, LCUI_GraphLayer *glayer );

/* 获取与图层中指定区域内层叠的子图层 */
LCUI_API int
GraphLayer_GetLayers(
	LCUI_GraphLayer *glayer, 
	LCUI_Rect rect, LCUI_Queue *queue );

/* 获取该图层和子图层混合后的图形数据 */
LCUI_API int
GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
			LCUI_Graph *graph_buff,
			LCUI_Rect rect );


/* 将指定图层显示在同等z-index值图层的前端 */
LCUI_API int
GraphLayer_Front( LCUI_GraphLayer *glayer );

/* 显示图层 */
LCUI_API int
GraphLayer_Show( LCUI_GraphLayer *glayer );

/* 隐藏图层 */
LCUI_API int
GraphLayer_Hide( LCUI_GraphLayer *glayer );

LCUI_END_HEADER

#endif
