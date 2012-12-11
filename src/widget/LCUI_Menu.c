/* ***************************************************************************
 * LCUI_Menu.c -- LCUI's Menu widget
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
 * LCUI_Menu.c -- LCUI 的菜单部件
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
 * 此部件可能与本版本的LCUI兼容，请等待后续版本更新中对该部件进行修改。
 * 需要改进的地方很多，如果鼠标指针离开菜单，菜单中的选项光标也会消失。等到让部件支持焦
 * 点状态的时候，菜单中处于焦点状态的部件会高亮显示，即使鼠标指针移开菜单，菜单中的焦点
 * 部件是不会变的，高亮状态还是保存着的。
 * */
 
#include <LCUI_Build.h>

#include LC_LCUI_H 
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_WIDGET_H
#include LC_BUTTON_H 
#include LC_DISPLAY_H
#include LC_MENU_H
#include LC_RES_H


static void Menu_Init(LCUI_Widget *widget)
/* 功能：初始化菜单的数据结构体 */
{
	/* 获取私有结构体指针，并分配内存 */
	LCUI_Menu *menu = Widget_Create_PrivData(widget, sizeof(LCUI_Menu));
	
	menu->parent_menu = NULL;
	menu->widget_link = NULL;
	menu->mini_width  = 50;
	WidgetQueue_Init(&menu->child_menu);
	Queue_Using_Pointer(&menu->child_menu);
	/* 设置边框风格类型为线条边框 */
	Set_Widget_Border_Style(widget, BORDER_STYLE_LINE);
	Set_Widget_Border(widget, RGB(50,50,50), Border(1,1,1,1));
	Set_Widget_BG_Mode(widget, BG_MODE_FILL_BACKCOLOR);
	LCUI_MouseEvent_Connect(Auto_Hide_Menu, MOUSE_EVENT_MOVE);
}

static void Exec_Update_Menu(LCUI_Widget *widget)
/* 功能：更新菜单的图形数据 */
{
	LCUI_Widget *item;
	LCUI_Menu *menu;
	int i, max_width = 0, total, max_height = 0;
	
	total = Queue_Get_Total(&widget->child);
	for(i=total-1; i>=0; --i)
	{   /* 按照顺序排列部件 */
		item = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(item->visible == IS_TRUE)
		{/* 如果该部件可见 */
			/* 设定部件的位置 */ 
			Move_Widget(item, Pos(0+widget->border.left, max_height+widget->border.top)); 
			/* 得出最大宽度 */
			max_width = (max_width < item->size.w ? item->size.w : max_width);
			/* 累加总高度 */
			max_height += item->size.h;
		}
	}
	menu = Get_Widget_PrivData(widget);
	/* 如果当前最大宽度低于设定的最低宽度 */
	if(max_width < menu->mini_width) 
		max_width = menu->mini_width;
	/* 得出最大宽度后，改变所有作为选项的部件的尺寸 */
	for(i=total-1; i>=0; --i)
	{   /* 按照顺序排列部件 */
		item = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(item == NULL) return;
		/* 改变尺寸 */
		Resize_Widget(item, Size(max_width, item->size.h));
	}
	
	int w, h;
	w = widget->border.left + widget->border.right;
	h = widget->border.top + widget->border.bottom;
	/* 更改窗口的尺寸，以适应部件的尺寸 */ 
	Resize_Widget(widget, Size(max_width+w, max_height+h));
	Refresh_Widget(widget); /* 每次更新，都需要刷新整个部件的图形显示 */
}

void Show_Menu(LCUI_Widget *src, LCUI_Widget *des_menu)
/* 
 * 功能：将菜单显示在部件的下面
 * 说明：给定触发菜单的部件以及菜单部件指针，即可在该部件下显示菜单，如果
 * 调用本函数时，目标菜单是显示状态，那么会对它进行隐藏
 */
{
	if(des_menu->visible == IS_TRUE)
	{/* 如果菜单是显示状态，隐藏之，这个隐藏，会将它的以显示的子菜单隐藏 */
		Hide_Menu(des_menu);
		return;
	}
	int height, width;
	LCUI_Pos src_pos, display_pos;  
	LCUI_Menu *menu = Get_Widget_PrivData(des_menu);
	height = Get_Screen_Height();
	width = Get_Screen_Width();
	
	/* 通过部件的全局坐标以及尺寸,得出菜单的显示位置 */
	src_pos = Get_Widget_Global_Pos(src);
	display_pos.x = src_pos.x;
	display_pos.y = src_pos.y + src->size.h;
	
	/* 如果菜单的显示位置超出屏幕显示范围,就对菜单的位置进行相应处理 */
	if(display_pos.x + des_menu->size.w > width) 
		display_pos.x = width - des_menu->size.w;
		
	if(display_pos.x < 0) 
		display_pos.x = 0;
		
	if(display_pos.y + des_menu->size.h > height) 
		display_pos.y = src_pos.y - des_menu->size.h;
		
	if(display_pos.y < 0) 
		display_pos.y = 0;
	
	menu->widget_link = src;/* 保存与之关联的部件 */
	Move_Widget(des_menu, display_pos);
	Show_Widget(des_menu);
}

void Side_Show_Menu(LCUI_Widget *src, void *arg)
/* 
 * 功能：将菜单显示在部件的侧边
 * 说明：给定触发菜单的部件以及菜单部件指针，即可在该部件右边显示菜单
 */
{
	LCUI_Widget *des_menu = (LCUI_Widget*)arg;
	if(des_menu == NULL) return;
	if(des_menu->visible == IS_TRUE)
	{/* 如果菜单是显示状态，隐藏之，这个隐藏，会将它的以显示的子菜单隐藏 */
		Hide_Menu(des_menu);
		return;
	}
	int height, width;
	LCUI_Pos src_pos, display_pos;  
	LCUI_Menu *menu = Get_Widget_PrivData(des_menu);
	
	height = Get_Screen_Height();
	width = Get_Screen_Width();
	
	/* 通过部件的全局坐标以及尺寸,得出菜单的显示位置 */
	src_pos = Get_Widget_Global_Pos(src);
	display_pos.x = src_pos.x + src->size.w;
	display_pos.y = src_pos.y;
	
	/* 如果菜单的显示位置超出屏幕显示范围,就对菜单的位置进行相应处理 */
	if(display_pos.x + des_menu->size.w > width) 
		display_pos.x = width - des_menu->size.w;
		
	if(display_pos.x < 0) 
		display_pos.x = 0;
		
	if(display_pos.y + des_menu->size.h > height) 
		display_pos.y = src_pos.y - des_menu->size.h;
		
	if(display_pos.y < 0) 
		display_pos.y = 0;
		
	menu->widget_link = src;/* 保存与之关联的部件 */
	Move_Widget(des_menu, display_pos);
	Show_Widget(des_menu);  
}


void Hide_Menu(LCUI_Widget *menu)
/*
 * 功能：隐藏指定菜单以及它所有显示的子菜单
 * 说明：在隐藏菜单的同时，还会对该菜单的所有子菜单进行隐藏
 * */
{
	if(menu == NULL) return;
	int i;
	LCUI_Menu *data = Get_Widget_PrivData(menu);
	LCUI_Widget *widget;
	for(i=0; ;i++)
	{/* 获取子菜单队列中的部件指针 */
		widget = (LCUI_Widget*)Queue_Get(&data->child_menu, i); 
		if(widget == NULL) break;/* 如果获取失败，退出循环 */
		Hide_Menu(widget);/* 递归调用 */
	}
	Hide_Widget(menu);
}


LCUI_Widget *Get_Child_Menu(LCUI_Widget *des_menu, LCUI_Widget *item)
/* 功能：获取菜单中指定选项想关联的子菜单的指针 */
{
	int i;
	LCUI_Widget *widget;
	LCUI_Menu *menu = (LCUI_Menu*)Get_Widget_PrivData(des_menu);
	LCUI_Menu *tmp_menu;
	for(i=0; ; ++i)
	{
		widget = (LCUI_Widget*)Queue_Get(&menu->child_menu, i);
		tmp_menu = (LCUI_Menu*)Get_Widget_PrivData(widget);
		if(tmp_menu->widget_link == item)
		{/* 如果这个菜单和item关联 */
			return widget;
		}
	}
	return NULL;
}

void Hide_Other_Menu(LCUI_Widget *des_menu, LCUI_Widget *item)
/* 功能：隐藏除菜单中的选项以外的其它选项所关联的子菜单 */
{
	if(des_menu == NULL) return;
	int i;
	LCUI_Widget *widget;
	LCUI_Menu *menu = (LCUI_Menu*)Get_Widget_PrivData(des_menu);
	LCUI_Menu *tmp_menu;
	for(i=0; ; ++i)
	{
		/* 获取子菜单指针 */
		widget = (LCUI_Widget*)Queue_Get(&menu->child_menu, i); 
		if(widget == NULL) break;
		tmp_menu = (LCUI_Menu*)Get_Widget_PrivData(widget);
		if(tmp_menu->widget_link != item)
		{/* 如果这个菜单不是和item关联的 */
			/* 隐藏这个部件关联的菜单 */
			Hide_Menu(widget);
		}
	}
}

static LCUI_Widget *save_menu = NULL;
void Auto_Hide_Menu(LCUI_MouseEvent *event)
/*
 * 功能：根据鼠标事件，自动隐藏菜单
 * 说明：在鼠标产生点击事件时，会自动判断是否需要隐藏菜单
 */
{
	LCUI_Widget *menu;
	if(event->widget != NULL)
	{/* 如果当前鼠标指针在部件上 */
		/* 获取该部件的类型为菜单的父部件，因为有时会遇到嵌套类型的部件 */
		menu = Get_Parent_Widget(event->widget, "menu");
		if(menu != NULL)
		{/* 如果该部件是菜单里的 */
			/* 显示与当前部件关联的子菜单 */
			Show_Child_Menu(event->widget);
		}
		/* 隐藏其它菜单项的子菜单 */
		Hide_Other_Menu(menu, event->widget);
		save_menu = menu;
	}
}

int Show_Child_Menu(LCUI_Widget *item)
/* 功能：显示与指定菜单项关联的子菜单 */
{
	/* 如果该菜单项没有父部件或者父部件不是菜单，直接退出函数 */
	if(item->parent == NULL) return 0;
	if(Strcmp(&item->parent->type, "menu") != 0) 
	{
		LCUI_Widget *t;
		t = item;
		while(t->parent != NULL)
		{
			if(Strcmp(&t->parent->type, "menu") == 0)
			{
				item = t;
				break;
			}
			t = t->parent;
		}
		/* 再判断一次，如果父部件不是菜单，就退出函数 */
		if(Strcmp(&item->parent->type, "menu") != 0) 
			return 0;
	}
	
	LCUI_Widget *child_menu;
	LCUI_Menu *menu, *tmp;
	int i;
	menu = (LCUI_Menu*)Get_Widget_PrivData(item->parent); 
	for(i=0; ; i++)
	{/* 获取各个子菜单的指针 */
		child_menu = (LCUI_Widget*)Queue_Get(&menu->child_menu, i);
		if(child_menu == NULL) 
			break;
		else
		{
			/* 获取子菜单的私有数据结构体的指针 */
			tmp = (LCUI_Menu*)Get_Widget_PrivData(child_menu); 
			if(item == tmp->widget_link)
			{/* 如果这个菜单项是与子菜单链接的那个部件 */
				Side_Show_Menu(item, child_menu);/* 显示在这个部件的侧边 */
				break;
			}
		}
	}
	return 1;
}

LCUI_Widget *Create_Child_Menu(LCUI_Widget *parent_menu, LCUI_Widget *item)
/*
 * 功能：为菜单创建子菜单
 * 说明：子菜单属于哪个菜单项，由item决定
 **/
{
	LCUI_Widget *child_menu;
	LCUI_Menu *menu, *pa_menu;
	child_menu = Create_Widget("menu");
	pa_menu = (LCUI_Menu*)Get_Widget_PrivData(parent_menu);
	menu = (LCUI_Menu*)Get_Widget_PrivData(child_menu);
	menu->parent_menu = parent_menu;/* 记录父级菜单 */
	menu->widget_link = item; /* 记录与子菜单关联部件 */
	Queue_Add(&pa_menu->child_menu, child_menu);/* 添加至父菜单的子菜单队列 */
	Widget_Clicked_Event_Connect(item, Side_Show_Menu, child_menu);
	return child_menu; /* 将创建的子菜单部件的指针返回给上一级函数 */
}

LCUI_Widget *Create_Menu_Item(LCUI_Widget *menu, const char *fmt, ...)
/*
 * 功能：为菜单添加选项
 * 说明：也就是以菜单为容器，将部件添加进去，你可以为菜单项添加显示文本
 */
{
	char text[LABEL_TEXT_MAX_SIZE];
    memset(text, 0, sizeof(text)); 
    
    va_list ap; 
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap); 
	
	/* 菜单栏中的选项，我用按钮来实现 */
	LCUI_Widget *item = Create_Button_With_Text(text);
	if(item == NULL) return NULL;
	Set_Widget_Style(item, "menu_style");
	Widget_Container_Add(menu, item); /* 将item放入容器 */
	Show_Widget(item); /* 显示之 */
	return item;
}


void Register_Menu()
/* 功能：注册部件类型-菜单至部件库 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("menu");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("menu",	Menu_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("menu",	Exec_Update_Menu,	FUNC_TYPE_UPDATE);
}
