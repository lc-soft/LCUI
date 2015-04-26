#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

/* 绘制水平线 */
LCUI_API void
Graph_DrawHorizLine( LCUI_Graph *graph, LCUI_Color color, int size, LCUI_Pos start, int len )
{
	int y, x;
	LCUI_Rect area;
	LCUI_Graph *des;

	des = Graph_GetQuote(graph);
	Graph_GetValidRect( graph, &area );
	start.x = area.x + start.x;
	start.y = area.y + start.y;

	if( start.x < area.x ) {
		len -= (area.x-start.x);
		start.x = area.x;
	}
	if( len > area.x + area.width ) {
		len = area.x + area.width;
	}
	if( start.y < area.y ) {
		size -= (area.y-start.y);
		start.y = area.y;
	}
	if( start.y + size > area.y + area.height ) {
		size = area.y + area.height - start.y;
	}
	if( des->color_type == COLOR_TYPE_ARGB ) {
		LCUI_ARGB *pPixel, *pRowPixel;
		pRowPixel = des->argb + start.y*des->w + start.x;
		for( y=0; y<size; ++y ) {
			pPixel = pRowPixel;
			for( x=0; x<len; ++x ) {
				pPixel->b = color.blue;
				pPixel->g = color.green;
				pPixel->r = color.red;
				pPixel->a = 255;
				++pPixel;
			}
			pRowPixel += des->w;
		}
	} else {
		uchar_t *pByte, *pRowByte;
		pRowByte = des->bytes + (start.y*des->w + start.x)*3;
		for( y=0; y<size; ++y ) {
			pByte = pRowByte;
			for( x=0; x<len; ++x ) {
				*pByte++ = color.blue;
				*pByte++ = color.green;
				*pByte++ = color.red;
			}
			pRowByte += des->w*3;
		}
	}
}

/* 绘制垂直线 */
LCUI_API void
Graph_DrawVertiLine( LCUI_Graph *graph, LCUI_Color color, int size, LCUI_Pos start, int len )
{
	int y, x;
	LCUI_Rect area;
	LCUI_Graph *des;

	des = Graph_GetQuote(graph);
	Graph_GetValidRect( graph, &area );
	start.x = area.x + start.x;
	start.y = area.y + start.y;

	if( start.x < area.x ) {
		size -= (area.x-start.x);
		start.x = area.x;
	}
	if( start.x + size > area.x + area.width ) {
		size = area.x + area.width - start.x;
	}

	if( start.y < area.y ) {
		len -= (area.y-start.y);
		start.y = area.y;
	}
	if( start.y + len > area.y + area.height ) {
		len = area.y + area.height - start.y;
	}

	if( des->color_type == COLOR_TYPE_ARGB ) {
		LCUI_ARGB *pPixel, *pRowPixel;
		pRowPixel = des->argb + start.y*des->w + start.x;
		for( y=0; y<len; ++y ) {
			pPixel = pRowPixel;
			for( x=0; x<size; ++x ) {
				pPixel->b = color.blue;
				pPixel->g = color.green;
				pPixel->r = color.red;
				pPixel->a = 255;
				++pPixel;
			}
			pRowPixel += des->w;
		}
	} else {
		uchar_t *pByte, *pRowByte;
		pRowByte = des->bytes + (start.y*des->w + start.x)*3;
		for( y=0; y<len; ++y ) {
			pByte = pRowByte;
			for( x=0; x<size; ++x ) {
				*pByte++ = color.blue;
				*pByte++ = color.green;
				*pByte++ = color.red;
			}
			pRowByte += des->w*3;
		}
	}
}
