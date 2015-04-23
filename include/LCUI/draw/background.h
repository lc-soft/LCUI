/* ***************************************************************************
 * background.h -- graph background image draw support.
 * 
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * background.h -- 背景图像绘制支持
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_DRAW_BACKGROUND_H__
#define __LCUI_DRAW_BACKGROUND_H__

LCUI_BEGIN_HEADER

/** 初始化背景绘制参数 */
LCUI_API void Background_Init( LCUI_Background *bg );

/**
* 在图层上绘制背景
* @param graph		需进行绘制的图层
* @param bg		背景样式数据
* @param box_size	容器尺寸
* @param rect		背景中实际需要绘制的区域，如果为NULL，则绘制整个背景
*/
LCUI_API void Graph_DrawBackground(
	LCUI_PaintContext	paint,
	const LCUI_Size		*box_size,
	LCUI_Background		*bg
);

LCUI_END_HEADER

#endif