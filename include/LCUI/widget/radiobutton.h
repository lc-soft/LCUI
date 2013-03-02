/* ***************************************************************************
 * radiobutton.h -- LCUI‘s RadioButton widget
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
 * radiobutton.h -- LCUI 的单选框部件
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

#ifndef __LCUI_RADIOBTN_H__
#define __LCUI_RADIOBTN_H__

LCUI_BEGIN_HEADER

typedef struct _LCUI_RadioButton LCUI_RadioButton;

/*********************** 单选框 **************************/
struct _LCUI_RadioButton
{
	int	on; /* 一个标志，指定是否被选中 */
	/* 当风格为Custom_Style(自定义)时，将根据按钮的状态使用这些图形 */
	/* 未打勾的状态 */
	LCUI_Graph	img_off_normal;	/* 普通状态 */
	LCUI_Graph	img_off_over;	/* 被鼠标覆盖 */
	LCUI_Graph	img_off_down;	/* 按下状态 */
	LCUI_Graph	img_off_focus;	/* 焦点状态 */
	LCUI_Graph	img_off_disable;/* 不可用的状态 */
	/* 打上勾的状态 */
	LCUI_Graph	img_on_normal;	/* 普通状态 */
	LCUI_Graph	img_on_over;	/* 被鼠标覆盖 */
	LCUI_Graph	img_on_down;	/* 按下状态 */
	LCUI_Graph	img_on_focus;	/* 焦点状态 */
	LCUI_Graph	img_on_disable;	/* 不可用的状态 */
	
	LCUI_Widget	*label;	/* 嵌套的文本标签 */
	LCUI_Widget	*imgbox;/* 用于显示图形 */
	
	LCUI_Queue		*mutex;	/* 记录互斥的单选框部件的指针 */
};
/****************************************************/

/* 将单选框从互斥关系链中移除 */
LCUI_EXPORT(void)
RadioButton_DeleteMutex( LCUI_Widget *widget );

/* 为两个单选框建立互斥关系 */
LCUI_EXPORT(void)
RadioButton_CreateMutex( LCUI_Widget *a, LCUI_Widget *b );

/* 设定单选框为选中状态 */
LCUI_EXPORT(void)
RadioButton_SetOn( LCUI_Widget *widget );

/* 设定单选框为未选中状态 */
LCUI_EXPORT(void)
RadioButton_SetOff( LCUI_Widget *widget );

/* 检测单选框是否被选中 */
LCUI_EXPORT(LCUI_BOOL)
RadioButton_IsOn( LCUI_Widget *widget );

/* 检测单选框是否未选中 */
LCUI_EXPORT(int)
RadioButton_IsOff( LCUI_Widget *widget );

/* 设定单选框中的图像框的尺寸 */
LCUI_EXPORT(void)
RadioButton_ImgBox_SetSize(LCUI_Widget *widget, LCUI_Size size);

/* 获取单选框部件中的label部件的指针 */
LCUI_EXPORT(LCUI_Widget*)
RadioButton_GetLabel( LCUI_Widget *widget );

/* 获取单选框部件中的PictureBox部件的指针 */
LCUI_EXPORT(LCUI_Widget*)
RadioButton_GetImgBox( LCUI_Widget *widget );

/* 设定与单选框部件关联的文本内容 */
LCUI_EXPORT(void)
RadioButton_Text( LCUI_Widget *widget, const char *text );

/* 创建一个带文本内容的单选框 */
LCUI_EXPORT(LCUI_Widget*)
RadioButton_New( const char *text );

LCUI_END_HEADER

#endif
