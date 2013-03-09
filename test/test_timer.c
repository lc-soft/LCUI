// 测试LCUI的定时器

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include <unistd.h>

static int state = 0;
static LCUI_Widget *label;

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

static void display_text(void)
{
	if( state == 0 ) {
		Widget_Show( label );
		state = 1;
	} else {
		Widget_Hide( label );
		state = 0;
	}
}

int main(void) 
{
	LCUI_Widget *window;
	LCUI_TextStyle style;
	
	LCUI_Init();
	TextStyle_Init( &style );
	
	window  = Widget_New("window");
	label   = Widget_New("label");
	
	Window_SetTitleText( window, "测试定时器" );
	Widget_Resize( window, Size(320, 240) );
	Window_ClientArea_Add( window, label );
	Widget_SetAlign( label, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Label_Text( label, "利用定时器实现的文本闪烁" );
	TextStyle_FontSize( &style, 20 );
	Label_TextStyle( label, style );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	/* 设置定时器，每隔500毫秒调用display_text函数，重复调用 */
	set_timer( 500, display_text, TRUE );
	Widget_Show( window ); 
	return LCUI_Main();
}
