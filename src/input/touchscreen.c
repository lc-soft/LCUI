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

#include "config.h"
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_CURSOR_H
#include LC_WIDGET_H
#include LC_DISPLAY_H
#include LC_INPUT_H

#ifdef USE_TSLIB
#include <tslib.h> 
#endif

/*********** 触屏相关 **************/
typedef struct _LCUI_TS
{
	void *td;
	int status;
}
LCUI_TS;
/**********************************/

static LCUI_TS ts_data;
#ifdef USE_TSLIB
static LCUI_MouseEvent mouse_event;
#endif

static BOOL proc_touchscreen()
{
#ifdef USE_TSLIB
	struct ts_sample samp;
	int button, ret;
	static LCUI_Pos pos;
	
	if (ts_data.status != INSIDE) {
		return FALSE;
	}

	/* 开始获取触屏点击处的坐标 */ 
	ret = ts_read (ts_data.td, &samp, 1); 
	if (ret < 0) {
		perror ("ts_read");
		return FALSE;
	}

	if (ret != 1) {
		return FALSE;
	}
	pos.x = samp.x;
	pos.y = samp.y;
	
	if (pos.x > Get_Screen_Width ()) {
		pos.x = Get_Screen_Width ();
	}
	if (pos.y > Get_Screen_Height ()) {
		pos.y = Get_Screen_Height ();
	}
	pos.x = pos.x<0 ? 0:pos.x; 
	pos.y = pos.y<0 ? 0:pos.y; 
	/* 设定游标位置 */ 
	Set_Cursor_Pos ( pos );
	
	mouse_event.global_pos = pos;
	mouse_event.widget = Get_Cursor_Overlay_Widget(); 
	if( mouse_event.widget ) {
		mouse_event.pos = GlobalPos_ConvTo_RelativePos( mouse_event.widget, pos );
	} else { 
		mouse_event.pos = pos;
	}
	button = samp.pressure > 0 ? 1:0;
	Handle_Mouse_Event(button, &mouse_event); 
	//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure); 
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL Enable_TouchScreen_Input()
/* 功能：启用触屏输入处理 */
{ 
#ifdef USE_TSLIB
	char *tsdevice;
	char str[256];
	if (ts_data.status != INSIDE) {
		tsdevice = getenv("TSLIB_TSDEVICE");
		if( tsdevice != NULL ) {
			ts_data.td = ts_open(tsdevice, 0);
		} else {
			tsdevice = TS_DEV;
		}
		ts_data.td = ts_open (tsdevice, 0);
		if (!ts_data.td) {
			sprintf (str, "ts_open: %s", tsdevice);
			perror (str);
			ts_data.status = REMOVE;
			return FALSE;
		}

		if (ts_config (ts_data.td)) {
			perror ("ts_config");
			ts_data.status = REMOVE;
			return FALSE;
		}
		ts_data.status = INSIDE;
	}
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL Disable_TouchScreen_Input()
/* 功能：撤销触屏输入处理 */
{
#ifdef USE_TSLIB
	if(ts_data.status == INSIDE) {
		ts_close(ts_data.td);
		ts_data.status = REMOVE;
		return TRUE;
	}
	return FALSE;
#else
	return FALSE;
#endif
}

void TouchScreen_Init()
{
	ts_data.td = NULL;
	ts_data.status = REMOVE;
	LCUI_Dev_Add( Enable_TouchScreen_Input, proc_touchscreen,
			Disable_TouchScreen_Input );
}
