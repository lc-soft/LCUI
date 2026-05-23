#if defined(PTK_LINUX) && defined(PTK_HAS_WAYLAND)

#include "wayland_internal.h"

static void ptk_waylandapp_on_output_geometry(
    void *data, struct wl_output *wl_output, int32_t x, int32_t y,
    int32_t physical_width, int32_t physical_height, int32_t subpixel,
    const char *make, const char *model, int32_t transform)
{
}

static void ptk_waylandapp_on_output_mode(void *data,
                                          struct wl_output *wl_output,
                                          uint32_t flags, int32_t width,
                                          int32_t height, int32_t refresh)
{
        if (flags & WL_OUTPUT_MODE_CURRENT) {
                wl_app.screen_width = width;
                wl_app.screen_height = height;
        }
}

static void ptk_waylandapp_on_output_done(void *data,
                                          struct wl_output *wl_output)
{
}

static void ptk_waylandapp_on_output_scale(void *data,
                                           struct wl_output *wl_output,
                                           int32_t factor)
{
        list_node_t *node;

        if (factor <= 0 || wl_app.output_scale == factor) {
                return;
        }
        wl_app.output_scale = factor;
        /* Destroy buffers so they are recreated at the new scale on
         * the next paint. The compositor will send a configure event
         * with updated logical size. */
        for (list_each(node, &wl_app.windows)) {
                ptk_window_t *wnd = node->data;
                if (wnd) {
                        ptk_waylandwindow_destroy_buffer(wnd);
                }
        }
}

const struct wl_output_listener output_listener = {
        .geometry = ptk_waylandapp_on_output_geometry,
        .mode = ptk_waylandapp_on_output_mode,
        .done = ptk_waylandapp_on_output_done,
        .scale = ptk_waylandapp_on_output_scale,
};

#endif /* PTK_LINUX && PTK_HAS_WAYLAND */
