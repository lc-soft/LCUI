// 测试LCUI的文本框部件

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_TEXTSTYLE_H
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

static void callback( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

#define TEST_1
#ifdef TEST_1
int main(int argc, char*argv[]) 
{
	int mode;
	LCUI_Widget *window, *text[2], *tb_username, *tb_password;
	LCUI_Widget *area, *login_btn;
	LCUI_Widget *fore_pb, *back_pb;
	
	LCUI_Init(argc, argv);
	/* 创建所需的部件 */
	window = Widget_New("window");
	area = Widget_New(NULL);
	tb_password = Widget_New("text_box");
	tb_username = Widget_New("text_box");
	text[0] = Widget_New("label");
	text[1] = Widget_New("label");
	fore_pb = Widget_New("picture_box");
	back_pb = Widget_New("picture_box");
	login_btn = Widget_New("button");
	/* 设置界面上显示的文本 */
	Window_SetTitleText(window, "用户登录"); 
	Label_Text( text[0], "帐号" );
	Label_Text( text[1], "密码" );
	Set_Button_Text( login_btn, "登录" );
	/* 调整部件的尺寸 */
	Widget_Resize( login_btn, Size(50, 50) ); 
	Widget_Resize( window, Size(320, 240) ); 
	Widget_Resize( back_pb, Size(128, 128));
	Widget_Resize( fore_pb, Size(95,95));
	Widget_Resize( area, Size(225,60) );
	/* 设置这两个图片框内显示的图片 */
	PictureBox_SetSizeMode( back_pb, SIZE_MODE_STRETCH ); 
	PictureBox_SetSizeMode( fore_pb, SIZE_MODE_STRETCH );
	PictureBox_SetImageFile( back_pb, "faceback.png" );
	PictureBox_SetImageFile( fore_pb, "image.jpg" );
	
	/* 限制文本框内的字符总数为32个 */
	TextBox_Text_SetMaxLength( tb_username, 32 );
	TextBox_Text_SetMaxLength( tb_password, 32 );
	TextBox_Text_SetPasswordChar( tb_password, L'●' );
	/* 设置文本框的占位符 */
	TextBox_Text_SetPlaceHolder( tb_username, NULL, "用户名" );
	TextBox_Text_SetPlaceHolder( tb_password, NULL, "请输入密码" );
	/* 限制文本框输入的字符 */
	mode = ONLY_0_TO_9 | ONLY_A_TO_Z | ONLY_a_TO_z | ONLY_UNDERLINE;
	TextBox_Text_Limit( tb_username, mode );
	tb_password->resize( tb_password, Size(125, 22) );
	tb_username->resize( tb_username, Size(125, 22) );
	/* 将部件添加至相应的容器中 */
	Widget_Container_Add( area, text[0] );
	Widget_Container_Add( area, text[1] );
	Widget_Container_Add( area, login_btn );
	Widget_Container_Add( back_pb, fore_pb );
	Widget_Container_Add( area, tb_username );
	Widget_Container_Add( area, tb_password );
	Window_ClientArea_Add( window, back_pb );
	Window_ClientArea_Add( window, area );
	/* 设置部件的布局 */
	area->set_align( area, ALIGN_MIDDLE_CENTER, Pos(0,60) );
	login_btn->set_align( login_btn, ALIGN_MIDDLE_RIGHT, Pos(0,0) );
	back_pb->set_align( fore_pb, ALIGN_MIDDLE_CENTER, Pos(-2,-2) );
	back_pb->set_align( back_pb, ALIGN_MIDDLE_CENTER, Pos(0,-35) );
	text[0]->set_align( text[0], ALIGN_MIDDLE_LEFT, Pos(4,-12) );
	text[1]->set_align( text[1], ALIGN_MIDDLE_LEFT, Pos(4,12) );
	tb_username->set_align( tb_username, ALIGN_MIDDLE_LEFT, Pos(35,-12) );
	tb_password->set_align( tb_password, ALIGN_MIDDLE_LEFT, Pos(35,12) );
	/* 设置边框 */
	Widget_SetBorder( fore_pb, Border(1, BORDER_STYLE_SOLID, RGB(120,120,120)) );
	Widget_SetBorder( area, Border(1, BORDER_STYLE_SOLID, RGB(209,209,209)) );
	Widget_SetBorderRadius( area, 2 );
	Widget_SetPadding( area, Padding(5,5,5,5) );
	/* 显示这些部件 */
	Widget_Show( back_pb );
	Widget_Show( fore_pb );
	Widget_Show( login_btn );
	Widget_Show( area );
	tb_password->show( tb_password );
	tb_username->show( tb_username );
	text[0]->show( text[0] );
	text[1]->show( text[1] );
	window->show( window );
	Widget_Event_Connect(Window_GetCloseButton(window), EVENT_CLICKED, callback);
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
	
	window  = Widget_New("window");
	textbox = Widget_New("text_box");

	Window_SetTitleText(window, "测试文本框部件"); 
	window->resize( window, Size(640, 480) ); 
	Window_ClientArea_Add( window, textbox );
	/* 启用多行文本显示 */
	TextBox_Multiline( textbox, TRUE );
	textbox->set_align( textbox, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	/* 打开README.md文件，并将内容读取至文本框上显示 */
	fp = fopen("../README", "r");
	if( fp ) {
		while(fgets( buff, sizeof(buff), fp )) {
			TextBox_Text_Append( textbox, buff );
		}
		fclose( fp );
	}
	Widget_SetDock( textbox, DOCK_TYPE_FILL );
	textbox->show(textbox);
	window->show(window);
	Widget_Event_Connect(Window_GetCloseButton(window), EVENT_CLICKED, callback);
	return LCUI_Main(); 
}
#endif
