#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/gui/widget.h>
#include <LCUI/display.h>
#include <LCUI/font.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/gui/widget/sidebar.h>

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
	LCUI_Widget w, sbi, sb = (LCUI_Widget)arg;
	static int x = 0, y = 0, vx = 1, vy = 1;
	vx = x > 320 ? -1:vx;
	vx = x < 0 ? 1:vx;
	vy = y > 240 ? -1:vy;
	vy = y < 0 ? 1:vy;
	x += vx;
	y += vy;
	LCUI_PrintStyleLibrary();
	printf("sidebar visible: %d\n", sb->computed_style.visible);
	LinkedList_ForEach( sbi, 0, &sb->children ) {
		printf("item: %p, pos: %d,%d, size: %d,%d, visibale: %d\n", 
			sbi, sbi->x, sbi->y, sbi->width, sbi->height, sbi->computed_style.visible );
		LinkedList_ForEach( w, 0, &sbi->children ) {
			printf("child: %p, pos: %d,%d, size: %d,%d, visibale: %d\n", 
				w, w->x, w->y, w->width, w->height, w->computed_style.visible );
		}
	}
	return;
	//Widget_Move( arg, x, y );
	//sprintf(str, "fps: %d", LCUIDisplay_GetFPS());
	//Button_SetText(btn, str);
	//_DEBUG_MSG("fps: %d\n", LCUIDisplay_GetFPS());
}

const char *main_css = ToString(

debug-widget {
	background-color: rgba(255,255,255,0.8);
	background-size: 50% 200px;
	background-position: bottom center;
	border: 1px solid rgb(0,122,204);
	box-shadow: 2px 2px 0 8px rgba(0,122,204,0.8);
}

root {
	background-color: rgb(255,242,223);
	background-image: file("images/background-image.png");
	background-position: center;
	background-size: 75% 75%;
}

sidebar-item .text {
	font-family: Microsoft YaHei;
}

sidebar-item .icon {
	color: rgb(132,240,109);
	font-family: GLYPHICONS Halflings;
}

);

const wchar_t test_str[] = L"[size=12px]12px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=13px]13px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=14px]14px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=15px]15px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=16px]16px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=17px]17px, 0123456789, hello,world! are you OK? I like this![/size]\n\
[size=18px]18px, 0123456789, hello,world! are you OK? I like this![/size]";

int main( int argc, char **argv )
{
	LCUI_TextStyle icon_style = { 0 };
	LCUI_Widget w, root, sidebar;
	
#ifdef LCUI_BUILD_IN_WIN32
	InitConsoleWindow();//test_gen_font_code();return 0;
#endif
	_DEBUG_MSG("test\n");
	
	LCUI_Init();
	LCUIDisplay_SetMode( LDM_WINDOWED );
	LCUIDisplay_SetSize( 960, 540 );
	w = LCUIWidget_New("debug-widget");
	sidebar = LCUIWidget_New("sidebar");
	LCUIFont_LoadFile("../../../fonts/glyphicons-halflings-regular.ttf");
	SideBar_AppendItem( sidebar, L"item-dashboard", L"\xe021", L"Dashboard" );
	SideBar_AppendItem( sidebar, L"item-settings", L"\xe019", L"Settings" );
	Widget_Append( w, sidebar );
	Widget_Top( w );
	Widget_Show( w );
	Widget_Show( sidebar );
	Widget_Resize( w, 520, 240 );
	Widget_Move( w, 200, 200 );
	root = LCUIWidget_GetRoot();
	LCUI_ParseStyle( main_css );
	Widget_Update( root, TRUE );
	Widget_Update( w, TRUE );
	LCUITimer_Set( 2000, onTimer, sidebar, FALSE );
	return LCUI_Main();
}
