#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/widget.h>
#include <LCUI/surface.h>
#include <LCUI/display.h>

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

void onTimer( void *arg )
{
	_DEBUG_MSG("tip\n");
	Surface_Show( arg );
}

int main( int argc, char **argv )
{
	LCUI_Surface s[4];
	LCUI_Widget widget[4];

	InitConsoleWindow();
	LCUI_Init(800,600,0);
	
	s[0] = Surface_New();
	s[1] = Surface_New();
	s[2] = Surface_New();
	s[3] = Surface_New();
	Surface_Resize( s[0], 320, 240 );
	Surface_Resize( s[1], 640, 480 );
	Surface_Resize( s[2], 160, 120 );
	Surface_Resize( s[3], 80, 60 );
	Surface_Show( s[0] );
	//LCUI_MSleep(500);
	Surface_Show( s[1] );
	Surface_Show( s[3] );
	Surface_Move( s[0], 320, 240 );
	Surface_Move( s[1], 640, 380 );
	Surface_Move( s[2], 800, 380 );
	Surface_Move( s[3], 1080, 600 );
	Surface_SetCaptionW( s[0], L"第一个窗口" );
	Surface_SetCaptionW( s[1], L"第二个窗口" );
	Surface_SetCaptionW( s[2], L"第三个窗口" );
	Surface_SetCaptionW( s[3], L"第四个窗口" );
	LCUITimer_Set( 1000, onTimer, s[2], FALSE );
	widget[0] = Widget_New(NULL);
	widget[1] = Widget_New(NULL);
	widget[2] = Widget_New(NULL);
	widget[3] = Widget_New(NULL);
	LCUIDisplay_SetMode( LDM_SEAMLESS );
	return LCUI_Main();
}
