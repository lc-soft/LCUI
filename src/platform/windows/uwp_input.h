/* ***************************************************************************
 * uwp_input.h -- UWP input support for LCUI
 *
 * Copyright (C) 2016-2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified,
 * and distributed under the terms of the GPLv2.
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
 * uwp_input.h -- LCUI 的 UWP 版输入支持，包括鼠标、键盘、触屏等的输入处理
 *
 * 版权所有 (C) 2016-2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是 LCUI 项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和
 * 发布。
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销
 * 性或特定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在 LICENSE 文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#pragma once

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

namespace LCUICore {

class InputDriver  {
public:
	InputDriver();
	void OnPointerPressed( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args );
	void OnPointerMoved( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args );
	void OnPointerReleased( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args );
	void OnPointerWheelChanged( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args );
	void OnKeyDown( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args );
	void OnKeyUp( Windows::UI::Core::CoreWindow^ sender,Windows::UI::Core:: KeyEventArgs^ args );
	void RegisterIME( void );
	void SelectIME( void );
private:
	int m_ime;
	struct {
		bool actived;
		bool leftButtonPressed;
		bool rightButtonPressed;
		Windows::Foundation::Point position;
	} m_mouse;
	struct {
		LinkedList points;
	} m_touch;
};

}
