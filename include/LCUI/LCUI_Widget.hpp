/* ***************************************************************************
 * LCUI_Widget.hpp -- C++ class for GUI widget 
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
 * LCUI_Widget.hpp -- GUI部件的C++类
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

#ifndef __LCUI_WIDGET_HPP__
#define __LCUI_WIDGET_HPP__

#ifdef __cplusplus
class LCUIWidget {
public: 
	LCUIWidget( const char* widget_type );
	~LCUIWidget(void);

	LCUI_Widget *getWidget( void );
	LCUI_Size getSize( void );
	int getHeight( void );
	int getWidth( void );
	LCUI_Rect getRect( void );
	LCUI_Pos getPos( void );
	void *getPrivateData( void );
	int addInvalidArea( LCUI_Rect );
	int addInvalidArea( int, int, int, int );
	void show( LCUI_BOOL );
	int connect( WidgetEventType, void (*)(LCUI_Widget*, LCUI_WidgetEvent*) );
	void enable( LCUI_BOOL );
	void modal( LCUI_BOOL );
	void move( LCUI_Pos );
	void move( int , int );
	void resize( LCUI_Size );
	void resize( int, int );
	void dock( DOCK_TYPE );
	void update( LCUI_BOOL );
	void addChild( LCUI_Widget * );
	void addChild( LCUIWidget & );
	void getGraph( LCUI_Graph *, LCUI_Rect );
	LCUI_Graph* getSelfGraph( void );
	void syncInvalidArea( void );
	void setAlpha( unsigned char );
	void setPadding( int, int, int, int );
	void setPadding( int , int );
	void setPadding( int all );
	void setBorder( unsigned int, BORDER_STYLE, LCUI_RGB );
	void setBorderRadius( unsigned int );
	void setAlign( ALIGN_TYPE , LCUI_Pos );
	void setBackgroundImage( LCUI_Graph * );
	void setBackgroundLayout( LAYOUT_TYPE );
	void setBackgroundColor( LCUI_RGB );
	void setBackgroundTransparent( LCUI_BOOL );
	void destroy( void );
protected:
	LCUI_Widget *widget;
};

LCUIWidget::LCUIWidget( const char *widget_type )
{
	widget = Widget_New( widget_type );
}

LCUIWidget::~LCUIWidget(void)
{
	Widget_Destroy( widget );
}

LCUI_Widget *LCUIWidget::getWidget( void )
{
	return widget;
}

LCUI_Size LCUIWidget::getSize( void )
{
	return Widget_GetSize( widget );
}

int LCUIWidget::getHeight( void )
{
	return Widget_GetHeight( widget );
}

int LCUIWidget::getWidth( void )
{
	return Widget_GetWidth( widget );
}

LCUI_Rect LCUIWidget::getRect( void )
{
	return Widget_GetRect( widget );
}

LCUI_Pos LCUIWidget::getPos( void )
{
	return Widget_GetPos( widget );
}

void *LCUIWidget::getPrivateData( void )
{
	return Widget_GetPrivData( widget );
}

int LCUIWidget::addInvalidArea( LCUI_Rect area )
{
	return Widget_InvalidArea( widget, area );
}

int LCUIWidget::addInvalidArea( int x, int y, int w, int h )
{
	return Widget_InvalidArea( widget, Rect(x, y, w, h) );
}

void LCUIWidget::show( LCUI_BOOL need_show = TRUE )
{
	if(need_show) {
		Widget_Show( widget );
	} else {
		Widget_Hide( widget );
	}
}

void LCUIWidget::modal( LCUI_BOOL is_modal = TRUE )
{
	Widget_SetModal( widget, is_modal );
}

void LCUIWidget::dock( DOCK_TYPE dock_type )
{
	Widget_SetDock( widget, dock_type );
}

void LCUIWidget::move( LCUI_Pos new_pos )
{
	Widget_Move( widget, new_pos );
}

void LCUIWidget::move( int x, int y )
{
	Widget_Move( widget, Pos(x,y) );
}

void LCUIWidget::resize( LCUI_Size new_size )
{
	Widget_Resize( widget, new_size );
}

void LCUIWidget::resize( int w, int h )
{
	Widget_Resize( widget, Size(w,h) );
}

void LCUIWidget::update( LCUI_BOOL keep_new = FALSE )
{
	if( keep_new ) {
		Widget_Update( widget );
	} else {
		__Widget_Update( widget );
	}
}

void LCUIWidget::addChild( LCUI_Widget *child_widget )
{
	Widget_Container_Add( widget, child_widget );
}

void LCUIWidget::addChild( LCUIWidget &child_widget )
{
	Widget_Container_Add( widget, child_widget.getWidget() );
}

void LCUIWidget::getGraph( LCUI_Graph *graph_buff, LCUI_Rect rect )
{
	Widget_GetGraph( widget, graph_buff, rect );
}

LCUI_Graph* LCUIWidget::getSelfGraph( void )
{
	return Widget_GetSelfGraph( widget );
}

void LCUIWidget::syncInvalidArea( void )
{
	Widget_SyncInvalidArea( widget );
}

void LCUIWidget::setAlpha( unsigned char alpha )
{
	Widget_SetAlpha( widget, alpha );
}

void LCUIWidget::setPadding( int top, int bottom, int left, int right )
{
	Widget_SetPadding( widget, Padding(top, bottom, left, right) );
}

void LCUIWidget::setPadding( int top_bottom, int left_right )
{
	Widget_SetPadding( widget, Padding(top_bottom, top_bottom, left_right, left_right) );
}

void LCUIWidget::setPadding( int all )
{
	Widget_SetPadding( widget, Padding(all, all, all, all) );
}

void LCUIWidget::setBorder( unsigned int width_px, BORDER_STYLE style, LCUI_RGB color )
{
	Widget_SetBorder( widget, Border(width_px, style, color) );
}

void LCUIWidget::setBorderRadius( unsigned int radius )
{
	Widget_SetBorderRadius( widget, radius );
}

void LCUIWidget::setAlign( ALIGN_TYPE align, LCUI_Pos offset = Pos(0,0) )
{
	Widget_SetAlign( widget, align, offset );
}

void LCUIWidget::setBackgroundImage( LCUI_Graph *img )
{
	Widget_SetBackgroundImage( widget, img );
}

void LCUIWidget::setBackgroundLayout( LAYOUT_TYPE layout )
{
	Widget_SetBackgroundLayout( widget, layout );
}

void LCUIWidget::setBackgroundColor( LCUI_RGB color )
{
	Widget_SetBackgroundColor( widget, color );
}

void LCUIWidget::setBackgroundTransparent( LCUI_BOOL flag = TRUE )
{
	Widget_SetBackgroundTransparent( widget, flag );
}

int LCUIWidget::connect( WidgetEventType event_id, void (*func)(LCUI_Widget*, LCUI_WidgetEvent*) )
{
	return Widget_Event_Connect( widget, event_id, func );
}

void LCUIWidget::destroy( void )
{
	Widget_Destroy( widget );
}
#endif

#endif
