// 测试LCUI的窗口
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_RADIOBTN_H
#include LC_BUTTON_H
#include LC_LABEL_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

//#define TEST_THIS
#ifdef TEST_THIS

static LCUI_Widget *main_window, *label, *btn_ok, *rdbtn[5];

/* 按下按钮后将调用的回调函数 */
void switch_style(LCUI_Widget *widget, LCUI_WidgetEvent *event)
{
	/* 找到被选中的单选框，并为窗口设定相应风格 */
	if(RadioButton_IsOn(rdbtn[0])) {
		Widget_SetStyleID( main_window, WINDOW_STYLE_PURE_BLUE );
	}
	if(RadioButton_IsOn(rdbtn[1])) {
		Widget_SetStyleID( main_window, WINDOW_STYLE_PURE_GREEN );
	}
	if(RadioButton_IsOn(rdbtn[2])) {
		Widget_SetStyleID( main_window, WINDOW_STYLE_PURE_RED );
	}
	if(RadioButton_IsOn(rdbtn[3])) {
		Widget_SetStyleID( main_window, WINDOW_STYLE_PURE_ORANGE );
	}
	if(RadioButton_IsOn(rdbtn[4])) {
		Widget_SetStyleID( main_window, WINDOW_STYLE_PURE_PURPLE );
	}
}

int main( int argc, char **argv )
{
	int i;
	char text[5][10] = { "蓝色","绿色", "红色", "橙色", "紫色" };
	LCUI_Pos offset_pos[5]={{-120,0},{-60,0},{0,0},{60,0},{120,0}};

	LCUI_Init();

	main_window = Widget_New( "window" );
	btn_ok = Widget_New( "button" );
	label = Widget_New( "label" );
	/* 设置窗口的尺寸以及标题栏文本 */
	Widget_Resize( main_window, Size(320, 240) );
	Window_SetTitleText( main_window, "测试窗口的风格切换" );
	/* 创建5个单选框部件 */
	for(i=0; i<5; ++i) {
		rdbtn[i] = Widget_New( "radio_button" );
		Window_ClientArea_Add( main_window, rdbtn[i] );
		Widget_SetAlign( rdbtn[i], ALIGN_MIDDLE_CENTER, offset_pos[i] );
		RadioButton_Text( rdbtn[i], text[i] );
		Widget_Show( rdbtn[i] );
	}
	/* 为单选框建立互斥关系 */
	RadioButton_CreateMutex(rdbtn[0], rdbtn[1]);
	RadioButton_CreateMutex(rdbtn[0], rdbtn[2]);
	RadioButton_CreateMutex(rdbtn[0], rdbtn[3]);
	RadioButton_CreateMutex(rdbtn[0], rdbtn[4]);
	RadioButton_SetOn( rdbtn[0] );
	/* 禁用按钮部件的自动尺寸调整 */
	Widget_SetAutoSize( btn_ok, FALSE, 0 );
	Widget_Resize( btn_ok, Size(100, 30) );
	/* 将部件加入窗口客户区 */
	Window_ClientArea_Add( main_window, btn_ok );
	Window_ClientArea_Add( main_window, label );
	/* 设置部件的布局 */
	Widget_SetAlign( btn_ok, ALIGN_MIDDLE_CENTER, Pos(0,50) );
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(0,-50) );
	/* 设置部件的文本 */
	Button_Text( btn_ok, "应用" );
	Label_Text( label, "选择窗口配色：" );
	/* 为按钮的点击事件关联回调函数 */
	Widget_Event_Connect( btn_ok, EVENT_CLICKED, switch_style );
	Widget_Event_Connect( Window_GetCloseButton(main_window), EVENT_CLICKED, destroy );
	/* 显示部件 */
	Widget_Show( btn_ok );
	Widget_Show( label );
	Widget_Show( main_window );
	/* 进入主循环 */
	return LCUI_Main();
}

#else

int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
	LCUI_Init(0,0,0);
	LCUI_Widget *windows[4];
	/* 创建窗口部件 */
	windows[0] = Window_New("主窗口", NULL, Size(320, 240));
	windows[1] = Window_New("子窗口 A", NULL, Size(240, 200));
	windows[2] = Window_New("子窗口 B", NULL, Size(180, 150));
	windows[3] = Window_New("子窗口 C", NULL, Size(150, 100));
	/* 改变风格 */
	Widget_SetStyleID( windows[1], WINDOW_STYLE_PURE_ORANGE );
	Widget_SetStyleID( windows[2], WINDOW_STYLE_PURE_GREEN );
	Widget_SetStyleID( windows[3], WINDOW_STYLE_PURE_RED );
	/* 将子窗口放入主窗口的客户区中 */
	Window_ClientArea_Add( windows[0], windows[1] );
	Window_ClientArea_Add( windows[0], windows[2] );
	Window_ClientArea_Add( windows[0], windows[3] );
	/* 显示它们 */
	Widget_Show(windows[0]);
	Widget_Show(windows[1]);
	Widget_Show(windows[2]);
	Widget_Show(windows[3]);
	Widget_Event_Connect( Window_GetCloseButton(windows[0]), EVENT_CLICKED, destroy);
	LCUI_Main(); /* 进入主循环 */
	return 0;
}
#endif
