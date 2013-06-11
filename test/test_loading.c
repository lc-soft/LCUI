// 一个演示程序，主要用了LCUI的图形旋转功能
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_RES_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

/* 改变显示的图形 */
void change_graph(void *arg)
{
	int i;
	LCUI_Graph frame, temp;
	LCUI_Widget *widget;

	widget = (LCUI_Widget *)arg;

	Graph_Init(&frame);/* 初始化 */
	Graph_LoadImage("ring.png", &frame);/* 载入之 */
	/* 居中显示图形 */
	Widget_SetAlign(widget, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	PictureBox_SetSizeMode(widget, SIZE_MODE_CENTER);
	Widget_Show(widget);
	while(1) {
		PictureBox_SetImage(widget, &frame);
		for(i=10; i<=360; i+=10) {
			Graph_Rotate(&frame, i, &temp);
			PictureBox_SetImage(widget, &temp);
			LCUI_MSleep(20);
		}
	}
	LCUIThread_Exit(NULL);
}

int main(void)
{
	LCUI_Thread t;
	int width, height;
	LCUI_Widget *window, *label, *logo, *img;

	LCUI_Init(0,0,0);
	width  = 320;//LCUIScreen_GetWidth(); /* 窗口的宽度 */
	height = 240;//LCUIScreen_GetHeight();; /* 窗口的高度 */

	/* 创建部件 */
	window	= Widget_New("window");
	label	= Widget_New("label");
	logo	= Widget_New("picture_box");
	img	= Widget_New("picture_box");

	Widget_Resize(img, Size(180, 180));
	Widget_Resize(logo, Size(128,128));
	Widget_Resize(window, Size(width, height));

	Window_SetTitleText(window, "LCUI");

	Widget_SetBackgroundColor(window, RGB(94,200,255));
	PictureBox_SetSizeMode(logo, SIZE_MODE_CENTER);
	PictureBox_SetImageFile(logo, "logo_128x128.png");

	Label_Text(label, "LCUI 正在载入...");
	Widget_SetAlign(logo, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Widget_SetAlign(label, ALIGN_MIDDLE_CENTER, Pos(0, +70));
	/* 将这三个部件放入窗口客户区中 */
	Window_ClientArea_Add(window, label);
	Window_ClientArea_Add(window, img);
	Window_ClientArea_Add(window, logo);

	LCUIThread_Create(&t, change_graph, (void*)img);
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	/* 显示部件以及窗口 */
	Widget_Show(label);
	Widget_Show(logo);
	Widget_Show(window);
	return LCUI_Main(); /* 进入主循环 */
}

