// 测试部件背景

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_GRAPH_H
#include LC_DRAW_H

static void destroy( LCUI_KeyboardEvent *event, void *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(void)
{
	LCUI_Graph img;
	LCUI_Widget *widget;
	
	LCUI_Init();
	Graph_Init( &img );
	widget = Widget_New(NULL);
	Load_Image("image.jpg", &img );
	Widget_Resize( widget, Size(320,240) );
	Widget_SetBackgroundTransparent( widget, FALSE );
	Widget_SetBackgroundImage( widget, &img );
	Widget_SetBackgroundLayout( widget, LAYOUT_STRETCH );
	Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	LCUI_KeyboardEvent_Connect( destroy, NULL );
	Widget_Show( widget );
	return LCUI_Main();
}

