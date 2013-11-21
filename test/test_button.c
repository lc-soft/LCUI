// 测试按钮部件的程序例子
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_BUTTON_H

static void
destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(int argc, char **argv)
{
	LCUI_Widget *window, *button;

	LCUI_Init(0,0,0);
	window  = Widget_New("window");
	button  = Widget_New("button");

	/* 设定窗口标题的文本 */
	Window_SetTitleText(window, "测试按钮");
	/* 改变窗口的尺寸 */
	Widget_Resize( window, Size(320, 240) );
	/* 将窗口客户区作为按钮的容器添加进去 */
	Window_ClientArea_Add( window, button );
	/* 居中显示按钮 */
	Widget_SetAlign( button, ALIGN_MIDDLE_CENTER, Pos(0, 0) );
	/* 禁用部件的自动尺寸调整 */
	Widget_SetAutoSize( button, FALSE, 0 );
	/* 自定义按钮的尺寸 */
	Widget_Resize( button, Size(180, 70) );

	/* 设定按钮上显示的文本内容，和label部件的用法一样,支持样式标签 */
	Button_Text( button,
		"<size=30px><color=255,0,0>彩</color>"
		"<color=0,255，0>色</color>"
		"<color=0,0,255>的</color>"
		"<color=255,255,0>按</color>"
		"<color=255,255,255>钮</color></size>");

	/* 显示部件 */
	Widget_Show( button );
	Widget_Show( window );
	Widget_ConnectEvent( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	return LCUI_Main(); /* 进入主循环 */
}

