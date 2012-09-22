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
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_WIDGET_H
#include LC_PROGBAR_H
#include LC_PICBOX_H 
#include LC_RES_H
#include LC_MEM_H
#include <unistd.h>
#include <pthread.h>

int Draw_Empty_Slot(LCUI_Graph *graph, int width, int height)
/* 功能：绘制进度条的空槽 */
{
	if(width < 4 || height < 4) {
		return -1;
	}
	graph->have_alpha = IS_TRUE;
	Malloc_Graph(graph, width, height); /* 申请内存 */
	Fill_Color(graph, RGB(250,250,250));/* 填充白色 */
	Fill_Graph_Alpha(graph, 255);
	
	Using_Graph( graph, 1 );
	/* 四个角上的一个像素点完全透明 */
	graph->rgba[3][0] = 0;
	graph->rgba[3][width-1] = 0;
	graph->rgba[3][width*(height-1)] = 0;
	graph->rgba[3][width*height-1] = 0;
	
	int i, n;
	unsigned char c;
	/* 绘制左边和右边的竖线条 */
	for(i=0; i<height; ++i)
	{
		n = i*width; 
		c = 160 +(30-(30.0/height)*i);
		graph->rgba[0][n] = c;
		graph->rgba[1][n] = c;
		graph->rgba[2][n] = c;
		graph->rgba[0][n+1] = 230;
		graph->rgba[1][n+1] = 230;
		graph->rgba[2][n+1] = 230; 
		
		n = n+width-1;
		graph->rgba[0][n] = c;
		graph->rgba[1][n] = c;
		graph->rgba[2][n] = c;
		graph->rgba[0][n-1] = 230;
		graph->rgba[1][n-1] = 230;
		graph->rgba[2][n-1] = 230;
	}
	
	/* 绘制顶端的线条 */
	memset(graph->rgba[0], 180, width);
	memset(graph->rgba[1], 180, width);
	memset(graph->rgba[2], 180, width);
	
	memset(graph->rgba[0]+width+1, 240, width-2); 
	memset(graph->rgba[1]+width+1, 240, width-2); 
	memset(graph->rgba[2]+width+1, 240, width-2); 
	
	/* 绘制底端的线条 */
	n = width*(height-1);
	memset(graph->rgba[0]+n, 140, width);
	memset(graph->rgba[1]+n, 140, width);
	memset(graph->rgba[2]+n, 140, width);
	
	n = width*(height-2);
	memset(graph->rgba[0]+n+1, 225, width-2); 
	memset(graph->rgba[1]+n+1, 225, width-2); 
	memset(graph->rgba[2]+n+1, 225, width-2); 
	End_Use_Graph( graph );
	return 0;
}

LCUI_Widget *Get_ProgressBar_Flash_Img_Widget(LCUI_Widget *widget)
/* 功能：获取进度条上闪光图形所在的PictureBox部件 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	return pb->img_pic_box;
}

static void Destroy_ProgressBar(LCUI_Widget *widget)
/* 功能：释放进度条部件占用的内存资源 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	Free_Graph(&pb->fore_graph);
	Free_Graph(&pb->flash_image);
}

static void ProgressBar_Init(LCUI_Widget *widget)
/* 功能：初始化进度条的数据结构体 */
{
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)
				Malloc_Widget_Private
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
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
	if(Strcmp(&widget->style, "dynamic") == 0) {
		/* 绘制空槽 */
		Draw_Empty_Slot(&widget->graph, widget->size.w, widget->size.h);
		Set_Widget_Border_Style(widget, BORDER_STYLE_NONE);
		/* 载入两个图形 */
		if(!Valid_Graph(&pb->fore_graph)) 
			Load_Graph_ProgressBar_Fore(&pb->fore_graph);  
			
		if(!Valid_Graph(&pb->flash_image)) 
			Load_Graph_ProgressBar_Img(&pb->flash_image);
		
		Resize_Widget(pb->img_pic_box, 
				Size(pb->flash_image.width, pb->flash_image.height)); 
		/* 让图片盒子显示这个图形 */
		Set_PictureBox_Image_From_Graph(pb->img_pic_box, &pb->flash_image);
	} else {
		Strcpy(&widget->style, "classic");
		if(!Valid_Graph(&pb->fore_graph)) 
			Malloc_Graph(&pb->fore_graph, 10, widget->size.h); 
		Fill_Color(&pb->fore_graph, RGB(80,80,200));
		Fill_Graph_Alpha(&pb->fore_graph, 255);
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
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	pb->max_value = max_value; 
	Draw_Widget(widget); 
}

int Get_ProgressBar_Max_Value(LCUI_Widget *widget)
/* 功能：获取进度条最大值 */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	return pb->max_value;
}

void Set_ProgressBar_Value(LCUI_Widget *widget, int value)
/* 功能：设定进度条当前值 */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	pb->value = value; 
	Draw_Widget(widget);
}

int Get_ProgressBar_Value(LCUI_Widget *widget)
/* 功能：获取进度条当前值 */
{
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
	return pb->value;
}

static void *Move_Flash_Img(void *arg)
/* 功能：移动进度条中闪光的位置 */
{
	int x=0;
	LCUI_Widget *widget = (LCUI_Widget*)arg;
	LCUI_Widget *flash = Get_ProgressBar_Flash_Img_Widget(widget);
	LCUI_ProgressBar *pb = Get_Widget_Private_Data(widget);
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
	LCUI_ProgressBar *pb = (LCUI_ProgressBar*)Get_Widget_Private_Data(widget);
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

