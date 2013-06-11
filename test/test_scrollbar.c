
#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_SCROLLBAR_H
#include LC_GRAPH_H
#include LC_RES_H

LCUI_Widget *label, *window, *scrollbar;

void callback_func( ScrollBar_Data data, void *arg )
{
	char str[25];
	sprintf( str, "%d", data.current_num );
	Label_Text( label, str );
}

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(int argc, char*argv[])
{
	LCUI_Init(0,0,0);

	window = Widget_New("window");
	scrollbar = Widget_New("scrollbar");
	label = Widget_New("label");
	/* 设定窗口标题的文本 */
	Window_SetTitleText(window, "测试滚动条");
	/* 改变窗口的尺寸 */
	window->resize(window, Size(320, 240));
	Widget_Resize( scrollbar, Size(10, 100) );
	ScrollBar_SetMaxSize( scrollbar, 400 );
	/* 设置部件布局 */
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(-20,0) );
	Widget_SetAlign( scrollbar, ALIGN_MIDDLE_CENTER, Pos(0,0) );

	Label_Text( label, "0" );

	/* 将窗口客户区作为部件的容器添加进去 */
	Window_ClientArea_Add(window, label);
	Window_ClientArea_Add(window, scrollbar);
	/* 将回调函数与滚动条部件连接 */
	ScrollBar_Connect( scrollbar, callback_func, NULL );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	/* 显示部件 */
	Widget_Show(scrollbar);
	Widget_Show(window);
	Widget_Show(label);
	return LCUI_Main(); /* 进入主循环 */
}

