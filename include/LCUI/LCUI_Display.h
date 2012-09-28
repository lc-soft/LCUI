/* ****************************************************************************
 * LCUI_Display.h -- APIs of graphics display.
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
 * LCUI_Build.h -- 与图形显示相关的APIs
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

#ifndef __LCUI_DISPLAY_H__
#define __LCUI_DISPLAY_H__

LCUI_BEGIN_HEADER

int Graph_Display (LCUI_Graph * src, LCUI_Pos pos);
/* 功能：显示图形 */

int Get_Screen_Width ();
/*
 * 功能：获取屏幕宽度
 * 返回值：屏幕的宽度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */ 

int Get_Screen_Height ();
/*
 * 功能：获取屏幕高度
 * 返回值：屏幕的高度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */ 

LCUI_Size Get_Screen_Size ();
/* 功能：获取屏幕尺寸 */ 

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color);
/* 功能：填充指定位置的像素点的颜色 */ 

int Get_Screen_Graph(LCUI_Graph *out);
/* 
 * 功能：获取屏幕上显示的图像
 * 说明：自动分配内存给指针，并把数据拷贝至指针的内存 
 * */ 

int Add_Screen_Refresh_Area (LCUI_Rect rect);
/* 功能：在整个屏幕内添加需要刷新的区域 */ 

int Get_Screen_Bits();
/* 功能：获取屏幕中的每个像素的表示所用的位数 */ 

LCUI_Pos Get_Screen_Center_Point();
/* 功能：获取屏幕中心点的坐标 */ 

int Widget_Layer_Is_Opaque(LCUI_Widget *widget);
/* 功能：判断部件图形是否不透明 */ 

int Widget_Layer_Not_Visible(LCUI_Widget *widget);
/* 功能：检测部件图形是否完全透明 */ 

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue);
/* 
 * 功能：获取与指定区域重叠的部件 
 * 说明：得到的队列，队列中的部件排列顺序为：底-》上 == 左-》右
 * */

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph);
/* 
 * 功能：获取屏幕中指定区域内实际要显示的图形 
 * 说明：指定的区域必须是与部件区域不部分重叠的
 * */ 

int Enable_Graph_Display();
/* 功能：启用图形输出 */ 

int Disable_Graph_Display();
/* 功能：禁用图形输出 */ 

LCUI_END_HEADER

#endif
