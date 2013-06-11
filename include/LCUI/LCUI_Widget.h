/* ***************************************************************************
 * LCUI_Widget.h -- GUI widget operation set.
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
 * LCUI_Widget.h -- GUI部件操作集
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

#ifndef __LCUI_WIDGET_H__
#define __LCUI_WIDGET_H__
#include LC_GRAPH_H

LCUI_BEGIN_HEADER

typedef void (*WidgetCallBackFunc)(LCUI_Widget*);

/***************** 部件相关函数的类型 *******************/
typedef enum FuncType_
{
	FUNC_TYPE_SHOW,
	FUNC_TYPE_HIDE,
	FUNC_TYPE_INIT,
	FUNC_TYPE_DRAW,
	FUNC_TYPE_RESIZE,
	FUNC_TYPE_UPDATE,
	FUNC_TYPE_DESTROY
}FuncType;
/****************************************************/

/***************************** 定位类型 ********************************/
typedef enum POS_TYPE_ {
	POS_TYPE_STATIC,
	POS_TYPE_RELATIVE,
	POS_TYPE_ABSOLUTE,
	POS_TYPE_FIXED,
} POS_TYPE;
/**********************************************************************/

/*------------ 部件停靠类型 --------------*/
typedef enum DOCK_TYPE_ {
	DOCK_TYPE_NONE,
	DOCK_TYPE_TOP,
	DOCK_TYPE_LEFT,
	DOCK_TYPE_RIGHT,
	DOCK_TYPE_FILL,
	DOCK_TYPE_BOTTOM
}
DOCK_TYPE;
/*---------------- END -----------------*/

/* --------------------- 部件状态 -----------------------*/
typedef enum WIDGET_STATE_ {
	WIDGET_STATE_NORMAL = 1,	/* 普通状态 */
	WIDGET_STATE_DISABLE = 1<<1,	/* 禁用状态 */
	WIDGET_STATE_OVERLAY = 1<<2,	/* 被鼠标游标覆盖 */
	WIDGET_STATE_ACTIVE = 1<<3,	/* 被鼠标点击 */
} WIDGET_STATE;
/* ------------------------------------------------------*/

/*--------------------- 背景属性 --------------------------*/
typedef struct LCUI_Background_ {
	LCUI_BOOL transparent;	/* 是否透明 */
	LCUI_Graph image;	/* 背景图 */
	LCUI_RGB color;		/* 背景色 */
	LAYOUT_TYPE layout;	/* 背景图的布局 */
} LCUI_Background;
/*--------------------------------------------------------*/

typedef struct	LCUI_Widget_	LCUI_Widget;
struct LCUI_Widget_ {
	LCUI_ID app_id;		/* 所属程序的ID */
	LCUI_ID self_id;	/* 自身ID */
	LCUI_ID type_id;	/* 类型编号 */
	LCUI_ID style_id;	/* 风格编号 */
	LCUI_String type_name;	/* 类型名 */
	LCUI_String style_name; /* 风格名，对某些部件有效 */

	LCUI_BOOL enabled;	/* 是否启用 */
	LCUI_BOOL visible;	/* 是否可见 */
	LCUI_BOOL modal;	/* 是否为模态部件 */

	LCUI_Pos pos;	/* 已计算出的实际位置 */
	LCUI_Pos max_pos;
	LCUI_Pos min_pos;
	/*------------ 位置（描述） ---------------*/
	IntOrFloat_t x, y;
	IntOrFloat_t max_x, min_x;
	IntOrFloat_t max_y, min_y;
	/*-------------- END -------------------*/

	LCUI_Size size; /* 已计算出的实际尺寸，单位为像素 */
	LCUI_Size max_size;
	LCUI_Size min_size;

	/*------------ 尺寸（描述） ---------------*/
	IntOrFloat_t w, h;
	IntOrFloat_t max_w, min_w;
	IntOrFloat_t max_h, min_h;
	/*--------------- END -------------------*/

	/*----------------- 部件布局相关 ----------------*/
	POS_TYPE	pos_type;	/* 位置类型 */
	ALIGN_TYPE	align;		/* 布局 */
	LCUI_Pos	offset;		/* x，y轴的偏移量 */
	DOCK_TYPE	dock;		/* 停靠位置 */
	/*------------------ END ----------------------*/

	LCUI_RGB color;			/* 前景颜色 */
	LCUI_Border border;		/* 边框 */
	LCUI_Background background;	/* 背景 */

	LCUI_Widget *parent;		/* 父部件 */
	LCUI_Queue child;		/* 子部件集 */
	LCUI_Queue event;		/* 保存部件的事件关联的数据 */
	LCUI_Queue msg_buff;		/* 记录子部件的消息 */
	LCUI_Queue msg_func;		/* 记录与自定义消息关联的函数 */
	LCUI_RectQueue invalid_area;	/* 记录无效区域 */

	WIDGET_STATE state;	/* 部件当前状态 */
	int valid_state;	/* 对部件有效的状态 */

	LCUI_BOOL auto_size;	/* 指定是否自动调整自身的大小，以适应内容的大小 */
	AUTOSIZE_MODE auto_size_mode;	/* 自动尺寸调整模式 */

	LCUI_BOOL focus;		/* 指定该部件是否需要焦点 */
	LCUI_Widget* focus_widget;	/* 获得焦点的子部件 */

	int clickable_mode;		/* 确定在对比像素alpha值时，是要“小于”还是“不小于”才使条件成立 */
	uchar_t clickable_area_alpha;	/* 指定部件图层中的区域的alpha值小于/不小于多少时可被鼠标点击，默认为0，最大为255 */

	LCUI_GraphLayer* glayer;	/* 部件的图层 */

	LCUI_Mutex mutex;	/* 互斥锁 */
	void *private_data;	/* 指针，指向部件私有数据，供预先注册的回调函数利用 */

	/* 以下是函数指针，闲函数名太长的话，可以直接用下面的 */
	void (*resize)(LCUI_Widget*, LCUI_Size);
	void (*move)(LCUI_Widget*, LCUI_Pos);
	void (*show)(LCUI_Widget*);
	void (*hide)(LCUI_Widget*);
	void (*disable)(LCUI_Widget*);
	void (*enable)(LCUI_Widget*);
	void (*set_align)(LCUI_Widget*, ALIGN_TYPE, LCUI_Pos);
	void (*set_alpha)(LCUI_Widget*, unsigned char);
	void (*set_border)(LCUI_Widget*, LCUI_Border);
};

/**********************************************************************/

LCUI_END_HEADER

#include LC_WIDGET_MSG_H
#include LC_WIDGET_BASE_H
#include LC_WIDGET_EVENT_H
#include LC_WIDGET_LIBRARY_H
#include LC_WIDGET_STYLE_H
#include LC_GUI_MSGBOX_H

#ifdef __cplusplus
#include LC_WIDGET_HPP
#endif
#endif
