/* ***************************************************************************
 * LCUI_ProgressBar.c -- LCUI's ProgressBar widget
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
 * LCUI_ProgressBar.c -- LCUI 的进度条部件
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

LCUI_Widget *Get_ProgressBar_Flash_Img_Widget(LCUI_Widget *widget)
/* 功能：获取进度条上闪光图形所在的PictureBox部件 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_PrivData(widget);
	return pb->img_pic_box;
}

static void Destroy_ProgressBar(LCUI_Widget *widget)
/* 功能：释放进度条部件占用的内存资源 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_PrivData(widget);
	Graph_Free(&pb->fore_graph);
	Graph_Free(&pb->flash_image);
}

static void ProgressBar_Init(LCUI_Widget *widget)
/* 功能：初始化进度条的数据结构体 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)
				Widget_Create_PrivData
				(widget, sizeof(LCUI_ProgressBar));
	pb->thread = 0;
	Graph_Init(&pb->fore_graph); 
	Graph_Init(&pb->flash_image);
	pb->img_move_speed = 200;
	pb->sleep_time = 100000;
	pb->max_value = 1;
	pb->value = 0;
	
	LCUI_Widget *f_pb, *img_pb;
	f_pb = Create_Widget("picture_box"); 
	img_pb = Create_Widget("picture_box");
	/* 以自己为容器，将这些部件放进去 */
	Widget_Container_Add(f_pb, img_pb);
	Widget_Container_Add(widget, f_pb); 
	/* 没有背景图时就填充背景色 */
	Set_Widget_BG_Mode(widget, BG_MODE_FILL_BACKCOLOR);
	Set_Widget_Border(widget, RGB(50,50,50), Border(1,1,1,1));
	
	Set_Widget_Style(widget, "classic"); /* 进度条为经典风格 */
	
	Set_PictureBox_Size_Mode(f_pb, SIZE_MODE_STRETCH); 
	
	Show_Widget(f_pb); 
	Show_Widget(img_pb);
	
	pb->fore_pic_box = f_pb; 
	pb->img_pic_box = img_pb;
}

static void Exec_Update_ProgressBar(LCUI_Widget *widget)
/* 功能：更新进度条的图形 */
{ 
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_PrivData(widget);
	if(Strcmp(&widget->style, "dynamic") == 0) {
		/* 绘制空槽 */
		Draw_Empty_Slot(&widget->graph, widget->size.w, widget->size.h);
		Set_Widget_Border_Style(widget, BORDER_STYLE_NONE);
		/* 载入两个图形 */
		if(!Graph_Valid(&pb->fore_graph)) 
			Load_Graph_ProgressBar_Fore(&pb->fore_graph);  
			
		if(!Graph_Valid(&pb->flash_image)) 
			Load_Graph_ProgressBar_Img(&pb->flash_image);
		
		Resize_Widget(pb->img_pic_box, Get_Graph_Size(&pb->flash_image)); 
		/* 让图片盒子显示这个图形 */
		Set_PictureBox_Image_From_Graph(pb->img_pic_box, &pb->flash_image);
	} else {
		Strcpy(&widget->style, "classic");
		if(!Graph_Valid(&pb->fore_graph)) 
			Graph_Create(&pb->fore_graph, 10, widget->size.h); 
		Graph_Fill_Color(&pb->fore_graph, RGB(80,80,200));
		Graph_Fill_Alpha(&pb->fore_graph, 255);
	}
	
	/* 让图片盒子显示这个图形 */
	Set_PictureBox_Image_From_Graph(pb->fore_pic_box, &pb->fore_graph); 
	
	int width, height;
	/* 计算进度条的长度 */ 
	width = (widget->size.w - widget->border.left - widget->border.right) 
			* pb->value / pb->max_value + 0.5;
	height = widget->size.h - widget->border.top - widget->border.bottom;
	
	Move_Widget(pb->fore_pic_box, Pos(widget->border.left, widget->border.top));
	/* 改变进度条的尺寸 */
	Resize_Widget(pb->fore_pic_box, Size(width, height));
}


void Set_ProgressBar_Max_Value(LCUI_Widget *widget, int max_value)
/* 功能：设定进度条最大值 */
{
	LCUI_ProgressBar *pb = Get_Widget_PrivData(widget);
	pb->max_value = max_value; 
	Draw_Widget(widget); 
}

int Get_ProgressBar_Max_Value(LCUI_Widget *widget)
/* 功能：获取进度条最大值 */
{
	LCUI_ProgressBar *pb = Get_Widget_PrivData(widget);
	return pb->max_value;
}

void Set_ProgressBar_Value(LCUI_Widget *widget, int value)
/* 功能：设定进度条当前值 */
{
	LCUI_ProgressBar *pb = Get_Widget_PrivData(widget);
	pb->value = value; 
	Draw_Widget(widget);
}

int Get_ProgressBar_Value(LCUI_Widget *widget)
/* 功能：获取进度条当前值 */
{
	LCUI_ProgressBar *pb = Get_Widget_PrivData(widget);
	return pb->value;
}

static void *Move_Flash_Img(void *arg)
/* 功能：移动进度条中闪光的位置 */
{
	int x=0;
	LCUI_Widget *widget = (LCUI_Widget*)arg;
	LCUI_Widget *flash = Get_ProgressBar_Flash_Img_Widget(widget);
	LCUI_ProgressBar *pb = Get_Widget_PrivData(widget);
	while(1) {
		for(x=(0-flash->size.w); 
			x<=flash->parent->size.w; 
			x+=(pb->img_move_speed/20.0+0.5)) {
			Move_Widget(flash, Pos(x , flash->pos.y));
			usleep(50000);
		}
		usleep(pb->sleep_time);
	}
	LCUI_Thread_Exit(NULL);
}

static void Show_ProgressBar(LCUI_Widget *widget)
/* 功能：在显示进度条时，处理其它操作 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_PrivData(widget);
	if(Strcmp(&widget->style, "dynamic") == 0) {
		if(pb->thread == 0) {
			Show_Widget(pb->img_pic_box);
			LCUI_Thread_Create(&pb->thread, NULL, Move_Flash_Img, (void*)widget);
		}
	} else {
		Hide_Widget(pb->img_pic_box);
		if(pb->thread != 0) {/* 否则，如果线程ID不为0，就撤销线程 */
			LCUI_Thread_Cancel(pb->thread);
			pb->thread = 0;
		}
	}
}

void Register_ProgressBar()
/* 功能：注册部件类型-进度条至部件库 */
{
	/* 添加部件类型 */
	WidgetType_Add("progress_bar");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("progress_bar",	ProgressBar_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("progress_bar",	Exec_Update_ProgressBar,FUNC_TYPE_UPDATE);
	WidgetFunc_Add("progress_bar",	Show_ProgressBar,	FUNC_TYPE_SHOW); 
	WidgetFunc_Add("progress_bar",	Destroy_ProgressBar,	FUNC_TYPE_DESTROY); 
}

