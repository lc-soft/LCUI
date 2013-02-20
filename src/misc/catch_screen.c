/* ***************************************************************************
 * catch_screen.c -- screenshot support
 * 
 * Copyright (C) 2012 by
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
 * 版权所有 (C) 2012 归属于 
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


#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_DISPLAY_H
#include <time.h>

void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out)
/* 
 * 功能：通过内存中的图像数据，捕获将在屏幕上显示的图像
 * 说明：效率较低，因为需要进行合成。
 *  */
{
	LCUIScreen_GetRealGraph(area, out);
}

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out)
/* 
 * 功能：直接读取帧缓冲中的图像数据
 * 说明：效率较高，但捕获的图像有可能会有问题。
 * */
{
	LCUI_Rect cut_rect;
	unsigned char *dest;
	dest = LCUI_Sys.screen.fb_mem;	/* 指向帧缓冲 */
	int x, y, n, k, count;
	
	if( !LCUI_Active() ) {
		return;
	}
	/* 如果需要裁剪图形 */
	if ( Get_Cut_Area ( LCUIScreen_GetSize(), area,&cut_rect ) ){
		if(!Rect_Valid(cut_rect)) {
			return;
		}
			
		area.x += cut_rect.x;
		area.y += cut_rect.y;
		area.width = cut_rect.width;
		area.height = cut_rect.height;
	}
	
	Graph_Create(out, area.width, area.height);
	Graph_Lock (out); 
	/* 只能正常捕获32位显示器中显示的图形，有待完善 */
	for (n=0,y=0; y < area.height; ++y) {
		k = (area.y + y) * LCUI_Sys.screen.size.w + area.x;
		for (x = 0; x < area.width; ++x) {
			count = k + x;
			count = count << 2;  
			out->rgba[2][n] = dest[count];
			out->rgba[1][n] = dest[count + 1];
			out->rgba[0][n] = dest[count + 2];
			++n;
		}
	}
	Graph_Unlock (out);
}

static int need_break = FALSE;
static LCUI_Thread t = 0;
static LCUI_Rect target_area;
static void catch()
/* 在截取动画时，会用这个函数捕获屏幕内容 */
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
		
		Catch_Screen_Graph_By_FB( area, &graph );
		write_png( filename, &graph );
		LCUI_MSleep(35);
	}
	LCUIThread_Exit(NULL);
}

int start_record_screen( LCUI_Rect area )
/* 录制屏幕指定区域的内容 */
{
	if( t != 0 ) {
		return -1;
	}
	need_break = FALSE;
	target_area = area;
	return LCUIThread_Create(&t, catch, NULL );
}

int end_catch_screen()
/* 结束录制 */
{
	if( t == 0 ) {
		return -1;
	}
	need_break = TRUE;
	return LCUIThread_Join(t, NULL);
}
