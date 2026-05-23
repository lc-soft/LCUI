#if defined(PTK_LINUX) && defined(PTK_HAS_WAYLAND)

#include "wayland_internal.h"

void ptk_waylandwindow_destroy_buffer(ptk_window_t *wnd)
{
        if (wnd->buffer) {
                wl_buffer_destroy(wnd->buffer);
                wnd->buffer = NULL;
        }
        if (wnd->buffer_data) {
                munmap(wnd->buffer_data, wnd->buffer_size);
                wnd->buffer_data = NULL;
                wnd->canvas.bytes = NULL;
        }
        wnd->buffer_size = 0;
}

void ptk_waylandwindow_post_size_event(ptk_window_t *wnd)
{
        ptk_event_t e = { 0 };

        e.type = PTK_EVENT_SIZE;
        e.window = wnd;
        e.size.width = wnd->width;
        e.size.height = wnd->height;
        ptk_post_event(&e);
}

static int ptk_waylandwindow_get_buffer_size(int size)
{
        return size * wl_app.output_scale;
}

static void ptk_waylandapp_on_wm_base_ping(void *data,
                                           struct xdg_wm_base *wm_base,
                                           uint32_t serial)
{
        xdg_wm_base_pong(wm_base, serial);
}

const struct xdg_wm_base_listener wm_base_listener = {
        ptk_waylandapp_on_wm_base_ping
};

static void ptk_waylandwindow_on_xdg_surface_configure(
    void *data, struct xdg_surface *surface, uint32_t serial)
{
        ptk_window_t *wnd = data;

        xdg_surface_ack_configure(surface, serial);
        wnd->configured = true;
}

const struct xdg_surface_listener xdg_surface_listener = {
        ptk_waylandwindow_on_xdg_surface_configure
};

static void ptk_waylandwindow_on_toplevel_configure(
    void *data, struct xdg_toplevel *xdg_toplevel, int32_t width,
    int32_t height, struct wl_array *states)
{
        ptk_window_t *wnd = data;
        int buffer_width;
        int buffer_height;

        if (width <= 0 || height <= 0) {
                return;
        }
        buffer_width = ptk_waylandwindow_get_buffer_size(width);
        buffer_height = ptk_waylandwindow_get_buffer_size(height);
        if (wnd->width == buffer_width && wnd->height == buffer_height) {
                return;
        }
        wnd->width = buffer_width;
        wnd->height = buffer_height;
        ptk_waylandwindow_destroy_buffer(wnd);
        ptk_waylandwindow_post_size_event(wnd);
}

static void ptk_waylandwindow_on_toplevel_close(
    void *data, struct xdg_toplevel *xdg_toplevel)
{
        ptk_window_t *wnd = data;
        ptk_event_t e = { 0 };

        e.type = PTK_EVENT_CLOSE;
        e.window = wnd;
        ptk_post_event(&e);
}

static void ptk_waylandwindow_on_toplevel_configure_bounds(
    void *data, struct xdg_toplevel *xdg_toplevel, int32_t width,
    int32_t height)
{
}

static void ptk_waylandwindow_on_toplevel_wm_capabilities(
    void *data, struct xdg_toplevel *xdg_toplevel,
    struct wl_array *capabilities)
{
}

const struct xdg_toplevel_listener xdg_toplevel_listener = {
        ptk_waylandwindow_on_toplevel_configure,
        ptk_waylandwindow_on_toplevel_close,
        ptk_waylandwindow_on_toplevel_configure_bounds,
        ptk_waylandwindow_on_toplevel_wm_capabilities
};

ptk_window_t *ptk_waylandwindow_create(const wchar_t *title, int x, int y,
                                       int width, int height,
                                       ptk_window_t *parent)
{
        ptk_window_t *wnd;

        if (!wl_app.compositor || !wl_app.wm_base || !wl_app.shm) {
                return NULL;
        }
        wnd = calloc(1, sizeof(*wnd));
        if (!wnd) {
                return NULL;
        }
        wnd->width = ptk_waylandwindow_get_buffer_size(
            width > 0 ? width : PTK_WINDOW_DEFAULT_WIDTH);
        wnd->height = ptk_waylandwindow_get_buffer_size(
            height > 0 ? height : PTK_WINDOW_DEFAULT_HEIGHT);
        wnd->surface = wl_compositor_create_surface(wl_app.compositor);
        if (!wnd->surface) {
                free(wnd);
                return NULL;
        }
        wnd->xdg_surface =
            xdg_wm_base_get_xdg_surface(wl_app.wm_base, wnd->surface);
        wnd->xdg_toplevel = xdg_surface_get_toplevel(wnd->xdg_surface);
        if (!wnd->xdg_surface || !wnd->xdg_toplevel) {
                if (wnd->xdg_toplevel) {
                        xdg_toplevel_destroy(wnd->xdg_toplevel);
                }
                if (wnd->xdg_surface) {
                        xdg_surface_destroy(wnd->xdg_surface);
                }
                wl_surface_destroy(wnd->surface);
                free(wnd);
                return NULL;
        }
        wnd->node.data = wnd;
        xdg_surface_add_listener(wnd->xdg_surface, &xdg_surface_listener, wnd);
        xdg_toplevel_add_listener(wnd->xdg_toplevel, &xdg_toplevel_listener,
                                  wnd);

        if (wl_app.decoration_manager) {
                wnd->decoration =
                    zxdg_decoration_manager_v1_get_toplevel_decoration(
                        wl_app.decoration_manager, wnd->xdg_toplevel);
                if (wnd->decoration) {
                        zxdg_toplevel_decoration_v1_set_mode(
                            wnd->decoration,
                            ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
                }
        }
        if (title) {
                size_t len = encode_utf8(NULL, title, 0) + 1;
                char *utf8_title = malloc(sizeof(char) * len);

                if (utf8_title) {
                        encode_utf8(utf8_title, title, len);
                        xdg_toplevel_set_title(wnd->xdg_toplevel, utf8_title);
                        free(utf8_title);
                }
        }
        wl_surface_commit(wnd->surface);
        wl_display_roundtrip(wl_app.display);
        list_append_node(&wl_app.windows, &wnd->node);
        return wnd;
}

static void ptk_waylandwindow_close(ptk_window_t *wnd)
{
        ptk_event_t e = { 0 };

        e.type = PTK_EVENT_CLOSE;
        e.window = wnd;
        ptk_post_event(&e);
}

static void ptk_waylandwindow_show(ptk_window_t *wnd)
{
        wl_surface_commit(wnd->surface);
}

static void ptk_waylandwindow_activate(ptk_window_t *wnd)
{
        wl_surface_commit(wnd->surface);
}

static void ptk_waylandwindow_set_title(ptk_window_t *wnd, const wchar_t *title)
{
        size_t len;
        char *utf8_title;

        if (!title || !wnd->xdg_toplevel) {
                return;
        }
        len = encode_utf8(NULL, title, 0) + 1;
        utf8_title = malloc(sizeof(char) * len);
        if (!utf8_title) {
                return;
        }
        encode_utf8(utf8_title, title, len);
        xdg_toplevel_set_title(wnd->xdg_toplevel, utf8_title);
        free(utf8_title);
}

static void ptk_waylandwindow_destroy(ptk_window_t *wnd)
{
        if (!wnd) {
                return;
        }
        list_unlink(&wl_app.windows, &wnd->node);
        if (wnd->decoration) {
                zxdg_toplevel_decoration_v1_destroy(wnd->decoration);
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

static void ptk_waylandwindow_set_size(ptk_window_t *wnd, int width, int height)
{
        int physical_width;
        int physical_height;

        if (width <= 0 || height <= 0) {
                return;
        }
        physical_width = ptk_waylandwindow_get_buffer_size(width);
        physical_height = ptk_waylandwindow_get_buffer_size(height);

        if (wnd->width == physical_width && wnd->height == physical_height) {
                return;
        }
        wnd->width = physical_width;
        wnd->height = physical_height;
        ptk_waylandwindow_destroy_buffer(wnd);
}

static void ptk_waylandwindow_set_position(ptk_window_t *wnd, int x, int y)
{
}

static void *ptk_waylandwindow_get_handle(ptk_window_t *wnd)
{
        return wnd->surface;
}

static int ptk_waylandwindow_get_width(ptk_window_t *wnd)
{
        return wnd->width / wl_app.output_scale;
}

static int ptk_waylandwindow_get_height(ptk_window_t *wnd)
{
        return wnd->height / wl_app.output_scale;
}

static void ptk_waylandwindow_apply_size_hints(ptk_window_t *wnd)
{
        if (!wnd->xdg_toplevel) {
                return;
        }
        xdg_toplevel_set_min_size(wnd->xdg_toplevel,
                                  wnd->min_width * wl_app.output_scale,
                                  wnd->min_height * wl_app.output_scale);
        xdg_toplevel_set_max_size(wnd->xdg_toplevel,
                                  wnd->max_width * wl_app.output_scale,
                                  wnd->max_height * wl_app.output_scale);
}

static void ptk_waylandwindow_set_min_width(ptk_window_t *wnd, int min_width)
{
        wnd->min_width = min_width;
        ptk_waylandwindow_apply_size_hints(wnd);
}

static void ptk_waylandwindow_set_min_height(ptk_window_t *wnd, int min_height)
{
        wnd->min_height = min_height;
        ptk_waylandwindow_apply_size_hints(wnd);
}

static void ptk_waylandwindow_set_max_width(ptk_window_t *wnd, int max_width)
{
        wnd->max_width = max_width;
        ptk_waylandwindow_apply_size_hints(wnd);
}

static void ptk_waylandwindow_set_max_height(ptk_window_t *wnd, int max_height)
{
        wnd->max_height = max_height;
        ptk_waylandwindow_apply_size_hints(wnd);
}

static unsigned ptk_waylandwindow_get_dpi(ptk_window_t *wnd)
{
        return wl_app.output_scale * 96;
}

static ptk_window_paint_t *ptk_waylandwindow_begin_paint(ptk_window_t *wnd,
                                                         pd_rect_t *rect)
{
        size_t size;
        int fd;
        struct wl_shm_pool *pool;

        if (!wnd->configured || !rect) {
                return NULL;
        }
        if (!wnd->buffer) {
                size = (size_t)wnd->width * (size_t)wnd->height * 4;
                fd = memfd_create("lcui-wayland-buffer", MFD_CLOEXEC);
                if (fd < 0) {
                        return NULL;
                }
                if (ftruncate(fd, (off_t)size) < 0) {
                        close(fd);
                        return NULL;
                }
                wnd->buffer_data =
                    mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                if (wnd->buffer_data == MAP_FAILED) {
                        wnd->buffer_data = NULL;
                        close(fd);
                        return NULL;
                }
                pool = wl_shm_create_pool(wl_app.shm, fd, (int)size);
                wnd->buffer = wl_shm_pool_create_buffer(
                    pool, 0, wnd->width, wnd->height, wnd->width * 4,
                    WL_SHM_FORMAT_XRGB8888);
                wl_shm_pool_destroy(pool);
                close(fd);
                wnd->buffer_size = size;
                /* Initialize the backing canvas once per buffer lifetime.
                 * pd_canvas_init leaves mem_size=0 and bytes=NULL; we
                 * override bytes to point at the mmap'd shm buffer.
                 * The canvas must NOT be passed to pd_canvas_destroy(),
                 * which would free() the pointer — use destroy_buffer instead.
                 */
                pd_canvas_init(&wnd->canvas);
                wnd->canvas.width = wnd->width;
                wnd->canvas.height = wnd->height;
                wnd->canvas.color_type = PD_COLOR_TYPE_ARGB;
                wnd->canvas.bytes = wnd->buffer_data;
                wnd->canvas.bytes_per_pixel = 4;
                wnd->canvas.bytes_per_row = wnd->width * 4;
        }
        if (!wnd->paint_ctx) {
                wnd->paint_ctx = calloc(1, sizeof(*wnd->paint_ctx));
                if (!wnd->paint_ctx) {
                        return NULL;
                }
        }
        /* Set up the paint context following pd_context_create() semantics:
         * paint->canvas must be a pd_canvas_quote() of the full backing canvas
         * clipped to the dirty rect. Pandagl renderers draw at (0,0) within
         * this quoted canvas, which corresponds to (rect->x, rect->y) in the
         * actual shm buffer. Setting canvas = full buffer (old code) caused
         * all renders to write to buffer[0,0] regardless of the dirty rect. */
        wnd->paint_ctx->rect = *rect;
        wnd->paint_ctx->with_alpha = false;
        pd_rect_correct(&wnd->paint_ctx->rect, wnd->width, wnd->height);
        pd_canvas_init(&wnd->paint_ctx->canvas);
        pd_canvas_quote(&wnd->paint_ctx->canvas, &wnd->canvas,
                        &wnd->paint_ctx->rect);
        /* Clear the dirty region to white before the widget renders into it */
        pd_canvas_fill(&wnd->paint_ctx->canvas, pd_rgb(255, 255, 255));
        return wnd->paint_ctx;
}

static void ptk_waylandwindow_end_paint(ptk_window_t *wnd,
                                        ptk_window_paint_t *paint)
{
        if (!wnd || !paint || !wnd->buffer) {
                return;
        }
        wl_surface_set_buffer_scale(wnd->surface, wl_app.output_scale);
        wl_surface_attach(wnd->surface, wnd->buffer, 0, 0);
        wl_surface_damage_buffer(wnd->surface, paint->rect.x, paint->rect.y,
                                 paint->rect.width, paint->rect.height);
        wl_surface_commit(wnd->surface);
        wl_display_flush(wl_app.display);
}

static void ptk_waylandwindow_present(ptk_window_t *wnd)
{
        wl_surface_commit(wnd->surface);
        wl_display_flush(wl_app.display);
}

void ptk_waylandwindow_driver_init(ptk_window_driver_t *driver)
{
        memset(driver, 0, sizeof(*driver));
        driver->show = ptk_waylandwindow_show;
        driver->activate = ptk_waylandwindow_activate;
        driver->close = ptk_waylandwindow_close;
        driver->destroy = ptk_waylandwindow_destroy;
        driver->set_title = ptk_waylandwindow_set_title;
        driver->set_size = ptk_waylandwindow_set_size;
        driver->set_position = ptk_waylandwindow_set_position;
        driver->get_handle = ptk_waylandwindow_get_handle;
        driver->get_width = ptk_waylandwindow_get_width;
        driver->get_height = ptk_waylandwindow_get_height;
        driver->get_dpi = ptk_waylandwindow_get_dpi;
        driver->set_min_width = ptk_waylandwindow_set_min_width;
        driver->set_min_height = ptk_waylandwindow_set_min_height;
        driver->set_max_width = ptk_waylandwindow_set_max_width;
        driver->set_max_height = ptk_waylandwindow_set_max_height;
        driver->begin_paint = ptk_waylandwindow_begin_paint;
        driver->end_paint = ptk_waylandwindow_end_paint;
        driver->present = ptk_waylandwindow_present;
}

#endif /* PTK_LINUX && PTK_HAS_WAYLAND */
