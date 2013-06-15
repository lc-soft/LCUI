/* ***************************************************************************
 * messagebox.h -- MessageBox for LCUI
 * 
 * Copyright (C) 2012-2013 by
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
 * messagebox.h -- LCUI 的消息框
 *
 * 版权所有 (C) 2012-2013 归属于
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
#ifndef __LCUI_GUI_MESSAGEBOX_H__
#define __LCUI_GUI_MESSAGEBOX_H__

LCUI_BEGIN_HEADER

typedef enum {
	MB_ICON_NONE,
	MB_ICON_ERROR,
	MB_ICON_HELP,
	MB_ICON_INFO,
	MB_ICON_WARNING
} MB_ICON_TYPE;

typedef enum {
	MB_BTN_OK,
	MB_BTN_OKCANCEL,
	MB_BTN_ABORTRETRYIGNORE,
	MB_BTN_RETRYCANCEL,
	MB_BTN_YESNO,
	MB_BTN_YESNOCANCEL
} MB_BTN_TYPE;

typedef enum {
	MB_BTN_IS_OK,
	MB_BTN_IS_CANCEL,
	MB_BTN_IS_ABORT,
	MB_BTN_IS_RETRY,
	MB_BTN_IS_IGNORE,
	MB_BTN_IS_YES,
	MB_BTN_IS_NO,
	MB_BTN_IS_QUIT
} MB_BTN;

/* LCUI的消息框
 * 参数：
 * icon_type	: 指定消息框中显示的图标，可取以下值：
 * 	MB_ICON_NONE	无图标
 * 	MB_ICON_ERROR	“错误”图标
 * 	MB_ICON_HELP	”帮助“图标
 * 	MB_ICON_INFO	“信息”图标
 * 	MB_ICON_WARNING	“警告”图标
 * text		: 消息框内显示的消息内容
 * title	: 消息框的标题文本
 * button	: 指定消息框内显示的按钮，可取以下值：
 * 	MB_BTN_OK		: 一个“确定”按钮
 * 	MB_BTN_OKCANCEL		: “确定”和“取消”按钮
 * 	MB_BTN_ABORTRETRYIGNORE	: “终止”、"重试"和“忽略”按钮
 * 	MB_BTN_RETRYCANCEL	: "重试"和“取消”按钮
 * 	MB_BTN_YESNO		: "是"和“否”按钮
 * 	MB_BTN_YESNOCANCEL	: “是”、"否"和“取消”按钮
 * 返回值：
 * -1	: button参数的值异常
 * -2	: 无法获取消息框关闭时所点击的按钮ID
 * 正常则会返回按钮的ID，有以下值：
 * 	MB_BTN_IS_OK	: "确定"按钮	
 * 	MB_BTN_IS_CANCEL: "取消"按钮
 * 	MB_BTN_IS_ABORT	: "终止"按钮
 * 	MB_BTN_IS_RETRY	: "重试"按钮
 * 	MB_BTN_IS_IGNORE: "忽略"按钮
 * 	MB_BTN_IS_YES	: "是"按钮
 * 	MB_BTN_IS_NO	: "否"按钮
 * 	MB_BTN_IS_QUIT	: 窗口右上角的关闭按钮
 *  */
LCUI_API int
LCUI_MessageBoxW(	MB_ICON_TYPE icon_type,	const wchar_t *text, 
			const wchar_t *title,	MB_BTN_TYPE button );

/* UTF-8版的LCUI_MessageBox */
LCUI_API int
LCUI_MessageBox(	MB_ICON_TYPE icon_type, const char *text, 
			const char *title, MB_BTN_TYPE button );

/* ASCII版的LCUI_MessageBox */
LCUI_API int
LCUI_MessageBoxA(	MB_ICON_TYPE icon_type, const char *text, 
			const char *title, MB_BTN_TYPE button );

LCUI_END_HEADER

#endif
