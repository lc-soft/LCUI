/* ***************************************************************************
 * radiobutton.c -- LCUI‘s RadioButton widget
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
 * radiobutton.c -- LCUI 的单选框部件
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
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_RADIOBTN_H
#include LC_GRAPH_H
#include LC_RES_H
#include LC_INPUT_H

static LCUI_Queue mutex_lib;
static int mutex_lib_init = 0; /* 标志，是否初始化过 */

static void __Destroy_MutexData( void *arg )
{
	LCUI_Queue *queue;
	queue = (LCUI_Queue*)arg;
	Queue_Destroy( queue );
}

/* 将单选框从互斥关系链中移除 */
LCUI_EXPORT(void)
RadioButton_DeleteMutex( LCUI_Widget *widget )
{
	int i, total;
	LCUI_Widget *tmp;
	LCUI_RadioButton *rb;
	
	rb = (LCUI_RadioButton *)Widget_GetPrivData(widget);
	total = Queue_GetTotal(rb->mutex);
	for(i=0; i<total; ++i) {
		tmp = (LCUI_Widget*)Queue_Get(rb->mutex, i);
		if(tmp == widget) {
			Queue_DeletePointer(rb->mutex, i);
			break;
		}
	}
}

/* 为两个单选框建立互斥关系 */
LCUI_EXPORT(void)
RadioButton_CreateMutex( LCUI_Widget *a, LCUI_Widget *b )
{
	int pos;
	LCUI_Queue *p, queue;
	LCUI_RadioButton *rb_a, *rb_b;
	if(mutex_lib_init == 0) {
		Queue_Init(&mutex_lib, sizeof(LCUI_Queue), __Destroy_MutexData);
		mutex_lib_init = 1;
	}
	
	rb_a = (LCUI_RadioButton *)Widget_GetPrivData(a);
	rb_b = (LCUI_RadioButton *)Widget_GetPrivData(b);
	
	if( !rb_a->mutex ) {
		if( !rb_b->mutex ) {
			Queue_Init(&queue, sizeof(LCUI_Widget*), NULL);
			/* 将子队列添加至父队列，并获取位置 */
			pos = Queue_Add(&mutex_lib, &queue);
			/* 从队列中获取指向子队列的指针 */
			p = (LCUI_Queue*)Queue_Get(&mutex_lib, pos);
			/* 添加指针至队列 */
			Queue_AddPointer(p, a);
			Queue_AddPointer(p, b);
			/* 保存指向关系队列的指针 */
			rb_a->mutex = p;
			rb_b->mutex = p;
		} else {
			Queue_AddPointer(rb_b->mutex, a);
			rb_a->mutex = rb_b->mutex;
		}
	} else {
		if( !rb_b->mutex ) {
			Queue_AddPointer(rb_a->mutex, b);
			rb_b->mutex = rb_a->mutex;
		} else {/* 否则，两个都和其它部件有互斥关系，需要将它们拆开，并重新建立互斥关系 */
			RadioButton_DeleteMutex(a);
			RadioButton_DeleteMutex(b);
			RadioButton_CreateMutex(a, b);
		}
	}
}

/* 设定单选框为选中状态 */
LCUI_EXPORT(void)
RadioButton_SetOn( LCUI_Widget *widget )
{
	LCUI_RadioButton *radio_button;
	LCUI_Widget *other;
	int i, total;
	
	radio_button = Widget_GetPrivData(widget); 
	/* 如果与其它部件有互斥关系，就将其它单选框部件的状态改为“未选中”状态 */
	if( radio_button->mutex ) {
		total = Queue_GetTotal(radio_button->mutex);
		for(i=0; i<total; ++i) {
			other = (LCUI_Widget*)Queue_Get(radio_button->mutex, i);
			RadioButton_SetOff(other);
		}
	}
	radio_button->on = TRUE;
	Widget_Draw(widget);
}

/* 设定单选框为未选中状态 */
LCUI_EXPORT(void)
RadioButton_SetOff( LCUI_Widget *widget )
{
	LCUI_RadioButton *radio_button;
	radio_button = Widget_GetPrivData(widget); 
	radio_button->on = FALSE;
	Widget_Draw(widget);
}

/* 检测单选框是否被选中 */
LCUI_EXPORT(LCUI_BOOL)
RadioButton_IsOn( LCUI_Widget *widget )
{
	LCUI_RadioButton *radio_button;
	radio_button = Widget_GetPrivData(widget); 
	return radio_button->on;
}

/* 检测单选框是否未选中 */
LCUI_EXPORT(int)
RadioButton_IsOff( LCUI_Widget *widget )
{
	LCUI_RadioButton *radio_button;
	radio_button = Widget_GetPrivData(widget); 
	return !radio_button->on;
}

static void
RadioButton_Click( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{ 
	if(RadioButton_IsOff(widget)) {
		RadioButton_SetOn(widget); 
	}
}

/* 设定单选框中的图像框的尺寸 */
LCUI_EXPORT(void)
RadioButton_ImgBox_SetSize(LCUI_Widget *widget, LCUI_Size size)
{
	LCUI_Widget *imgbox;
	if(size.w <= 0 && size.h <= 0) {
		return;
	}
	imgbox = RadioButton_GetImgBox(widget);
	Widget_Resize(imgbox, size);
	/* 由于没有布局盒子，不能自动调整部件间的间隔，暂时用这个方法 */
	Widget_SetAlign(imgbox->parent, ALIGN_MIDDLE_LEFT, Pos(size.w, 0));
}

/* 初始化单选框部件的数据 */
static void 
RadioButton_Init( LCUI_Widget *widget )
{
	int valid_state;
	LCUI_Widget *container[2];
	LCUI_RadioButton *radio_button;
	
	radio_button = WidgetPrivData_New(widget, sizeof(LCUI_RadioButton));
	radio_button->on = FALSE;
	/* 初始化图片数据 */ 
	Graph_Init(&radio_button->img_off_disable);
	Graph_Init(&radio_button->img_off_normal);
	Graph_Init(&radio_button->img_off_focus);
	Graph_Init(&radio_button->img_off_down);
	Graph_Init(&radio_button->img_off_over);
	Graph_Init(&radio_button->img_on_disable);
	Graph_Init(&radio_button->img_on_normal);
	Graph_Init(&radio_button->img_on_focus);
	Graph_Init(&radio_button->img_on_down);
	Graph_Init(&radio_button->img_on_over);
	
	radio_button->mutex = NULL;
	
	radio_button->label = Widget_New("label");/* 创建label部件 */
	radio_button->imgbox = Widget_New("picture_box"); /* 创建图像框部件 */
	/* 创建两个容器，用于调整上面两个部件的位置 */
	container[0] = Widget_New(NULL);
	container[1] = Widget_New(NULL);
	
	/* 启用这些部件的自动尺寸调整的功能 */
	Widget_SetAutoSize( widget, TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK );
	Widget_SetAutoSize( container[0], TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK );
	Widget_SetAutoSize( container[1], TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK );
	
	Widget_Container_Add(container[0], radio_button->imgbox);
	Widget_Container_Add(container[1], radio_button->label);
	Widget_Container_Add(widget, container[0]);
	Widget_Container_Add(widget, container[1]);
	
	/* 调整尺寸 */
	Widget_Resize(radio_button->imgbox, Size(15, 15));
	/* 调整布局 */
	Widget_SetAlign(container[0], ALIGN_MIDDLE_LEFT, Pos(0,0));
	Widget_SetAlign(container[1], ALIGN_MIDDLE_LEFT, Pos(17,0));
	Widget_SetAlign(radio_button->imgbox, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_SetAlign(radio_button->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	
	PictureBox_SetSizeMode(radio_button->imgbox, SIZE_MODE_STRETCH);
	
	/* 显示之 */
	Widget_Show(radio_button->label);
	Widget_Show(radio_button->imgbox);
	Widget_Show(container[0]);
	Widget_Show(container[1]);
	
	Widget_Event_Connect( widget, EVENT_CLICKED, RadioButton_Click );
	
	valid_state = (WIDGET_STATE_NORMAL | WIDGET_STATE_ACTIVE);
	valid_state |= (WIDGET_STATE_DISABLE | WIDGET_STATE_OVERLAY);
	Widget_SetValidState( widget, valid_state);
}

/* 更新单选框的图形数据 */
static void RadioButton_ExecUpdate( LCUI_Widget *widget )
{
	LCUI_Graph *p;
	LCUI_RadioButton *radio_button;
	
	radio_button = Widget_GetPrivData(widget);
	/* 如果为自定义风格，那就使用用户指定的图形 */ 	
	if(_LCUIString_Cmp(&widget->style_name, "custom") == 0) {
		//printf("custom\n"); 
		if(widget->enabled == FALSE) 
			widget->state = WIDGET_STATE_DISABLE;
			
		switch(widget->state)
		{/* 判断按钮的状态，以选择相应的背景色 */
		case WIDGET_STATE_NORMAL:
			if(radio_button->on) {
				p = &radio_button->img_on_normal;
			} else {
				p = &radio_button->img_off_normal;
			}
			PictureBox_SetImage(radio_button->imgbox, p);
			break;
		case WIDGET_STATE_OVERLAY :
			if(radio_button->on) {
				p = &radio_button->img_on_over;
			} else {
				p = &radio_button->img_off_over;
			}
			PictureBox_SetImage(radio_button->imgbox, p);
			break;
		case WIDGET_STATE_ACTIVE : 
			if(radio_button->on) {
				p = &radio_button->img_on_down;
			} else {
				p = &radio_button->img_off_down;
			}
			PictureBox_SetImage(radio_button->imgbox, p);
			break;
		case WIDGET_STATE_DISABLE :
			if(radio_button->on) {
				p = &radio_button->img_on_disable;
			} else {
				p = &radio_button->img_off_disable;
			}
			PictureBox_SetImage(radio_button->imgbox, p); 
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
		p = PictureBox_GetImage(radio_button->imgbox);
		Graph_Free(p);
		
		/* 由于本函数在退出后，使用局部变量保存的图形数据会无效，因此，申请内存空间来储存 */
		p = (LCUI_Graph*)calloc(1,sizeof(LCUI_Graph));
		
		switch(widget->state)
		{/* 判断按钮的状态，以选择相应的背景色 */
		    case WIDGET_STATE_NORMAL:
			if(radio_button->on) {
				Load_Graph_Default_RadioButton_On_Normal(p);
			} else {
				Load_Graph_Default_RadioButton_Off_Normal(p);
			}
			PictureBox_SetImage(radio_button->imgbox, p);
			break;
		    case WIDGET_STATE_OVERLAY :
			if(radio_button->on) {
				Load_Graph_Default_RadioButton_On_Selected(p);
			} else {
				Load_Graph_Default_RadioButton_Off_Selected(p);
			}
			PictureBox_SetImage(radio_button->imgbox, p);
			break;
		    case WIDGET_STATE_ACTIVE : 
			if(radio_button->on) {
				Load_Graph_Default_RadioButton_On_Pressed(p);
			} else {
				Load_Graph_Default_RadioButton_Off_Pressed(p);
			}
			PictureBox_SetImage(radio_button->imgbox, p);
			break;
		    case WIDGET_STATE_DISABLE :
			break;
			default :
			break;
		}
	} 
}

/* 获取单选框部件中的label部件的指针 */
LCUI_EXPORT(LCUI_Widget*)
RadioButton_GetLabel( LCUI_Widget *widget )
{
	LCUI_RadioButton *radio_button;
	
	radio_button = Widget_GetPrivData(widget); 
	if( !radio_button ) {
		return NULL;
	}
	return radio_button->label;
}

/* 获取单选框部件中的PictureBox部件的指针 */
LCUI_EXPORT(LCUI_Widget*)
RadioButton_GetImgBox( LCUI_Widget *widget )
{
	LCUI_RadioButton *radio_button;
	
	radio_button = Widget_GetPrivData(widget); 
	if( !radio_button ) {
		return NULL;
	}
	return radio_button->imgbox;
}

/* 设定与单选框部件关联的文本内容 */
LCUI_EXPORT(void)
RadioButton_Text( LCUI_Widget *widget, const char *text )
{
	LCUI_Widget *label;
	label = RadioButton_GetLabel(widget);
	Label_Text(label, text); 
}

LCUI_EXPORT(void)
RadioButton_TextW( LCUI_Widget *widget, const wchar_t *text )
{
	LCUI_Widget *label;
	label = RadioButton_GetLabel(widget);
	Label_TextW(label, text); 
}

/* 创建一个带文本内容的单选框 */
LCUI_EXPORT(LCUI_Widget*)
RadioButton_New( const char *text )
{
	LCUI_Widget *widget;
	widget = Widget_New("radio_button");
	RadioButton_Text(widget, text);
	return widget;
}

/* 注册单选框部件类型 */
LCUI_EXPORT(void)
Register_RadioButton(void)
{
	/* 添加几个部件类型 */
	WidgetType_Add("radio_button"); 
	
	/* 为部件类型关联相关函数 */ 
	WidgetFunc_Add("radio_button",	RadioButton_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("radio_button", RadioButton_ExecUpdate, FUNC_TYPE_DRAW); 
	WidgetFunc_Add("radio_button", RadioButton_ExecUpdate, FUNC_TYPE_UPDATE); 
}
