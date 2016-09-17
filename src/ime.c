/* ***************************************************************************
 * ime.c -- Input Method Editor/Engine
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
 * ime.c -- 输入法管理器
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

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include <LCUI/ime.h>

/** 输入法相关数据 */
typedef struct LCUI_IMERec_ {
	int id;				/**< 输入法ID */
	char *name;			/**< 名称 */
	LCUI_IMEHandlerRec handler;	/**< 处理器 */
	LinkedListNode node;		/**< 在链表中的结点 */
} LCUI_IMERec, *LCUI_IME;

/** 输入法管理模块的相关数据 */
static struct LCUIIMEModule {
	LCUI_BOOL is_inited;		/**< 是否已经初始化 */
	LinkedList list;		/**< 当前记录的输入法列表 */
	LCUI_IME ime;			/**< 当前激活的输入法 */
	LCUI_BOOL enable_caps_lock;	/**< 是否已经启用大写锁定  */
	int id_count;			/**< ID 计数器，用于生成输入法的 ID */
} self;


/* 通过ID查找输入法信息 */
static LCUI_IME LCUIIME_Find( int ime_id )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &self.list ) ) {
		LCUI_IME ime = node->data;
		if( ime->id == ime_id ) {
			return ime;
		}
	}
	return NULL;
}

/* 通过名字查找输入法信息 */
static LCUI_IME LCUIIME_FindByName( const char *name )
{
	LinkedListNode *node;
	for( LinkedList_Each( node, &self.list ) ) {
		LCUI_IME ime = node->data;
		if( strcmp( ime->name, name ) == 0 ) {
			return ime;
		}
	}
	return NULL;
}

/* 注册一个输入法 */
int LCUIIME_Register( const char *name, LCUI_IMEHandler handler )
{
	size_t len;
	LCUI_IME ime;
	if( !self.is_inited ) {
		return -1;
	}
	if( LCUIIME_FindByName( name )  ) {
		return -2;
	}
	ime = NEW( LCUI_IMERec, 1 );
	if( !ime ) {
		return -ENOMEM;
	}
	len = strlen( name ) + 1;
	ime->name = malloc( len * sizeof(char) );
	if( !ime->name ) {
		return -ENOMEM;
	}
	self.id_count += 1;
	ime->id = self.id_count;
	ime->node.data = ime;
	strncpy( ime->name, name, len );
	memcpy( &ime->handler, handler, sizeof(LCUI_IMEHandlerRec) );
	LinkedList_AppendNode( &self.list, &ime->node );
	return ime->id;
}

/* 打开输入法 */
static LCUI_BOOL LCUIIME_Open( LCUI_IME ime )
{
	if( ime && ime->handler.open ) {
		return ime->handler.open();
	}
	return FALSE;
}

/* 关闭输入法 */
static LCUI_BOOL LCUIIME_Close( LCUI_IME ime )
{
	if( ime && ime->handler.close ) {
		return ime->handler.open();
	}
	return FALSE;
}

/* 选定输入法 */
LCUI_BOOL LCUIIME_Select( int ime_id )
{
	LCUI_IME ime = LCUIIME_Find( ime_id );
	if( ime ) {
		LCUIIME_Close( self.ime );
		self.ime = ime;
		LCUIIME_Open( self.ime );
		return TRUE;
	}
	return FALSE;
}

LCUI_BOOL LCUIIME_SelectByName( const char *name )
{
	LCUI_IME ime = LCUIIME_FindByName( name );
	if( ime ) {
		LCUIIME_Close( self.ime );
		self.ime = ime;
		LCUIIME_Open( self.ime );
		return TRUE;
	}
	return FALSE;
}

/* 切换至下一个输入法 */
void LCUIIME_Switch( void )
{
	LCUI_IME ime;
	if( self.ime && self.ime->node.next ) {
		ime = self.ime->node.next->data;
		LCUIIME_Close( self.ime );
		self.ime = ime;
		LCUIIME_Open( self.ime );
	}
}

/* 销毁输入法信息 */
static void LCUIIME_OnDestroy( void *arg )
{
	LCUI_IME ime = arg;
	free( ime->name );
	ime->name = NULL;
}

static void LCUIIME_ToText( LCUI_SysEvent e )
{
	char ch = e->key.code;
	DEBUG_MSG("key code: %d\n", event->key_code);
	/* 如果没开启大写锁定，则将字母转换成小写 */
	if( !self.enable_caps_lock ) {
		if( ch >= 'A' && ch <= 'Z' ) {
			ch = e->key.code + 32;
		}
	}

	/* 如果shift键处于按下状态 */
	if( LCUIKeyboard_IsHit(LCUIKEY_SHIFT) ) {
		_DEBUG_MSG("hit shift, ch: %c\n", ch);
		if(ch >='a' && ch <= 'z') {
			ch = ch - 32;
		} else if(ch >='A' && ch <= 'Z') {
			ch = ch + 32;
		} else {
			switch(ch) {
			case '1': ch = '!'; break;
			case '2': ch = '@'; break;
			case '3': ch = '#'; break;
			case '4': ch = '$'; break;
			case '5': ch = '%'; break;
			case '6': ch = '^'; break;
			case '7': ch = '&'; break;
			case '8': ch = '*'; break;
			case '9': ch = '('; break;
			case '0': ch = ')'; break;
			case '`': ch = '~'; break;
			case '-': ch = '_'; break;
			case '=': ch = '+'; break;
			case '[': ch = '{'; break;
			case ']': ch = '}'; break;
			case '\\': ch = '|'; break;
			case ';': ch = ':'; break;
			case '\'': ch = '"'; break;
			case ',': ch = '<'; break;
			case '.': ch = '>'; break;
			case '/': ch = '?'; break;
			}
		}
	}
	DEBUG_MSG("ch = %c\n", ch);
	self.ime->handler.totext( ch );
}

LCUI_BOOL LCUIIME_ProcessKey( LCUI_SysEvent e )
{
	int key_state;
	/* 根据事件类型判定按键状态 */
	if( e->type == LCUI_KEYUP ) {
		key_state = LCUIKEYSTATE_RELEASE;
		/* 如果是caps lock按键被释放 */
		if( e->key.code == LCUIKEY_CAPITAL ) {
			self.enable_caps_lock = !self.enable_caps_lock;
			return FALSE;
		}
	} else {
		key_state = LCUIKEYSTATE_PRESSED;
		/* 如果按下的是 shift 键，但没释放，则直接退出 */
		if( e->key.code == LCUIKEY_SHIFT ) {
			return FALSE;
		}
	}
	if( self.ime && self.ime->handler.prockey ) {
		return self.ime->handler.prockey( e->key.code, key_state );
	}
	return FALSE;
}

int LCUIIME_Commit( const wchar_t *str, int length )
{
	LCUI_SysEventRec sys_ev;
	if( length < 0 ) {
		length = wcslen( str );
	}
	sys_ev.type = LCUI_TEXTINPUT;
	sys_ev.text.length = length;
	sys_ev.text.text = NEW( wchar_t, length + 1 );
	if( !sys_ev.text.text ) {
		return -ENOMEM;
	}
	wcsncpy( sys_ev.text.text, str, length + 1 );
	LCUI_TriggerEvent( &sys_ev, NULL );
	free( sys_ev.text.text );
	sys_ev.text.text = NULL;
	sys_ev.text.length = 0;
	return 0;
}

int LCUIIME_SetTarget( LCUI_Widget widget )
{
	if( self.ime && self.ime->handler.settarget ) {
		self.ime->handler.settarget( widget );
		return 0;
	}
	return -1;
}

LCUI_Widget LCUIIME_GetTarget( void )
{
	if( self.ime && self.ime->handler.gettarget ) {
		return self.ime->handler.gettarget();
	}
	return NULL;
}
int LCUIIME_ClearTarget( void )
{
	if( self.ime && self.ime->handler.cleartarget ) {
		self.ime->handler.cleartarget();
		return 0;
	}
	return -1;
}

static void LCUIIME_OnKeyDown( LCUI_SysEvent e, void *arg )
{
	_DEBUG_MSG("on keydown\n");
	if( LCUIIME_ProcessKey( e ) ) {
		LCUIIME_ToText( e );
	}
}

/* 初始化LCUI输入法模块 */
void LCUI_InitIME( void )
{
	int ime_id;
	LinkedList_Init( &self.list );
	self.is_inited = TRUE;
	LCUI_BindEvent( LCUI_KEYDOWN, LCUIIME_OnKeyDown, NULL, NULL );
#ifdef LCUI_BUILD_IN_WIN32
	ime_id = LCUI_RegisterWin32IME();
#else
	ime_id = LCUI_RegisterLinuxIME();
#endif
	LCUIIME_Select( ime_id );
}

/* 停用LCUI输入法模块 */
void LCUI_ExitIME( void )
{
	self.is_inited = FALSE;
	LinkedList_ClearData( &self.list, LCUIIME_OnDestroy );
}
