/*
 * lib/ptk/src/linux/mouse.c: -- Mouse support for linux.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk.h"

#ifdef PTK_LINUX
#include <yutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread.h>

static struct linux_mouse {
        int x;
        int y;
        int button_state[2];

        int dev_fd;
        const char *dev_path;

        thread_t tid;
        bool active;
} linux_mouse;

static void linux_dispatch_mouse_button_event(int button, int state)
{
        ptk_event_t ev = { 0 };

        if (button < 1 || button > 2) {
                return;
        }
        if (linux_mouse.button_state[button - 1]) {
                if (!(state & button)) {
                        ev.type = PTK_EVENT_MOUSEUP;
                        ev.mouse.x = linux_mouse.x;
                        ev.mouse.y = linux_mouse.y;
                        ev.mouse.button = button;
                        linux_mouse.button_state[button - 1] = 0;
                        ptk_post_event(&ev);
                        ptk_event_destroy(&ev);
                }
        } else if (state & button) {
                ev.type = PTK_EVENT_MOUSEDOWN;
                ev.mouse.x = linux_mouse.x;
                ev.mouse.y = linux_mouse.y;
                ev.mouse.button = button;
                linux_mouse.button_state[button - 1] = 1;
                ptk_post_event(&ev);
                ptk_event_destroy(&ev);
        }
}

static void linux_mouse_thread(void *arg)
{
        char buf[6];
        int state;
        fd_set readfds;
        struct timeval tv;
        ptk_event_t ev = { 0 };

        while (linux_mouse.active) {
                tv.tv_sec = 0;
                tv.tv_usec = 500000;
                FD_ZERO(&readfds);
                FD_SET(linux_mouse.dev_fd, &readfds);
                select(linux_mouse.dev_fd + 1, &readfds, NULL, NULL, &tv);
                if (!FD_ISSET(linux_mouse.dev_fd, &readfds)) {
                        continue;
                }
                if (read(linux_mouse.dev_fd, buf, 6) <= 0) {
                        continue;
                }
                state = buf[0] & 0x07;
                linux_mouse.x += buf[1];
                linux_mouse.y -= buf[2];
                linux_mouse.x = y_max(0, linux_mouse.x);
                linux_mouse.y = y_max(0, linux_mouse.y);
                linux_mouse.x = y_min(ptk_screen_get_width(), linux_mouse.x);
                linux_mouse.y = y_min(ptk_screen_get_height(), linux_mouse.y);
                ev.type = PTK_EVENT_MOUSEMOVE;
                ev.mouse.x = linux_mouse.x;
                ev.mouse.y = linux_mouse.y;
                ptk_post_event(&ev);
                ptk_event_destroy(&ev);
                linux_dispatch_mouse_button_event(MOUSE_BUTTON_LEFT, state);
                linux_dispatch_mouse_button_event(MOUSE_BUTTON_RIGHT, state);
        }
}

int ptk_linux_mouse_init(void)
{
        linux_mouse.active = true;
        linux_mouse.x = ptk_screen_get_width() / 2;
        linux_mouse.y = ptk_screen_get_height() / 2;
        linux_mouse.dev_path = getenv("LCUI_MOUSE_DEVICE");
        if (!linux_mouse.dev_path) {
                linux_mouse.dev_path = "/dev/input/mice";
        }
        logger_debug("[input] open mouse device: %s\n", linux_mouse.dev_path);
        if ((linux_mouse.dev_fd = open(linux_mouse.dev_path, O_RDONLY)) < 0) {
                logger_error("[input] open mouse device failed\n");
                return -1;
        }
        thread_create(&linux_mouse.tid, linux_mouse_thread, NULL);
        logger_debug("[input] mouse driver thread: %lld\n", linux_mouse.tid);
        return 0;
}

int ptk_linux_mouse_destroy(void)
{
        if (linux_mouse.active) {
                linux_mouse.active = false;
                thread_join(linux_mouse.tid, NULL);
                close(linux_mouse.dev_fd);
        }
        return 0;
}

#endif
