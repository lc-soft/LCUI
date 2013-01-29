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
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_WIDGET_H
#include LC_PICBOX_H

static void 
Destroy_PictureBox(LCUI_Widget* widget)
/* 功能：释放图片盒子占用的内存资源 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Widget_GetPrivData(widget);
	Graph_Free(&pic_box->buff_graph); 
	Graph_Free(&pic_box->error_image);
	Graph_Free(&pic_box->initial_image); 
}

static void 
PictureBox_Init(LCUI_Widget *widget)
/* 功能：初始化图片盒子 */
{
	LCUI_PictureBox *pic_box;
	
	pic_box = WidgetPrivData_New(widget, sizeof(LCUI_PictureBox));;
	
	Rect_Init(&pic_box->read_box);
	pic_box->image		= NULL; 
	pic_box->scale		= 1.0;
	pic_box->size_mode	= SIZE_MODE_CENTER; 
	Graph_Init(&pic_box->buff_graph); 
	Graph_Init(&pic_box->error_image);
	Graph_Init(&pic_box->initial_image); 
	//Set_Widget_BG_Mode(widget, BG_MODE_FILL_BACKCOLOR);
}

LCUI_Rect Get_PictureBox_View_Area(LCUI_Widget *widget)
/* 功能：获取图片盒子的图片显示的区域 */
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	return pic_box->read_box;
}

LCUI_Pos Get_PictureBox_View_Area_Pos(LCUI_Widget *widget)
/* 功能：获取获取图片盒子的图片显示的区域的坐标 */
{
	LCUI_Rect rect;
	rect = Get_PictureBox_View_Area(widget);
	return Pos(rect.x, rect.y);
}


static int Update_BuffGraph(LCUI_Widget *widget)
/* 功能：更新缩放后的图像的缓存 */
{
	float width = 0,height = 0;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	if(!Graph_Valid(pic_box->image)) {
		return -1;
	}
	width = pic_box->scale * pic_box->image->width;
	height = pic_box->scale * pic_box->image->height;
	
	if(pic_box->scale == 1.00) {
		Graph_Free(&pic_box->buff_graph); 
	} else {
		Graph_Zoom(pic_box->image, &pic_box->buff_graph,
				DEFAULT, Size(width, height)); 
	}
	return 0;
}

static int Update_ReadBox(LCUI_Widget *widget)
/* 功能：更新图像的读取区域 */
{
	LCUI_Graph *p;
	LCUI_Pos center_pos;
	float width = 0,height = 0;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	if(!Graph_Valid(pic_box->image)) {
		return -1;
	}
	/* 缩放后的图像尺寸 */
	width = pic_box->scale * pic_box->image->width;
	height = pic_box->scale * pic_box->image->height;
	//printf("Update_ReadBox(): scale: %.4f, size: %.2f, %.2f\n", pic_box->scale, width, height);
	
	if(pic_box->scale == 1.00) {
		p = pic_box->image;
	} else {
		p = &pic_box->buff_graph;  
	}
	
	/* 如果部件的宽或者高不大于0 */
	if( widget->size.w <= 0 || widget->size.h <= 0 ) { 
		pic_box->read_box.x = 0;
		pic_box->read_box.y = 0;
		pic_box->read_box.width = p->width;
		pic_box->read_box.height = p->height;
		pic_box->read_box.center_x = 0.5;
		pic_box->read_box.center_y = 0.5;
		return 0;
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
	return 0;
}

static void 
Exec_Update_PictureBox(LCUI_Widget *widget)
/* 功能：更新PictureBox部件 */
{
	LCUI_Pos pos;
	LCUI_PictureBox *pic_box;
	LCUI_Graph graph, *widget_graph, *p;
	
	pos = Pos(0,0);
	Graph_Init(&graph);
	pic_box  = (LCUI_PictureBox*)Widget_GetPrivData(widget);
	widget_graph = Widget_GetSelfGraph( widget );
	//print_widget_info(widget);
	//Print_Graph_Info(widget_graph);
	//Print_Graph_Info(pic_box->image);
	//printf("Exec_Update_PictureBox(): 1\n");
	if(! Graph_Valid(pic_box->image)) {
		return;
	}
	
	//printf("Exec_Update_PictureBox(): widget size: w: %d, h: %d\n", widget->size.w, widget->size.h);
	//printf("Exec_Update_PictureBox(): read box: %d,%d,%d,%d\n",
		//pic_box->read_box.x, pic_box->read_box.y, 
		//pic_box->read_box.width, pic_box->read_box.height);
	switch(pic_box->size_mode) {
	case SIZE_MODE_BLOCK_ZOOM:
	case SIZE_MODE_ZOOM:
	/* 裁剪图像 */ 
		if(pic_box->scale == 1.00) p = pic_box->image; 
		else p = &pic_box->buff_graph; 
		if(! Graph_Valid(p)) {
			//printf("! Graph_Valid(p)\n");
			return;
		}
		
		pos.x = (widget->size.w - pic_box->read_box.width)/2.0;
		pos.y = (widget->size.h - pic_box->read_box.height)/2.0;
		/* 引用图像中指定区域的图形 */
		Quote_Graph(&graph, p, pic_box->read_box);
		break;
		 
	case SIZE_MODE_NORMAL:/* 正常模式 */
		Quote_Graph(&graph, pic_box->image, pic_box->read_box); 
		break;
		
	case SIZE_MODE_STRETCH:/* 拉伸模式 */ 
		/* 开始缩放图片 */
		Graph_Zoom( pic_box->image, &graph, CUSTOM, widget->size ); 
		break;
		
	case SIZE_MODE_TILE:/* 平铺模式 */ 
		Graph_Tile( pic_box->image, &graph, widget->size.w, widget->size.h); 
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
	// 用于调试
	//printf("Exec_Update_PictureBox(): read box: %d,%d,%d,%d; %d/%d, %d/%d\n", 
	//pic_box->read_box.x, pic_box->read_box.y, 
	//pic_box->read_box.width, pic_box->read_box.height, 
	//pic_box->read_box.x + pic_box->read_box.width, pic_box->buff_graph.width, 
	//pic_box->read_box.y + pic_box->read_box.height, pic_box->buff_graph.height);
	if(!Graph_Valid(&widget->background.image)) {
		Graph_Replace( widget_graph, &graph, pos );
	} else {
		Graph_Mix( widget_graph, &graph, pos );
	}
	Graph_Free(&graph);
	//printf("scale: %.4f\n", pic_box->scale);
	//printf("Exec_Update_PictureBox(): end\n");
	Refresh_Widget(widget); 
}

float Get_PictureBox_Zoom_Scale(LCUI_Widget *widget)
/* 功能：获取图片盒子的缩放比例 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
			Widget_GetPrivData(widget);
	return pic_box->scale;
}


static int graph_mem_init = FALSE;
static LCUI_Queue picbox_graph_mem;

typedef struct _graph_data
{
	LCUI_Widget *widget;
	LCUI_Graph *image;
}
graph_data;

static void 
destroy_graph_data(void *data)
{
	graph_data *p = (graph_data *)data;
	Graph_Free(p->image);
	free(p->image);
}

static int 
find_widget_data(LCUI_Widget *widget)
{
	graph_data *temp;
	int total, i;
	/* 检查该部件是否用过本函数分配的内存，用过的话，清理它 */
	total = Queue_Get_Total(&picbox_graph_mem); 
	for(i=0; i<total; ++i) {
		temp = (graph_data *)Queue_Get(&picbox_graph_mem, i);
		if(temp->widget == widget) {
			return i;
		}
	} 
	return -1;
}

void Set_PictureBox_Image_From_Graph(LCUI_Widget *widget, LCUI_Graph *image)
/* 功能：添加一个图片数据至图片盒子 */
{ 
	int i;
	float scale_x,scale_y;
	LCUI_Graph *graph = image; 
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	for(i = 0;i < 2; ++i) {
		/* 如果image有效 */ 
		if(Graph_Valid(graph)) {
			/* 图片更换了，就释放缓存图形 */
			Graph_Free(&pic_box->buff_graph);
			pic_box->image = graph;
			/* 读取的范区域为整个图片区域 */
			pic_box->read_box.x = 0;
			pic_box->read_box.y = 0;
			pic_box->read_box.width = graph->width;
			pic_box->read_box.height = graph->height;
			pic_box->read_box.center_x = 0.5;
			pic_box->read_box.center_y = 0.5;
			pic_box->scale = 1.0; 
			
			//printf("Set_PictureBox_Image_From_Graph(): img, w: %d, h: %d\n", graph->width, graph->height);
			//printf("Set_PictureBox_Image_From_Graph(): pb: w: %d, h: %d\n", widget->size.w, widget->size.h);
			//printf("Set_PictureBox_Image_From_Graph(): size mode: %d\n", pic_box->size_mode);
			switch(pic_box->size_mode) {
			    case SIZE_MODE_BLOCK_ZOOM:
			    case SIZE_MODE_ZOOM: 
			//printf("Set_PictureBox_Image_From_Graph(): widget: w: %d, h: %d\n", widget->size.w, widget->size.h);
				if(widget->size.w <= 0 || widget->size.h <= 0) {
					//printf("Set_PictureBox_Image_From_Graph(): break\n");
					break;
				}
				scale_x = (float)widget->size.w / pic_box->image->width;
				scale_y = (float)widget->size.h / pic_box->image->height;
				if(scale_x < scale_y) pic_box->scale = scale_x;
				else pic_box->scale = scale_y;
				//printf("Set_PictureBox_Image_From_Graph(): scale: %.4f, x: %.4f, y: %.4f\n", 
				//pic_box->scale, scale_x, scale_y);
				Zoom_PictureBox_View_Area(widget, pic_box->scale); 
				//printf("Set_PictureBox_Image_From_Graph(): read box: %d,%d,%d,%d\n",
				//pic_box->read_box.x, pic_box->read_box.y, 
				//pic_box->read_box.width, pic_box->read_box.height);
				break;
			/* 正常模式 */
			    case SIZE_MODE_NORMAL: break;
			/* 拉伸模式 */
			    case SIZE_MODE_STRETCH: break;
			/* 平铺模式 */
			    case SIZE_MODE_TILE: break;
			    case SIZE_MODE_CENTER: 
				/* 判断图像的尺寸是否超出图片盒子的尺寸 */
				if(pic_box->image->width >= widget->size.w) {
					pic_box->read_box.x = (pic_box->image->width - widget->size.w)/2.0;
					pic_box->read_box.width = widget->size.w;
				}
				if(pic_box->image->height >= widget->size.h) {
					pic_box->read_box.y = (pic_box->image->height - widget->size.h)/2.0;
					pic_box->read_box.height = widget->size.h;
				}
				break;
				default : break;
			}
			break;
		} else if(pic_box->image_state == IMAGE_STATE_LOADING) {
			/* 使用对应的图片 */ 
			if(Graph_Valid(&pic_box->initial_image)) { 
				graph = &pic_box->initial_image; 
			} else {
				return;
			}
		}
		else { /* 使用对应的图片 */ 
			if(Graph_Valid(&pic_box->error_image)) {
				graph = &pic_box->error_image; 
				pic_box->image_state = IMAGE_STATE_FAIL;
			} else {
				return;
			} 
		}
	} 
	/* 如果记录中有该部件，那判断该部件使用的图像是否为同一个，不一样就释放之前的 */
	graph_data *data;
	i = find_widget_data(widget);
	if(i >= 0) {
		data = (graph_data*)Queue_Get(&picbox_graph_mem, i); 
		if(data->image != image) {
			Queue_Delete(&picbox_graph_mem, i);
		}
	}
	Widget_Draw(widget);
}


int Set_PictureBox_Image_From_File(LCUI_Widget *widget, char *image_file)
/* 功能：添加一个图片文件，并载入至图片盒子 */
{
	int ret;
	graph_data data;
	LCUI_PictureBox *pic_box;
	LCUI_Graph *graph;
	
	pic_box = Widget_GetPrivData(widget);
	if( !graph_mem_init ) {
		Queue_Init( &picbox_graph_mem, 
			sizeof(graph_data), destroy_graph_data);
		graph_mem_init = TRUE;
	}
	/* 如果在记录中没找到该部件，那么就分配内存，否则，直接使用那块内存 */
	ret = find_widget_data(widget);
	if(ret == -1) {
		data.image = (LCUI_Graph*) calloc(1, sizeof(LCUI_Graph));
		data.widget = widget;
		Queue_Add(&picbox_graph_mem, &data); 
	} else {
		data = *(graph_data*)Queue_Get(&picbox_graph_mem, ret); 
	}
	graph = data.image;
	
	pic_box->image_state = IMAGE_STATE_LOADING; /* 图片状态为正在载入 */
	Set_PictureBox_Image_From_Graph(widget, NULL);
	ret = Load_Image( image_file, graph );/* 载入图片文件 */
	if( ret == 0 ) {
		/* 载入成功 */
		pic_box->image_state = IMAGE_STATE_SUCCESS; 
		Set_PictureBox_Image_From_Graph(widget, graph);
	} else {
		/* 载入失败 */
		pic_box->image_state = IMAGE_STATE_FAIL;
		Set_PictureBox_Image_From_Graph(widget, NULL);
	}
	return ret;
}

int Set_PictureBox_ErrorImage(LCUI_Widget *widget, LCUI_Graph *pic)
/* 功能：设定当加载图像失败时显示的图像 */
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(Graph_Valid(pic)) {
		Graph_Copy(&pic_box->error_image, pic);
		return 0;
	}
	return -1;
}

int Set_PictureBox_InitImage(LCUI_Widget *widget, LCUI_Graph *pic)
/* 功能：设定正在加载另一图像时显示的图像 */
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(Graph_Valid(pic)) {
		Graph_Copy(&pic_box->initial_image, pic);
		return 0;
	}
	return -1;
}

void Set_PictureBox_Size_Mode(LCUI_Widget *widget, int mode)
/* 功能：设定图片盒子的图像显示模式 */
{
	LCUI_PictureBox *pic_box;
	LCUI_Size my_size;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(pic_box->size_mode == mode) {
		return;
	}
	
	float scale_x,scale_y;
	pic_box->size_mode = mode;
	if( !pic_box->image ) {
		return; 
	}
	my_size = _Widget_GetSize( widget );
	switch(mode) {
	case SIZE_MODE_BLOCK_ZOOM:
	case SIZE_MODE_ZOOM:
		if( my_size.w <= 0 || my_size.h <= 0) {
			pic_box->scale = 1.0; 
		} else {
			scale_x = (float)my_size.w / pic_box->image->width;
			scale_y = (float)my_size.h / pic_box->image->height;
			if(scale_x < scale_y) {
				pic_box->scale = scale_x;
			} else {
				pic_box->scale = scale_y;
			}
		}
		Zoom_PictureBox_View_Area(widget, pic_box->scale); 
		break;
	default: pic_box->scale = 1.0; break;
	}
	Widget_Draw(widget);
}


void Resize_PictureBox_View_Area(LCUI_Widget *widget, int width, int height)
/* 功能：设定PictureBox部件的图片显示区域的大小 */
{
	LCUI_Pos start, center_pos;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(width <= 0 || height <= 0) {
		return;
	}
	if(!Graph_Valid(pic_box->image)) {
		return;
	}
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
	Widget_Draw(widget);
	Refresh_Widget(widget); 
}

LCUI_Graph *Get_PictureBox_Graph(LCUI_Widget *widget)
/* 功能：获取PictureBox部件内的图像指针 */
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	return pic_box->image;
}

int Move_PictureBox_View_Area(LCUI_Widget *widget, LCUI_Pos des_pos)
/* 功能：移动图片盒子内的图片的显示区域的位置 */
{
	LCUI_Size size;
	LCUI_Graph *p;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(!Graph_Valid(pic_box->image)) {
		return -1;
	}
	if(pic_box->scale == 1.00 || !Graph_Valid(&pic_box->buff_graph)) {
		p = pic_box->image;
	} else {
		p = &pic_box->buff_graph;
	}
	size.w = pic_box->read_box.width;
	size.h = pic_box->read_box.height;
	/* 处理区域数据，使之为有效区域 */
	if(des_pos.x < 0) {
		des_pos.x = 0;
	}
	if(des_pos.y < 0) {
		des_pos.y = 0;
	}
	if(des_pos.x + size.w > p->width) {
		des_pos.x = p->width - size.w;
	}
	if(des_pos.y + size.h > p->height) {
		des_pos.y = p->height - size.h;
	}
	if(des_pos.x == pic_box->read_box.x 
	&& des_pos.y == pic_box->read_box.y) {
		return 0; 
	}
	/* 更新图片盒子内的图像 */
	pic_box->read_box.x = des_pos.x;
	pic_box->read_box.y = des_pos.y;
	/* 重新计算中心点的位置 */ 
	pic_box->read_box.center_x = (des_pos.x + size.w/2.0)/p->width;
	pic_box->read_box.center_y = (des_pos.y + size.h/2.0)/p->height;
	
	Widget_Draw(widget);
	//用于调试
	//printf("read box: %d,%d,%d,%d; %d/%d, %d/%d\n", 
	//pic_box->read_box.x, pic_box->read_box.y, 
	//pic_box->read_box.width, pic_box->read_box.height, 
	//pic_box->read_box.x + pic_box->read_box.width,
	//pic_box->read_box.y + pic_box->read_box.height,
	//pic_box->buff_graph.width, pic_box->buff_graph.height);
	return 0;
}

int Zoom_PictureBox_View_Area(LCUI_Widget *widget, float scale)
/* 功能：缩放PictureBox部件的图片浏览区域 */
{
	LCUI_Graph buff, temp;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	if(!Graph_Valid(pic_box->image)) {
		return -1;
	}
	Graph_Init(&buff);
	Graph_Init(&temp);
	/* 有效范围为2%~2000% */
	if(scale < 0.02) {
		scale = 0.02;
	}
	if(scale > 20) {
		scale = 20;
	}
	if(pic_box->size_mode != SIZE_MODE_ZOOM
	 && pic_box->size_mode != SIZE_MODE_BLOCK_ZOOM) {
		pic_box->size_mode = SIZE_MODE_ZOOM; /* 改为缩放模式 */
	}
	pic_box->scale = scale; 
	Update_BuffGraph(widget); 
	Update_ReadBox(widget);
	Widget_Draw(widget);
	Refresh_Widget(widget);
	return 0;
}

static void Exec_Resize_PictureBox(LCUI_Widget *widget)
/* 功能：改变PictureBox部件的尺寸 */
{
	LCUI_PictureBox *pic_box = (LCUI_PictureBox*)
				Widget_GetPrivData(widget);
	float scale_x, scale_y;
	if( widget->size.w <= 0 || widget->size.h <= 0 ) {
		Refresh_Widget(widget); 
		return;
	}
	
	switch(pic_box->size_mode) {
	case SIZE_MODE_BLOCK_ZOOM:
		/* 重新改变图像浏览区域的尺寸，这个尺寸指的是从源图像中截取出的图像的尺寸 */
		Resize_PictureBox_View_Area(
			widget, 
			widget->size.w / pic_box->scale, 
			widget->size.h / pic_box->scale );
		/* 以现在的缩放比例重新生成缩放后的图像 */
		Zoom_PictureBox_View_Area(widget, pic_box->scale);
		break;
	case SIZE_MODE_ZOOM:
		if(widget->size.w <= 0 || widget->size.h <= 0) {
			break; 
		}
		scale_x = (float)widget->size.w / pic_box->image->width;
		scale_y = (float)widget->size.h / pic_box->image->height;
		if(scale_x < scale_y) pic_box->scale = scale_x;
		else pic_box->scale = scale_y; 
		//printf("Exec_Resize_PictureBox(): scale: %.4f, x: %.4f, y: %.4f\n", pic_box->scale, scale_x, scale_y);
		Zoom_PictureBox_View_Area(widget, pic_box->scale); 
		//printf("Exec_Resize_PictureBox(): read box: %d,%d,%d,%d\n",
		//pic_box->read_box.x, pic_box->read_box.y, 
		//pic_box->read_box.width, pic_box->read_box.height);
		break;
	case SIZE_MODE_NORMAL:/* 正常模式 */
	case SIZE_MODE_STRETCH:/* 拉伸模式 */
	case SIZE_MODE_TILE:/* 平铺模式 */
		break;
	case SIZE_MODE_CENTER: /* 居中模式 */ 
		Resize_PictureBox_View_Area(widget, widget->size.w, widget->size.h);
		break;
		default : break;
	}
	
	Refresh_Widget(widget); 
}

void Register_PictureBox()
/* 注册图片部件类型 */
{
	/* 添加部件类型 */
	WidgetType_Add("picture_box");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("picture_box",	PictureBox_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("picture_box",	Exec_Update_PictureBox,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("picture_box",	Exec_Update_PictureBox,	FUNC_TYPE_DRAW); 
	WidgetFunc_Add("picture_box",	Exec_Resize_PictureBox,	FUNC_TYPE_RESIZE); 
	WidgetFunc_Add("picture_box",	Destroy_PictureBox,	FUNC_TYPE_DESTROY); 
}

