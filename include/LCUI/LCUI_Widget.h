/* ***************************************************************************
 * LCUI_Widget.h -- GUI widget operation set.
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
 * LCUI_Widget.h -- GUI部件操作集
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
 
#ifndef __LCUI_WIDGET_H__
#define __LCUI_WIDGET_H__

#include LC_DRAW_BORDER_H

/***************** 部件相关函数的类型 *******************/
typedef enum _FuncType
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
typedef enum _POS_TYPE
{
	POS_TYPE_STATIC,
	POS_TYPE_RELATIVE,
	POS_TYPE_ABSOLUTE,
	POS_TYPE_FIXED,
}
POS_TYPE;
/*
 * absolute:
 * 	绝对定位，相对于 static 定位以外的第一个父元素进行定位。
 * 	元素的位置通过 "left", "top", "right" 以及 "bottom" 属性进行规定。
 * fixed：
 * 	绝对定位，相对于屏幕进行定位。
 * 	元素的位置通过 "left", "top", "right" 以及 "bottom" 属性进行规定。 
 * static：
 * 	默认值。没有定位，忽略 top, bottom, left, right 或者 z-index。 
 */
/**********************************************************************/

/*------------ 部件停靠类型 --------------*/
typedef enum _DOCK_TYPE
{
	DOCK_TYPE_NONE,
	DOCK_TYPE_TOP,
	DOCK_TYPE_LEFT,
	DOCK_TYPE_RIGHT,
	DOCK_TYPE_FILL,
	DOCK_TYPE_BOTTOM
}
DOCK_TYPE;
/*---------------- END -----------------*/

/*----------------- 部件的几种状态 ------------------*/
typedef enum {
	WIDGET_STATE_NORMAL = 1,	/* 普通状态 */
	WIDGET_STATE_DISABLE = 1<<1,	/* 禁用状态 */
	WIDGET_STATE_OVERLAY = 1<<2,	/* 被鼠标游标覆盖 */
	WIDGET_STATE_ACTIVE = 1<<3,	/* 被鼠标点击 */
} WIDGET_STATE;
/*-------------------------------------------------*/

/******************************* 部件 **********************************/
typedef struct {
	BOOL transparent; /* 是否透明 */
	LCUI_Graph image; /* 背景图 */
	LCUI_RGB color; /* 背景色 */
	ALIGN_TYPE layout; /* 背景图的布局 */
} LCUI_Background;


struct _LCUI_Widget {
	LCUI_ID app_id;	/* 所属程序的ID */
	LCUI_ID self_id;	/* 自身ID */
	LCUI_ID type_id;	/* 类型编号 */
	LCUI_ID style_id;	/* 风格编号 */
	LCUI_String type_name;	/* 类型名 */
	LCUI_String style_name; /* 风格名，对某些部件有效 */
	
	BOOL enabled;	/* 是否启用 */
	BOOL visible;	/* 是否可见 */
	BOOL modal;	/* 是否为模态部件 */
	
	LCUI_Pos pos;	/* 已计算出的实际位置 */
	LCUI_Pos max_pos;
	LCUI_Pos min_pos;
	/*------------ 位置（描述） ---------------*/ 
	PX_P_t x, y;
	PX_P_t max_x, min_x;
	PX_P_t max_y, min_y;
	/*-------------- END -------------------*/
	
	LCUI_Size size; /* 已计算出的实际尺寸，单位为像素 */
	LCUI_Size max_size;
	LCUI_Size min_size;
	
	/*------------ 尺寸（描述） ---------------*/ 
	PX_P_t w, h;
	PX_P_t max_w, min_w;
	PX_P_t max_h, min_h;
	/*--------------- END -------------------*/
	
	/*----------------- 部件布局相关 ----------------*/
	POS_TYPE	pos_type;	/* 位置类型 */
	ALIGN_TYPE	align;		/* 布局 */
	LCUI_Pos	offset;		/* x，y轴的偏移量 */
	DOCK_TYPE	dock;		/* 停靠位置 */
	/*------------------ END ----------------------*/
	
	/*------------ 外边距和内边距 ---------------*/ 
	LCUI_Margin	margin;
	LCUI_Padding	padding;
	/*---------------- END -------------------*/
	
	LCUI_RGB color;		/* 前景颜色 */
	LCUI_Border border;		/* 边框 */
	LCUI_Background background;	/* 背景 */
	
	LCUI_Widget *parent;		/* 父部件 */
	LCUI_Queue child;		/* 子部件集 */
	LCUI_Queue event;		/* 保存部件的事件关联的数据 */
	LCUI_Queue data_buff;		/* 记录需要进行更新的数据 */ 
	LCUI_Queue invalid_area;	/* 记录无效区域 */
	
	WIDGET_STATE state;	/* 部件当前状态 */
	int valid_state;	/* 对部件有效的状态 */
	
	BOOL auto_size;	/* 指定是否自动调整自身的大小，以适应内容的大小 */
	AUTOSIZE_MODE auto_size_mode;	/* 自动尺寸调整模式 */
	
	BOOL focus;		/* 指定该部件是否需要焦点 */
	LCUI_Widget* focus_widget;	/* 获得焦点的子部件 */
	
	int clickable_mode;		/* 确定在对比像素alpha值时，是要“小于”还是“不小于”才使条件成立 */
	uchar_t clickable_area_alpha;	/* 指定部件图层中的区域的alpha值小于/不小于多少时可被鼠标点击，默认为0，最大为255 */
	
	LCUI_GraphLayer* main_glayer;		/* 部件的主图层 */
	LCUI_GraphLayer* client_glayer;	/* 客户区图层 */
	
	void *private_data;   /* 指针，指向部件私有数据，供预先注册的回调函数利用 */
	
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

#include LC_WORK_H
#include LC_GUI_MSGBOX_H

LCUI_BEGIN_HEADER

/***************************** Widget *********************************/
LCUI_Size Widget_GetSize(LCUI_Widget *widget);
/* 功能：获取部件的尺寸 */ 

LCUI_Size _Widget_GetSize(LCUI_Widget *widget);
/* 功能：通过计算获取部件的尺寸 */

int Widget_GetHeight(LCUI_Widget *widget);

int _Widget_GetHeight(LCUI_Widget *widget);

int Widget_GetWidth(LCUI_Widget *widget);

int _Widget_GetWidth(LCUI_Widget *widget);

LCUI_Rect Widget_GetRect(LCUI_Widget *widget);
/* 功能：获取部件的区域 */ 

LCUI_Pos Widget_GetPos(LCUI_Widget *widget);
/* 
 * 功能：获取部件相对于容器部件的位置
 * 说明：该位置相对于容器部件的左上角点，忽略容器部件的内边距。
 *  */

LCUI_Pos Get_Widget_RelativePos(LCUI_Widget *widget);
/* 
 * 功能：获取部件的相对于所在容器区域的位置
 * 说明：部件所在容器区域并不一定等于容器部件的区域，因为容器区域大小受到
 * 容器部件的内边距的影响。
 *  */

LCUI_Pos GlobalPos_ConvTo_RelativePos(LCUI_Widget *widget, LCUI_Pos global_pos);
/* 
 * 功能：全局坐标转换成相对坐标
 * 说明：传入的全局坐标，将根据传入的部件指针，转换成相对于该部件所在容器区域的坐标
 *  */
 
void *Widget_GetPrivData(LCUI_Widget *widget);
/* 功能：获取部件的私有数据结构体的指针 */ 

LCUI_Widget *Get_Widget_Parent(LCUI_Widget *widget);
/* 功能：获取部件的父部件 */ 

void print_widget_info(LCUI_Widget *widget);
/* 
 * 功能：打印widget的信息
 * 说明：在调试时需要用到它，用于确定widget是否有问题
 *  */ 

/* 在指定部件的内部区域内设定需要刷新的区域 */
int Widget_InvalidArea ( LCUI_Widget *widget, LCUI_Rect rect );

/* 转移子部件中的无效区域至父部件的无效区域记录中 */
int Widget_SyncInvalidArea( LCUI_Widget *widget );

/* 
 * 功能：让指定部件响应部件状态的改变
 * 说明：部件创建时，默认是不响应状态改变的，因为每次状态改变后，都要调用函数重绘部件，
 * 这对于一些部件是多余的，没必要重绘，影响效率。如果想让部件能像按钮那样，鼠标移动到它
 * 上面时以及鼠标点击它时，都会改变按钮的图形样式，那就需要用这个函数设置一下。
 * 用法：
 * Widget_SetValidState( widget, WIDGET_STATE_NORMAL );
 * Widget_SetValidState( widget, WIDGET_STATE_OVERLAY | WIDGET_STATE_ACTIVE );
 *  */
void Widget_SetValidState( LCUI_Widget *widget, int state );

/*
 * 功能：获取部件的指定类型的父部件的指针
 * 说明：本函数会在部件关系链中往头部查找父部件指针，并判断这个父部件是否为制定类型
 * 返回值：没有符合要求的父级部件就返回NULL，否则返回部件指针
 **/
LCUI_Widget *Widget_GetParent(LCUI_Widget *widget, char *widget_type);

/* 获取指定ID的子部件 */
LCUI_Widget *Widget_GetChildByID( LCUI_Widget *widget, LCUI_ID id );

/* 销毁指定ID的程序的所有部件 */
void LCUIApp_DestroyAllWidgets( LCUI_ID app_id );

LCUI_Widget *
Get_FocusWidget( LCUI_Widget *widget );
/* 获取指定部件内的已获得焦点的子部件 */ 

/* 检测指定部件是否处于焦点状态 */
BOOL Widget_GetFocus( LCUI_Widget *widget );

/* 检测指定部件是否获得全局焦点，也就是该部件以及上级所有父部件是否都得到了焦点 */
BOOL Widget_GetGlobalFocus( LCUI_Widget *widget );

/* 获取部件的风格名称 */
LCUI_String Widget_GetStyleName( LCUI_Widget *widget );

/* 设定部件的风格名称 */
void Widget_SetStyleName( LCUI_Widget *widget, const char *style_name );

/* 设定部件的风格ID */
void Widget_SetStyleID( LCUI_Widget *widget, int style_id );

/* 获取与指定坐标层叠的部件 */
LCUI_Widget *Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos );

int Widget_Is_Active(LCUI_Widget *widget);
/* 功能：判断部件是否为活动状态 */ 

int Empty_Widget();
/* 
 * 功能：用于检测程序的部件列表是否为空 
 * 返回值：
 *   1  程序的部件列表为空
 *   0  程序的部件列表不为空
 * */ 

/* 功能：为部件私有结构体指针分配内存 */
void *WidgetPrivData_New( LCUI_Widget *widget, size_t size );

/* 初始化部件队列 */
void WidgetQueue_Init(LCUI_Queue *queue);

/* 
 * 功能：创建指定类型的部件
 * 返回值：成功则部件的指针，失败则返回NULL
 */
LCUI_Widget *Widget_New( const char *widget_type );

/* 销毁部件 */
void Widget_Destroy( LCUI_Widget *widget );

LCUI_Pos Widget_GetGlobalPos(LCUI_Widget *widget);
/* 功能：获取部件的全局坐标 */ 

void Set_Widget_ClickableAlpha( LCUI_Widget *widget, uchar_t alpha, int mode );
/* 设定部件可被点击的区域的透明度 */

void Widget_SetAlign(LCUI_Widget *widget, ALIGN_TYPE align, LCUI_Pos offset);
/* 功能：设定部件的对齐方式以及偏移距离 */ 

/* 设定部件的高度，单位为像素 */
void Widget_SetHeight( LCUI_Widget *widget, int height );

void Widget_SetSize( LCUI_Widget *widget, char *width, char *height );
/* 
 * 功能：设定部件的尺寸大小
 * 说明：如果设定了部件的停靠位置，并且该停靠类型默认限制了宽/高，那么部件的宽/高就不能被改变。
 * 用法示例：
 * Widget_SetSize( widget, "100px", "50px" ); 部件尺寸最大为100x50像素
 * Widget_SetSize( widget, "100%", "50px" ); 部件宽度等于容器宽度，高度为50像素
 * Widget_SetSize( widget, "50", "50" ); 部件尺寸最大为50x50像素，px可以省略 
 * Widget_SetSize( widget, NULL, "50%" ); 部件宽度保持原样，高度为容器高度的一半
 * */ 

/* 指定部件是否为模态部件 */
void Widget_SetModal( LCUI_Widget *widget, BOOL is_modal );

void Widget_SetDock( LCUI_Widget *widget, DOCK_TYPE dock );
/* 设定部件的停靠类型 */ 

int Widget_SetMaxSize( LCUI_Widget *widget, char *width, char *height );
/* 
 * 功能：设定部件的最大尺寸 
 * 说明：当值为0时，部件的尺寸不受限制，用法示例可参考Set_Widget_Size()函数 
 * */ 

int Widget_SetMinSize( LCUI_Widget *widget, char *width, char *height );
/* 
 * 功能：设定部件的最小尺寸 
 * 说明：用法示例可参考Set_Widget_Size()函数 
 * */
void Widget_LimitSize(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size);
/* 功能：限制部件的尺寸变动范围 */ 

void Widget_LimitPos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos);
/* 功能：限制部件的移动范围 */ 

/* 设定部件的边框 */
void Widget_SetBorder( LCUI_Widget *widget, LCUI_Border border );

/* 设定部件边框的四个角的圆角半径 */
void Widget_SetBorderRadius( LCUI_Widget *widget, int radius );

/* 设定部件的背景图像 */
void Widget_SetBackgroundImage( LCUI_Widget *widget, LCUI_Graph *img );

/* 设定背景图的布局 */
void Widget_SetBackgroundLayout( LCUI_Widget *widget, LAYOUT_TYPE layout );

/* 设定部件的背景颜色 */
void Widget_SetBackgroundColor( LCUI_Widget *widget, LCUI_RGB color );

/* 设定部件背景是否透明 */
void Widget_SetBackgroundTransparent( LCUI_Widget *widget, BOOL flag );

void Widget_Enable(LCUI_Widget *widget);
/* 功能：启用部件 */ 

void Widget_Disable(LCUI_Widget *widget);
/* 功能：禁用部件 */ 

/* 指定部件是否可见 */
void Widget_Visible( LCUI_Widget *widget, BOOL flag );

void Widget_SetPos(LCUI_Widget *widget, LCUI_Pos pos);
/* 
 * 功能：设定部件的位置 
 * 说明：只修改坐标，不进行局部刷新
 * */ 

void Widget_SetPadding( LCUI_Widget *widget, LCUI_Padding padding );
/* 设置部件的内边距 */

void Widget_SetPosType( LCUI_Widget *widget, POS_TYPE pos_type );
/* 设定部件的定位类型 */

void Widget_SetAlpha(LCUI_Widget *widget, unsigned char alpha);
/* 功能：设定部件的透明度 */ 

void Widget_ExecMove(LCUI_Widget *widget, LCUI_Pos pos);
/*
 * 功能：执行移动部件位置的操作
 * 说明：更改部件位置，并添加局部刷新区域
 **/ 

void Widget_ExecHide(LCUI_Widget *widget);
/* 功能：执行隐藏部件的操作 */ 

void Widget_ExecShow(LCUI_Widget *widget);
/* 功能：执行显示部件的任务 */ 

/* 自动调整部件大小，以适应其内容大小 */
void Widget_AutoResize(LCUI_Widget *widget);

void Widget_ExecResize(LCUI_Widget *widget, LCUI_Size size);
/* 功能：执行改变部件尺寸的操作 */ 

/* 启用或禁用部件的自动尺寸调整功能 */
void Widget_SetAutoSize(	LCUI_Widget *widget,
				BOOL flag, AUTOSIZE_MODE mode );

void Widget_ExecRefresh(LCUI_Widget *widget);
/* 功能：执行刷新显示指定部件的整个区域图形的操作 */ 

void Widget_ExecUpdate(LCUI_Widget *widget);
/* 功能：执行部件的更新操作 */

void Widget_ExecDraw(LCUI_Widget *widget);
/* 功能：执行部件图形更新操作 */ 

/* 获取指向部件自身图形数据的指针 */
LCUI_Graph *Widget_GetSelfGraph( LCUI_Widget *widget );

/* 获取部件实际显示的图形 */
int Widget_GetGraph( 
		LCUI_Widget *widget, 
		LCUI_Graph *graph_buff, 
		LCUI_Rect rect );

LCUI_Pos Widget_GetValidPos( LCUI_Widget *widget, LCUI_Pos pos );
/* 获取有效化后的坐标数据，其实就是将在限制范围外的坐标处理成在限制范围内的 */

void Widget_Move(LCUI_Widget *widget, LCUI_Pos new_pos);
/* 
 * 功能：移动部件位置
 * 说明：如果部件的布局为ALIGN_NONE，那么，就可以移动它的位置，否则，无法移动位置
 * */ 

void Update_Widget_Pos(LCUI_Widget *widget);
/* 功能：更新部件的位置 */ 

void Update_Child_Widget_Pos(LCUI_Widget *widget);
/* 
 * 功能：更新指定部件的子部件的位置
 * 说明：当作为子部件的容器部件的尺寸改变后，有的部件的布局不为ALIGN_NONE，就需要重新
 * 调整位置。
 * */ 

void Widget_UpdateSize( LCUI_Widget *widget );
/* 部件尺寸更新 */ 

void Update_Child_Widget_Size(LCUI_Widget *widget);
/* 
 * 功能：更新指定部件的子部件的尺寸
 * 说明：当部件尺寸改变后，有的部件的尺寸以及位置是按百分比算的，需要重新计算。
 * */
 
void Offset_Widget_Pos(LCUI_Widget *widget, LCUI_Pos offset);
/* 功能：以部件原有的位置为基础，根据指定的偏移坐标偏移位置 */ 

void Move_Widget_To_Pos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed);
/* 
 * 功能：将部件以指定的速度向指定位置移动 
 * 说明：des_pos是目标位置，speed是该部件的移动速度，单位为：像素/秒
 * */ 

void Refresh_Widget(LCUI_Widget *widget);
/* 功能：刷新显示指定部件的整个区域图形 */ 

/* 调整部件的尺寸 */
void Widget_Resize( LCUI_Widget *widget, LCUI_Size new_size );

void Widget_Draw(LCUI_Widget *widget);
/* 功能：重新绘制部件 */ 

void Widget_Update(LCUI_Widget *widget);
/* 
 * 功能：让部件根据已设定的属性，进行相应数据的更新
 * 说明：此记录会添加至队列，如果队列中有一条相同记录，则覆盖上条记录。
 * */

void __Update_Widget(LCUI_Widget *widget);
/* 
 * 功能：让部件根据已设定的属性，进行相应数据的更新
 * 说明：与上个函数功能一样，但是，可以允许队列中有两条相同记录。
 * */

/* 将指定部件显示在同等z-index值的部件的前端 */
int Widget_Front( LCUI_Widget *widget );

void Widget_Show(LCUI_Widget *widget);
/* 功能：显示部件 */ 

void Widget_Hide(LCUI_Widget *widget);
/* 功能：隐藏部件 */ 

/* 改变部件的状态 */
int Widget_SetState( LCUI_Widget *widget, int state );
/************************* Widget End *********************************/



/**************************** Widget Update ***************************/
int Handle_WidgetUpdate(LCUI_Widget *widget);
/* 功能：处理部件的更新 */ 

void Handle_AllWidgetUpdate();
/* 功能：处理所有部件的更新 */ 
/************************ Widget Update End ***************************/


/************************** Widget Library ****************************/
int WidgetFunc_Add(
			const char *type, 
			void (*widget_func)(LCUI_Widget*), 
			FuncType func_type
		);
/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/ 

/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/
int WidgetType_Add( const char *type_name );

void WidgetLib_Init(LCUI_Queue *w_lib);
/* 功能：初始化部件库 */ 

int WidgetType_Delete(const char *type);
/* 功能：删除指定部件类型的相关数据 */ 

void NULL_Widget_Func(LCUI_Widget *widget);
/*
 * 功能：空函数，不做任何操作
 * 说明：如果获取指定部件类型的函数指针失败，将返回这个函数的函数指针
 **/ 

LCUI_ID WidgetType_Get_ID(const char *widget_type);
/* 功能：获取指定类型部件的类型ID */ 

int WidgetType_GetByID(LCUI_ID id, char *widget_type);
/* 功能：获取指定类型ID的类型名称 */ 

void ( *Get_WidgetFunc_By_ID(LCUI_ID id, FuncType func_type) ) (LCUI_Widget*);
/* 功能：获取指定部件类型ID的函数的函数指针 */ 

void ( *Get_WidgetFunc(const char *widget_type, FuncType func_type) ) (LCUI_Widget*);
/* 功能：获取指定类型部件的函数的函数指针 */ 

int WidgetType_Valid(const char *widget_type);
/* 功能：检测指定部件类型是否有效 */ 

/* 调用指定类型的部件函数 */
void WidgetFunc_Call( LCUI_Widget *widget, FuncType type );

void Register_Default_Widget_Type();
/* 功能：为程序的部件库添加默认的部件类型 */ 
/************************ Widget Library End **************************/


/*************************** Container ********************************/
/* 将部件添加至作为容器的部件内 */
int Widget_Container_Add( LCUI_Widget *ctnr, LCUI_Widget *widget );

int _Get_Widget_Container_Width(LCUI_Widget *widget);
/* 通过计算得出指定部件的容器的宽度，单位为像素 */ 

int _Get_Widget_Container_Height(LCUI_Widget *widget);
/* 通过计算得出指定部件的容器的高度，单位为像素 */ 

/* 获取容器的宽度 */
int Widget_GetContainerWidth( LCUI_Widget *widget );

/* 获取容器的高度 */
int Widget_GetContainerHeight( LCUI_Widget *widget );

/* 获取容器的尺寸 */
LCUI_Size Widget_GetContainerSize( LCUI_Widget *widget );
/************************* Container End ******************************/


LCUI_END_HEADER

#ifdef __cplusplus
#include LC_WIDGET_HPP
#endif
#endif
