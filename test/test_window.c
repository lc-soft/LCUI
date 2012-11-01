// 测试LCUI的窗口
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H 
#include LC_RADIOBTN_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include <unistd.h>

#define TEST_THIS
#ifdef TEST_THIS
/* 声明一些部件 */
static LCUI_Widget *main_window, *label, *btn_ok, *rdbtn[5];

/* 按下按钮后将调用的回调函数 */
void switch_style(LCUI_Widget *widget, void *arg)
{
	/* 找到被选中的单选框，并为窗口设定相应风格 */
	if(RadioButton_Is_On(rdbtn[0])) {
		Set_Widget_StyleID( main_window, WINDOW_STYLE_PURE_BLUE );
	}
	if(RadioButton_Is_On(rdbtn[1])) {
		Set_Widget_StyleID( main_window, WINDOW_STYLE_PURE_GREEN );
	}
	if(RadioButton_Is_On(rdbtn[2])) {
		Set_Widget_StyleID( main_window, WINDOW_STYLE_PURE_RED );
	}
	if(RadioButton_Is_On(rdbtn[3])) {
		Set_Widget_StyleID( main_window, WINDOW_STYLE_PURE_ORANGE );
	}
	if(RadioButton_Is_On(rdbtn[4])) {
		Set_Widget_StyleID( main_window, WINDOW_STYLE_PURE_PURPLE );
	}
}

int main(int argc, char *argv[]) 
{
	/* LCUI初始化 */
	LCUI_Init(argc, argv);
	
	int i; 
	char text[5][10] = { "蓝色","绿色", "红色", "橙色", "紫色" };
	LCUI_Pos offset_pos[5]={{-120,0},{-60,0},{0,0},{60,0},{120,0}};
	
	main_window = Create_Widget( "window" ); 
	btn_ok = Create_Widget( "button" );
	label = Create_Widget( "label" );
	/* 设置窗口的尺寸以及标题栏文本 */
	Resize_Widget( main_window, Size(320, 240) ); 
	Set_Window_Title_Text( main_window, "测试窗口的风格切换" ); 
	/* 创建5个单选框部件 */
	for(i=0; i<5; ++i) {
		rdbtn[i] = Create_Widget( "radio_button" );
		Window_Client_Area_Add( main_window, rdbtn[i] );
		Set_Widget_Align( rdbtn[i], ALIGN_MIDDLE_CENTER, offset_pos[i] );
		Set_RadioButton_Text( rdbtn[i], "%s", text[i] );
		Show_Widget( rdbtn[i] );
	}
	/* 为单选框建立互斥关系 */
	RadioButton_Create_Mutex(rdbtn[0], rdbtn[1]);
	RadioButton_Create_Mutex(rdbtn[0], rdbtn[2]);
	RadioButton_Create_Mutex(rdbtn[0], rdbtn[3]);
	RadioButton_Create_Mutex(rdbtn[0], rdbtn[4]);
	Set_RadioButton_On( rdbtn[0] );
	/* 禁用按钮部件的自动尺寸调整 */
	Disable_Widget_Auto_Size( btn_ok );
	Resize_Widget( btn_ok, Size(100, 30) );
	/* 将部件加入窗口客户区 */
	Window_Client_Area_Add( main_window, btn_ok );
	Window_Client_Area_Add( main_window, label );
	/* 设置部件的布局 */
	Set_Widget_Align( btn_ok, ALIGN_MIDDLE_CENTER, Pos(0,50) );
	Set_Widget_Align( label, ALIGN_MIDDLE_CENTER, Pos(0,-50) ); 
	/* 设置部件的文本 */
	Set_Button_Text( btn_ok, "应用" ); 
	Set_Label_Text( label, "选择窗口配色：" );
	/* 为按钮的点击事件关联回调函数 */
	Widget_Clicked_Event_Connect( btn_ok, switch_style, NULL );
	/* 显示部件 */
	Show_Widget( btn_ok );
	Show_Widget( label );
	Show_Widget( main_window );
	/* 进入主循环 */
	return LCUI_Main(); 
}

#else

int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
	LCUI_Init(argc, argv);
	LCUI_Widget *w1, *w2, *w3;
	/* 创建部件 */
	w1  = Create_Widget("window");
	w2  = Create_Widget("window");
	w3  = Create_Widget("window");
	/* 改变窗口的尺寸 */
	Resize_Widget(w1, Size(320, 240));
	Resize_Widget(w2, Size(200, 140));
	Resize_Widget(w3, Size(120, 100));
	Set_Window_Title_Text(w1, "窗口1");
	Set_Window_Title_Text(w2, "窗口2");
	Set_Window_Title_Text(w3, "窗口3");
	Window_Client_Area_Add(w2, w3);
	Window_Client_Area_Add(w1, w2);
	Show_Widget(w1); 
	Show_Widget(w2); 
	Show_Widget(w3); 
	LCUI_Main(); /* 进入主循环 */ 
	return 0;
}
#endif
