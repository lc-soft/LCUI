// 此程序适用于Win32平台
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H 
#include LC_RADIOBTN_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_DISPLAY_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

static LCUI_Widget *main_window, *label, *btn_ok, *rdbtn[5];

/* 按下按钮后将调用的回调函数 */
void switch_style(LCUI_Widget *widget, LCUI_WidgetEvent *event)
{
	/* 找到被选中的单选框，并设置相应视频模式 */
	if(RadioButton_IsOn(rdbtn[0])) {
		LCUIScreen_SetMode( 0, 0, LCUI_INIT_MODE_AUTO );
	}
	if(RadioButton_IsOn(rdbtn[1])) {
		LCUIScreen_SetMode( 800, 600, LCUI_INIT_MODE_FULLSCREEN );
	}
	if(RadioButton_IsOn(rdbtn[2])) {
		LCUIScreen_SetMode( 800, 600, LCUI_INIT_MODE_WINDOW );
	}
}

int main(int argc, char **argv)
{
	int i; 
	LCUI_Pos offset;
	wchar_t text[3][40] = { L"默认模式",L"全屏模式，800x600 分辨率", L"窗口模式，800x600 窗口"};
	LCUI_Init(0,0,0);
	
	main_window = Widget_New( "window" ); 
	btn_ok = Widget_New( "button" );
	label = Widget_New( "label" );
	/* 设置窗口的尺寸以及标题栏文本 */
	Widget_Resize( main_window, Size(320, 240) ); 
	Window_SetTitleTextW( main_window, L"测试视频模式的切换" ); 
	offset.x = 20;
	/* 创建3个单选框部件 */
	for(offset.y=-22,i=0; i<3; ++i,offset.y+=22) {
		rdbtn[i] = Widget_New( "radio_button" );
		Window_ClientArea_Add( main_window, rdbtn[i] );
		Widget_SetAlign( rdbtn[i], ALIGN_MIDDLE_LEFT, offset );
		RadioButton_TextW( rdbtn[i], text[i] );
		Widget_Show( rdbtn[i] );
	}
	/* 为单选框建立互斥关系 */
	RadioButton_CreateMutex(rdbtn[0], rdbtn[1]);
	RadioButton_CreateMutex(rdbtn[0], rdbtn[2]);
	RadioButton_SetOn( rdbtn[0] );
	/* 禁用按钮部件的自动尺寸调整 */
	Widget_SetAutoSize( btn_ok, FALSE, 0 );
	Widget_Resize( btn_ok, Size(100, 30) );
	/* 将部件加入窗口客户区 */
	Window_ClientArea_Add( main_window, btn_ok );
	Window_ClientArea_Add( main_window, label );
	/* 设置部件的布局 */
	Widget_SetAlign( btn_ok, ALIGN_MIDDLE_CENTER, Pos(0,70) );
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(0,-50) ); 
	/* 设置部件的文本 */
	Button_TextW( btn_ok, L"应用" ); 
	Label_TextW( label, L"选择视频模式：" );
	/* 为按钮的点击事件关联回调函数 */
	Widget_ConnectEvent( btn_ok, EVENT_CLICKED, switch_style );
	Widget_ConnectEvent( Window_GetCloseButton(main_window), EVENT_CLICKED, destroy );
	/* 显示部件 */
	Widget_Show( btn_ok );
	Widget_Show( label );
	Widget_Show( main_window );
	/* 进入主循环 */
	return LCUI_Main(); 
}
