// 测试LCUI的文本框部件

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H 
#include LC_TEXTBOX_H
#include LC_GRAPH_H
#include LC_RES_H
#include <unistd.h>

int main(int argc, char*argv[]) 
{
	LCUI_Init(argc, argv);
	
	LCUI_Widget *window, *textbox;
	
	window  = Create_Widget("window");
	textbox = Create_Widget("text_box");
	 
	Set_Window_Title_Text(window, "测试文本框部件"); 
	window->resize(window, Size(320, 240)); 
	Window_Client_Area_Add(window, textbox);
	textbox->set_align( textbox, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	TextBox_Text( textbox, "在此输入文本，仅支持英文字母+符号。"); 
	Resize_Widget(textbox, Size(300, 200));
	textbox->show(textbox);
	window->show(window); 
	return LCUI_Main(); 
}

