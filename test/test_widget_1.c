/*
 * 测试部件的Dock属性
 * */
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_GRAPH_H
#include <unistd.h>
static void *thread(void *arg)
{
	LCUI_Widget *widget, *label;
	widget = (LCUI_Widget*)arg; 
	label = Create_Widget( "label" );
	Widget_Container_Add( widget, label );
	Set_Widget_Align( label, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Show_Widget( label );
	while(LCUI_Active()) {
		Label_Text( label, "DOCK_TYPE_TOP" );
		Set_Widget_Dock( widget, DOCK_TYPE_TOP ); 
		Set_Widget_Size( widget, "120px", "120px" ); 
		/* 还原尺寸 */
		sleep(1);
		Label_Text( label, "DOCK_TYPE_LEFT" );
		Set_Widget_Dock( widget, DOCK_TYPE_LEFT ); 
		Set_Widget_Size( widget, "120px", "120px" ); 
		sleep(1);
		Label_Text( label, "DOCK_TYPE_RIGHT" );
		Set_Widget_Dock( widget, DOCK_TYPE_RIGHT ); 
		Set_Widget_Size( widget, "120px", "120px" ); 
		sleep(1);
		Label_Text( label, "DOCK_TYPE_BOTTOM" );
		Set_Widget_Dock( widget, DOCK_TYPE_BOTTOM ); 
		Set_Widget_Size( widget, "120px", "120px" ); 
		sleep(1);
		Label_Text( label, "DOCK_TYPE_FILL" );
		Set_Widget_Dock( widget, DOCK_TYPE_FILL ); 
		Set_Widget_Size( widget, "120px", "120px" ); 
		sleep(1);
		Label_Text( label, "DOCK_TYPE_NONE" );
		Set_Widget_Dock( widget, DOCK_TYPE_NONE ); 
		Set_Widget_Size( widget, "120px", "120px" );  
		sleep(1);
	}
	LCUI_Thread_Exit(NULL);
}

int main(int argc, char *argv[])
{
	thread_t t;
	LCUI_Init( argc, argv );
	
	LCUI_Widget *window, *widget;
	
	window = Create_Widget( "window" );
	widget = Create_Widget( NULL );
	Set_Window_Title_Text( window, "测试部件布局-1" );
	Window_Client_Area_Add( window, widget );
	Resize_Widget( widget, Size(120, 120) );
	Resize_Widget( window, Size(320, 240) );
	Set_Widget_BG_Mode( widget, BG_MODE_FILL_BACKCOLOR );
	Set_Widget_Border( widget, RGB(0,0,0), Border(1,1,1,1) );
	Set_Widget_Backcolor( widget, RGB(255,0,0) );
	
	Show_Widget( widget );
	Show_Widget( window );
	
	LCUI_Thread_Create( &t, NULL, thread, widget );
	
	return LCUI_Main();
}

