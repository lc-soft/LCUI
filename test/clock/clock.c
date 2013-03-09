// 时钟的例子程序

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPH_H 
#include LC_DRAW_H
#include LC_FONT_H
#include LC_PICBOX_H

void update_clock(void *arg)
{
	time_t rawtime;
	struct tm * timeinfo;
	LCUI_Graph h_temp, m_temp, clock_bg, hour_pointer, minute_pointer;
	LCUI_Widget *widget;
	LCUI_Widget *bg, *hour, *minute; 
	int h_angle, m_angle;
	
	widget = (LCUI_Widget *)arg;
	/* 初始化图形数据结构 */
	Graph_Init(&clock_bg);
	Graph_Init(&minute_pointer);
	Graph_Init(&hour_pointer);
	Graph_Init(&h_temp);
	Graph_Init(&m_temp);
	/* 创建几个部件 */
	bg = Widget_New("picture_box");
	hour = Widget_New("picture_box");
	minute = Widget_New("picture_box");
	/* PictureBox部件居中显示图片 */
	PictureBox_SetSizeMode(bg, SIZE_MODE_CENTER);
	PictureBox_SetSizeMode(hour, SIZE_MODE_CENTER);
	PictureBox_SetSizeMode(minute, SIZE_MODE_CENTER);
	/* 载入图片，保存图形数据 */
	Load_Image("new_daytime_background.png", &clock_bg);
	Load_Image("new_daytime_hour.png", &hour_pointer);
	Load_Image("new_daytime_minute.png", &minute_pointer);
	/* 设定PictureBox部件显示的图形 */
	PictureBox_SetImage(bg, &clock_bg);
	PictureBox_SetImage(hour, &hour_pointer);
	PictureBox_SetImage(minute, &minute_pointer);
	/* 将这些部件添加至相应容器中 */
	Widget_Container_Add(bg, hour);
	Widget_Container_Add(bg, minute);
	/* 将部件添加至窗口客户区中 */
	Window_ClientArea_Add(widget, bg);
	/* 改变部件尺寸，使用固定的尺寸 */
	Widget_Resize(bg, Size(280, 280));
	Widget_Resize(hour, Size(120, 120));
	Widget_Resize(minute, Size(120, 120));
	/* 改变部件的布局方式，都是居中显示 */
	Widget_SetAlign(bg, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Widget_SetAlign(hour, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	Widget_SetAlign(minute, ALIGN_MIDDLE_CENTER, Pos(0, 0));
	/* 显示 */
	Widget_Show(hour);
	Widget_Show(minute);
	Widget_Show(bg);
	
	while(1) {
	time ( &rawtime );
	timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */
	/* 计算时针分针的角度 */
	h_angle = 360*timeinfo->tm_hour / 12.0;
	m_angle = 360*timeinfo->tm_min / 60.0;
	h_angle += m_angle / 60;
	/* 根据这个角度来旋转图形 */
	Graph_Rotate(&hour_pointer, h_angle, &h_temp);
	Graph_Rotate(&minute_pointer, m_angle, &m_temp);
	/* 更改PictureBox部件显示的图形 */
	PictureBox_SetImage(hour, &h_temp);
	PictureBox_SetImage(minute, &m_temp);
	LCUI_Sleep(1);/* 暂停1秒 */
	}
	
	LCUIThread_Exit(NULL);
}

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{
	LCUI_MainLoop_Quit( NULL );
}

int main(void)
{
	LCUI_Widget *window;
	LCUI_Thread t;
	
	setenv( "LCUI_FONTFILE", "../../fonts/msyh.ttf", FALSE );
	LCUI_Init();
	window = Widget_New("window");
	Widget_Resize(window, Size(320, 320));
	Window_SetTitleText(window, "时钟");
	LCUIThread_Create(&t, update_clock, (void*)window); 
	Widget_Event_Connect(Window_GetCloseButton(window), EVENT_CLICKED, destroy);
	Widget_Show(window); 
	return LCUI_Main();
}


