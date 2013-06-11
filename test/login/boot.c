#define I_NEED_WINMAIN
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_FONT_H

#include "login.h"
#include "desktop.h"

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

int main(int argc, char **argv)
{
#ifdef LCUI_BUILD_IN_WIN32
	//InitConsoleWindow();
#endif
	LCUI_Init(800,600,LCUI_INIT_MODE_WINDOW);
	//LCUI_Init(0,0,0);
	ShowLoginPanel();
	return LCUI_Main();
}
