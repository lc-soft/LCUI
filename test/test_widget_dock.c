/*
 * 测试部件的Dock属性
 * */
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_GRAPH_H

static void thread(void *arg)
{
	LCUI_Widget *widget, *label;

	widget = (LCUI_Widget*)arg;
	label = Widget_New( "label" );
	Widget_Container_Add( widget, label );
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_Show( label );
	while(LCUI_Active()) {
		Label_Text( label, "DOCK_TYPE_TOP" );
		Widget_SetDock( widget, DOCK_TYPE_TOP );
		Widget_SetSize( widget, "120px", "120px" );
		/* 还原尺寸 */
		LCUI_Sleep(1);
		Label_Text( label, "DOCK_TYPE_LEFT" );
		Widget_SetDock( widget, DOCK_TYPE_LEFT );
		Widget_SetSize( widget, "120px", "120px" );
		LCUI_Sleep(1);
		Label_Text( label, "DOCK_TYPE_RIGHT" );
		Widget_SetDock( widget, DOCK_TYPE_RIGHT );
		Widget_SetSize( widget, "120px", "120px" );
		LCUI_Sleep(1);
		Label_Text( label, "DOCK_TYPE_BOTTOM" );
		Widget_SetDock( widget, DOCK_TYPE_BOTTOM );
		Widget_SetSize( widget, "120px", "120px" );
		LCUI_Sleep(1);
		Label_Text( label, "DOCK_TYPE_FILL" );
		Widget_SetDock( widget, DOCK_TYPE_FILL );
		Widget_SetSize( widget, "120px", "120px" );
		LCUI_Sleep(1);
		Label_Text( label, "DOCK_TYPE_NONE" );
		Widget_SetDock( widget, DOCK_TYPE_NONE );
		Widget_SetSize( widget, "120px", "120px" );
		LCUI_Sleep(1);
	}
	LCUIThread_Exit(NULL);
}

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main( int argc, char **argv )
{
	LCUI_Thread t;
	LCUI_Widget *window, *widget;

	LCUI_Init(0,0,0);
	window = Widget_New( "window" );
	widget = Widget_New( NULL );
	Window_SetTitleText( window, "测试部件布局-1" );
	Window_ClientArea_Add( window, widget );
	Widget_Resize( widget, Size(120, 120) );
	Widget_Resize( window, Size(320, 240) );
	Widget_SetBackgroundTransparent( widget, FALSE );
	Widget_SetBorder( widget, Border(1, BORDER_STYLE_SOLID, RGB(0,0,0)) );
	Widget_SetBackgroundColor( widget, RGB(255,0,0) );

	Widget_Show( widget );
	Widget_Show( window );
	Widget_ConnectEvent( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	LCUIThread_Create( &t, thread, widget );

	return LCUI_Main();
}

