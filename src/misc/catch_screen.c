#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_DISPLAY_H

#include <unistd.h>

void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out)
/* 
 * 功能：通过内存中的图像数据，捕获将在屏幕上显示的图像
 * 说明：效率较低，因为需要进行合成。
 *  */
{
	Get_Screen_Real_Graph(area, out);
}

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out)
/* 
 * 功能：直接读取帧缓冲中的图像数据
 * 说明：效率较高，但捕获的图像有可能会有问题。
 * */
{
	LCUI_Rect cut_rect;
	unsigned char *dest;
	dest = LCUI_Sys.screen.fb_mem;	/* 指向帧缓冲 */
	int x, y, n, k, count;
	
	/* 如果需要裁剪图形 */
	if ( Get_Cut_Area ( Get_Screen_Size(), area,&cut_rect ) ){
		if(!Rect_Valid(cut_rect)) {
			return;
		}
			
		area.x += cut_rect.x;
		area.y += cut_rect.y;
		area.width = cut_rect.width;
		area.height = cut_rect.height;
	}
	
	Graph_Create(out, area.width, area.height);
	Graph_Lock (out, 1); 
	/* 只能正常捕获32位显示器中显示的图形，有待完善 */
	for (n=0,y=0; y < area.height; ++y) {
		k = (area.y + y) * LCUI_Sys.screen.size.w + area.x;
		for (x = 0; x < area.width; ++x) {
			count = k + x;
			count = count << 2;  
			out->rgba[2][n] = dest[count];
			out->rgba[1][n] = dest[count + 1];
			out->rgba[0][n] = dest[count + 2];
			++n;
		}
	}
	Graph_Unlock (out);
}

static int need_break = IS_FALSE;
static pthread_t t;
static void *catch(void *arg)
/* 在截取动画时，会用这个函数捕获屏幕内容 */
{
	LCUI_Graph graph;
	LCUI_Rect *area;
	int i=0, tsec=0;
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	
	Graph_Init(&graph);
	area = (LCUI_Rect*)arg;
	
	while(!need_break) {
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		if(tsec != timeinfo->tm_sec)
			i=0;
			
		sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d-%02d.png",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, timeinfo->tm_hour, 
			timeinfo->tm_min, timeinfo->tm_sec, i++
		);
		tsec = timeinfo->tm_sec;
		
		Catch_Screen_Graph_By_FB( *area, &graph );
		write_png( filename, &graph );
		usleep(35000);
	}
	LCUI_Thread_Exit(NULL);
}

int start_catch_screen()
{
	need_break = IS_FALSE;
	return LCUI_Thread_Create(&t, NULL, catch, NULL);
}

int end_catch_screen()
{
	need_break = IS_TRUE;
	return LCUI_Thread_Join(t, NULL);
}
