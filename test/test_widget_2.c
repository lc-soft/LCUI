/*
 * 测试部件的static定位类型的处理
 * */
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_GRAPH_H
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;
	LCUI_Init( argc, argv );
	LCUI_RGB color[6]={{255,255,255},{127,127,127}, {255,0,0}, {255,165,0},
			{30,144,255}, {65,200,65} };
	LCUI_Widget *window, *widget[6], *label[6];
	
	window = Create_Widget( "window" );
	for(i=0; i<6; ++i) {
		widget[i] = Create_Widget( NULL );
		label[i] = Create_Widget( "label" );
		Set_Label_Text( label[i], "Area %d" , i);
		Widget_Container_Add( widget[i], label[i] );
		Window_Client_Area_Add( window, widget[i] );
		Resize_Widget( widget[i], Size(80,80) );
		Set_Widget_PosType( widget[i], POS_TYPE_STATIC );
		Set_Widget_Align( label[i], ALIGN_MIDDLE_CENTER, Pos(0,0) );
		Set_Widget_Border( widget[i], RGB(0,0,0), Border(1,1,1,1) );
		Set_Widget_BG_Mode( widget[i], BG_MODE_FILL_BACKCOLOR );
		Set_Widget_Backcolor( widget[i], color[i] );
		Show_Widget( label[i] );
		Show_Widget( widget[i] );
		printf("widget[%d]: %p\n", i, widget[i]);
	}
	Set_Window_Title_Text( window, "测试部件布局-2" ); 
	Resize_Widget( window, Size(320, 240) );
	
	Show_Widget( window );
	
	return LCUI_Main();
}

