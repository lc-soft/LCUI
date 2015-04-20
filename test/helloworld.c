#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
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
	LCUI_Widget w;
	LCUI_Graph image;

	InitConsoleWindow();
	LCUI_Init(0, 0, LDM_SEAMLESS);
	w = Widget_New(NULL);
	Widget_Top( w );
	Widget_Show( w );
	Widget_Resize( w, 320, 240 );
	Widget_Move( w, 480, 480 );
	Widget_SetTitleW( w, L"测试" );
	Graph_Init( &image );
	_DEBUG_MSG( "load image, result: %d\n", Graph_LoadImage( "bg.png", &image ) );
	Widget_PullStyle( w, WSS_BACKGROUND );
	w->style.background.color = RGB(255,0,0);
	w->style.background.image = image;
	Widget_PushStyle( w, WSS_BACKGROUND );
	return LCUI_Main();
}
