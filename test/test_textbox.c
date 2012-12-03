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
	FILE *fp;
	char buff[256];
	LCUI_Widget *window, *textbox;
	
	LCUI_Init(argc, argv);
	
	window  = Create_Widget("window");
	textbox = Create_Widget("text_box");
	 
	Set_Window_Title_Text(window, "测试文本框部件"); 
	window->resize(window, Size(320, 240)); 
	Window_Client_Area_Add(window, textbox);
	TextBox_Using_StyleTags( textbox, TRUE );
	textbox->set_align( textbox, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	/* 打开README.md文件，并将内容读取至文本框上显示 */
	fp = fopen("../README.md", "r");
	if( fp ) {
		while(fgets( buff, sizeof(buff), fp )) {
			TextBox_Text_Append( textbox, buff );
		}
		fclose( fp );
	}
	Set_Widget_Dock( textbox, DOCK_TYPE_FILL );
	textbox->show(textbox);
	window->show(window); 
	return LCUI_Main(); 
}

