// 测试复选框部件的程序例子

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_CHECKBOX_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H
#include <unistd.h>

LCUI_Widget *checkbox[5], *button, *window, *label, *result_text; 

void view_result(LCUI_Widget *widget, void *arg)
{
	char str[256]; 
	strcpy(str, "你的兴趣爱好有： ");
	/* 检测各个复选框是否被选中，以确定内容 */
	if(CheckBox_Is_On(checkbox[0]))
		strcat(str, "编程 ");
	if(CheckBox_Is_On(checkbox[1]))
		strcat(str, "电影 ");
	if(CheckBox_Is_On(checkbox[2]))
		strcat(str, "动漫 ");
	if(CheckBox_Is_On(checkbox[3]))
		strcat(str, "音乐 ");
	if(CheckBox_Is_On(checkbox[4]))
		strcat(str, "睡觉 ");
	
	strcat(str, "。");
	/* 设定文本内容 */
	Set_Label_Text(result_text, str);
	/* 禁用按钮部件 */
	Disable_Widget(button);
}

int main(int argc, char*argv[]) 
{
    int i;
    LCUI_Init(argc, argv);
    /* 创建部件 */
    window  = Create_Widget("window");
    label   = Create_Widget("label");
    button	= Create_Button_With_Text("提交");
    result_text = Create_Widget("label");
    checkbox[0] = Create_CheckBox_With_Text("编程");
    checkbox[1] = Create_CheckBox_With_Text("电影");
    checkbox[2] = Create_CheckBox_With_Text("动漫");
    checkbox[3] = Create_CheckBox_With_Text("音乐");
    checkbox[4] = Create_CheckBox_With_Text("睡觉");
    /* 设定窗口标题的文本 */
    Set_Window_Title_Text(window, "测试复选框部件"); 
    /* 改变窗口尺寸 */
    window->resize(window, Size(320, 240));
    /* 禁用按钮部件的自动尺寸调整，因为要自定义尺寸 */
    Disable_Widget_Auto_Size(button);
    Resize_Widget(button, Size(60, 25));
    /* 将窗口客户区作为这些部件的容器 */
    Window_Client_Area_Add(window, label);
    Window_Client_Area_Add(window, button);
    Window_Client_Area_Add(window, result_text);
    for(i=0; i<5; i++) 
		Window_Client_Area_Add(window, checkbox[i]); 
    /* 调整部件的布局*/
    Set_Widget_Align(label, ALIGN_MIDDLE_CENTER, Pos(0,-20));
    Set_Widget_Align(button, ALIGN_MIDDLE_CENTER, Pos(0,60));
    Set_Widget_Align(result_text, ALIGN_MIDDLE_CENTER, Pos(0, 30));
	Set_Widget_Align(checkbox[0], ALIGN_MIDDLE_CENTER, Pos(-120 ,5));
	Set_Widget_Align(checkbox[1], ALIGN_MIDDLE_CENTER, Pos(-60 ,5));
	Set_Widget_Align(checkbox[2], ALIGN_MIDDLE_CENTER, Pos(0 ,5));
	Set_Widget_Align(checkbox[3], ALIGN_MIDDLE_CENTER, Pos(60 ,5));
	Set_Widget_Align(checkbox[4], ALIGN_MIDDLE_CENTER, Pos(120 ,5));
	/* 设定label部件显示的文本，以及字体大小 */
    Set_Label_Text(label, "你都有哪些兴趣爱好？\n"); 
    Set_Label_Font(label, 14, NULL);
    /* 显示部件 */
    Show_Widget(label);
    Show_Widget(result_text);
    Show_Widget(button);
    for(i=0; i<5; ++i) 
		Show_Widget(checkbox[i]);  
    Show_Widget(window); 
    /* 为按钮部件关联点击事件，被关联的函数是view_result，它会在点击按钮后被调用 */
	Widget_Clicked_Event_Connect(button, view_result, NULL);
    return LCUI_Main(); /* 进入主循环 */
}

