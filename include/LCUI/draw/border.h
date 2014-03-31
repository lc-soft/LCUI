/* ***************************************************************************
 * border.h -- Graph border drawing operation set.
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
 * border.h -- 图形的边框绘制操作集
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
 
#ifndef __LCUI_DRAW_BORDER_H__
#define __LCUI_DRAW_BORDER_H__

LCUI_BEGIN_HEADER

/*************** 边框风格 *****************/
typedef enum _BORDER_STYLE
{
	BORDER_STYLE_NONE,	/* 无边框 */
	BORDER_STYLE_SOLID,	/* 实线 */
	BORDER_STYLE_DOTTED,	/* 点状 */
	BORDER_STYLE_DOUBLE,	/* 双线 */
	BORDER_STYLE_DASHED,	/* 虚线 */
}
BORDER_STYLE;
/*****************************************/

/* 完整的边框信息 */
typedef struct _LCUI_Border
{
	unsigned int top_width;
	unsigned int bottom_width;
	unsigned int left_width;
	unsigned int right_width;
	BORDER_STYLE top_style;
	BORDER_STYLE bottom_style;
	BORDER_STYLE left_style;
	BORDER_STYLE right_style;
	LCUI_RGB top_color;
	LCUI_RGB bottom_color;
	LCUI_RGB left_color;
	LCUI_RGB right_color;
	unsigned int top_left_radius;
	unsigned int top_right_radius;
	unsigned int bottom_left_radius;
	unsigned int bottom_right_radius;
}
LCUI_Border;

LCUI_API void Border_Init( LCUI_Border *border );
/* 初始化边框数据 */

LCUI_API LCUI_Border Border( unsigned int width_px, BORDER_STYLE style, LCUI_RGB color );
/* 简单的设置边框样式，并获取该样式数据 */

LCUI_API void Border_Radius( LCUI_Border *border, unsigned int radius );
/* 设置边框的圆角半径 */

/* 只绘制目标区域内的边框 */
LCUI_API int Graph_DrawBorderEx( LCUI_Graph *des, LCUI_Border border,
							LCUI_Rect area );

/* 简单的为图形边缘绘制边框 */
LCUI_API int Graph_DrawBorder( LCUI_Graph *des, LCUI_Border border );

LCUI_END_HEADER

#endif
