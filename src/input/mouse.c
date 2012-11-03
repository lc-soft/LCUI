/* ***************************************************************************
 * mouse.c -- mouse support
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
 * mouse.c -- 鼠标支持
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
 
#include <errno.h>
#include <unistd.h> 
#include <fcntl.h> 

/********************** 鼠标相关信息 ***************************/
typedef struct _LCUI_Mouse
{
	int fd, status;		 /* 句柄，状态 */
	float move_speed;	 /* 鼠标移动速度，1.0为正常速度 */
}
LCUI_Mouse;
/*************************************************************/

static LCUI_Mouse mouse_data;
static LCUI_MouseEvent mouse_event;

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
	total[0] = Queue_Get_Total( &LCUI_Sys.mouse_event );
	for(i[0]=0; i[0]<total[0]; ++i[0]) {
		temp = Queue_Get( &LCUI_Sys.mouse_event , i[0]);
		if( !temp || temp->id != event_id) {
			continue;
		}
		total[1] = Queue_Get_Total(&temp->func_data);
		for(i[1]=0; i[1]<total[1]; ++i[1]) {
			func = Queue_Get(&temp->func_data, i[1]);
			/* 
			 * 由于Mouse_Input函数只会在LCUI退出时才退出，因此，储存事件的
			 * 变量的生存周期很长，直接用指向它的指针event即可，无需申请内存
			 * 空间，保存副本。
			 * */
			func->arg[0] = event;
			func->arg[1] = NULL;
			Send_Task_To_App( func );
		}
	}
}

int KeyQueue_Find(LCUI_Queue *queue, int key_code)
/* 功能:在按键键值队列里查找指定键值的位置 */
{
	int *key; 
	int i, total;  
	total = Queue_Get_Total(queue);
	if( total <= 0 ) {
		return -1;
	}
	for (i = 0; i < total; ++i) {
		key = (int*)Queue_Get(queue, i);
		if(*key == key_code) {
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
	if(temp >= 0) {
		return;
	}
	temp = Queue_Add(&LCUI_Sys.press_key, &key_code);
	if( temp < 0 ) {
		return;
	}
	event->key.code = key_code; /* 保存键值 */
	event->key.status = PRESSED;/* 保存状态 */
	Send_Mouse_Event(MOUSE_EVENT_CLICK, event); 
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
	    case 1:		/* 鼠标左键被按下 */
		Press_MouseKey(event, MOUSE_LEFT_KEY); 
		Free_MouseKey(event, MOUSE_RIGHT_KEY);
		break;
	    case 2:		/* 鼠标右键被按下 */
		Press_MouseKey(event, MOUSE_RIGHT_KEY); 
		Free_MouseKey(event, MOUSE_LEFT_KEY); 
		break;
	    case 3:		/* 鼠标左右键被按下 */
		Press_MouseKey(event, MOUSE_RIGHT_KEY); 
		Press_MouseKey(event, MOUSE_LEFT_KEY); 
		break;
	    default:		/* 默认是释放的 */
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

static BOOL proc_mouse( void *arg )
{
	int  temp, button, retval; 
	char buf[6]; 
	fd_set readfds; 
	struct timeval tv;
	LCUI_Pos pos; 
	
	if (mouse_data.status == REMOVE || mouse_data.fd < 0) {
		return FALSE;
	}
	/* 设定select等待I/o的最长时间 */
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO (&readfds);
	FD_SET (mouse_data.fd, &readfds);

	retval = select (mouse_data.fd + 1, &readfds, NULL, NULL, &tv);
	if (retval == 0) {
		//printf("Time out!\n");
		return FALSE;
	}
	if ( !FD_ISSET (mouse_data.fd, &readfds) ) {
		return FALSE;
	}
	temp = read (mouse_data.fd, buf, 6);
	if (temp <= 0){
		if (temp < 0) {
			mouse_data.status = REMOVE;
		}
		return FALSE;
	}
	
	pos = Get_Cursor_Pos();
	pos.x += (buf[1] * mouse_data.move_speed); 
	pos.y -= (buf[2] * mouse_data.move_speed);

	if (pos.x > Get_Screen_Width ()) {
		pos.x = Get_Screen_Width ();
	}
	if (pos.y > Get_Screen_Height ()) {
		pos.y = Get_Screen_Height ();
	}
	pos.x = pos.x<0 ? 0:pos.x; 
	pos.y = pos.y<0 ? 0:pos.y; 
	/* 设定游标位置 */ 
	Set_Cursor_Pos (pos);
	
	button = (buf[0] & 0x07);
	//printf("x:%d, y:%d, button:%d\n", pos.x, pos.y, button);
	mouse_event.global_pos = pos; 
	
	mouse_event.widget = Get_Cursor_Overlay_Widget(); 
	if( mouse_event.widget ) {
		mouse_event.pos = GlobalPos_ConvTo_RelativePos( mouse_event.widget, pos );
	} else { 
		mouse_event.pos = pos;
	}
	/* 处理鼠标事件 */
	Handle_Mouse_Event( button, &mouse_event );
	return TRUE;
}

BOOL Enable_Mouse_Input()
/* 功能：启用鼠标输入处理 */
{
	char *msdev;
	
	if(mouse_data.status != REMOVE) {
		return FALSE;
	}
	msdev = getenv("LCUI_MOUSE_DEVICE");
	if( msdev == NULL ) {
		msdev = MS_DEV;
	}
	if ((mouse_data.fd = open (MS_DEV, O_RDONLY)) < 0) {
		printf("failed to open %s.\n", msdev );
		perror(NULL);
		mouse_data.status = REMOVE; 
		return FALSE;
	}
	mouse_data.status = INSIDE;
	printf("open %s successfuly.\n", msdev);
	return TRUE; 
}

BOOL Disable_Mouse_Input()
/* 功能：禁用鼠标输入处理 */
{
	if(mouse_data.status != INSIDE) {
		return FALSE;
	}
	Hide_Cursor();
	close (mouse_data.fd); 
	mouse_data.status = REMOVE;
	return TRUE;
}

int Mouse_Init()
/* 初始化鼠标 */
{
	mouse_data.fd = -1;  
	mouse_data.status = REMOVE;	/* 鼠标为移除状态 */
	mouse_data.move_speed = 1;	/* 移动数度为1 */
	/* 初始化鼠标事件信息队列 */ 
	EventQueue_Init( &LCUI_Sys.mouse_event ); 
	/* 启用鼠标输入处理 */
	nobuff_printf("enable mouse input: ");
	/* 注册鼠标设备 */
	return LCUI_Dev_Add( Enable_Mouse_Input, 
			proc_mouse, Disable_Mouse_Input );
}
/**************************** Mouse End *******************************/
