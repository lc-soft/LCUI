// 测试LCUI的文本框部件

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H 
#include LC_WINDOW_H 
#include LC_TEXTBOX_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_RES_H
#include <unistd.h>

//#define TEST_1
#ifdef TEST_1
int main(int argc, char*argv[]) 
{
	int mode;
	LCUI_Widget *window, *text[2], *tb_username, *tb_password;
	LCUI_Widget *area, *login_btn;
	LCUI_Widget *fore_pb, *back_pb;
	
	LCUI_Init(argc, argv);
	/* 创建所需的部件 */
	window = Create_Widget("window");
	area = Create_Widget(NULL);
	tb_password = Create_Widget("text_box");
	tb_username = Create_Widget("text_box");
	text[0] = Create_Widget("label");
	text[1] = Create_Widget("label");
	fore_pb = Create_Widget("picture_box");
	back_pb = Create_Widget("picture_box");
	login_btn = Create_Widget("button");
	/* 设置界面上显示的文本 */
	Set_Window_Title_Text(window, "用户登录"); 
	Label_Text( text[0], "帐号：" );
	Label_Text( text[1], "密码：" );
	Set_Button_Text( login_btn, "登录" );
	/* 调整部件的尺寸 */
	Resize_Widget( login_btn, Size(50, 50) ); 
	Resize_Widget( window, Size(320, 240) ); 
	Resize_Widget( back_pb, Size(128, 128));
	Resize_Widget( fore_pb, Size(95,95));
	Resize_Widget( area, Size(225,50) );
	/* 设置这两个图片框内显示的图片 */
	Set_PictureBox_Size_Mode( back_pb, SIZE_MODE_STRETCH ); 
	Set_PictureBox_Size_Mode( fore_pb, SIZE_MODE_STRETCH );
	Set_PictureBox_Image_From_File( back_pb, "faceback.png" );
	Set_PictureBox_Image_From_File( fore_pb, "image.jpg" );
	
	/* 限制文本框内的字符总数为32个 */
	TextBox_Text_Set_MaxLength( tb_username, 32 );
	TextBox_Text_Set_MaxLength( tb_password, 32 );
	tb_password->resize( tb_password, Size(125, 22) );
	TextBox_Text_Set_PasswordChar( tb_password, L'●' );
	/* 限制文本框输入的字符 */
	mode = ONLY_0_TO_9 | ONLY_A_TO_Z | ONLY_a_TO_z | ONLY_UNDERLINE;
	TextBox_Text_Limit( tb_username, mode );
	tb_username->resize( tb_username, Size(125, 22) );
	/* 将部件添加至相应的容器中 */
	Widget_Container_Add( area, text[0] );
	Widget_Container_Add( area, text[1] );
	Widget_Container_Add( area, login_btn );
	Widget_Container_Add( back_pb, fore_pb );
	Widget_Container_Add( area, tb_username );
	Widget_Container_Add( area, tb_password );
	Window_Client_Area_Add( window, back_pb );
	Window_Client_Area_Add( window, area );
	/* 设置部件的布局 */
	area->set_align( area, ALIGN_MIDDLE_CENTER, Pos(0,60) );
	login_btn->set_align( login_btn, ALIGN_MIDDLE_RIGHT, Pos(0,0) );
	back_pb->set_align( fore_pb, ALIGN_MIDDLE_CENTER, Pos(-2,-2) );
	back_pb->set_align( back_pb, ALIGN_MIDDLE_CENTER, Pos(0,-35) );
	text[0]->set_align( text[0], ALIGN_MIDDLE_LEFT, Pos(0,-12) );
	text[1]->set_align( text[1], ALIGN_MIDDLE_LEFT, Pos(0,12) );
	tb_username->set_align( tb_username, ALIGN_MIDDLE_LEFT, Pos(35,-12) );
	tb_password->set_align( tb_password, ALIGN_MIDDLE_LEFT, Pos(35,12) );
	/* 设置边框 */
	Set_Widget_Border( fore_pb, Border(1, BORDER_STYLE_SOLID, RGB(120,120,120)) );
	/* 显示这些部件 */
	Show_Widget( back_pb );
	Show_Widget( fore_pb );
	Show_Widget( login_btn );
	Show_Widget( area );
	tb_password->show( tb_password );
	tb_username->show( tb_username );
	text[0]->show( text[0] );
	text[1]->show( text[1] );
	window->show( window );
	/* 进入主循环 */
	return LCUI_Main(); 
}
#else
int main(int argc, char*argv[]) 
{
	FILE *fp;
	char buff[256];
	LCUI_Widget *window, *textbox;
	
	LCUI_Init(argc, argv);
	
	window  = Create_Widget("window");
	textbox = Create_Widget("text_box");

	Set_Window_Title_Text(window, "测试文本框部件"); 
	window->resize( window, Size(320, 240) ); 
	Window_Client_Area_Add( window, textbox );
	/* 启用多行文本显示 */
	TextBox_Multiline( textbox, TRUE );
	textbox->set_align( textbox, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	/* 打开README.md文件，并将内容读取至文本框上显示 */
	fp = fopen("../README.md", "r");
	if( fp ) {
		while(fgets( buff, sizeof(buff), fp )) {
			TextBox_Text_Append( textbox, buff );
		}
		fclose( fp );
	}
	Set_Widget_Dock( textbox, DOCK_TYPE_FILL );
	textbox->show(textbox);
	window->show(window);
	return LCUI_Main(); 
}
#endif
