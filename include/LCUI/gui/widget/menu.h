/* ***************************************************************************
 * menu.h -- LCUI's Menu widget，it is not yet perfect
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
 * menu.h -- LCUI 的菜单部件,还未完善
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
	LCUI_Widget widget_link; /* 记录与该菜单链接的部件 */
	LCUI_Widget parent_menu;	/* 记录父级菜单部件指针 */
	LCUI_Queue child_menu;/* 记录子菜单部件 */
};

LCUI_BEGIN_HEADER

LCUI_END_HEADER

#endif
