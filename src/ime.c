/* ime.c -- Input Method Engine
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

/* FIXME: redesign the input method engine
 * The current design used is not robust and needs to be redesigned with
 * reference to other complete input method engine frameworks.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/ime.h>

typedef struct LCUI_IMERec_ {
	int id;
	char *name;
	LCUI_IMEHandlerRec handler;
	LinkedListNode node;
} LCUI_IMERec, *LCUI_IME;

static struct LCUI_InputMethodEngine {
	int id_count;
	LinkedList list;

	LCUI_IME ime;
	LCUI_BOOL enable_caps_lock;
	LCUI_BOOL active;
} self;

static LCUI_IME LCUIIME_Find(int ime_id)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &self.list)) {
		LCUI_IME ime = node->data;
		if (ime->id == ime_id) {
			return ime;
		}
	}
	return NULL;
}

static LCUI_IME LCUIIME_FindByName(const char *name)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &self.list)) {
		LCUI_IME ime = node->data;
		if (strcmp(ime->name, name) == 0) {
			return ime;
		}
	}
	return NULL;
}

int LCUIIME_Register(const char *name, LCUI_IMEHandler handler)
{
	size_t len;
	LCUI_IME ime;
	if (!self.active) {
		return -1;
	}
	if (LCUIIME_FindByName(name)) {
		return -2;
	}
	ime = NEW(LCUI_IMERec, 1);
	if (!ime) {
		return -ENOMEM;
	}
	len = strlen(name) + 1;
	ime->name = malloc(len * sizeof(char));
	if (!ime->name) {
		return -ENOMEM;
	}
	self.id_count += 1;
	ime->id = self.id_count;
	ime->node.data = ime;
	strncpy(ime->name, name, len);
	memcpy(&ime->handler, handler, sizeof(LCUI_IMEHandlerRec));
	LinkedList_AppendNode(&self.list, &ime->node);
	return ime->id;
}

static LCUI_BOOL LCUIIME_Open(LCUI_IME ime)
{
	if (ime && ime->handler.open) {
		return ime->handler.open();
	}
	return FALSE;
}

static LCUI_BOOL LCUIIME_Close(LCUI_IME ime)
{
	if (ime && ime->handler.close) {
		return ime->handler.open();
	}
	return FALSE;
}

LCUI_BOOL LCUIIME_Select(int ime_id)
{
	LCUI_IME ime = LCUIIME_Find(ime_id);
	if (ime) {
		LCUIIME_Close(self.ime);
		self.ime = ime;
		LCUIIME_Open(self.ime);
		return TRUE;
	}
	return FALSE;
}

LCUI_BOOL LCUIIME_SelectByName(const char *name)
{
	LCUI_IME ime = LCUIIME_FindByName(name);
	if (ime) {
		LCUIIME_Close(self.ime);
		self.ime = ime;
		LCUIIME_Open(self.ime);
		return TRUE;
	}
	return FALSE;
}

void LCUIIME_Switch(void)
{
	LCUI_IME ime;
	if (self.ime && self.ime->node.next) {
		ime = self.ime->node.next->data;
		LCUIIME_Close(self.ime);
		self.ime = ime;
		LCUIIME_Open(self.ime);
	}
}

static void LCUIIME_OnDestroy(void *arg)
{
	LCUI_IME ime = arg;
	if (self.ime == ime) {
		self.ime = NULL;
	}
	free(ime->name);
	free(ime);
}

LCUI_BOOL LCUIIME_CheckCharKey(int key)
{
	switch (key) {
	case LCUI_KEY_TAB:
	case LCUI_KEY_ENTER:
	case LCUI_KEY_SPACE:
	case LCUI_KEY_SEMICOLON:
	case LCUI_KEY_MINUS:
	case LCUI_KEY_EQUAL:
	case LCUI_KEY_COMMA:
	case LCUI_KEY_PERIOD:
	case LCUI_KEY_SLASH:
	case LCUI_KEY_BRACKETLEFT:
	case LCUI_KEY_BACKSLASH:
	case LCUI_KEY_BRACKETRIGHT:
	case LCUI_KEY_APOSTROPHE:
		return TRUE;
	default:
		if (key >= LCUI_KEY_0 && key <= LCUI_KEY_Z) {
			return TRUE;
		}
		break;
	}
	return FALSE;
}

static int LCUIIME_ToUpperCase(int ch)
{
	if (ch >= 'a' && ch <= 'z') {
		ch = ch - 32;
	} else if (ch >= 'A' && ch <= 'Z') {
		ch = ch + 32;
	} else {
		switch (ch) {
		case '0':
			ch = ')';
			break;
		case '1':
			ch = '!';
			break;
		case '2':
			ch = '@';
			break;
		case '3':
			ch = '#';
			break;
		case '4':
			ch = '$';
			break;
		case '5':
			ch = '%';
			break;
		case '6':
			ch = '^';
			break;
		case '7':
			ch = '&';
			break;
		case '8':
			ch = '*';
			break;
		case '9':
			ch = '(';
			break;
		case '`':
			ch = '~';
			break;
		case '-':
			ch = '_';
			break;
		case '=':
			ch = '+';
			break;
		case '[':
			ch = '{';
			break;
		case ']':
			ch = '}';
			break;
		case '\\':
			ch = '|';
			break;
		case ';':
			ch = ':';
			break;
		case '\'':
			ch = '"';
			break;
		case ',':
			ch = '<';
			break;
		case '.':
			ch = '>';
			break;
		case '/':
			ch = '?';
			break;
		default:
			break;
		}
	}
	return ch;
}

static void LCUIIME_ToText(LCUI_SysEvent e)
{
	int ch = e->key.code;
	switch (ch) {
	case LCUI_KEY_TAB:
		ch = '\t';
		break;
	case LCUI_KEY_ENTER:
		ch = '\n';
		break;
	case LCUI_KEY_SEMICOLON:
		ch = ';';
		break;
	case LCUI_KEY_MINUS:
		ch = '-';
		break;
	case LCUI_KEY_EQUAL:
		ch = '=';
		break;
	case LCUI_KEY_COMMA:
		ch = ',';
		break;
	case LCUI_KEY_PERIOD:
		ch = '.';
		break;
	case LCUI_KEY_SLASH:
		ch = '/';
		break;
	case LCUI_KEY_BRACKETLEFT:
		ch = '[';
		break;
	case LCUI_KEY_BACKSLASH:
		ch = '\\';
		break;
	case LCUI_KEY_BRACKETRIGHT:
		ch = ']';
		break;
	case LCUI_KEY_APOSTROPHE:
		ch = '\'';
		break;
	default:
		/* 如果没开启大写锁定，则将字母转换成小写 */
		if (!self.enable_caps_lock) {
			if (ch >= 'A' && ch <= 'Z') {
				ch = e->key.code + 32;
			}
		}
		break;
	}
	/* 如果shift键处于按下状态 */
	if (LCUIKeyboard_IsHit(LCUI_KEY_SHIFT)) {
		_DEBUG_MSG("hit shift, ch: %c\n", ch);
		ch = LCUIIME_ToUpperCase(ch);
	}
	if ((ch >= 32 && ch <= 126) || ch == '\t' || ch == '\n' || ch == ' ') {
		LCUI_SysEventRec ev = { 0 };
		ev.type = LCUI_KEYPRESS;
		ev.key.code = ch;
		LCUI_TriggerEvent(&ev, NULL);
	}
	DEBUG_MSG("ch = %c\n", ch);
	self.ime->handler.totext(ch);
}

LCUI_BOOL LCUIIME_ProcessKey(LCUI_SysEvent e)
{
	int key_state;
	/* 根据事件类型判定按键状态 */
	if (e->type == LCUI_KEYUP) {
		key_state = LCUI_KSTATE_RELEASE;
		/* 如果是 caps lock 按键被释放 */
		if (e->key.code == LCUI_KEY_CAPITAL) {
			self.enable_caps_lock = !self.enable_caps_lock;
			return FALSE;
		}
	} else {
		key_state = LCUI_KSTATE_PRESSED;
		/* 如果按下的是 shift 键，但没释放，则直接退出 */
		if (e->key.code == LCUI_KEY_SHIFT) {
			return FALSE;
		}
	}
	if (self.ime && self.ime->handler.prockey) {
		return self.ime->handler.prockey(e->key.code, key_state);
	}
	return FALSE;
}

int LCUIIME_Commit(const wchar_t *str, size_t len)
{
	LCUI_SysEventRec sys_ev;
	if (len == 0) {
		len = wcslen(str);
	}
	sys_ev.type = LCUI_TEXTINPUT;
	sys_ev.text.length = len;
	sys_ev.text.text = NEW(wchar_t, len + 1);
	if (!sys_ev.text.text) {
		return -ENOMEM;
	}
	wcsncpy(sys_ev.text.text, str, len + 1);
	LCUI_TriggerEvent(&sys_ev, NULL);
	free(sys_ev.text.text);
	sys_ev.text.text = NULL;
	sys_ev.text.length = 0;
	return 0;
}

static void LCUIIME_OnKeyDown(LCUI_SysEvent e, void *arg)
{
	if (LCUIIME_ProcessKey(e)) {
		LCUIIME_ToText(e);
	}
}

void LCUI_InitIME(void)
{
	LinkedList_Init(&self.list);
	self.active = TRUE;
	LCUI_BindEvent(LCUI_KEYDOWN, LCUIIME_OnKeyDown, NULL, NULL);
#ifdef WINAPI_FAMILY_APP
	return;
#else
#ifdef LCUI_BUILD_IN_WIN32
	LCUIIME_Select(LCUI_RegisterWin32IME());
#else
	LCUIIME_Select(LCUI_RegisterLinuxIME());
#endif
#endif
}

void LCUI_FreeIME(void)
{
	self.active = FALSE;
	LinkedList_ClearData(&self.list, LCUIIME_OnDestroy);
}
