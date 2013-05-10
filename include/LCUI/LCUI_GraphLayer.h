/** ******************************************************************************
 * @file	LCUI_GraphLayer.h
 * @brief	GraphLayer operation set.
 * @author  Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by													\n
 * Liu Chao																		\n
 * 																				\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.									\n
 * 																				\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)				\n
 * 																				\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.					\n
 *  																			\n
 * The LCUI project is distributed in the hope that it will be useful, but 		\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.		\n
 * 																				\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	LCUI_GraphLayer.h
 * @brief	图层的操作集.
 * @author  刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于												\n
 * 刘超																			\n
 * 																				\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 																				\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)								\n
 * 																				\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。		\n
 * 																				\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 * 用途的隐含担保，详情请参照GPLv2许可协议。									\n
 * 																				\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 								\n
 * ******************************************************************************/
#ifndef __LCUI_GRAPHLAYER_H__
#define __LCUI_GRAPHLAYER_H__

LCUI_BEGIN_HEADER

typedef struct LCUI_GraphLayer_ LCUI_GraphLayer;

/// 图层(GraphLayer)的结构体
struct LCUI_GraphLayer_
{
	LCUI_BOOL visible;			///< 图层是否可见
	LCUI_BOOL inherit_alpha;	///< 是否继承父图层的透明度
	int z_index;				///< 图层的堆叠顺序，值越大，图层显示位置越靠前
	LCUI_Pos pos;				///< 图层的xy轴坐标
	LCUI_GraphLayer *parent;	///< 该图层的容器图层
	LCUI_Queue child;			///< 该图层中内的子图层记录
	LCUI_Graph graph;			///< 图层像素数据
};

/**
 * 打印子图层信息列表   
 *   
 * 	打印指定图层中的各个子图层信息，包括坐标、尺寸、z-index值。
 * 		@param		glayer 目标图层
 * 		@return		正常则返回0，如果目标图层无效，或者无法获取子图层列表，则返回-1 
 * 		@note		通常在调试时，用该函数打印各个图层的情况，以检测图层是否异常。
 */ 
LCUI_API int GraphLayer_PrintChildList( LCUI_GraphLayer *glayer );

/**
 * 将子图层从父图层中的子图层队列中移除  
 *   
 * 	移除指定图层在父图层中的记录，也就是断开父子图层的父子关系。
 * 		@param		child_glayer 指定的子图层
 * 		@return		正常则返回0，如果目标图层无效，或者无法获取子图层列表，则返回-1，子图层在父图层中没有记录，则返回1。
 */
LCUI_API int GraphLayer_DeleteChild( LCUI_GraphLayer *child_glayer );

/**
 * 释放图层占用的内存资源
 *   
 * 	移除图层在父图层中的记录，并释放图层占用的内存资源。
 * 		@param	glayer 目标图层
 */
LCUI_API void GraphLayer_Free( LCUI_GraphLayer *glayer );

/**
 * 创建新的图层
 *  	@return	正常则返回图层指针，出错返回NULL。
 * 		@note 新建的图层，默认是不可见的。
 */
LCUI_API LCUI_GraphLayer* GraphLayer_New( void );

/**
 * 为容器图层添加子图层
 * 		@param	des_ctnr 作为容器的图层
 * 		@param	glayer	 子图层
 *  	@return	正常则返回0，容器图层无效则返回-1，子图层无效，或者已经有父图层，则返回-2。
 */
LCUI_API int GraphLayer_AddChild(	LCUI_GraphLayer *des_ctnr,
									LCUI_GraphLayer *glayer );

/**
 * 移动子图层至新的容器图层中
 * 		@param	new_ctnr 作为新容器的图层
 * 		@param	glayer	 子图层
 *  	@return	正常则返回0，若在父图层中移除记录失败则返回-1，若在新图层中的添加子图层记录失败则返回-2。
 */
LCUI_API int GraphLayer_MoveChild(	LCUI_GraphLayer *new_ctnr, 
									LCUI_GraphLayer *glayer );

/**
 * 获取图层的矩形信息
 * 		@param		glayer	 图层
 *  	@return		该图层的矩形信息
 *		@warning	glayer 参数不能为NULL，否则会因数据访问越界而导致程序崩溃。
 */
LCUI_API LCUI_Rect GraphLayer_GetRect( LCUI_GraphLayer *glayer );

/**
 * 获取图层的尺寸信息
 * 		@param		glayer	 图层
 *  	@return		该图层的尺寸信息
 *		@warning	glayer 参数不能为NULL，否则会因数据访问越界而导致程序崩溃。
 */
LCUI_API LCUI_Size GraphLayer_GetSize( LCUI_GraphLayer *glayer );

/**
 * 获取图层的尺寸信息
 * 		@param		glayer	 图层
 *  	@return		图层的全局透明度
 *		@warning	glayer 参数不能为NULL，否则会因数据访问越界而导致程序崩溃。
 */
LCUI_API uchar_t GraphLayer_GetAlpha( LCUI_GraphLayer *glayer );

/**
 * 设定图层是否继承父图层的透明度
 * 		@param		glayer	 图层
 * 		@param		flag	 取值为TRUE时则继承，为FALSE时则不继承
 *		@warning	glayer 参数不能为NULL，否则会因数据访问越界而导致程序崩溃。
 * 		@note		在进行图层混合时，用于决定该子图层的透明度是否受父图层的透明度的影响。
 */
LCUI_API void GraphLayer_InerntAlpha( LCUI_GraphLayer *glayer, LCUI_BOOL flag );

/**
 * 设定图层的XY轴坐标
 * 		@param		glayer	图层
 * 		@param		x		X轴坐标，水平坐标
 *		@param		y		Y轴坐标，垂直坐标
 * 		@return		正常返回0，图层无效则返回-1
 */
LCUI_API int GraphLayer_SetPos( LCUI_GraphLayer *glayer, int x, int y );

/**
 * 设定图层的全局透明度
 * 		@param	glayer	图层
 * 		@param	alpha	透明度，取值范围为0~255
 */
LCUI_API void GraphLayer_SetAlpha( LCUI_GraphLayer *glayer, uchar_t alpha );

/**
 * 设定图层的Z轴坐标，即z-index值
 * 
 * 		@param	glayer	图层
 * 		@param	z_index	z-index值，它决定图层的堆叠顺序，值越大，越靠近顶层，反之越靠近底层。
 *		@note	调用此函数后，需要调用GraphLayer_Sort()函数对图层列表进行排序
 * 		@return	正常返回0，图层无效则返回-1
 */
LCUI_API int GraphLayer_SetZIndex( LCUI_GraphLayer *glayer, int z_index );

/**
 * 对目标图层的子图层进行排序
 * 
 * 排序是根据子图层的z-index值进行的，z-index值越大，越靠近队列前端。
 * 		@param	glayer	需要对子图层进行排序的图层
 * 		@return	正常返回0，图层无效则返回-1
 */
LCUI_API int GraphLayer_Sort( LCUI_GraphLayer *glayer );

/**
 * 调整图层的尺寸大小
 * 
 * 		@param	glayer	需要调整尺寸的图层
 *		@param	w		新的宽度
 *		@param	h		新的高度
 *		@retval -2	无法调整图层尺寸
 * 		@retval	-1	图层无效
 *		@retval 0	正常
 *		@retval 1	尺寸没有变化
 */
LCUI_API int GraphLayer_Resize( LCUI_GraphLayer *glayer, int w, int h );

/**
 * 获取指定图层中指定坐标上存在的子图层
 * 
 * 		@param ctnr	作为容器的图层
 *		@param x	x轴坐标
 *		@param y	y轴坐标
 * 		@return		正常返回子图层，否则返回NULL
 *		@note		由于没有该功能需求，此函数无实现代码。
 */
LCUI_API LCUI_GraphLayer* GraphLayer_ChildAt( LCUI_GraphLayer *ctnr, int x, int y );

/**
 * 获取指向图层自身图形数据的指针
 * 
 * 		@param glayer	目标图层
 * 		@return			图层自身图形数据的指针
 */
LCUI_API LCUI_Graph* GraphLayer_GetSelfGraph( LCUI_GraphLayer *glayer );

/**
 * 获取指定根图层中的子图层的有效区域
 * 
 * 		@param root_glayer	根图层
 * 		@param glayer		当前图层
 * 		@return				图层相对于根图层的实际有效显示范围
 */
LCUI_API LCUI_Rect GraphLayer_GetValidRect(	LCUI_GraphLayer *root_glayer,
											LCUI_GraphLayer *glayer );

/**
 * 获取当前子图层相对于根容器图层的全局坐标
 * 
 * 		@param root_glayer	当前图层的根图层
 * 		@param glayer		当前图层
 * 		@return				图层相对于根图层的全局坐标
 */
LCUI_API LCUI_Pos
GraphLayer_GetGlobalPos(	LCUI_GraphLayer *root_glayer,
							LCUI_GraphLayer *glayer );

/**
 * 获取与图层中指定区域内层叠的子图层列表
 * 
 * 		@param glayer	图层
 * 		@param rect		区域
 * 		@param queue	用于存放在区域内的子图层列表
 */
LCUI_API int GraphLayer_GetLayers(	LCUI_GraphLayer *glayer, 
									LCUI_Rect rect,
									LCUI_Queue *queue );


/**
 * 获取该图层和子图层混合后的图形数据
 * 
 * 		@param ctnr				作为容器的图层
 * 		@param[out] graph_buff	用于存放混合后的图形数据
 * 		@param rect				需要捕获的区域
 *		@retval -1		指定的区域超出容器图层的范围
 *		@retval -2		区域大小无效
 * 		@retval 0		已正常获取到区域内的图形
 */
LCUI_API int GraphLayer_GetGraph(	LCUI_GraphLayer *ctnr, 
									LCUI_Graph *graph_buff,
									LCUI_Rect rect );

/**
 * 前置图层
 * 
 *  将指定图层显示在同等z-index值图层的前端
 * 
 * 		@param glayer	需要前置的图层
 *		@retval -2		在父图层中没有找到该图层的记录
 *		@retval -1		图层无效，或者该图层的父图层无效
 *		@retval 1		位置没有变化
 * 		@retval 0		已经前置
 */
LCUI_API int GraphLayer_Front( LCUI_GraphLayer *glayer );

/**
 * 显示图层
 * 
 *  标记图层为可见
 * 
 * 		@param glayer	图层
 *		@retval -1		图层无效
 * 		@retval 0		正常
 */
LCUI_API int GraphLayer_Show( LCUI_GraphLayer *glayer );

/**
 * 隐藏图层
 * 
 *  标记图层为可见
 * 
 * 		@param glayer	图层
 *		@retval -1		图层无效
 * 		@retval 0		正常
 */
LCUI_API int GraphLayer_Hide( LCUI_GraphLayer *glayer );

LCUI_END_HEADER

#endif
