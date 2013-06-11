#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H

static void func(void *arg)
{
	int i;
	LCUI_Widget *widget;
	widget = Widget_New(NULL);
	Widget_Resize( widget, Size(800,600) );
	Widget_SetBackgroundTransparent( widget, FALSE );
	Widget_SetBackgroundColor( widget, RGB(255,255,255) );
	Widget_Show( widget );
	for(i=0; i<=400;++i) {
		Widget_Move( widget, Pos(i,0) );
		LCUI_MSleep(5);
	}
	Widget_Move( widget, Pos(400,300) );
	LCUI_Sleep(2);
	LCUI_MainLoop_Quit(NULL);
	LCUIThread_Exit(NULL);
}

int LCUIMainFunc(LCUI_ARGLIST)
{
	LCUI_Thread t;
	LCUI_Init(LCUI_DEFAULT_CONFIG);
	LCUIThread_Create( &t, func, NULL );
	return LCUI_Main();
}

