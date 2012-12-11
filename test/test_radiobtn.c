// 测试单选框部件的示例程序
#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_RADIOBTN_H
#include LC_MISC_H 
#include LC_GRAPH_H
#include LC_RES_H
#include <unistd.h>

LCUI_Widget *age_rb[6], *button, *window, *age_label; 

void view_result(LCUI_Widget *widget, void *arg)
{
	char str[256]; 
	strcpy(str, "你的年龄段是： ");
	if(RadioButton_Is_On(age_rb[0]))
		strcat(str, "15岁以下");
	if(RadioButton_Is_On(age_rb[1]))
		strcat(str, "15-20岁");
	if(RadioButton_Is_On(age_rb[2]))
		strcat(str, "21-25岁");
	if(RadioButton_Is_On(age_rb[3]))
		strcat(str, "26-30岁");
	if(RadioButton_Is_On(age_rb[4]))
		strcat(str, "31-40岁");
	if(RadioButton_Is_On(age_rb[5]))
		strcat(str, "40岁以上");
	
	strcat(str, "。");
	Label_Text(age_label, str);  
	Disable_Widget(button);
}

int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
	int i;
	LCUI_Init(argc, argv);
	/* 创建部件 */
	window  = Create_Widget("window");
	age_label = Create_Widget("label"); 
	button	= Create_Button_With_Text("提交"); 
	age_rb[0] = Create_RadioButton_With_Text("A. 15岁以下");
	age_rb[1] = Create_RadioButton_With_Text("B. 15-20岁");
	age_rb[2] = Create_RadioButton_With_Text("C. 21-25岁");
	age_rb[3] = Create_RadioButton_With_Text("D. 26-30岁");
	age_rb[4] = Create_RadioButton_With_Text("E. 31-40岁");
	age_rb[5] = Create_RadioButton_With_Text("F. 40岁以上");
	/* 设定窗口标题的文本 */
	Set_Window_Title_Text(window, "测试复选框部件"); 
	/* 改变尺寸 */
	Resize_Widget(window, Size(320, 240)); 
	Widget_AutoSize( button, FALSE, 0 );
	Resize_Widget(button, Size(60, 25));
	/* 将窗口客户区作为这些部件的容器 */ 
	Window_Client_Area_Add(window, age_label);
	Window_Client_Area_Add(window, button); 
	for(i=0; i<6; i++) 
		Window_Client_Area_Add(window, age_rb[i]); 
	/* 调整部件的布局*/
	Set_Widget_Align(age_label, ALIGN_MIDDLE_CENTER, Pos(0,-40));
	Set_Widget_Align(button, ALIGN_MIDDLE_CENTER, Pos(0,60));
	Set_Widget_Align(age_rb[0], ALIGN_MIDDLE_LEFT, Pos(5,-20));
	Set_Widget_Align(age_rb[1], ALIGN_MIDDLE_LEFT, Pos(100,-20));
	Set_Widget_Align(age_rb[2], ALIGN_MIDDLE_LEFT, Pos(200,-20)); 
	Set_Widget_Align(age_rb[3], ALIGN_MIDDLE_LEFT, Pos(5,0));
	Set_Widget_Align(age_rb[4], ALIGN_MIDDLE_LEFT, Pos(100,0));
	Set_Widget_Align(age_rb[5], ALIGN_MIDDLE_LEFT, Pos(200,0)); 
	Label_Text(age_label, "你的年龄段是？");  
	
	RadioButton_Create_Mutex(age_rb[0], age_rb[1]);
	RadioButton_Create_Mutex(age_rb[0], age_rb[2]);
	RadioButton_Create_Mutex(age_rb[0], age_rb[3]);
	RadioButton_Create_Mutex(age_rb[0], age_rb[4]);
	RadioButton_Create_Mutex(age_rb[0], age_rb[5]);
	
	/* 显示部件 */
	Show_Widget(age_label);
	Show_Widget(button);
	Show_Widget(age_rb[0]); 
	for(i=0; i<6; ++i) 
		Show_Widget(age_rb[i]);  
	Show_Widget(window); 
	Widget_Clicked_Event_Connect(button, view_result, NULL);
	LCUI_Main(); /* 进入主循环 */ 
	return 0;
}

