/* ***************************************************************************
 * LCUI_Widget.c -- processing GUI widget 
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
 * LCUI_Widget.c -- 处理GUI部件
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
#include <unistd.h>
#include <math.h>
#include LC_LCUI_H 
#include LC_WIDGET_H
#include LC_MISC_H
#include LC_GRAPHICS_H
#include LC_MEM_H
#include LC_FONT_H 
#include LC_ERROR_H 
#include LC_CURSOR_H
#include LC_INPUT_H


/***************************** Widget *********************************/

LCUI_Size Get_Widget_Size(LCUI_Widget *widget)
/* 功能：获取部件的尺寸 */
{
	return widget->size;
}

int Get_Widget_Height(LCUI_Widget *widget)
{
	return Get_Widget_Size(widget).h;
}

int Get_Widget_Width(LCUI_Widget *widget)
{
	return Get_Widget_Size(widget).w;
}

LCUI_Rect Get_Widget_Rect(LCUI_Widget *widget)
/* 功能：获取部件的区域 */
{
	return Rect(widget->pos.x, widget->pos.y, 
					widget->size.w, widget->size.h);
}

LCUI_Pos Get_Widget_Pos(LCUI_Widget *widget)
/* 功能：获取部件的位置 */
{
	return widget->pos;
}

void *Get_Widget_Private_Data(LCUI_Widget *widget)
/* 功能：获取部件的私有数据结构体的指针 */
{
	return widget->private;
}

LCUI_Widget *Get_Widget_Parent(LCUI_Widget *widget)
/* 功能：获取部件的父部件 */
{
	return widget->parent;
}

void print_widget_info(LCUI_Widget *widget)
/* 
 * 功能：打印widget的信息
 * 说明：在调试时需要用到它，用于确定widget是否有问题
 *  */
{
	if(widget != NULL)
	{
		printf("widget: %p, type: %s, visible: %d, pos: (%d,%d), size: (%d, %d)\n",
				widget, widget->type.string, widget->visible,
				widget->pos.x, widget->pos.y,
				widget->size.w, widget->size.h);  
	}
	else printf("NULL widget\n");
}


int Add_Widget_Refresh_Area (LCUI_Widget * widget, LCUI_Rect rect)
/* 功能：在指定部件的内部区域内设定需要刷新的区域 */
{
	if(widget == NULL)
		return Add_Screen_Refresh_Area(rect);
		
	if (rect.width <= 0 || rect.height <= 0)
		return -1;
	
	/* 调整矩形位置及尺寸 */
	rect = Get_Valid_Area(Get_Widget_Size(widget), rect);
	
	/* 保存至队列中 */
	if(0 != RectQueue_Add (&widget->update_area, rect))
		return -1;
	
	if(widget->visible == IS_TRUE) 
		LCUI_Sys.shift_flag = IS_TRUE; 
		
	return 0;
}


void Set_Widget_Border_Style(LCUI_Widget *widget, LCUI_Border_Style style)
/* 功能：设定部件的边框风格 */
{
	widget->border_style = style;
	Draw_Widget(widget);
}

void Response_Status_Change(LCUI_Widget *widget)
/* 
 * 功能：让指定部件响应部件状态的改变
 * 说明：部件创建时，默认是不响应状态改变的，因为每次状态改变后，都要调用函数重绘部件，
 * 这对于一些部件是多余的，没必要重绘，影响效率。如果想让部件能像按钮那样，鼠标移动到它
 * 上面时以及鼠标点击它时，都会改变按钮的图形样式，那就需要用这个函数设置一下。
 *  */
{
	widget->response_flag = 1;
}

void Shift_Widget_Refresh_Area(LCUI_Widget *widget)
/*
 * 功能：转移部件的rect队列成员至父部件中
 **/
{
	//printf("Shift_Widget_Refresh_Area(): enter\n");
	int i, total;
	LCUI_Widget *child;
	LCUI_Rect rect;
	LCUI_Queue *widget_list;
	
	if(NULL == widget) 
		widget_list = &LCUI_Sys.widget_list; 
	else  
		widget_list = &widget->child; 
	
	total = Queue_Get_Total(widget_list);
	
	for(i=total-1; i>=0; --i)
	{/* 从底到顶遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(widget_list, i);
		if(child != NULL && child->visible == IS_TRUE)
		{/* 如果有子部件 */
			/* 递归调用，将记录的区域转移至子部件内 */
			Shift_Widget_Refresh_Area(child);
			while( RectQueue_Get(&rect, 0, &child->update_area) )
			{
				rect.x += child->pos.x;
				rect.y += child->pos.y;
				//printf("[%d]\033[1;34mShift_Widget_Refresh_Area(x:%d, y:%d, w:%d, h:%d)\033[0m\n", child->update_area.total_num, rect.x, rect.y, rect.width, rect.height);
				/* 将子部件的rect队列成员复制到自己这里 */
				
				Add_Widget_Refresh_Area(widget, rect); 
					
				/* 删除子部件rect队列成员 */
				Queue_Delete(&child->update_area, 0);
			}
		}
	} 
	//printf("Shift_Widget_Refresh_Area(): quit\n");
}



void Process_Refresh_Area()
/*
 * 功能：处理已记录的刷新区域
 * 说明：此函数会将各个部件的rect队列中的处理掉，并将
 * 最终的局部刷新区域数据添加至屏幕刷新区域队列中，等
 * 待LCUI来处理。
 **/
{
	//printf("Process_Refresh_Area: enter\n");
	/* 如果flag标志的值为IS_TRUE */ 
	if ( LCUI_Sys.shift_flag == IS_TRUE ) {	
		/* 转移部件内记录的区域至主记录中 */ 
		Shift_Widget_Refresh_Area ( NULL );
		/* 
		 * 复位标志，这是为了避免每次进入本函数时都要进入递归，并对各
		 * 个部件的矩形数据进行移动，因为这是浪费时间，降低了程序的效
		 * 率 
		 * */
		LCUI_Sys.shift_flag = IS_FALSE; 
	}
	/* 分割区域，使之不与任何部件的区域重叠 */ 
	//printf("Process_Refresh_Area: quit\n"); 
}


LCUI_Widget *Get_Parent_Widget(LCUI_Widget *widget, char *widget_type)
/*
 * 功能：获取部件的指定类型的父部件的指针
 * 说明：本函数会在部件关系链中往头部查找父部件指针，并判断这个父部件是否为制定类型
 * 返回值：没有符合要求的父级部件就返回NULL，否则返回部件指针
 **/
{
	LCUI_Widget *temp;
	if(widget == NULL) return NULL; /* 本身是NULL就退出函数 */
	temp = widget;
	while(temp->parent != NULL)
	{
		if(temp->parent != NULL
		  && Strcmp(&temp->parent->type, widget_type) == 0
		)/* 如果指针有效，并且类型符合要求 */
			return temp->parent; /* 返回部件的指针 */
		temp = temp->parent;/* 获取部件的父级部件指针 */
	}
	return NULL;
}

int LCUI_Destroy_App_Widgets(LCUI_ID app_id)
/* 功能：销毁指定ID的程序的所有部件 */
{
	int i, total;
	LCUI_Widget *temp;
	
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	for(i=0; i<total; i++) {
		temp = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list,i);
		if(temp->app_id == app_id) {
			/* 
			 * 在Queue_Delete()函数将队列中的部件移除时，会调用初始化部件队列时指
			 * 定的Destroy_Widget()函数进行部件数据相关的清理。
			 * */
			Queue_Delete(&LCUI_Sys.widget_list, i);
			/* 重新获取部件总数 */
			total = Queue_Get_Total(&LCUI_Sys.widget_list);
			--i;/* 当前位置的部件已经移除，空位已由后面部件填补，所以，--i */
		}
	}
	return 0;
}


LCUI_String Get_Widget_Style(LCUI_Widget *widget)
/* 功能：获取部件的类型 */
{
	return widget->style;
}

LCUI_Border_Style Get_Widget_Border_Style(LCUI_Widget *widget)
/* 功能：获取部件的边框类型 */
{
	return widget->border_style;
}

void Set_Widget_Style(LCUI_Widget *widget, char *style)
/* 功能：设定部件的风格 */
{
	Strcpy(&widget->style, style); 
}

LCUI_Widget *Get_Widget_By_Pos(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * 功能：获取部件中包含指定坐标的点的子部件
 **/
{ 
	if(widget == NULL) 
		return NULL;
	int i, temp;
	LCUI_Widget *child;
	LCUI_Widget *w = widget; 
	for(i=0;;++i)
	{/* 从顶到底遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(NULL == child)
			break;
		if(child->visible == IS_TRUE)
		{ 
			temp = Rect_Inside_Point( pos, Get_Widget_Rect(child) );
			if(temp == 1)
			{/* 如果这个点被包含在部件区域内 */
				/* 递归调用，改变相对坐标 */
				w = Get_Widget_By_Pos(child, Pos_Sub(pos, child->pos));
				if(w == NULL)
					return widget;
				break;
			}
			else 
				continue;
		}
	}
	return w; 
}

LCUI_Widget *Get_Cursor_Overlay_Widget()
/*
 * 功能：获取鼠标光标当前覆盖的部件
 **/
{ 
	int temp;
	int total,k; 
	LCUI_Widget *widget, *w = NULL;
	
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	for (k=0; k<total; ++k)
	{/* 从最顶到底，遍历部件 */ 
		widget = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, k);
		if(widget == NULL) 
			break;
		if(widget->visible != IS_TRUE)
			continue;
			
		temp = Rect_Inside_Point(Get_Cursor_Pos(), Get_Widget_Rect(widget) );
		if(temp == 1)
		{/* 如果这个点被包含在部件区域内，那就获取部件中包含指定坐标的点的子部件 */ 
			w = Get_Widget_By_Pos(widget, 
					Pos_Sub(Get_Cursor_Pos(), Get_Widget_Pos(widget)));
			if(w == NULL) 
				w = widget; 
			break;
		}
		else 
			continue; 
	} 
	return w;  
}



LCUI_Widget *Get_Focus_Widget()
/*
 * 功能：获取当前焦点状态下的部件
 * 返回值：正常获取则返回部件的指针，否则，返回NULL
 **/
{
	#ifdef USE_JUNK___
	LCUI_App *app = Get_Self_AppPointer();
	LCUI_Widget *widget;
	widget = (LCUI_Widget*)Queue_Get(&app->widget_display, 0);
	if(widget != NULL)
	{
		while(widget->focus != NULL)
		{
			widget = widget->focus;
		}
	}
	else return NULL;
	return widget;
	#else 
	return NULL;
	#endif
}

int Widget_Is_Active(LCUI_Widget *widget)
/*
 * 功能：判断部件是否为活动状态
 **/
{
	if(widget->status != KILLED) return 1;
	return 0;
}


LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget)
/* 
 * 功能：获取部件在屏幕中实际显示的区域 
 * 说明：返回的是部件需要裁剪的区域
 * */
{
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect cut_rect;
	cut_rect.x = 0;
	cut_rect.y = 0;
	cut_rect.width = widget->size.w;
	cut_rect.height = widget->size.h;
	
	pos = widget->pos; 
	if(widget->parent == NULL) {
		w = Get_Screen_Width();
		h = Get_Screen_Height();
	} else {
		w = widget->parent->size.w;
		h = widget->parent->size.h;
	}
	
	/* 获取需裁剪的区域 */
	if(pos.x < 0) {
		cut_rect.width += pos.x;
		cut_rect.x = 0 - pos.x; 
	}
	if(pos.x + widget->size.w > w)
		cut_rect.width -= (pos.x +  widget->size.w - w); 
	
	if(pos.y < 0) {
		cut_rect.height += pos.y;
		cut_rect.y = 0 - pos.y; 
	}
	if(pos.y + widget->size.h > h)
		cut_rect.height -= (pos.y +  widget->size.h - h); 
	
	if(widget->parent == NULL)
		return cut_rect;
		
	LCUI_Rect rect;
	/* 获取父部件的有效显示范围 */
	rect = Get_Widget_Valid_Rect(widget->parent);
	/* 如果父部件需要裁剪，那么，子部件根据情况，也需要进行裁剪 */
	if(rect.x > 0) {/* 如果裁剪区域的x轴坐标大于0 */
		/* 裁剪区域和部件区域是在同一容器中，只要得出两个区域的重叠区域即可 */
		temp = pos.x + cut_rect.x;
		if(temp < rect.x) { /* 如果部件的 x轴坐标+裁剪起点x轴坐标 小于它 */
			temp = rect.x - pos.x;		/* 新的裁剪区域起点x轴坐标 */
			cut_rect.width -= (temp - cut_rect.x);/* 改变裁剪区域的宽度 */
			cut_rect.x = temp;			/* 改变部件的裁剪区域的x坐标 */
		}
	}
	if(rect.y > 0) {
		temp = pos.y + cut_rect.y;
		if(pos.y < rect.y) {
			temp = rect.y - pos.y;
			cut_rect.height -= (temp - cut_rect.y);
			cut_rect.y = temp;
		}
	}
	if(rect.width < w) {/* 如果父部件裁剪区域的宽度小于父部件的宽度 */
		temp = pos.x+cut_rect.x+cut_rect.width;
		if(temp > rect.x+rect.width) /* 如果部件裁剪区域左边部分与父部件裁剪区域重叠 */
			cut_rect.width -= (temp-(rect.x+rect.width));
	}
	if(rect.height < h) {
		temp = pos.y+cut_rect.y+cut_rect.height;
		if(temp > rect.y+rect.height)
			cut_rect.height -= (temp-(rect.y+rect.height));
	} 
	
	return cut_rect;
}

int Empty_Widget()
/* 
 * 功能：用于检测程序的部件列表是否为空 
 * 返回值：
 *   1  程序的部件列表为空
 *   0  程序的部件列表不为空
 * */
{
	if(Queue_Get_Total(&LCUI_Sys.widget_list) <= 0)
		return 1;
	return 0;
}

static void WidgetData_Init(LCUI_Queue *queue);

LCUI_Widget *Create_Widget(char *widget_type)
/* 
 * 功能：创建指定类型的窗口部件
 * 说明：创建出来的部件，默认是没有背景图时透明。
 * 返回值：成功则部件的指针，失败则返回NULL
 */
{
	int pos;
	LCUI_Widget *p, widget;
	LCUI_App *app;
	void (*func_init) (LCUI_Widget*);

	app = Get_Self_AppPointer(); 
	
	/* 初始化结构体中的数据 */
	widget.auto_size	= IS_FALSE;
	widget.type_id		= 0;
	widget.status		= WIDGET_STATUS_NORMAL;
	widget.app_id		= app->id; 
	widget.parent		= NULL;
	widget.enabled		= IS_TRUE;
	widget.visible		= IS_FALSE; 
	widget.limit_pos	= IS_FALSE; 
	widget.pos		= Pos(0, 0); 
	widget.max_pos		= Pos(0, 0); 
	widget.min_pos		= Pos(0, 0); 
	widget.limit_size	= IS_FALSE; 
	widget.size		= Size(0, 0); 
	widget.max_size		= Size(0, 0); 
	widget.min_size		= Size(0, 0); 
	widget.align		= NONE; 
	widget.pos_type		= POS_TYPE_IN_SCREEN;
	widget.offset		= Pos(0, 0); 
	widget.back_color	= RGB(238,243,250);
	widget.fore_color	= RGB(0,0,0);
	widget.border_color	= RGB(0,0,0);
	widget.border_style	= NONE;
	widget.private		= NULL;
	widget.bg_mode		= BG_MODE_TRANSPARENT;
	widget.response_flag	= 0;
	
	widget.set_align = Set_Widget_Align;
	widget.set_alpha = Set_Widget_Alpha;
	widget.set_border = Set_Widget_Border;
	widget.show = Show_Widget;
	widget.hide = Hide_Widget;
	widget.resize = Resize_Widget;
	widget.move = Move_Widget;
	widget.enable = Enable_Widget;
	widget.disable = Disable_Widget;
	
	//widget.matrix		= NULL;
	/* 初始化边框数据 */
	Border_Init(&widget.border);
	/* 初始化储存图形数据的结构体 */
	Graph_Init(&widget.graph);
	Graph_Init(&widget.background_image);
	widget.graph.flag = HAVE_ALPHA;
	RectQueue_Init(&widget.update_area);/* 初始化区域更新队列 */
	EventQueue_Init(&widget.event);/* 初始化部件的事件队列 */
	WidgetQueue_Init(&widget.child);/* 初始化子部件队列 */
	WidgetData_Init(&widget.data);/* 初始化数据更新队列 */
	String_Init(&widget.type);/* 初始化字符串 */
	String_Init(&widget.style);
	
	/* 最后，将该部件数据添加至部件队列中 */
	pos = Queue_Add(&LCUI_Sys.widget_list, &widget);
	p = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, pos);
	
	if(widget_type != NULL) {
		/* 验证部件类型是否有效 */
		if( !Check_WidgetType(widget_type)) {
			puts(WIDGET_ERROR_TYPE_NOT_FOUND);
			return NULL;
		}
		/* 保存部件类型 */
		Strcpy(&p->type, widget_type);
		p->type_id = WidgetType_Get_ID(widget_type);	/* 获取类型ID */
		/* 获取初始化部件私有结构体数据的函数指针 */ 
		func_init = Get_WidgetFunc_By_ID(p->type_id, FUNC_TYPE_INIT);
		/* 进行初始化 */
		func_init( p ); 
	} 
	return p;
}


void Delete_Widget(LCUI_Widget *widget)
/* 功能：删除一个部件 */
{
	int i, total;
	LCUI_Queue *p;
	LCUI_Widget *tmp;
	if(NULL == widget)
		return;
		
	if(widget->parent == NULL) p = &LCUI_Sys.widget_list;
	else p = &widget->parent->child; 
	
	total = Queue_Get_Total(p);
	for(i=0; i<total; ++i) {
		tmp = (LCUI_Widget*)Queue_Get(p, i);
		if(tmp == widget) {
			Queue_Delete(p, i);
			break;
		}
	}	
}

LCUI_Pos Count_Widget_Pos(LCUI_Widget *widget)
/* 功能：累计部件的位置坐标 */
{
	LCUI_Pos pos;
	if(widget->parent == NULL) return widget->pos; 
	pos = Count_Widget_Pos(widget->parent); 
	pos = Pos_Add(pos, widget->pos);
	return pos;
}

LCUI_Pos Get_Widget_Global_Pos(LCUI_Widget *widget)
/* 功能：获取部件的全局坐标 */
{
	return Count_Widget_Pos(widget);
}

void Set_Widget_BG_Mode(LCUI_Widget *widget, LCUI_BG_Mode bg_mode)
/*
 * 功能：改变部件的背景模式
 * 说明：背景模式决定了部件在没有背景图的时候是使用背景色填充还是完全透明。
 **/
{
	widget->bg_mode = bg_mode;
}

void Set_Widget_Align(LCUI_Widget *widget, LCUI_Align align, LCUI_Pos offset)
/* 功能：设定部件的对齐方式以及偏移距离 */
{
	widget->align = align;
	widget->offset = offset;
	Update_Widget_Pos(widget);/* 更新位置 */
}

void Limit_Widget_Size(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size)
/* 功能：限制部件的尺寸变动范围 */
{
	if(min_size.w < 0)
		min_size.w = 0;
	if(min_size.h < 0)
		min_size.h = 0;
	if(max_size.w < 0)
		max_size.w = 0;
	if(max_size.h < 0)
		max_size.h = 0;
	if(max_size.w < min_size.w)
		max_size.w = min_size.w;
	if(max_size.h < min_size.h)
		max_size.h = min_size.h;
		
	widget->min_size = min_size;
	widget->max_size = max_size;
	widget->limit_size = IS_TRUE;
}

void Limit_Widget_Pos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos)
/* 功能：限制部件的移动范围 */
{
	if(min_pos.x < 0)
		min_pos.x = 0;
	if(min_pos.y < 0)
		min_pos.y = 0;
	if(max_pos.x < 0)
		max_pos.x = 0;
	if(max_pos.y < 0)
		max_pos.y = 0;
	if(max_pos.x < min_pos.x)
		max_pos.x = min_pos.x;
	if(max_pos.y < min_pos.y)
		max_pos.y = min_pos.y;
		
	widget->min_pos = min_pos;
	widget->max_pos = max_pos;
	widget->limit_pos = IS_TRUE;
}

void Set_Widget_Border(LCUI_Widget *widget, LCUI_RGB color, LCUI_Border border)
/* 功能：设定部件的边框 */
{ 
	widget->border = border;
	widget->border_color = color;
	
	if(widget->border_style == BORDER_STYLE_NONE) 
		Set_Widget_Border_Style(widget, BORDER_STYLE_LINE_BORDER);
		
	Draw_Widget(widget); 
	Add_Widget_Refresh_Area(widget, Get_Widget_Rect(widget));
}


void Set_Widget_Backcolor(LCUI_Widget *widget, LCUI_RGB color)
/* 
* 功能：设定部件的背景色 
*/
{
	widget->back_color = color;
	Draw_Widget(widget);
	Refresh_Widget(widget);
}

int Set_Widget_Background_Image(LCUI_Widget *widget, LCUI_Graph *img, int flag)
/*
 * 功能：为部件填充背景图像
 */
{
	if(NULL == img) Free_Graph(&widget->background_image);
	else {
		widget->background_image_layout = flag;
		/* 填充背景图像 */
		Copy_Graph(&widget->background_image, img);
	}
	Draw_Widget(widget); 
	return 0;
}

int Mix_Widget_FontBitmap(	LCUI_Widget *widget, 
				int start_x, int start_y, 
				LCUI_WString *contents, 
				int rows, int space, 
				int linegap, int flag	)
/* 功能：混合部件内的字体位图，使字体能在部件上显示 */
{
	if(!Valid_Graph(&widget->graph)) return -1; 
	/* 如果文字行数不大于0 */ 
	if(rows <= 0) return -2;
	
	int i, j ,x = start_x, y = start_y, height = 0;
	for(i = 0; i < rows; ++i) {
		for(j=0; j < contents[i].size; ++j) {
			if(contents[i].string[j].bitmap.height > 0) {
				height = contents[i].string[j].bitmap.height;
				break;
			}
		}
		if(height > 0) break;
	}
	for(i = 0; i < rows; ++i) {
		/* 如果已经超出部件的尺寸，就不需要绘制字体位图了 */
		if(y > widget->size.h) break; 
		for(j=0; j < contents[i].size; ++j) {
			if(x > widget->size.w) break; 
			/* 粘贴每个字 */
			Mix_Fonts_Bitmap(
				&widget->graph,  x, y , 
				&contents[i].string[j].bitmap , 
				contents[i].string[j].color, flag );
			
			/* 如果这一行文字需要更新 */
			if(contents[i].update == IS_TRUE) {
				contents[i].update = IS_FALSE;
				Add_Widget_Refresh_Area( widget, 
					Rect(	start_x, y-1, widget->size.w, 
					contents[i].string[0].bitmap.height+2 )
				);
				/* 稍微增加点区域范围，所以y-1，height+2 */
			}
			/* 累计文字间距 */
			if( j < contents[i].size - 1) x = x + space;  
			/* 累计文字宽度 */
			x = x + contents[i].string[j].bitmap.width;
		}
		if(contents[i].size == 0) y += height;
		else y += contents[i].string[0].bitmap.height; 
		/* 累计文字行距 */
		if(i > 0 && i < rows - 1) y += linegap; 
		/* x归位 */
		x = start_x;
		//printf("Mix_Widget_FontBitmap():rows:%d/%d, size:%d\n", i, rows, contents[i].size);
	} 
	return 0;
}

void Enable_Widget(LCUI_Widget *widget)
/* 功能：启用部件 */
{
	widget->enabled = IS_TRUE; 
	Set_Widget_Status(widget, WIDGET_STATUS_NORMAL);
}

void Disable_Widget(LCUI_Widget *widget)
/* 功能：禁用部件 */
{
	widget->enabled = IS_FALSE; 
	Set_Widget_Status(widget, WIDGET_STATUS_DISABLE);
}

void Widget_Visible(LCUI_Widget *widget, int flag)
/* 功能：定义部件是否可见 */
{
	if(flag == IS_TRUE) 
		widget->visible = IS_TRUE; 
	else 
		widget->visible = IS_FALSE; 
}

void Set_Widget_Pos(LCUI_Widget *widget, LCUI_Pos pos)
/* 
 * 功能：设定部件的位置 
 * 说明：只修改坐标，不进行局部刷新
 * */
{
	widget->pos = pos;
}

void Set_Widget_Alpha(LCUI_Widget *widget, unsigned char alpha)
/* 功能：设定部件的透明度 */
{
	if(widget->graph.alpha != alpha)
	{
		widget->graph.alpha = alpha; 
		Refresh_Widget(widget);
	}
}

void Exec_Move_Widget(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * 功能：执行移动部件位置的操作
 * 说明：更改部件位置，并添加局部刷新区域
 **/
{
	LCUI_Pos t;
	LCUI_Rect old_rect;
	if(IS_TRUE == widget->limit_pos) {
		/* 如果指定了限制部件的移动范围，那么就调整位置 */
		if(pos.x > widget->max_pos.x)
			pos.x = widget->max_pos.x;
		if(pos.y > widget->max_pos.y)
			pos.y = widget->max_pos.y;
		if(pos.x < widget->min_pos.x)
			pos.x = widget->min_pos.x;
		if(pos.y < widget->min_pos.y)
			pos.y = widget->min_pos.y;
	}
	t = widget->pos; /* 记录老位置 */
	widget->pos = pos;/* 记录新位置 */
	if(widget->visible == IS_TRUE) {/* 如果该部件可见 */
		old_rect = Rect(t.x, t.y, widget->size.w, widget->size.h);
		Add_Widget_Refresh_Area(widget->parent, old_rect); /* 刷新老区域 */
		Refresh_Widget(widget); /* 刷新现在的区域 */
		/* 
		 * 不用担心老区域和新区域存在重叠区域而导致屏幕刷新效率下降的问题，因为LCUI会
		 * 在处理这些区域时，会将这些区域进行分割和删减，确保区域独立且不重叠。
		 *  */
	}
}

void Exec_Hide_Widget(LCUI_Widget *widget)
/* 功能：执行隐藏部件的操作 */
{
	if(widget == NULL) 
		return;
	if(widget->visible == IS_FALSE)
		return;
	
	void ( *func_hide ) (LCUI_Widget*);
	
	/* 获取隐藏部件需要调用的函数指针，并调用之 */
	func_hide = Get_WidgetFunc_By_ID( widget->type_id,
					FUNC_TYPE_HIDE );
	func_hide(widget);
	
	Widget_Visible(widget, IS_FALSE); 
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget)); 
}


void Exec_Show_Widget(LCUI_Widget *widget)
/* 功能：执行显示部件的任务 */
{ 
	void ( *func_show ) (LCUI_Widget*); 

	if(widget == NULL) 
		return;
		
	if(widget->visible == IS_TRUE)
		return;
		
	//Exec_Update_Widget(widget);/* 显示前，需要对部件图形数据进行一次更新 */
	Widget_Visible(widget, IS_TRUE); /* 部件可见 */
	
	/* 调用该部件在显示时需要用到的函数 */
	func_show = Get_WidgetFunc_By_ID(widget->type_id, FUNC_TYPE_SHOW); 
	func_show(widget); 
	
	Refresh_Widget(widget); /* 刷新部件所在区域的图形显示 */
}

void Auto_Resize_Widget(LCUI_Widget *widget)
/* 功能：自动调整部件大小，以适应其内容大小 */
{
	int i, total, temp;
	LCUI_Widget *child;
	LCUI_Queue point;
	LCUI_Size size;
	
	size.w = 0;
	size.h = 0;
	Queue_Init(&point, sizeof (LCUI_Pos), NULL);
	total = Queue_Get_Total(&widget->child);
	for(i=0; i<total; ++i)
	{/* 遍历每个子部件 */
		child = (LCUI_Widget *)Queue_Get(&widget->child, i);
		/* 保存各个子部件区域矩形的右上角顶点中，X轴坐标最大的 */
		temp = child->pos.x + child->size.w;
		if(temp > size.w)
			size.w = temp;
		/* 同上 */
		temp = child->pos.y + child->size.h;
		if(temp > size.h)
			size.h = temp;
	}
	size.w += 6;
	size.h += 6;
	//printf("Auto_Resize_Widget(): new size: %d,%d\n", size.w, size.h);
	//print_widget_info(widget);
	/* 得出适合的尺寸，调整之 */
	Resize_Widget(widget, size);
}

void Exec_Resize_Widget(LCUI_Widget *widget, LCUI_Size size)
/* 功能：执行改变部件尺寸的操作 */
{
	if(widget == NULL) 
		return;
		
	void ( *func_resize ) (LCUI_Widget*);
	
	if(widget->size.w == size.w && widget->size.h == size.h) 
		return;
	if(IS_TRUE == widget->limit_size) {
		/* 如果指定了限制部件的尺寸，那么就调整尺寸 */
		if(size.w > widget->max_size.w)
			size.w = widget->max_size.w;
		if(size.h > widget->max_size.h)
			size.h = widget->max_size.h;
		if(size.w < widget->min_size.w)
			size.w = widget->min_size.w;
		if(size.h < widget->min_size.h)
			size.h = widget->min_size.h;
	}
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget));
	widget->size = size;
	Malloc_Graph(&widget->graph, size.w, size.h);
	/* 获取改变部件尺寸时需要调用的函数 */
	func_resize = Get_WidgetFunc_By_ID( widget->type_id, 
						FUNC_TYPE_RESIZE );
	func_resize(widget); 
	Refresh_Widget(widget); 
	Update_Child_Widget_Pos(widget);/* 更新子部件的位置 */  
	
	if(widget->parent != NULL
	&& widget->parent->auto_size == IS_TRUE)
	/* 如果需要让它的容器能够自动调整大小 */
		Auto_Resize_Widget(widget->parent); 
}

void Enable_Widget_Auto_Size(LCUI_Widget *widget)
/* 功能：启用部件自动尺寸调整功能 */
{
	widget->auto_size = IS_TRUE;
}

void Disable_Widget_Auto_Size(LCUI_Widget *widget)
/* 功能：禁用部件自动尺寸调整功能 */
{
	widget->auto_size = IS_FALSE;
}

void Exec_Refresh_Widget(LCUI_Widget *widget)
/* 功能：执行刷新显示指定部件的整个区域图形的操作 */
{ 
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget));
}

void Exec_Draw_Widget(LCUI_Widget *widget)
/* 功能：执行部件图形更新操作 */
{ 
	if(widget == NULL) 
		return;
		 
	void ( *func_update ) (LCUI_Widget*); 
	
	if(Valid_Graph(&widget->background_image)) {/* 如果有背景图 */
		/* alpha通道中的每个像素的透明值为255，整个部件的图形不透明 */
		Fill_Graph_Alpha(&widget->graph, 255);
		
		Fill_Background_Image(
				&widget->graph, 
				&widget->background_image, 
				widget->background_image_layout,
				widget->back_color
		); /* 填充背景色 */
	} else {/* 否则根据背景模式来处理 */
		switch(widget->bg_mode) {
		    case BG_MODE_FILL_BACKCOLOR: /* 填充背景色 */
			Fill_Graph_Alpha(&widget->graph, 255);
			Fill_Color(&widget->graph, widget->back_color); 
			break;
				
		    case BG_MODE_TRANSPARENT: /* 完全透明 */
			Fill_Graph_Alpha(&widget->graph, 0); 
			break;
		    default:break;
		}
	}
	
	/* 获取函数 */
	func_update = Get_WidgetFunc_By_ID( widget->type_id, FUNC_TYPE_UPDATE );
	func_update(widget);
	/* 绘制边框线 */
	if(widget->border_style != BORDER_STYLE_NONE)
		Draw_Graph_Border( &widget->graph, widget->border_color, 
							widget->border); 
}


LCUI_Pos Widget_Align_Get_Pos(LCUI_Widget *widget)
/* 功能：根据部件的布局来获取部件的位置 */
{ 
	LCUI_Pos pos;
	LCUI_Size size;
	pos.x = pos.y = 0;
	/* 根据位置类型，来获取容器的尺寸 */ 
	switch(widget->pos_type) {
	    case POS_TYPE_IN_SCREEN :
		size = Get_Screen_Size(); 
		break;
	    case POS_TYPE_IN_WIDGET : 
		size = Get_Widget_Size(widget->parent); 
		break;
		default: return pos; 
	} 
	if(widget->align != ALIGN_NONE){
		pos = Align_Get_Pos(size, Get_Widget_Size(widget), widget->align);
		/* 加上偏移距离 */
		pos.x = pos.x + widget->offset.x;
		pos.y = pos.y + widget->offset.y; 
		return pos; 
	}
	return widget->pos;
}

/* 声明函数 */
static int Record_WidgetUpdate(LCUI_Widget *widget, void *data, int type);

void Move_Widget(LCUI_Widget *widget, LCUI_Pos new_pos)
/* 
 * 功能：移动部件位置
 * 说明：如果部件的布局为ALIGN_NONE，那么，就可以移动它的位置，否则，无法移动位置
 * */
{
	if(widget == NULL) 
		return;
	/* 记录部件的更新数据，等待进行更新 */
	Record_WidgetUpdate(widget, &new_pos, DATATYPE_POS);
}

void Update_Widget_Pos(LCUI_Widget *widget)
/* 功能：更新部件的位置 */
{ 
	Record_WidgetUpdate(widget, NULL, DATATYPE_POS);
}

void Update_Child_Widget_Pos(LCUI_Widget *widget)
/* 
 * 功能：更新指定部件的子部件的位置
 * 说明：当作为子部件的容器部件的尺寸改变后，有的部件的布局不为ALIGN_NONE，就需要重新
 * 调整位置。
 * */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&widget->child);
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		Update_Widget_Pos( child ); 
	}
}

void Offset_Widget_Pos(LCUI_Widget *widget, LCUI_Pos offset)
/* 功能：以部件原有的位置为基础，根据指定的偏移坐标偏移位置 */
{
	Move_Widget( widget, Pos_Add(widget->pos, offset) ); 
}

void Move_Widget_To_Pos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed)
/* 
 * 功能：将部件以指定的速度向指定位置移动 
 * 说明：des_pos是目标位置，speed是该部件的移动速度，单位为：像素/秒
 * */
{
	if(speed <= 0) return;
	
	int i, j;
	double w, h, l, n, x, y; 
	x = Get_Widget_Pos(widget).x;
	y = Get_Widget_Pos(widget).y;
	/* 求两点之间的距离 */
	w = des_pos.x-x;
	h = des_pos.y-y;
	l = sqrt(pow(w, 2) + pow(h, 2));
	
	n = l/speed;/* 求移动所需时间 */
	n = n*100;	/* 求移动次数，乘以100，是因为每隔0.01移动一次位置 */
	w = w/n;	/* 计算每次移动的x和y轴坐标的移动距离 */
	h = h/n;
	j = (int)(n>(int)n?n+1:n);
	for(i=0; i<j; i++) {
		x += w;
		y += h;
		Move_Widget(widget, Pos(x, y));
		usleep(10000);/* 停顿0.01秒 */
	}
}

void Refresh_Widget(LCUI_Widget *widget)
/* 功能：刷新显示指定部件的整个区域图形 */
{
	Record_WidgetUpdate(widget, NULL, DATATYPE_AREA);
}

void Resize_Widget(LCUI_Widget *widget, LCUI_Size new_size)
/* 功能：改变部件的尺寸 */
{
	if(widget == NULL) return; 
		
	Record_WidgetUpdate(widget, &new_size, DATATYPE_SIZE);
}

void Draw_Widget(LCUI_Widget *widget)
/* 功能：重新绘制部件 */
{
	if(widget == NULL) return; 
		
	Record_WidgetUpdate(widget, NULL, DATATYPE_GRAPH);
}


void Front_Widget(LCUI_Widget *widget)
/* 功能：将指定部件的显示位置移动到最前端 */
{
	LCUI_Queue *queue;
	/* 获取指向队列的指针 */
	if(widget->parent == NULL) queue = &LCUI_Sys.widget_list;
	else queue = &widget->parent->child;
	
	/* 将该部件移动至队列前端 */
	WidgetQueue_Move(queue, 0, widget);
} 

void Show_Widget(LCUI_Widget *widget)
/* 功能：显示部件 */
{
	if(widget == NULL) return; 
	Front_Widget(widget);/* 改变部件的排列位置 */
	Record_WidgetUpdate(widget, NULL, DATATYPE_SHOW);
}

void Hide_Widget(LCUI_Widget *widget)
/* 功能：隐藏部件 */
{
	if(widget == NULL) return;
	Record_WidgetUpdate(widget, NULL, DATATYPE_HIDE); 
}

void Set_Widget_Status(LCUI_Widget *widget, int status)
/* 功能：设定部件的状态 */
{
	if(widget == NULL) return; 
	Record_WidgetUpdate(widget, &status, DATATYPE_STATUS); 
}
/************************* Widget End *********************************/



/**************************** Widget Update ***************************/
typedef struct _WidgetData
{
	void	*data;	/* 需要更新的数据 */
	int	type;	/* 这个数据的类型 */
}
WidgetData;

static void Destroy_WidgetData(void *arg)
/* 功能：释放WidgetData结构体中的指针 */
{
	WidgetData *wdata = (WidgetData*)arg;
	if(NULL == wdata) return;
	if(wdata->data != NULL) free(wdata->data);
}

static void WidgetData_Init(LCUI_Queue *queue)
/* 功能：初始化记录部件数据更新的队列 */
{
	Queue_Init(queue, sizeof(WidgetData), Destroy_WidgetData);
}

static int Find_WidgetData(LCUI_Widget *widget, const WidgetData *data)
/* 功能：查询指定数据类型是否在队列中 */
{
	WidgetData *temp;
	int i, total;
	total = Queue_Get_Total(&widget->data);	/* 获取成员总数 */
	for(i=0; i<total; ++i) {
		temp = (WidgetData*)Queue_Get(&widget->data, i);
		if(NULL == temp) break;
		if(temp->type == data->type) return i; 
	}
	return -1;
}

static int Record_WidgetUpdate(LCUI_Widget *widget, void *data, int type)
/* 
 * 功能：记录需要进行数据更新的部件
 * 说明：将部件指针以及需更新的数据添加至队列，根据部件的显示顺序来排列队列
 * 返回值：出现问题则返回-1，正常返回不小于0的值
 *  */
{ 
	int pos, result = 0;
	size_t size = 0;
	WidgetData temp;
	/* 根据类型，来得知占用空间大小 */
	switch(type) {
	    case DATATYPE_POS	: size = sizeof(LCUI_Pos);break; /* 位置 */
	    case DATATYPE_SIZE	: size = sizeof(LCUI_Size);break; /* 尺寸 */
	    case DATATYPE_GRAPH	: size = sizeof(LCUI_Graph);break;/* 图形 */
	    case DATATYPE_STATUS: size = sizeof(int);break; /* 状态 */
	    case DATATYPE_AREA:
	    case DATATYPE_HIDE:
	    case DATATYPE_SHOW:	 break;
	    default: return -1;
	}
	
	temp.type = type;			/* 保存类型 */
	if(data != NULL) {
		temp.data = malloc(size);	/* 分配内存 */
		memcpy(temp.data, data, size);	/* 拷贝数据 */ 
	}
	else temp.data = NULL;
	  
	pos = Find_WidgetData(widget, &temp);
	if(pos >= 0)	/* 如果已经存在，就覆盖 */ 
		result = Queue_Replace(&widget->data, pos, &temp); 
	else	/* 否则，追加至队列末尾 */
		result = Queue_Add(&widget->data, &temp);
		
	return result;
}


int Process_WidgetUpdate(LCUI_Widget *widget)
/* 功能：处理部件的更新 */
{
	LCUI_Widget *child;
	WidgetData *temp;
	LCUI_Pos pos;
	int i, total;
	/* 处理部件中需要更新的数据 */
	//printf("Process_WidgetUpdate(): enter\n");
	Queue_Lock(&widget->data);/* 锁定队列，其它线程暂时不能访问 */
	while( ! Queue_Empty(&widget->data) ) {
		temp = (WidgetData *)Queue_Get(&widget->data, 0);
		/* 根据不同的类型来进行处理 */
		switch(temp->type) {
		    case DATATYPE_SIZE	: /* 部件尺寸更新，将更新部件的位置 */ 
			Exec_Resize_Widget(widget, *((LCUI_Size*)temp->data));
			Exec_Draw_Widget(widget);
				/* 需要更新位置，所以不用break */ 
		    case DATATYPE_POS	:
			/*
			 * 由于更新位置可能会是更新部件尺寸的附加操作，需要判断一下更新类型
			 * 是否为DATATYPE_POS 
			 * */
			if( temp->type == DATATYPE_POS 
			 && widget->align == ALIGN_NONE 
			 && temp->data != NULL )
				pos = *((LCUI_Pos*)temp->data);
			else
				pos = Widget_Align_Get_Pos(widget); 
				
			Exec_Move_Widget(widget, pos);
			break;
		    case DATATYPE_STATUS:
			widget->status = *(int*)temp->data;
				/* 改变部件状态后需要进行重绘，所以不用break */
		    case DATATYPE_GRAPH	:
			Exec_Draw_Widget(widget); 
			break;
		    case DATATYPE_HIDE:
			Exec_Hide_Widget(widget);
			break;
		    case DATATYPE_SHOW: 
			Exec_Show_Widget(widget); 
			break;
		    case DATATYPE_AREA:
			Exec_Refresh_Widget(widget);
			break;
		    default: break;
		} 
		Queue_Delete(&widget->data, 0);/* 移除该成员 */
	}
	Queue_UnLock(&widget->data);
	total = Queue_Get_Total(&widget->child);  
	for(i=total-1; i>=0; --i) {
		/* 从尾到首获取部件指针 */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);  
		if(child != NULL) {/* 递归调用 */
			Process_WidgetUpdate( child );
		}
	}
	//printf("Process_WidgetUpdate(): quit\n");
	/* 解锁 */
	return 0;
}

void Process_All_WidgetUpdate()
/* 功能：处理所有部件的更新 */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&LCUI_Sys.widget_list); 
	//printf("Process_All_WidgetUpdate():start\n");
	for(i=total-1; i>=0; --i) {
		/* 从尾到首获取部件指针 */
		child = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, i); 
		//printf("child: ");print_widget_info(child);
		if(child != NULL) {/* 递归调用 */ 
			Process_WidgetUpdate( child );
		}
	}
	//printf("Process_All_WidgetUpdate():end\n");
}

/************************ Widget Update End ***************************/



/************************** Widget Library ****************************/

typedef struct _WidgetTypeData
{ 
	LCUI_String		type;	 /* 部件类型，用字符串描述 */
	LCUI_ID		type_id; /* 类型ID */
	LCUI_FuncQueue	func;	 /* 部件相关的函数 */
}WidgetTypeData;

int WidgetFunc_Add(
			const char *type, 
			void (*widget_func)(LCUI_Widget*), 
			FuncType func_type
		)
/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/
{
	WidgetTypeData *temp;
	LCUI_Func *temp_func, func_data;
	int total, i, found = 0;
	
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) {
		printf("WidgetFunc_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}
	
	//printf("WidgetFunc_Add(): widget type: %s, func type: %d\n", type, func_type); 
	
	total = Queue_Get_Total(&app->widget_lib); 
		/* 遍历数据，找到对应的位置 */
	for(i = 0; i < total; ++i) {
		temp = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(strcmp(temp->type.string, type) == 0) { 
			total = Queue_Get_Total(&temp->func); 
			for(i=0; i<total; i++) {
				temp_func = Queue_Get(&temp->func, i);
				if(temp_func->id == func_type) {
					found = 1;
					break;
				}
			}
			
			Get_FuncData(&func_data, widget_func, NULL, NULL);
			func_data.id = func_type; /* 保存类型ID */
			if(found == 1) {/* 如果已经存在，就覆盖 */
				//printf("WidgetFunc_Add(): the function is already registered. repalce\n");
				Queue_Replace(&temp->func, i, &func_data); 
			} else Queue_Add(&temp->func, &func_data); 
			return 0;
		}
	}
	//printf("WidgetFunc_Add(): warning: the widget type was never recorded\n");
	return -1;
}

int WidgetType_Add(char *type)
/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/
{
	WidgetTypeData *wd, new_wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) {
		printf("WidgetType_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}
	
	int total, i; 
	//printf("WidgetType_Add(): add widget type: %s\n", type);
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, type) == 0) { /* 如果类型一致 */
			//printf("WidgetType_Add(): the widget type is already registered\n");
			return -1;
		}
	} 
	
	/* 以下需要初始化 */
	FuncQueue_Init(&new_wd.func);
	new_wd.type_id = rand()%100000; /* 用随机数作为类型ID */
	//printf("WidgetType_Add(): widget type id: %ld\n", new_wd.type_id); 
	String_Init(&new_wd.type);
	Strcpy(&new_wd.type, type);/* 保存部件类型 */
	Queue_Add(&app->widget_lib, &new_wd);/* 添加至队列 */
	return 0;
}

static void Destroy_WidgetType(void *arg)
/* 功能：移除部件类型数据 */
{
	WidgetTypeData *wd = (WidgetTypeData*)arg;
	Destroy_Queue(&wd->func);
}

void WidgetLib_Init(LCUI_Queue *w_lib)
/* 功能：初始化部件库 */
{
	Queue_Init(w_lib, sizeof(WidgetTypeData), Destroy_WidgetType);
}

int WidgetType_Delete(const char *type)
/* 功能：删除指定部件类型的相关数据 */
{
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	
	if(app == NULL) return -2;
	
	int total,  i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, type) == 0) /* 如果类型一致 */
			return Queue_Delete(&app->widget_lib, i);
	} 
	
	return -1;
}

void NULL_Widget_Func(LCUI_Widget *widget)
/*
 * 功能：空函数，不做任何操作
 * 说明：如果获取指定部件类型的函数指针失败，将返回这个函数的函数指针
 **/
{
	
}

LCUI_ID WidgetType_Get_ID(const char *widget_type)
/* 功能：获取指定类型部件的类型ID */
{ 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return -2;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0) { /* 如果类型一致 */
			return wd->type_id;
		}
	}
	
	return -1;
}



int Get_Widget_Type_By_ID(LCUI_ID id, char *widget_type)
/* 功能：获取指定类型ID的类型名称 */
{
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return -2;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id) { /* 如果类型一致 */
			strcpy(widget_type, wd->type.string); 
			return 0;
		}
	}
	
	return -1;
}

void ( *Get_WidgetFunc_By_ID(LCUI_ID id, FuncType func_type) ) (LCUI_Widget*)
/*
 * 功能：获取指定部件类型ID的函数的函数指针
 **/
{
	LCUI_Func *f = NULL; 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) 
		return NULL_Widget_Func;
	
	int total, i, found = 0; 
	//printf("Get_WidgetFunc_By_ID(): widget type id: %lu, func type: %d\n", id, func_type);
	total = Queue_Get_Total(&app->widget_lib); 
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id) { /* 如果类型一致 */  
			total = Queue_Get_Total(&wd->func); 
			for(i=0; i<total; i++) {
				f = Queue_Get(&wd->func, i); 
				if(f->id == func_type) {
					found = 1;
					break;
				}
			}
			/* 如果已经存在 */
			if(found == 1) return f->func; 
			else {
				//printf("Get_WidgetFunc_By_ID(): warning: widget func not found!\n");
				return NULL_Widget_Func;
			}
		}
	}
	
	return NULL_Widget_Func;
}

void ( *Get_WidgetFunc(const char *widget_type, FuncType func_type) ) (LCUI_Widget*)
/* 功能：获取指定类型部件的函数的函数指针 */
{
	LCUI_Func *f = NULL; 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return NULL_Widget_Func;
	
	int total, i, found = 0; 
	
	total = Queue_Get_Total(&app->widget_lib);
	//printf("Get_WidgetFunc(): widget type: %s, func type: %d\n", widget_type, func_type);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0) { /* 如果类型一致 */ 
			total = Queue_Get_Total(&wd->func);
			for(i=0; i<total; i++) {
				f = Queue_Get(&wd->func, i);
				if(f->id == func_type) {
					found = 1;
					break;
				}
			}
			/* 如果已经存在 */
			if(found == 1) return f->func; 
			else {
				//printf("Get_WidgetFunc(): warning: widget func not found!\n");
				return NULL_Widget_Func; 
			}
		}
	}
	
	return NULL_Widget_Func;
}

int Check_WidgetType(const char *widget_type)
/* 功能：检测指定部件类型是否有效 */
{ 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	
	if(app == NULL) return 0;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0)/* 如果类型一致 */ 
			return 1; 
	}
	
	return 0;
}


extern void Register_Window();
extern void Register_Label();
extern void Register_Button();
extern void Register_PictureBox();
extern void Register_ProgressBar();
extern void Register_Menu();
extern void Register_CheckBox();
extern void Register_RadioButton();
extern void Register_ActiveBox();

void Register_Default_Widget_Type()
/* 功能：为程序的部件库添加默认的部件类型 */
{
	WidgetType_Add(NULL); /* 添加一个NULL类型的部件 */
	Register_Window();/* 注册窗口部件 */
	Register_Label();/* 注册文本标签部件 */
	Register_Button();/* 注册按钮部件 */
	Register_PictureBox();/* 注册图片盒子部件 */
	Register_ProgressBar();
	Register_Menu();
	Register_CheckBox();
	Register_RadioButton();
	Register_ActiveBox();
}
/************************ Widget Library End **************************/


/*************************** Container ********************************/
void Widget_Container_Add(LCUI_Widget *container, LCUI_Widget *widget)
/* 功能：将部件添加至作为容器的部件内 */
{
	int pos;
	LCUI_Queue *old_queue, *new_queue;
	
	if(container == widget->parent) return;
		
	if(container == NULL) {/* 如果容器为NULL，那就代表是以屏幕作为容器 */
		new_queue = &LCUI_Sys.widget_list;
		/* 改变部件的位置类型，不改的话，得出来的部件位置会有问题 */
		widget->pos_type = POS_TYPE_IN_SCREEN;
	} else {/* 否则，以指定部件为容器 */ 
		new_queue = &container->child;
		widget->pos_type = POS_TYPE_IN_WIDGET; 
	}
	
	if(widget->parent != NULL){
	/* 如果部件有父部件，那就在父部件的子部件队列中 */ 
		old_queue = &widget->parent->child; 
	} else {
	/* 否则没有部件，那么这个部件在创建时就储存至系统部件队列中 */
		old_queue = &LCUI_Sys.widget_list;
	}
		
	/* 改变该部件的容器，需要将它从之前的容器中移除 */
	pos = WidgetQueue_Get_Pos(old_queue, widget);
	if(pos >= 0) {
		Queue_Delete_Pointer(old_queue, pos);
	}
	
	widget->parent = container; /* 保存父部件指针 */ 
	Queue_Add_Pointer(new_queue, widget); /* 添加至部件队列 */
	Update_Widget_Pos(widget);
}
/************************* Container End ******************************/

