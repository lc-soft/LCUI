/* ***************************************************************************
 * window.c -- LCUI's window widget
 * 
 * Copyright (C) 2013 by
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
 * window.c -- LCUI 的窗口部件
 *
 * 版权所有 (C) 2013 归属于
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

LCUI_EXPORT(LCUI_Widget*)
Window_GetTitleBar(LCUI_Widget *window)
/* 功能：获取窗口标题栏的指针 */
{ 
	LCUI_Window *win_p; 
	win_p = (LCUI_Window *)Widget_GetPrivData(window); 
	if(win_p == NULL) {
		return NULL;
	}
	return win_p->titlebar;
}

LCUI_EXPORT(LCUI_Widget*)
Window_GetClientArea(LCUI_Widget *window)
/* 功能：获取窗口客户区的指针 */
{
	LCUI_Window *win_p;
	win_p = (LCUI_Window *)Widget_GetPrivData(window); 
	return win_p->client_area;	
}

/* 获取窗口右上角关闭按钮 */
LCUI_EXPORT(LCUI_Widget*)
Window_GetCloseButton( LCUI_Widget *window )
{
	LCUI_Window *win_p;
	win_p = Widget_GetPrivData( window );
	return win_p->btn_close;
}

static void Window_ExecShow( LCUI_Widget *widget )
{
	Set_Focus( widget );
}

/* 处理鼠标移动事件 */
static void 
Window_ExecMove(LCUI_Widget *titlebar, LCUI_WidgetEvent *event)
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
	pos = Widget_GetGlobalPos( titlebar );
	offset = Pos_Sub( event->drag.new_pos, pos );
	pos = Widget_GetGlobalPos( window );
	/* 将偏移坐标加在窗口全局坐标上，得出窗口的新全局坐标 */
	pos = Pos_Add( pos, offset );
	/* 转换成在容器区域内的相对坐标 */
	pos = GlobalPos_ConvTo_RelativePos( window, pos );
	/* 解除之前设定的align */
	Widget_SetAlign( window, ALIGN_NONE, Pos(0,0) );
	/* 移动窗口的位置 */
	Widget_Move( window, pos );
}

LCUI_EXPORT(void)
Window_SetTitleIcon(LCUI_Widget *window, LCUI_Graph *icon)
/* 功能：自定义指定窗口的标题栏图标 */
{
	LCUI_Graph *image;
	LCUI_Widget *title_widget;
	LCUI_TitleBar *title_data;
	
	title_widget = Window_GetTitleBar(window);
	title_data = (LCUI_TitleBar *)Widget_GetPrivData(title_widget);
	
	image = PictureBox_GetImage(title_data->icon_box);
	Graph_Free(image);/* 释放PictureBox部件内的图像占用的资源 */
	if(icon == NULL) {
		return;
	}
	/* 设置新图标 */
	PictureBox_SetImage(title_data->icon_box, icon);
	Widget_SetAlign(title_data->icon_box, ALIGN_MIDDLE_LEFT, Pos(3,0));
	Widget_SetAlign(title_data->label, ALIGN_MIDDLE_LEFT, Pos(23,0));
 
}

static void 
Window_TitleBar_Init(LCUI_Widget *titlebar)
/* 功能：初始化窗口标题栏 */
{
	LCUI_Graph img;
	LCUI_TitleBar *t;
	
	Graph_Init(&img);
	t = WidgetPrivData_New(titlebar, sizeof(LCUI_TitleBar));
	t->icon_box = Widget_New("picture_box");
	t->label = Widget_New("label");
	/* 窗口图标和标题文字不可获得焦点，并忽略鼠标点击 */
	t->label->focus = FALSE;
	t->icon_box->focus = FALSE;
	Set_Widget_ClickableAlpha( t->label, 0, 1 );
	Set_Widget_ClickableAlpha( t->icon_box, 0, 1 );
	
	Widget_Container_Add(titlebar, t->icon_box);
	Widget_Container_Add(titlebar, t->label);
	
	Widget_Resize(t->icon_box, Size(18,18));
	PictureBox_SetSizeMode(t->icon_box, SIZE_MODE_CENTER);
	
	Widget_Show(t->icon_box);
	Widget_Show(t->label);
	
	Widget_SetAlign(t->icon_box, ALIGN_MIDDLE_LEFT, Pos(0,0));
	Widget_SetAlign(t->label, ALIGN_MIDDLE_LEFT, Pos(2,-2));
	Load_Graph_Default_TitleBar_BG(&img);
	Widget_SetBackgroundImage( titlebar, &img );
	Widget_SetBackgroundLayout( titlebar, LAYOUT_STRETCH );
}

static void
Window_ExecUpdate( LCUI_Widget *win_p )
{
	LCUI_Size size;
	LCUI_Graph *graph;
	LCUI_Widget *titlebar;
	LCUI_Widget *client_area;
	LCUI_Border border;
	LCUI_RGB border_color, back_color;
	
	titlebar = Window_GetTitleBar(win_p);
	client_area = Window_GetClientArea(win_p);
	graph = Widget_GetSelfGraph( win_p );
	/* 按不同的风格来处理 */ 
	switch( win_p->style_id ) {
	    case WINDOW_STYLE_NONE:  /* 没有边框 */
		/* 先计算坐标和尺寸 */
		Widget_SetDock( client_area, DOCK_TYPE_FILL );
		Widget_Hide( titlebar );/* 隐藏标题栏 */
		Widget_Show( client_area );/* 客户区需要显示 */
		break;
			
	    case WINDOW_STYLE_LINE: /* 线条边框 */
		Widget_SetBorder(win_p,
		 Border(1, BORDER_STYLE_SOLID, RGB(50,50,50)));
		Widget_SetPadding( win_p, Padding(1,1,1,1) );
		Widget_SetDock( client_area, DOCK_TYPE_FILL );
		Widget_Hide( titlebar );
		Widget_Show( client_area );
		break;
		
	    case WINDOW_STYLE_STANDARD: /* 标准边框 */ 
		Widget_SetBorder(win_p,
		 Border(1, BORDER_STYLE_SOLID, RGB(50,50,50)));
		Widget_SetPadding(win_p, Padding(1,1,1,1)); 
		
		size = Widget_GetContainerSize( win_p );
		
		Widget_Resize(titlebar, Size(size.w, 25));
		Widget_Resize(client_area, Size(size.w, size.h - 25));
		/* 标题栏向顶部停靠 */
		Widget_SetDock( titlebar, DOCK_TYPE_TOP );
		/* 客户区向底部停靠 */
		Widget_SetDock( client_area, DOCK_TYPE_BOTTOM );
		/* 标题栏和客户区都需要显示 */
		Widget_Show( titlebar ); 
		Widget_Show( client_area ); 
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
		if( !Widget_GetFocus( win_p ) ) {
			back_color = RGB(255,255,255);
			border_color = RGB(50,50,50); 
		}
		border = Border(1, BORDER_STYLE_SOLID, border_color);
		Widget_SetBorder( client_area, border);
		Widget_SetBorder( win_p, border);
		Widget_SetBackgroundColor( win_p, back_color );
		Graph_FillColor( graph, back_color );
		Widget_SetBackgroundColor( client_area, RGB(255,255,255) );
		Widget_SetBackgroundImage( titlebar, NULL );
		Widget_SetBackgroundLayout( titlebar, 0 );
		Widget_SetBackgroundTransparent( titlebar, TRUE );
		Widget_SetBackgroundTransparent( client_area, FALSE );
		Widget_SetPadding( win_p, Padding(1,4,4,4) );
		Widget_SetPadding( client_area, Padding(1,1,1,1) );
		size = Widget_GetContainerSize( win_p );
		Widget_Resize( titlebar, Size(size.h, 25) );
		Widget_Resize( client_area, Size(size.w, size.h - 25) );
		Widget_SetDock( titlebar, DOCK_TYPE_TOP ); 
		Widget_SetDock( client_area, DOCK_TYPE_BOTTOM );
		Widget_Show( titlebar );
		Widget_Show( client_area ); 
		break;
	    default:
			//
		break;
	}
}

/* 在窗口失去焦点时会调用此函数 */
static void
Window_FocusOut( LCUI_Widget *window, LCUI_WidgetEvent *unused )
{
	//_DEBUG_MSG( "%p, Window_FocusOut!\n", window );
	Widget_Update( window );
}

/* 在窗口获得焦点时会调用此函数 */
static void
Window_FocusIn( LCUI_Widget *window, LCUI_WidgetEvent *unused )
{
	//_DEBUG_MSG( "%p, Window_FocusIn!\n",window );
	Widget_Front( window ); /* 前置窗口 */
	Widget_Update( window ); /* 更新窗口 */
}

/* 初始化window部件相关数据 */
static void 
Window_Init( LCUI_Widget *win_p )
{
	LCUI_Widget *titlebar;
	LCUI_Widget *client_area;
	LCUI_Widget *btn_close;
	LCUI_Window *win;
	static LCUI_Graph btn_highlight, btn_normal, btn_down; 
	
	win = WidgetPrivData_New(win_p, sizeof(LCUI_Window));
	
	titlebar = Widget_New("titlebar"); 
	client_area = Widget_New(NULL); 
	btn_close = Widget_New("button"); 
	titlebar->focus = FALSE;
	Set_Focus( client_area );
	Graph_Init(&btn_down);
	Graph_Init(&btn_highlight);
	Graph_Init(&btn_normal);
	/* 载入默认图形 */
	Load_Graph_Default_TitleBar_CloseBox_Normal(&btn_normal);
	Load_Graph_Default_TitleBar_CloseBox_Down(&btn_down);
	Load_Graph_Default_TitleBar_CloseBox_HighLight(&btn_highlight);
	/* 显示在左上角 */
	Widget_SetAlign(btn_close, ALIGN_TOP_RIGHT, Pos(0, -2)); 
	/* 将尺寸改成和图片一样 */
	Widget_SetAutoSize( btn_close, FALSE, 0 );
	Widget_Resize(btn_close, Size(btn_normal.width, btn_normal.height));
	Button_CustomStyle(btn_close, &btn_normal, &btn_highlight, &btn_down, NULL, NULL);
	/* 保存部件指针 */
	win->client_area = client_area;
	win->titlebar = titlebar;
	win->btn_close = btn_close;
	/* 没有背景图就填充背景色 */
	Widget_SetBackgroundTransparent( win_p, FALSE );
	/* 部件的风格ID */
	win_p->style_id = WINDOW_STYLE_PURE_BLUE;
	
	/* 放入至容器 */
	Widget_Container_Add(titlebar, btn_close);
	Widget_Container_Add(win_p, titlebar);
	Widget_Container_Add(win_p, client_area);
	/* 窗口初始尺寸 */
	Widget_Resize(win_p, Size(100, 50));
	Widget_Show(btn_close);
	/* 关联拖动事件，让鼠标能够拖动标题栏并使窗口移动 */
	Widget_Event_Connect(titlebar, EVENT_DRAG, Window_ExecMove );
	/* 
	 * 由于需要在窗口获得/失去焦点时进行相关处理，因此需要将回调函数 与部件
	 * 的FOCUS_IN和FOCUS_OUT事件 进行关联
	 * */
	Widget_Event_Connect( win_p, EVENT_FOCUSOUT, Window_FocusOut );
	Widget_Event_Connect( win_p, EVENT_FOCUSIN, Window_FocusIn );
	/* 设置窗口部件的初始位置 */
	Widget_SetAlign( win_p, ALIGN_MIDDLE_CENTER, Pos(0,0) );
}

LCUI_EXPORT(void)
Window_SetTitleText(LCUI_Widget *win_p, const char *text)
/* 功能：为窗口设置标题文字 */
{ 
	LCUI_Widget *titlebar = Window_GetTitleBar(win_p); 
	LCUI_TitleBar *title = Widget_GetPrivData(titlebar); 
	Label_Text(title->label, text);
}

LCUI_EXPORT(void)
Window_SetTitleTextW(LCUI_Widget *win_p, const wchar_t *text)
{ 
	LCUI_Widget *titlebar = Window_GetTitleBar(win_p); 
	LCUI_TitleBar *title = Widget_GetPrivData(titlebar); 
	Label_TextW(title->label, text);
}

LCUI_EXPORT(void)
Window_ClientArea_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* 功能：将部件添加至窗口客户区 */
{
	LCUI_Widget *w = Window_GetClientArea(window);
	Widget_Container_Add(w, widget);
}

LCUI_EXPORT(void)
Window_TitleBar_Add(LCUI_Widget *window, LCUI_Widget *widget)
/* 功能：将部件添加至窗口标题栏 */
{
	LCUI_Widget *w = Window_GetTitleBar(window);
	Widget_Container_Add(w, widget);
}


/* 新建一个窗口 */
LCUI_EXPORT(LCUI_Widget*)
Window_New( const char *title, LCUI_Graph *icon, LCUI_Size size )
{
	LCUI_Widget *wnd;
	wnd = Widget_New("window");
	if( wnd == NULL ) {
		return NULL;
	}
	Window_SetTitleText( wnd, title );
	Window_SetTitleIcon( wnd, icon );
	Widget_Resize( wnd, size );
	return wnd;
}

LCUI_EXPORT(void)
Register_Window()
/* 注册窗口部件类型 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("window");
	WidgetType_Add("titlebar");
	
	/* 为部件类型关联相关函数 */ 
	WidgetFunc_Add("titlebar", Window_TitleBar_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("window", Window_ExecShow, FUNC_TYPE_SHOW);
	WidgetFunc_Add("window", Window_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("window", Window_ExecUpdate, FUNC_TYPE_UPDATE);
}

