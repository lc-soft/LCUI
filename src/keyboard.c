/* ***************************************************************************
 * keyboard.c -- keyboard input management
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
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
 * keyboard.c -- 键盘输入管理
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
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

#include <time.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/input.h>

typedef struct KeyStateNodeRec_ {
	int state;		/**< 当前状态 */
	clock_t hit_time;	/**< 按下此键时的时间 */
	clock_t interval_time;	/**< 近两次按下此键的时间间隔 */
} KeyStateNodeRec, *KeyStateNode;

static struct LCUIKeyboardModule {
	LCUI_Mutex mutex;
	RBTree state_tree;
} self;

/** 检测指定键值的按键是否处于按下状态 */
LCUI_BOOL LCUIKeyboard_IsHit( int key_code )
{
	KeyStateNode node;
	LCUIMutex_Lock( &self.mutex );
	node = RBTree_GetData( &self.state_tree, key_code );
	LCUIMutex_Unlock( &self.mutex );
	if( node && node->state == LCUIKEYSTATE_PRESSED ) {
		return TRUE;
	}
	return FALSE;
}

/**
 * 检测指定键值的按键是否按了两次
 * @param key_code 要检测的按键的键值
 * @param interval_time 该按键倒数第二次按下时的时间与当前时间的最大间隔
*/
LCUI_BOOL LCUIKeyboard_IsDoubleHit( int key_code, int interval_time )
{
	clock_t ct;
	KeyStateNode node;
	/* 计算当前时间（单位：毫秒） */
	ct = clock() * 1000 / CLOCKS_PER_SEC;
	LCUIMutex_Lock( &self.mutex );
	node = RBTree_GetData( &self.state_tree, key_code );
	LCUIMutex_Unlock( &self.mutex );
	if( !node ) {
		return FALSE;
	}
	/* 间隔时间为-1，说明该键是新记录的 */
	if( node->interval_time == -1 ) {
		return FALSE;
	}
	ct -= node->hit_time - node->interval_time;
	/* 判断按键被按下两次时是否在距当前 interval_time 毫秒的时间内发生 */
	if( ct  <= interval_time ) {
		return TRUE;
	}
	return FALSE;
}

/** 添加已被按下的按键 */
void LCUIKeyboard_HitKey( int key_code )
{
	clock_t ct;
	KeyStateNode node;
	LCUIMutex_Lock( &self.mutex );
	ct = clock() * 1000 / CLOCKS_PER_SEC;
	node = RBTree_GetData( &self.state_tree, key_code );
	if( !node ) {
		node = NEW( KeyStateNodeRec, 1 );
		node->interval_time = -1;
		node->hit_time = ct;
		node->state = LCUIKEYSTATE_PRESSED;
		RBTree_Insert( &self.state_tree, key_code, node );
		LCUIMutex_Unlock( &self.mutex );
		return;
	}
	if( node->state == LCUIKEYSTATE_RELEASE ) {
		node->state = LCUIKEYSTATE_PRESSED;
		/* 记录与上次此键被按下时的时间间隔 */
		node->interval_time = ct - node->hit_time;
		/* 记录本次此键被按下时的时间 */
		node->hit_time = ct;
	}
	LCUIMutex_Unlock( &self.mutex );
}

/** 标记指定键值的按键已释放 */
void LCUIKeyboard_ReleaseKey( int key_code )
{
	KeyStateNode node;
	LCUIMutex_Lock( &self.mutex );
	node = RBTree_GetData( &self.state_tree, key_code );
	if( node ) {
		node->state = LCUIKEYSTATE_RELEASE;
	}
	LCUIMutex_Unlock( &self.mutex );
}

static void OnKeyboardEvent( LCUI_SysEvent e, void *arg )
{
	if( e->type == LCUI_KEYDOWN ) {
		LCUIKeyboard_HitKey( e->key.code );
	} else if( e->type == LCUI_KEYUP ) {
		LCUIKeyboard_ReleaseKey( e->key.code );
	}
}

void LCUI_InitKeyboard( void )
{
	LCUIMutex_Init( &self.mutex );
	RBTree_Init( &self.state_tree );
	RBTree_OnDestroy( &self.state_tree, free );
	LCUI_BindEvent( LCUI_KEYDOWN, OnKeyboardEvent, NULL, NULL );
	LCUI_BindEvent( LCUI_KEYUP, OnKeyboardEvent, NULL, NULL );
}

void LCUI_FreeKeyboard( void )
{
	RBTree_Destroy( &self.state_tree );
	LCUIMutex_Destroy( &self.mutex );
}
