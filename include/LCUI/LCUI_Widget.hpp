/* ***************************************************************************
 * LCUI_Widget.hpp -- C++ class for GUI widget 
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
 * LCUI_Widget.hp -- GUI部件的C++类
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

#ifndef __LCUI_WIDGET_HPP__
#define __LCUI_WIDGET_HPP__

#ifdef __cplusplus

#define WIDGET_IS_NULL -5

class LCUIWidget
{
public: 
	LCUIWidget( const char* widget_type );
	~LCUIWidget();
	
	LCUI_Widget *getWidget( void );
	
	int resize( LCUI_Size new_size );
	int move( LCUI_Pos new_pos );
	int show( void );
	int hide( void );
	int refresh( void );
	int draw( void );
	int enable( void );
	int disable( void );
	
	int cancelFocus( void );
	int resetFocus( void );

	int setFocus( void );
	int setAlpha( unsigned char alpha );
	int setAlign( ALIGN_TYPE align, LCUI_Pos offset_pos );
	int setDock( DOCK_TYPE dock );
	int setBorder( LCUI_RGB color, LCUI_Border border );
	int setBackgroundImage( LCUI_Graph *img, int flag );
	int setStyle( char *style );
	int setStyleID( int style_id );

	int connectDragEvent( void (*func)(LCUI_Widget*, LCUI_DragEvent *) );
	int connectClickedEvent( void (*func)(LCUI_Widget*, void *), void *arg );
	int connectKeyboardEvent( void (*func)(LCUI_Widget*, LCUI_Key *) );
	int connectFocusInEvent( void (*func)(LCUI_Widget*, void*), void *arg );
	int connectFocusOutEvent( void (*func)(LCUI_Widget*, void*), void *arg );

	int addChildWidget( LCUIWidget &child_widget );

private:
	LCUI_Widget *widget;
};

LCUIWidget::LCUIWidget( const char* widget_type )
{
	widget = Create_Widget( widget_type );
}

LCUIWidget::~LCUIWidget( void )
{
	//
}


LCUI_Widget* LCUIWidget::getWidget( void )
{
	return widget;
}

int LCUIWidget::resize( LCUI_Size new_size )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Resize_Widget( widget , new_size );
	return 0;
}

int LCUIWidget::move( LCUI_Pos new_pos )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Move_Widget( widget , new_pos );
	return 0;
}

int LCUIWidget::show( void )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Show_Widget( widget );
	return 0;
}

int LCUIWidget::hide( void )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Hide_Widget( widget );
	return 0;
}

int LCUIWidget::refresh( void )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Refresh_Widget( widget );
	return 0;
}

int LCUIWidget::draw( void )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Draw_Widget( widget );
	return 0;
}

int LCUIWidget::enable( void )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Enable_Widget( widget );
	return 0;
}

int LCUIWidget::disable( void )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Disable_Widget( widget );
	return 0;
}

int LCUIWidget::setAlpha( unsigned char alpha )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Set_Widget_Alpha( widget , alpha );
	return 0;
}
int LCUIWidget::setAlign( ALIGN_TYPE align, LCUI_Pos offset_pos )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Set_Widget_Align( widget , align, offset_pos );
	return 0;
}
int LCUIWidget::setBorder( LCUI_RGB color, LCUI_Border border )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Set_Widget_Border( widget , color, border );
	return 0;
}
int LCUIWidget::setBackgroundImage( LCUI_Graph *img, int flag )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Set_Widget_Background_Image( widget , img, flag ); 
}

int LCUIWidget::setFocus( void ) 
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Set_Focus( widget );
}

int LCUIWidget::setDock( DOCK_TYPE dock )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Set_Widget_Dock( widget, dock );
	return 0;
}

int LCUIWidget::connectDragEvent( void (*func)(LCUI_Widget*, LCUI_DragEvent *) )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Widget_Drag_Event_Connect( widget, func );
}

int LCUIWidget::connectClickedEvent( void (*func)(LCUI_Widget*, void *), void *arg )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Widget_Clicked_Event_Connect( widget, func, arg );
}

int LCUIWidget::connectKeyboardEvent( void (*func)(LCUI_Widget*, LCUI_Key *) )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Widget_Keyboard_Event_Connect( widget, func );
}

int LCUIWidget::connectFocusInEvent( void (*func)(LCUI_Widget*, void*), void *arg )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Widget_FocusIn_Event_Connect( widget, func, arg );
}

int LCUIWidget::connectFocusOutEvent( void (*func)(LCUI_Widget*, void*), void *arg )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	return Widget_FocusOut_Event_Connect( widget, func, arg );
}

int LCUIWidget::addChildWidget( LCUIWidget &child_widget )
{
	if( !widget ) {
		return WIDGET_IS_NULL;
	}
	Widget_Container_Add( widget, child_widget.getWidget() );
	return 0;
}

#endif

#endif
