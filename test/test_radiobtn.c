// 测试单选框部件的示例程序
#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_RADIOBTN_H
#include LC_GRAPH_H
#include LC_RES_H
#include <unistd.h>

LCUI_Widget *age_rb[6], *button, *window, *age_label; 

void view_result(LCUI_Widget *widget, LCUI_WidgetEvent *unused)
{
	char str[256]; 
	strcpy(str, "你的年龄段是： ");
	if(RadioButton_IsOn(age_rb[0])) {
		strcat(str, "15岁以下");
	}
	if(RadioButton_IsOn(age_rb[1])) {
		strcat(str, "15-20岁");
	}
	if(RadioButton_IsOn(age_rb[2])) {
		strcat(str, "21-25岁");
	}
	if(RadioButton_IsOn(age_rb[3])) {
		strcat(str, "26-30岁");
	}
	if(RadioButton_IsOn(age_rb[4])) {
		strcat(str, "31-40岁");
	}
	if(RadioButton_IsOn(age_rb[5])) {
		strcat(str, "40岁以上");
	}
	
	strcat(str, "。");
	Label_Text(age_label, str);  
	Widget_Disable(button);
}

int main(int argc, char*argv[])
/* 主函数，程序的入口 */
{
	int i;
	LCUI_Init(argc, argv);
	/* 创建部件 */
	window  = Widget_New("window");
	age_label = Widget_New("label"); 
	button	= Button_New("提交"); 
	age_rb[0] = RadioButton_New("A. 15岁以下");
	age_rb[1] = RadioButton_New("B. 15-20岁");
	age_rb[2] = RadioButton_New("C. 21-25岁");
	age_rb[3] = RadioButton_New("D. 26-30岁");
	age_rb[4] = RadioButton_New("E. 31-40岁");
	age_rb[5] = RadioButton_New("F. 40岁以上");
	/* 设定窗口标题的文本 */
	Window_SetTitleText(window, "测试复选框部件"); 
	/* 改变尺寸 */
	Widget_Resize(window, Size(320, 240)); 
	Widget_SetAutoSize( button, FALSE, 0 );
	Widget_Resize(button, Size(60, 25));
	/* 将窗口客户区作为这些部件的容器 */ 
	Window_ClientArea_Add(window, age_label);
	Window_ClientArea_Add(window, button); 
	for(i=0; i<6; i++) {
		Window_ClientArea_Add(window, age_rb[i]); 
	}
	/* 调整部件的布局*/
	Widget_SetAlign(age_label, ALIGN_MIDDLE_CENTER, Pos(0,-40));
	Widget_SetAlign(button, ALIGN_MIDDLE_CENTER, Pos(0,60));
	Widget_SetAlign(age_rb[0], ALIGN_MIDDLE_LEFT, Pos(5,-20));
	Widget_SetAlign(age_rb[1], ALIGN_MIDDLE_LEFT, Pos(100,-20));
	Widget_SetAlign(age_rb[2], ALIGN_MIDDLE_LEFT, Pos(200,-20)); 
	Widget_SetAlign(age_rb[3], ALIGN_MIDDLE_LEFT, Pos(5,0));
	Widget_SetAlign(age_rb[4], ALIGN_MIDDLE_LEFT, Pos(100,0));
	Widget_SetAlign(age_rb[5], ALIGN_MIDDLE_LEFT, Pos(200,0)); 
	Label_Text(age_label, "你的年龄段是？");  
	
	RadioButton_CreateMutex(age_rb[0], age_rb[1]);
	RadioButton_CreateMutex(age_rb[0], age_rb[2]);
	RadioButton_CreateMutex(age_rb[0], age_rb[3]);
	RadioButton_CreateMutex(age_rb[0], age_rb[4]);
	RadioButton_CreateMutex(age_rb[0], age_rb[5]);
	
	/* 显示部件 */
	Widget_Show(age_label);
	Widget_Show(button);
	Widget_Show(age_rb[0]); 
	for(i=0; i<6; ++i) {
		Widget_Show(age_rb[i]); 
	}
	Widget_Show(window); 
	Widget_Event_Connect(button, EVENT_CLICKED, view_result );
	LCUI_Main();
	return 0;
}

