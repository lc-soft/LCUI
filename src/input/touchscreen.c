/* ***************************************************************************
 * touchscreen.c -- touchscreen support
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
 * touchscreen.c -- 触屏支持
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
#include LC_CURSOR_H
#include LC_WIDGET_H
#include LC_DISPLAY_H
#include LC_INPUT_H

static void * Handle_TouchScreen_Input ()
/* 功能：处理触屏输入 */
{
	char *tsdevice;
	struct ts_sample samp;
	int button, x, y, ret;
	LCUI_MouseEvent event;
	
	char str[100];
	while (LCUI_Active()) {
		if (LCUI_Sys.ts.status != INSIDE) {
			tsdevice = getenv("TSLIB_TSDEVICE");
			if( tsdevice != NULL ) {
				LCUI_Sys.ts.td = ts_open(tsdevice, 0);
			} else {
				tsdevice = TS_DEV;
			}
			LCUI_Sys.ts.td = ts_open (tsdevice, 0);
			if (!LCUI_Sys.ts.td) { 
				sprintf (str, "ts_open: %s", tsdevice);
				perror (str);
				LCUI_Sys.ts.status = REMOVE;
				break;
			}

			if (ts_config (LCUI_Sys.ts.td)) {
				perror ("ts_config");
				LCUI_Sys.ts.status = REMOVE;
				break;
			}
			LCUI_Sys.ts.status = INSIDE;
		}

		/* 开始获取触屏点击处的坐标 */ 
		ret = ts_read (LCUI_Sys.ts.td, &samp, 1); 
		if (ret < 0) {
			perror ("ts_read");
			continue;
		}

		if (ret != 1) {
			continue;
		}
		x = samp.x;
		y = samp.y;
		
		if (x > Get_Screen_Width ()) {
			x = Get_Screen_Width ();
		}
		if (y > Get_Screen_Height ()) {
			y = Get_Screen_Height ();
		}
		if (x < 0) {
			x = 0;
		}
		if (y < 0) {
			y = 0;
		}
		/* 设定游标位置 */ 
		Set_Cursor_Pos (Pos(x, y));
		
		event.global_pos.x = x;
		event.global_pos.y = y;
		/* 获取当前鼠标指针覆盖到的部件的指针 */
		event.widget = Get_Cursor_Overlay_Widget();
		/* 如果有覆盖到的部件，就需要计算鼠标指针与部件的相对坐标 */
		if(event.widget != NULL) {
			event.pos.x = x - Get_Widget_Global_Pos(event.widget).x;
			event.pos.y = y - Get_Widget_Global_Pos(event.widget).y;
		} else {/* 否则，和全局坐标一样 */
			event.pos.x = x;
			event.pos.y = y;
		}
		if (samp.pressure > 0) {
			button = 1; 
		} else {
			button = 0; 
		}
			/* 处理鼠标事件 */
		Handle_Mouse_Event(button, &event); 
		//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
	}
	if(LCUI_Sys.ts.status == INSIDE) {
		ts_close(LCUI_Sys.ts.td); 
	}
	LCUI_Sys.ts.status = REMOVE;
	thread_exit (NULL);
}

int Enable_TouchScreen_Input()
/* 功能：启用触屏输入处理 */
{ 
	if(LCUI_Sys.ts.status == REMOVE) {
		return  thread_create ( &LCUI_Sys.ts.thread, NULL, 
					Handle_TouchScreen_Input, NULL ); 
	}
	return 0;
}

int Disable_TouchScreen_Input()
/* 功能：撤销触屏输入处理 */
{
	if(LCUI_Sys.ts.status == INSIDE) {
		return thread_cancel ( LCUI_Sys.ts.thread ); 
	}
	return 0;
}

