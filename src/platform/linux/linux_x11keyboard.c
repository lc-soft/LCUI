/* ***************************************************************************
 * linux_x11keyboard.h -- keyboard support for linux xwindow.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
 * linux_x11keyboard.h -- linux 平台的键盘支持，基于 xwindow。
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
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

#include <stdio.h>
#include <LCUI_Build.h>
#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#include LCUI_KEYBOARD_H

static void OnKeyboardMessage( LCUI_Event ev, void *arg )
{
	XEvent *x_ev = arg;
	LCUI_SysEventRec sys_ev;
	switch( x_ev->type ) {
	case KeyPress:
		sys_ev.type = LCUI_KEYDOWN;
		break;
	case KeyRelease:
		sys_ev.type = LCUI_KEYUP;
		break;
	default: return;
	}
	sys_ev.key.code = x_ev->xkey.keycode;
	_DEBUG_MSG("keycode: %d\n", sys_ev.key.code);
	LCUI_TriggerEvent( &sys_ev, NULL );
}

void LCUI_InitLinuxX11Keyboard( void )
{
	LCUI_BindSysEvent( KeyPress, OnKeyboardMessage, NULL, NULL );
	LCUI_BindSysEvent( KeyRelease, OnKeyboardMessage, NULL, NULL );
}

void LCUI_ExitLinuxX11Keyboard( void )
{
	LCUI_UnbindSysEvent( KeyPress, OnKeyboardMessage );
	LCUI_UnbindSysEvent( KeyRelease, OnKeyboardMessage );
}

#endif
