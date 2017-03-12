/* ***************************************************************************
 * ime.h -- Input Method Editor/Engine
 *
 * Copyright (C) 2016-2017 by Liu Chao <lc-soft@live.cn>
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
 * ime.h -- 输入法管理器
 *
 * 版权所有 (C) 2016-2017 归属于 刘超 <lc-soft@live.cn>
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

#ifndef LCUI_IME_H
#define LCUI_IME_H

LCUI_BEGIN_HEADER

typedef struct LCUI_IMEHandlerRec_ {
	LCUI_BOOL (*prockey)(int, int);
	void (*totext)(int);
	LCUI_BOOL (*open)(void);
	LCUI_BOOL (*close)(void);
} LCUI_IMEHandlerRec, *LCUI_IMEHandler;

/** 注册一个输入法 */
LCUI_API int LCUIIME_Register( const char *ime_name, LCUI_IMEHandler handler );

/** 选定输入法 */
LCUI_API LCUI_BOOL LCUIIME_Select( int ime_id );

LCUI_API LCUI_BOOL LCUIIME_SelectByName( const char *name );

/** 检测键值是否为字符键值 */
LCUI_API LCUI_BOOL LCUIIME_CheckCharKey( int key );

/** 切换至下一个输入法 */
LCUI_API void LCUIIME_Switch( void );

/** 检测输入法是否要处理按键事件 */
LCUI_API LCUI_BOOL LCUIIME_ProcessKey( LCUI_SysEvent e );

/** 提交输入法输入的内容至目标 */
LCUI_API int LCUIIME_Commit( const wchar_t *str, int length );

/* 初始化LCUI输入法模块 */
LCUI_API void LCUI_InitIME( void );

/* 停用LCUI输入法模块 */
LCUI_API void LCUI_ExitIME( void );

#ifdef LCUI_BUILD_IN_WIN32
int LCUI_RegisterWin32IME( void );
#else
int LCUI_RegisterLinuxIME( void );
#endif

LCUI_END_HEADER

#endif
