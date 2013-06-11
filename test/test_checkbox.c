// 测试复选框部件的程序例子
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_CHECKBOX_H
#include LC_INPUT_H
#include LC_GRAPH_H
#include LC_RES_H

LCUI_Widget *checkbox[5], *button, *window, *label;

static void
destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

static void
view_result( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	wchar_t str[256];
	wcscpy(str, L"你的兴趣爱好有： ");
	/* 检测各个复选框是否被选中，以确定内容 */
	if(CheckBox_IsOn(checkbox[0])) {
		wcscat(str, L"编程 ");
	}
	if(CheckBox_IsOn(checkbox[1])){
		wcscat(str, L"电影 ");
	}
	if(CheckBox_IsOn(checkbox[2])){
		wcscat(str, L"动漫 ");
	}
	if(CheckBox_IsOn(checkbox[3])){
		wcscat(str, L"音乐 ");
	}
	if(CheckBox_IsOn(checkbox[4])){
		wcscat(str, L"睡觉 ");
	}

	wcscat(str, L".");
	LCUI_MessageBoxW( MB_ICON_INFO, str, L"结果", MB_BTN_OK );
}

int main( int argc, char **argv )
{
	int i;
#ifdef LCUI_BUILD_IN_WIN32
	Win32_LCUI_Init( hInstance );
#endif
	LCUI_Init(0,0,0);
	/* 创建部件 */
	window  = Widget_New("window");
	label   = Widget_New("label");
	button	= Widget_New("button");
	for(i=0; i<5; ++i) {
		checkbox[i] = Widget_New("check_box");
	}
	CheckBox_TextW( checkbox[0], L"编程" );
	CheckBox_TextW( checkbox[1], L"电影" );
	CheckBox_TextW( checkbox[2], L"动漫" );
	CheckBox_TextW( checkbox[3], L"音乐" );
	CheckBox_TextW( checkbox[4], L"睡觉" );

	Button_TextW( button, L"提交" );
	Window_SetTitleTextW( window, L"测试复选框部件") ;
	window->resize(window, Size(320, 240));
	/* 禁用按钮部件的自动尺寸调整，因为要自定义尺寸 */
	Widget_SetAutoSize( button, FALSE, 0 );
	Widget_Resize(button, Size(60, 25));
	/* 将窗口客户区作为这些部件的容器 */
	Window_ClientArea_Add(window, label);
	Window_ClientArea_Add(window, button);
	for(i=0; i<5; i++) {
		Window_ClientArea_Add(window, checkbox[i]);
	}
	/* 调整部件的布局*/
	Widget_SetAlign(label, ALIGN_MIDDLE_CENTER, Pos(0,-20));
	Widget_SetAlign(button, ALIGN_MIDDLE_CENTER, Pos(0,60));
	Widget_SetAlign(checkbox[0], ALIGN_MIDDLE_CENTER, Pos(-120 ,5));
	Widget_SetAlign(checkbox[1], ALIGN_MIDDLE_CENTER, Pos(-60 ,5));
	Widget_SetAlign(checkbox[2], ALIGN_MIDDLE_CENTER, Pos(0 ,5));
	Widget_SetAlign(checkbox[3], ALIGN_MIDDLE_CENTER, Pos(60 ,5));
	Widget_SetAlign(checkbox[4], ALIGN_MIDDLE_CENTER, Pos(120 ,5));

	Label_TextW(label, L"你都有哪些兴趣爱好？\n");
	/* 显示部件 */
	Widget_Show(label);
	Widget_Show(button);
	for(i=0; i<5; ++i) {
		Widget_Show(checkbox[i]);
	}
	Widget_Show(window);
	/* 为按钮部件关联点击事件，被关联的函数是view_result，它会在点击按钮后被调用 */
	Widget_Event_Connect( button, EVENT_CLICKED, view_result );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	return LCUI_Main();
}

