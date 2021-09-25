﻿#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>

void pd_graph_draw_horiz_line(pd_canvas_t *graph, pd_color_t color, int size,
			 LCUI_Pos start, int len)
{
	int y, x;
	pd_rect_t area;
	pd_canvas_t *des;

	des = pd_graph_get_quote(graph);
	pd_graph_get_valid_rect(graph, &area);
	start.x = area.x + start.x;
	start.y = area.y + start.y;

	if (start.x < area.x) {
		len -= (area.x - start.x);
		start.x = area.x;
	}
	if (len > area.x + area.width) {
		len = area.x + area.width;
	}
	if (start.y < area.y) {
		size -= (area.y - start.y);
		start.y = area.y;
	}
	if (start.y + size > area.y + area.height) {
		size = area.y + area.height - start.y;
	}
	if (des->color_type == LCUI_COLOR_TYPE_ARGB) {
		pd_color_t *pPixel, *pRowPixel;
		pRowPixel = des->argb + start.y*des->width + start.x;
		for (y = 0; y < size; ++y) {
			pPixel = pRowPixel;
			for (x = 0; x < len; ++x) {
				pPixel->b = color.blue;
				pPixel->g = color.green;
				pPixel->r = color.red;
				pPixel->a = 255;
				++pPixel;
			}
			pRowPixel += des->width;
		}
	} else {
		uchar_t *pByte, *pRowByte;
		pRowByte = des->bytes + start.y*des->bytes_per_row + start.x * 3;
		for (y = 0; y < size; ++y) {
			pByte = pRowByte;
			for (x = 0; x < len; ++x) {
				*pByte++ = color.blue;
				*pByte++ = color.green;
				*pByte++ = color.red;
			}
			pRowByte += des->bytes_per_row;
		}
	}
}

void pd_graph_draw_verti_line(pd_canvas_t *graph, pd_color_t color,
				  int size, LCUI_Pos start, int len)
{
	int y, x;
	pd_rect_t area;
	pd_canvas_t *des;

	des = pd_graph_get_quote(graph);
	pd_graph_get_valid_rect(graph, &area);
	start.x = area.x + start.x;
	start.y = area.y + start.y;

	if (start.x < area.x) {
		size -= (area.x - start.x);
		start.x = area.x;
	}
	if (start.x + size > area.x + area.width) {
		size = area.x + area.width - start.x;
	}

	if (start.y < area.y) {
		len -= (area.y - start.y);
		start.y = area.y;
	}
	if (start.y + len > area.y + area.height) {
		len = area.y + area.height - start.y;
	}

	if (des->color_type == LCUI_COLOR_TYPE_ARGB) {
		pd_color_t *pPixel, *pRowPixel;
		pRowPixel = des->argb + start.y*des->width + start.x;
		for (y = 0; y < len; ++y) {
			pPixel = pRowPixel;
			for (x = 0; x < size; ++x) {
				pPixel->b = color.blue;
				pPixel->g = color.green;
				pPixel->r = color.red;
				pPixel->a = 255;
				++pPixel;
			}
			pRowPixel += des->width;
		}
	} else {
		uchar_t *pByte, *pRowByte;
		pRowByte = des->bytes + start.y*des->bytes_per_row + start.x * 3;
		for (y = 0; y < len; ++y) {
			pByte = pRowByte;
			for (x = 0; x < size; ++x) {
				*pByte++ = color.blue;
				*pByte++ = color.green;
				*pByte++ = color.red;
			}
			pRowByte += des->bytes_per_row;
		}
	}
}
