/* ***************************************************************************
 * mouse.c -- mouse input support.
 * 
 * Copyright (C) 2012-2014 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * mouse.c -- 鼠标输入支持。
 *
 * 版权所有 (C) 2012-2014 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/cursor.h>
#include <LCUI/display.h>

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

/** 投递鼠标按键按下事件 */
int LCUI_PostMouseDownEvent( int key_code )
{
	LCUI_Pos pos;
	LCUI_SystemEvent event;

	if( button_state[key_code] == LCUIKEYSTATE_PRESSED ) {
		return -1;
	}
	button_state[key_code] = LCUIKEYSTATE_PRESSED;
	LCUICursor_GetPos( &pos );
	event.type = LCUI_MOUSEDOWN;
	event.type_name = NULL;
	event.x = pos.x;
	event.y = pos.y;
	event.which = key_code;
	LCUI_PostEvent( &event );
	return 0;
}

/** 投递鼠标按键释放事件 */
int LCUI_PostMouseUpEvent( int key_code )
{
	LCUI_Pos pos;
	LCUI_SystemEvent event;
	
	if( button_state[key_code] == LCUIKEYSTATE_RELEASE ) {
		return -1;
	}
	button_state[key_code] = LCUIKEYSTATE_RELEASE;
	LCUICursor_GetPos( &pos );
	event.type = LCUI_MOUSEUP;
	event.type_name = NULL;
	event.x = pos.x;
	event.y = pos.y;
	event.which = key_code;
	LCUI_PostEvent( &event );
	return 0;
}

/** 投递鼠标移动事件 */
void LCUI_PostMouseMoveEvent( LCUI_Pos new_pos )
{
	LCUI_SystemEvent event;
	static LCUI_Pos old_pos = {0,0};

	if( new_pos.x == old_pos.x && new_pos.y == old_pos.y ) {
		return;
	}
	event.type = LCUI_MOUSEMOVE;
	event.type_name = NULL;
	event.x = new_pos.x;
	event.y = new_pos.y;
	LCUI_PostEvent( &event );
	old_pos = new_pos;
}

#ifdef LCUI_MOUSE_DRIVER_LINUX
static LCUI_BOOL MouseProc( void )
{
	static int  tmp;
	static char buf[6];
	static fd_set readfds;
	static struct timeval tv;
	static LCUI_Pos pos;

	if (mouse_state == REMOVE || mouse_fd < 0) {
		return FALSE;
	}
	/* 设定select等待I/o的最长时间 */
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO (&readfds);
	FD_SET (mouse_fd, &readfds);

	tmp = select (mouse_fd+1, &readfds, NULL, NULL, &tv);
	if (tmp == 0) {
		//printf("Time out!\n");
		return FALSE;
	}
	if ( !FD_ISSET (mouse_fd, &readfds) ) {
		return FALSE;
	}
	tmp = read (mouse_fd, buf, 6);
	if (tmp <= 0){
		if (tmp < 0) {
			mouse_state = REMOVE;
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
	LCUI_PostMouseMoveEvent( pos );
	switch ( buf[0]&0x07 ) {
	    case 1:		/* 鼠标左键被按下 */
		LCUI_PostMouseDownEvent( LCUIKEY_LEFTBUTTON );
		LCUI_PostMouseUpEvent( LCUIKEY_RIGHTBUTTON );
		break;
	    case 2:		/* 鼠标右键被按下 */
		LCUI_PostMouseDownEvent( LCUIKEY_RIGHTBUTTON );
		LCUI_PostMouseUpEvent( LCUIKEY_LEFTBUTTON );
		break;
	    case 3:		/* 鼠标左右键被按下 */
		LCUI_PostMouseDownEvent( LCUIKEY_RIGHTBUTTON );
		LCUI_PostMouseDownEvent( LCUIKEY_LEFTBUTTON );
		break;
	    default:		/* 默认是释放的 */
		LCUI_PostMouseUpEvent( LCUIKEY_RIGHTBUTTON );
		LCUI_PostMouseUpEvent( LCUIKEY_LEFTBUTTON );
		break;
	}
	return TRUE;
}
#else

static LCUI_BOOL MouseProc( void )
{
	LCUI_Pos pos;
	POINT new_pos;

	/* 获取鼠标坐标 */
	GetCursorPos( &new_pos );
	/* 转换成相对于窗口客户区的坐标 */
	ScreenToClient( Win32_GetSelfHWND(), &new_pos );
	if (new_pos.x > LCUIScreen_GetWidth() ) {
		new_pos.x = LCUIScreen_GetWidth();
	}
	if (new_pos.y > LCUIScreen_GetHeight() ) {
		new_pos.y = LCUIScreen_GetHeight();
	}
	new_pos.x = new_pos.x<0 ? 0:new_pos.x;
	new_pos.y = new_pos.y<0 ? 0:new_pos.y;
	LCUICursor_GetPos( &pos );
	if( pos.x == new_pos.x && pos.y == new_pos.y ) {
		return FALSE;
	}
	pos.x = new_pos.x;
	pos.y = new_pos.y;
	/* 更新鼠标游标的位置 */
	LCUICursor_SetPos( pos );
	LCUI_PostMouseMoveEvent( pos );
	return TRUE;
}
#endif

/** 初始化鼠标输入处理 */
static LCUI_BOOL MouseInit(void)
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	char *msdev;

	if(mouse_state != REMOVE) {
		return FALSE;
	}
	msdev = getenv("LCUI_MOUSE_DEVICE");
	if( msdev == NULL ) {
		msdev = MS_DEV;
	}
	if ((mouse_fd = open (MS_DEV, O_RDONLY)) < 0) {
		printf("failed to open %s.\n", msdev );
		perror(NULL);
		mouse_state = REMOVE;
		return FALSE;
	}
	mouse_state = INSIDE;
	printf("open %s successfuly.\n", msdev);
#else
	/* 隐藏windows的鼠标游标 */
	ShowCursor( FALSE );
#endif
	return TRUE;
}

/** 退出鼠标输入处理 */
static LCUI_BOOL MouseExit(void)
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	if(mouse_state != INSIDE) {
		return FALSE;
	}
	LCUICursor_Hide();
	close( mouse_fd );
	mouse_state = REMOVE;
#endif
	return TRUE;
}

/** 初始化鼠标输入模块 */
int LCUIModule_Mouse_Init( void )
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	mouse_fd = -1;
	mouse_state = REMOVE;	/* 鼠标为移除状态 */
	/* 启用鼠标输入处理 */
	nobuff_printf("enable mouse input: ");
#endif
	/* 注册鼠标设备 */
	return LCUIDevice_Add( MouseInit, MouseProc, MouseExit );
}
