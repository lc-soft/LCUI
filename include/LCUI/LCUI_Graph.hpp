/* ***************************************************************************
 * LCUI_Graph.hpp -- C++ class for base graphics handling module
 * 
 * Copyright (C) 2012 by
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
 * 版权所有 (C) 2012 归属于 
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

class LCUIGraph
{
public:
	LCUIGraph();
	~LCUIGraph();
	LCUI_BOOL isValid( void );
	int isOpaque( void );
	LCUI_BOOL haveAlpha( void );
	int create( int, int );
	void copyTo( LCUIGraph & );
	int quote( LCUIGraph &, LCUI_Rect );
	int mix( LCUIGraph &, LCUI_Pos );
	int tile( LCUIGraph & );
	int fillImage( LCUI_Graph *, int, LCUI_RGB );
	int fillAlpha( LCUI_Graph *, uchar_t );
	int fillColor( LCUI_RGB );

	int loadFromFile( const char * );
	int writeToFile( const char * );
	
	LCUI_Graph *getGraph( void );
	LCUI_Graph *getQuoteGraph( void );
	LCUI_Rect getGraphValidRect( void );
private:
	LCUI_Graph graph;
};

LCUIGraph::LCUIGraph()
{
	Graph_Init( &graph );
}
LCUIGraph::~LCUIGraph()
{
	Graph_Free( &graph );
}
LCUI_BOOL LCUIGraph::isValid( void )
{
	return Graph_Valid( &graph );
}
int LCUIGraph::isOpaque( void )
{
	return Graph_Is_Opaque( &graph );
}
LCUI_BOOL LCUIGraph::haveAlpha( void )
{
	return Graph_Have_Alpha( &graph );
}
int LCUIGraph::create( int w, int h )
{
	return Graph_Create( &graph, w, h );
}
void LCUIGraph::copyTo( LCUIGraph &des_graph )
{
	Graph_Copy( des_graph.getGraph(), &graph );
}
int LCUIGraph::quote( LCUIGraph &src_graph, LCUI_Rect area )
{
	Quote_Graph( &graph, src_graph.getGraph(), area );
}

int LCUIGraph::mix( LCUIGraph &fore_graph, LCUI_Pos pos )
{
	Graph_Mix( &graph, fore_graph.getGraph(), pos );
}
int LCUIGraph::tile( LCUIGraph &chunk )
{
	return Graph_Tile( chunk.getGraph(), &graph, graph.width, graph.height );
}
int LCUIGraph::fillImage( LCUI_Graph *bg, int flag, LCUI_RGB color )
{
	return Graph_Fill_Image( &graph, bg, flag, color );
}
int LCUIGraph::fillAlpha( LCUI_Graph *src, uchar_t alpha )
{
	return Graph_Fill_Alpha( &graph, alpha );
}
int LCUIGraph::fillColor( LCUI_RGB color )
{
	return Graph_Fill_Color( &graph, color );
}

int LCUIGraph::loadFromFile( const char *in_imgfile )
{
	return Load_Image( in_imgfile, &graph );
}
int LCUIGraph::writeToFile( const char *out_imgfile )
{
	/* 暂时只能创建png文件 */
	return write_png( out_imgfile, &graph );
}

LCUI_Graph *LCUIGraph::getGraph( void )
{
	return &graph;
}
LCUI_Graph *LCUIGraph::getQuoteGraph( void )
{
	return Get_Quote_Graph( &graph );
}
LCUI_Rect LCUIGraph::getGraphValidRect( void )
{
	return Get_Graph_Valid_Rect( &graph );
}
#endif

#endif
