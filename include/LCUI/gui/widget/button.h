/* ***************************************************************************
 * button.h -- LCUI‘s Button widget
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
 * button.h -- LCUI 的按钮部件
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

#ifndef __LCUI_BUTTON_H__
#define __LCUI_BUTTON_H__

LCUI_BEGIN_HEADER

typedef struct _LCUI_Button LCUI_Button;

/***********************按钮**************************/
struct _LCUI_Button
{
	/* 当风格为Custom_Style(自定义)时，将根据按钮的状态使用这些图形 */
	LCUI_Graph	btn_normal;	/* 普通状态 */
	LCUI_Graph	btn_over;	/* 被鼠标覆盖 */
	LCUI_Graph	btn_down;	/* 按下状态 */
	LCUI_Graph	btn_focus;	/* 焦点状态 */
	LCUI_Graph	btn_disable;	/* 不可用的状态 */
	
	LCUI_Widget	*label;	/* 嵌套的文本标签 */
};
/****************************************************/

typedef enum {
	BUTTON_STYLE_DEFAULT,
	BUTTON_STYLE_CUSTOM
} BUTTON_STYLE;

/* 获取嵌套在按钮部件里的label部件 */
LCUI_API LCUI_Widget*
Button_GetLabel( LCUI_Widget *widget );

/* 自定义按钮在各种状态下显示的位图 */
LCUI_API void
Button_CustomStyle(	LCUI_Widget *widget, LCUI_Graph *normal, 
			LCUI_Graph *over, LCUI_Graph *down, 
			LCUI_Graph *focus, LCUI_Graph *disable);

/* 设定按钮部件显示的文本内容 */
LCUI_API void
Button_Text( LCUI_Widget *widget, const char *text );

LCUI_API void
Button_TextW( LCUI_Widget *widget, const wchar_t *text );

/* 创建一个带文本内容的按钮 */
LCUI_API LCUI_Widget*
Button_New( const char *text );

LCUI_END_HEADER

#endif
