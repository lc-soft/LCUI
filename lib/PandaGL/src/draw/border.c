// Copyright to be defined

#include <math.h>
#include <PandaGL/draw/border.h>
#include <PandaGL/canvas.h>
#include <PandaGL/util/rect.h>
#include <PandaGL/util/math.h>
#include <stdio.h>

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

#define POW2(X) ((X) * (X))
#define CIRCLE_R(R) (R - 0.5)

/*  Convert screen Y coordinate to geometric Y coordinate */
#define ToGeoY(Y, CENTER_Y) ((CENTER_Y)-Y)

/*  Convert screen X coordinate to geometric X coordinate */
#define ToGeoX(X, CENTER_X) (X - (CENTER_X))

#define smooth_left_pixel(PX, X) (uint8_t)((PX)->a * (1.0 - (X - 1.0 * (int)X)))
#define smooth_right_pixel(PX, X) (uint8_t)((PX)->a * (X - 1.0 * (int)X))

#define border_render_contexr()                             \
    int x, y;                                         \
    int right;                                        \
    double circle_x, circle_y;                        \
    int outer_xi, inner_xi;                           \
    double outer_x, split_x, inner_x;                 \
    double outer_d, inner_d;                          \
                                                          \
    const double r = CIRCLE_R(radius);                \
    const double radius_x = max(0, r - yline->width); \
    const double radius_y = max(0, r - xline->width); \
    const int width = max(radius, yline->width);      \
                                                          \
    pd_rect_t rect;                                   \
    pd_color_t *p;                                     \
    pd_color_t color;


double Sqrt( double dNum )
{
    double dVal = -1;
    double dLastVal;
  
    //负数不符合运算规则，返回 -1
    if ( dNum < 0 )
    {
        return dVal;
    }
    //根据上面的计算公式计算
    dVal = 1.0;
    do
    {
        dLastVal = dVal;
        dVal = ( dLastVal + dNum / dLastVal ) / 2.0;
    } while ( dVal - dLastVal > 0.000001 || dVal - dLastVal < -0.000001);
  
    return dVal;
}

static double ellipse_x(double radius_x, double radius_y, double y)
{
    double value;
    if (radius_x == radius_y) {
        value = radius_x * radius_x - y * y;
    } else {
        value = (1.0 - 1.0 * (y * y) / (radius_y * radius_y)) *
            radius_x * radius_x;
    }
    if (value < 0) {
        value = -value;
    }
    return Sqrt(value);
}

/**
 * FIXME: Improve the rounded border drawing code
 * Merge the four functions of DrawBorder* into one function and make it simple.
 */



/** Draw border top left corner */
static int draw_border_corner(pd_context_t* context_ptr,
                 int bound_left,
                 int bound_top, const pd_border_line_t *xline,
                 const pd_border_line_t *yline, unsigned int radius) {
    border_render_contexr();

    double circle_center_x = bound_left + r;
    double circle_center_y = bound_top + r;
    double split_k = 1.0 * yline->width / xline->width;
    double split_center_x = bound_left + 1.0 * yline->width;
    double split_center_y = bound_top + 1.0 * xline->width;
    int inner_ellipse_top = (int)split_center_y;

    /* Get the actual rectangle that can be drawn */
    rect = context_ptr->rect;

    pd_canvas_t* dst = context_ptr->source;
    
    right = min(rect.width, bound_left + width);
    for (y = 0; y < rect.height; ++y) {
        outer_x = 0;
        split_x = 0;
        inner_x = width;
        circle_y = ToGeoY(y, circle_center_y);
        if (r > 0 && circle_y >= 0) {
            outer_x = r - ellipse_x(r, r, circle_y);
            if (radius_y > 0 && y >= inner_ellipse_top) {
                inner_x =
                    r - ellipse_x(radius_x, radius_y, circle_y);
            }
        }
        if (xline->width > 0) {
            split_x = split_center_x -
                  ToGeoY(y, split_center_y) * split_k;
        }
        outer_x = bound_left + outer_x;
        inner_x = bound_left + inner_x;
        /* Limit coordinates into the current drawing region */
        outer_x = max(0, min(right, outer_x));
        inner_x = max(0, min(right, inner_x));
        outer_xi = max(0, (int)outer_x - (int)radius / 2);
        inner_xi = min(right, (int)inner_x + (int)radius / 2);
        p = pd_canvas_get_pixel_pointer(dst, rect.x, rect.y + y);
        /* Clear the outer pixels */
        for (x = 0; x < outer_xi; ++x, ++p) {
            p->alpha = 0;
        }
        for (; x < inner_xi; ++x, ++p) {
            outer_d = -1;
            inner_d = inner_x - 1.0 * x;
            circle_x = ToGeoX(x, circle_center_x);
            /* If in the circle */
            if (r > 0 && circle_y >= 0 && circle_x <= 0) {
                outer_d =
                    Sqrt(POW2(circle_x) + POW2(circle_y)) - r;
                /* If the inside is a circle is not an ellipse,
                 * Use the same anti-aliasing method
                 */
                if (radius_x == radius_y && radius_y > 0 &&
                    y >= inner_ellipse_top) {
                    inner_d = outer_d + r - radius_x;
                }
            }
            if (outer_d >= 1.0) {
                p->value = 0;
                continue;
            }
            if (x < split_x) {
                color = yline->color;
            } else {
                color = xline->color;
            }
            if (outer_d >= 0) {
                /* Fill the border color if the border width is
                 * valid */
                if (inner_d - outer_d >= 0.5) {
                    *p = color;
                }
                p->a = smooth_left_pixel(p, outer_d);
            } else if (inner_d >= 1.0) {
                *p = color;
            } else if (inner_d >= 0) {
                color.a = smooth_right_pixel(&color, inner_d);
                *p = color;
            } else {
                break;
            }
        }
    }
    return 0;
}

/**
 * FIXME: Improve the content cropping code
 * Merge the four functions of CropContent* into one function and make it
 * simple.
 */

/** Crop the top left corner of the content area */
static int crop_content_corner(pd_context_t* context_ptr, int bound_left, int bound_top,
                  double radius_x, double radius_y) {
    int xi, yi;
    int outer_xi;
    double x, y, d;
    double outer_x;
    double center_x, center_y;

    pd_rect_t rect;
    pd_color_t *p;

    radius_x -= 0.5;
    radius_y -= 0.5;
    center_x = bound_left + radius_x;
    center_y = bound_top + radius_y;

    rect = context_ptr->rect;

    pd_canvas_t* dst = context_ptr->source;
    if (!context_ptr->is_valid) {
        return -1;
    }
    for (yi = 0; yi < rect.height; ++yi) {
        y = ToGeoY(yi, center_y);
        x = ellipse_x(radius_x + 1.0, radius_y + 1.0, y);
        outer_xi = (int)(center_x - x);
        outer_xi = max(0, min(outer_xi, rect.width));
        p = pd_canvas_get_pixel_pointer(dst, rect.x, rect.y + yi);
        for (xi = 0; xi < outer_xi; ++xi, ++p) {
            p->alpha = 0;
        }
        /* If inner ellipse is circle */
        if (radius_x == radius_y) {
            for (; xi < rect.width; ++xi, ++p) {
                x = ToGeoX(xi, center_x);
                d = Sqrt(x * x + y * y) - radius_x;
                if (d >= 1.0) {
                    p->alpha = 0;
                } else if (d >= 0) {
                    p->alpha = smooth_left_pixel(p, d);
                } else {
                    break;
                }
            }
        } else {
            outer_x =
                ToGeoX(ellipse_x(radius_x, radius_y, y), center_x);
            for (; xi < rect.width; ++xi, ++p) {
                x = ToGeoX(xi, center_x);
                d = x - outer_x;
                if (d >= 1.0) {
                    p->alpha = 0;
                } else if (d >= 0) {
                    p->alpha = smooth_left_pixel(p, d);
                } else {
                    break;
                }
            }
        }
    }
    return 0;
}

int border_crop_content(const pd_border_t *border, const pd_rect_t *box,
               pd_context_t* paint) {
    pd_canvas_t canvas;
    pd_rect_t bound, rect;
    
    int radius;
    int bound_top, bound_left;

    pd_context_t context_buffer;
    pd_canvas_t canvas_buffer;
    pd_context_init(&context_buffer);
    pd_canvas_init(&canvas_buffer);
    context_buffer.source = &canvas;
    // crop top left
    radius = border->top_left_radius;
    bound.x = box->x + border->left.width;
    bound.y = box->y + border->top.width;
    bound.width = radius - border->left.width;
    bound.height = radius - border->top.width;
    if (bound.width > 0 && bound.height > 0 &&
        pd_rect_get_overlay_rect(&bound, &paint->rect, &rect)) {
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;
        rect.x -= paint->rect.x;
        rect.y -= paint->rect.y;
        pd_context_quote(&context_buffer, paint, &rect);
        pd_canvas_create(&canvas_buffer, rect.width, rect.height);
        crop_content_corner(&context_buffer, bound_left, bound_top, bound.width,
                   bound.height);
                   
        pd_color_t *src_color_ptr;
        pd_color_t *dst_color_ptr;
        for (int y = 0; y < rect.height; ++y) {
            src_color_ptr = pd_canvas_get_pixel_pointer(&canvas_buffer, 0, y);
            dst_color_ptr = pd_canvas_get_pixel_pointer(&canvas, rect.x, rect.y + y);
            for (int x = 0; x < rect.width; ++x,
                  ++src_color_ptr, ++dst_color_ptr) {
                pd_over_pixel(dst_color_ptr, src_color_ptr);
            }
        }
    }
    // crop top right
    radius = border->top_right_radius;
    bound.x = box->x + box->width - radius;
    bound.y = box->y + border->top.width;
    bound.width = radius - border->right.width;
    bound.height = radius - border->top.width;
    if (bound.width > 0 && bound.height > 0 &&
        pd_rect_get_overlay_rect(&bound, &paint->rect, &rect)) {
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;
        rect.x -= paint->rect.x;
        rect.y -= paint->rect.y;
        pd_context_quote(&context_buffer, paint, &rect);
        pd_canvas_create(&canvas_buffer, rect.width, rect.height);

        crop_content_corner(&context_buffer, bound_left, bound_top, bound.width,
                    bound.height);

        pd_color_t *src_color_ptr;
        pd_color_t *dst_color_ptr;
        for (int y = 0; y < rect.height; ++y) {
            src_color_ptr = pd_canvas_get_pixel_pointer(&canvas_buffer, 0, y);
            dst_color_ptr = pd_canvas_get_pixel_pointer(&canvas, rect.x + rect.width, rect.y);
            for (int x = 0; x < rect.width; ++x,
                  ++src_color_ptr, --dst_color_ptr) {
                pd_over_pixel(dst_color_ptr, src_color_ptr);
            }
        }
    }
    // crop bottom left
    radius = border->bottom_left_radius;
    bound.x = box->x + border->left.width;
    bound.y = box->y + box->height - radius;
    bound.width = radius - border->left.width;
    bound.height = radius - border->bottom.width;
    if (bound.width > 0 && bound.height > 0 &&
        pd_rect_get_overlay_rect(&bound, &paint->rect, &rect)) {
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;
        rect.x -= paint->rect.x;
        rect.y -= paint->rect.y;
        pd_context_quote(&context_buffer, paint, &rect);
        pd_canvas_create(&canvas_buffer, rect.width, rect.height);

        crop_content_corner(&context_buffer, bound_left, bound_top,
                      bound.width, bound.height);

        pd_color_t *src_color_ptr;
        pd_color_t *dst_color_ptr;
        for (int y = 0; y < rect.height; ++y) {
            src_color_ptr = pd_canvas_get_pixel_pointer(&canvas_buffer, 0, y);
            dst_color_ptr = pd_canvas_get_pixel_pointer(&canvas, rect.x, rect.y + rect.height);
            for (int x = 0; x < rect.width; ++x,
                  ++src_color_ptr, ++dst_color_ptr) {
                pd_over_pixel(dst_color_ptr, src_color_ptr);
            }
        }
    }
    // crop bottom right
    radius = border->bottom_right_radius;
    bound.x = box->x + box->width - radius;
    bound.y = box->y + box->height - radius;
    bound.width = radius - border->right.width;
    bound.height = radius - border->bottom.width;
    if (bound.width > 0 && bound.height > 0 &&
        pd_rect_get_overlay_rect(&bound, &paint->rect, &rect)) {
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;
        rect.x -= paint->rect.x;
        rect.y -= paint->rect.y;
        pd_context_quote(&context_buffer, paint, &rect);
        pd_canvas_create(&canvas_buffer, rect.width, rect.height);

        crop_content_corner(&context_buffer, bound_left, bound_top,
                       bound.width, bound.height);

        pd_color_t *src_color_ptr;
        pd_color_t *dst_color_ptr;
        for (int y = 0; y < rect.height; ++y) {
            src_color_ptr = pd_canvas_get_pixel_pointer(&canvas_buffer, 0, y);
            dst_color_ptr = pd_canvas_get_pixel_pointer(&canvas, rect.x + rect.width, rect.y + rect.height);
            for (int x = 0; x < rect.width; ++x,
                  ++src_color_ptr, --dst_color_ptr) {
                pd_over_pixel(dst_color_ptr, src_color_ptr);
            }
        }
    }
    return 0;
}

int pd_border_paint(const pd_border_t *border, const pd_rect_t *box,
         pd_context_t* context_ptr) {
    pd_rect_t bound, rect;

    int bound_top, bound_left;
    
    int tl_width = max(border->top_left_radius, border->left.width);
    int tl_height = max(border->top_left_radius, border->top.width);
    int tr_width = max(border->top_right_radius, border->right.width);
    int tr_height = max(border->top_right_radius, border->top.width);
    int bl_width = max(border->bottom_left_radius, border->left.width);
    int bl_height = max(border->bottom_left_radius, border->bottom.width);
    int br_width = max(border->bottom_right_radius, border->right.width);
    int br_height = max(border->bottom_right_radius, border->bottom.width);

    if (!pd_canvas_is_valid(context_ptr->source)) {
        return -1;
    }
    pd_context_t context_buffer;            // 缓存 context
    pd_canvas_t canvas;                     // 缓存 canvas
    pd_canvas_t* canvas_buffer = &canvas;   // 缓存 canvas的指针

    // 初始化缓存并绑定
    pd_context_init(&context_buffer);
    pd_canvas_init(canvas_buffer);
    context_buffer.source = canvas_buffer;


    /* Draw border top left angle */
    bound.x = box->x;
    bound.y = box->y;
    bound.width = tl_width;
    bound.height = tl_height;
    uint8_t res = pd_rect_get_overlay_rect(&bound, &context_ptr->rect, &rect);
    if (rect.height > 0 && rect.width > 0) {
        // rect为左上角可以绘制的区域
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;

        // 把rect坐标转变成相对context_ptr.rect的坐标
        rect.x -= context_ptr->rect.x;
        rect.y -= context_ptr->rect.y;

        // 根据rect的大小分配canvas_buffer的存储空间

        if(pd_canvas_create(canvas_buffer, rect.width, rect.height) >= 0) {
            pd_context_init(&context_buffer);
            context_buffer.left = bound_left;
            context_buffer.rect.x = bound_left;
            context_buffer.top = bound_top;
            context_buffer.rect.y = bound_top;
            context_buffer.rect.width = rect.width;
            context_buffer.rect.height = rect.height;
            context_buffer.source = canvas_buffer;
            // 把top_left绘制到canvas_buffer中
            
            draw_border_corner(&context_buffer, bound_left, bound_top,
                    &border->top, &border->left, border->top_left_radius);
            pd_color_t *src_color_ptr;
            pd_color_t *dst_color_ptr;
            for (int y = 0; y < rect.height; ++y) {
                src_color_ptr = pd_canvas_get_pixel_pointer(canvas_buffer, 0, y);
                dst_color_ptr = pd_canvas_get_pixel_pointer(context_ptr->source, rect.x, rect.y + y);
                for (int x = 0; x < rect.width; ++x,
                        ++src_color_ptr, ++dst_color_ptr) {
                    pd_over_pixel(dst_color_ptr, src_color_ptr);
                }
            }
        }
    }

    /* Draw border top right angle */
    bound.y = box->y;
    bound.width = tr_width;
    bound.height = tr_height;
    bound.x = box->x + box->width - bound.width;
    res = pd_rect_get_overlay_rect(&bound, &context_ptr->rect, &rect);
    if (rect.height > 0 && rect.width > 0) {
        bound_left = rect.x + rect.width - bound.x - bound.width;
        bound_top = bound.y - rect.y;

        // 把rect坐标转变成相对paint.rect的坐标
        rect.x -= context_ptr->rect.x;
        rect.y -= context_ptr->rect.y;

        pd_canvas_free(canvas_buffer);
        pd_canvas_t canvas2;
        canvas_buffer = &canvas2;
        pd_canvas_init(canvas_buffer);
        if(pd_canvas_create(canvas_buffer, rect.width, rect.height) >= 0) {
            pd_context_init(&context_buffer);
            context_buffer.left = bound_left;
            context_buffer.rect.x = bound_left;
            context_buffer.top = bound_top;
            context_buffer.rect.y = bound_top;
            context_buffer.rect.width = rect.width;
            context_buffer.rect.height = rect.height;
            context_buffer.source = canvas_buffer;
            // 把top_left绘制到canvas_buffer中
            draw_border_corner(&context_buffer, bound_left, bound_top,
                    &border->top, &border->right, border->top_right_radius);

            pd_color_t *src_color_ptr;
            pd_color_t *dst_color_ptr;
            for (int y = 0; y < rect.height; ++y) {
                src_color_ptr = pd_canvas_get_pixel_pointer(canvas_buffer, 0, y);
                dst_color_ptr = pd_canvas_get_pixel_pointer(context_ptr->source, rect.x + rect.width - 1, rect.y + y);
                for (int x = 0; x < rect.width; ++x,
                        ++src_color_ptr, --dst_color_ptr) {
                    pd_over_pixel(dst_color_ptr, src_color_ptr);
                }
            }
        }
        
    }

    /* Draw border bottom left angle */
    bound.x = box->x;
    bound.width = bl_width;
    bound.height = bl_height;
    bound.y = box->y + box->height - bound.height;
    res = pd_rect_get_overlay_rect(&bound, &context_ptr->rect, &rect);
    if (rect.height > 0 && rect.width > 0) {
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;
        rect.x -= context_ptr->rect.x;
        rect.y -= context_ptr->rect.y;
        pd_canvas_free(canvas_buffer);
        pd_canvas_t canvas3;
        canvas_buffer = &canvas3;
        pd_canvas_init(canvas_buffer);
        if(pd_canvas_create(canvas_buffer, rect.width, rect.height) >= 0) {
            pd_context_init(&context_buffer);
            context_buffer.left = bound_left;
            context_buffer.rect.x = bound_left;
            context_buffer.top = bound_top;
            context_buffer.rect.y = bound_top;
            context_buffer.rect.width = rect.width;
            context_buffer.rect.height = rect.height;
            context_buffer.source = canvas_buffer;
            // 把top_left绘制到canvas_buffer中
            draw_border_corner(&context_buffer, bound_left, bound_top,
                    &border->bottom, &border->left, border->bottom_left_radius);

            pd_color_t *src_color_ptr;
            pd_color_t *dst_color_ptr;
            for (int y = 0; y < rect.height; ++y) {
                src_color_ptr = pd_canvas_get_pixel_pointer(canvas_buffer, 0, y);
                dst_color_ptr = pd_canvas_get_pixel_pointer(context_ptr->source, rect.x, rect.y + rect.height - y - 1);
                for (int x = 0; x < rect.width; ++x,
                        ++src_color_ptr, ++dst_color_ptr) {
                    pd_over_pixel(dst_color_ptr, src_color_ptr);
                }
            }
        }
    }

    /* Draw border bottom right angle */
    bound.width = br_width;
    bound.height = br_height;
    bound.x = box->x + box->width - bound.width;
    bound.y = box->y + box->height - bound.height;
    res = pd_rect_get_overlay_rect(&bound, &context_ptr->rect, &rect);
    if (rect.height > 0 && rect.width > 0) {
        bound_left = bound.x - rect.x;
        bound_top = bound.y - rect.y;
        rect.x -= context_ptr->rect.x;
        rect.y -= context_ptr->rect.y;
        pd_canvas_free(canvas_buffer);
        pd_canvas_t canvas4;
        canvas_buffer = &canvas4;
        pd_canvas_init(canvas_buffer);
        if(pd_canvas_create(canvas_buffer, rect.width, rect.height) >= 0) {
            pd_context_init(&context_buffer);
            context_buffer.left = bound_left;
            context_buffer.rect.x = bound_left;
            context_buffer.top = bound_top;
            context_buffer.rect.y = bound_top;
            context_buffer.rect.width = rect.width;
            context_buffer.rect.height = rect.height;
            context_buffer.source = canvas_buffer;
            // 把top_left绘制到canvas_buffer中
            draw_border_corner(&context_buffer, bound_left, bound_top,
                    &border->bottom, &border->right, border->bottom_right_radius);

            pd_color_t *src_color_ptr;
            pd_color_t *dst_color_ptr;
            for (int y = 0; y < rect.height; ++y) {
                src_color_ptr = pd_canvas_get_pixel_pointer(canvas_buffer, 0, y);
                dst_color_ptr = pd_canvas_get_pixel_pointer
                        (context_ptr->source, rect.x + rect.width - 1,
                        rect.y + rect.height - y - 1);
                for (int x = 0; x < rect.width; ++x,
                        src_color_ptr++, dst_color_ptr--) {
                    pd_over_pixel(dst_color_ptr, src_color_ptr);
                }
            }
        }
        
    }

    /* Draw top border line */
    bound.x = box->x + tl_width;
    bound.y = box->y;
    bound.width = box->width - tl_width - tr_width;
    bound.height = border->top.width;
    pd_rect_get_overlay_rect(&bound, &(context_ptr->rect), &bound);
    if (bound.width > 0 && bound.height > 0) {
        bound.x -= context_ptr->rect.x;
        bound.y -= context_ptr->rect.y;
        pd_canvas_fill_rect(context_ptr, border->top.color, &bound, TRUE);
    }
    /* Draw bottom border line */
    bound.x = box->x + bl_width;
    bound.y = box->y + box->height - border->bottom.width;
    bound.width = box->width - bl_width - br_width;
    bound.height = border->bottom.width;
    pd_rect_get_overlay_rect(&bound, &(context_ptr->rect), &bound);
    if (bound.width > 0 && bound.height > 0) {
        bound.x -= context_ptr->rect.x;
        bound.y -= context_ptr->rect.y;
        pd_canvas_fill_rect(context_ptr, border->bottom.color, &bound, TRUE);
    }
    /* Draw left border line */
    bound.y = box->y + tl_height;
    bound.x = box->x;
    bound.width = border->left.width;
    bound.height = box->height - tl_height - bl_height;
    pd_rect_get_overlay_rect(&bound, &(context_ptr->rect), &bound);
    if (bound.width > 0 && bound.height > 0) {
        bound.x -= context_ptr->rect.x;
        bound.y -= context_ptr->rect.y;
        pd_canvas_fill_rect(context_ptr, border->left.color, &bound, TRUE);
    }
    /* Draw right border line */
    bound.x = box->x + box->width - border->right.width;
    bound.y = box->y + tr_height;
    bound.width = border->right.width;
    bound.height = box->height - tr_height - br_height;
    pd_rect_get_overlay_rect(&bound, &(context_ptr->rect), &bound);
    if (bound.width > 0 && bound.height > 0) {
        bound.x -= context_ptr->rect.x;
        bound.y -= context_ptr->rect.y;
        pd_canvas_fill_rect(context_ptr, border->right.color, &bound, TRUE);
    }
    return 0;
}