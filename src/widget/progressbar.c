/* ***************************************************************************
 * progressbar.c -- LCUI's ProgressBar widget
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
 * progressbar.c -- LCUI 的进度条部件
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

#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_WIDGET_H
#include LC_PROGBAR_H
#include LC_PICBOX_H 
#include LC_RES_H 

#include <unistd.h>

/* 功能：获取进度条上闪光图形所在的PictureBox部件 */
static LCUI_Widget *
ProgressBar_GetMoveingLight(LCUI_Widget *widget)
{
	LCUI_ProgressBar *pb;
	pb = Widget_GetPrivData(widget);
	return pb->img_pic_box;
}

static void 
Destroy_ProgressBar(LCUI_Widget *widget)
/* 功能：释放进度条部件占用的内存资源 */
{
	LCUI_ProgressBar *pb;
	pb = Widget_GetPrivData(widget);
	Graph_Free(&pb->fore_graph);
	Graph_Free(&pb->flash_image);
}

static void 
ProgressBar_Init(LCUI_Widget *widget)
/* 功能：初始化进度条的数据结构体 */
{
	LCUI_ProgressBar *pb;
	
	pb = WidgetPrivData_New (widget, sizeof(LCUI_ProgressBar));
	pb->thread = 0;
	Graph_Init(&pb->fore_graph); 
	Graph_Init(&pb->flash_image);
	pb->img_move_speed = 200;
	pb->sleep_time = 100000;
	pb->max_value = 1;
	pb->value = 0;
	
	LCUI_Widget *f_pb, *img_pb;
	f_pb = Widget_New("picture_box"); 
	img_pb = Widget_New("picture_box");
	/* 以自己为容器，将这些部件放进去 */
	Widget_Container_Add( f_pb, img_pb );
	Widget_Container_Add( widget, f_pb ); 
	/* 没有背景图时就填充背景色 */
	Widget_SetBackgroundTransparent( widget, FALSE );
	/* 进度条为经典风格 */
	Widget_SetStyleName( widget, "classic" ); 
	
	Set_PictureBox_Size_Mode( f_pb, SIZE_MODE_STRETCH ); 
	
	Widget_Show(f_pb); 
	Widget_Show(img_pb);
	
	pb->fore_pic_box = f_pb; 
	pb->img_pic_box = img_pb;
}

static void 
ProgressBar_ExecUpdate(LCUI_Widget *widget)
/* 功能：更新进度条数据 */
{
	static double scale;
	static char scale_str[15];
	static LCUI_ProgressBar *pb;
	
	pb = Widget_GetPrivData(widget);
	/* 计算进度条的长度 */ 
	scale = 100.0 * pb->value / pb->max_value;
	sprintf( scale_str, "%.2lf%%", scale );
	/* 改变进度条的尺寸 */
	Widget_SetSize( pb->fore_pic_box, scale_str, "100%" );
}

static void 
ProgressBar_ExecDraw(LCUI_Widget *widget)
/* 功能：更新进度条的图形 */
{
	LCUI_ProgressBar *pb;
	LCUI_Graph *widget_graph;
	
	widget_graph = Widget_GetSelfGraph( widget );
	pb = Widget_GetPrivData(widget);
	if(Strcmp(&widget->style_name, "dynamic") == 0) {
		/* 绘制空槽 */
		Draw_Empty_Slot(widget_graph, widget->size.w, widget->size.h);
		/* 载入两个图形 */
		if(!Graph_Valid(&pb->fore_graph)) {
			Load_Graph_ProgressBar_Fore(&pb->fore_graph);  
		}
		if(!Graph_Valid(&pb->flash_image)) {
			Load_Graph_ProgressBar_Img(&pb->flash_image);
		}
		
		Widget_Resize(pb->img_pic_box, Get_Graph_Size(&pb->flash_image));
		/* 让图片盒子显示这个图形 */
		Set_PictureBox_Image_From_Graph(pb->img_pic_box, &pb->flash_image);
		Widget_SetPadding( widget, Padding(1,1,1,1) );
	} else {
		Strcpy(&widget->style_name, "classic");
		if(!Graph_Valid(&pb->fore_graph)) {
			Graph_Create(&pb->fore_graph, 10, widget->size.h); 
		}
		Graph_Fill_Color(&pb->fore_graph, RGB(80,80,200));
		Graph_Fill_Alpha(&pb->fore_graph, 255);
		Widget_SetBackgroundColor( widget, RGB(255,255,255) );
		Widget_SetBackgroundTransparent( widget, FALSE );
		Widget_SetBorder( widget,
		 Border(1, BORDER_STYLE_SOLID, RGB(50,50,50)) );
		Widget_SetPadding( widget, Padding(1,1,1,1) );
	}
	
	/* 让图片盒子显示这个图形 */
	Set_PictureBox_Image_From_Graph(pb->fore_pic_box, &pb->fore_graph);
}

void ProgressBar_SetMaxValue(LCUI_Widget *widget, int max_value)
/* 功能：设定进度条最大值 */
{
	LCUI_ProgressBar *pb = Widget_GetPrivData(widget);
	pb->max_value = max_value; 
	Widget_Update(widget); 
}

int ProgressBar_GetMaxValue(LCUI_Widget *widget)
/* 功能：获取进度条最大值 */
{
	LCUI_ProgressBar *pb = Widget_GetPrivData(widget);
	return pb->max_value;
}

void ProgressBar_SetValue(LCUI_Widget *widget, int value)
/* 功能：设定进度条当前值 */
{
	LCUI_ProgressBar *pb = Widget_GetPrivData(widget);
	pb->value = value; 
	Widget_Update(widget);
}

int ProgressBar_GetValue(LCUI_Widget *widget)
/* 功能：获取进度条当前值 */
{
	LCUI_ProgressBar *pb = Widget_GetPrivData(widget);
	return pb->value;
}

static void *
ProgressBar_ProcMoveingLight(void *arg)
/* 功能：移动进度条中闪光的位置 */
{
	int x=0;
	LCUI_Widget *widget = (LCUI_Widget*)arg;
	LCUI_Widget *flash = ProgressBar_GetMoveingLight(widget);
	LCUI_ProgressBar *pb = Widget_GetPrivData(widget);
	while(1) {
		for(x=(0-flash->size.w); 
			x<=flash->parent->size.w; 
			x+=(pb->img_move_speed/20.0+0.5)) {
			Widget_Move(flash, Pos(x , flash->pos.y));
			usleep(50000);
		}
		usleep(pb->sleep_time);
	}
	LCUI_Thread_Exit(NULL);
}

static void 
ProgressBar_ExecShow(LCUI_Widget *widget)
/* 功能：在显示进度条时，处理其它操作 */
{
	LCUI_ProgressBar *pb;
	
	pb = Widget_GetPrivData(widget);
	if(Strcmp(&widget->style_name, "dynamic") == 0) {
		if(pb->thread == 0) {
			Widget_Show(pb->img_pic_box);
			LCUI_Thread_Create(&pb->thread, NULL, ProgressBar_ProcMoveingLight, (void*)widget);
		}
	} else {
		Widget_Hide(pb->img_pic_box);
		if(pb->thread != 0) {/* 否则，如果线程ID不为0，就撤销线程 */
			LCUI_Thread_Cancel(pb->thread);
			pb->thread = 0;
		}
	}
}

void Register_ProgressBar()
/* 注册进度条部件类型 */
{
	/* 添加部件类型 */
	WidgetType_Add("progress_bar");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("progress_bar",	ProgressBar_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("progress_bar",	ProgressBar_ExecUpdate,FUNC_TYPE_UPDATE);
	WidgetFunc_Add("progress_bar",	ProgressBar_ExecDraw,	FUNC_TYPE_DRAW);
	WidgetFunc_Add("progress_bar",	ProgressBar_ExecShow,	FUNC_TYPE_SHOW); 
	WidgetFunc_Add("progress_bar",	Destroy_ProgressBar,	FUNC_TYPE_DESTROY); 
}

