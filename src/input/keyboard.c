 /* ***************************************************************************
 * keyboard.c -- keyboard support
 * 
 * Copyright (C) 2013-2014 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/
 
/* ****************************************************************************
 * keyboard.c -- 键盘支持
 *
 * 版权所有 (C) 2013-2014 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/input.h>

#include <LCUI/misc/linkedlist.h>

#include <time.h>

typedef struct key_state_ {
	int key_code;		/**< 按键键值 */
	int state;		/**< 当前状态 */
	clock_t hit_time;	/**< 按下此键时的时间 */
	clock_t interval_time;	/**< 近两次按下此键的时间间隔 */
} key_state;

static LCUI_Mutex record_mutex;
static LinkedList key_state_record;

#ifdef LCUI_KEYBOARD_DRIVER_LINUX

//#define __NEED_CATCHSCREEN__
#ifdef __NEED_CATCHSCREEN__
#include <LCUI/graph.h>
#include <LCUI/display.h>
#endif

#include <termios.h>
#include <unistd.h>
#include <sys/fcntl.h>

static struct termios tm;//, tm_old; 
static int fd = STDIN_FILENO;
#endif

/** 获取指定按键的状态数据 */
static key_state* KeyStateRecord_FindData( int key_code )
{
	int i, n;
	key_state* data_ptr;

	n = LinkedList_GetTotal( &key_state_record );
	for( i=0; i<n; ++i ) {
		data_ptr = (key_state*)LinkedList_Get( &key_state_record );
		if( data_ptr && data_ptr->key_code == key_code ) {
			return data_ptr;
		}
		LinkedList_ToNext( &key_state_record );
	}
	return NULL;
}

/** 检测指定键值的按键是否处于按下状态 */
LCUI_BOOL LCUIKey_IsHit( int key_code )
{
	key_state *data_ptr;
	LCUIMutex_Lock( &record_mutex );
	data_ptr = KeyStateRecord_FindData( key_code );
	LCUIMutex_Unlock( &record_mutex );
	if( data_ptr && data_ptr->state == LCUIKEYSTATE_PRESSED ) {
		return TRUE;
	}
	return FALSE;
}

/**
检测指定键值的按键是否按了两次
@param key_code
	要检测的按键的键值
@param interval_time
	该按键倒数第二次按下时的时间与当前时间的最大间隔
 */
LCUI_BOOL LCUIKey_IsDoubleHit( int key_code, int interval_time )
{
	clock_t ct;
	key_state* data_ptr;
	/* 计算当前时间（单位：毫秒） */
	ct = clock()*1000 / CLOCKS_PER_SEC;
	LCUIMutex_Lock( &record_mutex );
	data_ptr = KeyStateRecord_FindData( key_code );
	LCUIMutex_Unlock( &record_mutex );
	if( !data_ptr ) {
		return FALSE;
	}
	/* 间隔时间为-1，说明该键是新记录的 */
	if( data_ptr->interval_time == -1 ) {
		return FALSE;
	}
	ct -= (data_ptr->hit_time - data_ptr->interval_time);
	/* 判断按键被按下两次时是否在距当前interval_time毫秒的时间内发生 */
	if( ct  <= interval_time ) {
		return TRUE;
	}
	return FALSE;
}

/** 添加已被按下的按键 */
void LCUIKeyBoard_HitKey( int key_code )
{
	clock_t ct;
	key_state data, *data_ptr;

	LCUIMutex_Lock( &record_mutex );
	ct = clock()*1000 / CLOCKS_PER_SEC;
	data_ptr = KeyStateRecord_FindData( key_code );
	if( !data_ptr ) {
		data.key_code = key_code;
		data.interval_time = -1;
		data.hit_time = ct;
		data.state = LCUIKEYSTATE_PRESSED;
		LinkedList_AddDataCopy( &key_state_record, &data );
		LCUIMutex_Unlock( &record_mutex );
		return;
	}
	if( data_ptr->state == LCUIKEYSTATE_RELEASE ) {
		data_ptr->state = LCUIKEYSTATE_PRESSED;
		/* 记录与上次此键被按下时的时间间隔 */
		data_ptr->interval_time = ct - data_ptr->hit_time;
		/* 记录本次此键被按下时的时间 */
		data_ptr->hit_time = ct;
	}
	LCUIMutex_Unlock( &record_mutex );
}

/** 标记指定键值的按键已释放 */
void LCUIKeyBoard_ReleaseKey( int key_code )
{
	key_state *data_ptr;
	LCUIMutex_Lock( &record_mutex );
	data_ptr = KeyStateRecord_FindData( key_code );
	if( data_ptr ) {
		data_ptr->state = LCUIKEYSTATE_RELEASE;
	}
	LCUIMutex_Unlock( &record_mutex );
}

/* 初始化键盘输入 */
static LCUI_BOOL LCUIKeyboard_Init( void )
{
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	if(tcgetattr(fd, &tm) < 0) {
		return -1; 
	} 
	/* 设置终端为无回显无缓冲模式 */
	tm.c_lflag &= ~(ICANON|ECHO);
	tm.c_cc[VMIN] = 1;
	tm.c_cc[VTIME] = 0;
	if(tcsetattr(fd, TCSANOW, &tm) < 0) {
		return -1; 
	}
	printf("\033[?25l");/* 隐藏光标 */
#endif
	return 0;
}

/* 停用键盘输入 */
static int LCUIKeyboard_Exit( void )
{
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	tm.c_lflag |= ICANON;
	tm.c_lflag |= ECHO;
	tm.c_cc[VMIN] = 1;
	tm.c_cc[VTIME] = 0;
	if(tcsetattr(fd,TCSANOW,&tm)<0) {
		return FALSE;
	}
	printf("\033[?25h"); /* 显示光标 */ 
#endif
	return TRUE;
}

/* 添加键盘的按键按下事件 */
void LCUI_PostKeyDownEvent( int key_code )
{
	key_state *data_ptr;
	LCUI_SystemEvent event;
	
	LCUIMutex_Lock( &record_mutex );
	data_ptr = KeyStateRecord_FindData( key_code );
	LCUIMutex_Unlock( &record_mutex );
	/* 已经按下过的按键就不用再触发KEYDOWN事件了 */
	if( data_ptr && data_ptr->state == LCUIKEYSTATE_PRESSED ) {
		return;
	}
	event.type = LCUI_KEYDOWN;
	event.which = key_code;
	LCUIKeyBoard_HitKey( key_code );
	LCUI_PostEvent( &event );
}

/* 添加键盘的按键释放事件 */
void LCUI_PostKeyUpEvent( int key_code )
{
	LCUI_SystemEvent event;
	event.type = LCUI_KEYUP;
	event.which = key_code;
	LCUIKeyBoard_ReleaseKey( key_code );
	LCUI_PostEvent( &event );
}

/** 检测键盘是否有按键按下（类似于kbhit函数） */
LCUI_BOOL LCUIKeyboard_IsHit( void )
{
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	struct termios oldt;//, newt;  
	int ch, oldf;  
	tcgetattr(STDIN_FILENO, &oldt);  
	//newt = oldt;  
	//newt.c_lflag &= ~(ICANON | ECHO);  
	//tcsetattr(STDIN_FILENO, TCSANOW, &newt);  
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  
	ch = getchar();  /* 获取一个字符 */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  
	fcntl(STDIN_FILENO, F_SETFL, oldf); 
	/* 如果字符有效，将字符放回输入缓冲区中 */
	if(ch != EOF) {
		ungetc(ch, stdin);  
		return TRUE;
	} 
	return FALSE;
#endif
	return FALSE;
}

/** 获取被按下的按键的键值（类似于getch函数） */
int LCUIKeyboard_GetKey( void )
{ 
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	int k,c;
	static int input = 0, count = 0;
	++count;
	/* 获取输入缓冲中的字符 */
	k = fgetc(stdin);
	input += k;
	/* 如果还有字符在缓冲中 */
	if(LCUIKeyboard_IsHit()) {
		LCUIKeyboard_GetKey();
	}
	c = input;
	--count;
	/* 递归结束后就置0 */
	if(count == 0) {
		input = 0;
	}
	if(c == 3) {
		exit(1);
	}
	return c; 
#else 
	return -1;
#endif
}

#ifdef LCUI_KEYBOARD_DRIVER_LINUX
static LCUI_BOOL LCUIKeyboard_Proc(void)
{
	LCUI_Event event;
	 /* 如果没有按键输入 */ 
	if ( !LCUIKeyboard_IsHit() ) {
		return FALSE;
	}
	
	event.type = LCUI_KEYDOWN;
	event.key.key_code = LCUIKeyboard_GetKey();
	LCUI_PushEvent( &event );
	return TRUE;
}
#endif

/** 初始化键盘输入模块 */
void LCUIModule_Keyboard_Init( void )
{
	LCUIMutex_Init( &record_mutex );
	LinkedList_Init( &key_state_record, sizeof(key_state) );
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	LCUIDevice_Add( LCUIKeyboard_Init, LCUIKeyboard_Proc, LCUIKeyboard_Exit );
#endif
}

/** 停用键盘输入模块 */
void LCUIModule_Keyboard_End( void )
{
	LinkedList_Destroy( &key_state_record );
	LCUIMutex_Destroy( &record_mutex );
}
