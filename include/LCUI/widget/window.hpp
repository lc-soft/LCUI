/* ***************************************************************************
 * LCUI_Window.h -- C++ class and function for window widget
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
 * LCUI_Window.h -- 窗口部件的C++类和函数
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
#ifndef __LCUI_WINDOW_HPP_
#define __LCUI_WINDOW_HPP_ 

/* LCUIWindow类继承自LCUIWidget类 */
class LCUIWindow:public LCUIWidget
{
public:
	LCUIWindow():LCUIWidget("window"){};
	void setTitleIcon( LCUI_Graph* );
	void setTitleIcon( LCUIGraph & );
	void setTitleText( const char* );
	void addToTitleBar( LCUIWidget& );
	void addToClientArea( LCUIWidget& );
};

void LCUIWindow::setTitleIcon( LCUI_Graph *icon )
{
	Set_Window_Title_Icon( getWidget(), icon );
}

void LCUIWindow::setTitleIcon( LCUIGraph &icon )
{
	Set_Window_Title_Icon( getWidget(), icon.getGraph() );
}

void LCUIWindow::setTitleText( const char *text )
{
	Set_Window_Title_Text( getWidget(), text );
}

void LCUIWindow::addToTitleBar( LCUIWidget &obj )
{
	Window_TitleBar_Add( getWidget(), obj.getWidget() );
}

void LCUIWindow::addToClientArea( LCUIWidget &obj )
{
	Window_Client_Area_Add( getWidget(), obj.getWidget() );
}
#endif





