/* ***************************************************************************
 * LCUI_Button.c -- LCUI‘s Button widget
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
 * LCUI_Button.c -- LCUI 的按钮部件
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
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_MISC_H
#include LC_INPUT_H 
#include LC_MEM_H 

static void Exec_Update_Button(LCUI_Widget *widget)
/* 功能：更新按钮的图形数据 */
{
	LCUI_RGB color;
	DEBUG_MSG("Exec_Update_Button(): enter\n");
	LCUI_Button *button = (LCUI_Button *)Get_Widget_PrivData(widget);
	/* 根据按钮的不同风格来处理 */
	if(Strcmp(&widget->style, "custom") == 0) {
		int no_bitmap = 0;
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
		/* 判断按钮的状态，以选择相应的背景色 */
		switch(widget->status) {
		case WIDGET_STATUS_NORMAL:
			/* 如果已为按钮的normal状态设定有效的自定义图形 */
			if(Graph_Valid(&button->btn_normal)) {
				/* 缩放图像 */ 
				Graph_Zoom(&button->btn_normal, &widget->graph, 
						CUSTOM, widget->size);
				no_bitmap = 0;/* 标记有位图 */
			} else {
				no_bitmap = 1;/* 标记无位图 */
				/* 需填充的颜色，具体可用颜色选择器查看其它颜色的RGB值 */
				color = RGB(100, 150, 255); 
			}
			break;
		case WIDGET_STATUS_OVERLAY :
			if(Graph_Valid(&button->btn_over)) { 
				Graph_Zoom(&button->btn_over, &widget->graph, 
					CUSTOM, widget->size);
				no_bitmap = 0;
			} else {
				no_bitmap = 1;
				color = RGB(50, 180, 240); /* 浅蓝色 */
			}
			break;
		case WIDGET_STATUS_CLICKING :
			if(Graph_Valid(&button->btn_down)) {
				/* 缩放图像 */
				Graph_Zoom(&button->btn_down, &widget->graph, 
					CUSTOM, widget->size);
				no_bitmap = 0;
			} else {
				no_bitmap = 1;
				color = RGB(255, 50, 50); /* 红色 */
			}
			break;
		case WIDGET_STATUS_CLICKED :
			if(Graph_Valid(&button->btn_down)) {
				/* 缩放图像 */
				Graph_Zoom(&button->btn_down, &widget->graph, 
					CUSTOM, widget->size);
				no_bitmap = 0;
			} else {
				no_bitmap = 1;
				color = RGB(220, 220, 220); /* 灰色 */
			} 
			break;
		case WIDGET_STATUS_FOCUS :
			if(Graph_Valid(&button->btn_focus)) {
				/* 缩放图像 */
				Graph_Zoom(&button->btn_focus, &widget->graph, 
					CUSTOM, widget->size );
				no_bitmap = 0;
			} else {
				no_bitmap = 1;
				color = RGB(50, 50, 255); /* 蓝色 */
			} 
			break;
		case WIDGET_STATUS_DISABLE :
			if(Graph_Valid(&button->btn_disable)) {
				/* 缩放图像 */
				Graph_Zoom(&button->btn_disable, &widget->graph, 
					CUSTOM, widget->size );
				no_bitmap = 0;
			} else {
				no_bitmap = 1;
				color = RGB(190, 190, 190); /* 灰色 */
			} 
			break;
			default : break;
		}
		if(no_bitmap == 1) {/* 如果没有位图 */
			/* alpha通道的值改为255，不透明 */
			Graph_Fill_Alpha(&widget->graph, 255);
			/* 为部件填充背景图 */
			Graph_Fill_Image( &widget->graph, 
				&widget->background_image, 0, color);
			/* 然后绘制按钮边框，黑色的 */
			Graph_Draw_Border(&widget->graph, RGB(0,0,0), 
						Border(1, 1, 1, 1));
		}
	}
	else if(Strcmp(&widget->style, "menu_style") == 0){
		/* 菜单默认使用的按钮风格 */ 
		switch(widget->status) {
		case WIDGET_STATUS_NORMAL :
			Graph_Free(&widget->background_image);
			Graph_Fill_Alpha(&widget->graph, 0);  
			break;
		case WIDGET_STATUS_OVERLAY :
			color = RGB(80, 180, 240);
			Graph_Fill_Color(&widget->graph, color);
			Graph_Draw_Border(&widget->graph, RGB(50,50,255), 
						Border(1, 1, 1, 1));
			Graph_Fill_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_CLICKING :
			color = RGB(80, 170, 255);
			Graph_Fill_Color(&widget->graph, color);
			Graph_Draw_Border(&widget->graph, RGB(50,50,255), 
						Border(1, 1, 1, 1));
			Graph_Fill_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_CLICKED :
			color = RGB(80, 170, 255);
			Graph_Fill_Color(&widget->graph, color);
			Graph_Draw_Border(&widget->graph, RGB(50,50,255), 
						Border(1, 1, 1, 1));
			Graph_Fill_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_FOCUS :
			color = RGB(50, 50, 255);
			Graph_Fill_Alpha(&widget->graph, 255);
			break;
		case WIDGET_STATUS_DISABLE :
			Graph_Free(&widget->background_image);
			Graph_Fill_Alpha(&widget->graph, 0); 
			break;
			default :
			break;
		}
	} else {/* 如果按钮的风格为缺省 */ 
		Strcpy(&widget->style, "default");
		switch(widget->status) { 
		case WIDGET_STATUS_NORMAL :
			color = RGB(30, 145, 255); 
			break;
		case WIDGET_STATUS_OVERLAY :
			color = RGB(50, 180, 240);  
			break;
		case WIDGET_STATUS_CLICKING :
			color = RGB(255, 50, 50); 
			break;
		case WIDGET_STATUS_CLICKED :
			color = RGB(220, 220, 220); 
			break;
		case WIDGET_STATUS_FOCUS :
			color = RGB(50, 50, 255);
			break;
		case WIDGET_STATUS_DISABLE :
			color = RGB(190, 190, 190); 
			break;
			default : break;
		} 
		Graph_Fill_Alpha(&widget->graph, 255);
		Graph_Fill_Image(&widget->graph, 
			&widget->background_image, 0, color); 
		Graph_Draw_Border(&widget->graph, RGB(0,0,0), 
			Border(1, 1, 1, 1));
	}
	/* 按钮每次更新都需要更新整个按钮区域内的图形 */ 
	Refresh_Widget(widget); 
	DEBUG_MSG("Exec_Update_Button(): quit\n");
}


static void Button_Init(LCUI_Widget *widget)
/* 功能：初始化按钮部件的数据 */
{
	LCUI_Button *button = (LCUI_Button*)
		Widget_Create_PrivData(widget, sizeof(LCUI_Button));
	 
	/* 初始化图像数据 */ 
	Graph_Init(&button->btn_disable);
	Graph_Init(&button->btn_normal);
	Graph_Init(&button->btn_focus);
	Graph_Init(&button->btn_down);
	Graph_Init(&button->btn_over);
	 
	button->label = Create_Widget("label");/* 创建label部件 */ 
	
	Response_Status_Change(widget); /* 响应状态改变 */ 
	/* 将按钮部件作为label部件的容器 */
	Widget_Container_Add(widget, button->label);
	/* label部件居中显示 */
	Set_Widget_Align(button->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Show_Widget(button->label); /* 显示label部件 */
	Enable_Widget_Auto_Size(widget);/* 启用自动尺寸调整，以适应内容 */
}


static void Destroy_Button(LCUI_Widget *widget)
{
	LCUI_Button *button = (LCUI_Button*)Get_Widget_PrivData(widget);
	/* 释放图像数据占用的内存 */ 
	Graph_Free(&button->btn_disable);
	Graph_Free(&button->btn_normal);
	Graph_Free(&button->btn_focus);
	Graph_Free(&button->btn_down);
	Graph_Free(&button->btn_over);
}


LCUI_Widget *Get_Button_Label(LCUI_Widget *widget)
/* 功能：获取嵌套在按钮部件里的label部件 */
{
	LCUI_Button *button = (LCUI_Button*)Get_Widget_PrivData(widget);
	return button->label;
}

void Custom_Button_Style(	LCUI_Widget *widget, LCUI_Graph *normal, 
				LCUI_Graph *over, LCUI_Graph *down, 
				LCUI_Graph *focus, LCUI_Graph *disable)
/* 功能：自定义按钮在各种状态下显示的位图 */
{
	LCUI_Button *button = (LCUI_Button*)Get_Widget_PrivData(widget);
	/* 如果图形有效，就拷贝 */
	if(Graph_Valid(normal)) Graph_Copy(&button->btn_normal, normal);
	if(Graph_Valid(over)) Graph_Copy(&button->btn_over, over);
	if(Graph_Valid(down)) Graph_Copy(&button->btn_down, down);
	if(Graph_Valid(focus)) Graph_Copy(&button->btn_focus, focus);
	if(Graph_Valid(disable)) Graph_Copy(&button->btn_disable, disable);
	/* 设定为自定义风格 */
	Set_Widget_Style(widget, "custom");
	Draw_Widget(widget); /* 重新绘制部件 */
}

void Set_Button_Text(LCUI_Widget *widget, const char *fmt, ...)
/* 功能：设定按钮部件显示的文本内容 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Button *button = (LCUI_Button*)Get_Widget_PrivData(widget);
	LCUI_Widget *label = button->label;  
	
	memset(text, 0, sizeof(text));
	/* 由于是可变参数，让vsnprintf函数根据参数将字符串保存至text中 */
	va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap);
	/* 设定部件显示的文本 */
	Set_Label_Text(label, text);
}


LCUI_Widget *Create_Button_With_Text(const char *fmt, ...)
/* 功能：创建一个带文本内容的按钮 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *widget = Create_Widget("button");
	
	memset(text, 0, sizeof(text)); 
    
	va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap); 
	
	Set_Button_Text(widget, text);
	return widget;
}


void Register_Button()
/*功能：将按钮部件类型注册至部件库 */
{
	/* 添加部件类型 */
	WidgetType_Add("button");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("button", Button_Init,		FUNC_TYPE_INIT);
	WidgetFunc_Add("button", Exec_Update_Button,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("button", Destroy_Button,	FUNC_TYPE_DESTROY); 
}

