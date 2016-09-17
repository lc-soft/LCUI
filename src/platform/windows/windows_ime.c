
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>

#ifdef LCUI_BUILD_IN_WIN32
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include <LCUI/ime.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H

static struct {
	LCUI_Widget target;
} ime;


/**
判断按键是否需要处理，也可以用于设置当前的输入法，比如：
按shift键，切换输入法的中英文输入，按PageUP和PageDown切换词组页
按backspace键，如果输入的内容为空，则return FALSE;让LCUI将该按键事件发给部件
的相应回调函数处理；否则，return TRUE;并从输入的内容中删除一个字符
**/
static LCUI_BOOL IME_ProcessKey( int key, int key_state )
{
	return FALSE;
}

static void IME_SetTarget( LCUI_Widget widget )
{
	ime.target = widget;
}

static LCUI_Widget IME_GetTarget( void )
{
	return ime.target;
}

static void IME_ClearTarget( void )
{
	ime.target = NULL;
}

static void IME_ToText( char ch )
{
	wchar_t text[2];
	switch( ch ) {
	case '\r': ch = '\n'; break;
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
	default: break;
	}
	text[0] = ch;
	text[1] = '\0';
	DEBUG_MSG("%S, %d\n", text, ch);
	LCUIIME_Commit( text, 2 ); // 直接提交该字符
}

static void WinIME_OnChar( LCUI_Event e, void *arg )
{
	MSG *msg = arg;
	wchar_t text[2];
	text[0] = msg->wParam;
	text[1] = 0;
	LCUIIME_Commit( text, 2 );
}

/**
* 输入法被打开时的处理
* 可以在输入法被打开时，初始化相关数据，链接至词库什么的
**/
static LCUI_BOOL IME_Open(void)
{
	LCUI_BindSysEvent( WM_CHAR, WinIME_OnChar, NULL, NULL );
	return TRUE;
}

/** 输入法被关闭时的处理 */
static LCUI_BOOL IME_Close(void)
{
	LCUI_UnbindSysEvent( WM_CHAR, WinIME_OnChar );
	return TRUE;
}

int LCUI_RegisterWin32IME( void )
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

#endif
