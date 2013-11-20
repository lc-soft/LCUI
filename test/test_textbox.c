// 测试LCUI的文本框部件
// 0.15.0 版本中，在对文本框中的文本进行编辑时会有问题，计划在以后的版本中解决
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_TEXTBOX_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_GRAPH_H

static void callback( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(int argc, char*argv[])
{
	LCUI_Widget *window, *textbox;

	LCUI_Init(800,600,LCUI_INIT_MODE_AUTO);

	window  = Widget_New("window");
	textbox = Widget_New("text_box");

	Window_SetTitleTextW(window, L"测试文本框部件");
	window->resize( window, Size(640, 480) );
	Window_ClientArea_Add( window, textbox );
	/* 启用多行文本显示 */
	TextBox_SetMultiline( textbox, TRUE );
	textbox->set_align( textbox, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_SetDock( textbox, DOCK_TYPE_FILL );
	textbox->show(textbox);
	window->show(window);
	Widget_ConnectEvent(Window_GetCloseButton(window), EVENT_CLICKED, callback);
	return LCUI_Main();
}
