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
	int fd, state;		 /* 句柄，状态 */
	float move_speed;	 /* 鼠标移动速度，1.0为正常速度 */
}
LCUI_Mouse;
/*************************************************************/

static LCUI_Mouse mouse_data;

/****************************** Mouse *********************************/
/*
 * 功能：检测鼠标事件中鼠标左键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件无效
 *   -1  键值不是鼠标左键的键值
 *   0   鼠标左键已经释放
 *   1   鼠标左键处于按下状态
 **/
int Mouse_LeftButton( LCUI_MouseButtonEvent *event )
{
	if( !event ) {
		return -2;
	}
	else if(event->button == MOUSE_LEFT_KEY) {
		return event->state;
	}
	return -1;
}

/*
 * 功能：检测鼠标事件中鼠标右键的状态
 * 说明：该函数只适用于响应鼠标按键状态发生改变时，判断按键状态。
 * 返回值：
 *   -2  事件指针为NULL
 *   -1  键值不是鼠标右键的键值
 *   0   鼠标右键已经释放
 *   1   鼠标右键处于按下状态
 **/
int Mouse_RightButton( LCUI_MouseButtonEvent *event )
{
	if(NULL == event) {
		return -2;
	}
	else if(event->button == MOUSE_RIGHT_KEY) {
		return event->state; 
	}
	return -1;
}

int Click_LeftButton (LCUI_MouseButtonEvent *event)
/* 功能：检测是否是按鼠标左键 */
{
	/* 如果按下的是鼠标左键，并且之前没有按住它 */
	if (Mouse_LeftButton(event) == PRESSED
		&& !LCUIKey_IsHit(event->button)) {
		return 1;
	}
	return 0;
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

/* 记录被按下的指定键的键值，并添加LCUI_MOUSEBUTTONDOWN事件 */
static void LCUIMouse_ButtonDown( LCUI_Pos pos, int key_code )
{
	int temp;
	LCUI_Event event;
	
	/* 若该键已经按下，就不需要再添加至队列了 */
	if( LCUIKey_IsHit( key_code ) ) {
		return;
	}
	temp = Queue_Add(&LCUI_Sys.press_key, &key_code);
	if( temp < 0 ) {
		return;
	}
	event.type = LCUI_MOUSEBUTTONDOWN;
	event.button.x = pos.x;
	event.button.y = pos.y;
	event.button.button = key_code;
	event.button.state = PRESSED;
	LCUI_PushEvent( &event );
}

/* 记录被释放的指定键的键值，并添加LCUI_MOUSEBUTTONUP事件 */
static void LCUIMouse_ButtonFree( LCUI_Pos pos, int key_code )
{
	LCUI_Event event;
	
	if( !LCUIKey_IsHit( key_code ) ) {
		return; 
	}
	LCUIKey_Free( key_code );
	event.type = LCUI_MOUSEBUTTONUP;
	event.button.x = pos.x;
	event.button.y = pos.y;
	event.button.button = key_code;
	event.button.state = FREE;
	LCUI_PushEvent( &event );
}

/* 功能：处理鼠标产生的事件 */
void LCUI_PushMouseEvent( LCUI_Pos new_pos, int button_type )
{
	int key_state = FREE;
	static LCUI_Pos old_pos;
	
	switch (button_type) {
	    case 1:		/* 鼠标左键被按下 */
		LCUIMouse_ButtonDown( new_pos, MOUSE_LEFT_KEY );
		key_state = PRESSED;
		LCUIMouse_ButtonFree( new_pos, MOUSE_RIGHT_KEY );
		break;
	    case 2:		/* 鼠标右键被按下 */
		LCUIMouse_ButtonDown( new_pos, MOUSE_RIGHT_KEY );
		LCUIMouse_ButtonFree( new_pos, MOUSE_LEFT_KEY );
		key_state = FREE;
		break;
	    case 3:		/* 鼠标左右键被按下 */
		LCUIMouse_ButtonDown( new_pos, MOUSE_RIGHT_KEY ); 
		LCUIMouse_ButtonDown( new_pos, MOUSE_LEFT_KEY ); 
		key_state = PRESSED;
		break;
	    default:		/* 默认是释放的 */
		LCUIMouse_ButtonFree( new_pos, MOUSE_RIGHT_KEY );
		LCUIMouse_ButtonFree( new_pos, MOUSE_LEFT_KEY );
		key_state = FREE;
		break;
	}
	
	/* 如果鼠标位置有改变 */
	if(Pos_Cmp( new_pos, old_pos ) != 0) {
		LCUI_Event event;
		event.type = LCUI_MOUSEMOTION;
		event.motion.x = new_pos.x;
		event.motion.y = new_pos.y;
		event.motion.xrel = new_pos.x - old_pos.x;
		event.motion.yrel = new_pos.y - old_pos.y;
		event.motion.state = key_state;
		LCUI_PushEvent( &event );
		old_pos = new_pos;
	}
}

#ifdef LCUI_MOUSE_DRIVER_LINUX
static BOOL proc_mouse( void *arg )
{
	static int  tmp; 
	static char buf[6]; 
	static fd_set readfds; 
	static struct timeval tv;
	static LCUI_Pos pos;
	
	if (mouse_data.state == REMOVE || mouse_data.fd < 0) {
		return FALSE;
	}
	/* 设定select等待I/o的最长时间 */
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO (&readfds);
	FD_SET (mouse_data.fd, &readfds);

	tmp = select (mouse_data.fd+1, &readfds, NULL, NULL, &tv);
	if (tmp == 0) {
		//printf("Time out!\n");
		return FALSE;
	}
	if ( !FD_ISSET (mouse_data.fd, &readfds) ) {
		return FALSE;
	}
	tmp = read (mouse_data.fd, buf, 6);
	if (tmp <= 0){
		if (tmp < 0) {
			mouse_data.state = REMOVE;
		}
		return FALSE;
	}
	
	pos = Get_Cursor_Pos();
	pos.x += (buf[1] * mouse_data.move_speed); 
	pos.y -= (buf[2] * mouse_data.move_speed);

	if (pos.x > LCUIScreen_GetWidth ()) {
		pos.x = LCUIScreen_GetWidth ();
	}
	if (pos.y > LCUIScreen_GetHeight ()) {
		pos.y = LCUIScreen_GetHeight ();
	}
	pos.x = pos.x<0 ? 0:pos.x; 
	pos.y = pos.y<0 ? 0:pos.y;
	/* 应用鼠标游标的位置变更 */
	Set_Cursor_Pos( pos );
	/* 处理鼠标事件 */
	LCUI_PushMouseEvent( pos, buf[0] & 0x07 );
	return TRUE;
}
#endif

BOOL Enable_Mouse_Input()
/* 功能：启用鼠标输入处理 */
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	char *msdev;
	
	if(mouse_data.state != REMOVE) {
		return FALSE;
	}
	msdev = getenv("LCUI_MOUSE_DEVICE");
	if( msdev == NULL ) {
		msdev = MS_DEV;
	}
	if ((mouse_data.fd = open (MS_DEV, O_RDONLY)) < 0) {
		printf("failed to open %s.\n", msdev );
		perror(NULL);
		mouse_data.state = REMOVE; 
		return FALSE;
	}
	mouse_data.state = INSIDE;
	printf("open %s successfuly.\n", msdev);
#endif
	return TRUE; 
}

BOOL Disable_Mouse_Input()
/* 功能：禁用鼠标输入处理 */
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	if(mouse_data.state != INSIDE) {
		return FALSE;
	}
	Hide_Cursor();
	close (mouse_data.fd); 
	mouse_data.state = REMOVE;
#endif
	return TRUE;
}

/* 初始化鼠标输入模块 */
int LCUIModule_Mouse_Init( void )
{
#ifdef LCUI_MOUSE_DRIVER_LINUX
	mouse_data.fd = -1;  
	mouse_data.state = REMOVE;	/* 鼠标为移除状态 */
	mouse_data.move_speed = 1;	/* 移动数度为1 */
	/* 启用鼠标输入处理 */
	nobuff_printf("enable mouse input: ");
	/* 注册鼠标设备 */
	return LCUI_Dev_Add( Enable_Mouse_Input, 
			proc_mouse, Disable_Mouse_Input );
#else
	return 0;
#endif
}
/**************************** Mouse End *******************************/
