
#include <LCUI_Build.h> /* 包含LCUI的头文件 */
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H 
#include LC_LABEL_H
#include LC_SCROLLBAR_H
#include LC_GRAPH_H
#include LC_RES_H 

LCUI_Widget *label, *window, *scrollbar; 

void callback_func( ScrollBar_Data data, void *arg )
{
	Label_Text( label, "%d", data.current_num );
}

int main(int argc, char*argv[]) 
{
	LCUI_Init(argc, argv);
	
	window = Create_Widget("window");
	scrollbar = Create_Widget("scrollbar");
	label = Create_Widget("label");
	/* 设定窗口标题的文本 */
	Set_Window_Title_Text(window, "测试滚动条"); 
	/* 改变窗口的尺寸 */
	window->resize(window, Size(320, 240));
	Resize_Widget( scrollbar, Size(10, 100) );
	ScrollBar_Set_MaxSize( scrollbar, 400 );
	/* 设置部件布局 */
	Set_Widget_Align( label, ALIGN_MIDDLE_CENTER, Pos(-20,0) );
	Set_Widget_Align( scrollbar, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	
	Label_Text( label, "0" );
	
	/* 将窗口客户区作为部件的容器添加进去 */
	Window_Client_Area_Add(window, label);
	Window_Client_Area_Add(window, scrollbar); 
	/* 将回调函数与滚动条部件连接 */
	ScrollBar_Connect( scrollbar, callback_func, NULL );
	/* 显示部件 */
	scrollbar->show(scrollbar);
	window->show(window); 
	label->show(label);
	return LCUI_Main(); /* 进入主循环 */
}

