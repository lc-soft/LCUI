/*
 * 测试部件的static定位类型的处理
 * */
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_GRAPH_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main(void)
{
	int i;
	char str[10];
	LCUI_RGB color[6]={{255,255,255},{127,127,127}, {255,0,0}, {255,165,0},
			{30,144,255}, {65,200,65} };
	LCUI_Widget *window, *widget[6], *label[6];
	
	LCUI_Init();
	window = Widget_New( "window" );
	for(i=0; i<6; ++i) {
		widget[i] = Widget_New( NULL );
		label[i] = Widget_New( "label" );
		sprintf(str, "Area %d" , i);
		Label_Text( label[i], str);
		Widget_Container_Add( widget[i], label[i] );
		Window_ClientArea_Add( window, widget[i] );
		Widget_Resize( widget[i], Size(80,80) );
		Widget_SetPosType( widget[i], POS_TYPE_STATIC );
		Widget_SetAlign( label[i], ALIGN_MIDDLE_CENTER, Pos(0,0) );
		Widget_SetBorder( widget[i], Border(1,BORDER_STYLE_SOLID,RGB(0,0,0)) );
		Widget_SetBackgroundTransparent( widget[i], FALSE );
		Widget_SetBackgroundColor( widget[i], color[i] );
		Widget_Show( label[i] );
		Widget_Show( widget[i] );
	}
	
	Window_SetTitleText( window, "测试部件布局-2" ); 
	Widget_Resize( window, Size(320, 240) );
	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );
	
	Widget_Show( window );
	
	return LCUI_Main();
}

