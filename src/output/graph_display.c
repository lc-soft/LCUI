//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include LC_CURSOR_H

#include <unistd.h>

int Get_Screen_Width ()
/*
 * 功能：获取屏幕宽度
 * 返回值：屏幕的宽度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
{
	if ( !LCUI_Sys.init ) {
		return 0; 
	}
	return LCUI_Sys.screen.size.w; 
}

int Get_Screen_Height ()
/*
 * 功能：获取屏幕高度
 * 返回值：屏幕的高度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
{
	if ( !LCUI_Sys.init ) {
		return 0; 
	}
	return LCUI_Sys.screen.size.h; 
}

LCUI_Size Get_Screen_Size ()
/* 功能：获取屏幕尺寸 */
{
	return LCUI_Sys.screen.size; 
}

int Add_Screen_Refresh_Area (LCUI_Rect rect)
/* 功能：在整个屏幕内添加需要刷新的区域 */
{
	if (rect.width <= 0 || rect.height <= 0) {
		return -1; 
	} 
	rect = Get_Valid_Area(Get_Screen_Size(), rect); 
	return RectQueue_Add (&LCUI_Sys.update_area, rect);
}

int Get_Screen_Bits()
/* 功能：获取屏幕中的每个像素的表示所用的位数 */
{
	return LCUI_Sys.screen.bits;
}

LCUI_Pos Get_Screen_Center_Point()
/* 功能：获取屏幕中心点的坐标 */
{
	return Pos(LCUI_Sys.screen.size.w/2.0, LCUI_Sys.screen.size.h/2.0);
}

int Widget_Layer_Is_Opaque(LCUI_Widget *widget)
/* 功能：判断部件图形是否不透明 */
{ 
	return Graph_Is_Opaque(&widget->graph);
}

int Widget_Layer_Not_Visible(LCUI_Widget *widget)
/* 功能：检测部件图形是否完全透明 */
{
	if(Graph_Is_Opaque(&widget->graph) == -1) {
		return 1;
	}
	return 0;
}

void __Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue)
/* 
 * 功能：获取与指定区域重叠的部件 
 * 说明：得到的队列，队列中的部件排列顺序为：底-》上 == 左-》右
 * */
{
	int i, total;
	LCUI_Pos pos;
	LCUI_Rect tmp;
	LCUI_Widget *child;
	LCUI_Queue *widget_list;

	if(widget == NULL) {
		widget_list = &LCUI_Sys.widget_list; 
	} else {
		if( !widget->visible ) {
			return;
		}
		widget_list = &widget->child; 
	}

	total = Queue_Get_Total(widget_list); 
	/* 从底到顶遍历子部件 */
	for(i=total-1; i>=0; --i) {
		child = Queue_Get( widget_list, i ); 
		if( child == NULL || !child->visible ) {
			continue;
		}
		tmp = Get_Widget_Valid_Rect( child ); 
		pos = Get_Widget_Global_Pos( child );
		tmp.x += pos.x;
		tmp.y += pos.y;
		if( !Rect_Valid(tmp) ){
			continue;
		}
		if (Rect_Is_Overlay(tmp, rect)) { 
			/* 记录与该区域重叠的部件 */
			Queue_Add_Pointer( queue, child ); 
			Get_Overlay_Widget( rect, child, queue );  
		} 
	}
}

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue)
/* 
 * 功能：获取与指定区域重叠的部件 
 * 说明：得到的队列，队列中的部件排列顺序为：底-》上 == 左-》右
 * */
{
	int i, flag, total;
	LCUI_Pos pos;
	LCUI_Rect tmp;
	LCUI_Widget *child; 
	LCUI_Queue *widget_list;

	if(widget == NULL) {
		widget_list = &LCUI_Sys.widget_list; 
	} else {
		if( !widget->visible ) {
			return;
		}
		widget_list = &widget->child; 
	}
	flag = 0;
	
filter_widget:;
	/* 从底到顶遍历子部件 */
	total = Queue_Get_Total(widget_list);
	for(i=total-1; i>=0; --i) {
		child = (LCUI_Widget*)Queue_Get(widget_list, i);
		if( child == NULL ) {
			continue;
		}
		
		if( flag==0 ) {
			/* 过滤定位类型不为static的部件 */
			if( child->pos_type != POS_TYPE_STATIC ) {
				continue;
			}
		}
		else if( flag==1 ) {
			/* 过滤定位类型不为relative的部件 */
			if( child->pos_type != POS_TYPE_RELATIVE ) {
				continue;
			}
		}
		else if( flag==2 ) {
			/* 过滤定位类型为relative和static的部件 */
			if( child->pos_type == POS_TYPE_RELATIVE
			 && child->pos_type == POS_TYPE_STATIC ) {
				continue;
			}
		} 
		/* 过滤掉不可见的子部件 */ 
		if( !child->visible ) {
			continue;
		}
		
		tmp = Get_Widget_Valid_Rect(child); 
		pos = Get_Widget_Global_Pos(child);
		tmp.x += pos.x;
		tmp.y += pos.y;
		if(!Rect_Valid(tmp)){
			continue;
		}
		if (Rect_Is_Overlay(tmp, rect)) { 
			/* 记录与该区域重叠的部件 */
			Queue_Add_Pointer(queue, child);
			/* 递归调用 */
			Get_Overlay_Widget(rect, child, queue);  
		}
	}
	++flag;
	if(flag < 3) {
		goto filter_widget;
	} 
}


int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph)
/* 获取屏幕中指定区域内实际要显示的图形 */
{
	/* 检测这个区域是否有效 */
	if (rect.x < 0 || rect.y < 0) {
		return -1; 
	}
	if (rect.x + rect.width > Get_Screen_Width () 
	 || rect.y + rect.height > Get_Screen_Height ()) {
		 return -1;
	}
	if (rect.width <= 0 && rect.height <= 0) {
		return -2;
	}
	
	int i, total; 
	
	LCUI_Pos pos, widget_pos;
	LCUI_Widget *widget; 
	LCUI_Queue widget_buff;
	LCUI_Rect valid_area;
	LCUI_Graph buff;

	Graph_Init(&buff);
	Queue_Init(&widget_buff, sizeof(LCUI_Widget), NULL);
	Queue_Using_Pointer(&widget_buff); /* 只用于存放指针 */
	
	graph->have_alpha = FALSE; 
	buff.have_alpha = TRUE;
	/* 根据指定的尺寸，分配内存空间，用于储存图形数据 */
	Graph_Create( graph, rect.width, rect.height );
	/* 获取与该区域重叠的部件，并记录至队列widget_buff中 */
	Get_Overlay_Widget( rect, NULL, &widget_buff ); 
	total = Queue_Get_Total( &widget_buff ); 
	
	if( total > 0 ) {
		//printf("\nrect(%d,%d,%d,%d)\n",
		//rect.x, rect.y, rect.width, rect.height );
		//printf("list cover widget:\n");
		for(i=total-1; i>=0; --i) {
			widget = Queue_Get( &widget_buff, i );
			valid_area = Get_Widget_Valid_Rect( widget ); 
			widget_pos = Get_Widget_Global_Pos( widget );
			valid_area.x += widget_pos.x;
			valid_area.y += widget_pos.y;
			pos = Get_Widget_Global_Pos( widget );
			/* 检测图层属性 */
			switch( Graph_Is_Opaque(&widget->graph) ) {
			    case -1: 
				/* 如果完全不可见，直接从队列中移除 */
				Queue_Delete_Pointer(&widget_buff, i);
				break;
			    case 0: break;
			    case 1:
				/* 如果不透明，并且图层区域包含需刷新的区域 */
				if( Rect_Include_Rect(valid_area, rect) ) { 
					goto skip_loop;
				}
				break;
			    default:break;
			} 
		}
skip_loop:
		total = Queue_Get_Total(&widget_buff);
		//printf("list end, total: %d\n", i);
		//i = -1;
		if(i <= 0){
			i=0;
			Graph_Cut (&LCUI_Sys.screen.buff, rect, graph);
		}
		for(; i<total; ++i) {
			widget = Queue_Get( &widget_buff, i ); 
			pos = Get_Widget_Global_Pos( widget );
			/* 引用部件中的有效区域中的图形 */
			valid_area = Get_Widget_Valid_Rect( widget );
			Quote_Graph( &buff, &widget->graph, valid_area ); 
			/* 获取相对坐标 */
			pos.x = pos.x - rect.x + valid_area.x;
			pos.y = pos.y - rect.y + valid_area.y; 
			Graph_Mix( graph, &buff, pos );
		}
	} else {/* 否则，直接贴背景图 */ 
		Graph_Cut ( &LCUI_Sys.screen.buff, rect, graph );
	}

	if ( LCUI_Sys.cursor.visible ) { /* 如果游标可见 */
		/* 如果该区域与游标的图形区域重叠 */ 
		if ( Rect_Is_Overlay( rect, Get_Cursor_Rect()) ) {
			pos.x = LCUI_Sys.cursor.pos.x - rect.x;
			pos.y = LCUI_Sys.cursor.pos.y - rect.y;
			/* 将图形合成 */ 
			Graph_Mix (graph, &LCUI_Sys.cursor.graph, pos);
		}
	} 
	/* 释放队列占用的内存空间 */
	Destroy_Queue(&widget_buff);
	return 0;
}

static void Handle_Screen_Update()
/* 功能：进行屏幕内容更新 */
{ 
	LCUI_Rect rect;
	LCUI_Graph fill_area, graph;
	
	Graph_Init(&graph);
	Graph_Init(&fill_area);
	/* 锁住队列，其它线程不能访问 */
	Queue_Lock(&LCUI_Sys.update_area); 
	while(LCUI_Active()) {
		/* 如果从队列中获取数据成功 */
		if ( RectQueue_Get(&rect, 0, &LCUI_Sys.update_area) ) { 
			/* 获取内存中对应区域的图形数据 */  
			Get_Screen_Real_Graph (rect, &graph); 
			/* 写入至帧缓冲，让屏幕显示图形 */  
			Graph_Display (&graph, Pos(rect.x, rect.y)); 
			/* 移除队列中的成员 */ 
			Queue_Delete (&LCUI_Sys.update_area, 0); 
		} else {
			break;
		}
	}
	/* 解锁队列 */
	Queue_UnLock(&LCUI_Sys.update_area); 
	Graph_Free(&graph);
}

//#define need_autoquit
#ifdef need_autoquit
static int auto_flag = 0;
static void *autoquit()
/* 在超时后，会自动终止程序，用于调试 */
{
	LCUI_ID time = 0;
	while(time <5000000) {
		if(auto_flag == 0) {
			usleep(10000);
			time += 10000;
		} else {
			auto_flag = 0;
			time = 0;
		}
	}
	exit(-1);
}
#endif

static void *Handle_Area_Update ()
/* 功能：进行屏幕内容更新 */
{
#ifdef need_autoquit
	pthread_t t;
	LCUI_Thread_Create(&t, NULL, autoquit, NULL);
#endif
	while(LCUI_Active()) {
		Handle_All_WidgetUpdate();/* 处理所有部件更新 */ 
		usleep(5000);/* 停顿一段时间，让程序主循环处理任务 */ 
		Handle_Refresh_Area(); /* 处理需要刷新的区域 */ 
		Handle_Screen_Update();/* 处理屏幕更新 */ 
#ifdef need_autoquit
		auto_flag = 1;
#endif
	}
	thread_exit(NULL);
}



extern int Screen_Init();
extern int Screen_Destroy();

int Enable_Graph_Display()
/* 功能：启用图形输出 */
{
	Screen_Init();
	return thread_create( &LCUI_Sys.display_thread, 
			NULL, Handle_Area_Update, NULL );
}

int Disable_Graph_Display()
/* 功能：禁用图形输出 */
{
	Screen_Destroy();
	return thread_join( LCUI_Sys.display_thread, NULL );
}
