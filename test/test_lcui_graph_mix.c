//测试LCUI的图像混合速度

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_GRAPH_H
#include <time.h>

#define IMG_W	1920
#define IMG_H	1080
#define MIX_COUNT	255

int main(void)
{
	int i;
	clock_t ct;
	LCUI_Graph bg, fg;
	
	Graph_Init( &bg );
	Graph_Init( &fg );
	bg.color_type = COLOR_TYPE_RGB;
	fg.color_type = COLOR_TYPE_RGBA;
	//为前景图和背景图分配内存
	Graph_Create( &bg, IMG_W, IMG_H );
	Graph_Create( &fg, IMG_W, IMG_H );
	ct = clock(); //开始计时
	printf("start test, please wait......\n");
	for(i=0; i<255; i+=5){
		Graph_FillAlpha( &fg, i );
		Graph_Mix(&bg, &fg, Pos(0,0));
	} 
	i = (clock()-ct)*1000/CLOCKS_PER_SEC;
	printf("graph size: %dx%d, total mix count: %d, total time: %dms,  mix count per sec: %.2lf\n",
	IMG_W, IMG_H, MIX_COUNT, i, (double)(1.0*i/MIX_COUNT));
	return 0;
}

