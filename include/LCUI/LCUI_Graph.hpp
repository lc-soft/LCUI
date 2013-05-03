/* ***************************************************************************
 * LCUI_Graph.hpp -- C++ class for base graphics handling module
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
 * LCUI_Graph.hpp -- 基本图形处理模块的C++类
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
#ifndef __LCUI_GRAPH_HPP__
#define __LCUI_GRAPH_HPP__
#include LC_DRAW_H
#include LC_RES_H

#ifdef __cplusplus
class LCUIGraph {
public:
	LCUIGraph( void );
	LCUIGraph( int, int, LCUI_BOOL );
	int create( int, int, LCUI_BOOL );
	LCUI_Graph *getGraph( void );
private:
	LCUI_Graph graph;
};

LCUIGraph::LCUIGraph( void )
{
	Graph_Init( &graph );
}

LCUIGraph::LCUIGraph( int w, int h, LCUI_BOOL have_alpha = TRUE )
{
	Graph_Init( &graph );
	graph.have_alpha = have_alpha;
	Graph_Create( &graph, w, h );
}

int LCUIGraph::create( int w, int h, LCUI_BOOL have_alpha = TRUE )
{
	graph.have_alpha = have_alpha;
	return Graph_Create( &graph, w, h );
}

LCUI_Graph* LCUIGraph::getGraph( void )
{
	return &graph;
}
#endif

#endif
