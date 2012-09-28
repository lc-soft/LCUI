#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include LC_WIDGET_H
#include LC_CURSOR_H

#include <linux/fb.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/ioctl.h> 
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

extern void count_time();
extern void end_count_time();

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

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color)
/* 功能：填充指定位置的像素点的颜色 */
{
	int k;
	uchar_t *dest;
	
	k = (pos.y * LCUI_Sys.screen.size.w + pos.x) << 2; 
	//这里需要根据不同位的显示器来进行相应处理
	dest = LCUI_Sys.screen.fb_mem;	/* 指向帧缓冲 */
	dest[k] = color.blue;
	dest[k + 1] = color.green;
	dest[k + 2] = color.red; 
}

int Get_Screen_Graph(LCUI_Graph *out)
/* 
 * 功能：获取屏幕上显示的图像
 * 说明：自动分配内存给指针，并把数据拷贝至指针的内存 
 * */
{
	uchar_t  *dest;
	int i, temp, h, w;
	if(LCUI_Sys.init != IS_TRUE) {/* 如果没有初始化过 */
		return -1; 
	}
	
	out->have_alpha = IS_FALSE;/* 无alpha通道 */
	out->type = TYPE_BMP;
	temp = Graph_Create(out, LCUI_Sys.screen.size.w, LCUI_Sys.screen.size.h);
	if(temp != 0) {
		return -2; 
	}
	
	dest = LCUI_Sys.screen.fb_mem; /* 指针指向帧缓冲的内存 */
	for (i=0,h=0; h < LCUI_Sys.screen.size.h; ++h) {
		for (w = 0; w < LCUI_Sys.screen.size.w; ++w) {
			/* 读取帧缓冲的内容 */
			out->rgba[2][i] = *(dest++);
			out->rgba[1][i] = *(dest++);
			out->rgba[0][i] = *(dest++);
			dest++;
			++i; 
		}
	}
	return 0;
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


static void print_screeninfo(
		struct fb_var_screeninfo fb_vinfo,
		struct fb_fix_screeninfo fb_fix
	)
/* 功能：打印屏幕相关的信息 */
{
	char visual[256], type[256];
	
	switch(fb_fix.type) {
	    case FB_TYPE_PACKED_PIXELS:
		strcpy(type, "packed pixels");break;
	    case FB_TYPE_PLANES:
		strcpy(type, "non interleaved planes");break;
	    case FB_TYPE_INTERLEAVED_PLANES:
		strcpy(type, "interleaved planes");break;
	    case FB_TYPE_TEXT:
		strcpy(type, "text/attributes");break;
	    case FB_TYPE_VGA_PLANES:
		strcpy(type, "EGA/VGA planes");break;
	    default: 
		strcpy(type, "unkown");break;
	}
	
	switch(fb_fix.visual) {
	    case FB_VISUAL_MONO01:  
		strcpy(visual, "Monochr. 1=Black 0=White");break;
	    case FB_VISUAL_MONO10:
		strcpy(visual, "Monochr. 1=White 0=Black");break;
	    case FB_VISUAL_TRUECOLOR: 
		strcpy(visual, "true color");break;
	    case FB_VISUAL_PSEUDOCOLOR: 
		strcpy(visual, "pseudo color (like atari)");break;
	    case FB_VISUAL_DIRECTCOLOR:  
		strcpy(visual, "direct color");break;
	    case FB_VISUAL_STATIC_PSEUDOCOLOR:
		strcpy(visual, "pseudo color readonly");break;
	    default: 
		strcpy(type, "unkown");break;
	}
	printf(
		"============== screen info =============\n" 
		"FB mem start  : 0x%08lX\n"
		"FB mem length : %d\n"
		"FB type       : %s\n"
		"FB visual     : %s\n"
		"accel         : %d\n"
		"geometry      : %d %d %d %d %d\n"
		"timings       : %d %d %d %d %d %d\n"
		"rgba          : %d/%d, %d/%d, %d/%d, %d/%d\n"
		"========================================\n",
		fb_fix.smem_start, fb_fix.smem_len,
		type, visual,
		fb_fix.accel,
		fb_vinfo.xres, fb_vinfo.yres, 
		fb_vinfo.xres_virtual, fb_vinfo.yres_virtual,  
		fb_vinfo.bits_per_pixel,
		fb_vinfo.upper_margin, fb_vinfo.lower_margin,
		fb_vinfo.left_margin, fb_vinfo.right_margin, 
		fb_vinfo.hsync_len, fb_vinfo.vsync_len,
		fb_vinfo.red.length, fb_vinfo.red.offset,
		fb_vinfo.green.length, fb_vinfo.green.offset,
		fb_vinfo.blue.length, fb_vinfo.blue.offset,
		fb_vinfo. transp.length, fb_vinfo. transp.offset
	);
}

static int Screen_Init()
/* 功能：初始化屏幕 */
{
	__u16 rr[256],gg[256],bb[256];
	struct fb_var_screeninfo fb_vinfo;
	struct fb_fix_screeninfo fb_fix;
	struct fb_cmap oldcmap = {0,256,rr,gg,bb} ;
	
	char *fb_dev;
	/* 获取环境变量中指定的帧缓冲设备的位置 */
	fb_dev = getenv("LCUI_FB_DEVICE");
	if(fb_dev == NULL) {
		fb_dev = FB_DEV;
	}
	
	nobuff_printf("open video output device..."); 
	LCUI_Sys.screen.fb_dev_fd = open(fb_dev, O_RDWR);
	if (LCUI_Sys.screen.fb_dev_fd== -1) {
		printf("fail\n");
		perror("error");
		exit(-1);
	} else {
		printf("success\n");
	}
	LCUI_Sys.screen.fb_dev_name = fb_dev;
	/* 获取屏幕相关信息 */
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_VSCREENINFO, &fb_vinfo);
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_FSCREENINFO, &fb_fix);
	/* 打印屏幕信息 */
	print_screeninfo(fb_vinfo, fb_fix);
	
	LCUI_Sys.screen.bits = fb_vinfo.bits_per_pixel;
	if (fb_vinfo.bits_per_pixel==8) {
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGETCMAP, &oldcmap); 
	}
	nobuff_printf("mapping framebuffer...");
	LCUI_Sys.screen.smem_len = fb_fix.smem_len;/* 保存内存空间大小 */
	/* 映射帧缓存至内存空间 */
	LCUI_Sys.screen.fb_mem = mmap(NULL,fb_fix.smem_len,
					PROT_READ|PROT_WRITE,MAP_SHARED,
					LCUI_Sys.screen.fb_dev_fd, 0);
							
	if((void *)-1 == LCUI_Sys.screen.fb_mem) { 
		printf("fail\n");
		perror(strerror(errno));
		exit(-1);
	} else {
		printf("success\n");
	}
	
	Graph_Init(&LCUI_Sys.screen.buff); /* 初始化图形数据 */
	
	LCUI_Sys.screen.buff.have_alpha = NO_ALPHA;/* 无alpha通道 */
	LCUI_Sys.screen.buff.type = TYPE_BMP;/* bmp位图 */
	LCUI_Sys.screen.size.w = fb_vinfo.xres; /* 保存屏幕尺寸 */
	LCUI_Sys.screen.size.h = fb_vinfo.yres; 
	LCUI_Sys.screen.buff.width = fb_vinfo.xres; 
	LCUI_Sys.screen.buff.height = fb_vinfo.yres;
	return 0;
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

void Get_Overlay_Widget(LCUI_Rect rect, LCUI_Widget *widget, LCUI_Queue *queue)
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

	if(widget == NULL) 
		widget_list = &LCUI_Sys.widget_list; 
	else {
		if(widget->visible == IS_FALSE) {
			return;
		}
		widget_list = &widget->child; 
	}

	total = Queue_Get_Total(widget_list); 

	for(i=total-1; i>=0; --i) {/* 从底到顶遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(widget_list, i); 
		if(child != NULL && child->visible == IS_TRUE) {
		/* 如果有可见的子部件 */ 
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
	}
}

int Get_Screen_Real_Graph (LCUI_Rect rect, LCUI_Graph * graph)
/* 
 * 功能：获取屏幕中指定区域内实际要显示的图形 
 * 说明：指定的区域必须是与部件区域不部分重叠的
 * */
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
	
	graph->have_alpha = IS_FALSE; 
	buff.have_alpha = IS_TRUE;
	/* 根据指定的尺寸，分配内存空间，用于储存图形数据 */
	Graph_Create(graph, rect.width, rect.height);
	/* 获取与该区域重叠的部件，并记录至队列widget_buff中 */
	Get_Overlay_Widget(rect, NULL, &widget_buff); 
	total = Queue_Get_Total(&widget_buff); 
	if(total > 0) {
		//printf("rect(%d,%d,%d,%d), list cover widget:\n",
		//rect.x, rect.y, rect.width, rect.height
		//);
		//printf("list cover widget:\n");
		for(i=total-1; i>=0; --i) {
			widget = (LCUI_Widget*)Queue_Get(&widget_buff, i);
			valid_area = Get_Widget_Valid_Rect(widget);
			widget_pos = Get_Widget_Global_Pos(widget);
			valid_area.x += widget_pos.x;
			valid_area.y += widget_pos.y;
			//Print_Graph_Info(&widget->graph);
			//print_widget_info(widget);
			/* 检测图层属性 */
			switch(Graph_Is_Opaque(&widget->graph)) {
			    case -1:
				//printf("delete\n");
				/* 如果完全不可见，直接从队列中移除 */
				Queue_Delete_Pointer(&widget_buff, i);
				break;
			    case 0: break;
			    case 1:
				if(Rect_Include_Rect(valid_area, rect)) { 
					/* 如果不透明，并且图层区域包含需刷新的区域 */
					//printf("goto\n");
					goto skip_loop;
				}
				break;
			    default:break;
			} 
		}
skip_loop:
		//nobuff_print("mix graph layer, use time:");
		//count_time();
		total = Queue_Get_Total(&widget_buff);
		//printf("list end, total: %d\n", i);
		//i = -1;
		if(i <= 0){
			i=0;
			Graph_Cut (&LCUI_Sys.screen.buff, rect, graph);
		}
		for(; i<total; ++i) {
			widget = (LCUI_Widget*)Queue_Get(&widget_buff, i); 
			pos = Get_Widget_Global_Pos(widget);
			/* 获取部件中有效显示区域 */
			valid_area = Get_Widget_Valid_Rect(widget);
			/* 引用部件中的有效区域中的图形 */
			Quote_Graph(&buff, &widget->graph, valid_area); 
			//print_widget_info(widget);
			//printf("src size: %d, %d\n", widget->graph.width, widget->graph.height);
			//printf("cut rect: %d, %d, %d, %d\n",
				//valid_area.x, valid_area.y, 
				//valid_area.width, valid_area.height);
			/* 获取相对坐标 */
			pos.x = pos.x - rect.x + valid_area.x;
			pos.y = pos.y - rect.y + valid_area.y;
			//Print_Graph_Info(&buff);
			/* 叠加 */ 
			Graph_Mix(graph, &buff, pos);
		} 
		//end_count_time();
	} else {/* 否则，直接贴背景图 */ 
		Graph_Cut (&LCUI_Sys.screen.buff, rect, graph);
	}

	if (LCUI_Sys.cursor.visible == IS_TRUE) { /* 如果游标可见 */
		/* 如果该区域与游标的图形区域重叠 */ 
		if (Rect_Is_Overlay( rect, Get_Cursor_Rect()) ) {
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
	int i;
	LCUI_Rect rect;
	LCUI_Graph fill_area, graph;
	
	Graph_Init(&graph);
	Graph_Init(&fill_area);
	/* 锁住队列，其它线程不能访问 */
	Queue_Lock(&LCUI_Sys.update_area);
	i = 0;  
	while(LCUI_Active()) {
		/* 如果从队列中获取数据成功 */
		if ( RectQueue_Get(&rect, 0, &LCUI_Sys.update_area) ) { 
			/* 获取内存中对应区域的图形数据 */  
			Get_Screen_Real_Graph (rect, &graph); 
			/* 写入至帧缓冲，让屏幕显示图形 */  
			Graph_Display (&graph, Pos(rect.x, rect.y)); 
			/* 移除队列中的成员 */ 
			Queue_Delete (&LCUI_Sys.update_area, 0);
			++i;
		} else {
			break;
		}
	}
	/* 解锁队列 */
	Queue_UnLock(&LCUI_Sys.update_area); 
	Graph_Free(&graph);
}


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

static void *LCUI_Core ()
/* 功能：进行屏幕内容更新 */
{
#ifdef need_autoquit
	pthread_t t;
	LCUI_Thread_Create(&t, NULL, autoquit, NULL);
#endif
	Screen_Init();
	while(LCUI_Active()) { 
		Handle_All_WidgetUpdate();/* 处理所有部件更新 */ 
		usleep(5000);/* 停顿一段时间，让程序主循环处理任务 */ 
		Handle_Refresh_Area(); /* 处理需要刷新的区域 */ 
		Handle_Screen_Update();/* 处理屏幕更新 */
	}
	pthread_exit(NULL);
}

int Enable_Graph_Display()
/* 功能：启用图形输出 */
{
	LCUI_Sys.status = ACTIVE;
	return pthread_create( &LCUI_Sys.core_thread, NULL, LCUI_Core, NULL );
}

int Disable_Graph_Display()
/* 功能：禁用图形输出 */
{
	LCUI_Sys.status = KILLED;
	return pthread_join( LCUI_Sys.core_thread, NULL );
}
