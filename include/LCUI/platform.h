/* ***************************************************************************
 * platform.c -- cross-platform driver interface
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
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
 * platform.c -- 各个平台的驱动支持接口
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_PLATFORM_H
#define LCUI_PLATFORM_H

#ifdef LCUI_BUILD_IN_WIN32
#define LCUI_CreateAppDriver LCUI_CreateWinAppDriver
#define LCUI_DestroyAppDriver LCUI_DestroyWinAppDriver
#define LCUI_PreInitApp LCUI_PreInitWinApp
#define LCUI_CreateDisplayDriver LCUI_CreateWinDisplay
#define LCUI_DestroyDisplayDriver LCUI_DestroyWinDisplay
#define LCUI_InitMouseDriver LCUI_InitWinMouse
#define LCUI_ExitMouseDriver LCUI_ExitWinMouse
#define LCUI_InitKeyboardDriver LCUI_InitWinKeyboard
#define LCUI_ExitKeyboardDriver LCUI_ExitWinKeyboard
#define LCUI_EVENTS_H	<LCUI/platform/windows/windows_events.h>
#define LCUI_MOUSE_H	<LCUI/platform/windows/windows_mouse.h>
#define LCUI_KEYBOARD_H	<LCUI/platform/windows/windows_keyboard.h>
#define LCUI_DISPLAY_H	<LCUI/platform/windows/windows_display.h>
#elif defined(LCUI_BUILD_IN_LINUX)
#define LCUI_CreateAppDriver LCUI_CreateLinuxAppDriver
#define LCUI_DestroyAppDriver LCUI_DestroyLinuxAppDriver
#define LCUI_PreInitApp LCUI_PreInitLinuxApp
#define LCUI_CreateDisplayDriver LCUI_CreateLinuxDisplayDriver
#define LCUI_DestroyDisplayDriver LCUI_DestroyLinuxDisplayDriver
#define LCUI_InitMouseDriver LCUI_InitLinuxMouse
#define LCUI_ExitMouseDriver LCUI_ExitLinuxMouse
#define LCUI_InitKeyboardDriver LCUI_InitLinuxKeyboard
#define LCUI_ExitKeyboardDriver LCUI_ExitLinuxKeyboard
#ifdef linux
#undef linux
#endif
#define LCUI_EVENTS_H	<LCUI/platform/linux/linux_events.h>
#define LCUI_MOUSE_H	<LCUI/platform/linux/linux_mouse.h>
#define LCUI_KEYBOARD_H	<LCUI/platform/linux/linux_keyboard.h>
#define LCUI_DISPLAY_H	<LCUI/platform/linux/linux_display.h>
#else
#error current platform is not supported.
#endif

#endif
