#include <LCUI_Build.h>
#include <unistd.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_WINDOW_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_RES_H

int main(int argc,char*argv[])
/* 主函数，程序的入口 */
{ 
	LCUI_Widget	*window, 
					*label,
					*pic_box,
					*fore_box; /* 使用指向widget部件的指针 */
					
	int			 width, height; 
	/* 自定义默认字体文件位置 */
	//Set_Default_Font("msyh.ttf");
	/* 初始化LCUI */ 
	LCUI_Init(argc, argv);
	/* 创建一个LCUI程序窗口 */
	width  = 320; /* 窗口的宽度 */
	height = 240; /* 窗口的高度 */
	/* 创建部件 */
	window		= Create_Widget("window");
	label		= Create_Widget("label");
	pic_box		= Create_Widget("picture_box");
	fore_box	= Create_Widget("picture_box");
	 
	Resize_Widget(fore_box, Size(190, 190));
	Resize_Widget(pic_box, Size(135,135));
	Resize_Widget(window, Size(width, height));
	 
	Set_Window_Title_Text(window, "头像");
	
	LCUI_Graph pic;
	Graph_Init(&pic); 
	Set_PictureBox_Size_Mode(pic_box, SIZE_MODE_STRETCH); 
	Set_PictureBox_Image_From_File(pic_box, "image.jpg");
	Set_PictureBox_Image_From_File(fore_box, "border.png");
	 
	Set_Label_Text(label, "蛋疼的头像");
	Set_Label_Font(label, 16, NULL);
	
	Set_Widget_Align(pic_box, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Set_Widget_Align(fore_box, ALIGN_MIDDLE_CENTER, Pos(0, -20));
	Set_Widget_Align(label, ALIGN_MIDDLE_CENTER, Pos(0, +75));
	
	Window_Client_Area_Add(window, label);
	Window_Client_Area_Add(window, pic_box); 
	Window_Client_Area_Add(window, fore_box);
	
	Show_Widget(label);
	Show_Widget(pic_box);
	Show_Widget(fore_box);
	Show_Widget(window); 
	return LCUI_Main(); 
}

