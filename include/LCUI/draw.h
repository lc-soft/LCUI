/* ****************************************************************************
 * LCUI_Draw.h -- The graphics draw module of LCUI.
 * 
 * Copyright (C) 2012-2016 by
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
 * LCUI_Draw.h -- LCUI的图形绘制模块
 *
 * 版权所有 (C) 2012-2016 归属于 
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

#ifndef LCUI_DRAW_H
#define LCUI_DRAW_H

LCUI_BEGIN_HEADER

#include <LCUI/draw/line.h>
#include <LCUI/draw/border.h>
#include <LCUI/draw/boxshadow.h>
#include <LCUI/draw/background.h>

/* 
 * 功能：旋转图形
 * 说明：指定旋转中心点坐标以及旋转角度，即可得到旋转后的图形
 * 本源代码参考自互联网相关代码
 * 算法有待优化完善。
 */
LCUI_API int Graph_Rotate( LCUI_Graph *src, int rotate_angle, LCUI_Graph *des );

/** 对图像进行高斯模糊处理 */
LCUI_API int GaussianSmooth( LCUI_Graph *src, LCUI_Graph *des, double sigma );

/* 对图像进行模糊处理 */
LCUI_API int Graph_Smooth( LCUI_Graph *src, LCUI_Graph *des, double sigma );

LCUI_END_HEADER

#endif
