/* ***************************************************************************
 * catch_screen.c -- screenshot support
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * catch_screen.c -- 屏幕截图支持
 *
 * 版权所有 (C) 2013 归属于
 * 刘超
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

#ifdef USE_THIS_CODE
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/display.h>
#include <LCUI/thread.h>
#include <time.h>

static int need_break = FALSE;
static LCUI_Thread t = 0;
static LCUI_Rect target_area;

/* 在截取动画时，会用这个函数捕获屏幕内容 */
static void catch_screen( void *unused )
{
	LCUI_Graph graph;
	LCUI_Rect area;
	int i=0, tsec=0;
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	
	Graph_Init(&graph);
	area = target_area;
	
	while(!need_break) {
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		if(tsec != timeinfo->tm_sec)
			i=0;
			
		sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d-%02d.png",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, timeinfo->tm_hour, 
			timeinfo->tm_min, timeinfo->tm_sec, i++
		);
		tsec = timeinfo->tm_sec;
		
		LCUIScreen_CatchGraph( &graph, area );
		Graph_WritePNG( filename, &graph );
		LCUI_MSleep(35);
	}
	LCUIThread_Exit(NULL);
}

/* 录制屏幕指定区域的内容 */
LCUI_API int
LCUIScreen_StartRecord( LCUI_Rect area )
{
	if( t != 0 ) {
		return -1;
	}
	need_break = FALSE;
	target_area = area;
	return LCUIThread_Create(&t, catch_screen, NULL );
}

/* 结束录制 */
LCUI_API int
LCUIScreen_EndRecord( void )
{
	if( t == 0 ) {
		return -1;
	}
	need_break = TRUE;
	return LCUIThread_Join(t, NULL);
}
#endif
