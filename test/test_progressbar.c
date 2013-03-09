//测试进度条部件的示例程序
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPH_H
#include LC_LABEL_H
#include LC_RES_H
#include LC_PROGBAR_H
#include LC_PICBOX_H

/* 动态改变进度条部件的数据 */
static void change_progress_1( void *arg )
{
	char str[15];
	int i, max = 100;
	LCUI_Widget *label;
	LCUI_Widget *widget;
	
	widget = (LCUI_Widget *)arg; /* 转换类型 */
	label = Widget_New("label");
	Widget_Container_Add(widget, label);
	Widget_SetAlign(label, ALIGN_MIDDLE_CENTER, Pos(0,0)); 
	Widget_Show(label);
	ProgressBar_SetMaxValue(widget, max); /* 最大值 */
	srand(time(NULL));
	for(i=0; i<max; i+=rand()%5) {
		ProgressBar_SetValue(widget, i);/* 当前值 */
		sprintf( str, "%d%%", (int)(i*100.0/max) );
		Label_Text( label, str );
		LCUI_MSleep(100);/* 暂停0.1秒 */
	}
	ProgressBar_SetValue(widget, max);
	Label_Text(label, "100%");
	LCUIThread_Exit(NULL);
}

static void change_progress_2( void *arg )
{
	char str[15];
	int i, max = 100;
	LCUI_Widget *label;
	LCUI_Widget *widget;
	
	widget = (LCUI_Widget *)arg; 
	label = Widget_New("label");
	Widget_Container_Add(widget, label);
	label->set_align(label, ALIGN_MIDDLE_CENTER, Pos(0,0)); 
	label->show(label);
	ProgressBar_SetMaxValue(widget, max); 
	srand(time(NULL));
	for(i=0; i<max; i+=rand()%5) {
		ProgressBar_SetValue(widget, i); 
		sprintf( str, "%d/%d", i, max );
		Label_Text( label, str );
		LCUI_MSleep(100);
	}
	ProgressBar_SetValue(widget, max);
	sprintf( str, "%d/%d", max, max );
	Label_Text( label, str );
	LCUIThread_Exit(NULL);
}

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(void)
{
	LCUI_Thread t[2];
	LCUI_Widget *window, *pb_a, *pb_b;
	
	LCUI_Init();
	/* 创建部件 */
	printf("main(): create widget\n");
	window = Widget_New("window");
	pb_a = Widget_New("progress_bar");
	pb_b = Widget_New("progress_bar");
	printf("main(): create widget done\n");
	/* 设定窗口标题的文本 */
	printf("main(): setting widget\n");
	Window_SetTitleText(window, "测试进度条部件"); 
	/* 改变窗口的尺寸 */
	Widget_Resize(window, Size(320, 240));
	/* 将窗口客户区作为部件的容器添加进去 */
	Window_ClientArea_Add(window, pb_a);
	Window_ClientArea_Add(window, pb_b);
	/* 居中显示 */
	Widget_SetAlign(pb_a, ALIGN_MIDDLE_CENTER, Pos(0, -25)); 
	Widget_SetAlign(pb_b, ALIGN_MIDDLE_CENTER, Pos(0, 25));
	/* 设定部件的风格 */
	Widget_SetStyleName(pb_a, "dynamic");
	Widget_SetStyleName(pb_b, "classic");
	/* 改变部件尺寸 */
	Widget_Resize(pb_a, Size(300, 25));
	Widget_Resize(pb_b, Size(300, 25));
	printf("main(): create thread 1\n");
	/* 创建线程，此函数和pthread_create函数用法一样 */
	LCUIThread_Create(&t[0], change_progress_1, (void*)pb_a);
	printf("main(): create thread 2\n");
	LCUIThread_Create(&t[1], change_progress_2, (void*)pb_b); 
	printf("main(): create thread done\n");
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	/* 显示部件 */
	Widget_Show(pb_a);
	Widget_Show(pb_b);
	Widget_Show(window);
	printf("main(): enter LCUI_Main()\n");
	return LCUI_Main(); /* 进入主循环 */  
}

