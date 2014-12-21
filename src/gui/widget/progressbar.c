/* ***************************************************************************
 * progressbar.c -- LCUI's ProgressBar widget
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
 * progressbar.c -- LCUI 的进度条部件
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
 
#ifdef ENABLE_THIS_MODULE
#include <LCUI_Build.h>
#include <LCUI/LCUI.h> 
#include <LCUI/graph.h>
#include <LCUI/widget.h>
#include LC_PROGBAR_H
#include LC_PICBOX_H 
#include LC_RES_H 

typedef LCUI_ProgressBar LCUIProgBar;

/* 释放进度条部件占用的内存资源 */
static void ProgressBar_ExecDestroy( LCUI_Widget widget )
{

}

/* 初始化进度条的数据结构体 */
static void 
ProgressBar_ExecInit( LCUI_Widget widget )
{
	LCUIProgBar *pb;
	LCUI_Widget fore_wdg, *glisten_wdg;
	
	pb = (LCUIProgBar*)Widget_NewPrivData(widget, sizeof(LCUIProgBar));
	pb->move_speed = 200;
	pb->sleep_time = 100;
	pb->max_value = 1;
	pb->value = 0;
	
	fore_wdg = Widget_New(NULL); 
	glisten_wdg = Widget_New(NULL);

	Widget_Container_Add( fore_wdg, glisten_wdg );
	Widget_Container_Add( widget, fore_wdg ); 
	
	Widget_SetStyleID( widget, PROGBAR_STYLE_GREEN );
	Widget_SetBackgroundTransparent( widget, FALSE );
	Widget_SetBackgroundTransparent( fore_wdg, FALSE );
	Widget_SetBackgroundColor( widget, RGB(230,230,230));
	Widget_SetBorder( widget, Border(1,BORDER_SOLID,RGB(188,188,188)) );
	Widget_SetPadding( widget, Padding(1,1,1,1) );
	Widget_SetZIndex( fore_wdg, -100 );
	Widget_Show( fore_wdg ); 
	Widget_Show( glisten_wdg );
	
	pb->fore_wdg = fore_wdg; 
	pb->glisten_wdg = glisten_wdg;
}

/* 更新进度条数据 */
static void 
ProgressBar_ExecUpdate( LCUI_Widget widget )
{
	double scale;
	char scale_str[15];
	LCUIProgBar *pb;
	
	pb = (LCUIProgBar*)Widget_GetPrivData(widget);
	/* 计算进度条的长度 */ 
	scale = 100.0 * pb->value / pb->max_value;
	sprintf( scale_str, "%.2lf%%", scale );
	/* 改变进度条的尺寸 */
	Widget_SetSize( pb->fore_wdg, scale_str, "100%" );
	switch( widget->style_id ) {
	case PROGBAR_STYLE_RED:
		Widget_SetBackgroundColor( pb->fore_wdg, RGB(218,38,38) );
		break;
	case PROGBAR_STYLE_BLUE:
		Widget_SetBackgroundColor( pb->fore_wdg, RGB(38,160,218) );
		break;
	case PROGBAR_STYLE_GREEN:
		Widget_SetBackgroundColor( pb->fore_wdg, RGB(6,176,37) );
		break;
	default:break;
	}
}

/* 设定进度条最大值 */
LCUI_API void ProgressBar_SetMaxValue(LCUI_Widget widget, int max_value)
{
	LCUIProgBar *pb;
	pb = (LCUIProgBar*)Widget_GetPrivData(widget);
	pb->max_value = max_value; 
	Widget_Update(widget); 
}

/* 获取进度条最大值 */
LCUI_API int ProgressBar_GetMaxValue( LCUI_Widget widget )
{
	LCUIProgBar *pb;
	pb = (LCUIProgBar*)Widget_GetPrivData(widget);
	return pb->max_value;
}

/* 设定进度条当前值 */
LCUI_API void ProgressBar_SetValue( LCUI_Widget widget, int value )
{
	LCUIProgBar *pb;
	pb = (LCUIProgBar*)Widget_GetPrivData(widget);
	pb->value = value; 
	Widget_Update(widget);
}

/* 获取进度条当前值 */
LCUI_API int ProgressBar_GetValue( LCUI_Widget widget )
{
	LCUIProgBar *pb;
	pb = (LCUIProgBar*)Widget_GetPrivData(widget);
	return pb->value;
}

/* 注册进度条部件类型 */
LCUI_API void Register_ProgressBar(void)
{
	/* 添加部件类型 */
	WidgetType_Add("progress_bar");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("progress_bar", ProgressBar_ExecInit,	FUNC_TYPE_INIT);
	WidgetFunc_Add("progress_bar", ProgressBar_ExecUpdate,	FUNC_TYPE_UPDATE);
	WidgetFunc_Add("progress_bar", ProgressBar_ExecDestroy,	FUNC_TYPE_DESTROY); 
}

#endif
