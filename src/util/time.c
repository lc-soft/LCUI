/* ***************************************************************************
* time.c -- The time operation set.
*
* Copyright (C) 2015-2017 by Liu Chao <lc-soft@live.cn>
*
* This file is part of the LCUI project, and may only be used, modified, and
* distributed under the terms of the GPLv2.
*
* (GPLv2 is abbreviation of GNU General Public License Version 2)
*
* By continuing to use, modify, or distribute this file you indicate that you
* have read the license and understand and accept it fully.
*
* The LCUI project is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
*
* You should have received a copy of the GPLv2 along with this file. It is
* usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
* ****************************************************************************/

/* ****************************************************************************
* time.c -- 时间相关函数集
*
* 版权所有 (C) 2015-2017 归属于 刘超 <lc-soft@live.cn>
*
* 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
*
* (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
*
* 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
*
* LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
* 定用途的隐含担保，详情请参照GPLv2许可协议。
*
* 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
* 没有，请查看：<http://www.gnu.org/licenses/>.
* ****************************************************************************/

#include <time.h>
#include <stdint.h>
#include <LCUI_Build.h>
#include <LCUI/util/time.h>

#define TIME_WRAP_VALUE (~(int64_t)0)

#ifdef LCUI_BUILD_IN_WIN32
#include <Windows.h>

static int hires_timer_available = 0;	/**< 标志，指示高精度计数器是否可用 */
static LONGLONG hires_ticks_per_second;	/**< 高精度计数器每秒的滴答数 */

void LCUITime_Init( void )
{
	LARGE_INTEGER hires;
	if( QueryPerformanceFrequency( &hires ) ) {
		hires_timer_available = 1;
		hires_ticks_per_second = hires.QuadPart;
	}
}

int64_t LCUI_GetTime( void )
{
	int64_t time;
	LARGE_INTEGER hires_now;
	FILETIME *ft = (FILETIME*)&time;
	if( hires_timer_available ) {
		QueryPerformanceCounter( &hires_now );
		time = hires_now.QuadPart * 1000;
		return time / hires_ticks_per_second;
	}
	GetSystemTimeAsFileTime( ft );
	return time / 1000 - 11644473600000;
}

#elif defined LCUI_BUILD_IN_LINUX
#include <unistd.h>
#include <sys/time.h>


void LCUITime_Init( void )
{
	return;
}

int64_t LCUI_GetTime( void )
{
	int64_t t;
	struct timeval tv;

	gettimeofday( &tv, NULL );
	t = tv.tv_sec*1000 + tv.tv_usec/1000;
	return t;
}

#endif

int64_t LCUI_GetTimeDelta( int64_t start )
{
	int64_t now = LCUI_GetTime();
	if( now < start ) {
		return (TIME_WRAP_VALUE - start) + now;
	}
	return now - start;
}

void LCUI_Sleep( unsigned int s )
{
#ifdef LCUI_BUILD_IN_WIN32
	Sleep( s * 1000 );
#else
	sleep( s );
#endif
}

void LCUI_MSleep( unsigned int ms )
{
#ifdef LCUI_BUILD_IN_WIN32
	Sleep( ms );
#else
	usleep( ms * 1000 );
#endif
}
