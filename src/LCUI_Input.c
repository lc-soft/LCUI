/* ***************************************************************************
 * LCUI_Input.c -- Processing  input device's data
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
 * LCUI_Input.c -- 处理输入设备的数据
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
#include LC_QUEUE_H 
#include LC_THREAD_H
#include LC_MISC_H
#include LC_WIDGET_H
#include LC_CURSOR_H
#include LC_INPUT_H 

#include <linux/unistd.h>
#include <errno.h>
#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

/****************************** Mouse *********************************/
int Mouse_LeftButton(LCUI_MouseEvent *event)
/*
 * 功能：检测鼠标事件中鼠标左键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标左键的键值
 *   0   鼠标左键已经释放
 *   1   鼠标左键处于按下状态
 **/
{
	if(NULL == event) 
		return -2;
	else if(event->key.code == MOUSE_LEFT_KEY) 
		return event->key.status; 
	
	return -1;
}

int Mouse_RightButton(LCUI_MouseEvent *event)
/*
 * 功能：检测鼠标事件中鼠标右键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标右键的键值
 *   0   鼠标右键已经释放
 *   1   鼠标右键处于按下状态
 **/
{
	if(NULL == event) return -2;
	else if(event->key.code == MOUSE_RIGHT_KEY) 
		return event->key.status; 
	
	return -1;
}

int Click_LeftButton (LCUI_MouseEvent *event)
/*
 * 功能：检测是否是按鼠标左键
 **/
{
	if (Mouse_LeftButton(event) == PRESSED
		&& !Find_Pressed_Key(event->key.code))
	{/* 如果按下的是鼠标左键，并且 */
		return 1;
	}
	return 0;
}

void Send_Mouse_Event(int event_id, LCUI_MouseEvent *event)
/* 功能：向已关联鼠标事件的程序发送任务 */
{
	int i[2], total[2];
	LCUI_Event *temp;
	LCUI_Func *func;
	total[0] = Queue_Get_Total(&LCUI_Sys.mouse.event);
	for(i[0]=0; i[0]<total[0]; ++i[0])
	{
		temp = (LCUI_Event*)Queue_Get(&LCUI_Sys.mouse.event, i[0]);
		if(temp->id == event_id)
		{
			total[1] = Queue_Get_Total(&temp->func_data);
			for(i[1]=0; i[1]<total[1]; ++i[1])
			{
				func = (LCUI_Func*)Queue_Get(&temp->func_data, i[1]);
				/* 
				 * 由于Mouse_Input函数只会在LCUI退出时才退出，因此，储存事件的
				 * 变量的生存周期很长，直接用指向它的指针event即可，无需申请内存
				 * 空间，保存副本。
				 * */
				func->arg[0] = event;
				func->arg[2] = NULL;
				Send_Task_To_App(func);
			}
		}
	}
}

int KeyQueue_Find(LCUI_Queue *queue, int key_code)
/* 功能:在按键键值队列里查找指定键值的位置 */
{
	int *key; 
	int i, total;  
	total = Queue_Get_Total(queue);
	if (total > 0)
	{ /* 如果程序总数大于0 */
		for (i = 0; i < total; ++i)
		{
			key = (int*)Queue_Get(queue, i);
			if(*key == key_code)
				return i;
		}
	}
	
	return -1;
} 

static void Press_MouseKey(LCUI_MouseEvent *event, int key_code)
/* 功能：记录被按下的指定键的键值，并发布相应事件 */
{
	int temp; 
	/* 若该键已经按下，就不需要再添加至队列了 */
	temp = KeyQueue_Find(&LCUI_Sys.press_key, key_code);
	if(temp >= 0) 
		return;
		
	temp = Queue_Add(&LCUI_Sys.press_key, &key_code);
	if(temp >= 0)
	{/* 如果键值添加成功，就触发click事件 */ 
		event->key.code = key_code; /* 保存键值 */
		event->key.status = PRESSED;/* 保存状态 */
		Send_Mouse_Event(MOUSE_EVENT_CLICK, event); 
	}
}

static void Free_MouseKey(LCUI_MouseEvent *event, int key_code)
/* 功能：记录被释放的指定键的键值，并发布相应事件 */
{
	int pos; 
	pos = KeyQueue_Find(&LCUI_Sys.press_key, key_code);
	if(pos < 0)  
		return; 
	if( Queue_Delete(&LCUI_Sys.press_key, pos) )
	{
		event->key.code = key_code;
		event->key.status = FREE; 
		Send_Mouse_Event(MOUSE_EVENT_CLICK, event);
	}
}

void Process_Mouse_Event(int button_type, LCUI_MouseEvent *event)
/* 功能：处理鼠标产生的事件 */
{
	static LCUI_Pos old_pos;/* 保存鼠标之前的坐标 */
	switch (button_type)
	{
	case 1:			/* 鼠标左键被按下 */
		Press_MouseKey(event, MOUSE_LEFT_KEY); 
		Free_MouseKey(event, MOUSE_RIGHT_KEY);
		break;
	case 2:			/* 鼠标右键被按下 */
		Press_MouseKey(event, MOUSE_RIGHT_KEY); 
		Free_MouseKey(event, MOUSE_LEFT_KEY); 
		break;
	case 3:			/* 鼠标左右键被按下 */
		Press_MouseKey(event, MOUSE_RIGHT_KEY); 
		Press_MouseKey(event, MOUSE_LEFT_KEY); 
		break;
	default:			/* 默认是释放的 */
		Free_MouseKey(event, MOUSE_RIGHT_KEY); 
		Free_MouseKey(event, MOUSE_LEFT_KEY); 
		break;
	}
	
	/* 如果鼠标位置有改变 */
	if(Pos_Cmp(event->global_pos, old_pos) != 0)
	{
		Send_Mouse_Event(MOUSE_EVENT_MOVE, event);/*  触发事件 */ 
		old_pos = event->global_pos;
	}
}

static int disable_mouse = IS_FALSE;

static void * Process_Mouse_Input ()
/*
 * 功能：处理鼠标的移动，以及按键的点击
 * 说明：本函数会通过读取/dev/input/mice来获得鼠标的相对移动位置，以及按键状态，并
 * 处理相应的鼠标事件。
 */
{ 
	int  temp, button, retval;

	char buf[6];

	fd_set readfds;

	struct timeval tv;
	
	LCUI_MouseEvent event;
	LCUI_Pos pos; 
	disable_mouse = IS_FALSE;
	while (LCUI_Active()) { 
		if(disable_mouse == 1) break;
		if (LCUI_Sys.mouse.status == REMOVE) { 
			if ((LCUI_Sys.mouse.fd =
				 open (MS_DEV, O_RDONLY)) < 0) {
				//printf("Failed to open \"/dev/input/mice\".\n");
				LCUI_Sys.mouse.status = REMOVE;
				usleep (100000);
				//continue;
			} else {
				LCUI_Sys.mouse.status = INSIDE;
				//printf("open \"/dev/input/mice\" successfuly.\n");
			}
		} 
		/* 设定select等待I/o的最长时间 */
		tv.tv_sec = 0;
		tv.tv_usec = 200000;

		FD_ZERO (&readfds);
		FD_SET (LCUI_Sys.mouse.fd, &readfds);

		retval = select (LCUI_Sys.mouse.fd + 1, &readfds, NULL, NULL, &tv);
		if (retval == 0) {
			//printf("Time out!\n");
			if(disable_mouse == IS_TRUE) 
				break;
		}
		if (FD_ISSET (LCUI_Sys.mouse.fd, &readfds)) {
			temp = read (LCUI_Sys.mouse.fd, buf, 6);
			//终端设备，一次只能读取一行
			if (temp <= 0){
				if (temp < 0)
					LCUI_Sys.mouse.status = REMOVE;
				continue;
			}
  
			pos = Get_Cursor_Pos();
			pos.x += (buf[1] * LCUI_Sys.mouse.move_speed); 
			pos.y -= (buf[2] * LCUI_Sys.mouse.move_speed);

			if (pos.x > Get_Screen_Width ())
				pos.x = Get_Screen_Width ();
			if (pos.y > Get_Screen_Height ())
				pos.y = Get_Screen_Height ();
			if (pos.x < 0) pos.x = 0;
			if (pos.y < 0) pos.y = 0;
			/* 设定游标位置 */ 
			Set_Cursor_Pos (pos);
			
			button = (buf[0] & 0x07);
			//printf("x:%d, y:%d, button:%d\n", pos.x, pos.y, button);
			event.global_pos = pos; 
			
			/* 获取当前鼠标指针覆盖到的部件的指针 */
			event.widget = Get_Cursor_Overlay_Widget();
			/* 如果有覆盖到的部件，就需要计算鼠标指针与部件的相对坐标 */
			if(event.widget != NULL) {
				event.pos.x = pos.x - event.widget->pos.x;
				event.pos.y = pos.y - event.widget->pos.y;
			} else {/* 否则，和全局坐标一样 */
				event.pos = pos;
			}
			/* 处理鼠标事件 */
			Process_Mouse_Event(button, &event); 
		}
	}
	disable_mouse = IS_FALSE;
	Hide_Cursor();
	if (LCUI_Sys.mouse.status == INSIDE)
		close (LCUI_Sys.mouse.fd);
		
	LCUI_Sys.mouse.status = REMOVE; 
	pthread_exit (NULL);
}

int Check_Mouse_Support()
/* 功能：检测鼠标的支持 */
{
	/* 只是测试是否能打开这个设备 */
	int fd;
	if ((fd = open (MS_DEV, O_RDONLY)) < 0)
		 return -1;
		 
	return 0;
}

int Enable_Mouse_Input()
/* 功能：启用鼠标输入处理 */
{
	/* 创建一个线程，用于刷显示鼠标指针 */
	if(LCUI_Sys.mouse.status == REMOVE) 
		return pthread_create(&LCUI_Sys.mouse.thread, 
					NULL, Process_Mouse_Input, NULL); 
	return 0;
}

int Disable_Mouse_Input()
/* 功能：禁用鼠标输入处理 */
{ 
	if(LCUI_Sys.mouse.status == INSIDE) {
		disable_mouse = IS_TRUE;
		return pthread_join (LCUI_Sys.mouse.thread, NULL);	/* 等待LCUI子线程结束 */
	}
	return 0;
}
/**************************** Mouse End *******************************/


/***************************** Key *************************************/
static struct termios tm;//, tm_old; 
static int fd = STDIN_FILENO;
int Set_Raw(int t)
{
	if (t > 0) {
		if(tcgetattr(fd, &tm) < 0) 
		return -1; 
		//tm_old = tm; 
		//27906, 5, 1215, 35387
		/**
		 * printf("c_iflag: %d, c_oflag: %d, c_cflag: %d, c_lflag: %d\n"
		 * "c_cc[VMIN]: %d, c_cc[VTIME]: %d\n",
		 * tm.c_iflag, tm.c_oflag, tm.c_cflag, tm.c_lflag, tm.c_cc[VMIN], tm.c_cc[VTIME]);
		 * */
		tm.c_lflag &= ~(ICANON|ECHO);
		tm.c_cc[VMIN] = 1;
		tm.c_cc[VTIME] = 0;
		if(tcsetattr(fd, TCSANOW, &tm) < 0) 
			return -1; 
		printf("\033[?25l");/* 隐藏光标 */
	} else {
		/* 以下值是通过获取正常终端属性得到的 */
		tm.c_iflag = 11522;
		tm.c_oflag = 5;
		tm.c_cflag = 1215;
		tm.c_lflag = 35387;
		tm.c_cc[VMIN] = 1;
		tm.c_cc[VTIME] = 0;
		if(tcsetattr(fd,TCSANOW,&tm)<0) 
			return -1;
		printf("\033[?25h"); /* 显示光标 */ 
	}
	return 0;
}

int Check_Key(void)  
/* 
 * 功能：检测是否有按键输入 
 * 返回值：
 *   1   有按键输入
 *   2   无按键输入
 * */
{
	struct termios oldt, newt;  
	int ch;  
	int oldf;  
	tcgetattr(STDIN_FILENO, &oldt);  
	newt = oldt;  
	newt.c_lflag &= ~(ICANON | ECHO);  
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);  
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  
	ch = getchar();  /* 获取一个字符 */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  
	fcntl(STDIN_FILENO, F_SETFL, oldf);  
	if(ch != EOF)  
	{/* 如果字符有效，将字符放回输入缓冲区中 */
		ungetc(ch, stdin);  
		return 1;  
	} 
	return 0;
}


int Get_Key(void)
/* 功能：获取被按下的按键的键值 */
{ 
	int k,c;
	static int input,count = 0;
	++count;
	k = fgetc(stdin); /* 获取输入缓冲中的字符 */
	input += k; 
	if(Check_Key()) /* 如果还有字符在缓冲中 */
		Get_Key(); /* 递归调用 */
	c = input;
	--count;
	if(count == 0) /* 递归结束后就置0 */
		input = 0;
	if(c == 3) exit(1);
	return c; 
}

int Find_Pressed_Key(int key)
/*
 * 功能：检测指定键值的按键是否处于按下状态
 * 返回值：
 *   1 存在
 *   0 不存在
 **/
{
	int t;
	int i, total;
	
	total = Queue_Get_Total(&LCUI_Sys.press_key);
	for(i=0; i<total; ++i) {
		t = *(int*)Queue_Get(&LCUI_Sys.press_key, i);
		if(t == key)
			return 1;
	}
	return 0;
}

int debug_mark = 0;
static int disable_key = IS_FALSE;
extern int LCUI_Active();
static void * Process_Key_Input ()
/* 功能：处理按键输入 */
{
	int key; 
	int sleep_time = 1500;
	//LCUI_Graph graph;
	//Graph_Init(&graph);
	while (LCUI_Active()) {
		if (Check_Key ()) {/* 如果有按键输入 */ 
			sleep_time = 1500;
			key = Get_Key ();
			if(key == 'd') debug_mark = 1;
			else  debug_mark = 0;
			//#define __NEED_CATCHSCREEN__
			#ifdef __NEED_CATCHSCREEN__
			if(key == 'c')
			{//当按下c键后，可以进行截图，只截取指定区域的图形
				time_t rawtime;
				struct tm * timeinfo;
				char filename[100];
				time ( &rawtime );
				timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */ 
				sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d.png",
					timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
					timeinfo->tm_mday, timeinfo->tm_hour, 
					timeinfo->tm_min, timeinfo->tm_sec
				);
				Catch_Screen_Graph_By_FB(Rect((Get_Screen_Width()-320)/2, 
						(Get_Screen_Height()-240)/2, 320, 240), &graph);
				write_png_file(filename, &graph);
			}
			#endif
			/* 处理程序中关联的按键事件 */
			Process_Event(&LCUI_Sys.key_event, key);
		}
		else if(disable_key == IS_TRUE) break;
		else {/* 停顿一段时间 */
			usleep(sleep_time);
			sleep_time+=1500;
			if(sleep_time >= 30000)
				sleep_time = 30000;
		}
	}
	//Free_Graph(&graph);
	disable_key = IS_FALSE; 
	pthread_exit (NULL);
}

int Enable_Key_Input()
/* 功能：启用按键输入处理 */
{
	if(disable_key == IS_FALSE) {
		Set_Raw(1);/* 设置终端属性 */ 
		/* 创建一个线程，用于处理按键输入 */
		return pthread_create (&LCUI_Sys.key_thread, 
				NULL, Process_Key_Input, NULL);
	}
	return 0;
}

int Disable_Key_Input()
/* 功能：撤销按键输入处理 */
{
	if(disable_key == IS_FALSE) {
		disable_key = IS_TRUE;
		Set_Raw(0);/* 恢复终端属性 */
		return pthread_join (LCUI_Sys.key_thread, NULL);/* 等待线程结束 */
	}
	return 0;
}
/*************************** Key End **********************************/


/************************* TouchScreen *********************************/ 
static void * Process_TouchScreen_Input ()
/* 功能：处理触屏输入 */
{
	char *tsdevice;
	struct ts_sample samp;
	int button, x, y, ret;
	LCUI_MouseEvent event;
	
	char str[100];
	while (LCUI_Active()) {
		if (LCUI_Sys.ts.status != INSIDE) {
			tsdevice = getenv("TSLIB_TSDEVICE");
			if( tsdevice != NULL ) {
				LCUI_Sys.ts.td = ts_open(tsdevice, 0);
			}
			else tsdevice = TS_DEV;
			LCUI_Sys.ts.td = ts_open (tsdevice, 0);
			if (!LCUI_Sys.ts.td) { 
				sprintf (str, "ts_open: %s", tsdevice);
				perror (str);
				LCUI_Sys.ts.status = REMOVE;
				break;
			}

			if (ts_config (LCUI_Sys.ts.td))
			{
				perror ("ts_config");
				LCUI_Sys.ts.status = REMOVE;
				break;
			}
			LCUI_Sys.ts.status = INSIDE;
		}

		/* 开始获取触屏点击处的坐标 */ 
		ret = ts_read (LCUI_Sys.ts.td, &samp, 1); 
		if (ret < 0) {
			perror ("ts_read");
			continue;
		}

		if (ret != 1) continue;

		x = samp.x;
		y = samp.y;
		
		if (x > Get_Screen_Width ())
			x = Get_Screen_Width ();
		if (y > Get_Screen_Height ())
			y = Get_Screen_Height ();
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		/* 设定游标位置 */ 
		Set_Cursor_Pos (Pos(x, y));
		
		event.global_pos.x = x;
		event.global_pos.y = y;
		/* 获取当前鼠标指针覆盖到的部件的指针 */
		event.widget = Get_Cursor_Overlay_Widget();
		/* 如果有覆盖到的部件，就需要计算鼠标指针与部件的相对坐标 */
		if(event.widget != NULL) {
			event.pos.x = x - Get_Widget_Global_Pos(event.widget).x;
			event.pos.y = y - Get_Widget_Global_Pos(event.widget).y;
		} else {/* 否则，和全局坐标一样 */
			event.pos.x = x;
			event.pos.y = y;
		}
		if (samp.pressure > 0)  button = 1; 
		else  button = 0; 
			/* 处理鼠标事件 */
		Process_Mouse_Event(button, &event); 
		//printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
	}
	if(LCUI_Sys.ts.status == INSIDE)
		ts_close(LCUI_Sys.ts.td); 
	LCUI_Sys.ts.status = REMOVE;
	pthread_exit (NULL);
}

int Enable_TouchScreen_Input()
/* 功能：启用鼠标输入处理 */
{
	/* 创建一个线程，用于刷显示鼠标指针 */
	if(LCUI_Sys.ts.status == REMOVE)
		return  pthread_create ( &LCUI_Sys.ts.thread, NULL, 
					Process_TouchScreen_Input, NULL ); 
	return 0;
}

int Disable_TouchScreen_Input()
/* 功能：撤销鼠标输入处理 */
{
	if(LCUI_Sys.ts.status == INSIDE) 
		return pthread_cancel ( LCUI_Sys.ts.thread );/* 撤销LCUI子线程 */ 
	return 0;
}

/*********************** TouchScreen End *******************************/
