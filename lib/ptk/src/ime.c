/*
 * lib/ptk/src/ime.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <yutil.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ptk/app.h"
#include "ptk/events.h"
#include "ime.h"

typedef struct {
        int id;
        char *name;
        ptk_ime_handler_t handler;
        list_node_t node;
} ptk_ime_t;

static struct ptk_ime {
        int id_count;
        list_t list;

        ptk_ime_t *ime;
        bool enable_caps_lock;
        bool active;
} ptk_ime;

static ptk_ime_t *ptk_ime_get(int ptk_ime_id)
{
        list_node_t *node;
        for (list_each(node, &ptk_ime.list)) {
                ptk_ime_t *ime = node->data;
                if (ime->id == ptk_ime_id) {
                        return ime;
                }
        }
        return NULL;
}

static ptk_ime_t *ptk_ime_get_by_name(const char *name)
{
        list_node_t *node;

        for (list_each(node, &ptk_ime.list)) {
                ptk_ime_t *ime = node->data;
                if (strcmp(ime->name, name) == 0) {
                        return ime;
                }
        }
        return NULL;
}

int ptk_ime_add(const char *name, ptk_ime_handler_t *handler)
{
        size_t len;
        ptk_ime_t *ime;

        if (!ptk_ime.active) {
                return -1;
        }
        if (ptk_ime_get_by_name(name)) {
                return -2;
        }
        ime = malloc(sizeof(ptk_ime_t));
        if (!ime) {
                return -ENOMEM;
        }
        len = strlen(name) + 1;
        ime->name = malloc(len * sizeof(char));
        if (!ime->name) {
                return -ENOMEM;
        }
        ptk_ime.id_count += 1;
        ime->id = ptk_ime.id_count;
        ime->node.data = ime;
        strncpy(ime->name, name, len);
        memcpy(&ime->handler, handler, sizeof(ptk_ime_handler_t));
        list_append_node(&ptk_ime.list, &ime->node);
        return ime->id;
}

static bool ptk_ime_open(ptk_ime_t *ime)
{
        if (ime && ime->handler.open) {
                return ime->handler.open();
        }
        return false;
}

static bool ptk_ime_close(ptk_ime_t *ime)
{
        if (ime && ime->handler.close) {
                logger_debug("[ime] close engine: %s\n", ime->name);
                return ime->handler.close();
        }
        return false;
}

bool ptk_ime_select(int id)
{
        ptk_ime_t *ime = ptk_ime_get(id);
        if (ime) {
                ptk_ime_close(ptk_ime.ime);
                logger_debug("[ime] select engine: %s\n", ime->name);
                ptk_ime.ime = ime;
                ptk_ime_open(ptk_ime.ime);
                return true;
        }
        return false;
}

bool ptk_ime_select_by_name(const char *name)
{
        ptk_ime_t *ime = ptk_ime_get_by_name(name);
        if (ime) {
                ptk_ime_close(ptk_ime.ime);
                ptk_ime.ime = ime;
                ptk_ime_open(ptk_ime.ime);
                return true;
        }
        return false;
}

void ptk_ime_switch(void)
{
        ptk_ime_t *ime;
        if (ptk_ime.ime && ptk_ime.ime->node.next) {
                ime = ptk_ime.ime->node.next->data;
                ptk_ime_close(ptk_ime.ime);
                ptk_ime.ime = ime;
                ptk_ime_open(ptk_ime.ime);
        }
}

static void ptk_ime_on_destroy(void *arg)
{
        ptk_ime_t *ime = arg;
        if (ptk_ime.ime == ime) {
                ptk_ime.ime = NULL;
        }
        free(ime->name);
        free(ime);
}

bool ptk_ime_check_char_key(int key)
{
        switch (key) {
        case KEY_TAB:
        case KEY_ENTER:
        case KEY_SPACE:
        case KEY_SEMICOLON:
        case KEY_MINUS:
        case KEY_EQUAL:
        case KEY_COMMA:
        case KEY_PERIOD:
        case KEY_SLASH:
        case KEY_BRACKETLEFT:
        case KEY_BACKSLASH:
        case KEY_BRACKETRIGHT:
        case KEY_APOSTROPHE:
                return true;
        default:
                if (key >= KEY_0 && key <= KEY_Z) {
                        return true;
                }
                break;
        }
        return false;
}

static void ptk_ime_to_text(ptk_event_t *e)
{
        ptk_ime.ime->handler.totext(e->key.code);
}

bool ptk_ime_process_key(ptk_event_t *e)
{
        bool is_presed = false;

        /* 根据事件类型判定按键状态 */
        if (e->type == PTK_EVENT_KEYUP) {
                /* 如果是 caps lock 按键被释放 */
                if (e->key.code == KEY_CAPITAL) {
                        ptk_ime.enable_caps_lock = !ptk_ime.enable_caps_lock;
                        return false;
                }
        } else {
                is_presed = true;
                /* 如果按下的是 shift 键，但没释放，则直接退出 */
                if (e->key.code == KEY_SHIFT) {
                        return false;
                }
        }
        if (ptk_ime.ime && ptk_ime.ime->handler.prockey) {
                return ptk_ime.ime->handler.prockey(e->key.code, is_presed);
        }
        return false;
}

int ptk_ime_commit(const wchar_t *str, size_t len)
{
        ptk_event_t e;

        ptk_composition_event_init(&e, str, len);
        ptk_process_event(&e);
        ptk_event_destroy(&e);
        return 0;
}

static void ptk_ime_on_keydown(ptk_event_t *e, void *arg)
{
        if (ptk_ime_process_key(e)) {
                ptk_ime_to_text(e);
        }
}

void ptk_ime_set_caret(int x, int y)
{
        if (ptk_ime.ime && ptk_ime.ime->handler.setcaret) {
                ptk_ime.ime->handler.setcaret(x, y);
        }
}

void ptk_ime_init(void)
{
        list_create(&ptk_ime.list);
        ptk_ime.active = true;
        ptk_on_event(PTK_EVENT_KEYDOWN, ptk_ime_on_keydown, NULL);
#ifdef PTK_UWP
        return;
#else
#ifdef PTK_WIN_DESKTOP
        ptk_ime_select(ptk_ime_add_win32());
#else
        ptk_ime_select(ptk_ime_add_linux());
#endif
#endif
}

void ptk_ime_destroy(void)
{
        ptk_ime.active = false;
        ptk_ime_close(ptk_ime.ime);
        list_destroy_without_node(&ptk_ime.list, ptk_ime_on_destroy);
}
