/* ***************************************************************************
 * LCUI_Resources.h -- LCUI‘s Built-in resources, usually are graphics
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Resources.h -- LCUI 的内置资源，通常是一些图形
 *
 * 版权所有 (C) 2012 归属于 
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

#ifndef __LCUI_RES_H__
#define __LCUI_RES_H__

LCUI_BEGIN_HEADER

LCUI_EXPORT(int) Load_Graph_Default_Cursor(LCUI_Graph *pic);
/* 功能：载入默认的鼠标指针的图形 */ 

LCUI_EXPORT(int) Load_Graph_Mosaics(LCUI_Graph *pic);
/* 功能：载入马赛克图形 */ 

LCUI_EXPORT(int) Load_Graph_ProgressBar_Img(LCUI_Graph *out);

LCUI_EXPORT(int) Load_Graph_ProgressBar_Fore(LCUI_Graph *out);

LCUI_EXPORT(int) Load_Graph_Icon_Error(LCUI_Graph *pic);
/* 功能：载入图形，该图形为“错误”图标 */ 

LCUI_EXPORT(int) Load_Graph_Icon_Help(LCUI_Graph *pic);
/* 功能：载入图形，该图形为“帮助”图标 */ 

LCUI_EXPORT(int) Load_Graph_Icon_Info(LCUI_Graph *pic);
/* 功能：载入图形，该图形为“信息”图标 */ 

LCUI_EXPORT(int) Load_Graph_Icon_Warning(LCUI_Graph *pic);
/* 功能：载入图形，该图形为“警告”图标 */ 

LCUI_EXPORT(int) Load_Graph_Icon_LCUI(LCUI_Graph *pic);
/* 功能：载入图形，该图形为LCUI的LOGO */

LCUI_EXPORT(int) Load_Graph_Icon_LCUI_18x18(LCUI_Graph *out);

LCUI_EXPORT(int) Load_Graph_Default_TitleBar_CloseBox_Normal(LCUI_Graph *pic);
/* 功能：载入图形，该图形为标题栏上的关闭按钮，按钮为正常状态 */ 

LCUI_EXPORT(int) Load_Graph_Default_TitleBar_CloseBox_HighLight(LCUI_Graph *pic);
/* 功能：载入图形，该图形为标题栏上的关闭按钮，按钮为高亮状态 */ 

LCUI_EXPORT(int) Load_Graph_Default_TitleBar_CloseBox_Down(LCUI_Graph *pic);
/* 功能：载入图形，该图形为标题栏上的关闭按钮，按钮为按下状态 */ 

LCUI_EXPORT(int) Load_Graph_Default_TitleBar_BG(LCUI_Graph *pic);
/*
 * 功能：载入默认的标题栏背景图
 */

LCUI_EXPORT(int) Load_Graph_Default_CheckBox_Off_Normal(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_CheckBox_Off_Selected(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_CheckBox_Off_Disabled(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_CheckBox_On_Normal(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_CheckBox_On_Selected(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_CheckBox_On_Disabled(LCUI_Graph *pic);


LCUI_EXPORT(int) Load_Graph_Default_RadioButton_Off_Normal(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_RadioButton_Off_Selected(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_RadioButton_Off_Pressed(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_RadioButton_On_Normal(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_RadioButton_On_Selected(LCUI_Graph *pic);

LCUI_EXPORT(int) Load_Graph_Default_RadioButton_On_Pressed(LCUI_Graph *pic);

LCUI_END_HEADER

#endif
