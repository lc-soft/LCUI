 /* ***************************************************************************
 * keyboard.c -- keyboard support
 *
 * Copyright (C) 2013-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2015 归属于 刘超 <lc-soft@live.cn>
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

typedef struct KeyState {
	int key_code;		/**< 按键键值 */
	int state;		/**< 当前状态 */
	clock_t hit_time;	/**< 按下此键时的时间 */
	clock_t interval_time;	/**< 近两次按下此键的时间间隔 */
} KeyState;

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
static KeyState *GetKeySate( int key_code )
{
	KeyState *ks;
	LinkedListNode *node;

	LinkedList_ForEach( node, &key_state_record ) {
		ks = (KeyState*)node->data;
		if( ks && ks->key_code == key_code ) {
			return ks;
		}
	}
	return NULL;
}

/** 检测指定键值的按键是否处于按下状态 */
LCUI_BOOL LCUIKey_IsHit( int key_code )
{
	KeyState *ks;
	LCUIMutex_Lock( &record_mutex );
	ks = GetKeySate( key_code );
	LCUIMutex_Unlock( &record_mutex );
	if( ks && ks->state == LCUIKEYSTATE_PRESSED ) {
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
	KeyState* ks;
	/* 计算当前时间（单位：毫秒） */
	ct = clock()*1000 / CLOCKS_PER_SEC;
	LCUIMutex_Lock( &record_mutex );
	ks = GetKeySate( key_code );
	LCUIMutex_Unlock( &record_mutex );
	if( !ks ) {
		return FALSE;
	}
	/* 间隔时间为-1，说明该键是新记录的 */
	if( ks->interval_time == -1 ) {
		return FALSE;
	}
	ct -= (ks->hit_time - ks->interval_time);
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
	KeyState *ks;

	LCUIMutex_Lock( &record_mutex );
	ct = clock()*1000 / CLOCKS_PER_SEC;
	ks = GetKeySate( key_code );
	if( !ks ) {
		ks = NEW(KeyState, 1);
		ks->key_code = key_code;
		ks->interval_time = -1;
		ks->hit_time = ct;
		ks->state = LCUIKEYSTATE_PRESSED;
		LinkedList_Append( &key_state_record, ks );
		LCUIMutex_Unlock( &record_mutex );
		return;
	}
	if( ks->state == LCUIKEYSTATE_RELEASE ) {
		ks->state = LCUIKEYSTATE_PRESSED;
		/* 记录与上次此键被按下时的时间间隔 */
		ks->interval_time = ct - ks->hit_time;
		/* 记录本次此键被按下时的时间 */
		ks->hit_time = ct;
	}
	LCUIMutex_Unlock( &record_mutex );
}

/** 标记指定键值的按键已释放 */
void LCUIKeyBoard_ReleaseKey( int key_code )
{
	KeyState *ks;
	LCUIMutex_Lock( &record_mutex );
	ks = GetKeySate( key_code );
	if( ks ) {
		ks->state = LCUIKEYSTATE_RELEASE;
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
static LCUI_BOOL LCUIKeyboard_Exit( void )
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

static void OnKeyboardEvent( LCUI_SystemEvent *e, void *arg )
{
	if( e->type == LCUI_KEYDOWN ) {
		LCUIKeyBoard_HitKey( e->key_code );
	}
	else if( e->type == LCUI_KEYUP ) {
		LCUIKeyBoard_ReleaseKey( e->key_code );
	}
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
	LCUI_SystemEvent e;
	 /* 如果没有按键输入 */
	if ( !LCUIKeyboard_IsHit() ) {
		return FALSE;
	}

	e.type = LCUI_KEYDOWN;
	e.type_name = NULL;
	e.key_code = LCUIKeyboard_GetKey();
	LCUI_PostEvent( &e );
	return TRUE;
}
#endif

/** 初始化键盘输入模块 */
int LCUI_InitKeyboard( void )
{
	int ret;
	LCUIMutex_Init( &record_mutex );
	LinkedList_Init( &key_state_record );
	nobuff_printf("[keyboard] set event ... ");
	ret = LCUI_AddEvent( "keydown", LCUI_KEYDOWN );
	ret |= LCUI_AddEvent( "keyup", LCUI_KEYUP );
	ret |= LCUI_BindEvent( "keydown", OnKeyboardEvent, NULL, NULL );
	ret |= LCUI_BindEvent( "keyup", OnKeyboardEvent, NULL, NULL );
	nobuff_printf(ret < 0 ? "failed\n":"ok\n");
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	ret |= LCUIDevice_Add( LCUIKeyboard_Init, LCUIKeyboard_Proc, LCUIKeyboard_Exit );
#endif
	return ret;
}

/** 停用键盘输入模块 */
int LCUI_ExitKeyboard( void )
{
	LinkedList_Clear( &key_state_record, free );
	LCUIMutex_Destroy( &record_mutex );
	return 0;
}
