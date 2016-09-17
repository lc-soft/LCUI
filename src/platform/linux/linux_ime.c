
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>

#ifdef LCUI_BUILD_IN_LINUX
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include <LCUI/ime.h>
#include <LCUI/platform.h>
#include LCUI_EVENTS_H


static struct {
	LCUI_Widget target;
} ime;

static LCUI_BOOL X11IME_ProcessKey( int key, int key_state )
{
	if( key_state != LCUIKEYSTATE_PRESSED ) {
		return FALSE;
	}
	_DEBUG_MSG("key: %d\n", key);
	if( key == LCUIKEY_ENTER || (key >= ' ' && key <= '~') ) {
	    	return TRUE;

	}
	return FALSE;
}

static void X11IME_SetTarget( LCUI_Widget widget )
{
	ime.target = widget;
}

static LCUI_Widget X11IME_GetTarget( void )
{
	return ime.target;
}

static void X11IME_ClearTarget( void )
{
	ime.target = NULL;
}

static void X11IME_ToText( char ch )
{
	wchar_t text[2];
	text[0] = ch;
	text[1] = '\0';
	_DEBUG_MSG("%S, %d\n", text, ch);
	LCUIIME_Commit( text, 2 );
}

static LCUI_BOOL X11IME_Open( void )
{
	return TRUE;
}

static LCUI_BOOL X11IME_Close( void )
{
	return TRUE;
}

int LCUI_RegisterLinuxIME( void )
{
	LCUI_IMEHandlerRec handler;
	LCUI_BOOL is_x11_mode = TRUE;
	if( is_x11_mode ) {
		handler.gettarget = X11IME_GetTarget;
		handler.settarget = X11IME_SetTarget;
		handler.cleartarget = X11IME_ClearTarget;
		handler.prockey = X11IME_ProcessKey;
		handler.totext = X11IME_ToText;
		handler.close = X11IME_Close;
		handler.open = X11IME_Open;
		return LCUIIME_Register( "LCUI Input Method", &handler );
	}
	return -1;
}

#endif
