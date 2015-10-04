/* ***************************************************************************
 * widget_style.h -- widget style library module for LCUI.
 *
 * Copyright (C) 2015 by Liu Chao <lc-soft@live.cn>
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
 * widget_style.h -- LCUI 的部件样式库模块。
 *
 * 版权所有 (C) 2015 归属于 刘超 <lc-soft@live.cn>
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


#ifndef __LCUI_WIDGET_STYLE_LIBRARY_H__
#define __LCUI_WIDGET_STYLE_LIBRARY_H__

/** 初始化 */
void LCUIWidget_InitStyle( void );

/** 销毁，释放资源 */
void LCUIWidget_ExitStyle( void );

/** 新建一个样式表 */
LCUI_API LCUI_StyleSheet StyleSheet( void );

/** 合并两个样式表 */
LCUI_API int MergeStyleSheet( LCUI_StyleSheet dest, LCUI_StyleSheet src );

/** 覆盖样式表 */
LCUI_API int ReplaceStyleSheet( LCUI_StyleSheet dest, LCUI_StyleSheet src );

/** 删除样式表 */
LCUI_API void DeleteStyleSheet( LCUI_StyleSheet *ss );

/** 根据字符串内容生成相应的选择器 */
LCUI_API LCUI_Selector Selector( const char *selector );

/** 删除选择器 */
LCUI_API void DeleteSelector( LCUI_Selector *selector );

/** 判断两个选择器是否相等 */
LCUI_API LCUI_BOOL SelectorIsEqual( LCUI_Selector s1, LCUI_Selector s2 );

/** 匹配元素路径与样式结点路径 */
LCUI_API LCUI_BOOL IsMatchPath( LCUI_Widget *wlist, LCUI_Selector selector );

/** 向样式库添加样式表 */
LCUI_API int LCUI_PutStyle( LCUI_Selector selector, LCUI_StyleSheet in_ss );

/** 打印样式表的内容 */
LCUI_API void LCUI_PrintStyleSheet( LCUI_StyleSheet ss );

/** 打印选择器的内容 */
LCUI_API void LCUI_PrintSelector( LCUI_Selector selector );

/** 打印样式库中的内容 */
LCUI_API void LCUI_PrintStyleLibrary(void);

/** 计算部件继承得到的样式表 */
LCUI_API int Widget_ComputeInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss );

/** 更新当前部件的样式 */
LCUI_API void Widget_Update( LCUI_Widget w, LCUI_BOOL is_update_all );

#include <LCUI/gui/css_parser.h>

#endif
