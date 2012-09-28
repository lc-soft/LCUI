#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

int Graph_Draw_Border(LCUI_Graph *src,LCUI_RGB color, LCUI_Border border)
/* 功能：为图形边缘绘制矩形边框 */
{
	int x,y,count, k, start_x,start_y;
	if(Graph_Valid(src)) {
		/* 绘制上边的线 */
		Graph_Lock(src, 1);
		for(y=0;y<border.top;++y) {
			k = y * src->width;
			for(x = 0; x < src->width; ++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(Graph_Have_Alpha(src)) 
					src->rgba[3][count] = 255;
			}
		}
		start_y = src->height - border.bottom;
		/* 绘制下边的线 */
		for(y=0;y<border.bottom;++y) {
			k = (start_y+y) * src->width;
			for(x=0;x<src->width;++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(Graph_Have_Alpha(src)) 
					src->rgba[3][count] = 255;
			}
		}
		/* 绘制左边的线 */
		for(y=0;y<src->height;++y) {
			k = y * src->width;
			for(x=0;x<border.left;++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(Graph_Have_Alpha(src)) 
					src->rgba[3][count] = 255;
			}
		}
		/* 绘制右边的线 */
		start_x = src->width - border.right;
		for(y=0;y<src->height;++y) {
			k = y * src->width + start_x;
			for(x=0;x<border.right;++x) {
				count = k + x;
				src->rgba[0][count] = color.red;
				src->rgba[1][count] = color.green;
				src->rgba[2][count] = color.blue;
				if(Graph_Have_Alpha(src)) 
					src->rgba[3][count] = 255;
			}
		}
		/* 边框线绘制完成 */
		Graph_Unlock(src);
	}
	else return -1;
	return 0;
}
