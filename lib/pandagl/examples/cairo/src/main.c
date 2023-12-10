/*
 * lib/pandagl/examples/cairo/src/main.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <cairo/cairo.h>
#include <pandagl.h>

#define M_PI 3.1415926

// Source code from: https://www.cairographics.org/samples/text_align_center/

void draw_cairo_text(cairo_surface_t* surface)
{
        cairo_t* cr;
        cairo_text_extents_t extents;

        const char* utf8 = "cairo";
        double x, y;

        cr = cairo_create(surface);

        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_NORMAL);

        cairo_set_font_size(cr, 52.0);
        cairo_text_extents(cr, utf8, &extents);
        x = 128.0 - (extents.width / 2 + extents.x_bearing);
        y = 128.0 - (extents.height / 2 + extents.y_bearing);

        cairo_move_to(cr, x, y);
        cairo_show_text(cr, utf8);

        /* draw helping lines */
        cairo_set_source_rgba(cr, 1, 0.2, 0.2, 0.6);
        cairo_set_line_width(cr, 6.0);
        cairo_arc(cr, x, y, 10.0, 0, 2 * M_PI);
        cairo_fill(cr);
        cairo_move_to(cr, 128.0, 0);
        cairo_rel_line_to(cr, 0, 256);
        cairo_move_to(cr, 0, 128.0);
        cairo_rel_line_to(cr, 256, 0);
        cairo_stroke(cr);
        cairo_destroy(cr);
}

int main(int argc, char** argv)
{
        pd_canvas_t canvas;
        cairo_surface_t* surface;

        // Create a canvas
        pd_canvas_init(&canvas);
        canvas.color_type = PD_COLOR_TYPE_ARGB;
        pd_canvas_create(&canvas, 256, 256);
        pd_canvas_fill(&canvas, pd_rgb(255, 255, 255));

        // Create a surface based on the canvas so that cairo can also modify
        // the pixel data in the canvas
        surface = cairo_image_surface_create_for_data(
            canvas.bytes, CAIRO_FORMAT_RGB24, canvas.width, canvas.height,
            canvas.bytes_per_row);
        draw_cairo_text(surface);

        // Fill the four rectangles
        pd_canvas_fill_rect(&canvas, pd_rgb(255, 0, 0),
                            pd_rect(20, 20, 40, 40));
        pd_canvas_fill_rect(&canvas, pd_rgb(0, 255, 0),
                            pd_rect(196, 20, 40, 40));
        pd_canvas_fill_rect(&canvas, pd_rgb(0, 0, 255),
                            pd_rect(20, 196, 40, 40));
        pd_canvas_fill_rect(&canvas, pd_rgb(100, 100, 100),
                            pd_rect(196, 196, 40, 40));

        cairo_surface_write_to_png(surface, "output.png");
        cairo_surface_destroy(surface);

        pd_canvas_destroy(&canvas);
        return 0;
}
