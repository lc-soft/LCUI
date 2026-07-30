#define _GNU_SOURCE
#include "ptk.h"

#if defined(PTK_LINUX) && defined(PTK_HAS_WAYLAND)

#include "wayland_internal.h"

ptk_wayland_app_t wl_app;

ptk_window_t *ptk_waylandapp_get_window(void *handle)
{
        list_node_t *node;

        for (list_each(node, &wl_app.windows)) {
                ptk_window_t *wnd = node->data;

                if (wnd && wnd->surface == handle) {
                        return wnd;
                }
        }
        return NULL;
}

static void ptk_waylandapp_on_registry_global(void *data,
                                              struct wl_registry *registry,
                                              uint32_t name,
                                              const char *interface,
                                              uint32_t version)
{
        if (strcmp(interface, wl_compositor_interface.name) == 0) {
                wl_app.compositor = wl_registry_bind(
                    registry, name, &wl_compositor_interface, 4);
                return;
        }
        if (strcmp(interface, wl_shm_interface.name) == 0) {
                wl_app.shm =
                    wl_registry_bind(registry, name, &wl_shm_interface, 1);
                return;
        }
        if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
                uint32_t bind_version = version < 1 ? version : 1;
                wl_app.wm_base = wl_registry_bind(
                    registry, name, &xdg_wm_base_interface, bind_version);
                return;
        }
        if (strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
                wl_app.decoration_manager = wl_registry_bind(
                    registry, name, &zxdg_decoration_manager_v1_interface, 1);
                return;
        }
        if (strcmp(interface, wl_output_interface.name) == 0 &&
            !wl_app.output) {
                wl_app.output =
                    wl_registry_bind(registry, name, &wl_output_interface, 2);
                wl_output_add_listener(wl_app.output, &output_listener, NULL);
                return;
        }
        if (strcmp(interface, wl_seat_interface.name) == 0 && !wl_app.seat) {
                wl_app.seat =
                    wl_registry_bind(registry, name, &wl_seat_interface, 5);
                wl_seat_add_listener(wl_app.seat, &seat_listener, NULL);
                return;
        }
}

static void ptk_waylandapp_on_registry_global_remove(
    void *data, struct wl_registry *registry, uint32_t name)
{
}

const struct wl_registry_listener registry_listener = {
        ptk_waylandapp_on_registry_global,
        ptk_waylandapp_on_registry_global_remove
};

static int ptk_waylandapp_poll_events(int timeout_ms, bool dispatch_all)
{
        struct pollfd pfd;
        int result;

        result = wl_display_dispatch_pending(wl_app.display);
        if (result < 0) {
                return -1;
        }
        if (dispatch_all && result > 0) {
                do {
                        result = wl_display_dispatch_pending(wl_app.display);
                } while (result > 0);
                if (result < 0) {
                        return -1;
                }
        }
        if (wl_display_flush(wl_app.display) < 0 && errno != EAGAIN) {
                return -1;
        }

        pfd.fd = wl_display_get_fd(wl_app.display);
        pfd.events = POLLIN;
        pfd.revents = 0;
        result = poll(&pfd, 1, timeout_ms);
        if (result <= 0) {
                return result;
        }
        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
                return -1;
        }
        if (!(pfd.revents & POLLIN)) {
                return 0;
        }
        result = wl_display_dispatch(wl_app.display);
        if (result < 0) {
                return -1;
        }
        if (!dispatch_all) {
                return result;
        }
        while ((result = wl_display_dispatch_pending(wl_app.display)) > 0) {
        }
        return result < 0 ? -1 : 1;
}

static int ptk_waylandapp_destroy(void)
{
        list_node_t *node, *next;
        ptk_window_t *wnd;

        for (node = wl_app.windows.head.next; node; node = next) {
                next = node->next;
                wnd = node->data;
                if (wnd) {
                        if (wnd->decoration) {
                                zxdg_toplevel_decoration_v1_destroy(
                                    wnd->decoration);
                        }
                        if (wnd->xdg_toplevel) {
                                xdg_toplevel_destroy(wnd->xdg_toplevel);
                        }
                        if (wnd->xdg_surface) {
                                xdg_surface_destroy(wnd->xdg_surface);
                        }
                        if (wnd->surface) {
                                wl_surface_destroy(wnd->surface);
                        }
                        ptk_waylandwindow_destroy_buffer(wnd);
                        free(wnd->paint_ctx);
                        free(wnd);
                }
        }
        list_create(&wl_app.windows);
        if (wl_app.pointer) {
                wl_pointer_destroy(wl_app.pointer);
        }
        if (wl_app.cursor_surface) {
                wl_surface_destroy(wl_app.cursor_surface);
                wl_app.cursor_surface = NULL;
        }
        if (wl_app.cursor_theme) {
                wl_cursor_theme_destroy(wl_app.cursor_theme);
                wl_app.cursor_theme = NULL;
                wl_app.default_cursor = NULL;
        }
        if (wl_app.keyboard) {
                wl_keyboard_destroy(wl_app.keyboard);
        }
        if (wl_app.xkb_state) {
                xkb_state_unref(wl_app.xkb_state);
                wl_app.xkb_state = NULL;
        }
        if (wl_app.xkb_map) {
                xkb_keymap_unref(wl_app.xkb_map);
                wl_app.xkb_map = NULL;
        }
        if (wl_app.xkb_context) {
                xkb_context_unref(wl_app.xkb_context);
                wl_app.xkb_context = NULL;
        }
        if (wl_app.seat) {
                wl_seat_destroy(wl_app.seat);
        }
        if (wl_app.output) {
                wl_output_destroy(wl_app.output);
        }
        if (wl_app.decoration_manager) {
                zxdg_decoration_manager_v1_destroy(wl_app.decoration_manager);
        }
        if (wl_app.wm_base) {
                xdg_wm_base_destroy(wl_app.wm_base);
        }
        if (wl_app.shm) {
                wl_shm_destroy(wl_app.shm);
        }
        if (wl_app.compositor) {
                wl_compositor_destroy(wl_app.compositor);
        }
        if (wl_app.registry) {
                wl_registry_destroy(wl_app.registry);
        }
        if (wl_app.display) {
                wl_display_disconnect(wl_app.display);
        }
        memset(&wl_app, 0, sizeof(wl_app));
        return 0;
}

static int ptk_waylandapp_init(const wchar_t *name)
{
        memset(&wl_app, 0, sizeof(wl_app));
        wl_app.output_scale = 1;
        wl_app.display = wl_display_connect(NULL);
        if (!wl_app.display) {
                logger_error("[wayland] wl_display_connect() failed\n");
                return -1;
        }
        wl_app.screen_width = WAYLAND_DEFAULT_SCREEN_WIDTH;
        wl_app.screen_height = WAYLAND_DEFAULT_SCREEN_HEIGHT;
        wl_app.registry = wl_display_get_registry(wl_app.display);
        if (!wl_app.registry) {
                logger_error("[wayland] wl_display_get_registry() failed\n");
                wl_display_disconnect(wl_app.display);
                wl_app.display = NULL;
                return -1;
        }
        list_create(&wl_app.windows);
        wl_registry_add_listener(wl_app.registry, &registry_listener, NULL);
        if (wl_display_roundtrip(wl_app.display) < 0) {
                logger_error("[wayland] wl_display_roundtrip() failed during "
                             "registry init\n");
                ptk_waylandapp_destroy();
                return -1;
        }
        if (!wl_app.compositor || !wl_app.shm || !wl_app.wm_base) {
                logger_error("[wayland] missing globals: compositor=%p shm=%p "
                             "wm_base=%p\n",
                             wl_app.compositor, wl_app.shm, wl_app.wm_base);
                ptk_waylandapp_destroy();
                return -1;
        }
        xdg_wm_base_add_listener(wl_app.wm_base, &wm_base_listener, NULL);
        wl_app.running = true;
        wl_app.exit_code = 0;
        return 0;
}

static int ptk_waylandapp_process_events(ptk_process_events_option_t option)
{
        int result = 0;

        wl_app.exit_code = 0;
        if (option == PTK_PROCESS_EVENTS_ONE_IF_PRESENT ||
            option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT) {
                ptk_tick();
                ptk_process_events();
                result = ptk_waylandapp_poll_events(
                    0, option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
                ptk_process_events();
                return result < 0 ? -1 : wl_app.exit_code;
        }
        while (wl_app.running) {
                ptk_tick();
                ptk_process_events();
                result = ptk_waylandapp_poll_events(1, true);
                if (result < 0) {
                        break;
                }
                ptk_process_events();
        }
        return wl_app.exit_code;
}

static void ptk_waylandapp_present(void)
{
        wl_display_flush(wl_app.display);
}

static void ptk_waylandapp_exit(int exit_code)
{
        wl_app.running = false;
        wl_app.exit_code = exit_code;
}

static int ptk_waylandapp_on_event(int type, ptk_native_event_handler_t handler,
                                   void *data)
{
        return -1;
}

static int ptk_waylandapp_off_event(int type,
                                    ptk_native_event_handler_t handler)
{
        return -1;
}

static int ptk_waylandapp_get_screen_width(void)
{
        return wl_app.screen_width / wl_app.output_scale;
}

static int ptk_waylandapp_get_screen_height(void)
{
        return wl_app.screen_height / wl_app.output_scale;
}

void ptk_waylandapp_driver_init(ptk_app_driver_t *driver)
{
        memset(driver, 0, sizeof(*driver));
        driver->init = ptk_waylandapp_init;
        driver->destroy = ptk_waylandapp_destroy;
        driver->process_events = ptk_waylandapp_process_events;
        driver->on_event = ptk_waylandapp_on_event;
        driver->off_event = ptk_waylandapp_off_event;
        driver->get_screen_width = ptk_waylandapp_get_screen_width;
        driver->get_screen_height = ptk_waylandapp_get_screen_height;
        driver->create_window = ptk_waylandwindow_create;
        driver->get_window = ptk_waylandapp_get_window;
        driver->present = ptk_waylandapp_present;
        driver->exit = ptk_waylandapp_exit;
}

#endif /* PTK_LINUX && PTK_HAS_WAYLAND */
