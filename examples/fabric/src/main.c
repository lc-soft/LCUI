#include <LCUI.h>
#include <platform/main.h>
#include <cairo.h>
#include "fabric.h"

#define M_PI 3.1415926
#define FABRIC_WIDTH 800
#define FABRIC_HEIGHT 600

typedef struct ui_fabric_t {
        int timer;
} ui_fabric_t;

bool displayPoints = false;
bool displaySpans = true;

ui_widget_prototype_t *ui_fabric_proto;

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

void ui_fabric_on_frame(ui_widget_t *w)
{
        ui_canvas_context_t *ctx = ui_canvas_get_context(w);
        cairo_surface_t *surface = cairo_image_surface_create_for_data(
            ctx->buffer.bytes, CAIRO_FORMAT_RGB24, ctx->buffer.width,
            ctx->buffer.height, ctx->buffer.bytes_per_row);
        cairo_t *cr = cairo_create(surface);

        Fabric_update();
        // clears previous frame
        ctx->fill_color = pd_color(255, 255, 255, 255);
        ctx->fill_rect(ctx, 0, 0, ctx->width, ctx->height);
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
}

static void ui_fabric_on_mousedown(ui_widget_t *w, ui_event_t *e, void *arg)
{
        float x, y;

        ui_widget_get_offset(w, NULL, &x, &y);
        Fabric_grab(e->mouse.x - x, e->mouse.y - y);
        ui_widget_set_mouse_capture(w);
}

static void ui_fabric_on_mousemove(ui_widget_t *w, ui_event_t *e, void *arg)
{
        float x, y;

        ui_widget_get_offset(w, NULL, &x, &y);
        Fabric_move(e->mouse.x - x, e->mouse.y - y);
}

static void ui_fabric_on_mouseup(ui_widget_t *w, ui_event_t *e, void *arg)
{
        Fabric_drop();
        ui_widget_release_mouse_capture(w);
}

void ui_fabric_on_init(ui_widget_t *w)
{
        ui_fabric_t *data;
        ui_fabric_proto->proto->init(w);
        data = ui_widget_add_data(w, ui_fabric_proto, sizeof(ui_fabric_t));
        ui_widget_on(w, "mousedown", ui_fabric_on_mousedown, NULL);
        ui_widget_on(w, "mousemove", ui_fabric_on_mousemove, NULL);
        ui_widget_on(w, "mouseup", ui_fabric_on_mouseup, NULL);
        ui_widget_resize(w, FABRIC_WIDTH, FABRIC_HEIGHT);
        ui_widget_set_style_string(w, "background-color", "#f00");
        data->timer = lcui_set_interval(LCUI_MAX_FRAME_MSEC,
                                        (timer_callback)ui_fabric_on_frame, w);
}

void ui_fabric_on_destroy(ui_widget_t *w)
{
        ui_fabric_t *data;
        data = ui_widget_get_data(w, ui_fabric_proto);
        lcui_destroy_timer(data->timer);
}

void ui_register_fabric(void)
{
        ui_fabric_proto = ui_create_widget_prototype("fabric", "canvas");
        ui_fabric_proto->init = ui_fabric_on_init;
}

const char *app_css = "\
root {\
        display: flex;\
        align-items: center;\
        justify-content: center;\
        background: #f2f4f5;\
}\
fabric {\
        border: 1px solid #697b8c;\
        box-sizing : border-box;\
        box-shadow : 0 3px 6px rgba(0, 0, 0, 0.1);\
}";

int main(int argc, char **argv)
{
        ui_widget_t *w;

        lcui_init();
        Fabric_init(66, 55, FABRIC_WIDTH, FABRIC_HEIGHT);
        ui_register_fabric();
        ui_load_css_string(app_css, __FILE__);
        ui_widget_set_title(ui_root(), L"Fabric");
        ui_widget_resize(ui_root(), FABRIC_WIDTH + 100, FABRIC_HEIGHT + 100);
        w = ui_create_widget("fabric");
        ui_widget_set_style_unit_value(w, css_key_max_width, FABRIC_WIDTH,
                                       CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_key_min_width, FABRIC_WIDTH,
                                       CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_key_max_height, FABRIC_HEIGHT,
                                       CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_key_min_height, FABRIC_HEIGHT,
                                       CSS_UNIT_PX);
        ui_root_append(w);
        return lcui_main();
}
