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
	char ch;

	switch(e->key.code) {
#ifdef LCUI_BUILD_IN_WIN32
	case 189: ch = '-'; break;
	case 187: ch = '='; break;
	case 188: ch = ','; break;
	case 190: ch = '.'; break;
	case 191: ch = '/'; break;
	case 222: ch = '\''; break;
	case 186: ch = ';'; break;
	case 220: ch = '\\'; break;
	case 221: ch = ']'; break;
	case 219: ch = '['; break;
#endif
	default:ch = e->key.code;break;
	}

	DEBUG_MSG("key code: %d\n", event->key_code);
	/* 如果没开启大写锁定，则将字母转换成小写 */
	if( !self.enable_caps_lock ) {
		if( ch >= 'A' && ch <= 'Z' ) {
			ch = e->key.code + 32;
		}
	}

	/* 如果shift键处于按下状态 */
	if( LCUIKeyboard_IsHit(LCUIKEY_SHIFT) ) {
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
		/* 如果输入法要处理该键，则调用LCUIIME_ToText函数 */
		if( self.ime->handler.prockey( e->key.code, key_state ) ) {
			LCUIIME_ToText( e );
			return TRUE;
		}
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

static int LCUI_RegisterDefaultIME( void );

/* 初始化LCUI输入法模块 */
void LCUI_InitIME( void )
{
	int ime_id;
	LinkedList_Init( &self.list );
	self.is_inited = TRUE;
	ime_id = LCUI_RegisterDefaultIME();
	LCUIIME_Select( ime_id );
}

/* 停用LCUI输入法模块 */
void LCUI_ExitIME( void )
{
	self.is_inited = FALSE;
	LinkedList_ClearData( &self.list, LCUIIME_OnDestroy );
}


/*-------------------------- 默认的输入法 ------------------------------*/

#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/platform.h>
#include LCUI_EVENTS_H
#endif

static struct {
	LCUI_Widget target;
} default_ime;


/**
判断按键是否需要处理，也可以用于设置当前的输入法，比如：
按shift键，切换输入法的中英文输入，按PageUP和PageDown切换词组页
按backspace键，如果输入的内容为空，则return FALSE;让LCUI将该按键事件发给部件
的相应回调函数处理；否则，return TRUE;并从输入的内容中删除一个字符
**/
static LCUI_BOOL IME_ProcessKey( int key, int key_state )
{
#ifdef LCUI_BUILD_IN_LINUX
	/* *
	 * LCUI的linux键盘驱动获取的是输入的字符的ASCII码，而不是实际键值，
	 * 因此，直接判断是否为可显字符即可
	 * */
	if( key >= '!' && key <= '~') {
		return TRUE;
	}
#elif defined LCUI_BUILD_IN_WIN32
	if( key_state != LCUIKEYSTATE_PRESSED ) {
		return FALSE;
	}
#endif
	switch(key) {
	case LCUIKEY_ENTER:
	case LCUIKEY_SPACE:
#ifdef LCUI_BUILD_IN_WIN32
	case LCUIKEY_0:
	case LCUIKEY_1:
	case LCUIKEY_2:
	case LCUIKEY_3:
	case LCUIKEY_4:
	case LCUIKEY_5:
	case LCUIKEY_6:
	case LCUIKEY_7:
	case LCUIKEY_8:
	case LCUIKEY_9:
	case LCUIKEY_A:
	case LCUIKEY_B:
	case LCUIKEY_C:
	case LCUIKEY_D:
	case LCUIKEY_E:
	case LCUIKEY_F:
	case LCUIKEY_G:
	case LCUIKEY_H:
	case LCUIKEY_I:
	case LCUIKEY_J:
	case LCUIKEY_K:
	case LCUIKEY_L:
	case LCUIKEY_M:
	case LCUIKEY_N:
	case LCUIKEY_O:
	case LCUIKEY_P:
	case LCUIKEY_Q:
	case LCUIKEY_R:
	case LCUIKEY_S:
	case LCUIKEY_T:
	case LCUIKEY_U:
	case LCUIKEY_V:
	case LCUIKEY_W:
	case LCUIKEY_X:
	case LCUIKEY_Y:
	case LCUIKEY_Z:
	case 189:
	case 187:
	case 188:
	case 190:
	case 191:
	case 222:
	case 186:
	case 220:
	case 221:
	case 219:
#endif
	return TRUE;
	default:break;
	}
	return FALSE;
}

static void IME_SetTarget( LCUI_Widget widget )
{
	default_ime.target = widget;
}

static LCUI_Widget IME_GetTarget( void )
{
	return default_ime.target;
}

static void IME_ClearTarget( void )
{
	default_ime.target = NULL;
}

static void IME_ToText( char ch )
{
	wchar_t text[2];
	
#ifdef LCUI_BUILD_IN_WIN32
	/* 回车符转换行符 */
	if( ch == '\r' ) {
		ch = '\n';
	}
#endif
	text[0] = ch;
	text[1] = '\0';
	DEBUG_MSG("%S, %d\n", text, ch);
	LCUIIME_Commit( text, 2 ); // 直接提交该字符
}


#ifdef LCUI_BUILD_IN_WIN32

static void WinIME_OnChar( LCUI_Event e, void *arg )
{
	MSG *msg = arg;
	wchar_t text[2];
	text[0] = msg->wParam;
	text[1] = 0;
	LCUIIME_Commit( text, 2 );
}

#endif

/**
 * 输入法被打开时的处理
 * 可以在输入法被打开时，初始化相关数据，链接至词库什么的
 **/
static LCUI_BOOL IME_Open(void)
{
#ifdef LCUI_BUILD_IN_WIN32
	LCUI_BindSysEvent( WM_CHAR, WinIME_OnChar, NULL, NULL );
#endif
	return TRUE;
}

/** 输入法被关闭时的处理 */
static LCUI_BOOL IME_Close(void)
{
#ifdef LCUI_BUILD_IN_WIN32
	LCUI_UnbindSysEvent( WM_CHAR, WinIME_OnChar );
#endif
	return TRUE;
}

/** 注册LCUI默认的输入法 */
static int LCUI_RegisterDefaultIME( void )
{
	LCUI_IMEHandlerRec handler;
	handler.gettarget = IME_GetTarget;
	handler.settarget = IME_SetTarget;
	handler.cleartarget = IME_ClearTarget;
	handler.prockey = IME_ProcessKey;
	handler.totext = IME_ToText;
	handler.close = IME_Close;
	handler.open = IME_Open;
	return LCUIIME_Register( "LCUI Input Method", &handler );
}
