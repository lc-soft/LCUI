/* ***************************************************************************
 * LCUI_PictureBox.c -- LCUI's PictureBox widget
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
 * LCUI_PictureBox.c -- LCUI 的图像框部件
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
#include LC_PICBOX_H
#include LC_MEM_H
static void Destroy_PictureBox(LCUI_Widget* widget)
/* 功能：释放图片盒子占用的内存资源 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	Free_Graph(&pic_box->buff_graph); 
	Free_Graph(&pic_box->error_image);
	Free_Graph(&pic_box->initial_image); 
}

static void PictureBox_Init(LCUI_Widget *widget)
/* 功能：初始化图片盒子 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
	Malloc_Widget_Private(widget, sizeof(LCUI_PictureBox));;
	
	Rect_Init(&pic_box->read_box);
	pic_box->image		= NULL; 
	pic_box->scale		= 1.0;
	pic_box->size_mode	= SIZE_MODE_CENTER; 
	Graph_Init(&pic_box->buff_graph); 
	Graph_Init(&pic_box->error_image);
	Graph_Init(&pic_box->initial_image); 
}

LCUI_Rect Get_PictureBox_View_Area(LCUI_Widget *widget)
/* 功能：获取图片盒子的图片显示的区域 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	return pic_box->read_box;
}

LCUI_Pos Get_PictureBox_View_Area_Pos(LCUI_Widget *widget)
/* 功能：获取获取图片盒子的图片显示的区域的坐标 */
{
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(widget);
	return Pos(rect.x, rect.y);
}

static void Exec_Update_PictureBox(LCUI_Widget *widget)
/* 功能：更新PictureBox部件 */
{
	LCUI_PictureBox *pic_box;
	LCUI_Graph graph, *p; 
	int width, height; 
	LCUI_Pos pos;
	pos = Pos(0,0);
	Graph_Init(&graph); 
	pic_box  = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	/************************
	printf("Exec_Update_PictureBox start\n");
	Print_Graph_Info(pic_box->image);
	printf("pic_box:%p, parent:%s, visible:%d, pos:%d,%d, size:%d,%d\n", 
		widget,widget->parent->type.string, widget->parent->visible,widget->pos.x,
		widget->pos.y, widget->size.w, widget->size.h);
	* ***********************/
	if(Valid_Graph(pic_box->image)) {/* 如果有图片. 并且图片有效 */ 
		switch(pic_box->size_mode) {
		case SIZE_MODE_ZOOM:
		/* 裁剪图像 */ 
			if(pic_box->scale == 1.00) p = pic_box->image; 
			else p = &pic_box->buff_graph;
				
			width = pic_box->read_box.width;
			height = pic_box->read_box.height;
			pos.x = (widget->size.w - width)/2.0;
			pos.y = (widget->size.h - height)/2.0; 
			/* 引用图像中指定区域的图形 */
			Quote_Graph(&graph, p, pic_box->read_box);   
			break;
			 
		case SIZE_MODE_NORMAL:/* 正常模式 */ 
			/* 引用图像中指定区域的图形 */
			Quote_Graph(&graph, pic_box->image, pic_box->read_box); 
			break;
			
		case SIZE_MODE_STRETCH:/* 拉伸模式 */ 
			/* 开始缩放图片 */
			Zoom_Graph( pic_box->image, &graph, CUSTOM, widget->size ); 
			break;
			
		case SIZE_MODE_TILE:/* 平铺模式 */ 
			Tile_Graph( pic_box->image, &graph, widget->size.w, widget->size.h); 
			break;
			
		case SIZE_MODE_CENTER:
			/* 判断图像的尺寸是否小于图片盒子的尺寸，并计算坐标位置 */
			if(pic_box->image->width < widget->size.w) {
				pic_box->read_box.x = 0;
				pic_box->read_box.width = pic_box->image->width;
				pos.x = (widget->size.w - pic_box->image->width)/2 + 0.5;
			}
			if(pic_box->image->height < widget->size.h) {
				pos.y = (widget->size.h - pic_box->image->height)/2 + 0.5;
				pic_box->read_box.y = 0;
				pic_box->read_box.height = pic_box->image->height;
			}
			if(pic_box->read_box.y + pic_box->read_box.height >= pic_box->image->height) 
			/* 如果读取区域的尺寸大于图片尺寸 */
				pic_box->read_box.y = pic_box->image->height - pic_box->read_box.height;
			if(pic_box->read_box.x + pic_box->read_box.width >= pic_box->image->width) 
				pic_box->read_box.x = pic_box->image->width - pic_box->read_box.width;
				
			Quote_Graph(&graph, pic_box->image, pic_box->read_box); 
			break;
			default : break;
		}
		if(!Valid_Graph(&widget->background_image)) 
			Replace_Graph(&widget->graph, &graph, pos); 
		else Mix_Graph(&widget->graph, &graph, pos);  
		Free_Graph(&graph);
	}
	//printf("Exec_Update_PictureBox end\n");
	Refresh_Widget(widget); 
}

float Get_PictureBox_Zoom_Scale(LCUI_Widget *widget)
/* 功能：获取图片盒子的缩放比例 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
			Get_Widget_Private_Data(widget);
	return pic_box->scale;
}



void Set_PictureBox_Image_From_Graph(LCUI_Widget *widget, LCUI_Graph *image)
/* 功能：添加一个图片数据至图片盒子 */
{ 
	int i;
	float scale_x,scale_y;
	LCUI_Graph *graph = image; 
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
			Get_Widget_Private_Data(widget);
	
	/* 图片更换了，就释放缓存图形 */
	Free_Graph(&pic_box->buff_graph);
	
	for(i = 0;i < 2; ++i) {
		if(Valid_Graph(graph)) {/* 如果image有效 */
			/* 保存图像数据指针 */ 
			pic_box->image = graph;
			/* 读取的范区域为整个图片区域 */
			pic_box->read_box.x = 0;
			pic_box->read_box.y = 0;
			pic_box->read_box.width = graph->width;
			pic_box->read_box.height = graph->height;
			pic_box->read_box.center_x = 0.5;
			pic_box->read_box.center_y = 0.5;
			pic_box->scale = 1; /* 缩放比例为100% */
			switch(pic_box->size_mode) {
			case SIZE_MODE_ZOOM:
				scale_x = (float)widget->size.w / pic_box->read_box.width;
				scale_y = (float)widget->size.h / pic_box->read_box.height;
				if(scale_x < scale_y) pic_box->scale = scale_x;
				else pic_box->scale = scale_y;
				break;
			case SIZE_MODE_NORMAL:/* 正常模式 */
				break;
			case SIZE_MODE_STRETCH:/* 拉伸模式 */
				break;
			case SIZE_MODE_TILE:/* 平铺模式 */
				break;
			case SIZE_MODE_CENTER:
				/* 判断图像的尺寸是否超出图片盒子的尺寸 */
				if(pic_box->image->width >= widget->size.w) {
					pic_box->read_box.x = (pic_box->image->width - widget->size.w)/2;
					pic_box->read_box.width = widget->size.w;
				}
				if(pic_box->image->height >= widget->size.h) {
					pic_box->read_box.y = (pic_box->image->height - widget->size.h)/2;
					pic_box->read_box.height = widget->size.h;
				}
				break;
				default : break;
			}
			break;
		}
		else if(pic_box->image_status == IMAGE_STATUS_LOADING) 
			/* 使用对应的图片 */
			graph = &pic_box->initial_image; 
		else { /* 使用对应的图片 */
			graph = &pic_box->error_image;
			pic_box->image_status = IMAGE_STATUS_FAIL;
		}
	}
	Draw_Widget(widget);
	Refresh_Widget(widget); 
}

int Set_PictureBox_Image_From_File(LCUI_Widget *widget, char *image_file)
/* 功能：添加一个图片文件，并载入至图片盒子 */
{
	int value;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
			Get_Widget_Private_Data(widget);
	LCUI_Graph *graph = (LCUI_Graph*)
			calloc(1, sizeof(LCUI_Graph));
			
	pic_box->image_status = IMAGE_STATUS_LOADING; /* 图片状态为正在载入 */
	Set_PictureBox_Image_From_Graph(widget, NULL);
	value = Load_Image(image_file, graph);/* 载入图片文件 */
	if(value != 0) {
		/* 载入失败 */
		pic_box->image_status = IMAGE_STATUS_FAIL;
		Set_PictureBox_Image_From_Graph(widget, NULL);
	} else {
		/* 载入成功 */
		pic_box->image_status = IMAGE_STATUS_SUCCESS;
		Set_PictureBox_Image_From_Graph(widget, graph);
	}
	return value;
}



int Set_PictureBox_ErrorImage(LCUI_Widget *widget, LCUI_Graph *pic)
/* 功能：设定当加载图像失败时显示的图像 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	
	if(Valid_Graph(pic)) {
		Copy_Graph(&pic_box->error_image, pic);
		return 0;
	}
	return -1;
}

int Set_PictureBox_InitImage(LCUI_Widget *widget, LCUI_Graph *pic)
/* 功能：设定正在加载另一图像时显示的图像 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)Get_Widget_Private_Data(widget);
	
	if(Valid_Graph(pic)) {
		Copy_Graph(&pic_box->initial_image, pic);
		return 0;
	}
	return -1;
}

void Set_PictureBox_Size_Mode(LCUI_Widget *widget, int mode)
/* 功能：设定图片盒子的图像显示模式 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	
	if(pic_box->size_mode != mode) {
		float scale_x,scale_y;
		pic_box->size_mode = mode;
		if(pic_box->image == NULL) return;
		switch(mode) {
		case SIZE_MODE_ZOOM:
			scale_x = (float)widget->size.w / pic_box->image->width;
			scale_y = (float)widget->size.h / pic_box->image->height;
			if(scale_x < scale_y) pic_box->scale = scale_x;
			else pic_box->scale = scale_y;
			/*
			 * pic_box->read_box.x = 0;
			 * pic_box->read_box.y = 0;
			 * pic_box->read_box.width = pic_box->image->width;
			 * pic_box->read_box.height = pic_box->image->height;
			 * */
			pic_box->read_box.center_x = 0.5;
			pic_box->read_box.center_y = 0.5;
			Zoom_PictureBox_View_Area(widget, pic_box->scale); 
			break;
		default: break;
		}
		Draw_Widget(widget);
		Refresh_Widget(widget); 
	} 
}


void Resize_PictureBox_View_Area(LCUI_Widget *widget, int width, int height)
/* 功能：设定PictureBox部件的图片显示区域的大小 */
{
	LCUI_Pos start, center_pos;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	
	if(width <= 0 || height <= 0) return;
	
	if(!Valid_Graph(pic_box->image)) return;
	/* 将中心点位置转换成坐标 */
	center_pos.x = pic_box->read_box.center_x * pic_box->scale * pic_box->image->width;
	center_pos.y = pic_box->read_box.center_y * pic_box->scale * pic_box->image->height;
	/* 处理区域数据，使之为有效区域 */
	start.x = center_pos.x - width/2.0;
	start.y = center_pos.y - height/2.0;
	if(start.x < 0) start.x = 0;
	if(start.y < 0) start.y = 0;
	if(start.x + width > pic_box->image->width)
	start.x = pic_box->image->width - width;
	if(start.y + height > pic_box->image->height)
	start.y = pic_box->image->height - height;
	if(start.x < 0) {
		start.x = 0;
		width = pic_box->image->width;
	}
	if(start.y < 0) {
		start.y = 0;
		height = pic_box->image->height;
	}
	pic_box->read_box.x = start.x;
	pic_box->read_box.y = start.y;
	pic_box->read_box.width = width;
	pic_box->read_box.height = height;
	/* 更新图片盒子内的图像 */
	Draw_Widget(widget);
	Refresh_Widget(widget); 
}

LCUI_Graph *Get_PictureBox_Graph(LCUI_Widget *widget)
/* 功能：获取PictureBox部件内的图像指针 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
			Get_Widget_Private_Data(widget);
	return pic_box->image;
}

int Move_PictureBox_View_Area(LCUI_Widget *widget, LCUI_Pos des_pos)
/* 功能：移动图片盒子内的图片的显示区域的位置 */
{
	LCUI_Size size;
	LCUI_Graph *p;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	
	if(!Valid_Graph(pic_box->image)) return -1;
		
	if(pic_box->scale == 1.00 || !Valid_Graph(&pic_box->buff_graph))
		p = pic_box->image;
	else p = &pic_box->buff_graph;
		
	size.w = pic_box->read_box.width;
	size.h = pic_box->read_box.height;
	/* 处理区域数据，使之为有效区域 */
	if(des_pos.x < 0) des_pos.x = 0;
	if(des_pos.y < 0) des_pos.y = 0;
	if(des_pos.x + size.w > p->width)
	des_pos.x = p->width - size.w;
	if(des_pos.y + size.h > p->height)
	des_pos.y = p->height - size.h;
	
	if(des_pos.x == pic_box->read_box.x 
	&& des_pos.y == pic_box->read_box.y)
		return 0; 
		
	/* 更新图片盒子内的图像 */
	pic_box->read_box.x = des_pos.x;
	pic_box->read_box.y = des_pos.y;
	/* 重新计算中心点的位置 */ 
	pic_box->read_box.center_x = (des_pos.x + size.w/2.0)/p->width;
	pic_box->read_box.center_y = (des_pos.y + size.h/2.0)/p->height;
	
	Draw_Widget(widget);
	//
	//printf("read box: %d,%d,%d,%d\n", 
	//pic_box->read_box.x, pic_box->read_box.y, 
	//pic_box->read_box.width, pic_box->read_box.height);
	return 0;
}


int Zoom_PictureBox_View_Area(LCUI_Widget *widget, float scale)
/* 功能：缩放PictureBox部件的图片浏览区域 */
{
	LCUI_Graph *p;
	LCUI_Pos center_pos;
	float width = 0,height = 0;
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	if(!Valid_Graph(pic_box->image)) return -1;
	
	/* 有效范围为2%~2000% */
	if(scale < 0.02) scale = 0.02;
	if(scale > 20) scale = 20;
	pic_box->size_mode = SIZE_MODE_ZOOM; /* 改为缩放模式 */
	pic_box->scale = scale;
	
	/* 缩放后的图像尺寸 */
	width = scale * pic_box->image->width;
	height = scale * pic_box->image->height;
	
	if(scale == 1.00) { /* 原100%比例就不需要缓存了，直接用现成的 */
		Free_Graph(&pic_box->buff_graph);
		p = pic_box->image;
	} else {/* 缩放图像 */
		p = &pic_box->buff_graph;
		Zoom_Graph(pic_box->image, &pic_box->buff_graph,
				DEFAULT, Size(width, height));
	}
	/* 如果缩放后的图像的宽度还是不大于部件的宽度 */
	if(width <= widget->size.w) {
		pic_box->read_box.width = width;
		pic_box->read_box.x = 0;
		pic_box->read_box.center_x = 0.5; 
	} else {/* 否则，调整读取区域 */
		pic_box->read_box.width = widget->size.w;
		center_pos.x = pic_box->read_box.center_x * width;
		pic_box->read_box.x = center_pos.x - pic_box->read_box.width/2.0;
		/* 如果小于0，就需要调整中心点位置 */
		if(pic_box->read_box.x < 0) {
			center_pos.x = pic_box->read_box.width/2.0 + pic_box->read_box.x;
			pic_box->read_box.center_x = center_pos.x / width;
			pic_box->read_box.x = 0;
		}
		if(pic_box->read_box.x + pic_box->read_box.width
		 > p->width) {
			pic_box->read_box.x = pic_box->buff_graph.width - pic_box->read_box.width; 
			center_pos.x = p->width/2.0 + pic_box->read_box.x;
			pic_box->read_box.center_x = center_pos.x / width; 
		}
	} 
	/* 如果缩放后的图像的宽度还是不大于部件的宽度 */
	if(height <= widget->size.h) {
		pic_box->read_box.height = height;
		pic_box->read_box.y = 0;
		pic_box->read_box.center_y = 0.5; 
	} else {/* 否则，调整读取区域 */
		pic_box->read_box.height = widget->size.h;
		center_pos.y = pic_box->read_box.center_y * height;
		pic_box->read_box.y = center_pos.y - pic_box->read_box.height/2.0;
		/* 如果小于0，就需要调整中心点位置 */
		if(pic_box->read_box.y < 0) {
			center_pos.y = p->height/2.0 + pic_box->read_box.y;
			pic_box->read_box.center_y = center_pos.y / height;
			pic_box->read_box.y = 0;
		}
		if(pic_box->read_box.y + pic_box->read_box.height
		 > p->height) {
			pic_box->read_box.y = p->height - pic_box->read_box.height; 
			center_pos.y = pic_box->read_box.height/2.0 + pic_box->read_box.y;
			pic_box->read_box.center_y = center_pos.y / height; 
		}
	}
	
	Draw_Widget(widget);
	Refresh_Widget(widget);
	return 0;
}

static void Exec_Resize_PictureBox(LCUI_Widget *widget)
/* 功能：改变PictureBox部件的尺寸 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Get_Widget_Private_Data(widget);
	if(widget->size.w * widget->size.h > 0) {
		switch(pic_box->size_mode) {
		case SIZE_MODE_ZOOM: 
			/* 重新改变图像浏览区域的尺寸，这个尺寸指的是从源图像中截取出的图像的尺寸 */
			Resize_PictureBox_View_Area(
				widget, 
				widget->size.w / pic_box->scale, 
				widget->size.h / pic_box->scale );
			/* 以现在的缩放比例重新生成缩放后的图像 */
			Zoom_PictureBox_View_Area(widget, pic_box->scale);
			break;
		case SIZE_MODE_NORMAL:/* 正常模式 */
			break;
		case SIZE_MODE_STRETCH:/* 拉伸模式 */
			break;
		case SIZE_MODE_TILE:/* 平铺模式 */
			break;
		case SIZE_MODE_CENTER: /* 居中模式 */ 
			Resize_PictureBox_View_Area(widget, widget->size.w, widget->size.h);
			break;
			default : break;
		}
	}
	Refresh_Widget(widget); 
}

void Register_PictureBox()
/* 功能：注册部件类型-按钮至部件库 */
{
	/* 添加部件类型 */
	WidgetType_Add("picture_box");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("picture_box",	PictureBox_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("picture_box",	Exec_Update_PictureBox,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("picture_box",	Exec_Resize_PictureBox,	FUNC_TYPE_RESIZE); 
	WidgetFunc_Add("picture_box",	Destroy_PictureBox,	FUNC_TYPE_DESTROY); 
}

