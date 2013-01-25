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
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include LC_BUTTON_H 
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_WINDOW_H 
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_RES_H
#include LC_INPUT_H
#include LC_FONT_H
#include LC_ERROR_H

LCUI_Widget *
Get_Window_TitleBar(LCUI_Widget *window)
/* 功能：获取窗口标题栏的指针 */
{ 
	LCUI_Window *win_p; 
	win_p = (LCUI_Window *)Get_Widget_PrivData(window); 
	if(win_p == NULL) {
		return NULL;
	}
	return win_p->titlebar;
}

LCUI_Widget *
Get_Window_Client_Area(LCUI_Widget *window)
/* 功能：获取窗口客户区的指针 */
{
	LCUI_Window *win_p;
	win_p = (LCUI_Window *)Get_Widget_PrivData(window); 
	return win_p->client_area;	
}

static void 
Move_Window(LCUI_Widget *titlebar, LCUI_DragEvent *event)
/* 功能：处理鼠标移动事件 */
{
	LCUI_Pos pos, offset;
	LCUI_Widget *window;
	
	window = titlebar->parent;
	if( !window ) {
		return;
	}
	//_DEBUG_MSG( "new:%d,%d, cursor:%d,%d\n", 
	//event->new_pos.x, event->new_pos.y, 
	//event->cursor_pos.x, event->cursor_pos.y );
	/* 将新全局坐标减去标题栏的全局坐标，得到偏移坐标 */
	pos = Get_Widget_Global_Pos( titlebar );
	offset = Pos_Sub( event->new_pos, pos );
	pos = Get_Widget_Global_Pos( window );
	/* 将偏移坐标加在窗口全局坐标上，得出窗口的新全局坐标 */
	pos = Pos_Add( pos, offset );
	/* 转换成在容器区域内的相对坐标 */
	pos = GlobalPos_ConvTo_RelativePos( window, pos );
	/* 移动窗口的位置 */
	Move_Widget( window, pos );
}

void 
Set_Window_Title_Icon(LCUI_Widget *window, LCUI_Graph *icon)
/* 功能：自定义指定窗口的标题栏图标 */
{
	LCUI_Graph *image;
	LCUI_Widget *title_widget;
	LCUI_TitleBar *title_data;
	
	title_widget = Get_Window_TitleBar(window);
	title_data = (LCUI_TitleBar *)Get_Widget_PrivData(title_widget);
	
	image = Get_PictureBox_Graph(title_data->icon_box);
	Graph_Free(image);/* 释放PictureBox部件内的图像占用的资源 */
	if(icon == NULL) {
		return;
	}
	/* 设置新图标 */
	Set_PictureBox_Image_From_Graph(title_data->icon_box, icon);
	Set_Widget_Align(title_data->icon_box, ALIGN_MIDDLE_LEFT, Pos(3,0));
	Set_Widget_Align(title_data->label, ALIGN_MIDDLE_LEFT, Pos(23,0));
 
}

static void 
Window_TitleBar_Init(LCUI_Widget *titlebar)
/* 功能：初始化窗口标题栏 */
{
	LCUI_Graph img;
	LCUI_TitleBar *t;
	
	Graph_Init(&img);
	t = Widget_Create_PrivData(titlebar, sizeof(LCUI_TitleBar));
	t->icon_box = Create_Widget("picture_box");
	t->label = Create_Widget("label");
	/* 窗口图标和标题文字不可获得焦点，并忽略鼠标点击 */
	t->label->focus = FALSE;
	t->icon_box->focus = FALSE;
	Set_Widget_ClickableAlpha( t->label, 0, 1 );
	Set_Widget_ClickableAlpha( t->icon_box, 0, 1 );
	
	Widget_Container_Add(titlebar, t->icon_box);
	Widget_Container_Add(titlebar, t->label);
	
	Resize_Widget(t->icon_box, Size(18,18));
	Set_PictureBox_Size_Mode(t->icon_box, SIZE_MODE_CENTER);
	
	Show_Widget(t->icon_box);
	Show_Widget(t->label);
	
	Set_Widget_Align(t->icon_box, ALIGN_MIDDLE_LEFT, Pos(0,0));
	Set_Widget_Align(t->label, ALIGN_MIDDLE_LEFT, Pos(2,-2));
	Load_Graph_Default_TitleBar_BG(&img);
	Set_Widget_Background_Image(titlebar, &img, LAYOUT_STRETCH);
}

LCUI_Size 
Get_Window_Client_Size(LCUI_Widget *win_p)
/* 功能：获取窗口的客户区的尺寸 */
{
	LCUI_Widget *client_area = Get_Window_Client_Area(win_p);
	return client_area->size;
}

static void
Exec_Update_Window( LCUI_Widget *win_p )
{
	LCUI_Size size;
	LCUI_Graph *graph;
	LCUI_Widget *titlebar;
	LCUI_Widget *client_area;
	LCUI_RGB border_color, back_color;
	
	titlebar = Get_Window_TitleBar(win_p);
	client_area = Get_Window_Client_Area(win_p);
	graph = Widget_GetSelfGraph( win_p );
	/* 按不同的风格来处理 */ 
	switch( win_p->style_id ) {
	    case WINDOW_STYLE_NONE:  /* 没有边框 */
		/* 先计算坐标和尺寸 */
		Set_Widget_Dock( client_area, DOCK_TYPE_FILL );
		Hide_Widget( titlebar );/* 隐藏标题栏 */
		Show_Widget( client_area );/* 客户区需要显示 */
		break;
			
	    case WINDOW_STYLE_LINE: /* 线条边框 */
		Set_Widget_Border(win_p,
		 Border(1, BORDER_STYLE_SOLID, RGB(50,50,50)));
		Set_Widget_Padding( win_p, Padding(1,1,1,1) );
		Set_Widget_Dock( client_area, DOCK_TYPE_FILL );
		Hide_Widget( titlebar );
		Show_Widget( client_area );
		break;
		
	    case WINDOW_STYLE_STANDARD: /* 标准边框 */ 
		Set_Widget_Border(win_p,
		 Border(1, BORDER_STYLE_SOLID, RGB(50,50,50)));
		Set_Widget_Padding(win_p, Padding(1,1,1,1)); 
		
		size = Get_Container_Size( win_p );
		
		Resize_Widget(titlebar, Size(size.w, 25));
		Resize_Widget(client_area, Size(size.w, size.h - 25));
		/* 标题栏向顶部停靠 */
		Set_Widget_Dock( titlebar, DOCK_TYPE_TOP );
		/* 客户区向底部停靠 */
		Set_Widget_Dock( client_area, DOCK_TYPE_BOTTOM );
		/* 标题栏和客户区都需要显示 */
		Show_Widget( titlebar ); 
		Show_Widget( client_area ); 
		break;
			
	    case WINDOW_STYLE_PURE_BLUE: 
		back_color = RGB(30,160,225); 
		border_color = RGB(0,130,195);
		goto union_draw_method;
	    case WINDOW_STYLE_PURE_GREEN:
		back_color = RGB(140,190,40);
		border_color = RGB(110,160,10);
		goto union_draw_method;
	    case WINDOW_STYLE_PURE_RED: 
		back_color = RGB(230,20,0);
		border_color = RGB(200,0,0);
		goto union_draw_method;
	    case WINDOW_STYLE_PURE_ORANGE: 
		back_color = RGB(240,150,10);
		border_color = RGB(210,120,0); 
		goto union_draw_method;
	    case WINDOW_STYLE_PURE_PURPLE:
		back_color = RGB(110,20,95);
		border_color = RGB(80,0,65); 
union_draw_method:;
		/* 若窗口未获得焦点 */
		if( !Focus_Widget( win_p ) ) {
			back_color = RGB(255,255,255);
			border_color = RGB(50,50,50); 
		}
		
		Set_Widget_Border( win_p,
		 Border(1, BORDER_STYLE_SOLID, border_color));
		Set_Widget_Border( client_area,
		 Border(1, BORDER_STYLE_SOLID, border_color));
		Set_Widget_Backcolor( win_p, back_color );
		Graph_Fill_Color( graph, back_color );
		Set_Widget_Backcolor( client_area, RGB(255,255,255) );
		Set_Widget_Background_Image( titlebar, NULL, 0 );
		Set_Widget_BG_Mode( titlebar, BG_MODE_TRANSPARENT ); 
		Set_Widget_BG_Mode( client_area, BG_MODE_FILL_BACKCOLOR ); 
		Set_Widget_Padding( win_p, Padding(1,4,4,4) );
		Set_Widget_Padding( client_area, Padding(1,1,1,1) );
		size = Get_Container_Size( win_p );
		Resize_Widget( titlebar, Size(size.h, 25) );
		Resize_Widget( client_area, Size(size.w, size.h - 25) );
		Set_Widget_Dock( titlebar, DOCK_TYPE_TOP ); 
		Set_Widget_Dock( client_area, DOCK_TYPE_BOTTOM );
		Show_Widget( titlebar );
		Show_Widget( client_area ); 
		break;
	    default:
			//
		break;
	}
}

LCUI_Widget *
Get_Parent_Window(LCUI_Widget *widget)
/* 功能：获取指定部件所在的窗口 */
{
	if( !widget || !widget->parent ) {
		return NULL;
	}
	if(strcmp(widget->parent->type.string, "window") == 0) {
		return widget->parent;
	}
	
	return Get_Parent_Window(widget->parent);
}


static void 
Quit_Parent_Window(LCUI_Widget *btn, void *arg)
/* 功能：退出部件btn所在的窗口 */
{
	//printf("Quit_Parent_Window start\n");
	LCUI_StopMainLoop();
	//printf("Quit_Parent_Window end\n");
	//return;
	//LCUI_Widget *win_p = Get_Parent_Window(btn);
	//if(win_p == NULL) 
	//	puts(QUIT_PARENT_WINDOW_ERROR);
	//Delete_Widget(win_p);
}

static void 
Destroy_Window(LCUI_Widget *win_p)
/*
 * 功能：释放window部件占用的内存资源
 * 说明：类似于析构函数
 **/
{
	//由于没有指针变量申请过内存，因此不需要释放指针变量
}

static void
Window_FocusOut( LCUI_Widget *window, void *arg )
/* 在窗口失去焦点时会调用此函数 */
{
	//printf( "Window_FocusOut!\n" );
	Update_Widget( window );
}

static void
Window_FocusIn( LCUI_Widget *window, void *arg )
/* 在窗口获得焦点时会调用此函数 */
{
	//printf( "Window_FocusIn!\n" );
	Front_Widget( window ); /* 前置窗口 */
	Update_Widget( window ); /* 更新窗口 */
}

static void 
Window_Init(LCUI_Widget *win_p)
/* 初始化window部件相关数据 */
{
	LCUI_Widget *titlebar;
	LCUI_Widget *client_area;
	LCUI_Widget *btn_close;
	LCUI_Window *win;
	
	win = Widget_Create_PrivData(win_p, sizeof(LCUI_Window));
	
	win->hide_style	= NONE;
	win->show_style	= NONE;
	win->count	= 0;
	win->init_align	= ALIGN_MIDDLE_CENTER;
	
	titlebar = Create_Widget("titlebar"); 
	client_area = Create_Widget(NULL); 
	btn_close = Create_Widget("button"); 
	
	titlebar->focus = FALSE;
	
	Set_Focus( client_area );
	
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
	Widget_AutoSize( btn_close, FALSE, 0 );
	Resize_Widget(btn_close, Size(btn_normal.width, btn_normal.height));
	Custom_Button_Style(btn_close, &btn_normal, &btn_highlight, &btn_down, NULL, NULL);
	/* 关联按钮的点击事件，当按钮被点击后，调用Quit_Window函数 */
	Widget_Clicked_Event_Connect(btn_close, Quit_Parent_Window, NULL);
	/* 释放图形数据 */
	Graph_Free(&btn_highlight);
	Graph_Free(&btn_down);
	Graph_Free(&btn_normal);
	/* 保存部件指针 */
	win->client_area = client_area;
	win->titlebar = titlebar;
	win->btn_close = btn_close;
	/* 没有背景图就填充背景色 */
	Set_Widget_BG_Mode(win_p, BG_MODE_FILL_BACKCOLOR);
	/* 部件的风格ID */
	win_p->style_id = WINDOW_STYLE_PURE_BLUE;
	
	/* 放入至容器 */
	Widget_Container_Add(titlebar, btn_close);
	Widget_Container_Add(win_p, titlebar);
	Widget_Container_Add(win_p, client_area);
	/* 窗口初始尺寸 */
	Resize_Widget(win_p, Size(100, 50));
	Show_Widget(btn_close);
	/* 关联拖动事件，让鼠标能够拖动标题栏并使窗口移动 */
	Widget_Drag_Event_Connect(titlebar, Move_Window); 
	/* 
	 * 由于需要在窗口获得/失去焦点时进行相关处理，因此需要将回调函数 与部件
	 * 的FOCUS_IN和FOCUS_OUT事件 进行关联
	 * */
	Widget_FocusOut_Event_Connect( win_p, Window_FocusOut, NULL );
	Widget_FocusIn_Event_Connect( win_p, Window_FocusIn, NULL );
}

static void 
Show_Window(LCUI_Widget *win_p)
/* 功能：在窗口显示时，进行相关处理 */
{
	LCUI_Size ctnr_size, size;
	LCUI_Pos pos;
	LCUI_Window *win;
	
	win = Get_Widget_PrivData( win_p );
	win->count++;
	/* 如果是第一次显示 */
	if(win->count == 1) {
		ctnr_size = _Get_Widget_Container_Size( win_p );
		size = _Get_Widget_Size( win_p );
		pos = Align_Get_Pos( ctnr_size, size, win->init_align );
		Exec_Move_Widget( win_p, pos );
	}
	//有待扩展 
}

static void 
Hide_Window(LCUI_Widget *win_p)
/* 功能：在隐藏窗口时使用视觉特效 */
{
	//有待扩展
}

void 
Set_Window_Title_Text(LCUI_Widget *win_p, const char *text)
/* 功能：为窗口设置标题文字 */
{ 
	LCUI_Widget *titlebar = Get_Window_TitleBar(win_p); 
	LCUI_TitleBar *title = Get_Widget_PrivData(titlebar); 
	Label_Text(title->label, text);
}

void 
Window_Client_Area_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* 功能：将部件添加至窗口客户区 */
{
	LCUI_Widget *w = Get_Window_Client_Area(window);
	Widget_Container_Add(w, widget);
}

void 
Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* 功能：将部件添加至窗口标题栏 */
{
	LCUI_Widget *w = Get_Window_TitleBar(window);
	Widget_Container_Add(w, widget);
}

void 
Register_Window()
/* 注册窗口部件类型 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("window");
	WidgetType_Add("titlebar");
	
	/* 为部件类型关联相关函数 */ 
	WidgetFunc_Add("titlebar", Window_TitleBar_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("window", Window_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("window", Exec_Update_Window, FUNC_TYPE_UPDATE);
	WidgetFunc_Add("window", Exec_Update_Window, FUNC_TYPE_DRAW);
	WidgetFunc_Add("window", Show_Window, FUNC_TYPE_SHOW);
	WidgetFunc_Add("window", Hide_Window, FUNC_TYPE_HIDE);
	WidgetFunc_Add("window", Destroy_Window, FUNC_TYPE_DESTROY);
}

