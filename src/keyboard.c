/* keyboard.c -- Keyboard input management
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


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
