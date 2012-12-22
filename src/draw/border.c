#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

LCUI_Border Border( int width_px, BORDER_STYLE style, LCUI_RGB color )
/* 简单的设置边框样式，并获取该样式数据 */
{
	LCUI_Border border;
	border.top_width = width_px;
	border.bottom_width = width_px;
	border.left_width = width_px;
	border.right_width = width_px;
	border.top_style = style;
	border.bottom_style = style;
	border.left_style = style;
	border.right_style = style;
	border.top_color = color;
	border.bottom_color = color;
	border.left_color = color;
	border.right_color = color;
	return border; 
}

void Border_Init( LCUI_Border *border )
/* 初始化边框数据 */
{
	border->top_width = 0;
	border->bottom_width = 0;
	border->left_width = 0;
	border->right_width = 0;
	border->top_style = BORDER_STYLE_NONE;
	border->bottom_style = BORDER_STYLE_NONE;
	border->left_style = BORDER_STYLE_NONE;
	border->right_style = BORDER_STYLE_NONE;
	border->top_color = RGB(0,0,0);
	border->bottom_color = RGB(0,0,0);
	border->left_color = RGB(0,0,0);
	border->right_color = RGB(0,0,0);
}

int Graph_Draw_Border( LCUI_Graph *des, LCUI_Border border )
/* 简单的为图形边缘绘制边框 */
{
	if( !Graph_Valid(des) ) {
		return -1;
	}
	
	int x,y,count, k, start_x,start_y;
	
	Graph_Lock(des, 1);
	/* 绘制上边的线 */
	for(y=0;y<border.top_width;++y) {
		k = y * des->width;
		for(x = 0; x < des->width; ++x) {
			count = k + x;
			des->rgba[0][count] = border.top_color.red;
			des->rgba[1][count] = border.top_color.green;
			des->rgba[2][count] = border.top_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	start_y = des->height - border.bottom_width;
	/* 绘制下边的线 */
	for(y=0;y<border.bottom_width;++y) {
		k = (start_y+y) * des->width;
		for(x=0;x<des->width;++x) {
			count = k + x;
			des->rgba[0][count] = border.bottom_color.red;
			des->rgba[1][count] = border.bottom_color.green;
			des->rgba[2][count] = border.bottom_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* 绘制左边的线 */
	for(y=0;y<des->height;++y) {
		k = y * des->width;
		for(x=0;x<border.left_width;++x) {
			count = k + x;
			des->rgba[0][count] = border.left_color.red;
			des->rgba[1][count] = border.left_color.green;
			des->rgba[2][count] = border.left_color.blue;
			if(Graph_Have_Alpha(des)) 
				des->rgba[3][count] = 255;
		}
	}
	/* 绘制右边的线 */
	start_x = des->width - border.right_width;
	for(y=0;y<des->height;++y) {
		k = y * des->width + start_x;
		for(x=0;x<border.right_width;++x) {
			count = k + x;
			des->rgba[0][count] = border.right_color.red;
			des->rgba[1][count] = border.right_color.green;
			des->rgba[2][count] = border.right_color.blue;
			if(Graph_Have_Alpha(des)) {
				des->rgba[3][count] = 255;
			}
		}
	}
	/* 边框线绘制完成 */
	Graph_Unlock(des);
	return 0;
}
