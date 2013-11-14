// 图像高斯模糊处理
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_DRAW_H
#include LC_GRAPH_H
#include <time.h>

int main(int argc, char *argv[])
{
	clock_t start;
	LCUI_Graph slot, graph;
	/* 初始化数据结构 */
	Graph_Init( &graph );
	Graph_Init( &slot );

	if(argc == 2) {
		/* 载入图片文件 */
		if( Graph_LoadImage( argv[1], &graph ) != 0) {
			printf("can not load images!\n");
			return -1;
		}
		/* 开始计时 */
		start = clock();
		/* 默认使用高斯模糊算法，输入数据为graph, 数据输出至slot， sigma = 20 */
		Graph_Smooth( &graph, &slot, 20.0 );
		printf("smooth, use time: %ld us\n", clock()-start);
		start = clock();
		/* 将图像数据保存至文件 */
		Graph_WritePNG( "output.png", &slot );
		printf("write, use time: %ld us\n", clock()-start);
		/* 释放资源 */
		Graph_Free( &slot );
		Graph_Free( &graph );
	} else {
		printf( "usage:\n%s  imgfile\n", argv[0] );
	}
	return 0;
}
