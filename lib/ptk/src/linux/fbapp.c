/*
 * lib/ptk/src/linux/fbapp.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk.h"

#ifdef PTK_LINUX

#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pandagl.h>

#define MIN_WIDTH 320
#define MIN_HEIGHT 240

struct ptk_window {
        int x;
        int y;
        int width;
        int height;
        int min_width, min_height;
        int max_width, max_height;

        pd_rect_t rect;
        pd_rect_t actual_rect;
        pd_canvas_t canvas;
        list_t rects;
};

typedef struct linux_fb {
        int dev_fd;
        const char *dev_path;

        unsigned char *mem;
        size_t mem_len;

        struct fb_var_screeninfo var_info;
        struct fb_fix_screeninfo fix_info;
        struct fb_cmap cmap;
} linux_frame_buffer_t;

static struct ptk_fbapp {
        linux_frame_buffer_t fb;

        unsigned screen_width;
        unsigned screen_height;

        ptk_window_t window;
        unsigned window_count;
        int exit_code;

        bool active;
        bool should_quit_loop;
        pd_canvas_t canvas;
} fbapp;

static void ptk_fb_window_set_size(ptk_window_t *wnd, int width, int height);

static void ptk_fbapp_print_info(void)
{
        char visual[256], type[256];

        switch (fbapp.fb.fix_info.type) {
        case FB_TYPE_PACKED_PIXELS:
                strcpy(type, "packed pixels");
                break;
        case FB_TYPE_PLANES:
                strcpy(type, "non interleaved planes");
                break;
        case FB_TYPE_INTERLEAVED_PLANES:
                strcpy(type, "interleaved planes");
                break;
        case FB_TYPE_TEXT:
                strcpy(type, "text/attributes");
                break;
        case FB_TYPE_VGA_PLANES:
                strcpy(type, "EGA/VGA planes");
                break;
        default:
                strcpy(type, "unkown");
                break;
        }
        switch (fbapp.fb.fix_info.visual) {
        case FB_VISUAL_MONO01:
                strcpy(visual, "Monochr. 1=Black 0=White");
                break;
        case FB_VISUAL_MONO10:
                strcpy(visual, "Monochr. 1=White 0=Black");
                break;
        case FB_VISUAL_TRUECOLOR:
                strcpy(visual, "true color");
                break;
        case FB_VISUAL_PSEUDOCOLOR:
                strcpy(visual, "pseudo color (like atari)");
                break;
        case FB_VISUAL_DIRECTCOLOR:
                strcpy(visual, "direct color");
                break;
        case FB_VISUAL_STATIC_PSEUDOCOLOR:
                strcpy(visual, "pseudo color readonly");
                break;
        default:
                strcpy(type, "unkown");
                break;
        }
        logger_debug(
            "============== screen info =============\n"
            "FB mem start  : 0x%08lX\n"
            "FB mem length : %d\n"
            "FB type       : %s\n"
            "FB visual     : %s\n"
            "accel         : %d\n"
            "geometry      : %d %d %d %d %d\n"
            "timings       : %d %d %d %d %d %d\n"
            "rgba          : %d/%d, %d/%d, %d/%d, %d/%d\n"
            "========================================\n",
            fbapp.fb.fix_info.smem_start, fbapp.fb.fix_info.smem_len, type,
            visual, fbapp.fb.fix_info.accel, fbapp.fb.var_info.xres,
            fbapp.fb.var_info.yres, fbapp.fb.var_info.xres_virtual,
            fbapp.fb.var_info.yres_virtual, fbapp.fb.var_info.bits_per_pixel,
            fbapp.fb.var_info.upper_margin, fbapp.fb.var_info.lower_margin,
            fbapp.fb.var_info.left_margin, fbapp.fb.var_info.right_margin,
            fbapp.fb.var_info.hsync_len, fbapp.fb.var_info.vsync_len,
            fbapp.fb.var_info.red.length, fbapp.fb.var_info.red.offset,
            fbapp.fb.var_info.green.length, fbapp.fb.var_info.green.offset,
            fbapp.fb.var_info.blue.length, fbapp.fb.var_info.blue.offset,
            fbapp.fb.var_info.transp.length, fbapp.fb.var_info.transp.offset);
}

static void ptk_fbapp_init_root_window(void)
{
        ptk_window_t *wnd = &fbapp.window;

        pd_canvas_init(&wnd->canvas);
        list_create(&wnd->rects);
        fbapp.window_count = 0;
        wnd->canvas.color_type = PD_COLOR_TYPE_ARGB;
        ptk_fb_window_set_size(&fbapp.window, fbapp.screen_width,
                               fbapp.screen_height);
}

static void ptk_fbapp_destroy_root_window(void)
{
        ptk_window_t *wnd = &fbapp.window;

        list_destroy(&wnd->rects, free);
}

static void ptk_fbapp_init_canvas(void)
{
        fbapp.canvas.width = fbapp.screen_width;
        fbapp.canvas.height = fbapp.screen_height;
        fbapp.canvas.bytes = fbapp.fb.mem;
        fbapp.canvas.bytes_per_row = fbapp.fb.fix_info.line_length;
        fbapp.canvas.mem_size = fbapp.fb.mem_len;
        switch (fbapp.fb.var_info.bits_per_pixel) {
        case 32:
                fbapp.canvas.color_type = PD_COLOR_TYPE_ARGB8888;
                break;
        case 24:
                fbapp.canvas.color_type = PD_COLOR_TYPE_RGB888;
                break;
        case 8:
                ioctl(fbapp.fb.dev_fd, FBIOGETCMAP, &fbapp.fb.cmap);
        default:
                break;
        }
        memset(fbapp.canvas.bytes, 0, fbapp.canvas.mem_size);
}

static ptk_window_t *ptk_fb_window_create(const wchar_t *title, int x, int y,
                                          int width, int height,
                                          ptk_window_t *parent)
{
        if (fbapp.window_count > 0) {
                return NULL;
        }
        fbapp.window_count += 1;
        ptk_fb_window_set_size(&fbapp.window, width, height);
        return &fbapp.window;
}

static int ptk_fb_window_get_width(ptk_window_t *wnd)
{
        return wnd->width;
}

static int ptk_fb_window_get_height(ptk_window_t *wnd)
{
        return wnd->height;
}

static void ptk_fb_window_set_min_width(ptk_window_t *wnd, int min_width)
{
        wnd->min_width = min_width;
}

static void ptk_fb_window_set_min_height(ptk_window_t *wnd, int min_height)
{
        wnd->min_height = min_height;
}

static void ptk_fb_window_set_max_width(ptk_window_t *wnd, int max_width)
{
        wnd->max_width = max_width;
}

static void ptk_fb_window_set_max_height(ptk_window_t *wnd, int max_height)
{
        wnd->max_height = max_height;
}

static void ptk_fb_window_set_size(ptk_window_t *wnd, int width, int height)
{
        wnd->width = y_max(width, MIN_WIDTH);
        wnd->height = y_max(height, MIN_HEIGHT);
        wnd->x = (fbapp.screen_width - wnd->width) / 2;
        wnd->y = (fbapp.screen_height - wnd->height) / 2;
        wnd->rect.x = wnd->x;
        wnd->rect.y = wnd->y;
        wnd->rect.width = wnd->width;
        wnd->rect.height = wnd->height;
        wnd->actual_rect = wnd->rect;
        pd_rect_correct(&wnd->actual_rect, fbapp.screen_width,
                        fbapp.screen_height);
        pd_canvas_create(&wnd->canvas, wnd->width, wnd->height);
}

static void ptk_fb_window_set_position(ptk_window_t *wnd, int x, int y)
{
}

static ptk_window_t *ptk_fbapp_get_window_by_handle(void *handle)
{
        return &fbapp.window;
}

static void ptk_fb_window_set_title(ptk_window_t *wnd, const wchar_t *title)
{
}

static void ptk_fb_window_close(ptk_window_t *wnd)
{
        ptk_event_t e = { 0 };

        fbapp.window_count -= 1;
        e.type = PTK_EVENT_CLOSE;
        e.window = wnd;
        ptk_post_event(&e);
}

static void ptk_fb_window_destroy(ptk_window_t *wnd)
{
}

static void ptk_fb_window_activate(ptk_window_t *wnd)
{
}

static void ptk_fb_window_show(ptk_window_t *wnd)
{
}

static void ptk_fb_window_hide(ptk_window_t *wnd)
{
}

static void *ptk_fb_window_get_handle(ptk_window_t *wnd)
{
        return NULL;
}

static void ptk_fb_window_sync_rect16(pd_canvas_t *canvas, int x, int y)
{
        uint32_t iy, ix;
        pd_rect_t rect;
        pd_color_t *pixel, *pixel_row;
        unsigned char *dst, *dst_row;

        pd_canvas_get_quote_rect(canvas, &rect);
        pixel_row = canvas->argb + rect.y * canvas->width + rect.x;
        dst_row = fbapp.fb.mem + y * fbapp.canvas.bytes_per_row + x * 2;
        for (iy = 0; iy < rect.width; ++iy) {
                dst = dst_row;
                pixel = pixel_row;
                for (ix = 0; ix < rect.width; ++ix) {
                        dst[0] = (pixel->r & 0xF8) | (pixel->b >> 5);
                        dst[1] = ((pixel->g & 0x1C) << 3) | (pixel->b >> 3);
                }
                pixel_row += canvas->width;
                dst_row += fbapp.canvas.bytes_per_row;
        }
}

static void ptk_fb_window_sync_rect8(pd_canvas_t *canvas, int x, int y)
{
        uint32_t ix, iy;
        pd_rect_t rect;
        pd_color_t *pixel, *pixel_row;
        unsigned char *dst, *dst_row;
        unsigned int r, g, b, i;
        struct fb_cmap cmap;
        __u16 cmap_buf[256 * 3] = { 0 };

        cmap.start = 0;
        cmap.len = 255;
        cmap.transp = 0;
        cmap.red = cmap_buf;
        cmap.green = cmap_buf + 256;
        cmap.blue = cmap_buf + 512;

        pd_canvas_get_quote_rect(canvas, &rect);
        pixel_row = canvas->argb + rect.y * canvas->width + rect.x;
        dst_row = fbapp.fb.mem + y * fbapp.canvas.bytes_per_row + x;
        for (iy = 0; iy < rect.height; ++iy) {
                dst = dst_row;
                pixel = pixel_row;
                for (ix = 0; ix < rect.width; ++ix, ++dst) {
                        r = pixel->r * 0.92;
                        g = pixel->g * 0.92;
                        b = pixel->b * 0.92;

                        i = ((r & 0xc0)) + ((g & 0xf0) >> 2) +
                            ((b & 0xc0) >> 6);

                        cmap.red[i] = r * 256;
                        cmap.green[i] = g * 256;
                        cmap.blue[i] = b * 256;
                        *dst = (((r & 0xc0)) + ((g & 0xf0) >> 2) +
                                ((b & 0xc0) >> 6));
                }
                pixel_row += canvas->width;
                dst_row += fbapp.canvas.bytes_per_row;
        }
        ioctl(fbapp.fb.dev_fd, FBIOPUTCMAP, &cmap);
}

static void ptk_fb_window_sync_rect24(pd_canvas_t *canvas, int x, int y)
{
        pd_canvas_replace(&fbapp.canvas, canvas, x, y);
}

static void ptk_fb_window_sync_rect32(pd_canvas_t *canvas, int x, int y)
{
        pd_canvas_replace(&fbapp.canvas, canvas, x, y);
}

/** FIXME: 屏幕上并未显示图形 */
static void ptk_fb_window_sync_rect(ptk_window_t *wnd, pd_rect_t *rect)
{
        int x, y;
        pd_canvas_t canvas;
        pd_rect_t actual_rect;

        /* Get actual write rectangle */
        actual_rect.x = rect->x + wnd->x;
        actual_rect.y = rect->y + wnd->y;
        actual_rect.width = rect->width;
        actual_rect.height = rect->height;
        pd_rect_correct(&actual_rect, fbapp.screen_width, fbapp.screen_height);
        /* Convert this rectangle to surface canvas related rectangle */
        x = actual_rect.x;
        y = actual_rect.y;
        actual_rect.x -= wnd->x;
        actual_rect.y -= wnd->y;
        pd_canvas_init(&canvas);
        /* Use this rectangle as a canvas rectangle to write pixels */
        pd_canvas_quote(&canvas, &wnd->canvas, &actual_rect);
        /* Write pixels to the framebuffer by pixel format */
        switch (fbapp.fb.var_info.bits_per_pixel) {
        case 32:
                ptk_fb_window_sync_rect32(&canvas, x, y);
                break;
        case 24:
                ptk_fb_window_sync_rect24(&canvas, x, y);
                break;
        case 16:
                ptk_fb_window_sync_rect16(&canvas, x, y);
                break;
        case 8:
                ptk_fb_window_sync_rect8(&canvas, x, y);
                break;
        default:
                break;
        }
}

static ptk_window_paint_t *ptk_fb_window_begin_paint(ptk_window_t *wnd,
                                                     pd_rect_t *rect)
{
        ptk_window_paint_t *paint;
        pd_rect_t actual_rect = *rect;
        pd_rect_correct(&actual_rect, wnd->width, wnd->height);
        actual_rect.x += wnd->rect.x;
        actual_rect.y += wnd->rect.y;
        pd_rect_overlap(&actual_rect, &wnd->actual_rect, &actual_rect);
        actual_rect.x -= wnd->rect.x;
        actual_rect.y -= wnd->rect.y;
        paint = pd_context_create(&wnd->canvas, &actual_rect);
        pd_canvas_fill(&paint->canvas, pd_rgb(255, 255, 255));
        pd_rects_add(&wnd->rects, rect);
        return paint;
}

static void ptk_fb_window_end_paint(ptk_window_t *wnd,
                                    ptk_window_paint_t *paint)
{
        pd_context_destroy(paint);
}

static void ptk_fb_window_present(ptk_window_t *wnd)
{
        list_node_t *node;
        list_t rects;

        list_create(&rects);
        list_concat(&rects, &wnd->rects);
        for (list_each(node, &rects)) {
                ptk_fb_window_sync_rect(wnd, node->data);
        }
        list_destroy(&rects, free);
}

static int ptk_fb_screen_get_width(void)
{
        return fbapp.screen_width;
}

static int ptk_fb_screen_get_height(void)
{
        return fbapp.screen_height;
}

static void ptk_fbapp_present(void)
{
}

static int ptk_fbapp_init(const wchar_t *name)
{
        fbapp.fb.dev_path = getenv("LCUI_FRAMEBUFFER_DEVICE");
        if (!fbapp.fb.dev_path) {
                fbapp.fb.dev_path = "/dev/fb0";
        }
        logger_debug("[fb-app] open framebuffer device: %s\n",
                     fbapp.fb.dev_path);
        fbapp.fb.dev_fd = open(fbapp.fb.dev_path, O_RDWR);
        if (fbapp.fb.dev_fd == -1) {
                logger_error(
                    "[fb-app] open framebuffer device failed, errno(%d): %s\n",
                    errno, strerror(errno));
                return -1;
        }
        ioctl(fbapp.fb.dev_fd, FBIOGET_VSCREENINFO, &fbapp.fb.var_info);
        ioctl(fbapp.fb.dev_fd, FBIOGET_FSCREENINFO, &fbapp.fb.fix_info);
        fbapp.screen_width = fbapp.fb.var_info.xres;
        fbapp.screen_height = fbapp.fb.var_info.yres;
        fbapp.fb.mem_len = fbapp.fb.fix_info.smem_len;
        fbapp.fb.mem = mmap(NULL, fbapp.fb.mem_len, PROT_READ | PROT_WRITE,
                            MAP_SHARED, fbapp.fb.dev_fd, 0);
        if ((void *)-1 == fbapp.fb.mem) {
                logger_error("[fb-app] framebuffer mmap failed\n");
                return -2;
        }
        ptk_fbapp_print_info();
        ptk_fbapp_init_canvas();
        ptk_fbapp_init_root_window();
        fbapp.active = true;
        fbapp.should_quit_loop = false;
        return 0;
}

static int ptk_fbapp_destroy(void)
{
        if (!fbapp.active) {
                return -1;
        }
        if (munmap(fbapp.fb.mem, fbapp.fb.mem_len) != 0) {
                perror("[fb-app] framebuffer munmap failed");
        }
        switch (fbapp.fb.var_info.bits_per_pixel) {
        case 8:
                ioctl(fbapp.fb.dev_fd, FBIOPUTCMAP, &fbapp.fb.cmap);
        default:
                pd_canvas_destroy(&fbapp.window.canvas);
                break;
        }
        ptk_fbapp_destroy_root_window();
        close(fbapp.fb.dev_fd);
        fbapp.fb.mem = NULL;
        fbapp.fb.mem_len = 0;
        fbapp.active = false;
        return 0;
}

static int ptk_fb_on_event(int type, ptk_native_event_handler_t handler,
                           void *data)
{
        return -1;
}

static int ptk_fb_off_event(int type, ptk_native_event_handler_t handler)
{
        return -1;
}

static int ptk_fb_process_events(ptk_process_events_option_t option)
{

        fbapp.exit_code = 0;
        if (option == PTK_PROCESS_EVENTS_ONE_IF_PRESENT ||
            option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT) {
                ptk_tick();
                ptk_process_events();
                return fbapp.exit_code;
        }
        do {
                ptk_tick();
                ptk_process_events();
                sleep_ms(1);
        } while (option == PTK_PROCESS_EVENTS_UNTIL_QUIT && !fbapp.should_quit_loop);
        fbapp.should_quit_loop = false;
        return fbapp.exit_code;
}

static void ptk_fbapp_exit(int exit_code)
{
        fbapp.should_quit_loop = true;
        fbapp.exit_code = exit_code;
}

void ptk_fbapp_driver_init(ptk_app_driver_t *driver)
{
        driver->init = ptk_fbapp_init;
        driver->destroy = ptk_fbapp_destroy;
        driver->on_event = ptk_fb_on_event;
        driver->off_event = ptk_fb_off_event;
        driver->process_events = ptk_fb_process_events;
        driver->get_screen_width = ptk_fb_screen_get_width;
        driver->get_screen_height = ptk_fb_screen_get_height;
        driver->create_window = ptk_fb_window_create;
        driver->get_window = ptk_fbapp_get_window_by_handle;
        driver->present = ptk_fbapp_present;
        driver->exit = ptk_fbapp_exit;
}

void ptk_fbwindow_driver_init(ptk_window_driver_t *driver)
{
        driver->close = ptk_fb_window_close;
        driver->close = ptk_fb_window_destroy;
        driver->show = ptk_fb_window_show;
        driver->hide = ptk_fb_window_hide;
        driver->activate = ptk_fb_window_activate;
        driver->set_title = ptk_fb_window_set_title;
        driver->set_size = ptk_fb_window_set_size;
        driver->set_position = ptk_fb_window_set_position;
        driver->get_width = ptk_fb_window_get_width;
        driver->get_height = ptk_fb_window_get_height;
        driver->get_handle = ptk_fb_window_get_handle;
        driver->set_max_width = ptk_fb_window_set_max_width;
        driver->set_max_height = ptk_fb_window_set_max_height;
        driver->set_min_width = ptk_fb_window_set_min_width;
        driver->set_min_height = ptk_fb_window_set_min_height;
        driver->begin_paint = ptk_fb_window_begin_paint;
        driver->end_paint = ptk_fb_window_end_paint;
        driver->present = ptk_fb_window_present;
}

#endif
