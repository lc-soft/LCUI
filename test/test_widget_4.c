// 测试圆角边框绘制

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H 
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_RES_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(int argc, char*argv[]) 
{
	LCUI_Widget *window, *round;
	LCUI_Border border;
	
	LCUI_Init();
	
	/* 创建部件 */
	window  = Widget_New("window");
	round = Widget_New(NULL);
	border = Border( 1, BORDER_STYLE_SOLID, RGB(0,0,0) );
	Border_Radius( &border, 10 );
	//border.bottom_width = 0;
	//border.right_width = 0;
	//border.bottom_left_radius = 0;
	//border.bottom_right_radius = 0;
	//border.top_right_radius = 0;
	//border.left_width = 0;
	Widget_Resize( round, Size(100, 100) );
	Widget_SetBackgroundColor( round, RGB(200,200,200) );
	Widget_SetBackgroundTransparent( round, FALSE );
	Widget_SetBorder( round, border );
	Widget_SetAlign( round, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	/* 设定窗口标题的文本 */
	Window_SetTitleText(window, "测试圆角边框");
	/* 改变窗口的尺寸 */
	window->resize(window, Size(320, 240));
	Window_ClientArea_Add( window, round );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	window->show(window); 
	Widget_Show( round );
	return LCUI_Main();
}
