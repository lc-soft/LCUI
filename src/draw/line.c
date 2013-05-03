#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

/* 绘制水平线 */
LCUI_API void
Graph_DrawHorizLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Pos start, int end_x )
{
	int k, pos, y, x, end_y;
	LCUI_Rect area;
	LCUI_Graph *des;

	des = Graph_GetQuote(graph);
	area = Graph_GetValidRect(graph);
	start.x = area.x + start.x;
	start.y = area.y + start.y;
	end_y = start.y + size;

	if( start.x < area.x ) {
		end_x -= (area.x-start.x);
		start.x = area.x;
	}
	if( end_x > area.x + area.width ) {
		end_x = area.x + area.width;
	}
	
	if( start.y < area.y ) {
		end_y -= (area.y-start.y);
		start.y = area.y;
	}
	if( end_y > area.y + area.height ) {
		end_y = area.y + area.height;
	}

	for(y=start.y; y<end_y; ++y) {
		k = y * des->width;
		for(x=start.x; x < end_x; ++x) {
			pos = k + x;
			des->rgba[0][pos] = color.red;
			des->rgba[1][pos] = color.green;
			des->rgba[2][pos] = color.blue;
			if( des->have_alpha ) {
				des->rgba[3][pos] = 255;
			}
		}
	}
}

/* 绘制垂直线 */
LCUI_API void
Graph_DrawVertiLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Pos start, int end_y )
{
	int k, pos, y, x, end_x;
	LCUI_Rect area;
	LCUI_Graph *des;

	des = Graph_GetQuote(graph);
	area = Graph_GetValidRect(graph);
	start.x = area.x + start.x;
	start.y = area.y + start.y;
	end_x = start.x + size;

	if( start.x < area.x ) {
		end_x -= (area.x-start.x);
		start.x = area.x;
	}
	if( end_x > area.x + area.width ) {
		end_x = area.x + area.width;
	}
	
	if( start.y < area.y ) {
		end_y -= (area.y-start.y);
		start.y = area.y;
	}
	if( end_y > area.y + area.height ) {
		end_y = area.y + area.height;
	}

	for(y=start.y; y<end_y; ++y) {
		k = y * des->width;
		for(x=start.x; x < end_x; ++x) {
			pos = k + x;
			des->rgba[0][pos] = color.red;
			des->rgba[1][pos] = color.green;
			des->rgba[2][pos] = color.blue;
			if( des->have_alpha ) {
				des->rgba[3][pos] = 255;
			}
		}
	}
}

/* 绘制线条框 */
LCUI_API void
Graph_DrawBoxLine( LCUI_Graph *graph, LCUI_RGB color, int size, LCUI_Rect rect )
{
	int end;
	LCUI_Pos start;
	start.x = rect.x;
	start.y = rect.y;
	end = rect.x+rect.width;
	Graph_DrawHorizLine( graph, color, size, start, end );
	start.y = rect.y+rect.height-size;
	Graph_DrawHorizLine( graph, color, size, start, end );

	start.y = rect.y;
	end = rect.y+rect.height;
	Graph_DrawVertiLine( graph, color, size, start, end );
	start.x = rect.x+rect.width-size;
	Graph_DrawVertiLine( graph, color, size, start, end );
}
