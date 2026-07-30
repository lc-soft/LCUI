#ifndef PTK_LINUX_WAYLAND_INTERNAL_H
#define PTK_LINUX_WAYLAND_INTERNAL_H

#if defined(PTK_LINUX) && defined(PTK_HAS_WAYLAND)

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <xkbcommon/xkbcommon.h>
#include <pandagl.h>
#include <yutil.h>

#include "ptk.h"
#include "keyboard.h"
#include "waylandapp.h"
#include "xdg-shell-client-protocol.h"
#include "xdg-decoration-unstable-v1-client-protocol.h"
#include "viewporter-client-protocol.h"
#include "fractional-scale-v1-client-protocol.h"

#ifndef WL_KEYBOARD_KEY_STATE_REPEATED
#define WL_KEYBOARD_KEY_STATE_REPEATED 2
#endif

#define WAYLAND_DEFAULT_SCREEN_WIDTH 1280
#define WAYLAND_DEFAULT_SCREEN_HEIGHT 720

typedef struct ptk_window {
        struct wl_surface *surface;
        struct xdg_surface *xdg_surface;
        struct xdg_toplevel *xdg_toplevel;
        struct zxdg_toplevel_decoration_v1 *decoration;
        struct wp_viewport *viewport;
        struct wp_fractional_scale_v1 *fractional_scale;
        struct wl_buffer *buffer;
        void *buffer_data;
        size_t buffer_size;
        int width;
        int height;
        double scale;
        bool configured;
        int min_width;
        int min_height;
        int max_width;
        int max_height;
        /** Full-window canvas wrapping buffer_data (external memory, not
         *  owned by pandagl). Used as the quote source in begin_paint. */
        pd_canvas_t canvas;
        pd_context_t *paint_ctx;
        list_node_t node;
} ptk_window_t;

typedef struct ptk_wayland_app {
        struct wl_display *display;
        struct wl_registry *registry;
        struct wl_compositor *compositor;
        struct wl_shm *shm;
        struct zxdg_decoration_manager_v1 *decoration_manager;
        struct wp_viewporter *viewporter;
        struct wp_fractional_scale_manager_v1 *fractional_scale_manager;
        struct xdg_wm_base *wm_base;
        struct wl_output *output;
        struct wl_seat *seat;
        struct wl_pointer *pointer;
        struct wl_keyboard *keyboard;
        ptk_window_t *pointer_focus;
        ptk_window_t *keyboard_focus;
        double pointer_x;
        double pointer_y;
        list_t windows;
        int exit_code;
        int screen_width;
        int screen_height;
        double output_scale;

        struct xkb_context *xkb_context;
        struct xkb_keymap *xkb_map;
        struct xkb_state *xkb_state;
        uint32_t shift_mask;
        uint32_t ctrl_mask;
        uint32_t alt_mask;
        uint32_t meta_mask;
        uint32_t modifiers;

        /* Key repeat parameters from wl_keyboard.repeat_info (rate=0 means
         * disabled) */
        int repeat_rate;  /* characters/second */
        int repeat_delay; /* milliseconds before first repeat */

        struct wl_cursor_theme *cursor_theme;
        struct wl_cursor *default_cursor;
        struct wl_surface *cursor_surface;

        bool running;
} ptk_wayland_app_t;

extern ptk_wayland_app_t wl_app;

extern const struct wl_output_listener output_listener;
extern const struct wl_seat_listener seat_listener;
extern const struct wl_registry_listener registry_listener;
extern const struct xdg_wm_base_listener wm_base_listener;
extern const struct xdg_surface_listener xdg_surface_listener;
extern const struct xdg_toplevel_listener xdg_toplevel_listener;
extern const struct wp_fractional_scale_v1_listener fractional_scale_listener;
void ptk_waylandwindow_destroy(ptk_window_t *wnd);

ptk_window_t *ptk_waylandapp_get_window(void *handle);
void ptk_waylandwindow_destroy_buffer(ptk_window_t *wnd);
void ptk_waylandwindow_post_size_event(ptk_window_t *wnd);
ptk_window_t *ptk_waylandwindow_create(const wchar_t *title, int x, int y,
                                       int width, int height,
                                       ptk_window_t *parent);

#endif /* PTK_LINUX && PTK_HAS_WAYLAND */

#endif
