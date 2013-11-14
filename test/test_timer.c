// 测试LCUI的定时器
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H

static int state = 0;
static LCUI_Widget *label;

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	DEBUG_MSG("quit\n");
	LCUI_MainLoop_Quit(NULL);
}

static void display_text(void *arg)
{
	if( state == 0 ) {
		Widget_Show( label );
		state = 1;
	} else {
		Widget_Hide( label );
		state = 0;
	}
}

int main( int argc, char **argv )
{
	LCUI_Widget *window;
	LCUI_TextStyle style;

	LCUI_Init(0,0,0);
	TextStyle_Init( &style );

	window  = Widget_New("window");
	label   = Widget_New("label");

	Window_SetTitleTextW( window, L"测试定时器" );
	Widget_Resize( window, Size(320, 240) );
	Window_ClientArea_Add( window, label );
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Label_TextW( label, L"利用定时器实现的文本闪烁" );
	TextStyle_FontSize( &style, 20 );
	Label_TextStyle( label, style );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	/* 设置定时器，每隔500毫秒调用display_text函数，重复调用 */
	LCUITimer_Set( 500, display_text, NULL, TRUE );
	Widget_Show( window );
	return LCUI_Main();
}
