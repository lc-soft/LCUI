#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H
#include LC_LABEL_H
#include LC_TEXTBOX_H
#include LC_BUTTON_H
#include LC_DISPLAY_H
#include LC_STYLE_LIBRARY_H

#define MAX_CLOUD 3

static int total_cloud = 0;
static LCUI_Widget *(wdg_cloud[MAX_CLOUD]);
static LCUI_Widget *error_tip, *imgbox, *btn, *usernamelabel, *passwdbox;
static LCUI_Graph bg_img, cloud_img;

#define IMGBOX_SIZE	Size(150,150)
#define LOGINBOX_SIZE	Size(320,240)
#define TEXTPANEL_SIZE	Size(290,120)
#define IMGBOX_PADDING	10

#define TEXT_PASSWD_ERROR	L"密码输入错误，请重新输入！"
#define USER_AVATAR_PATH	"data/avatar/avatar.png"
#define DEFAULT_AVATAR_PATH	"data/avatar/default.png"

static LCUI_Graph user_avatar;

static void LeftMoveCloud( LCUI_Widget *widget )
{
	int w, x;
	x = _Widget_GetX(widget);
	w = Graph_GetSize(&cloud_img).w;
	if( x <= (0-w) ) {
		x = LCUIScreen_GetWidth();
	}
	Widget_Move( widget, Pos(x-1,0) );
}

static void LeftMoveAllCloud(void)
{
	int i;
	for(i=0; i<total_cloud; ++i) {
		LeftMoveCloud( wdg_cloud[i] );
	}
}

static void CreateCloud(void)
{
	int i, screen_w, img_w, x, avg_w;

	total_cloud = MAX_CLOUD;
	screen_w = LCUIScreen_GetWidth();
	img_w = Graph_GetSize( &cloud_img ).w;
	avg_w = (screen_w + img_w) / MAX_CLOUD;
	if( avg_w <= img_w ) {
		avg_w = img_w + 20;
		total_cloud = (screen_w + img_w)/avg_w;
		avg_w = (screen_w + img_w) / total_cloud;
	}
	x = 0;
	for(i=0; i<total_cloud; ++i) {
		wdg_cloud[i] = Widget_New(NULL);
		Widget_SetBackgroundTransparent( wdg_cloud[i], TRUE );
		Widget_SetBackgroundImage( wdg_cloud[i], &cloud_img );
		Widget_SetBackgroundLayout( wdg_cloud[i], LAYOUT_STRETCH );
		Widget_Resize( wdg_cloud[i], Graph_GetSize(&cloud_img) );
		Widget_Move( wdg_cloud[i], Pos(x,0) );
		Widget_SetZIndex( wdg_cloud[i], -100 );
		Widget_Show( wdg_cloud[i] );
		x += avg_w;
	}
	LCUITimer_Set( 50, LeftMoveAllCloud, TRUE );
}


static void BackgroundImage_Init(void)
{
	LCUI_Widget *widget;

	widget = RootWidget_GetSelf();
	Widget_SetBackgroundTransparent( widget, FALSE );
	Widget_SetBackgroundImage( widget, &bg_img );
	Widget_SetBackgroundLayout( widget, LAYOUT_STRETCH );
	CreateCloud();
}

/* 释放程序的资源 */
static void free_resource(void)
{
	Graph_Free( &user_avatar );
	Graph_Free( &bg_img );
	Graph_Free( &cloud_img );
}

/* 载入程序所需资源 */
static void load_resource(void)
{
	int ret;
	LCUI_Graph buff;
	LCUI_Size size;

	Graph_Init( &buff );
	Graph_Init( &user_avatar );
	ret = Graph_LoadImage( USER_AVATAR_PATH, &user_avatar );
	if( ret != 0 ) {
		Graph_LoadImage( DEFAULT_AVATAR_PATH, &user_avatar );
	}
	/* 如果头像有效，则缩放头像尺寸 */
	if( Graph_IsValid(&user_avatar) ) {
		size = IMGBOX_SIZE;
		size.w -= IMGBOX_PADDING;
		size.h -= IMGBOX_PADDING;
		Graph_Zoom( &user_avatar, &buff, FALSE, size );
		Graph_Free( &user_avatar );
		user_avatar = buff;
	}
	Graph_Init( &bg_img );
	Graph_Init( &cloud_img );
	Graph_LoadImage( "data/bg.png", &bg_img );
	Graph_LoadImage( "data/cloud.png", &cloud_img );
	LCUIApp_AtQuit( free_resource );
}

void create_imgbox( LCUI_Widget **imgbox )
{
	LCUI_Widget *widget;
	widget = Widget_New(NULL);
	Widget_SetStyleName(widget, "AvatarBox");
	Widget_SetBackgroundImage( widget, &user_avatar );
	Widget_Resize( widget, IMGBOX_SIZE );
	*imgbox = widget;
}

static void create_usernamelabel( LCUI_Widget **usernamelabel )
{
	LCUI_Widget *widget;
	LCUI_TextStyle style;

	widget = Widget_New("label");
	TextStyle_Init( &style );
	TextStyle_FontSize( &style, 20 );
	Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(0,15) );
	Label_TextStyle( widget, style );
	Label_TextW( widget, L"测试用户" );
	*usernamelabel = widget;
}

static void create_passwdbox( LCUI_Widget **passwdbox )
{
	LCUI_Widget *widget;
	LCUI_TextStyle style;

	widget = Widget_New("text_box");
	TextStyle_Init( &style );
	TextStyle_FontSize( &style, 20 );
	Widget_Resize( widget, Size(165,30) );
	Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(-10,50) );
	TextBox_Text_SetDefaultStyle( widget, style );
	TextBox_Text_SetPlaceHolderW( widget, NULL, L"请输入密码" );
	TextBox_Text_SetPasswordChar( widget, L'●' );
	*passwdbox = widget;
}

static void create_error_tip( LCUI_Widget **tip )
{
	LCUI_Widget *widget;
	LCUI_TextStyle style;
	widget = Widget_New("label");
	TextStyle_Init( &style );
	TextStyle_FontColor( &style, RGB(255,0,0) );
	TextStyle_FontSize( &style, 15 );
	Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(0,85) );
	Label_TextStyle( widget, style );
	*tip = widget;
}

static void verify_passwd( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	wchar_t buff[32];

	TextBox_GetText( passwdbox, buff, 32 );
	if( wcscmp( buff, L"123456" ) == 0 ) {
		LCUI_MainLoop_Quit(NULL);
	} else {
		Label_TextW( error_tip, TEXT_PASSWD_ERROR );
		Widget_Show( error_tip );
	}
}

static void create_loginbtn( LCUI_Widget **btn )
{
	LCUI_Widget *widget;
	widget = Widget_New("button");
	Widget_SetStyleID( widget, BUTTON_STYLE_FLAT );
	Widget_SetStyleName(widget,"LoginButton");
	Widget_SetAutoSize( widget, FALSE, 0 );
	Widget_Resize( widget, Size(30,30) );
	Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(93,50) );
	Widget_Event_Connect( widget, EVENT_CLICKED, verify_passwd );
	*btn = widget;
}

/* 显示登录界面 */
void ShowLoginPanel(void)
{
	LCUI_MainLoop *loop;

	load_resource();
	WidgetStyle_LoadFromFile("data/main.css");
	BackgroundImage_Init();

	create_imgbox( &imgbox );
	create_passwdbox( &passwdbox );
	create_usernamelabel( &usernamelabel );
	create_loginbtn( &btn );
	create_error_tip( &error_tip );

	Widget_SetZIndex( imgbox, 10 );
	Widget_Show( imgbox );
	Widget_Show( usernamelabel );
	Widget_Show( passwdbox );
	Widget_Show( btn );
}
