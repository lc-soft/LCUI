/* ***************************************************************************
 * widget_base.h -- the widget base operation set.
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
 * widget_base.h -- 部件的基本操作集。
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

#ifndef __LCUI_WIDGET_BASE_H__
#define __LCUI_WIDGET_BASE_H__

/*************************** Container ********************************/
/* 将部件添加至作为容器的部件内 */
LCUI_API int
Widget_Container_Add( LCUI_Widget *ctnr, LCUI_Widget *widget );

/* 通过计算得出指定部件的容器的宽度，单位为像素 */
LCUI_API int
_Widget_GetContainerWidth(LCUI_Widget *widget);

/* 通过计算得出指定部件的容器的高度，单位为像素 */
LCUI_API int
_Widget_GetContainerHeight(LCUI_Widget *widget);

/* 获取容器的宽度 */
LCUI_API int
Widget_GetContainerWidth( LCUI_Widget *widget );

/* 获取容器的高度 */
LCUI_API int
Widget_GetContainerHeight( LCUI_Widget *widget );

/* 获取容器的尺寸 */
LCUI_API LCUI_Size
Widget_GetContainerSize( LCUI_Widget *widget );
/************************* Container End ******************************/

/***************************** Widget *********************************/


/* 获取部件的图层指针 */
LCUI_API LCUI_GraphLayer *
Widget_GetGraphLayer( LCUI_Widget *widget );

/* 获取部件的子部件队列 */
LCUI_API LCUI_Queue*
Widget_GetChildList( LCUI_Widget *widget );

/* 获取部件的矩形区域队列 */
LCUI_API LCUI_RectQueue*
Widget_GetInvalidAreaQueue( LCUI_Widget *widget );

/*-------------------------- Widget Pos ------------------------------*/
/* 通过计算得出部件的x轴坐标 */
LCUI_API int
_Widget_GetX( LCUI_Widget *widget );

/* 通过计算得出部件的y轴坐标 */
LCUI_API int
_Widget_GetY( LCUI_Widget *widget );


/* 功能：通过计算得出部件的位置，单位为像素 */
LCUI_API LCUI_Pos
_Widget_GetPos(LCUI_Widget *widget);

/*
 * 功能：获取部件相对于容器部件的位置
 * 说明：该位置相对于容器部件的左上角点，忽略容器部件的内边距。
 *  */
LCUI_API LCUI_Pos
Widget_GetPos(LCUI_Widget *widget);

/*
 * 功能：获取部件的相对于所在容器区域的位置
 * 说明：部件所在容器区域并不一定等于容器部件的区域，因为容器区域大小受到
 * 容器部件的内边距的影响。
 *  */
LCUI_API LCUI_Pos
Widget_GetRelPos(LCUI_Widget *widget);

/*
 * 功能：全局坐标转换成相对坐标
 * 说明：传入的全局坐标，将根据传入的部件指针，转换成相对于该部件的坐标
 *  */
LCUI_API LCUI_Pos
Widget_ToRelPos(LCUI_Widget *widget, LCUI_Pos global_pos);

LCUI_API int
_Widget_GetMaxX( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMaxY( LCUI_Widget *widget );

LCUI_API LCUI_Pos
_Widget_GetMaxPos( LCUI_Widget *widget );

LCUI_API LCUI_Pos
Widget_GetMaxPos(LCUI_Widget *widget);

LCUI_API int
_Widget_GetMinX( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMinY( LCUI_Widget *widget );

LCUI_API LCUI_Pos
_Widget_GetMinPos( LCUI_Widget *widget );

LCUI_API LCUI_Pos
Widget_GetMinPos(LCUI_Widget *widget);
/*------------------------ END Widget Pos -----------------------------*/

/*------------------------- Widget Size -------------------------------*/
LCUI_API int
_Widget_GetMaxWidth( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMaxHeight( LCUI_Widget *widget );

LCUI_API LCUI_Size
_Widget_GetMaxSize( LCUI_Widget *widget );

LCUI_API LCUI_Size
Widget_GetMaxSize( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMinWidth( LCUI_Widget *widget );

LCUI_API int
_Widget_GetMinHeight( LCUI_Widget *widget );

LCUI_API int
_Widget_GetHeight(LCUI_Widget *widget);

LCUI_API int
_Widget_GetWidth(LCUI_Widget *widget);

LCUI_API int
Widget_GetHeight(LCUI_Widget *widget);

LCUI_API int
Widget_GetWidth(LCUI_Widget *widget);

/* 获取部件的尺寸 */
LCUI_API LCUI_Size
Widget_GetSize(LCUI_Widget *widget);

/* 通过计算获取部件的尺寸 */
LCUI_API LCUI_Size
_Widget_GetSize(LCUI_Widget *widget);

/*------------------------ END Widget Size ----------------------------*/


/* 获取部件的区域 */
LCUI_API LCUI_Rect
Widget_GetRect(LCUI_Widget *widget);

/* 获取部件的私有数据结构体的指针 */
LCUI_API void*
Widget_GetPrivData(LCUI_Widget *widget);

LCUI_API int
Widget_PrintChildList( LCUI_Widget *widget );

/*
 * 功能：打印widget的信息
 * 说明：在调试时需要用到它，用于确定widget是否有问题
 *  */
LCUI_API void
print_widget_info(LCUI_Widget *widget);

/* 在指定部件的内部区域内设定需要刷新的区域 */
LCUI_API int
Widget_InvalidArea( LCUI_Widget *widget, LCUI_Rect rect );

/* 将所有可见部件的无效区域 同步至 屏幕无效区域队列中 */
LCUI_API void Widget_SyncInvalidArea( void );

/*
 * 功能：让指定部件响应部件状态的改变
 * 说明：部件创建时，默认是不响应状态改变的，因为每次状态改变后，都要调用函数重绘部件，
 * 这对于一些部件是多余的，没必要重绘，影响效率。如果想让部件能像按钮那样，鼠标移动到它
 * 上面时以及鼠标点击它时，都会改变按钮的图形样式，那就需要用这个函数设置一下。
 * 用法：
 * Widget_SetValidState( widget, WIDGET_STATE_NORMAL );
 * Widget_SetValidState( widget, WIDGET_STATE_OVERLAY | WIDGET_STATE_ACTIVE );
 * */
LCUI_API void
Widget_SetValidState( LCUI_Widget *widget, int state );

/*
 * 功能：获取部件的指定类型的父部件的指针
 * 说明：本函数会在部件关系链中往头部查找父部件指针，并判断这个父部件是否为制定类型
 * 返回值：没有符合要求的父级部件就返回NULL，否则返回部件指针
 **/
LCUI_API LCUI_Widget*
Widget_GetParent(LCUI_Widget *widget, char *widget_type);

/* 获取指定ID的子部件 */
LCUI_API LCUI_Widget*
Widget_GetChildByID( LCUI_Widget *widget, LCUI_ID id );

/* 销毁指定ID的程序的所有部件 */
LCUI_API void
LCUIApp_DestroyAllWidgets( LCUI_ID app_id );

/* 检测指定部件是否处于焦点状态 */
LCUI_API LCUI_BOOL
Widget_GetFocus( LCUI_Widget *widget );

/* 检测指定部件是否获得全局焦点，也就是该部件以及上级所有父部件是否都得到了焦点 */
LCUI_API LCUI_BOOL
Widget_GetGlobalFocus( LCUI_Widget *widget );

/* 获取部件的风格名称 */
LCUI_API LCUI_String
Widget_GetStyleName( LCUI_Widget *widget );

/* 设定部件的风格名称 */
LCUI_API void
Widget_SetStyleName( LCUI_Widget *widget, const char *style_name );

/* 设定部件的风格ID */
LCUI_API void
Widget_SetStyleID( LCUI_Widget *widget, int style_id );

/** 获取指定坐标上的子部件，有则返回子部件指针，否则返回NULL */
LCUI_API LCUI_Widget* Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos );

/** 为部件分配一个用于存放私有数据的内存空间 */
LCUI_API void* Widget_NewPrivData( LCUI_Widget *widget, size_t size );

/* 初始化部件队列 */
LCUI_API void WidgetQueue_Init(LCUI_Queue *queue);

LCUI_API LCUI_Queue *Widget_GetMsgFunc( LCUI_Widget *widget );

LCUI_API LCUI_Queue *Widget_GetMsgBuff( LCUI_Widget *widget );

LCUI_API int Widget_Lock( LCUI_Widget *widget );

LCUI_API int Widget_TryLock( LCUI_Widget *widget );

LCUI_API int Widget_Unlock( LCUI_Widget *widget );

/* 初始化根部件 */
LCUI_API void RootWidget_Init(void);

/* 销毁根部件 */
LCUI_API void RootWidget_Destroy(void);

/* 获取根部件图层指针 */
LCUI_API LCUI_GraphLayer *RootWidget_GetGraphLayer(void);

/* 获取根部件指针 */
LCUI_API LCUI_Widget *RootWidget_GetSelf(void);

/***********************************************************
<Function>
	Widget_New

<Description>
	创建指定类型的部件

<Input>
	widget_type :: 部件的类型名

<Return>
	成功则返回指向该部件的指针，失败则返回NULL
************************************************************/
LCUI_API LCUI_Widget* Widget_New( const char *widget_type );

/* 获取部件的全局坐标 */
LCUI_API LCUI_Pos
Widget_GetGlobalPos(LCUI_Widget *widget);

/* 设定部件可被点击的区域的透明度 */
LCUI_API void
Widget_SetClickableAlpha( LCUI_Widget *widget, uchar_t alpha, int mode );

/* 设定部件的对齐方式以及偏移距离 */
LCUI_API void
Widget_SetAlign(LCUI_Widget *widget, ALIGN_TYPE align, LCUI_Pos offset);

/*
 * 功能：设定部件的最大尺寸
 * 说明：当值为0时，部件的尺寸不受限制，用法示例可参考Set_Widget_Size()函数
 * */
LCUI_API int
Widget_SetMaxSize( LCUI_Widget *widget, char *width, char *height );

/* 设定部件的最小尺寸 */
LCUI_API int
Widget_SetMinSize( LCUI_Widget *widget, char *width, char *height );

/* 限制部件的尺寸变动范围 */
LCUI_API void
Widget_LimitSize(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size);

/* 限制部件的移动范围 */
LCUI_API void
Widget_LimitPos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos);

/* 设定部件的边框 */
LCUI_API void
Widget_SetBorder( LCUI_Widget *widget, LCUI_Border border );

/* 设定部件边框的四个角的圆角半径 */
LCUI_API void
Widget_SetBorderRadius( LCUI_Widget *widget, unsigned int radius );

/* 设定部件的背景图像 */
LCUI_API void
Widget_SetBackgroundImage( LCUI_Widget *widget, LCUI_Graph *img );

/* 设定背景图的布局 */
LCUI_API void
Widget_SetBackgroundLayout( LCUI_Widget *widget, LAYOUT_TYPE layout );

/* 设定部件的背景颜色 */
LCUI_API void
Widget_SetBackgroundColor( LCUI_Widget *widget, LCUI_RGB color );

/* 设定部件背景是否透明 */
LCUI_API void
Widget_SetBackgroundTransparent( LCUI_Widget *widget, LCUI_BOOL flag );

/* 启用部件 */
LCUI_API void
Widget_Enable(LCUI_Widget *widget);

/* 禁用部件 */
LCUI_API void
Widget_Disable(LCUI_Widget *widget);

/* 指定部件是否可见 */
LCUI_API void
Widget_Visible( LCUI_Widget *widget, LCUI_BOOL flag );

/*
 * 功能：设定部件的位置
 * 说明：只修改坐标，不进行局部刷新
 * */
LCUI_API void
Widget_SetPos(LCUI_Widget *widget, LCUI_Pos pos);

/* 设置部件的内边距 */
LCUI_API void
Widget_SetPadding( LCUI_Widget *widget, LCUI_Padding padding );

/* 设置部件的外边距 */
LCUI_API void
Widget_SetMargin( LCUI_Widget *widget, LCUI_Margin margin );

/* 设定部件的定位类型 */
LCUI_API void
Widget_SetPosType( LCUI_Widget *widget, POS_TYPE pos_type );

/* 获取部件的堆叠顺序 */
LCUI_API int Widget_GetZIndex( LCUI_Widget *widget );

/* 设置部件的堆叠顺序 */
LCUI_API int
Widget_SetZIndex( LCUI_Widget *widget, int z_index );

/** 获取部件的透明度 */
LCUI_API uchar_t Widget_GetAlpha( LCUI_Widget *widget );

/** 设定部件的透明度 */
LCUI_API void Widget_SetAlpha( LCUI_Widget *widget, unsigned char alpha );

/*
 * 功能：执行移动部件位置的操作
 * 说明：更改部件位置，并添加局部刷新区域
 **/
LCUI_API void
Widget_ExecMove( LCUI_Widget *widget, LCUI_Pos pos );

/* 执行隐藏部件的操作 */
LCUI_API void
Widget_ExecHide(LCUI_Widget *widget);

/* 将部件显示在同等z-index值的部件的前端 */
LCUI_API int
Widget_Front( LCUI_Widget *widget );

LCUI_API void
Widget_ExecShow(LCUI_Widget *widget);

/* 自动调整部件大小，以适应其内容大小 */
LCUI_API void
Widget_AutoResize(LCUI_Widget *widget);

/* 执行改变部件尺寸的操作 */
LCUI_API int
Widget_ExecResize(LCUI_Widget *widget, LCUI_Size size);

/* 启用或禁用部件的自动尺寸调整功能 */
LCUI_API void
Widget_SetAutoSize(	LCUI_Widget *widget,
			LCUI_BOOL flag, AUTOSIZE_MODE mode );

/* 执行刷新显示指定部件的整个区域图形的操作 */
LCUI_API void
Widget_ExecRefresh(LCUI_Widget *widget);

/* 执行部件的更新操作 */
LCUI_API void
Widget_ExecUpdate(LCUI_Widget *widget);

/* 执行部件图形更新操作 */
LCUI_API void
Widget_ExecDraw(LCUI_Widget *widget);

/* 获取指向部件自身图形数据的指针 */
LCUI_API LCUI_Graph*
Widget_GetSelfGraph( LCUI_Widget *widget );

/* 获取部件实际显示的图形 */
LCUI_API int
Widget_GetGraph(
	LCUI_Widget *widget,
	LCUI_Graph *graph_buff,
	LCUI_Rect rect );

/* 获取有效化后的坐标数据，其实就是将在限制范围外的坐标处理成在限制范围内的 */
LCUI_API LCUI_Pos
Widget_GetValidPos( LCUI_Widget *widget, LCUI_Pos pos );

/*
 * 功能：移动部件位置
 * 说明：如果部件的布局为ALIGN_NONE，那么，就可以移动它的位置，否则，无法移动位置
 * */
LCUI_API void
Widget_Move(LCUI_Widget *widget, LCUI_Pos new_pos);

/* 更新部件的位置 */
LCUI_API void
Widget_UpdatePos(LCUI_Widget *widget);

/* 更新部件的位置，以及位置变动范围 */
LCUI_API void
Widget_ExecUpdatePos( LCUI_Widget *widget );

/* 部件尺寸更新 */
LCUI_API void
Widget_UpdateSize( LCUI_Widget *widget );

/*
 * 功能：更新指定部件的子部件的尺寸
 * 说明：当部件尺寸改变后，有的部件的尺寸以及位置是按百分比算的，需要重新计算。
 * */
LCUI_API void
Widget_UpdateChildSize(LCUI_Widget *widget);

LCUI_API int
Widget_ExecUpdateSize( LCUI_Widget *widget );

/*
 * 功能：更新指定部件的子部件的位置
 * 说明：当作为子部件的容器部件的尺寸改变后，有的部件的布局不为ALIGN_NONE，就需要重新
 * 调整位置。
 * */
LCUI_API void
Widget_UpdateChildPos(LCUI_Widget *widget);

/* 设定部件的高度，单位为像素 */
LCUI_API void
Widget_SetHeight( LCUI_Widget *widget, int height );

/**
 * 功能：设定部件的尺寸大小
 * 说明：如果设定了部件的停靠位置，并且该停靠类型默认限制了宽/高，那么部件的宽/高就不能被改变。
 * 用法示例：
 * Widget_SetSize( widget, "100px", "50px" ); 部件尺寸最大为100x50像素
 * Widget_SetSize( widget, "100%", "50px" ); 部件宽度等于容器宽度，高度为50像素
 * Widget_SetSize( widget, "50", "50" ); 部件尺寸最大为50x50像素，px可以省略
 * Widget_SetSize( widget, NULL, "50%" ); 部件宽度保持原样，高度为容器高度的一半
 * */
LCUI_API void Widget_SetSize( LCUI_Widget *widget, char *width, char *height );

/** 指定部件是否为模态部件 */
LCUI_API void Widget_SetModal( LCUI_Widget *widget, LCUI_BOOL is_modal );

/** 设定部件的停靠类型 */
LCUI_API void Widget_SetDock( LCUI_Widget *widget, DOCK_TYPE dock );

/** 刷新显示指定部件的整个区域图形 */
LCUI_API void Widget_Refresh(LCUI_Widget *widget);

/** 调整部件的尺寸 */
LCUI_API void Widget_Resize( LCUI_Widget *widget, LCUI_Size new_size );

/** 重新绘制部件 */
LCUI_API void Widget_Draw( LCUI_Widget *widget );

/** 销毁部件 */
LCUI_API void Widget_Destroy( LCUI_Widget *widget );

/**
 * 功能：让部件根据已设定的属性，进行相应数据的更新
 * 说明：此记录会添加至队列，如果队列中有一条相同记录，则覆盖上条记录。
 * */
LCUI_API void Widget_Update( LCUI_Widget *widget );

/*
 * 功能：让部件根据已设定的属性，进行相应数据的更新
 * 说明：与上个函数功能一样，但是，可以允许队列中有两条相同记录。
 * */
LCUI_API void __Widget_Update( LCUI_Widget *widget );

/** 显示部件 */
LCUI_API void Widget_Show( LCUI_Widget *widget );

/** 隐藏部件 */
LCUI_API void Widget_Hide( LCUI_Widget *widget );

/** 改变部件的状态 */
LCUI_API int Widget_SetState( LCUI_Widget *widget, int state );

/** 设置部件的alpha透明度 */
LCUI_API void Widget_SetAlpha( LCUI_Widget *widget, uchar_t alpha );

/************************* Widget End *********************************/

LCUI_API LCUI_BOOL WidgetMsg_Dispatch( LCUI_Widget *widget, WidgetMsgData *data_ptr );

#endif
