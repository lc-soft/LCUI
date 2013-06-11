// 测试图像显示
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_GRAPH_H
#include LC_RES_H

static void destroy( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

int main( int argc, char*argv[] )
{
	LCUI_Graph pic;
	int width = 320, height = 240;
	LCUI_Widget *window, *label, *pic_box, *fore_box;

	LCUI_Init(0,0,0);
	Graph_Init(&pic);

	window	 = Widget_New("window");
	label	 = Widget_New("label");
	pic_box	 = Widget_New("picture_box");
	fore_box = Widget_New("picture_box");

	Widget_Resize(fore_box, Size(190, 190));
	Widget_Resize(pic_box, Size(135,135));
	Widget_Resize(window, Size(width, height));

	Window_SetTitleText(window, "头像");

	PictureBox_SetSizeMode(pic_box, SIZE_MODE_STRETCH);
	PictureBox_SetImageFile(pic_box, "image.jpg");
	PictureBox_SetImageFile(fore_box, "border.png");

	Label_Text(label, "蛋疼的头像");

	Widget_SetAlign(pic_box, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Widget_SetAlign(fore_box, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Widget_SetAlign(label, ALIGN_MIDDLE_CENTER, Pos(0, +75));

	Window_ClientArea_Add(window, label);
	Window_ClientArea_Add(window, pic_box);
	Window_ClientArea_Add(window, fore_box);

	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, destroy );

	Widget_Show(label);
	Widget_Show(pic_box);
	Widget_Show(fore_box);
	Widget_Show(window);
	return LCUI_Main();
}

