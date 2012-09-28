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
#include LC_MISC_H
#include LC_WIDGET_H
#include LC_CURSOR_H
#include LC_DISPLAY_H
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
	if(NULL == event) {
		return -2;
	}
	else if(event->key.code == MOUSE_LEFT_KEY) {
		return event->key.status; 
	}
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
	if(NULL == event) {
		return -2;
	}
	else if(event->key.code == MOUSE_RIGHT_KEY) {
		return event->key.status; 
	}
	return -1;
}

int Click_LeftButton (LCUI_MouseEvent *event)
/* 功能：检测是否是按鼠标左键 */
{
	/* 如果按下的是鼠标左键，并且之前没有按住它 */
	if (Mouse_LeftButton(event) == PRESSED
		&& !Find_Pressed_Key(event->key.code)) {
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
	for(i[0]=0; i[0]<total[0]; ++i[0]) {
		temp = (LCUI_Event*)Queue_Get(&LCUI_Sys.mouse.event, i[0]);
		if(temp->id == event_id) {
			total[1] = Queue_Get_Total(&temp->func_data);
			for(i[1]=0; i[1]<total[1]; ++i[1]) {
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
	if (total > 0) { /* 如果程序总数大于0 */
		for (i = 0; i < total; ++i) {
			key = (int*)Queue_Get(queue, i);
			if(*key == key_code) {
				return i;
			}
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
	if(temp >= 0) {
		return;
	}
	temp = Queue_Add(&LCUI_Sys.press_key, &key_code);
	if(temp >= 0) {/* 如果键值添加成功，就触发click事件 */ 
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
	if(pos < 0) {
		return; 
	}
	if( Queue_Delete(&LCUI_Sys.press_key, pos) ) {
		event->key.code = key_code;
		event->key.status = FREE; 
		Send_Mouse_Event(MOUSE_EVENT_CLICK, event);
	}
}

void Handle_Mouse_Event(int button_type, LCUI_MouseEvent *event)
/* 功能：处理鼠标产生的事件 */
{
	static LCUI_Pos old_pos;/* 保存鼠标之前的坐标 */
	switch (button_type) {
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
	if(Pos_Cmp(event->global_pos, old_pos) != 0) {
		Send_Mouse_Event(MOUSE_EVENT_MOVE, event);/*  触发事件 */ 
		old_pos = event->global_pos;
	}
}

static int disable_mouse = IS_FALSE;

static void * Handle_Mouse_Input ()
/*
 * 功能：处理鼠标的移动，以及按键的点击
 * 说明：本函数会通过读取/dev/input/mice来获得鼠标的相对移动位置，以及按键状态，并
 * 处理相应的鼠标事件。
 */
{ 
	int  temp, button, retval;

	char *msdev, buf[6];

	fd_set readfds;

	struct timeval tv;
	
	LCUI_MouseEvent event;
	LCUI_Pos pos; 
	disable_mouse = IS_FALSE;
	while (LCUI_Active()) { 
		if(disable_mouse == 1) break;
		if (LCUI_Sys.mouse.status == REMOVE) { 
			
			msdev = getenv("LCUI_MOUSE_DEVICE");
			if( msdev == NULL ) msdev = MS_DEV;
			
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
			Handle_Mouse_Event(button, &event); 
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
	if(LCUI_Sys.mouse.status == REMOVE) {
		return pthread_create(&LCUI_Sys.mouse.thread, 
					NULL, Handle_Mouse_Input, NULL); 
	}
	return 0;
}

int Disable_Mouse_Input()
/* 功能：禁用鼠标输入处理 */
{ 
	if(LCUI_Sys.mouse.status == INSIDE) {
		disable_mouse = IS_TRUE;
		/* 等待LCUI子线程结束 */
		return pthread_join (LCUI_Sys.mouse.thread, NULL);	
	}
	return 0;
}
/**************************** Mouse End *******************************/
