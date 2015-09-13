/* ***************************************************************************
 * mouse.c -- mouse input support.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/display.h>

#ifdef LCUI_MOUSE_DRIVER_LINUX
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define MS_DEV	"/dev/input/mice"	/* 鼠标设备 */
#endif

enum LCUIMouseState {
	STATE_REMOVE,
	STATE_INSIDE
};

static struct LCUIMouseDeviceContext {
#ifdef LCUI_MOUSE_DRIVER_LINUX
	int fd;			/**< 鼠标设备标示符 */
	int state;		/**< 鼠标设备的状态 */
#elif defined(LCUI_BUILD_IN_WIN32)
	int x, y;
#endif
	int button_state[2];	/** 鼠标左右键的状态 */
} mouse = {
#ifdef LCUI_MOUSE_DRIVER_LINUX
	0, 0,
#elif defined(LCUI_BUILD_IN_WIN32)
	0, 0,
#endif
	{LCUIKEYSTATE_RELEASE, LCUIKEYSTATE_RELEASE}
};

static void OnMouseButtonEvent( LCUI_SystemEvent *e, void *arg )
{
	if( e->key_code < 1 || e->key_code > 2 ) {
		return;
	}
	_DEBUG_MSG("key_code: %d\n", e->key_code);
	switch( e->type ) {
	case LCUI_MOUSEDOWN:
		mouse.button_state[e->key_code] = LCUIKEYSTATE_PRESSED;
		break;
	case LCUI_MOUSEUP:
		mouse.button_state[e->key_code] = LCUIKEYSTATE_RELEASE;
	default:break;
	}
}

#ifdef LCUI_MOUSE_DRIVER_LINUX
static LCUI_BOOL MouseProc( void )
{
	static int  tmp;
	static char buf[6];
	static fd_set readfds;
	static struct timeval tv;

	if (mouse.state == STATE_REMOVE || mouse.fd < 0) {
		return FALSE;
	}
	/* 设定select等待I/o的最长时间 */
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO (&readfds);
	FD_SET (mouse.fd, &readfds);

	tmp = select (mouse.fd+1, &readfds, NULL, NULL, &tv);
	if (tmp == 0) {
		//printf("Time out!\n");
		return FALSE;
	}
	if ( !FD_ISSET (mouse.fd, &readfds) ) {
		return FALSE;
	}
	tmp = read (mouse.fd, buf, 6);
	if (tmp <= 0){
		if (tmp < 0) {
			mouse.state = STATE_REMOVE;
		}
		return FALSE;
	}

	return TRUE;
}
#else

static LCUI_BOOL MouseProc( void )
{
	POINT new_pos;
	LCUI_SystemEvent e;
	static LCUI_Pos old_pos = {0, 0};

	/* 如果是无缝模式则获取系统鼠标游标的坐标 */
	if( LCUIDisplay_GetMode() == LDM_SEAMLESS ) {
		GetCursorPos( &new_pos );
	} else {
		new_pos.x = mouse.x;
		new_pos.y = mouse.y;
	}
	e.type = LCUI_MOUSEMOVE;
	e.type_name = NULL;
	e.data = e.destroy_data = NULL;
	e.rel_x = new_pos.x - old_pos.x;
	e.rel_y = new_pos.y - old_pos.y;
	if( e.rel_x == 0 && e.rel_y == 0 ) {
		return FALSE;
	}
	old_pos.x = new_pos.x;
	old_pos.y = new_pos.y;
	DEBUG_MSG("x: %d, y: %d, rel_x: %d, rel_y: %d\n", new_pos.x, new_pos.y, e.rel_x, e.rel_y);
	LCUI_PostEvent( &e );
	return TRUE;
}

void LCUIMouse_SetPos( int x, int y )
{
	mouse.x = x;
	mouse.y = y;
}
#endif

/** 初始化鼠标输入处理 */
static LCUI_BOOL MouseInit(void)
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	char *msdev, err_str[64];

	if(mouse.state != STATE_REMOVE) {
		return FALSE;
	}
	msdev = getenv("LCUI_MOUSE_DEVICE");
	if( msdev == NULL ) {
		msdev = MS_DEV;
	}
	if ((mouse.fd = open (MS_DEV, O_RDONLY)) < 0) {
		sprintf(err_str, "[mouse] failed to open %s", msdev);
		perror(err_str);
		mouse.state = STATE_REMOVE;
		return FALSE;
	}
	mouse.state = STATE_INSIDE;
	printf("[mouse] open %s successfuly.\n", msdev);
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
	if( mouse.state != STATE_INSIDE ) {
		return FALSE;
	}
	close( mouse.fd );
	mouse.state = STATE_REMOVE;
#endif
	return TRUE;
}

/** 初始化鼠标输入模块 */
int LCUI_InitMouse( void )
{
	int ret;
#ifdef LCUI_MOUSE_DRIVER_LINUX
	mouse.fd = -1;
	mouse.state = STATE_REMOVE;	/* 鼠标为移除状态 */
#endif
	nobuff_printf("[mouse] set event ... ");
	ret = LCUI_AddEvent( "mousedown", LCUI_MOUSEDOWN );
	ret |= LCUI_AddEvent( "mouseup", LCUI_MOUSEUP );
	ret |= LCUI_AddEvent( "mousemove", LCUI_MOUSEMOVE );
	ret |= LCUI_BindEvent( "mousedown", OnMouseButtonEvent, NULL, NULL );
	ret |= LCUI_BindEvent( "mouseup", OnMouseButtonEvent, NULL, NULL );
	nobuff_printf(ret < 0 ? "failed\n":"ok\n");
	ret |= LCUIDevice_Add( MouseInit, MouseProc, MouseExit );
	return ret;
}
