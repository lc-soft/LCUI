/* ***************************************************************************
 * display.h -- APIs of graphics display..
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
 * ****************************************************************************/

/* ****************************************************************************
 * display.h -- 与图形显示相关的接口。
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
 * ****************************************************************************/

#ifndef __LCUI_DISPLAY_H__
#define __LCUI_DISPLAY_H__

LCUI_BEGIN_HEADER

/** 图形显示模式 */
enum LCUIDisplayMode {
	LDM_DEFAULT,
	LDM_WINDOWED,		/**< 窗口化 */
	LDM_SEAMLESS,		/**< 与系统GUI无缝结合 */
	LDM_FULLSCREEN		/**< 全屏模式 */
};

/** 一秒内的最大画面帧数 */
#define MAX_FRAMES_PER_SEC	100

#ifdef LCUI_BUILD_IN_WIN32
#define WIN32_WINDOW_WIDTH 800
#define WIN32_WINDOW_HEIGHT 600

HWND Win32_GetSelfHWND( void );
void Win32_SetSelfHWND( HWND hwnd );
#endif

/* 设置呈现模式 */
LCUI_API int LCUIDisplay_SetMode( int mode );

/** 获取屏幕显示模式 */
LCUI_API int LCUIDisplay_GetMode( void );

/** 获取屏幕宽度 */
LCUI_API int LCUIDisplay_GetWidth( void );

/** 获取屏幕高度 */
LCUI_API int LCUIDisplay_GetHeight( void );

/** 获取当前的屏幕内容每秒更新的帧数 */
LCUI_API int LCUIDisplay_GetFPS(void);

/** 初始化图形输出模块 */
int LCUIModule_Video_Init( void );

/** 停用图形输出模块 */
int LCUIModule_Video_End( void );

LCUI_END_HEADER

#endif
