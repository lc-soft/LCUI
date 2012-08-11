/* ***************************************************************************
 * LCUI_Window.c -- LCUI's window widget
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
 * LCUI_Window.c -- LCUI 的窗口部件
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

#include <LCUI_Build.h> 
#include LC_LCUI_H 
#include LC_WIDGET_H
#include LC_BUTTON_H
#include LC_MEM_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_WINDOW_H
#include LC_MISC_H 
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_INPUT_H
#include LC_FONT_H
#include LC_ERROR_H

LCUI_Widget *Get_Window_TitleBar(LCUI_Widget *window)
/* 功能：获取窗口标题栏的指针 */
{ 
	LCUI_Window *win_p = (LCUI_Window *)window->private; 
	return win_p->titlebar;
}

LCUI_Widget *Get_Window_Client_Area(LCUI_Widget *window)
/* 功能：获取窗口客户区的指针 */
{
	LCUI_Window *win_p = (LCUI_Window *)window->private;
	return win_p->client_area;	
}

static void Move_Window(LCUI_Widget *titlebar, LCUI_DragEvent *event)
/* 功能：处理鼠标移动事件 */
{
	LCUI_Pos pos;
	LCUI_Widget *window; 
	if(event->first_click == 0 )
	{
		window = titlebar->parent;
		pos = event->new_pos;
		if(window != NULL)
		{
			/* 减去在窗口中的相对坐标, 得出窗口位置 */
			pos = Pos_Sub(pos, Get_Widget_Pos(titlebar));
			/* 移动窗口的位置 */
			Move_Widget(window, pos);
		}
	}
}

void Set_Window_Title_Icon(LCUI_Widget *window, LCUI_Graph *icon)
/* 功能：自定义指定窗口的标题栏图标 */
{
	LCUI_Graph *image;
	LCUI_Widget *title_widget = Get_Window_TitleBar(window);
	LCUI_TitleBar *title_data = (LCUI_TitleBar *)
							Get_Widget_Private_Data(title_widget);
	image = Get_PictureBox_Graph(title_data->icon_box);
	Free_Graph(image);/* 释放内存 */
	if(icon != NULL)
	{
		Set_PictureBox_Image_From_Graph(title_data->icon_box, icon);
		Set_Widget_Align(title_data->icon_box, ALIGN_MIDDLE_LEFT, Pos(3,0));
		Set_Widget_Align(title_data->label, ALIGN_MIDDLE_LEFT, Pos(23,0));
	}
}

static void Window_TitleBar_Init(LCUI_Widget *titlebar)
/* 功能：初始化窗口标题栏 */
{
	LCUI_Graph img;
	LCUI_TitleBar *t;
	Graph_Init(&img);
	t = Malloc_Widget_Private(titlebar, sizeof(LCUI_TitleBar));
	t->icon_box = Create_Widget("picture_box");
	t->label = Create_Widget("label");
	Widget_Container_Add(titlebar, t->icon_box);
	Widget_Container_Add(titlebar, t->label);
	Resize_Widget(t->icon_box, Size(18,18));
	Set_PictureBox_Size_Mode(t->icon_box, SIZE_MODE_CENTER);
	Show_Widget(t->icon_box);
	Show_Widget(t->label);
	Set_Widget_Align(t->icon_box, ALIGN_MIDDLE_LEFT, Pos(0,0));
	Set_Widget_Align(t->label, ALIGN_MIDDLE_LEFT, Pos(0,0));
	Load_Graph_Default_TitleBar_BG(&img);
	Set_Widget_Background_Image(titlebar, &img, LAYOUT_STRETCH);
}

LCUI_Size Get_Window_Client_Size(LCUI_Widget *win_p)
/* 功能：获取窗口的客户区的尺寸 */
{
	LCUI_Widget *client_area	= Get_Window_Client_Area(win_p);
	return client_area->size;
}

void Window_Widget_Auto_Size(LCUI_Widget *win_p)
/* 功能：在窗口尺寸改变时自动改变标题栏和客户区的尺寸 */
{
	int x, y, width, height;
	LCUI_Widget *titlebar		= Get_Window_TitleBar(win_p);
	LCUI_Widget *client_area	= Get_Window_Client_Area(win_p);
	/* 按不同的风格来处理 */
	switch(Get_Widget_Border_Style(win_p))
	{
		case BORDER_STYLE_NONE:  /* 没有边框 */
			/* 先计算坐标和尺寸 */
			x = win_p->border.left;
			y = win_p->border.top;
			width = win_p->size.w - x - win_p->border.right;
			height = win_p->size.h - y - win_p->border.bottom;
			
			Move_Widget( client_area, Pos(x, y) );/* 调整位置 */
			Resize_Widget( client_area, Size(width, height) );/* 调整大小 */
			Hide_Widget( titlebar );/* 隐藏标题栏 */
			Show_Widget( client_area );/* 客户区需要显示 */
			break;
			
		case BORDER_STYLE_LINE_BORDER: /* 线条边框 */
			Move_Widget( client_area, Pos(0, 0) );
			Resize_Widget( client_area, Size(win_p->size.w, win_p->size.h) );
			Hide_Widget( titlebar);
			Show_Widget( client_area);
			break;
			
		case BORDER_STYLE_STANDARD: /* 标准边框 */
		/* 
		 * 说明：由于用户区的尺寸和位置的调整，需要确定标题栏的尺寸，因此，
		 * 调用Exec_Resize_Widget()函数立刻调整标题栏尺寸，如果是调用
		 * Resize_Widget()函数，那么，确定的尺寸会有误，因为标题栏的尺
		 * 寸还是以前的尺寸。
		 * */ 
			Set_Widget_Border(win_p, RGB(50,50,50), Border(1,1,1,1));
			/* 先计算坐标和尺寸 */
			x = win_p->border.left;
			y = win_p->border.top;
			width = win_p->size.w - x - win_p->border.right;
			height = win_p->size.h - y - win_p->border.bottom;
			
			Move_Widget(titlebar, Pos(x, y) );
			Exec_Resize_Widget(titlebar, Size(width, 25));
			
			Move_Widget(client_area, Pos(x, y + titlebar->size.h));
			Resize_Widget(client_area, Size(width, height - titlebar->size.h));
			/* 标题栏和客户区都需要显示 */
			Show_Widget(titlebar); 
			Show_Widget(client_area); 
			break;
			
		default:
			//
			break;
	} 
}

static void Exec_Update_Window(LCUI_Widget *win_p)
/* 功能：更新窗口图形数据 */
{
	LCUI_Widget *titlebar = Get_Window_TitleBar(win_p);
	LCUI_Widget *client_area = Get_Window_Client_Area(win_p);
	/* 更改窗口边框风格时，这个函数起到一定作用，因为要根据风格来调整窗口的内容 */
	Window_Widget_Auto_Size(win_p);
	/* 更新下面的部件 */
	Draw_Widget(titlebar);
	Draw_Widget(client_area);
}

LCUI_Widget *Get_Parent_Window(LCUI_Widget *widget)
/* 功能：获取指定部件所在的窗口 */
{
	if(widget == NULL) return NULL;
	if(widget->parent == NULL) return NULL;
	if(strcmp(widget->parent->type.string, "window") == 0)
		return widget->parent;
	
	return Get_Parent_Window(widget->parent);
}


static void Quit_Parent_Window(LCUI_Widget *btn, void *arg)
/* 功能：退出部件btn所在的窗口 */
{
	//printf("Quit_Parent_Window start\n");
	Main_Loop_Quit();
	//printf("Quit_Parent_Window end\n");
	//return;
	//LCUI_Widget *win_p = Get_Parent_Window(btn);
	//if(win_p == NULL) 
	//	puts(QUIT_PARENT_WINDOW_ERROR);
	//Delete_Widget(win_p);
}

static void Destroy_Window(LCUI_Widget *win_p)
/*
 * 功能：释放window部件占用的内存资源
 * 说明：类似于析构函数
 **/
{
	//由于没有指针变量申请过内存，因此不需要释放指针变量
}

static void Window_Init(LCUI_Widget *win_p)
/*
 * 功能：初始化窗口
 * 说明：类似于构造函数
 **/
{
	LCUI_Widget *titlebar;
	LCUI_Widget *client_area;
	LCUI_Widget *btn_close;
	LCUI_Window *win;
	win = (LCUI_Window*)Malloc_Widget_Private(win_p, sizeof(LCUI_Window));
	win->hide_style	 = NONE;
	win->show_style	 = NONE;
	win->count		 = 0;
	win->init_align = ALIGN_MIDDLE_CENTER;
	
	/* 创建一个标题栏部件 */
	titlebar = Create_Widget("titlebar");
	/* 再创建一个客户区部件 */
	client_area = Create_Widget(NULL); 
	btn_close = Create_Widget("button"); 

	static LCUI_Graph btn_highlight, btn_normal, btn_down; 
	Graph_Init(&btn_down);
	Graph_Init(&btn_highlight);
	Graph_Init(&btn_normal);
	/* 载入默认图形 */
	Load_Graph_Default_TitleBar_CloseBox_Normal(&btn_normal);
	Load_Graph_Default_TitleBar_CloseBox_Down(&btn_down);
	Load_Graph_Default_TitleBar_CloseBox_HighLight(&btn_highlight);
	/* 显示在左上角 */
	Set_Widget_Align(btn_close, ALIGN_TOP_RIGHT, Pos(0, -2)); 
	/* 将尺寸改成和图片一样 */
	Resize_Widget(btn_close, Size(btn_normal.width, btn_normal.height));
	Custom_Button_Style(btn_close, 
			&btn_normal, &btn_highlight, &btn_down, NULL, NULL);
	/* 关联按钮的点击事件，当按钮被点击后，调用Quit_Window函数 */
	Widget_Clicked_Event_Connect(btn_close, Quit_Parent_Window, NULL);
	Free_Graph(&btn_highlight);
	Free_Graph(&btn_down);
	Free_Graph(&btn_normal);
	
	win->client_area = client_area;
	win->titlebar = titlebar;
	win->btn_close = btn_close;
	/* 没有背景图就填充背景色 */
	Set_Widget_BG_Mode(win_p, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_Border_Style(win_p, BORDER_STYLE_STANDARD); 
	
	/* 放入至容器 */
	Widget_Container_Add(titlebar, btn_close);
	Widget_Container_Add(win_p, titlebar);
	Widget_Container_Add(win_p, client_area);
	Resize_Widget(win_p, Size(50, 50));
	Show_Widget(btn_close);
	
	Widget_Drag_Event_Connect(titlebar, Move_Window); 
}

static void Show_Window(LCUI_Widget *win_p)
/* 功能：在窗口显示时，进行相关处理 */
{
	int w, h;
	LCUI_Pos pos;
	LCUI_Window *win = (LCUI_Window*)Get_Widget_Private_Data(win_p);
	pos.x = 0;
	pos.y = 0;
	win->count++;
	if(win->count == 1)
	{/* 如果是第一次显示 */
		if(win_p->parent == NULL && win_p->pos_type == POS_TYPE_IN_SCREEN)
		{
			w = Get_Screen_Width();
			h = Get_Screen_Height();
		}
		else 
		{
			w = Get_Widget_Width(win_p->parent);
			h = Get_Widget_Height(win_p->parent);
		}
		
		switch(win->init_align)
		{/* 窗口的位置 */
		case ALIGN_TOP_LEFT : 
			break;
		case ALIGN_TOP_CENTER :
			pos.x = (w - Get_Widget_Width(win_p))/2;
			break;
		case ALIGN_TOP_RIGHT :
			pos.x = w - Get_Widget_Width(win_p);
			break;
		case ALIGN_MIDDLE_LEFT :
			pos.y = (h - Get_Widget_Height(win_p))/2;
			break;
		case ALIGN_MIDDLE_CENTER :
			pos.y = (h - Get_Widget_Height(win_p))/2;
			pos.x = (w - Get_Widget_Width(win_p))/2; 
			break;
		case ALIGN_MIDDLE_RIGHT :
			pos.x = w - Get_Widget_Width(win_p);
			pos.y = (h - Get_Widget_Height(win_p))/2;
			break;
		case ALIGN_BOTTOM_LEFT :
			pos.y = h - Get_Widget_Height(win_p);
			pos.x = 0;
		case ALIGN_BOTTOM_CENTER :
			pos.y = h - Get_Widget_Height(win_p);
			pos.x = (w - Get_Widget_Width(win_p))/2;
		case ALIGN_BOTTOM_RIGHT :
			pos.y = h - Get_Widget_Height(win_p);
			pos.x = w - Get_Widget_Width(win_p);
		default : 
			break;
		}
		//printf("window pos: %d,%d\n",x, y);
		Set_Widget_Pos(win_p, pos);
	}
	//有待扩展 
}

static void Hide_Window(LCUI_Widget *win_p)
/* 功能：在隐藏窗口时使用视觉特效 */
{
	//有待扩展
}

static void Resize_Window(LCUI_Widget *win_p)
/* 功能：在改变窗口尺寸时使用视觉特效 */
{
	//有待扩展
	
	/* 为该类型部件添加相关函数，让这两个部件在窗口尺寸改变时也改变自己的尺寸 */ 
	Window_Widget_Auto_Size(win_p); 
}

void Set_Window_Title_Text(LCUI_Widget *win_p, char *text)
/* 功能：为窗口设置标题文字 */
{
	LCUI_Widget *titlebar = Get_Window_TitleBar(win_p);
	LCUI_TitleBar *title = Get_Widget_Private_Data(titlebar);
	Set_Label_Text(title->label, text);
}


void Window_Client_Area_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* 功能：将部件添加至窗口客户区 */
{
	LCUI_Widget *w = Get_Window_Client_Area(window);
	Widget_Container_Add(w, widget);
}

void Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* 功能：将部件添加至窗口标题栏 */
{
	LCUI_Widget *w = Get_Window_TitleBar(window);
	Widget_Container_Add(w, widget);
}

void Register_Window()
/* 功能：注册部件类型-窗口至部件库 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("window");
	WidgetType_Add("titlebar");
	
	/* 为部件类型关联相关函数 */ 
	WidgetFunc_Add("titlebar",	Window_TitleBar_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("window",	Window_Init,			FUNC_TYPE_INIT);
	WidgetFunc_Add("window",	Exec_Update_Window,		FUNC_TYPE_UPDATE);
	WidgetFunc_Add("window",	Resize_Window,			FUNC_TYPE_RESIZE);
	WidgetFunc_Add("window",	Show_Window,			FUNC_TYPE_SHOW);
	WidgetFunc_Add("window",	Hide_Window,			FUNC_TYPE_HIDE);
	WidgetFunc_Add("window",	Destroy_Window,			FUNC_TYPE_DESTROY);
}

