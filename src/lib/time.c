
#include <time.h>
#include <stdint.h>
#include <LCUI_Build.h>
#include <LCUI/lib/time.h>

#define TIME_WRAP_VALUE (~(int64_t)0)

#ifdef LCUI_BUILD_IN_WIN32
#include <Windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

static int hires_timer_available;	/**< 标志，指示高精度计数器是否可用 */
static double hires_ticks_per_second;	/**< 高精度计数器每秒的滴答数 */

void LCUI_StartTicks( void )
{
	LARGE_INTEGER hires;
	if( QueryPerformanceFrequency(&hires) ) {
		hires_timer_available = 1;
		hires_ticks_per_second = hires.QuadPart;
	} else {
		hires_timer_available = 0;
		timeBeginPeriod(1);
	}
}

int64_t LCUI_GetTickCount( void )
{
	LARGE_INTEGER hires_now;
	if (hires_timer_available) {
		QueryPerformanceCounter(&hires_now);
		hires_now.QuadPart *= 1000;
		hires_now.QuadPart /= hires_ticks_per_second;
		return (int64_t)hires_now.QuadPart;
	}
	return (int64_t)timeGetTime();
}

#elif defined LCUI_BUILD_IN_LINUX
#include <sys/time.h>

void LCUI_StartTicks( void )
{
	return;
}

int64_t LCUI_GetTickCount( void )
{
	int64_t t;
	struct timeval tv;

	gettimeofday( &tv, NULL );
	t = tv.tv_sec*1000 + tv.tv_usec/1000;
	return t;
}

#endif

int64_t LCUI_GetTicks( int64_t start_ticks )
{
	int64_t now_ticks;

	now_ticks = LCUI_GetTickCount();
	if ( now_ticks < start_ticks ) {
		return (TIME_WRAP_VALUE-start_ticks) + now_ticks;
	}
	return now_ticks - start_ticks;
}
