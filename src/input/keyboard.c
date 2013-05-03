/* ***************************************************************************
 * keyboard.c -- keyboard support
 * 
 * Copyright (C) 2012-2013 by
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
 * keyboard.c -- 键盘支持
 *
 * 版权所有 (C) 2013 归属于
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

#ifdef LCUI_KEYBOARD_DRIVER_LINUX

//#define __NEED_CATCHSCREEN__
#ifdef __NEED_CATCHSCREEN__
#include LC_GRAPH_H
#include LC_DRAW_H
#include LC_DISPLAY_H
#endif

#include <termios.h>
#include <unistd.h>
#include <sys/fcntl.h>

static struct termios tm;//, tm_old; 
static int fd = STDIN_FILENO;
#endif

/* 检测指定键值的按键是否处于按下状态 */
LCUI_API LCUI_BOOL
LCUIKey_IsHit( int key_code )
{
	int *t;
	int i, total;
	
	total = Queue_GetTotal(&LCUI_Sys.press_key);
	for(i=0; i<total; ++i) {
		t = Queue_Get(&LCUI_Sys.press_key, i);
		if( t && *t == key_code ) {
			return TRUE;
		}
	}
	return FALSE;
}

/* 添加已被按下的按键 */
LCUI_API void
LCUIKey_Hit( int key_code )
{
	Queue_Add( &LCUI_Sys.press_key, &key_code );
}

/* 标记指定键值的按键已释放 */
LCUI_API void
LCUIKey_Free( int key_code )
{
	int *t;
	int i, total;
	
	total = Queue_GetTotal(&LCUI_Sys.press_key);
	for(i=0; i<total; ++i) {
		t = Queue_Get(&LCUI_Sys.press_key, i);
		if( t && *t == key_code ) {
			Queue_Delete( &LCUI_Sys.press_key, i );
		}
	}
}

/* 初始化键盘输入 */
LCUI_API int
LCUIKeyboard_Init( void )
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
LCUI_API int
LCUIKeyboard_End( void )
{
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	tm.c_lflag |= ICANON;
	tm.c_lflag |= ECHO;
	tm.c_cc[VMIN] = 1;
	tm.c_cc[VTIME] = 0;
	if(tcsetattr(fd,TCSANOW,&tm)<0) {
		return -1;
	}
	printf("\033[?25h"); /* 显示光标 */ 
#endif
	return 0;
}

/* 添加键盘的按键按下事件 */
LCUI_API void
LCUIKeyboard_HitKey( int key_code )
{
	LCUI_Event event;
	event.type = LCUI_KEYDOWN;
	event.key.key_code = key_code;
	LCUIKey_Hit( key_code );
	LCUI_PushEvent( &event );
}

/* 添加键盘的按键释放事件 */
LCUI_API void
LCUIKeyboard_FreeKey( int key_code )
{
	LCUI_Event event;
	event.type = LCUI_KEYUP;
	event.key.key_code = key_code;
	LCUIKey_Free( key_code );
	LCUI_PushEvent( &event );
}

/* 检测是否有按键按下 */
LCUI_API LCUI_BOOL
LCUIKeyboard_IsHit( void )
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
	if( Queue_GetTotal( &LCUI_Sys.press_key ) > 0) {
		return TRUE;
	}
	return FALSE;
}

/* 获取被按下的按键的键值 */
LCUI_API int
LCUIKeyboard_Get( void )
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
		LCUIKeyboard_Get();
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
	int *key_ptr;
	while( Queue_GetTotal(&LCUI_Sys.press_key) == 0 ) {
		LCUI_MSleep(100);
	}
	key_ptr = Queue_Get( &LCUI_Sys.press_key, 0 );
	if( key_ptr ) {
		return *key_ptr;
	}
	return -1;
#endif
}

#ifdef LCUI_KEYBOARD_DRIVER_LINUX
static LCUI_BOOL proc_keyboard(void)
{
	LCUI_Event event;
	 /* 如果没有按键输入 */ 
	if ( !LCUIKeyboard_IsHit() ) {
		return FALSE;
	}
	
	event.type = LCUI_KEYDOWN;
	event.key.key_code = LCUIKeyboard_Get();
	
	#ifdef __NEED_CATCHSCREEN__
	LCUI_Rect area;
	area.width = 320;
	area.height = 240;
	area.x = (LCUIScreen_GetWidth()-area.width)/2;
	area.y = (LCUIScreen_GetHeight()-area.height)/2;
	//当按下c键后，可以进行截图，只截取指定区域的图形
	if(event.key.key_code == 'c') {
		time_t rawtime;
		struct tm * timeinfo;
		char filename[100];
		LCUI_Graph graph;
		
		Graph_Init(&graph);
		time ( &rawtime );
		timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */ 
		sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d.png",
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
			timeinfo->tm_mday, timeinfo->tm_hour, 
			timeinfo->tm_min, timeinfo->tm_sec
		);
		LCUIScreen_CatchGraph( area, &graph );
		write_png(filename, &graph);
		Graph_Free(&graph);
	}
	else if(event.key.key_code == 'r') {
		/* 如果按下r键，就录制指定区域的图像 */
		LCUIScreen_StartRecord( area );
	}
	#endif
	LCUI_PushEvent( &event );
	return TRUE;
}

/* 键盘输入模块的初始化 */
static LCUI_BOOL Enable_Keyboard_Input( void )
{
	LCUIKeyboard_Init(); /* 设置终端属性 */
	return TRUE;
}

/* 键盘输入模块的销毁 */
static LCUI_BOOL Disable_Keyboard_Input( void )
{
	LCUIKeyboard_End(); /* 恢复终端属性 */
	return TRUE;
}
#endif

/* 初始化键盘输入模块 */
LCUI_API void
LCUIModule_Keyboard_Init( void )
{
	Queue_Init( &LCUI_Sys.press_key, sizeof(int), NULL );
#ifdef LCUI_KEYBOARD_DRIVER_LINUX
	LCUIDevice_Add( Enable_Keyboard_Input, proc_keyboard, Disable_Keyboard_Input );
#endif
}

/* 停用键盘输入模块 */
LCUI_API void
LCUIModule_Keyboard_End( void )
{
	Queue_Destroy( &LCUI_Sys.press_key );
}
