/* ***************************************************************************
 * LCUI_Main.c -- LCUI's kernel
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
 * LCUI_Main.c -- LCUI 的核心
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
#include LC_GRAPHICS_H
#include LC_RES_H
#include LC_CURSOR_H 
#include LC_INPUT_H
#include LC_MISC_H 
#include LC_MEM_H
#include LC_ERROR_H
#include LC_FONT_H 
#include LC_WIDGET_H

#include <linux/fb.h>
#include <sys/mman.h>
#include <stdio.h>  
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

LCUI_System LCUI_Sys; 

static clock_t start_time;
void count_time()
{
	start_time = clock();
}

void end_count_time()
{
	printf("%ldms\n",clock()-start_time);
}

/***************************** Core ***********************************/
static void Process_Screen_Update()
/* 功能：处理屏幕内容更新 */
{
	int i;
	LCUI_Rect rect;
	LCUI_Graph fill_area, graph;  
	Graph_Init(&graph);
	Graph_Init(&fill_area);
	/* 锁住队列，其它线程不能访问 */
	Queue_Lock(&LCUI_Sys.update_area);
	i = 0;
	//printf("Process_Screen_Update(): start\n"); 
	while(LCUI_Active()) {
		/* 如果从队列中获取数据成功 */
		if ( RectQueue_Get(&rect, 0, &LCUI_Sys.update_area) ) {
			//printf("RectQueue_Get(): %d,%d,%d,%d\n", rect.x, rect.y, rect.width, rect.height);
			/* 获取内存中对应区域的图形数据 */ 
			//printf("Process_Screen_Update(): get\n"); 
			//nobuff_print("[%d]Process_Screen_Update(): ", i);
			//count_time();
			//printf("Get_Screen_Real_Graph(): start\n");
			//Malloc_Graph(&fill_area, rect.width, rect.height);
			//Fill_Color(&fill_area, RGB(255,0,0));
			//Write_Graph_To_FB (&fill_area, Pos(rect.x, rect.y)); 
			Get_Screen_Real_Graph (rect, &graph);
			//printf("Get_Screen_Real_Graph(): end\n");
			//printf("Process_Screen_Update(): end\n");
			//nobuff_print("Write_Graph_To_FB(): ");
			//count_time();
			/* 写入至帧缓冲，让屏幕显示图形 */ 
			//usleep(100000);
			Write_Graph_To_FB (&graph, Pos(rect.x, rect.y)); 
			//end_count_time(); 
			//printf("Process_Screen_Update(): queue delete start\n"); 
			//count_time();
			//printf("queue mode: %d\n", LCUI_Sys.update_area.mode);
			Queue_Delete (&LCUI_Sys.update_area, 0);/* 移除队列中的成员 */
			//end_count_time(); 
			//printf("Process_Screen_Update(): queue delete end\n");
			++i;
		}
		else break;
	}
	/* 解锁队列 */
	Queue_UnLock(&LCUI_Sys.update_area);
	
	//printf("Process_Screen_Update(): end\n"); 
	Free_Graph(&graph);
}

#ifdef need_test
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
	exit(0);
}
#endif

static void *LCUI_Core ()
/* 功能：进行LCUI的核心处理工作 */
{
	//pthread_t t;
	//LCUI_Thread_Create(&t, NULL, autoquit, NULL);
	while(LCUI_Active()) {
		//printf("core start\n");
		//count_time();
		//nobuff_print("Process_All_WidgetUpdate(): ");
		Process_All_WidgetUpdate();/* 处理所有部件更新 */
		//end_count_time();
		usleep(5000);/* 停顿一段时间，让程序处理任务 */
		 
		//nobuff_print("Process_Refresh_Area(): "); 
		//count_time();
		Process_Refresh_Area(); /* 处理需要刷新的区域 */  
		//end_count_time();
		//nobuff_print("Process_Screen_Update(): "); 
		//count_time(); 
		Process_Screen_Update();/* 处理屏幕更新 */  
		//end_count_time();
		//auto_flag = 1;
		//printf("core end\n"); 
	}
	
	pthread_exit(NULL);
}

static int Core_Start()
/* 功能：启用内核 */
{
	LCUI_Sys.status = ACTIVE;	/* 改变LCUI的状态 */
	return pthread_create( &LCUI_Sys.core_thread, NULL, LCUI_Core, NULL );
}

static int Core_End()
/* 功能：禁用内核 */
{
	LCUI_Sys.status = KILLED;	/* 改变LCUI的状态 */
	return pthread_join( LCUI_Sys.core_thread, NULL );
}
/**************************** Core End *********************************/

/************************* App Manage *********************************/
LCUI_App *Find_App(LCUI_ID id)
/* 功能：根据程序的ID，获取指向程序数据结构的指针 */
{
	LCUI_App *app; 
	int i, total;  
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0) { /* 如果程序总数大于0 */
		for (i = 0; i < total; ++i) {
			app = (LCUI_App*)Queue_Get(&LCUI_Sys.app_list, i);
			if(app->id == id)
				return app;
		}
	}
	
	return NULL;
}

LCUI_App* Get_Self_AppPointer()
/* 功能：获取程序的指针 */
{
	Thread_TreeNode *ttn;
	LCUI_ID id;
	
	id = pthread_self(); /* 获取本线程ID */  
	if(id == LCUI_Sys.core_thread
	|| id == LCUI_Sys.key_thread
	|| id == LCUI_Sys.mouse.thread
	|| id == LCUI_Sys.ts.thread)
	{/* 由于内核及其它线程ID没有被记录，只有直接返回LCUI主程序的线程ID了 */
		return Find_App(LCUI_Sys.self_id);
	}
	/* 获取父线程的ID */
	ttn = Search_Thread_Tree(&LCUI_Sys.thread_tree, id);
	/* 
	 * 往父级遍历，直至父级指针为NULL，因为根线程是没
	 * 有父线程结点指针的，程序的线程ID都在根线程里的
	 * 子线程ID队列中 
	 * */
	while(ttn->parent != NULL) { 
		ttn = ttn->parent; 
		if(ttn != NULL && ttn->parent == NULL) 
			break;
	}
	
	return Find_App(ttn->tid);
}


void LCUI_App_Init(LCUI_App *app)
/* 功能：初始化程序数据结构体 */
{
	app->stop_loop = IS_FALSE;
	app->id = 0;
	FuncQueue_Init(&app->task_queue); 
	//EventQueue_Init(&app->key_event);
	WidgetLib_Init(&app->widget_lib);
	app->encoding_type = ENCODEING_TYPE_UTF8;
}


static void LCUI_Quit ()
/*
 * 功能：退出LCUI
 * 说明：在没有任何LCUI程序时，LCUI会调用本函数来恢复运行LCUI前的现场。
 * */
{
	int err = 0;
	LCUI_Sys.status = KILLED;	/* 状态标志置为KILLED */
	Free_LCUI_Font ();		/* 释放LCUI的默认字体数据占用的内存资源 */
	 
	Core_End();/* 等待Core线程退出 */ 
	Destroy_Queue(&LCUI_Sys.key_event);/* 撤销按键事件数据队列 */
	Disable_Mouse_Input();		/* 禁用鼠标输入 */ 
	Disable_TouchScreen_Input();	/* 禁用触屏支持 */ 
	Disable_Key_Input();		/* 禁用按键输入 */ 
	/* 恢复屏幕初始内容 */ 
	Write_Graph_To_FB (&LCUI_Sys.screen.buff, Pos(0, 0));	
	/* 解除帧缓冲在内存中的映射 */
	err = munmap (LCUI_Sys.screen.fb_mem, LCUI_Sys.screen.smem_len);
	if (err != 0) perror ("munmap()");
	close (LCUI_Sys.screen.fb_dev_fd);  
	exit (err);
}


static int LCUI_AppList_Delete (LCUI_ID app_id)
/* 功能：关闭一个LCUI程序 */
{
	int pos = -1;
	LCUI_App *app; 
	int i, total;  
	total = Queue_Get_Total(&LCUI_Sys.app_list);
	if (total > 0) { /* 如果程序总数大于0， 查找程序信息所在队列的位置 */
		for (i = 0; i < total; ++i) {
			app = (LCUI_App*)Queue_Get(&LCUI_Sys.app_list, i);
			if(app->id == app_id) {
				pos = i;
				break;
			}
		}
		if(pos < 0) return -1;
	}
	else return -1;
	/* 从程序显示顺序队列中删除这个程序ID */ 
	Queue_Delete (&LCUI_Sys.app_list, pos); 
	
	if (Queue_Empty(&LCUI_Sys.app_list)) /* 如果程序列表为空 */  
		LCUI_Quit (); /* 退出LCUI */ 
	return 0;
}

static void LCUI_Destroy_App(LCUI_App *app)
/* 功能：销毁程序相关信息 */
{
	if(app == NULL) return;
	
	LCUI_App_Thread_Cancel(app->id); /* 撤销这个程序的所有线程 */
	LCUI_Destroy_App_Widgets(app->id); /* 销毁这个程序的所有部件 */
}


static void LCUI_AppList_Init()
/* 功能：初始化程序数据表 */
{
	Queue_Init(&LCUI_Sys.app_list, sizeof(LCUI_App), LCUI_Destroy_App);
}


static int LCUI_AppList_Add ()
/* 
 * 功能：创建一个LCUI程序
 * 说明：此函数会将程序信息添加至程序列表
 * 返回值：成功则返回程序的ID，失败则返回-1
 **/
{
	LCUI_App app;
	LCUI_ID id = pthread_self();
	/* 初始化程序数据结构体 */
	LCUI_App_Init (&app);
	app.id	= id;	/* 保存ID */ 
	Queue_Add(&LCUI_Sys.app_list, &app);/* 添加至队列 */
	return 0;
}

static int App_Quit()
/* 功能：退出程序 */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if(NULL == app) {
		printf("App_Quit(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	} 
	
	return LCUI_AppList_Delete(app->id); 
}

void Main_Loop_Quit()
/* 功能：让程序退出主循环 */
{ 
	LCUI_App *app = Get_Self_AppPointer();
	if(NULL == app) {
		printf("Main_Loop_Quit(): "APP_ERROR_UNRECORDED_APP);
		return;
	}
	app->stop_loop = IS_TRUE; 
}
/*********************** App Manage End ******************************/

static void Print_LCUI_Copyright_Text()
/* 功能：打印LCUI的信息 */
{
	printf(
	"============| LCUI v0.12.5 |============\n"
	"Copyright (C) 2012 Liu Chao.\n"
	"Licensed under GPLv2.\n"
	"Report bugs to <lc-soft@live.cn>.\n"
	"Project Homepage: www.lcui.org.\n"
	"========================================\n"
	);
}

int Get_Screen_Width ()
/*
 * 功能：获取屏幕宽度
 * 返回值：屏幕的宽度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
{
	if (LCUI_Sys.init == IS_FALSE) return 0; 
	else return LCUI_Sys.screen.size.w; 
}

int Get_Screen_Height ()
/*
 * 功能：获取屏幕高度
 * 返回值：屏幕的高度，单位为像素，必须在使用LCUI_Init()函数后使用，否则无效
 * */
{
	if (LCUI_Sys.init == IS_FALSE) return 0; 
	else return LCUI_Sys.screen.size.h; 
}

LCUI_Size Get_Screen_Size ()
/* 功能：获取屏幕尺寸 */
{
	return LCUI_Sys.screen.size; 
}

void Fill_Pixel(LCUI_Pos pos, LCUI_RGB color)
/* 功能：填充指定位置的像素点的颜色 */
{
	unsigned char *dest;
	int k;
	k = (pos.y * LCUI_Sys.screen.size.w + pos.x) << 2; 

	dest = LCUI_Sys.screen.fb_mem;		/* 指向帧缓冲 */
	dest[k] = color.blue;
	dest[k + 1] = color.green;
	dest[k + 2] = color.red; 
}

void Write_Graph_To_FB (LCUI_Graph * src, LCUI_Pos pos)
/* 
 * 功能：写入图形数据至帧缓存，从而在屏幕上显示图形 
 * 说明：此函数的主要代码，参考自mgaveiw 0.1.4 的mga_vfb.c文件中的
 * write_to_fb函数的源代码.
 * */
{
	int bits;
	unsigned char *dest;
	struct fb_cmap kolor; 
	unsigned int x, y, n, k, count;
	unsigned int temp1, temp2, temp3, i; 
	LCUI_Rect cut_rect;
	LCUI_Graph temp, *pic;

	if (!Valid_Graph (src)) return;
	
	dest = LCUI_Sys.screen.fb_mem;		/* 指向帧缓冲 */
	pic = src; 
	Graph_Init (&temp);
	
	if ( Get_Cut_Area ( 
			Get_Screen_Size(), 
			Rect ( pos.x, pos.y, src->width, src->height ), 
			&cut_rect
		) ) {/* 如果需要裁剪图形 */
		if(!Rect_Valid(cut_rect))
			return;
			
		pos.x += cut_rect.x;
		pos.y += cut_rect.y;
		Cut_Graph (pic, cut_rect, &temp);
		pic = &temp;
	}
	
	Using_Graph (pic, 0);
	/* 获取显示器的位数 */
	bits = Get_Screen_Bits(); 
	switch(bits) {
		case 32:/* 32位，其中RGB各占8位，剩下的8位用于alpha，共4个字节 */ 
		for (n=0,y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 4 * (k + x);/* 计算需填充的像素点的坐标 */
				count = count << 2; 
				/* 由于帧缓冲(FrameBuffer)的颜色排列是BGR，图片数组是RGB，需要改变一下写入顺序 */
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n]; 
			}
		}
		break;
		case 24:/* 24位，RGB各占8位，也就是共3个字节 */ 
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;//count = 3 * (k + x); 
				count = (count << 1) + count;
				dest[count] = pic->rgba[2][n];
				dest[count + 1] = pic->rgba[1][n];
				dest[count + 2] = pic->rgba[0][n];
			}
		}
		break;
		case 16:/* 16位，rgb分别占5位，6位，5位，也就是RGB565 */
		/*
		 * GB565彩色模式, 一个像素占两个字节, 其中:
		 * 低字节的前5位用来表示B(BLUE)
		 * 低字节的后三位+高字节的前三位用来表示G(Green)
		 * 高字节的后5位用来表示R(RED)
		 * */  
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = (k + x) << 1;//count = 2 * (k + x);
				temp1 = pic->rgba[0][n];
				temp2 = pic->rgba[2][n];
				temp3 = pic->rgba[1][n];
				dest[count] = ((temp3 & 0x1c)<<3)+((temp2 & 0xf8)>>3);
				dest[count+1] = ((temp1 & 0xf8))+((temp3 & 0xe0)>>5);
			}
		}
		break;
		case 8: /* 8位，占1个字节 */
		kolor.start = 0;
		kolor.len = 255; 
		kolor.red = calloc(256, sizeof(__u16));
		kolor.green = calloc(256, sizeof(__u16));
		kolor.blue = calloc(256, sizeof(__u16));
		kolor.transp = 0; 
		
		for (i=0;i<256;i++) {
			kolor.red[i]=0;
			kolor.green[i]=0;
			kolor.blue[i]=0;
		}
		
		for (n=0, y = 0; y < pic->height; ++y) {
			k = (pos.y + y) * LCUI_Sys.screen.size.w + pos.x;
			for (x = 0; x < pic->width; ++x, ++n) {
				count = k + x;
				
				temp1 = pic->rgba[0][n]*0.92;
				temp2 = pic->rgba[1][n]*0.92;
				temp3 = pic->rgba[2][n]*0.92; 
				
				i = ((temp1 & 0xc0))
					+((temp2 & 0xf0)>>2)
					+((temp3 & 0xc0)>>6);
						
				kolor.red[i] = temp1*256;
				kolor.green[i] = temp2*256;
				kolor.blue[i] = temp3*256;
				dest[count] = (((temp1 & 0xc0))
						+((temp2 & 0xf0)>>2)
						+((temp3 & 0xc0)>>6)); 
			}
		}
		
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOPUTCMAP, &kolor); 
		free(kolor.red);
		free(kolor.green);
		free(kolor.blue);
		break;
		default: break;
    }
    
	End_Use_Graph (pic);
	Free_Graph (&temp);
}

static void Get_Widget_Real_Graph(LCUI_Widget *widget, LCUI_Rect rect, LCUI_Graph *graph)
/*
 * 功能：获取指定区域中部件的实际图形
 * 说明：获取图形前，需要指定背景图，之后才能正常合成；获取的图形是指定区域中显示的实际图形
 **/
{
	//printf("Get_Widget_Real_Graph(): enter\n");
	//print_widget_info(widget);
	if(widget->visible == IS_FALSE) 
		return;
		
	int x, y, i;
	LCUI_Graph temp;
	LCUI_Widget *child;
	/* 获取部件在屏幕内的坐标 */
	LCUI_Pos pos = Get_Widget_Global_Pos(widget);
	/* 计算部件图形粘贴至背景中的位置 */
	x = pos.x - rect.x;
	y = pos.y - rect.y;
	//printf("global pos: %d, %d\n", pos.x, pos.y);
	//printf("update area: %d, %d, %d, %d\n", rect.x, rect.y, rect.width, rect.height);
	/*
	 * 由于父部件是子部件的容器，子部件的显示范围仅在父部件内，需
	 * 要调整这个矩形的区域，如果子部件的图形超出容器范围，则裁剪子
	 * 部件的图形并粘贴至背景图中。
	 **/
	 if(widget->parent != NULL) {/* 如果有父部件 */ 
		LCUI_Rect cut_rect;
		/* 获取该部件的有效显示区域 */
		cut_rect = Get_Widget_Valid_Rect(widget);
		/* 加上裁剪起点坐标 */
		x += cut_rect.x;
		y += cut_rect.y;
		/* 引用图层中指定区域的图形 */
		Quote_Graph(&temp, &widget->graph, cut_rect);
		/* 合成之 */
		Mix_Graph(&widget->graph, &temp, Pos(x, y));
	} else  /* 先将父部件合成至背景图中 */
		Mix_Graph (graph, &widget->graph, Pos(x, y));
	//printf("widget overlay pos: %d, %d\n", x, y);
	int total;
	LCUI_Rect tmp;
	
	total = Queue_Get_Total(&widget->child); 
	/* 貌似只需要x和y，区域尺寸靠背景图的信息即可得到 */
	for(i=total-1; i>=0; --i) {/* 从底到顶遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		//printf("get child widget: %p, type: %s\n", child, child->type.string);
		if(child != NULL) {
			if(child->visible == IS_TRUE) {/* 如果有可见的子部件 */
				tmp = Get_Widget_Rect(child);
				tmp.x += pos.x;
				tmp.y += pos.y;
				//printf("tmp: %d, %d, %d, %d\n", tmp.x, tmp.y, tmp.width, tmp.height);
				//printf("rect: %d, %d, %d, %d\n", rect.x, rect.y, rect.width, rect.height);
				/* 如果与该区域重叠，将子部件合成至背景图中 */
				if (Rect_Is_Overlay(tmp, rect)) {
					//printf("overlay\n");
					Get_Widget_Real_Graph(child, rect, graph);  /* 递归调用 */
				}
				//else printf("not bverlay\n");
			}
		} else {
			//printf("break\n");
			continue;
		}
	}
	//printf("Get_Widget_Real_Graph(): quit\n");
}

void Catch_Screen_Graph_By_Cache(LCUI_Rect area, LCUI_Graph *out)
/* 
 * 功能：通过内存中的图像数据，捕获将在屏幕上显示的图像
 * 说明：效率较低，因为需要进行合成。
 *  */
{
	int k; 
	LCUI_Widget *widget;
	LCUI_Rect w_rect;
	LCUI_Pos pos;
	/* 检测这个区域是否有效 */
	if (area.x < 0) return; 
	if (area.y < 0) return; 
	if (area.x + area.width > Get_Screen_Width ())
		return;
	if (area.y + area.height > Get_Screen_Height ())
		return;
		
	if (area.width <= 0 || area.height <= 0)
		return;
		
	int total; 
	/* 截取该区域的背景图像 */
	out->have_alpha = NO_ALPHA;
	Cut_Graph (&LCUI_Sys.screen.buff, area, out);
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	
	/* 先从底到上遍历部件，将与该区域重叠的图层的图形进行混合，得到最终的图形 */
	for (k = total - 1; k >= 0; --k){
		widget = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, k);
		w_rect = Get_Widget_Rect(widget);
		
		if (Rect_Is_Overlay(w_rect, area))/* 如果重叠 */
			Get_Widget_Real_Graph(widget, area, out);
	} 
	/* 如果游标可见 */
	if (LCUI_Sys.cursor.visible == IS_TRUE){						
		/* 检查该区域与游标的图形区域重叠 */ 
		if (Rect_Is_Overlay( area, Get_Cursor_Rect() )) { 
			pos = Pos_Sub(Get_Cursor_Pos(), Pos(area.x, area.y));
			/* 将图形合成 */
			Mix_Graph (out, &LCUI_Sys.cursor.graph, pos);
		}
	}

	return;
}

void Catch_Screen_Graph_By_FB (LCUI_Rect area, LCUI_Graph *out)
/* 
 * 功能：直接读取帧缓冲中的图像数据
 * 说明：效率较高，但捕获的图像有可能会有问题。
 * */
{
	LCUI_Rect cut_rect;
	unsigned char *dest;
	dest = LCUI_Sys.screen.fb_mem;		/* 指向帧缓冲 */
	int x, y, n, k, count;
	
	/* 如果需要裁剪图形 */
	if ( Get_Cut_Area ( Get_Screen_Size(), area,&cut_rect ) ){
		if(!Rect_Valid(cut_rect))
			return;
			
		area.x += cut_rect.x;
		area.y += cut_rect.y;
		area.width = cut_rect.width;
		area.height = cut_rect.height;
	}
	
	Malloc_Graph(out, area.width, area.height);
	Using_Graph (out, 1); 
	n = 0;
	for (y = 0; y < area.height; ++y)
	{
		k = (area.y + y) * LCUI_Sys.screen.size.w + area.x;
		for (x = 0; x < area.width; ++x)
		{
			count = k + x;
			count = count << 2;  
			out->rgba[2][n] = dest[count];
			out->rgba[1][n] = dest[count + 1];
			out->rgba[0][n] = dest[count + 2];
			++n;
		}
	}
	End_Use_Graph (out);
}


int Add_Screen_Refresh_Area (LCUI_Rect rect)
/* 功能：在整个屏幕内添加需要刷新的区域 */
{
	if (rect.width <= 0 || rect.height <= 0)
		return -1; 
		
	rect = Get_Valid_Area(Get_Screen_Size(), rect);
	
	return RectQueue_Add (&LCUI_Sys.update_area, rect);
}


#define __MAX_TEXT_LEN 1024
void nobuff_print(const char *fmt, ...)
/* 功能：打印正执行的操作的信息 */
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap); 
	printf(" ");
	fflush(stdout);	/* 清空输出缓冲区 */
}


static void Mouse_Init(void)
/* 功能：初始化鼠标数据 */
{
	LCUI_Sys.mouse.fd = 0;  
	LCUI_Sys.mouse.status = REMOVE; /* 鼠标为移除状态 */
	LCUI_Sys.mouse.move_speed = 1; /* 移动数度为1 */
	EventQueue_Init(&LCUI_Sys.mouse.event);/* 初始化鼠标事件信息队列 */
}


static void Cursor_Init()
/* 功能：初始化游标数据 */
{
	LCUI_Graph pic;
	Graph_Init(&pic);
	Load_Graph_Default_Cursor(&pic);/* 载入自带的游标的图形数据 */ 
	Set_Cursors_Graph(&pic); 
}
 

static void LCUI_IO_Init()
/* 功能：初始化输入输出功能 */
{
	int result;

	/* 检测是否支持鼠标 */
	nobuff_print("checking mouse support...");
	result = Check_Mouse_Support();
	if(result == 0) {
		printf("yes\n");
		/* 启用鼠标输入处理 */
		nobuff_print("enable mouse input..."); 
		result = Enable_Mouse_Input();
		if(result == 0) printf("success\n");
		else printf("fail\n");
	}
	else printf("no\n");
	
	LCUI_Sys.ts.status = REMOVE;
	LCUI_Sys.ts.thread = 0;
	LCUI_Sys.ts.td = NULL; 
	/* 启用触屏输入处理 */
	printf("enable touchscreen input processing\n"); 
	Enable_TouchScreen_Input();  
	
	Enable_Key_Input();
}

static void print_screeninfo(
		struct fb_var_screeninfo fb_vinfo,
		struct fb_fix_screeninfo fb_fix
)
/* 功能：打印屏幕相关的信息 */
{
	char visual[256], type[256];
	
	switch(fb_fix.type) {
		case FB_TYPE_PACKED_PIXELS:	strcpy(type, "packed pixels");break;
		case FB_TYPE_PLANES:		strcpy(type, "non interleaved planes");break;
		case FB_TYPE_INTERLEAVED_PLANES:strcpy(type, "interleaved planes");break;
		case FB_TYPE_TEXT:		strcpy(type, "text/attributes");break;
		case FB_TYPE_VGA_PLANES:	strcpy(type, "EGA/VGA planes");break;
		default: strcpy(type, "unkown");break;
	}
	
	switch(fb_fix.visual) {
		case FB_VISUAL_MONO01:  strcpy(visual, "Monochr. 1=Black 0=White");break;
		case FB_VISUAL_MONO10:  strcpy(visual, "Monochr. 1=White 0=Black");break;
		case FB_VISUAL_TRUECOLOR:  strcpy(visual, "true color");break;
		case FB_VISUAL_PSEUDOCOLOR:  strcpy(visual, "pseudo color (like atari)");break;
		case FB_VISUAL_DIRECTCOLOR:  strcpy(visual, "direct color");break;
		case FB_VISUAL_STATIC_PSEUDOCOLOR:  strcpy(visual, "pseudo color readonly");break;
		default: strcpy(type, "unkown");break;
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
	if(fb_dev == NULL) fb_dev = FB_DEV;
		
	nobuff_print("open video output device..."); 
	LCUI_Sys.screen.fb_dev_fd = open(fb_dev, O_RDWR);
	if (LCUI_Sys.screen.fb_dev_fd== -1) {
		printf("fail\n");
		perror("error");
		exit(-1);
	}
	else printf("success\n");
	LCUI_Sys.screen.fb_dev_name = fb_dev;
	/* 获取屏幕相关信息 */
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_VSCREENINFO, &fb_vinfo);
	ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGET_FSCREENINFO, &fb_fix);
	/* 打印屏幕信息 */
	print_screeninfo(fb_vinfo, fb_fix);
	
	LCUI_Sys.screen.bits = fb_vinfo.bits_per_pixel;
	if (fb_vinfo.bits_per_pixel==8) 
		ioctl(LCUI_Sys.screen.fb_dev_fd, FBIOGETCMAP, &oldcmap); 
	
	nobuff_print("mapping framebuffer...");
	LCUI_Sys.screen.smem_len = fb_fix.smem_len;/* 保存内存空间大小 */
	/* 映射帧缓存至内存空间 */
	LCUI_Sys.screen.fb_mem = mmap(NULL,fb_fix.smem_len,
					PROT_READ|PROT_WRITE,MAP_SHARED,
					LCUI_Sys.screen.fb_dev_fd, 0);
							
	if((void *)-1 == LCUI_Sys.screen.fb_mem) { 
		printf("fail\n");
		perror(strerror(errno));
		exit(-1);
	} else printf("success\n");
	
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

int LCUI_Active()
/* 功能：检测LCUI是否活动 */
{
	if(LCUI_Sys.status == ACTIVE)
		return 1;
	
	return 0;
}
//extern int debug_mark;
int LCUI_Init(int argc, char *argv[])
/* 
 * 功能：用于对LCUI进行初始化操作 
 * 说明：每个使用LCUI实现图形界面的程序，都需要先调用此函数进行LCUI的初始化
 * */
{
	int temp;
	if(LCUI_Sys.init != IS_TRUE) {/* 如果LCUI没有初始化过 */ 
		srand(time(NULL));/* 生成随机数需要用到，只调用一次即可 */
		LCUI_Sys.init = IS_TRUE;
		Print_LCUI_Copyright_Text();
		
		Thread_TreeNode_Init (&LCUI_Sys.thread_tree);	/* 初始化根线程结点 */
		LCUI_Sys.thread_tree.tid = pthread_self();	/* 当前线程ID作为根结点 */
		LCUI_Sys.self_id = pthread_self();		/* 保存线程ID */
		/* 设定最大空闲时间 */
		LCUI_Sys.max_app_idle_time = MAX_APP_IDLE_TIME;
		LCUI_Sys.max_lcui_idle_time = MAX_LCUI_IDLE_TIME;
		
		EventQueue_Init(&LCUI_Sys.key_event);	/* 初始化按键事件队列 */
		LCUI_Font_Init (&LCUI_Sys.default_font);/* 初始化默认的字体数据 */
		LCUI_AppList_Init (&LCUI_Sys.app_list); /* 初始化LCUI程序数据 */
		RectQueue_Init (&LCUI_Sys.update_area);	/* 初始化屏幕区域更新队列 */ 
		WidgetQueue_Init (&LCUI_Sys.widget_list); /* 初始化部件队列 */
		/* 初始化用于储存已按下的键的键值队列 */
		Queue_Init(&LCUI_Sys.press_key, sizeof(int), NULL);
		
		/* 记录程序信息 */
		temp = LCUI_AppList_Add();
		if(temp != 0) exit(-1);
		
		Core_Start();	/* LCUI的核心开始工作 */ 
		Screen_Init();	/* 初始化屏幕图形输出功能 */
		Mouse_Init();	/* 初始化鼠标 */
		Cursor_Init();	/* 初始化鼠标游标 */
		LCUI_IO_Init();	/* 初始化输入输出设备 */ 
		Widget_Event_Init(); /* 初始化部件事件处理 */
		/* 保存当前屏幕内容，以便退出LCUI后还原 */
		Get_Screen_Graph(&LCUI_Sys.screen.buff); 
		//debug_mark = 1;
		/* 鼠标游标居中 */
		Set_Cursor_Pos(Get_Screen_Center_Point());  
		Show_Cursor();	/* 显示鼠标游标 */ 
	} else {
		temp = LCUI_AppList_Add();
		if(temp != 0)  exit(-1);
	}
	/* 注册默认部件类型 */
	Register_Default_Widget_Type();
	return 0;
}

void *catch()
/* 在截取动画时，会用这个函数捕获屏幕内容 */
{ 
	LCUI_Graph graph;
	int i=0, tsec=0;
	time_t rawtime;
	struct tm * timeinfo;
	char filename[100];
	Graph_Init(&graph);
	while(1) {
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		if(tsec != timeinfo->tm_sec)
			i=0;
			
		sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d-%02d.png",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, timeinfo->tm_hour, 
			timeinfo->tm_min, timeinfo->tm_sec, i++
		);
		tsec = timeinfo->tm_sec;
		
		Catch_Screen_Graph_By_FB(Rect((Get_Screen_Width()-320)/2.0, 
				(Get_Screen_Height()-240)/2.0, 320, 240), &graph);
		write_png_file(filename, &graph);
		usleep(35000);
	}
}

int Need_Main_Loop(LCUI_App *app)
/* 功能：检测主循环是否需要继续进行 */
{
	if(app->stop_loop == IS_TRUE)
		return 0;
	return 1;
}

int LCUI_Main ()
/* 
 * 功能：LCUI程序的主循环
 * 说明：每个LCUI程序都需要调用它，此函数会让程序执行LCUI分配的任务
 *  */
{
	LCUI_App *app;
	LCUI_ID idle_time = 1500;
	LCUI_Graph graph;
	Graph_Init(&graph);
	//创建线程，用于截图。
	//pthread_t t;
	//LCUI_Thread_Create(&t, NULL, catch, NULL);
	
	app = Get_Self_AppPointer();
	if(app == NULL) {
		printf("LCUI_Main(): "APP_ERROR_UNRECORDED_APP);
		return -1;
	}
	
	while (Need_Main_Loop(app)) {/* 循环条件是程序不需要关闭 */ 
	
		if(Empty_Widget()) /* 没有部件，就不需要循环 */
			break;
		/* 如果有需要执行的任务 */
		if(Have_Task(app)) {
			idle_time = 1500;
			Run_Task(app); 
		} else {/* 否则暂停一段时间 */
			usleep (idle_time);
			idle_time += 1500;	/* 每次循环的空闲时间越来越长 */
			if (idle_time >= LCUI_Sys.max_app_idle_time)
				idle_time = LCUI_Sys.max_app_idle_time;
		}
	} 
	return App_Quit ();	/* 直接关闭程序，并释放资源 */ 
}

int Get_LCUI_Version(char *out)
/* 功能：获取LCUI的版本 */
{
	return sprintf(out, "%s", LCUI_VERSION);
}
