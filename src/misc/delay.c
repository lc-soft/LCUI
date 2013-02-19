#include <LCUI_Build.h>
#include LC_LCUI_H

#ifdef LCUI_BUILD_IN_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
/* 秒级延时 */
void LCUI_Sleep( unsigned int s )
{
#ifdef LCUI_BUILD_IN_WIN32
	Sleep(s*1000);
#else
	sleep(s);
#endif
}

/* 毫秒级延时 */
void LCUI_MSleep( unsigned int ms )
{
#ifdef LCUI_BUILD_IN_WIN32
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}
