/* ***************************************************************************
 * picturebox.c -- LCUI's PictureBox widget
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
 * picturebox.c -- LCUI 的图像框部件
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

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_MISC_H
#include LC_GRAPH_H
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

/* 获取图片盒子的图片显示的区域 */
LCUI_API LCUI_Rect
PictureBox_GetViewArea( LCUI_Widget *widget )
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	return pic_box->read_box;
}

/* 获取获取图片盒子的图片显示的区域的坐标 */
LCUI_API LCUI_Pos
PictureBox_GetViewAreaPos( LCUI_Widget *widget )
{
	LCUI_Rect rect;
	rect = PictureBox_GetViewArea(widget);
	return Pos(rect.x, rect.y);
}


static int Update_BuffGraph(LCUI_Widget *widget)
/* 功能：更新缩放后的图像的缓存 */
{
	float width = 0,height = 0;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	if(!Graph_IsValid(pic_box->image)) {
		return -1;
	}
	width = pic_box->scale * pic_box->image->w;
	height = pic_box->scale * pic_box->image->h;
	
	if(pic_box->scale == 1.00) {
		Graph_Free(&pic_box->buff_graph); 
	} else {
		Graph_Zoom(pic_box->image, &pic_box->buff_graph,
				TRUE, Size(width, height)); 
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
	if(!Graph_IsValid(pic_box->image)) {
		return -1;
	}
	/* 缩放后的图像尺寸 */
	width = pic_box->scale * pic_box->image->w;
	height = pic_box->scale * pic_box->image->h;
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
		pic_box->read_box.width = p->w;
		pic_box->read_box.height = p->h;
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
		 > p->w) {
			pic_box->read_box.x = pic_box->buff_graph.w - pic_box->read_box.width; 
			center_pos.x = p->w/2.0 + pic_box->read_box.x;
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
			center_pos.y = p->h/2.0 + pic_box->read_box.y;
			pic_box->read_box.center_y = center_pos.y / height;
			pic_box->read_box.y = 0;
		}
		if(pic_box->read_box.y + pic_box->read_box.height
		 > p->h) {
			pic_box->read_box.y = p->h - pic_box->read_box.height; 
			center_pos.y = pic_box->read_box.height/2.0 + pic_box->read_box.y;
			pic_box->read_box.center_y = center_pos.y / height; 
		}
	}
	return 0;
}

/* 在PictureBox部件更新时进行附加处理 */
static void 
PictureBox_ExecUpdate(LCUI_Widget *widget)
{
	LCUI_Pos pos;
	LCUI_PictureBox *pic_box;
	LCUI_Graph graph, *widget_graph, *p;
	
	pos = Pos(0,0);
	Graph_Init(&graph);
	pic_box  = (LCUI_PictureBox*)Widget_GetPrivData(widget);
	widget_graph = Widget_GetSelfGraph( widget );
	//print_widget_info(widget);
	//Graph_PrintInfo(widget_graph);
	//Graph_PrintInfo(pic_box->image);
	//printf("PictureBox_ExecUpdate(): 1\n");
	if(! Graph_IsValid(pic_box->image)) {
		return;
	}
	
	//printf("PictureBox_ExecUpdate(): widget size: w: %d, h: %d\n", widget->size.w, widget->size.h);
	//printf("PictureBox_ExecUpdate(): read box: %d,%d,%d,%d\n",
		//pic_box->read_box.x, pic_box->read_box.y, 
		//pic_box->read_box.width, pic_box->read_box.height);
	switch(pic_box->size_mode) {
	case SIZE_MODE_BLOCK_ZOOM:
	case SIZE_MODE_ZOOM:
	/* 裁剪图像 */ 
		if(pic_box->scale == 1.00) p = pic_box->image; 
		else p = &pic_box->buff_graph; 
		if(! Graph_IsValid(p)) {
			//printf("! Graph_IsValid(p)\n");
			return;
		}
		
		pos.x = (widget->size.w - pic_box->read_box.width)/2.0;
		pos.y = (widget->size.h - pic_box->read_box.height)/2.0;
		/* 引用图像中指定区域的图形 */
		Graph_Quote(&graph, p, pic_box->read_box);
		break;
		 
	case SIZE_MODE_NORMAL:/* 正常模式 */
		Graph_Quote(&graph, pic_box->image, pic_box->read_box); 
		break;
		
	case SIZE_MODE_STRETCH:/* 拉伸模式 */ 
		/* 开始缩放图片 */
		Graph_Zoom( pic_box->image, &graph, FALSE, widget->size ); 
		break;
		
	case SIZE_MODE_TILE:/* 平铺模式 */ 
		Graph_Tile( pic_box->image, &graph, TRUE );
		break;
		
	case SIZE_MODE_CENTER:
		/* 判断图像的尺寸是否小于图片盒子的尺寸，并计算坐标位置 */
		if(pic_box->image->w < widget->size.w) {
			pic_box->read_box.x = 0;
			pic_box->read_box.width = pic_box->image->w;
			pos.x = (widget->size.w - pic_box->image->w)/2 + 0.5;
		}
		if(pic_box->image->h < widget->size.h) {
			pos.y = (widget->size.h - pic_box->image->h)/2 + 0.5;
			pic_box->read_box.y = 0;
			pic_box->read_box.height = pic_box->image->h;
		}
		if(pic_box->read_box.y + pic_box->read_box.height >= pic_box->image->h) 
		/* 如果读取区域的尺寸大于图片尺寸 */
			pic_box->read_box.y = pic_box->image->h - pic_box->read_box.height;
		if(pic_box->read_box.x + pic_box->read_box.width >= pic_box->image->w) 
			pic_box->read_box.x = pic_box->image->w - pic_box->read_box.width;
			
		Graph_Quote(&graph, pic_box->image, pic_box->read_box); 
		break;
	default : break;
	}
	// 用于调试
	//printf("PictureBox_ExecUpdate(): read box: %d,%d,%d,%d; %d/%d, %d/%d\n", 
	//pic_box->read_box.x, pic_box->read_box.y, 
	//pic_box->read_box.width, pic_box->read_box.height, 
	//pic_box->read_box.x + pic_box->read_box.width, pic_box->buff_graph.w, 
	//pic_box->read_box.y + pic_box->read_box.height, pic_box->buff_graph.h);
	if(!Graph_IsValid(&widget->background.image)) {
		Graph_Replace( widget_graph, &graph, pos );
	} else {
		Graph_Mix( widget_graph, &graph, pos );
	}
	Graph_Free(&graph);
	//printf("scale: %.4f\n", pic_box->scale);
	//printf("PictureBox_ExecUpdate(): end\n");
	Widget_Refresh(widget); 
}

/* 获取缩放比例 */
LCUI_API float
PictureBox_GetScale( LCUI_Widget *widget )
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
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
	total = Queue_GetTotal(&picbox_graph_mem); 
	for(i=0; i<total; ++i) {
		temp = (graph_data *)Queue_Get(&picbox_graph_mem, i);
		if(temp->widget == widget) {
			return i;
		}
	} 
	return -1;
}

/* 设定PictureBox部件显示的图像 */
LCUI_API void
PictureBox_SetImage( LCUI_Widget *widget, LCUI_Graph *image )
{ 
	int i;
	float scale_x,scale_y;
	LCUI_Graph *graph; 
	LCUI_PictureBox *pic_box;
	graph_data *data;

	if( widget == NULL ) {
		return;
	}
	graph = image;
	pic_box = Widget_GetPrivData(widget);
	for(i = 0;i < 2; ++i) {
		/* 如果image有效 */ 
		if(Graph_IsValid(graph)) {
			/* 图片更换了，就释放缓存图形 */
			Graph_Free(&pic_box->buff_graph);
			pic_box->image = graph;
			/* 读取的范区域为整个图片区域 */
			pic_box->read_box.x = 0;
			pic_box->read_box.y = 0;
			pic_box->read_box.width = graph->w;
			pic_box->read_box.height = graph->h;
			pic_box->read_box.center_x = 0.5;
			pic_box->read_box.center_y = 0.5;
			pic_box->scale = 1.0; 
			
			//printf("PictureBox_SetImage(): img, w: %d, h: %d\n", graph->w, graph->h);
			//printf("PictureBox_SetImage(): pb: w: %d, h: %d\n", widget->size.w, widget->size.h);
			//printf("PictureBox_SetImage(): size mode: %d\n", pic_box->size_mode);
			switch(pic_box->size_mode) {
			    case SIZE_MODE_BLOCK_ZOOM:
			    case SIZE_MODE_ZOOM: 
			//printf("PictureBox_SetImage(): widget: w: %d, h: %d\n", widget->size.w, widget->size.h);
				if(widget->size.w <= 0 || widget->size.h <= 0) {
					//printf("PictureBox_SetImage(): break\n");
					break;
				}
				scale_x = (float)widget->size.w / pic_box->image->w;
				scale_y = (float)widget->size.h / pic_box->image->h;
				if(scale_x < scale_y) pic_box->scale = scale_x;
				else pic_box->scale = scale_y;
				//printf("PictureBox_SetImage(): scale: %.4f, x: %.4f, y: %.4f\n", 
				//pic_box->scale, scale_x, scale_y);
				PictureBox_ZoomViewArea(widget, pic_box->scale); 
				//printf("PictureBox_SetImage(): read box: %d,%d,%d,%d\n",
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
				if(pic_box->image->w >= widget->size.w) {
					pic_box->read_box.x = (pic_box->image->w - widget->size.w)/2.0;
					pic_box->read_box.width = widget->size.w;
				}
				if(pic_box->image->h >= widget->size.h) {
					pic_box->read_box.y = (pic_box->image->h - widget->size.h)/2.0;
					pic_box->read_box.height = widget->size.h;
				}
				break;
				default : break;
			}
			break;
		} else if(pic_box->image_state == IMAGE_STATE_LOADING) {
			/* 使用对应的图片 */ 
			if(Graph_IsValid(&pic_box->initial_image)) { 
				graph = &pic_box->initial_image; 
			} else {
				return;
			}
		}
		else { /* 使用对应的图片 */ 
			if(Graph_IsValid(&pic_box->error_image)) {
				graph = &pic_box->error_image; 
				pic_box->image_state = IMAGE_STATE_FAIL;
			} else {
				return;
			} 
		}
	} 
	/* 如果记录中有该部件，那判断该部件使用的图像是否为同一个，不一样就释放之前的 */
	i = find_widget_data(widget);
	if(i >= 0) {
		data = (graph_data*)Queue_Get(&picbox_graph_mem, i); 
		if(data->image != image) {
			Queue_Delete(&picbox_graph_mem, i);
		}
	}
	Widget_Draw(widget);
}


/* 设定图片文件中的图像为PictureBox部件显示的图像 */
LCUI_API int
PictureBox_SetImageFile( LCUI_Widget *widget, char *image_file )
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
	PictureBox_SetImage(widget, NULL);
	ret = Graph_LoadImage( image_file, graph );/* 载入图片文件 */
	if( ret == 0 ) {
		/* 载入成功 */
		pic_box->image_state = IMAGE_STATE_SUCCESS; 
		PictureBox_SetImage(widget, graph);
	} else {
		/* 载入失败 */
		pic_box->image_state = IMAGE_STATE_FAIL;
		PictureBox_SetImage(widget, NULL);
	}
	return ret;
}

/* 设定当加载图像失败时显示的图像 */
LCUI_API int
PictureBox_SetErrorImage( LCUI_Widget *widget, LCUI_Graph *pic )
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(Graph_IsValid(pic)) {
		Graph_Copy(&pic_box->error_image, pic);
		return 0;
	}
	return -1;
}

/* 设定正在加载另一图像时显示的图像 */
LCUI_API int
PictureBox_SetInitImage( LCUI_Widget *widget, LCUI_Graph *pic )
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(Graph_IsValid(pic)) {
		Graph_Copy(&pic_box->initial_image, pic);
		return 0;
	}
	return -1;
}

/* 设定图像显示模式 */
LCUI_API void
PictureBox_SetSizeMode( LCUI_Widget *widget, int mode )
{
	LCUI_Size my_size;
	float scale_x,scale_y;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	if(pic_box->size_mode == mode) {
		return;
	}
	
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
			scale_x = (float)my_size.w / pic_box->image->w;
			scale_y = (float)my_size.h / pic_box->image->h;
			if(scale_x < scale_y) {
				pic_box->scale = scale_x;
			} else {
				pic_box->scale = scale_y;
			}
		}
		PictureBox_ZoomViewArea(widget, pic_box->scale); 
		break;
	default: pic_box->scale = 1.0; break;
	}
	Widget_Draw(widget);
}


/* 设定PictureBox部件的图片显示区域的大小 */
LCUI_API void
PictureBox_ResizeViewArea( LCUI_Widget *widget, int width, int height )
{
	LCUI_Pos start, center_pos;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(width <= 0 || height <= 0) {
		return;
	}
	if(!Graph_IsValid(pic_box->image)) {
		return;
	}
	/* 将中心点位置转换成坐标 */
	center_pos.x = pic_box->read_box.center_x * pic_box->scale * pic_box->image->w;
	center_pos.y = pic_box->read_box.center_y * pic_box->scale * pic_box->image->h;
	/* 处理区域数据，使之为有效区域 */
	start.x = center_pos.x - width/2.0;
	start.y = center_pos.y - height/2.0;
	if(start.x < 0) start.x = 0;
	if(start.y < 0) start.y = 0;
	if(start.x + width > pic_box->image->w)
	start.x = pic_box->image->w - width;
	if(start.y + height > pic_box->image->h)
	start.y = pic_box->image->h - height;
	if(start.x < 0) {
		start.x = 0;
		width = pic_box->image->w;
	}
	if(start.y < 0) {
		start.y = 0;
		height = pic_box->image->h;
	}
	pic_box->read_box.x = start.x;
	pic_box->read_box.y = start.y;
	pic_box->read_box.width = width;
	pic_box->read_box.height = height;
	/* 更新图片盒子内的图像 */
	Widget_Draw(widget);
	Widget_Refresh(widget); 
}

/* 获取PictureBox部件内的图像 */
LCUI_API LCUI_Graph*
PictureBox_GetImage( LCUI_Widget *widget )
{
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	return pic_box->image;
}

/* 移动PictureBox部件内的图片的显示区域的位置 */
LCUI_API int
PictureBox_MoveViewArea( LCUI_Widget *widget, LCUI_Pos des_pos )
{
	LCUI_Size size;
	LCUI_Graph *p;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	
	if(!Graph_IsValid(pic_box->image)) {
		return -1;
	}
	if(pic_box->scale == 1.00 || !Graph_IsValid(&pic_box->buff_graph)) {
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
	if(des_pos.x + size.w > p->w) {
		des_pos.x = p->w - size.w;
	}
	if(des_pos.y + size.h > p->h) {
		des_pos.y = p->h - size.h;
	}
	if(des_pos.x == pic_box->read_box.x 
	&& des_pos.y == pic_box->read_box.y) {
		return 0; 
	}
	/* 更新图片盒子内的图像 */
	pic_box->read_box.x = des_pos.x;
	pic_box->read_box.y = des_pos.y;
	/* 重新计算中心点的位置 */ 
	pic_box->read_box.center_x = (des_pos.x + size.w/2.0)/p->w;
	pic_box->read_box.center_y = (des_pos.y + size.h/2.0)/p->h;
	
	Widget_Draw(widget);
	//用于调试
	//printf("read box: %d,%d,%d,%d; %d/%d, %d/%d\n", 
	//pic_box->read_box.x, pic_box->read_box.y, 
	//pic_box->read_box.width, pic_box->read_box.height, 
	//pic_box->read_box.x + pic_box->read_box.width,
	//pic_box->read_box.y + pic_box->read_box.height,
	//pic_box->buff_graph.w, pic_box->buff_graph.h);
	return 0;
}

/* 缩放PictureBox部件的图片浏览区域 */
LCUI_API int
PictureBox_ZoomViewArea( LCUI_Widget *widget, double scale )
{
	LCUI_Graph buff, temp;
	LCUI_PictureBox *pic_box;
	
	pic_box = Widget_GetPrivData(widget);
	if(!Graph_IsValid(pic_box->image)) {
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
	Widget_Refresh(widget);
	return 0;
}

/* 在PictureBox部件的尺寸改变时，进行些附加操作 */
static void 
PictureBox_ExecResize(LCUI_Widget *widget)
{
	LCUI_PictureBox *pic_box;
	float scale_x, scale_y;
	
	pic_box = Widget_GetPrivData(widget);
	if( widget->size.w <= 0 || widget->size.h <= 0 ) {
		Widget_Refresh(widget); 
		return;
	}
	
	switch(pic_box->size_mode) {
	case SIZE_MODE_BLOCK_ZOOM:
		/* 重新改变图像浏览区域的尺寸，这个尺寸指的是从源图像中截取出的图像的尺寸 */
		PictureBox_ResizeViewArea(
			widget, 
			widget->size.w / pic_box->scale, 
			widget->size.h / pic_box->scale );
		/* 以现在的缩放比例重新生成缩放后的图像 */
		PictureBox_ZoomViewArea(widget, pic_box->scale);
		break;
	case SIZE_MODE_ZOOM:
		if(widget->size.w <= 0 || widget->size.h <= 0) {
			break; 
		}
		scale_x = (float)widget->size.w / pic_box->image->w;
		scale_y = (float)widget->size.h / pic_box->image->h;
		if(scale_x < scale_y) pic_box->scale = scale_x;
		else pic_box->scale = scale_y; 
		//printf("PictureBox_ExecResize(): scale: %.4f, x: %.4f, y: %.4f\n", pic_box->scale, scale_x, scale_y);
		PictureBox_ZoomViewArea(widget, pic_box->scale); 
		//printf("PictureBox_ExecResize(): read box: %d,%d,%d,%d\n",
		//pic_box->read_box.x, pic_box->read_box.y, 
		//pic_box->read_box.width, pic_box->read_box.height);
		break;
	case SIZE_MODE_NORMAL:/* 正常模式 */
	case SIZE_MODE_STRETCH:/* 拉伸模式 */
	case SIZE_MODE_TILE:/* 平铺模式 */
		break;
	case SIZE_MODE_CENTER: /* 居中模式 */ 
		PictureBox_ResizeViewArea(widget, widget->size.w, widget->size.h);
		break;
		default : break;
	}
	
	Widget_Refresh(widget); 
}

LCUI_API void
Register_PictureBox()
/* 注册图片部件类型 */
{
	/* 添加部件类型 */
	WidgetType_Add("picture_box");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("picture_box",	PictureBox_Init,	FUNC_TYPE_INIT);
	//WidgetFunc_Add("picture_box",	PictureBox_ExecUpdate,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("picture_box",	PictureBox_ExecUpdate,	FUNC_TYPE_DRAW); 
	WidgetFunc_Add("picture_box",	PictureBox_ExecResize,	FUNC_TYPE_RESIZE); 
	WidgetFunc_Add("picture_box",	Destroy_PictureBox,	FUNC_TYPE_DESTROY); 
}

