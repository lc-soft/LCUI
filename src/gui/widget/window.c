/* ***************************************************************************
 * window.c -- LCUI's window widget
 * 
 * Copyright (C) 2012-2014 by
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
 * 版权所有 (C) 2012-2014 归属于
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
#include LC_INPUT_H
#include LC_FONT_H
#include LC_ERROR_H

#define DEFAULT_THEME_COLOR	ARGB(150,34,177,76)
#define TOP_PANDDING		2
#define WIDGET_CLOSE_BUTTON	"LCUI::WindowCloseButton"
#define ALLOW_STATE_LIST	(WIDGET_STATE_ACTIVE|WIDGET_STATE_NORMAL\
				 |WIDGET_STATE_OVERLAY|WIDGET_STATE_DISABLE)

typedef struct LCUI_WindowRec_ {
	LCUI_Widget *titlebar;		/**< 标题栏 */
	LCUI_Widget *client_area;	/**< 客户区 */
	LCUI_Widget *btn_close;		/**< close按钮 */
	LCUI_Widget *icon;		/**< 图标 */
	LCUI_Widget *text;		/**< 标题文本 */
	LCUI_Color theme_color;	/**< 顶部线条颜色 */
} LCUI_Window;

static unsigned char close_btn_alpha[]={
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,
	0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
	0x00,0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,0x00,
	0x00,0x00,0xff,0xff,0xff,0x00,0xff,0xff,0x00,0x00,
	0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
	0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,0x00,
	0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00
};

static void LoadCloseButtonIMG( LCUI_Graph *graph )
{
	graph->color_type = COLOR_TYPE_ARGB;
	Graph_Create( graph, 10, 10 );
	Graph_FillColor( graph, RGB(255,255,255) );
	Graph_SetAlphaBits( graph, close_btn_alpha, 10*10 );
}

/** 在窗口失去焦点时会调用此函数 */
static void Window_OnFocusOut( LCUI_Widget *window, LCUI_WidgetEvent *unused )
{
	Widget_Update( window );
}

/** 在窗口获得焦点时会调用此函数 */
static void Window_OnFocusIn( LCUI_Widget *window, LCUI_WidgetEvent *unused )
{
	//_DEBUG_MSG( "%p, Window_FocusIn!\n",window );
	Widget_Front( window );
	Widget_Update( window );
}

static void Window_OnShow( LCUI_Widget *widget )
{
	Widget_SetFocus( widget );
}

/* 处理鼠标移动事件 */
static void Window_ExecMove( LCUI_Widget *window, LCUI_WidgetEvent *event )
{
	LCUI_Pos pos;
	/* 解除之前设定的align */
	Widget_SetAlign( window, ALIGN_NONE, Pos(0,0) );
	pos = Widget_ToRelPos( window->parent, event->drag.new_pos );
	/* 移动窗口的位置 */
	Widget_Move( window, pos );
}

/** 初始化window部件相关数据 */
static void Window_OnInit( LCUI_Widget *window )
{
	LCUI_Window *wnd;
	LCUI_Graph btn_bg;
	LCUI_TextStyle style;
	LCUI_Border border;
	
	wnd = Widget_NewPrivateData( window, LCUI_Window );

	wnd->theme_color = DEFAULT_THEME_COLOR;
	wnd->titlebar = Widget_New(NULL); 
	wnd->client_area = Widget_New(NULL); 
	wnd->btn_close = Widget_New(WIDGET_CLOSE_BUTTON); 
	wnd->icon = Widget_New(NULL);
	wnd->text = Label_New();

	wnd->titlebar->focus = FALSE;
	wnd->icon->focus = FALSE;
	wnd->text->focus = FALSE;
	Widget_SetFocus( wnd->client_area );
	Widget_SetClickable( wnd->icon, FALSE );
	Widget_SetClickable( wnd->text, FALSE );
	Widget_SetClickable( wnd->titlebar, FALSE );
	Widget_SetClickable( wnd->client_area, FALSE );

	Graph_Init( &btn_bg );
	/* 载入按钮背景 */
	LoadCloseButtonIMG( &btn_bg );
	/* close按钮显示在左上角 */
	Widget_SetAlign( wnd->btn_close, ALIGN_TOP_RIGHT, Pos(0,0) );
	Widget_Resize( wnd->btn_close, Size(30, 30) );

	Widget_SetBackgroundImage( wnd->btn_close, &btn_bg );
	Widget_SetBackgroundLayout( wnd->btn_close, LAYOUT_CENTER );
	Widget_SetBackgroundTransparent( wnd->icon, TRUE );
	Widget_SetBackgroundTransparent( wnd->titlebar, TRUE );
	Widget_SetBackgroundTransparent( wnd->client_area, FALSE );
	Widget_SetBackgroundTransparent( window, FALSE );
	Widget_SetBackgroundColor( window, RGB(255,255,255) );
	Widget_SetBackgroundLayout( wnd->icon, LAYOUT_ZOOM );
	Widget_SetDock( wnd->titlebar, DOCK_TYPE_TOP );
	Widget_SetDock( wnd->client_area, DOCK_TYPE_BOTTOM );
	Widget_SetPadding( window, Padding(TOP_PANDDING,1,1,1) );
	Widget_SetPadding( wnd->client_area, Padding(1,1,1,1) );
	Widget_SetSize( wnd->titlebar, "100%", "35px" );
	
	Border_Init( &border );
	border.top_width = 0;
	border.bottom_width = 1;
	border.left_width = 1;
	border.right_width = 1;
	border.bottom_color = RGB(200,200,200);
	border.left_color = RGB(255,255,255);
	border.right_color = RGB(255,255,255);
	Widget_SetBorder( wnd->titlebar, border );

	TextStyle_Init( &style );
	TextStyle_FontSize( &style, 18 );
	Label_SetTextStyle( wnd->text, style );

	/* 放入至容器 */
	Widget_Container_Add( wnd->titlebar, wnd->icon );
	Widget_Container_Add( wnd->titlebar, wnd->text );
	Widget_Container_Add( wnd->titlebar, wnd->btn_close );
	Widget_Container_Add( window, wnd->titlebar );
	Widget_Container_Add( window, wnd->client_area );

	Widget_Resize( window, Size(100, 50) );
	Widget_Resize( wnd->icon, Size(24,24) );
	
	Widget_SetAlign( window, ALIGN_MIDDLE_CENTER, Pos(0,0) );
	Widget_SetAlign( wnd->icon, ALIGN_MIDDLE_LEFT, Pos(6,0) );
	Widget_SetAlign( wnd->text, ALIGN_MIDDLE_LEFT, Pos(6,0) );

	/* 关联拖动事件，让鼠标能够拖动标题栏并使窗口移动 */
	Widget_ConnectEvent( window, EVENT_DRAG, Window_ExecMove );
	/* 
	 * 由于需要在窗口获得/失去焦点时进行相关处理，因此需要将回调函数 与部件
	 * 的FOCUS_IN和FOCUS_OUT事件 进行关联
	 * */
	Widget_ConnectEvent( window, EVENT_FOCUSOUT, Window_OnFocusOut );
	Widget_ConnectEvent( window, EVENT_FOCUSIN, Window_OnFocusIn );
	
	Widget_Show( wnd->btn_close );
	Widget_Show( wnd->icon );
	Widget_Show( wnd->text );
	Widget_Show( wnd->titlebar );
	Widget_Show( wnd->client_area );
}

static void Window_OnUpdate( LCUI_Widget *window )
{
	LCUI_Window *wnd;
	LCUI_Border border;
	char h_str[20];

	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	border = Border( 1, BORDER_STYLE_SOLID, RGB(200,200,200) );
	border.top_width = TOP_PANDDING;
	/* 若窗口已获得焦点 */
	if( Widget_GetFocus( window ) ) {
		border.top_color = wnd->theme_color;
		border.bottom_color = wnd->theme_color;
		border.left_color = wnd->theme_color;
		border.right_color = wnd->theme_color;
		Widget_SetShadow( window, BoxShadow(0,0,8,wnd->theme_color) );
	} else {
		Widget_SetShadow( window, BoxShadow(0,0,8,ARGB(200,200,200,200)) );
	}
	Widget_SetBorder( window, border );
	/* 更新窗口标题栏上的关闭按钮 */
	Widget_Update( wnd->btn_close );
	sprintf( h_str, "%dpx", Widget_GetContainerHeight(window)-35 );
	Widget_SetSize( wnd->client_area, "100%", h_str );
}

/** 获取窗口的客户区 */
LCUI_API LCUI_Widget *Window_GetClientArea( LCUI_Widget *window )
{
	LCUI_Window *wnd;
	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	return wnd->client_area;
}

/** 获取窗口的close按钮 */
LCUI_API LCUI_Widget *Window_GetCloseButton( LCUI_Widget *window )
{
	LCUI_Window *wnd;
	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	return wnd->btn_close;
}

/** 将部件添加至窗口客户区内 */
LCUI_API void Window_ClientArea_Add( LCUI_Widget *window, LCUI_Widget *w )
{
	LCUI_Window *wnd;
	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	Widget_Container_Add( wnd->client_area, w );
}

/** 设置窗口标题栏中显示的文本 */
LCUI_API int Window_SetTextW( LCUI_Widget *window, const wchar_t *text )
{
	LCUI_Window *wnd;
	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	return Label_SetTextW( wnd->text, text );
}

/** 设置窗口的图标 */
LCUI_API void Window_SetIcon( LCUI_Widget *window, LCUI_Graph *icon )
{
	LCUI_Window *wnd;
	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	Widget_SetBackgroundImage( wnd->icon, icon );
	Widget_SetAlign( wnd->icon, ALIGN_MIDDLE_LEFT, Pos(6,0) );
	Widget_SetAlign( wnd->text, ALIGN_MIDDLE_LEFT, Pos(34,0) );
}

/** 设置窗口主题色 */
LCUI_API void Window_SetThemeColor( LCUI_Widget *window, LCUI_Color color )
{
	LCUI_Window *wnd;
	wnd = (LCUI_Window*)Widget_GetPrivateData( window );
	wnd->theme_color = color;
	Widget_Update( window );
}

static void CloseButton_OnInit( LCUI_Widget *widget )
{
	Widget_NewPrivateData( widget, LCUI_Button );
	widget->valid_state = ALLOW_STATE_LIST;

}

static void CloseButton_OnUpdate( LCUI_Widget *widget )
{
	LCUI_Graph img;
	LCUI_Color color;
	LCUI_Widget *wnd;

	Graph_Init( &img );
	LoadCloseButtonIMG( &img );
	
	wnd = Widget_GetParent( widget, WIDGET_WINDOW );
	if( wnd ) {
		/* 若所在窗口并未获得焦点 */
		if( !Widget_GetFocus(wnd) ) {
			color = RGB(200,200,200);
		} else {
			LCUI_Window *wnd_data;
			wnd_data = (LCUI_Window*)Widget_GetPrivateData( wnd );
			color = wnd_data->theme_color;
		}
	} else {
		color = RGB(255,0,0);
	}
	
	switch(widget->state) {
	case WIDGET_STATE_NORMAL:
		Graph_FillColor( &img, RGB(150,150,150) );
		Widget_SetBackgroundImage( widget, &img );
		Widget_SetBackgroundLayout( widget, LAYOUT_CENTER );
		Widget_SetBackgroundTransparent( widget, TRUE );
		break;
	case WIDGET_STATE_ACTIVE:
		color.red = _ALPHA_BLEND( 0, color.red, 100 );
		color.green = _ALPHA_BLEND( 0, color.green, 100 );
		color.blue = _ALPHA_BLEND( 0, color.blue, 100 );
		Graph_FillColor( &img, RGB(255,255,255) );
		Widget_SetBackgroundImage( widget, &img );
		Widget_SetBackgroundColor( widget, color );
		Widget_SetBackgroundLayout( widget, LAYOUT_CENTER );
		Widget_SetBackgroundTransparent( widget, FALSE );
		break;
	case WIDGET_STATE_OVERLAY:
		Graph_FillColor( &img, RGB(255,255,255) );
		Widget_SetBackgroundImage( widget, &img );
		Widget_SetBackgroundColor( widget, color );
		Widget_SetBackgroundLayout( widget, LAYOUT_CENTER );
		Widget_SetBackgroundTransparent( widget, FALSE );
	case WIDGET_STATE_DISABLE:
	default:break;
	}
	Graph_Free( &img );
}

void RegisterWindow(void)
{
	WidgetType_Add( WIDGET_WINDOW );
	WidgetFunc_Add( WIDGET_WINDOW, Window_OnShow, FUNC_TYPE_SHOW );
	WidgetFunc_Add( WIDGET_WINDOW, Window_OnInit, FUNC_TYPE_INIT );
	WidgetFunc_Add( WIDGET_WINDOW, Window_OnUpdate, FUNC_TYPE_UPDATE );

	WidgetType_Add( WIDGET_CLOSE_BUTTON );
	WidgetFunc_Add( WIDGET_CLOSE_BUTTON, CloseButton_OnInit, FUNC_TYPE_INIT );
	WidgetFunc_Add( WIDGET_CLOSE_BUTTON, CloseButton_OnUpdate, FUNC_TYPE_UPDATE );
}
