#include <LCUI.h>
#include <LCUI/main.h>
#ifdef HAS_CAIRO
#include <cairo.h>
#endif
#include "fabric.h"

#define M_PI 3.1415926

typedef struct ui_fabric {
        int timer;
} ui_fabric_t;

typedef struct ui_fabric_viewport {
        double scale;
        double offset_x;
        double offset_y;
} ui_fabric_viewport_t;

bool displayPoints = false;
bool displaySpans = true;

ui_widget_prototype_t *ui_fabric_proto;

// Computes the viewport that fits the fixed-size fabric world into the
// widget content box, like CSS `object-fit: contain`.
static void ui_fabric_get_viewport(ui_widget_t *w, ui_fabric_viewport_t *vp)
{
        double scale_x = w->content_box.width / FABRIC_WORLD_WIDTH;
        double scale_y = w->content_box.height / FABRIC_WORLD_HEIGHT;

        vp->scale = scale_x < scale_y ? scale_x : scale_y;
        if (vp->scale <= 0) {
                vp->scale = 1;
        }
        vp->offset_x =
            (w->content_box.width - FABRIC_WORLD_WIDTH * vp->scale) / 2;
        vp->offset_y =
            (w->content_box.height - FABRIC_WORLD_HEIGHT * vp->scale) / 2;
}

// Maps a point in the root (or window) coordinate space to the fabric world.
static void ui_fabric_window_to_world(ui_widget_t *w, float x, float y,
                                      double *world_x, double *world_y)
{
        ui_fabric_viewport_t vp;

        ui_fabric_get_viewport(w, &vp);
        *world_x = (x - w->content_box.x - vp.offset_x) / vp.scale;
        *world_y = (y - w->content_box.y - vp.offset_y) / vp.scale;
}

#ifdef HAS_CAIRO
// displays points
void render_points(cairo_t *cr)
{
        int i, count;
        Point p, *points;

        Fabric_getPoints(&points, &count);
        cairo_set_source_rgb(cr, 0, 1, 0);
        for (i = 0; i < count; i++) {
                p = points[i];
                cairo_arc(cr, p->cx, p->cy, 3, 0, 2 * M_PI);
        }
        cairo_fill(cr);
}

// displays spans
void render_spans(cairo_t *cr)
{
        int i, count;
        Span s, *spans;

        Fabric_getSpans(&spans, &count);
        cairo_set_source_rgb(cr, 0.33, 0.15, 0);
        cairo_set_line_width(cr, 0.66);
        for (i = 0; i < count; i++) {
                s = spans[i];
                if (s != NULL) {
                        cairo_move_to(cr, s->p1->cx, s->p1->cy);
                        cairo_line_to(cr, s->p2->cx, s->p2->cy);
                }
        }
        cairo_stroke(cr);
}

#endif

void ui_fabric_on_frame(ui_widget_t *w)
{
#ifdef HAS_CAIRO
        float scale;
        ui_fabric_viewport_t vp;
        ui_canvas_context_t *ctx = ui_canvas_get_context(w);
        cairo_surface_t *surface = cairo_image_surface_create_for_data(
            ctx->buffer.bytes, CAIRO_FORMAT_RGB24, ctx->buffer.width,
            ctx->buffer.height, ctx->buffer.bytes_per_row);
        cairo_t *cr = cairo_create(surface);

        Fabric_update();
        // clears previous frame
        ctx->fill_color = pd_color(255, 255, 255, 255);
        ctx->fill_rect(ctx, 0, 0, ctx->width, ctx->height);
        // First map logical pixels to buffer pixels, then map the fabric
        // world to the contain viewport inside the content box. The buffer
        // is created with the window DPI, which may differ from the global
        // metrics read during the timer callback, so derive the ratio from
        // the buffer and content box instead of using ctx->scale.
        if (w->content_box.width > 0) {
                scale = (float)(ctx->buffer.width / w->content_box.width);
        } else {
                scale = ctx->scale > 0 ? ctx->scale : 1.0f;
        }
        ui_fabric_get_viewport(w, &vp);
        cairo_scale(cr, scale, scale);
        cairo_translate(cr, vp.offset_x, vp.offset_y);
        cairo_scale(cr, vp.scale, vp.scale);
        if (displayPoints) {
                render_points(cr);
        }
        if (displaySpans) {
                render_spans(cr);
        }
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
        ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_GRAPH_BOX);
        ctx->release(ctx);
#endif
}

static void ui_fabric_on_mousedown(ui_widget_t *w, ui_event_t *e, void *arg)
{
        double x, y;

        ui_fabric_window_to_world(w, e->mouse.x, e->mouse.y, &x, &y);
        Fabric_grab(x, y);
        ui_widget_set_mouse_capture(w);
}

static void ui_fabric_on_mousemove(ui_widget_t *w, ui_event_t *e, void *arg)
{
        double x, y;

        ui_fabric_window_to_world(w, e->mouse.x, e->mouse.y, &x, &y);
        Fabric_move(x, y);
}

static void ui_fabric_on_mouseup(ui_widget_t *w, ui_event_t *e, void *arg)
{
        Fabric_drop();
        ui_widget_release_mouse_capture(w);
}

static void ui_fabric_on_resize(ui_widget_t *w, float width, float height)
{
        // The fabric keeps its fixed world size; only the canvas buffer needs
        // to be rebuilt for the new physical size.
        ui_fabric_proto->proto->resize(w, width, height);
}

void ui_fabric_on_init(ui_widget_t *w)
{
        ui_fabric_t *data;
        ui_fabric_proto->proto->init(w);
        data = ui_widget_add_data(w, ui_fabric_proto, sizeof(ui_fabric_t));
        ui_widget_on(w, "mousedown", ui_fabric_on_mousedown, NULL);
        ui_widget_on(w, "mousemove", ui_fabric_on_mousemove, NULL);
        ui_widget_on(w, "mouseup", ui_fabric_on_mouseup, NULL);
        data->timer = ptk_set_interval(LCUI_MAX_FRAME_MSEC,
                                       (ptk_timer_cb)ui_fabric_on_frame, w);
}

void ui_fabric_on_destroy(ui_widget_t *w)
{
        ui_fabric_t *data;
        data = ui_widget_get_data(w, ui_fabric_proto);
        ptk_clear_timer(data->timer);
        ui_fabric_proto->proto->destroy(w);
}

void ui_register_fabric(void)
{
        ui_fabric_proto = ui_create_widget_prototype("fabric", "canvas");
        ui_fabric_proto->init = ui_fabric_on_init;
        ui_fabric_proto->destroy = ui_fabric_on_destroy;
        ui_fabric_proto->resize = ui_fabric_on_resize;
}

const char *app_css = "\
root {\
        display: flex;\
        background: #f2f4f5;\
        width: 100%;\
        height: 100%;\
}\
fabric {\
        width: 100%;\
        height: 100%;\
        border: 1px solid #697b8c;\
        box-sizing: border-box;\
        box-shadow: 0 3px 6px rgba(0, 0, 0, 0.1);\
}";

int main(int argc, char **argv)
{
        lcui_window_options_t opt = { 0 };
        lcui_window_t *window;

        lcui_init();
        Fabric_init(66, 55, FABRIC_WORLD_WIDTH, FABRIC_WORLD_HEIGHT);
        ui_register_fabric();
        ui_load_css_string(app_css, __FILE__);
        ui_widget_set_title(ui_root(), L"Fabric");
        ui_root_append(ui_create_widget("fabric"));
        opt.width = FABRIC_WORLD_WIDTH;
        opt.height = FABRIC_WORLD_HEIGHT;
        window = lcui_window_create_for_widget(ui_root(), &opt);
        lcui_window_show(window);
        return lcui_main();
}
