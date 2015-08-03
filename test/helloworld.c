#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/widget.h>
#include <LCUI/display.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget/textview.h>

#ifdef LCUI_BUILD_IN_WIN32
#include <io.h>
#include <fcntl.h>

/* 在运行程序时会打开控制台，以查看打印的调试信息 */
static void InitConsoleWindow(void)
{
	int hCrt;
	FILE *hf;
	AllocConsole();
	hCrt=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT );
	hf=_fdopen( hCrt, "w" );
	*stdout=*hf;
	setvbuf (stdout, NULL, _IONBF, 0);
	// test code
	printf ("InitConsoleWindow OK!\n");
}

#endif

void onTimer( void *arg )
{
	static int x = 0, y = 0, vx = 1, vy = 1;
	vx = x > 320 ? -1:vx;
	vx = x < 0 ? 1:vx;
	vy = y > 240 ? -1:vy;
	vy = y < 0 ? 1:vy;
	x += vx;
	y += vy;
	Widget_Move( arg, x, y );
	DEBUG_MSG("fps: %d\n", LCUIDisplay_GetFPS());
}

const wchar_t test_str[] = L"[size=12px]12px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=13px]13px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=14px]14px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=15px]15px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=16px]16px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=17px]17px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=18px]18px, 0123456789, hello,world! are you OK? I like this![/size]";

int main( int argc, char **argv )
{
	int i;
	wchar_t str[64];
	LCUI_Widget w, root, text[4], btn;
	LCUI_Graph *desktop_image = Graph_New();
	
#ifdef LCUI_BUILD_IN_WIN32
	InitConsoleWindow();//test_gen_font_code();return 0;
#endif
	_DEBUG_MSG("test\n");
	
	LCUI_Init();
	LCUIDisplay_SetMode( LDM_WINDOWED );
	LCUIDisplay_SetSize( 960, 540 );
	w = LCUIWidget_New("debug-widget");
	for( i=0; i<4; ++i ) {
		text[i] = LCUIWidget_New("textview");
		swprintf( str, 60, L"textview-%d", i );
		TextView_SetTextW( text[i], str );
		Widget_Append( w, text[i] );
		Widget_Show( text[i] );
		Widget_Move( text[i], i*15, i*15 );
	}
	btn = LCUIWidget_New("button");
	Widget_Append( w, btn );
	Widget_Top( w );
	Widget_Show( w );
	Widget_Show( btn );
	Widget_Resize( w, 520, 240 );
	Widget_Resize( btn, 80, 40 );
	Widget_Move( w, 200, 200 );
	Widget_Move( btn, 80, 120 );
	Widget_SetTitleW( w, L"测试" );
	Graph_LoadImage( "images/background-image.png", desktop_image );
	root = LCUIWidget_GetRoot();
	
	SetStyle( root->style, key_background_color, RGB(255,242,223), color );
	SetStyle( root->style, key_background_image, desktop_image, image );
	//SetStyle( root->style, key_background_size, SV_COVER, style );
	SetStyle( root->style, key_background_position, SV_CENTER, style );
	SetStyle( root->style, key_background_size_width, 0.75, scale );
	SetStyle( root->style, key_background_size_height, 0.75, scale );
	Widget_Update( root, FALSE );

	SetStyle( w->style, key_background_color, ARGB(200,255,255,255), color );
	SetStyle( w->style, key_background_size_width, 0.50, scale ); 
	SetStyle( w->style, key_background_size_height, 200, px );
	SetStyle( w->style, key_background_position, SV_BOTTOM_CENTER, style );
	SetStyle( w->style, key_box_shadow_color, ARGB(200,0,122,204), color );
	SetStyle( w->style, key_box_shadow_x, 2, px );
	SetStyle( w->style, key_box_shadow_y, 2, px );
	SetStyle( w->style, key_box_shadow_spread, 0, px );
	SetStyle( w->style, key_box_shadow_blur, 8, px );
	SetStyle( w->style, key_border_top_width, 1, px );
	SetStyle( w->style, key_border_right_width, 1, px );
	SetStyle( w->style, key_border_bottom_width, 1, px );
	SetStyle( w->style, key_border_left_width, 1, px );
	SetStyle( w->style, key_border_top_color, RGB(0,122,204), color );
	SetStyle( w->style, key_border_right_color, RGB(0,122,204), color );
	SetStyle( w->style, key_border_bottom_color, RGB(0,122,204), color );
	SetStyle( w->style, key_border_left_color, RGB(0,122,204), color );
	Widget_Update( w, FALSE );
	//LCUITimer_Set( 10, onTimer, btn, TRUE );
	return LCUI_Main();
}
