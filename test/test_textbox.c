// 测试LCUI的文本框部件
#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_WINDOW_H
#include LC_TEXTBOX_H
#include LC_BUTTON_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_GRAPH_H

#define IMG_PATH_FACE	"image.png"
#define IMG_PATH_BORDER	"faceback.png"

static void callback( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	LCUI_MainLoop_Quit(NULL);
}

#define TEST_1
#ifdef TEST_1

#define USERNAME	L"root"
#define PASSWD		L"123456"

static LCUI_Widget *tb_username, *tb_password;

static void check_passwd( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	wchar_t buff[32];
	LCUI_WString username, passwd;

	LCUIWString_Init( &username );
	LCUIWString_Init( &passwd );

	TextBox_GetText( tb_username, buff, 32 );
	_LCUIWString_Copy( &username, buff );

	TextBox_GetText( tb_password, buff, 32 );
	_LCUIWString_Copy( &passwd, buff );

	if( _LCUIWString_Cmp( &username, USERNAME ) == 0
	 && _LCUIWString_Cmp( &passwd, PASSWD ) == 0 ) {
		LCUI_MessageBoxW( MB_ICON_INFO, L"账号及密码验证通过！", L"验证结果", MB_BTN_OK );
	 } else {
		LCUI_MessageBoxW( MB_ICON_ERROR, L"账号或密码输入错误！", L"验证结果", MB_BTN_OK );
	 }
}

int main(int argc, char**argv)
{
	LCUI_Widget *window, *text[2];
	LCUI_Widget *area, *login_btn;
	LCUI_Widget *fore_pb, *back_pb;

	LCUI_Init(800,600,LCUI_INIT_MODE_WINDOW);
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
	Window_SetTitleTextW(window, L"用户登录");
	Label_TextW( text[0], L"帐号" );
	Label_TextW( text[1], L"密码" );
	Button_TextW( login_btn, L"登录" );

	/* 调整部件的尺寸 */
	Widget_Resize( login_btn, Size(50, 50) );
	Widget_Resize( window, Size(320, 240) );
	Widget_Resize( back_pb, Size(128, 128));
	Widget_Resize( fore_pb, Size(95,95));
	Widget_Resize( area, Size(225,60) );

	/* 设置这两个图片框内显示的图片 */
	PictureBox_SetSizeMode( back_pb, SIZE_MODE_STRETCH );
	PictureBox_SetSizeMode( fore_pb, SIZE_MODE_STRETCH );
	PictureBox_SetImageFile( back_pb, IMG_PATH_BORDER );
	PictureBox_SetImageFile( fore_pb, IMG_PATH_FACE );

	/* 限制文本框内的字符总数为32个 */
	TextBox_Text_SetMaxLength( tb_username, 32 );
	TextBox_Text_SetMaxLength( tb_password, 32 );
	TextBox_Text_SetPasswordChar( tb_password, L'●' );

	/* 设置文本框的占位符 */
	TextBox_Text_SetPlaceHolderW( tb_username, NULL, L"用户名" );
	TextBox_Text_SetPlaceHolderW( tb_password, NULL, L"请输入密码" );

	/* 限制文本框输入的字符 */
	TextBox_LimitInput( tb_username, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.-" );
	Widget_Resize( tb_password, Size(125, 22) );
	Widget_Resize( tb_username, Size(125, 22) );

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
	Widget_SetAlign( area, ALIGN_MIDDLE_CENTER, Pos(0,60) );
	Widget_SetAlign( login_btn, ALIGN_MIDDLE_RIGHT, Pos(0,0) );
	Widget_SetAlign( fore_pb, ALIGN_MIDDLE_CENTER, Pos(-2,-2) );
	Widget_SetAlign( back_pb, ALIGN_MIDDLE_CENTER, Pos(0,-35) );
	Widget_SetAlign( text[0], ALIGN_MIDDLE_LEFT, Pos(4,-12) );
	Widget_SetAlign( text[1], ALIGN_MIDDLE_LEFT, Pos(4,12) );
	Widget_SetAlign( tb_username, ALIGN_MIDDLE_LEFT, Pos(35,-12) );
	Widget_SetAlign( tb_password, ALIGN_MIDDLE_LEFT, Pos(35,12) );

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
	Widget_Show( tb_password );
	Widget_Show( tb_username );
	Widget_Show( text[0] );
	Widget_Show( text[1] );
	Widget_Show( window );

	Widget_Event_Connect( Window_GetCloseButton(window), EVENT_CLICKED, callback );
	Widget_Event_Connect( login_btn, EVENT_CLICKED, check_passwd );
	/* 进入主循环 */
	return LCUI_Main();
}

#else
int main(int argc, char*argv[])
{
	FILE *fp;
	char buff[256];
	LCUI_Widget *window, *textbox;

	LCUI_Init(800,600,LCUI_INIT_MODE_AUTO);

	window  = Widget_New("window");
	textbox = Widget_New("text_box");

	Window_SetTitleTextW(window, L"测试文本框部件");
	window->resize( window, Size(640, 480) );
	Window_ClientArea_Add( window, textbox );
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
	Widget_SetDock( textbox, DOCK_TYPE_FILL );
	textbox->show(textbox);
	window->show(window);
	Widget_Event_Connect(Window_GetCloseButton(window), EVENT_CLICKED, callback);
	return LCUI_Main();
}
#endif
