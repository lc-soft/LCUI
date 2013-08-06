/** ******************************************************************************
 * @file	mouse.c
 * @brief	mouse input support.
 * @author	Liu Chao <lc-soft@live.cn>
 * @warning
 * Copyright (C) 2012-2013 by							\n
 * Liu Chao									\n
 * 										\n
 * This file is part of the LCUI project, and may only be used, modified, and	\n
 * distributed under the terms of the GPLv2.					\n
 * 										\n
 * (GPLv2 is abbreviation of GNU General Public License Version 2)		\n
 * 										\n
 * By continuing to use, modify, or distribute this file you indicate that you	\n
 * have read the license and understand and accept it fully.			\n
 *  										\n
 * The LCUI project is distributed in the hope that it will be useful, but 	\n
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 	\n
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.	\n
 * 										\n
 * You should have received a copy of the GPLv2 along with this file. It is 	\n
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.	\n
 * ******************************************************************************/
 
/** ******************************************************************************
 * @file	mouse.c
 * @brief	鼠标输入支持。
 * @author	刘超 <lc-soft@live.cn>
 * @warning
 * 版权所有 (C) 2012-2013 归属于						\n
 * 刘超										\n
 * 										\n
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。	\n
 * 										\n
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)				\n
 * 										\n
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。	\n
 * 										\n
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特定\n
 用途的隐含担保，详情请参照GPLv2许可协议。					\n
 * 										\n
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果	\n
 * 没有，请查看：<http://www.gnu.org/licenses/>. 				\n
 * ******************************************************************************/

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_CURSOR_H
#include LC_DISPLAY_H
#include LC_INPUT_H

#ifdef LCUI_MOUSE_DRIVER_LINUX
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef LCUI_MOUSE_DRIVER_LINUX
static int mouse_fd;	/**< 鼠标设备标示符 */
static int mouse_state;	/**< 鼠标设备的状态 */
#endif

/** 鼠标左右键的状态 */
static int button_state[2]={
		LCUIKEYSTATE_RELEASE,
		LCUIKEYSTATE_RELEASE
};

/** 按下指定鼠标按键 */
LCUI_API int LCUIMouse_ButtonDown( int key_code )
{
	LCUI_Event event;
	LCUI_Pos pos;

	if( button_state[key_code] == LCUIKEYSTATE_PRESSED ) {
		return -1;
	}
	button_state[key_code] = LCUIKEYSTATE_PRESSED;
	event.type = LCUI_MOUSEBUTTONDOWN;
	pos = LCUICursor_GetPos();
	event.button.x = pos.x;
	event.button.y = pos.y;
	event.button.button = key_code;
	event.button.state = LCUIKEYSTATE_PRESSED;
	LCUI_PushEvent( &event );
	return 0;
}

/** 释放指定鼠标按键 */
LCUI_API int LCUIMouse_ButtonUp( int key_code )
{
	LCUI_Event event;
	LCUI_Pos pos;
	
	if( button_state[key_code] == LCUIKEYSTATE_RELEASE ) {
		return -1;
	}
	button_state[key_code] = LCUIKEYSTATE_RELEASE;
	event.type = LCUI_MOUSEBUTTONUP;
	pos = LCUICursor_GetPos();
	event.button.x = pos.x;
	event.button.y = pos.y;
	event.button.button = key_code;
	event.button.state = LCUIKEYSTATE_RELEASE;
	LCUI_PushEvent( &event );
	return 0;
}

/** 添加鼠标移动事件 */
LCUI_API void LCUI_PushMouseMotionEvent( LCUI_Pos new_pos )
{
	LCUI_Event event;
	static LCUI_Pos old_pos = {0,0};

	if( new_pos.x == old_pos.x
	 && new_pos.y == old_pos.y ) {
		return;
	}

	event.type = LCUI_MOUSEMOTION;
	event.motion.x = new_pos.x;
	event.motion.y = new_pos.y;
	event.motion.xrel = new_pos.x - old_pos.x;
	event.motion.yrel = new_pos.y - old_pos.y;
	if( button_state[0] == LCUIKEYSTATE_PRESSED
	 || button_state[1] == LCUIKEYSTATE_PRESSED ) {
		event.motion.state = LCUIKEYSTATE_PRESSED;
	} else {
		event.motion.state = LCUIKEYSTATE_RELEASE;
	}
	LCUI_PushEvent( &event );
	old_pos = new_pos;
}

#ifdef LCUI_MOUSE_DRIVER_LINUX
static LCUI_BOOL proc_mouse( void )
{
	static int  tmp;
	static char buf[6];
	static fd_set readfds;
	static struct timeval tv;
	static LCUI_Pos pos;

	if (mouse_data.state == REMOVE || mouse_data.fd < 0) {
		return FALSE;
	}
	/* 设定select等待I/o的最长时间 */
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO (&readfds);
	FD_SET (mouse_data.fd, &readfds);

	tmp = select (mouse_data.fd+1, &readfds, NULL, NULL, &tv);
	if (tmp == 0) {
		//printf("Time out!\n");
		return FALSE;
	}
	if ( !FD_ISSET (mouse_data.fd, &readfds) ) {
		return FALSE;
	}
	tmp = read (mouse_data.fd, buf, 6);
	if (tmp <= 0){
		if (tmp < 0) {
			mouse_data.state = REMOVE;
		}
		return FALSE;
	}

	pos = LCUICursor_GetNewPos();
	pos.x += buf[1];
	pos.y -= buf[2];
	if( pos.x > LCUIScreen_GetWidth() ) {
		pos.x = LCUIScreen_GetWidth();
	}
	if( pos.y > LCUIScreen_GetHeight() ) {
		pos.y = LCUIScreen_GetHeight();
	}
	pos.x = pos.x<0 ? 0:pos.x;
	pos.y = pos.y<0 ? 0:pos.y;
	/* 应用鼠标游标的位置变更 */
	LCUICursor_SetPos( pos );
	switch ( buf[0]&0x07 ) {
	    case 1:		/* 鼠标左键被按下 */
		LCUIMouse_ButtonDown( new_pos, LCUIKEY_LEFTBUTTON );
		LCUIMouse_ButtonUp( new_pos, LCUIKEY_RIGHTBUTTON );
		break;
	    case 2:		/* 鼠标右键被按下 */
		LCUIMouse_ButtonDown( new_pos, LCUIKEY_RIGHTBUTTON );
		LCUIMouse_ButtonUp( new_pos, LCUIKEY_LEFTBUTTON );
		break;
	    case 3:		/* 鼠标左右键被按下 */
		LCUIMouse_ButtonDown( new_pos, LCUIKEY_RIGHTBUTTON );
		LCUIMouse_ButtonDown( new_pos, LCUIKEY_LEFTBUTTON );
		break;
	    default:		/* 默认是释放的 */
		LCUIMouse_ButtonUp( new_pos, LCUIKEY_RIGHTBUTTON );
		LCUIMouse_ButtonUp( new_pos, LCUIKEY_LEFTBUTTON );
		break;
	}
	LCUI_PushMouseMotionEvent( new_pos );
	return TRUE;
}
#else

static LCUI_BOOL proc_mouse( void )
{
	LCUI_Pos pos;
	POINT new_pos;

	/* 获取鼠标坐标 */
	GetCursorPos( &new_pos );
	/* 转换成相对于窗口客户区的坐标 */
	ScreenToClient( Win32_GetSelfHWND(), &new_pos );
	if (new_pos.x > LCUIScreen_GetWidth ()) {
		new_pos.x = LCUIScreen_GetWidth ();
	}
	if (new_pos.y > LCUIScreen_GetHeight ()) {
		new_pos.y = LCUIScreen_GetHeight ();
	}
	new_pos.x = new_pos.x<0 ? 0:new_pos.x;
	new_pos.y = new_pos.y<0 ? 0:new_pos.y;
	pos = LCUICursor_GetPos();
	if( pos.x == new_pos.x && pos.y == new_pos.y ) {
		return FALSE;
	}
	pos.x = new_pos.x;
	pos.y = new_pos.y;
	/* 更新鼠标游标的位置 */
	LCUICursor_SetPos( pos );
	LCUI_PushMouseMotionEvent( pos );
	DEBUG_MSG("new pos: %d,%d\n", pos.x, pos.y);
	return TRUE;
}
#endif

/** 启用鼠标输入处理 */
LCUI_API LCUI_BOOL Enable_Mouse_Input(void)
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	char *msdev;

	if(mouse_data.state != REMOVE) {
		return FALSE;
	}
	msdev = getenv("LCUI_MOUSE_DEVICE");
	if( msdev == NULL ) {
		msdev = MS_DEV;
	}
	if ((mouse_data.fd = open (MS_DEV, O_RDONLY)) < 0) {
		printf("failed to open %s.\n", msdev );
		perror(NULL);
		mouse_data.state = REMOVE;
		return FALSE;
	}
	mouse_data.state = INSIDE;
	printf("open %s successfuly.\n", msdev);
#else
	/* 隐藏windows的鼠标游标 */
	ShowCursor( FALSE );
#endif
	return TRUE;
}

/** 禁用鼠标输入处理 */
LCUI_API LCUI_BOOL Disable_Mouse_Input(void)
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	if(mouse_data.state != INSIDE) {
		return FALSE;
	}
	LCUICursor_Hide();
	close (mouse_data.fd);
	mouse_data.state = REMOVE;
#endif
	return TRUE;
}

/** 初始化鼠标输入模块 */
LCUI_API int LCUIModule_Mouse_Init( void )
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	mouse_data.fd = -1;
	mouse_data.state = REMOVE;	/* 鼠标为移除状态 */
	/* 启用鼠标输入处理 */
	nobuff_printf("enable mouse input: ");
#endif
	/* 注册鼠标设备 */
	return LCUIDevice_Add( Enable_Mouse_Input,
			proc_mouse, Disable_Mouse_Input );
}
