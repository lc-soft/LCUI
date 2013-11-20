//测试进度条部件的示例程序
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPH_H
#include LC_LABEL_H
#include LC_RES_H
#include LC_PROGBAR_H
#include LC_PICBOX_H

#include <time.h>

/* 动态改变进度条的数据 */
static void change_progress( void *arg )
{
	char str[15];
	int i, max = 100;
	LCUI_Widget *label;
	LCUI_Widget *widget;
	
	srand((unsigned int)time(NULL));
	/* 创建一个label部件 */
	widget = (LCUI_Widget *)arg;
	label = Widget_New("label");
	/* 放到进度条里 */
	Widget_Container_Add( widget, label );
	/* 并且居中 */
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(0,0) ); 
	/* 显示它 */
	Widget_Show( label );

	/* 设置最大值 */
	ProgressBar_SetMaxValue(widget, max); 
	for( i=0; i<max; i+=(rand()%3+1) ) {
		/* 设置当前值 */
		ProgressBar_SetValue(widget, i); 
		/* 转换成字符串 */
		sprintf( str, "%d%%", (int)(i*100.0/max) );
		/* 设置显示的文本 */
		Label_Text( label, str );
		/* 随机暂停一段时间 */
		LCUI_MSleep(20+(rand()%80));
	}
	ProgressBar_SetValue( widget, max );
	Label_Text( label, "100%" );
	LCUIThread_Exit(NULL);
}

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(int argc, char **argv)
{
	LCUI_Thread thread;
	LCUI_Widget *window, *progbar;
	
	LCUI_Init(800,600,0);
	
	window = Widget_New("window");
	progbar = Widget_New("progress_bar");

	Window_SetTitleTextW( window, L"测试进度条部件" );
	Widget_Resize( window, Size(320, 240) );
	Window_ClientArea_Add( window, progbar );
	Widget_SetAlign( progbar, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_Resize( progbar, Size(300, 25) );

	LCUIThread_Create( &thread, change_progress, (void*)progbar );
	Widget_ConnectEvent( Window_GetCloseButton(window), EVENT_CLICKED, destroy );

	Widget_Show( progbar );
	Widget_Show( window );
	return LCUI_Main();
}

