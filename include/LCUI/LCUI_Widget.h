/* ***************************************************************************
 * LCUI_Widget.h -- processing GUI widget 
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
 * LCUI_Widget.h -- 处理GUI部件
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

/* 定义数据类型标识 */
#define DATATYPE_POS	1
#define DATATYPE_SIZE	2
#define DATATYPE_GRAPH	3
#define DATATYPE_STATUS	4    
#define DATATYPE_SHOW	5
#define DATATYPE_HIDE	6
#define DATATYPE_AREA	7

/***************** 部件相关函数的类型 *******************/
typedef enum _FuncType
{
	FUNC_TYPE_SHOW,
	FUNC_TYPE_HIDE,
	FUNC_TYPE_INIT,
	FUNC_TYPE_RESIZE,
	FUNC_TYPE_UPDATE,
	FUNC_TYPE_DESTROY
}FuncType;
/****************************************************/

/***************************** Widget *********************************/

LCUI_Size Get_Widget_Size(LCUI_Widget *widget);
/* 功能：获取部件的尺寸 */

int Get_Widget_Height(LCUI_Widget *widget);

int Get_Widget_Width(LCUI_Widget *widget);

LCUI_Rect Get_Widget_Rect(LCUI_Widget *widget);
/* 功能：获取部件的区域 */ 

LCUI_Pos Get_Widget_Pos(LCUI_Widget *widget);
/* 功能：获取部件的位置 */ 

void *Get_Widget_Private_Data(LCUI_Widget *widget);
/* 功能：获取部件的私有数据结构体的指针 */ 

LCUI_Widget *Get_Widget_Parent(LCUI_Widget *widget);
/* 功能：获取部件的父部件 */ 

void print_widget_info(LCUI_Widget *widget);
/* 
 * 功能：打印widget的信息
 * 说明：在调试时需要用到它，用于确定widget是否有问题
 * */ 

int Add_Widget_Refresh_Area (LCUI_Widget * widget, LCUI_Rect rect);
/* 功能：在指定部件的内部区域内设定需要刷新的区域 */ 

void Set_Widget_Border_Style(LCUI_Widget *widget, LCUI_Border_Style style);
/* 功能：设定部件的边框风格 */ 

void Split_Screen_Area_By_Widget(LCUI_Widget *widget);
/* 
 * 功能：根据部件队列中的部件区域，来对区域队列中的区域进行分割
 * 说明：分割后的区域会添加至rect_queue中，并删除被分割的区域。
 * */ 

void Response_Status_Change(LCUI_Widget *widget);
/* 
 * 功能：让指定部件响应部件状态的改变
 * 说明：部件创建时，默认是不响应状态改变的，因为每次状态改变后，都要调用函数重绘部件，
 * 这对于一些部件是多余的，没必要重绘，影响效率。如果想让部件能像按钮那样，鼠标移动到它
 * 上面时以及鼠标点击它时，都会改变按钮的图形样式，那就需要用这个函数设置一下。
 *  */ 

void Shift_Widget_Refresh_Area(LCUI_Widget *widget);
/* 功能：转移部件的rect队列成员至父部件中 */ 

void Processing_Refresh_Area();
/*
 * 功能：处理已记录的刷新区域
 * 说明：此函数会将各个部件的rect队列中的处理掉，并将
 * 最终的局部刷新区域数据添加至屏幕刷新区域队列中，等
 * 待LCUI来处理。
 **/ 

LCUI_Widget *Get_Parent_Widget(LCUI_Widget *widget, char *widget_type);
/*
 * 功能：获取部件的指定类型的父部件的指针
 * 说明：本函数会在部件关系链中往头部查找父部件指针，并判断这个父部件是否为制定类型
 * 返回值：没有符合要求的父级部件就返回NULL，否则返回部件指针
 **/ 

int LCUI_Destroy_App_Widgets(LCUI_ID app_id);
/* 功能：销毁指定ID的程序的所有部件 */ 

LCUI_String Get_Widget_Style(LCUI_Widget *widget);
/* 功能：获取部件的类型 */ 

LCUI_Border_Style Get_Widget_Border_Style(LCUI_Widget *widget);
/* 功能：获取部件的边框类型 */ 

void Set_Widget_Style(LCUI_Widget *widget, char *style);
/* 功能：设定部件的风格 */ 

LCUI_Widget *Get_Widget_By_Pos(LCUI_Widget *widget, LCUI_Pos pos);
/* 功能：获取部件中包含指定坐标的点的子部件 */ 

LCUI_Widget *Get_Cursor_Overlay_Widget();
/* 功能：获取鼠标光标当前覆盖的部件 */ 

LCUI_Widget *Get_Focus_Widget();
/*
 * 功能：获取当前焦点状态下的部件
 * 返回值：正常获取则返回部件的指针，否则，返回NULL
 **/ 

int Widget_Is_Active(LCUI_Widget *widget);
/* 功能：判断部件是否为活动状态 */ 

LCUI_Rect Get_Widget_Valid_Rect_By_New_Pos(LCUI_Widget *widget, LCUI_Pos offset);
/* 
 * 功能：获取移动到新位置的部件在屏幕上的有效显示区域 
 * 说明：offset是原位置与新位置的差
 * */ 

LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget);
/* 功能：获取部件在屏幕中实际显示的区域 */ 

int Empty_Widget();
/* 
 * 功能：用于检测程序的部件列表是否为空 
 * 返回值：
 *   1  程序的部件列表为空
 *   0  程序的部件列表不为空
 * */ 

LCUI_Widget *Create_Widget(char *widget_type);
/* 
 * 功能：创建指定类型的窗口部件
 * 说明：创建出来的部件，默认是没有背景图时透明。
 * 返回值：成功则部件的指针，失败则返回NULL
 */ 

void Delete_Widget(LCUI_Widget *widget);
/* 功能：删除一个部件 */ 

LCUI_Pos Count_Widget_Pos(LCUI_Widget *widget);
/* 功能：累计部件的位置坐标 */ 

LCUI_Pos Get_Widget_Global_Pos(LCUI_Widget *widget);
/* 功能：获取部件的全局坐标 */ 

void Set_Widget_BG_Mode(LCUI_Widget *widget, LCUI_BG_Mode bg_mode);
/*
 * 功能：改变部件的背景模式
 * 说明：背景模式决定了部件在没有背景图的时候是使用背景色填充还是完全透明。
 **/ 

void Set_Widget_Align(LCUI_Widget *widget, LCUI_Align align, LCUI_Pos offset);
/* 功能：设定部件的对齐方式以及偏移距离 */ 

void Limit_Widget_Size(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size);
/* 功能：限制部件的尺寸变动范围 */ 

void Limit_Widget_Pos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos);
/* 功能：限制部件的移动范围 */ 

void Set_Widget_Border(LCUI_Widget *widget, LCUI_RGB color, LCUI_Border border);
/* 功能：设定部件的边框 */ 

void Set_Widget_Backcolor(LCUI_Widget *widget, LCUI_RGB color);
/* 功能：设定部件的背景色 */ 

int Set_Widget_Background_Image(LCUI_Widget *widget, LCUI_Graph *img, int flag);
/* 功能：为部件填充背景图像 */ 

int Mix_Widget_FontBitmap(	LCUI_Widget *widget, 
							int start_x, int start_y, 
							LCUI_WString *contents, 
							int rows, int space, 
							int linegap, int flag	);
/* 功能：混合部件内的字体位图，使字体能在部件上显示 */ 

void Enable_Widget(LCUI_Widget *widget);
/* 功能：启用部件 */ 

void Disable_Widget(LCUI_Widget *widget);
/* 功能：禁用部件 */ 

void Widget_Visible(LCUI_Widget *widget, int flag);
/* 功能：定义部件是否可见 */ 

void Set_Widget_Pos(LCUI_Widget *widget, LCUI_Pos pos);
/* 
 * 功能：设定部件的位置 
 * 说明：只修改坐标，不进行局部刷新
 * */ 

void Set_Widget_Alpha(LCUI_Widget *widget, unsigned char alpha);
/* 功能：设定部件的透明度 */ 

void Exec_Move_Widget(LCUI_Widget *widget, LCUI_Pos pos);
/*
 * 功能：执行移动部件位置的操作
 * 说明：更改部件位置，并添加局部刷新区域
 **/ 

void Exec_Hide_Widget(LCUI_Widget *widget);
/* 功能：执行隐藏部件的操作 */ 

void Exec_Show_Widget(LCUI_Widget *widget);
/* 功能：执行显示部件的任务 */ 

void Auto_Resize_Widget(LCUI_Widget *widget);
/* 功能：自动调整部件大小，以适应其内容大小 */ 

void Exec_Resize_Widget(LCUI_Widget *widget, LCUI_Size size);
/* 功能：执行改变部件尺寸的操作 */ 

void Enable_Widget_Auto_Size(LCUI_Widget *widget);
/* 功能：启用部件自动尺寸调整功能 */ 

void Disable_Widget_Auto_Size(LCUI_Widget *widget);
/* 功能：禁用部件自动尺寸调整功能 */ 

void Exec_Refresh_Widget(LCUI_Widget *widget);
/* 功能：执行刷新显示指定部件的整个区域图形的操作 */ 

void Exec_Draw_Widget(LCUI_Widget *widget);
/* 功能：执行部件图形更新操作 */ 

LCUI_Pos Widget_Align_Get_Pos(LCUI_Widget *widget);
/* 功能：根据部件的布局来获取部件的位置 */ 

void Move_Widget(LCUI_Widget *widget, LCUI_Pos new_pos);
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

void Offset_Widget_Pos(LCUI_Widget *widget, LCUI_Pos offset);
/* 功能：以部件原有的位置为基础，根据指定的偏移坐标偏移位置 */ 

void Move_Widget_To_Pos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed);
/* 
 * 功能：将部件以指定的速度向指定位置移动 
 * 说明：des_pos是目标位置，speed是该部件的移动速度，单位为：像素/秒
 * */ 

void Refresh_Widget(LCUI_Widget *widget);
/* 功能：刷新显示指定部件的整个区域图形 */ 

void Resize_Widget(LCUI_Widget *widget, LCUI_Size new_size);
/* 功能：改变部件的尺寸 */ 

void Draw_Widget(LCUI_Widget *widget);
/* 功能：重新绘制部件 */ 

void Front_Widget(LCUI_Widget *widget);
/* 功能：将指定部件的显示位置移动到最前端 */ 

void Show_Widget(LCUI_Widget *widget);
/* 功能：显示部件 */ 

void Hide_Widget(LCUI_Widget *widget);
/* 功能：隐藏部件 */ 

void Set_Widget_Status(LCUI_Widget *widget, int status);
/* 功能：设定部件的状态 */ 
/************************* Widget End *********************************/


/**************************** Widget Update ***************************/ 
int Processing_WidgetUpdate(LCUI_Widget *widget);
/* 功能：处理部件的更新 */ 

void Processing_All_WidgetUpdate();
/* 功能：处理所有部件的更新 */ 
/************************ Widget Update End ***************************/



/************************** Widget Library ****************************/

int WidgetFunc_Add(
			char *type, 
			void (*widget_func)(LCUI_Widget*), 
			FuncType func_type
		);
/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/ 

int WidgetType_Add(char *type);
/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/ 

void WidgetLib_Init(LCUI_Queue *w_lib);
/* 功能：初始化部件库 */ 

int WidgetType_Delete(char *type);
/* 功能：删除指定部件类型的相关数据 */ 

void NULL_Widget_Func(LCUI_Widget *widget);
/*
 * 功能：空函数，不做任何操作
 * 说明：如果获取指定部件类型的函数指针失败，将返回这个函数的函数指针
 **/ 

LCUI_ID WidgetType_Get_ID(char *widget_type);
/* 功能：获取指定类型部件的类型ID */ 

int Get_Widget_Type_By_ID(LCUI_ID id, char *widget_type);
/* 功能：获取指定类型ID的类型名称 */ 

void ( *Get_WidgetFunc_By_ID(LCUI_ID id, FuncType func_type) ) (LCUI_Widget*);
/* 功能：获取指定部件类型ID的函数的函数指针 */ 

void ( *Get_WidgetFunc(char *widget_type, FuncType func_type) ) (LCUI_Widget*);
/* 功能：获取指定类型部件的函数的函数指针 */ 

int Check_WidgetType(char *widget_type);
/* 功能：检测指定部件类型是否有效 */

void Register_Default_Widget_Type();
/* 功能：为程序的部件库添加默认的部件类型 */ 
/************************ Widget Library End **************************/


/*************************** Container ********************************/
void Widget_Container_Add(LCUI_Widget *container, LCUI_Widget *widget);
/* 功能：将部件添加至作为容器的部件内 */ 
/************************* Container End ******************************/

#endif
