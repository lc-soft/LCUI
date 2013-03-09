// 测试部件的状态改变

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(void)
{
	LCUI_Widget *wnd, *btn[3];
	LCUI_Init();
	wnd = Widget_New ("window");
	btn[0] = Widget_New ("button");
	btn[1] = Widget_New ("button");
	btn[2] = Widget_New ("button");
	Widget_Resize (btn[0], Size(200,200));
	Widget_Resize (btn[1], Size(100,100));
	Widget_Resize (btn[2], Size(50,50));
	Widget_Resize (wnd, Size(320,240));
	Widget_SetAutoSize( btn[0], 0, FALSE );
	Widget_SetAutoSize( btn[1], 0, FALSE );
	Widget_SetAutoSize( btn[2], 0, FALSE );
	Widget_Container_Add (btn[0], btn[1]);
	Widget_Container_Add (btn[1], btn[2]);
	Window_ClientArea_Add (wnd, btn[0]);
	Widget_SetAlign (btn[0], ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_SetAlign (btn[1], ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_SetAlign (btn[2], ALIGN_MIDDLE_CENTER, Pos(0,0));
	Window_SetTitleText( wnd, "测试部件的状态改变" );
	Widget_Event_Connect( Window_GetCloseButton(wnd), EVENT_CLICKED, destroy );
	Widget_Show (btn[0]);
	Widget_Show (btn[1]);
	Widget_Show (btn[2]);
	Widget_Show (wnd);
	return LCUI_Main();
}
