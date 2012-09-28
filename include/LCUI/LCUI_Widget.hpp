/* ***************************************************************************
 * LCUI_Widget.hpp -- C++ class of GUI widget 
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

#include LC_GRAPH_H

class LCUIWidget
{
	public: 
	LCUIWidget(const char* widget_type);
	~LCUIWidget();
	int isError();
	LCUI_Widget *getWidget();
	int resize(LCUI_Size new_size);
	int move(LCUI_Pos new_pos);
	int show();
	int hide();
	int refresh();
	int draw();
	int enable();
	int disable();
	int setAlpha(unsigned char alpha);
	int setAlign(LCUI_Align align, LCUI_Pos offset_pos);
	int setBorder(LCUI_RGB color, LCUI_Border border);
	int setBackgroundImage(LCUI_Graph *img, int flag);
	//int connectDragEvent(void (*func)(LCUIWidget &obj, LCUI_DragEvent *event));
	//int connectClickedEvent(void (*func)(LCUIWidget &obj, LCUI_DragEvent *event));
	/* window widget class */
	class _Window
	{
		public: 
		_Window( LCUIWidget *object );
		int setTitle( const char *text );
		int setIcon( LCUIGraph &icon );
		int setIcon( LCUI_Graph *icon );
		int addToClientArea( LCUIWidget &object );
		
		private:
		LCUIWidget *parent; 
	}; 
	/* window widget class --end-- */
	
	/* label widget class */
	class _Label
	{
		public: 
		_Label(LCUIWidget *object);
		int setText(const char *fmt, ...);
		int setFont(int font_pixel_size, char *fontfile);
		
		private:
		LCUIWidget *parent; 
	};
	/* label widget class --end-- */
	_Label Label;
	_Window Window; 
	
	private:
	int error;
	LCUI_Widget **widget;
	
};


#include LC_WINDOW_H
#include LC_LABEL_H


LCUIWidget::LCUIWidget(const char* widget_type):Label(this),Window(this)
{
	error = 0;
	widget = new (LCUI_Widget *);
	*widget = Create_Widget(widget_type);
	if(*widget == NULL){
		error = 1;
	}
}

LCUIWidget::~LCUIWidget()
{
	delete this->widget;
}

int LCUIWidget::isError()
{
	return error;
}

LCUI_Widget *LCUIWidget::getWidget()
{
	return *widget;
}

int LCUIWidget::resize(LCUI_Size new_size)
{
	if( isError() ) return WIDGET_IS_NULL;
	Resize_Widget( getWidget() , new_size );
	return 0;
}

int LCUIWidget::move(LCUI_Pos new_pos)
{
	if( isError() ) return WIDGET_IS_NULL;
	Move_Widget( getWidget() , new_pos );
	return 0;
}

int LCUIWidget::show()
{
	if( isError() ) return WIDGET_IS_NULL;
	Show_Widget( getWidget() );
	return 0;
}

int LCUIWidget::hide()
{
	if( isError() ) return WIDGET_IS_NULL;
	Hide_Widget( getWidget() );
	return 0;
}

int LCUIWidget::refresh()
{
	if( isError() ) return WIDGET_IS_NULL;
	Refresh_Widget( getWidget() );
	return 0;
}

int LCUIWidget::draw()
{
	if( isError() ) return WIDGET_IS_NULL;
	Draw_Widget( getWidget() );
	return 0;
}

int LCUIWidget::enable()
{
	if( isError() ) return WIDGET_IS_NULL;
	Enable_Widget( getWidget() );
	return 0;
}

int LCUIWidget::disable()
{
	if( isError() ) return WIDGET_IS_NULL;
	Disable_Widget( getWidget() );
	return 0;
}

int LCUIWidget::setAlpha(unsigned char alpha)
{
	if( isError() ) return WIDGET_IS_NULL;
	Set_Widget_Alpha( getWidget() , alpha);
	return 0;
}
int LCUIWidget::setAlign(LCUI_Align align, LCUI_Pos offset_pos)
{
	if( isError() ) return WIDGET_IS_NULL; 
	Set_Widget_Align( getWidget() , align, offset_pos);
	return 0;
}
int LCUIWidget::setBorder(LCUI_RGB color, LCUI_Border border)
{
	if( isError() ) return WIDGET_IS_NULL;
	Set_Widget_Border( getWidget() , color, border);
	return 0;
}
int LCUIWidget::setBackgroundImage(LCUI_Graph *img, int flag)
{
	if( isError() ) return WIDGET_IS_NULL;
	return Set_Widget_Background_Image( getWidget() , img, flag); 
}

#endif

#endif
