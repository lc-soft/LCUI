#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H

#include <memory.h>

int Draw_Empty_Slot(LCUI_Graph *graph, int width, int height)
/* 功能：绘制进度条的空槽 */
{
	if(width < 4 || height < 4) {
		return -1;
	}
	graph->have_alpha = IS_TRUE;
	Graph_Create(graph, width, height); /* 申请内存 */
	Graph_Fill_Color(graph, RGB(250,250,250));/* 填充白色 */
	Graph_Fill_Alpha(graph, 255);
	
	/* 四个角上的一个像素点完全透明 */
	graph->rgba[3][0] = 0;
	graph->rgba[3][width-1] = 0;
	graph->rgba[3][width*(height-1)] = 0;
	graph->rgba[3][width*height-1] = 0;
	
	int i, n;
	unsigned char c;
	/* 绘制左边和右边的竖线条 */
	for(i=0; i<height; ++i)
	{
		n = i*width; 
		c = 160 +(30-(30.0/height)*i);
		graph->rgba[0][n] = c;
		graph->rgba[1][n] = c;
		graph->rgba[2][n] = c;
		graph->rgba[0][n+1] = 230;
		graph->rgba[1][n+1] = 230;
		graph->rgba[2][n+1] = 230; 
		
		n = n+width-1;
		graph->rgba[0][n] = c;
		graph->rgba[1][n] = c;
		graph->rgba[2][n] = c;
		graph->rgba[0][n-1] = 230;
		graph->rgba[1][n-1] = 230;
		graph->rgba[2][n-1] = 230;
	}
	
	/* 绘制顶端的线条 */
	memset(graph->rgba[0], 180, width);
	memset(graph->rgba[1], 180, width);
	memset(graph->rgba[2], 180, width);
	
	memset(graph->rgba[0]+width+1, 240, width-2); 
	memset(graph->rgba[1]+width+1, 240, width-2); 
	memset(graph->rgba[2]+width+1, 240, width-2); 
	
	/* 绘制底端的线条 */
	n = width*(height-1);
	memset(graph->rgba[0]+n, 140, width);
	memset(graph->rgba[1]+n, 140, width);
	memset(graph->rgba[2]+n, 140, width);
	
	n = width*(height-2);
	memset(graph->rgba[0]+n+1, 225, width-2); 
	memset(graph->rgba[1]+n+1, 225, width-2); 
	memset(graph->rgba[2]+n+1, 225, width-2);
	return 0;
}
