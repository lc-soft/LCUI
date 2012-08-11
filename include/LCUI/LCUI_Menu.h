/* ***************************************************************************
 * LCUI_Menu.h -- LCUI's Menu widget，it is not yet perfect
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
 * LCUI_Menu.h -- LCUI 的菜单部件,还未完善
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

/* 
 * 说明：
 * 需要改进的地方很多，如果鼠标指针离开菜单，菜单中的选项光标也会消失。等到让部件支持焦
 * 点状态的时候，菜单中处于焦点状态的部件会高亮显示，即使鼠标指针移开菜单，菜单中的焦点
 * 部件是不会变的，高亮状态还是保存着的。
 * */
 
#ifndef __LCUI_MENU_H__
#define __LCUI_MENU_H__

typedef struct _LCUI_Menu	LCUI_Menu;

struct _LCUI_Menu
{
	int mini_width;		/* 菜单的最低宽度 */
	LCUI_Widget *widget_link; /* 记录与该菜单链接的部件 */
	LCUI_Widget *parent_menu;	/* 记录父级菜单部件指针 */
	LCUI_Queue child_menu;/* 记录子菜单部件 */
};


void Show_Menu(LCUI_Widget *src, LCUI_Widget *des_menu);
/* 
 * 功能：将菜单显示在部件的下面
 * 说明：给定触发菜单的部件以及菜单部件指针，即可在该部件下显示菜单
 */
 
void Side_Show_Menu(LCUI_Widget *src, void *arg);
/* 
 * 功能：将菜单显示在部件的侧边
 * 说明：给定触发菜单的部件以及菜单部件指针，即可在该部件右边显示菜单
 */
 

int Show_Child_Menu(LCUI_Widget *item);
/*
 * 功能：显示与指定菜单项关联的子菜单
 * */

void Auto_Hide_Menu(LCUI_MouseEvent *event);
/*
 * 功能：根据鼠标事件，自动隐藏菜单
 * 说明：在鼠标产生点击事件时，会自动判断是否需要隐藏菜单
 */
 
LCUI_Widget *Create_Child_Menu(LCUI_Widget *parent_menu, LCUI_Widget *item);
/*
 * 功能：为菜单创建子菜单
 * 说明：子菜单属于哪个菜单项，由item决定
 **/

LCUI_Widget *Create_Menu_Item(LCUI_Widget *menu, const char *fmt, ...);
/*
 * 功能：为菜单添加选项
 * 说明：也就是以菜单为容器，将部件添加进去，你可以为菜单项添加显示文本
 */

void Hide_Menu(LCUI_Widget *menu);
/*
 * 功能：隐藏指定菜单以及它所有显示的子菜单
 * 说明：在隐藏菜单的同时，还会对该菜单的所有子菜单进行隐藏
 * */
#endif
