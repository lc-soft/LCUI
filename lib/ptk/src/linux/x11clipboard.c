/*
 * lib/ptk/src/linux/x11clipboard.c: clipboard support for linux x11
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

/*
 *
 * References:
 * - https://github.com/exebook/x11clipboard/blob/master/x11paste.c
 * - https://github.com/edrosten/x_clipboard/blob/master/paste.cc
 * -
 * https://github.com/libsdl-org/SDL/blob/515b7e93b5af3dc552a7974ed7245a0e0456ae2a/src/video/x11/SDL_x11events.c
 * -
 * https://github.com/libsdl-org/SDL/blob/main/src/video/x11/SDL_x11clipboard.c
 * -
 * https://github.com/GNOME/gtk/blob/b539c92312d449f41710e6930aaf086454d667d2/gdk/x11/gdkclipboard-x11.c
 * -
 * https://github.com/godotengine/godot/blob/a7011fa29488f5356949667eb8e2b296cbbd9923/ptk/linuxbsd/display_server_x11.cpp
 * - https://www.jwz.org/doc/x-cut-and-paste.html
 *
 */

#include <yutil.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>

#include "ptk/events.h"
#include "x11app.h"
#include "fbapp.h"
#include "x11clipboard.h"

#if defined(PTK_LINUX) && defined(PTK_HAS_LIBX11)

#define CLIPBOARD_TIMEOUT 1000

typedef struct ptk_clipboard_action {
        void *arg;
        ptk_clipboard_callback_t callback;
        bool running;
} ptk_clipboard_action_t;

// @WhoAteDaCake
// TODO: do we need a mutex here? As the action is unlikely to happen
// often, safety seems to be a bigger priority than speed
static struct ptk_x11clipboard_module_t {
        char *text;
        size_t text_len;
        ptk_clipboard_action_t *action;
        //
        Atom xclipboard;
        Atom xprimary;
        Atom xutf8_string;
        Atom xa_string;
        Atom xa_targets;
        Atom xa_text;

        int timer;
} ptk_x11clipboard;

void ptk_x11clipboard_notify(ptk_clipboard_t *cb)
{
        ptk_clipboard_action_t *action = ptk_x11clipboard.action;

        if (!action->running) {
                logger_debug("Tried to ptk_x11clipboard_notify before "
                             "copying started\n");
                return;
        }
        action->callback(cb, action->arg);
        // Reset properties, so if something goes wrongly, we crash
        // instead of having undefined behaviour
        action->arg = NULL;
        action->callback = NULL;
        action->running = false;
}

void ptk_x11clipboard_execute_action(void)
{
        ptk_clipboard_t clipboard_data = { 0 };
        size_t len = ptk_x11clipboard.text_len + 1;
        wchar_t *wstr = malloc(sizeof(wchar_t) * len);

        len = decode_utf8(wstr, ptk_x11clipboard.text, len);
        wstr[len] = 0;
        // Assign the data
        clipboard_data.text = wstr;
        clipboard_data.len = len;
        clipboard_data.image = NULL;
        ptk_clear_timeout(ptk_x11clipboard.timer);
        ptk_x11clipboard_notify(&clipboard_data);
        ptk_x11clipboard.timer = 0;
        free(wstr);
}

void ptk_x11clipboard_request_timeout(void *arg)
{
        ptk_clipboard_t clipboard_data = { 0 };

        logger_debug("action timed out\n");
        ptk_x11clipboard_notify(&clipboard_data);
}

/**
 * This function assumes that text pointer is only owned by clipboard
 */
int ptk_x11clipboard_set_text(const wchar_t *text, size_t len)
{
        Display *display = ptk_x11_get_display();
        Window window = ptk_x11_get_main_window();
        Window clipboard_owner =
            XGetSelectionOwner(display, ptk_x11clipboard.xclipboard);

        // X11 doesn't support wchar_t, so we need to send it regular char
        size_t raw_len = encode_utf8(NULL, text, 0);
        char *raw_text = malloc((raw_len + 1) * sizeof(char));

        raw_len = encode_utf8(raw_text, text, raw_len);
        raw_text[raw_len] = '\0';
        if (raw_len == -1) {
                logger_debug("Failed converting wchar_t* to char*\n");
                // Something failed here, should probably add debug message
                return -1;
        }
        // Make sure to free old text
        if (ptk_x11clipboard.text) {
                free(ptk_x11clipboard.text);
        }

        logger_debug("Copying text to clipboard\n");
        if (ptk_x11clipboard.text) {
                free(ptk_x11clipboard.text);
        }
        ptk_x11clipboard.text = raw_text;
        ptk_x11clipboard.text_len = raw_len;
        if (clipboard_owner == window) {
                logger_debug("Clipboard already owned\n");
                return -2;
        }
        XSetSelectionOwner(display, XA_PRIMARY, window, CurrentTime);
        XSetSelectionOwner(display, ptk_x11clipboard.xclipboard, window,
                           CurrentTime);
        logger_debug("Taken ownership of the clipboard\n");
        return 0;
}

int ptk_x11clipboard_request_text(ptk_clipboard_callback_t callback, void *arg)
{
        Display *display = ptk_x11_get_display();
        Window window = ptk_x11_get_main_window();
        Atom XSEL_DATA = XInternAtom(display, "XSEL_DATA", false);
        Window clipboard_owner =
            XGetSelectionOwner(display, ptk_x11clipboard.xclipboard);
        ptk_clipboard_action_t *action = ptk_x11clipboard.action;

        if (action->running) {
                logger_debug("Tried to paste, while a paste was in progress\n");
                return 1;
        }
        action->arg = arg;
        action->callback = callback;
        action->running = true;

        // No need to continue, we should have stored text already
        // when copy was done
        // This branch will only get executed once we implement copy event
        if (clipboard_owner == window) {
                logger_debug("Clipboard owned by self\n");
                ptk_x11clipboard_execute_action();
                return 0;
        }
        if (clipboard_owner == None) {
                logger_debug("Clipboard owner not found\n");
                return 1;
        }
        // @WhoAteDaCake
        // TODO: needs error handling if we can't access the clipboard?
        // TODO: some other implementations will try XA_STRING if no text was
        // retrieved from UTF8_STRING, however, our implementation is not
        // synchronous, so not sure how it would work, it needs further
        // investigation
        ptk_set_timeout(CLIPBOARD_TIMEOUT, ptk_x11clipboard_request_timeout,
                        NULL);
        XConvertSelection(display, ptk_x11clipboard.xclipboard,
                          ptk_x11clipboard.xutf8_string, XSEL_DATA, window,
                          CurrentTime);
        logger_debug("Clipboard content requested, expect SelectionNotify\n");
        return 0;
}

static void ptk_x11clipboard_on_notify(ptk_native_event_t *ev, void *arg)
{
        XEvent *x_ev = arg;
        ptk_clipboard_action_t *action = ptk_x11clipboard.action;

        if (!action->running) {
                logger_debug("SelectionNotify received, with no action\n");
                return;
        }
        logger_debug("SelectionNotify received\n");

        if (x_ev->xselection.selection == ptk_x11clipboard.xclipboard) {
                logger_debug("Received Clipboard event\n");
                unsigned long N, size;
                char *data;
                Atom target;
                int format;
                XGetWindowProperty(
                    x_ev->xselection.display, x_ev->xselection.requestor,
                    x_ev->xselection.property, 0L, (~0L), 0, AnyPropertyType,
                    &target, &format, &size, &N, (unsigned char **)&data);
                if (target == ptk_x11clipboard.xutf8_string ||
                    target == ptk_x11clipboard.xa_string) {
                        ptk_x11clipboard.text = strndup(data, size);
                        ptk_x11clipboard.text_len = size;
                        XFree(data);
                }
                XDeleteProperty(x_ev->xselection.display,
                                x_ev->xselection.requestor,
                                x_ev->xselection.property);
                ptk_x11clipboard_execute_action();
        }
}

static void ptk_x11clipboard_on_clear(ptk_native_event_t *ev, void *arg)
{
        // @WhoAteDaCake
        // TODO: validate against timestamp of event
        // if it has come too late, we should ignore event
        logger_debug("Lost ownership of the clipboard\n");
        if (ptk_x11clipboard.text) {
                free(ptk_x11clipboard.text);
        }
        ptk_x11clipboard.text = NULL;
}

// This function gets called when another process requests for the
// text we copied in our clipboard
static void ptk_x11clipboard_on_request(ptk_native_event_t *ev, void *arg)
{
        XEvent *x_ev = ev;
        XEvent reply;

        logger_debug("Received SelectionRequest\n");
        // Generates a refuse request by default
        reply.xselection.type = SelectionNotify;
        reply.xselection.requestor = x_ev->xselectionrequest.requestor;
        reply.xselection.selection = x_ev->xselectionrequest.selection;
        reply.xselection.target = x_ev->xselectionrequest.target;
        reply.xselection.property = None;
        // ^ None means refusal
        reply.xselection.time = x_ev->xselectionrequest.time;

        if (x_ev->xselectionrequest.target == ptk_x11clipboard.xa_targets) {
                logger_debug("Requested supported targets\n");
                Atom target_list[4];
                target_list[0] = ptk_x11clipboard.xa_targets;
                target_list[1] = ptk_x11clipboard.xa_text;
                target_list[2] = ptk_x11clipboard.xutf8_string;
                target_list[3] = ptk_x11clipboard.xa_string;

                reply.xselection.property = x_ev->xselectionrequest.property;
                XChangeProperty(x_ev->xselection.display,
                                x_ev->xselectionrequest.requestor,
                                reply.xselection.property, XA_ATOM, 32,
                                PropModeReplace, (unsigned char *)&target_list,
                                sizeof(target_list) / sizeof(target_list[0]));
        } else if (reply.xselection.target == ptk_x11clipboard.xa_text ||
                   reply.xselection.target == ptk_x11clipboard.xutf8_string ||
                   reply.xselection.target == ptk_x11clipboard.xa_string) {
                if (!ptk_x11clipboard.text) {
                        logger_debug(
                            "SelectionRequest received with no text\n");
                } else {
                        logger_debug("Requested for clipboard text\n");
                        reply.xselection.property =
                            x_ev->xselectionrequest.property;
                        XChangeProperty(x_ev->xselection.display,
                                        x_ev->xselectionrequest.requestor,
                                        reply.xselection.property,
                                        reply.xselection.target, 8,
                                        PropModeReplace,
                                        (unsigned char *)ptk_x11clipboard.text,
                                        ptk_x11clipboard.text_len);
                }
        }
        XSendEvent(x_ev->xselection.display, x_ev->xselectionrequest.requestor,
                   true, 0, &reply);
}

// @WhoAteDaCake
// TODO: Implement INCR retrieval
void ptk_x11clipboard_init(void)
{
        Display *display = ptk_x11_get_display();

        // Allocate action once
        ptk_x11clipboard.action = malloc(sizeof(ptk_clipboard_action_t));
        ptk_x11clipboard.action->running = false;
        // Set atoms, that will be re-used
        ptk_x11clipboard.xclipboard = XInternAtom(display, "CLIPBOARD", false);
        ptk_x11clipboard.xprimary = XA_PRIMARY;
        ptk_x11clipboard.xutf8_string =
            XInternAtom(display, "UTF8_STRING", false);
        ptk_x11clipboard.xa_string = XA_STRING;
        ptk_x11clipboard.xa_targets = XInternAtom(display, "TARGETS", false);
        ptk_x11clipboard.xa_text = XInternAtom(display, "TEXT", false);
        ptk_x11clipboard.timer = 0;

        ptk_on_native_event(SelectionNotify, ptk_x11clipboard_on_notify, NULL);
        ptk_on_native_event(SelectionClear, ptk_x11clipboard_on_clear, NULL);
        ptk_on_native_event(SelectionRequest, ptk_x11clipboard_on_request,
                            NULL);
}

void ptk_x11clipboard_destroy(void)
{
        free(ptk_x11clipboard.action);
        ptk_off_native_event(SelectionNotify, ptk_x11clipboard_on_notify);
        ptk_off_native_event(SelectionClear, ptk_x11clipboard_on_clear);
        ptk_off_native_event(SelectionRequest, ptk_x11clipboard_on_request);
        if (ptk_x11clipboard.timer) {
                ptk_clear_timeout(ptk_x11clipboard.timer);
        }
        ptk_x11clipboard.timer = 0;
}

#endif
