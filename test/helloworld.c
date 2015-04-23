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
	Widget_PullStyle( w, WSS_BACKGROUND|WSS_SHADOW );
	w->style.background.color.value = 0xf6f6f6;
	w->style.background.image = image; 
	w->style.background.size.w.scale = 0.50;
	w->style.background.size.h.px = 200;
	w->style.background.size.w.type = SVT_SCALE;
	w->style.background.size.h.type = SVT_PX;
	w->style.background.size.using_value = FALSE;
	w->style.background.position.using_value = TRUE;
	w->style.background.position.value = SV_BOTTOM_CENTER;
	w->style.shadow.color = ARGB( 50, 0, 0, 0 );
	w->style.shadow.x = 1;
	w->style.shadow.y = 1;
	w->style.shadow.spread = 5;
	w->style.shadow.blur = 5;
	Widget_PushStyle( w, WSS_BACKGROUND|WSS_SHADOW );
	return LCUI_Main();
}
