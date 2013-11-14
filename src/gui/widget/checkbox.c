/* ***************************************************************************
 * checkbox.c -- LCUI's CheckBox widget
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
 * checkbox.c -- LCUI 的复选框部件
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

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_CHECKBOX_H
#include LC_GRAPH_H
#include LC_RES_H

/* 设定复选框为选中状态 */
LCUI_API void
CheckBox_SetOn( LCUI_Widget *widget )
{
	LCUI_CheckBox *check_box;
	check_box = Widget_GetPrivData(widget); 
	check_box->on = TRUE;
	Widget_Draw( widget );
}

/* 设定复选框为未选中状态 */
LCUI_API void
CheckBox_SetOff( LCUI_Widget *widget )
{
	LCUI_CheckBox *check_box;
	check_box = Widget_GetPrivData(widget); 
	check_box->on = FALSE;
	Widget_Draw( widget );
}

/* 检测复选框是否被选中 */
LCUI_API int
CheckBox_IsOn( LCUI_Widget *widget )
{
	LCUI_CheckBox *check_box;
	check_box = Widget_GetPrivData(widget); 
	if( check_box->on ) {
		return TRUE;
	}
	return FALSE;
}

/* 检测复选框是否未选中 */
LCUI_API LCUI_BOOL
CheckBox_IsOff( LCUI_Widget *widget )
{
	LCUI_CheckBox *check_box;
	check_box = Widget_GetPrivData(widget); 
	if( check_box->on ) {
		return FALSE;
	}
	return TRUE;
}

/* 
 * 功能：切换复选框的状态
 * 说明：这个状态指的是打勾与没打勾的两种状态
 *  */
LCUI_API void
CheckBox_SwitchState( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{ 
	if(CheckBox_IsOn(widget)) {
		CheckBox_SetOff(widget);
	} else {
		CheckBox_SetOn(widget); 
	}
}

/* 设定复选框中的图像框的尺寸 */
LCUI_API void
CheckBox_ImgBox_SetSize( LCUI_Widget *widget, LCUI_Size size )
{
	LCUI_Widget *imgbox;
	if(size.w <= 0 && size.h <= 0) {
		return;
	}
		
	imgbox = CheckBox_GetImgBox(widget);
	Widget_Resize(imgbox, size);
	/* 由于没有布局盒子，不能自动调整部件间的间隔，暂时用这个方法 */
	Widget_SetAlign(imgbox->parent, ALIGN_MIDDLE_LEFT, Pos(size.w, 0));
}

/* 初始化复选框部件的数据 */
static void 
CheckBox_Init(LCUI_Widget *widget)
{
	int valid_state;
	LCUI_Widget *container[2];
	LCUI_CheckBox *check_box;
	
	check_box = Widget_NewPrivData(widget, sizeof(LCUI_CheckBox));
	
	check_box->on = FALSE;
	/* 初始化图像数据 */ 
	Graph_Init(&check_box->img_off_disable);
	Graph_Init(&check_box->img_off_normal);
	Graph_Init(&check_box->img_off_focus);
	Graph_Init(&check_box->img_off_down);
	Graph_Init(&check_box->img_off_over);
	Graph_Init(&check_box->img_on_disable);
	Graph_Init(&check_box->img_on_normal);
	Graph_Init(&check_box->img_on_focus);
	Graph_Init(&check_box->img_on_down);
	Graph_Init(&check_box->img_on_over);
	/* 创建所需的部件 */
	check_box->label = Widget_New("label");
	check_box->imgbox = Widget_New("picture_box");  
	/* 创建两个容器，用于调整上面两个部件的位置 */
	container[0] = Widget_New(NULL);
	container[1] = Widget_New(NULL);
	
	/* 启用这些部件的自动尺寸调整的功能 */
	Widget_SetAutoSize( widget, TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK );
	Widget_SetAutoSize( container[0], TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK );
	Widget_SetAutoSize( container[1], TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK );
	
	Widget_Container_Add(container[0], check_box->imgbox);
	Widget_Container_Add(container[1], check_box->label);
	Widget_Container_Add(widget, container[0]);
	Widget_Container_Add(widget, container[1]);
	
	/* 调整尺寸 */
	Widget_Resize(check_box->imgbox, Size(15, 15));
	//Widget_Resize(container[0], Size(18,18));
	//Widget_Resize(widget, Size(18,18));
	/* 调整布局 */
	Widget_SetAlign(container[0], ALIGN_MIDDLE_LEFT, Pos(0,0));
	Widget_SetAlign(container[1], ALIGN_MIDDLE_LEFT, Pos(17,0));
	Widget_SetAlign(check_box->imgbox, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_SetAlign(check_box->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	/* 设置图像框的尺寸模式为拉伸 */
	PictureBox_SetSizeMode(check_box->imgbox, SIZE_MODE_STRETCH);
	
	/* 显示之 */
	Widget_Show(check_box->label);
	Widget_Show(check_box->imgbox);
	Widget_Show(container[0]);
	Widget_Show(container[1]);
	/* 关联鼠标左键点击事件 */
	Widget_ConnectEvent( widget, EVENT_CLICKED, CheckBox_SwitchState );
	/* 响应状态改变 */
	valid_state = (WIDGET_STATE_NORMAL | WIDGET_STATE_ACTIVE);
	valid_state |= (WIDGET_STATE_DISABLE | WIDGET_STATE_OVERLAY);
	Widget_SetValidState( widget, valid_state );
}

static void 
Destroy_CheckBox(LCUI_Widget *widget)
/* 释放复选框部件占用的资源 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Widget_GetPrivData(widget); 
	/* 释放图像数据占用的内存资源 */ 
	Graph_Free(&check_box->img_off_disable);
	Graph_Free(&check_box->img_off_normal);
	Graph_Free(&check_box->img_off_focus);
	Graph_Free(&check_box->img_off_down);
	Graph_Free(&check_box->img_off_over);
	Graph_Free(&check_box->img_on_disable);
	Graph_Free(&check_box->img_on_normal);
	Graph_Free(&check_box->img_on_focus);
	Graph_Free(&check_box->img_on_down);
	Graph_Free(&check_box->img_on_over);
}

/* 更新复选框的图形数据 */
static void 
CheckBox_ExecDraw(LCUI_Widget *widget)
{
	LCUI_Graph *p;
	LCUI_CheckBox *check_box;
	
	check_box = Widget_GetPrivData(widget); 
								
	if(_LCUIString_Cmp(&widget->style_name, "custom") == 0) {
		/* 如果为自定义风格，那就使用用户指定的图形，具体可参考按钮部件的处理方法 */ 
		if( !widget->enabled ) {
			widget->state = WIDGET_STATE_DISABLE;
		}
		switch(widget->state){
		case WIDGET_STATE_NORMAL:
			if( check_box->on ) {
				p = &check_box->img_on_normal;
			} else {
				p = &check_box->img_off_normal;
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
		case WIDGET_STATE_OVERLAY :
			if( check_box->on ) {
				p = &check_box->img_on_over;
			} else {
				p = &check_box->img_off_over;
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
		case WIDGET_STATE_ACTIVE : 
			if( check_box->on ) {
				p = &check_box->img_on_down;
			} else {
				p = &check_box->img_off_down;
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
		case WIDGET_STATE_DISABLE :
			if( check_box->on ) {
				p = &check_box->img_on_disable;
			} else {
				p = &check_box->img_off_disable;
			}
			PictureBox_SetImage(check_box->imgbox, p); 
			break;
			default :
			break;
		} 
	} else {/* 如果按钮的风格为缺省 */
		_LCUIString_Copy(&widget->style_name, "default");
		if( !widget->enabled ) {
			widget->state = WIDGET_STATE_DISABLE;
		}
		/* 先释放PictureBox部件中保存的图形数据的指针 */
		p = PictureBox_GetImage(check_box->imgbox);
		Graph_Free(p);
		
		/* 由于本函数在退出后，使用局部变量保存的图形数据会无效，因此，申请内存空间来储存 */
		p = (LCUI_Graph*)calloc(1,sizeof(LCUI_Graph));
		
		switch(widget->state) { 
		case WIDGET_STATE_NORMAL:
			if( check_box->on ) {
				Load_Graph_Default_CheckBox_On_Normal(p);
			} else {
				Load_Graph_Default_CheckBox_Off_Normal(p);
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
		case WIDGET_STATE_OVERLAY :
			if( check_box->on ) {
				Load_Graph_Default_CheckBox_On_Selected(p);
			} else {
				Load_Graph_Default_CheckBox_Off_Selected(p);
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
		case WIDGET_STATE_ACTIVE : 
			if( check_box->on ) {
				Load_Graph_Default_CheckBox_On_Selected(p);
			} else {
				Load_Graph_Default_CheckBox_Off_Selected(p);
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
		case WIDGET_STATE_DISABLE :
			if( check_box->on ) {
				Load_Graph_Default_CheckBox_On_Disabled(p);
			} else {
				Load_Graph_Default_CheckBox_Off_Disabled(p);
			}
			PictureBox_SetImage(check_box->imgbox, p);
			break;
			default : break;
		}
	}
}

/* 获取复选框部件中的label部件的指针 */
LCUI_API LCUI_Widget*
CheckBox_GetLabel( LCUI_Widget *widget )
{
	LCUI_CheckBox *check_box;
	check_box = Widget_GetPrivData(widget); 
	if( !check_box ) {
		return NULL;
	}
	return check_box->label;
}

/* 获取复选框部件中的PictureBox部件的指针 */
LCUI_API LCUI_Widget*
CheckBox_GetImgBox( LCUI_Widget *widget )
{
	LCUI_CheckBox *check_box;
	check_box = Widget_GetPrivData(widget);
	if( !check_box ) {
		return NULL;
	}
	return check_box->imgbox;
}

/* 设定与复选框部件关联的文本内容 */
LCUI_API void
CheckBox_Text( LCUI_Widget *widget, const char *text )
{
	LCUI_Widget *label;
	label = CheckBox_GetLabel(widget);
	Label_Text(label, text); 
}

LCUI_API void
CheckBox_TextW( LCUI_Widget *widget, const wchar_t *text )
{
	LCUI_Widget *label;
	label = CheckBox_GetLabel(widget);
	Label_TextW(label, text); 
}

/* 创建一个带文本内容的复选框 */
LCUI_API LCUI_Widget*
CheckBox_New( const char *text )
{
	LCUI_Widget *widget;
	widget = Widget_New("check_box");
	CheckBox_Text(widget, text);
	return widget;
}

/* 注册复选框部件类型 */
LCUI_API void
Register_CheckBox(void)
{
	/* 添加几个部件类型 */
	WidgetType_Add("check_box"); 
	
	/* 为部件类型关联相关函数 */ 
	WidgetFunc_Add("check_box", CheckBox_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("check_box", CheckBox_ExecDraw, FUNC_TYPE_DRAW); 
	WidgetFunc_Add("check_box", Destroy_CheckBox, FUNC_TYPE_DESTROY);
}
