// 测试 LCUI_GraphLayer.c 模块

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_GRAPHLAYER_H

#define OUTPUT_GRAPHFILE "output_graph.png"

int main()
{
	LCUI_Graph graph_buff;
	LCUI_GraphLayer *root_glayer;
	LCUI_GraphLayer *a_glayer, *b_glayer, *c_glayer;
	
	Graph_Init( &graph_buff );
	/* 新建图层 */
	root_glayer = GraphLayer_New();
	a_glayer = GraphLayer_New();
	b_glayer = GraphLayer_New();
	c_glayer = GraphLayer_New();
	/* 调整图层尺寸 */
	GraphLayer_Resize( root_glayer, 320, 240 );
	GraphLayer_Resize( a_glayer, 200, 150 );
	GraphLayer_Resize( b_glayer, 100, 100 );
	GraphLayer_Resize( c_glayer, 80, 80 );
	/* 填充颜色 */
	Graph_Fill_Color( &root_glayer->graph, RGB(255,255,255) );
	Graph_Fill_Color( &a_glayer->graph, RGB(255,50,50) );
	Graph_Fill_Color( &b_glayer->graph, RGB(50,255,50) );
	Graph_Fill_Color( &c_glayer->graph, RGB(50,50,255) );
	/* 建立父子图层关系 */
	GraphLayer_AddChild( b_glayer, c_glayer );
	GraphLayer_AddChild( root_glayer, a_glayer );
	GraphLayer_AddChild( root_glayer, b_glayer );
	/* 调整图层坐标 */
	GraphLayer_SetPos( a_glayer, 50, 50 );
	GraphLayer_SetPos( b_glayer, 175, 125 );
	GraphLayer_SetPos( c_glayer, -15, 30 );
	/* 显示图层 */
	GraphLayer_Show( root_glayer );
	GraphLayer_Show( a_glayer );
	GraphLayer_Show( b_glayer );
	GraphLayer_Show( c_glayer );
	/* 获取根图层指定区域内实际显示的图形 */
	GraphLayer_GetGraph( root_glayer, &graph_buff, Rect(0,0,320,240) );
	/* 写入至文件 */
	write_png( OUTPUT_GRAPHFILE, &graph_buff );
	printf( "please see file: %s \n", OUTPUT_GRAPHFILE );
	GraphLayer_Free( root_glayer );
}
