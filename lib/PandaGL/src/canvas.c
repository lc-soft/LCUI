/* canvas.c -- The graphics processing module.
 *
 * Copyright to be defined
 *
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <PandaGL/canvas.h>
#include <PandaGL/util/rect.h>

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>

void pd_canvas_print_info(pd_canvas_t *canvas)
{
    printf("address:%p\n", canvas);
    if (!canvas) {
        return;
    }

    printf("width:%d, ", canvas->width);
    printf("height:%d, ", canvas->height);
    printf("opacity:%.2f, ", canvas->opacity);
}

void pd_context_create(pd_context_t* context_ptr){
    context_ptr->is_valid = FALSE;
    context_ptr->top = 0;
    context_ptr->left = 0;
    pd_canvas_t canvas;
    pd_canvas_init(&canvas);
    context_ptr->source = &canvas;
}

void pd_context_init(pd_context_t* context_ptr){
    context_ptr->is_valid = FALSE;
    context_ptr->source = NULL;
    context_ptr->top = 0;
    context_ptr->left = 0;
    context_ptr->rect.x = 0;
    context_ptr->rect.y = 0;
    context_ptr->rect.width = 0;
    context_ptr->rect.height = 0;
    context_ptr->with_alpha = TRUE;
    context_ptr->is_writable = FALSE;
}

void pd_canvas_init(pd_canvas_t *canvas)
{
    canvas->palette = NULL;
    canvas->color_type = PD_COLOR_TYPE_ARGB;
    canvas->bytes = NULL;
    canvas->opacity = 1.0;
    canvas->mem_size = 0;
    canvas->width = 0;
    canvas->height = 0;
    canvas->bytes_per_pixel = 4;
    canvas->bytes_per_row = 0;
}

pd_color_t pd_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    pd_color_t color;
    color.red = r;
    color.green = g;
    color.blue = b;
    color.alpha = 255;
    return color;
}

pd_color_t pd_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    pd_color_t color;
    color.alpha = a;
    color.red = r;
    color.green = g;
    color.blue = b;
    return color;
}

INLINE uint8_t pd_get_pixel_size(int color_type)
{
    switch (color_type) {
    case PD_COLOR_TYPE_INDEX8:
    case PD_COLOR_TYPE_GRAY8:
    case PD_COLOR_TYPE_RGB323:
    case PD_COLOR_TYPE_ARGB2222:
        return 1;
    case PD_COLOR_TYPE_RGB555:
    case PD_COLOR_TYPE_RGB565:
        return 2;
    case PD_COLOR_TYPE_ARGB:
    default:
        break;
    }
    return 4;
}

/*---------------------------------- ARGB ----------------------------------*/

static int pd_canvas_cut_argb(const pd_canvas_t *canvas, pd_rect_t rect,
             pd_canvas_t *buff)
{
    int x, y;
    pd_color_t *pixel_src, *pixel_des;

    buff->color_type = canvas->color_type;
    if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
        return -1;
    }

    buff->opacity = canvas->opacity;
    for (y = 0; y < rect.height; ++y) {
        pixel_des = buff->argb + y * buff->width;
        pixel_src = canvas->argb;
        pixel_src += (rect.y + y) * canvas->width + rect.x;
        for (x = 0; x < rect.width; ++x) {
            *pixel_des++ = *pixel_src++;
        }
    }
    return 0;
}

/* FIXME: improve alpha blending method
 * Existing alpha blending methods are inefficient and need to be optimized
 */

static void pd_canvas_mix_argb_with_alpha(pd_canvas_t *dst, pd_rect_t des_rect,
                   const pd_canvas_t *src, int src_x, int src_y)
{
    int x, y;
    pd_color_t *px_src, *px_dst;
    pd_color_t *px_row_src, *px_row_des;
    double a, out_a, out_r, out_g, out_b, src_a;
    px_row_src = src->argb + src_y * src->width + src_x;
    px_row_des = dst->argb + des_rect.y * dst->width + des_rect.x;
    if (src->opacity < 1.0) {
        goto mix_with_opacity;
    }
    for (y = 0; y < des_rect.height; ++y) {
        px_src = px_row_src;
        px_dst = px_row_des;
        for (x = 0; x < des_rect.width; ++x) {
            src_a = px_src->a / 255.0;
            a = (1.0 - src_a) * px_dst->a / 255.0;
            out_r = px_dst->r * a + px_src->r * src_a;
            out_g = px_dst->g * a + px_src->g * src_a;
            out_b = px_dst->b * a + px_src->b * src_a;
            out_a = src_a + a;
            if (out_a > 0) {
                out_r /= out_a;
                out_g /= out_a;
                out_b /= out_a;
            }
            px_dst->r = (uint8_t)(out_r + 0.5);
            px_dst->g = (uint8_t)(out_g + 0.5);
            px_dst->b = (uint8_t)(out_b + 0.5);
            px_dst->a = (uint8_t)(255.0 * out_a + 0.5);
            ++px_src;
            ++px_dst;
        }
        px_row_des += dst->width;
        px_row_src += src->width;
    }
    return;

mix_with_opacity:
    for (y = 0; y < des_rect.height; ++y) {
        px_src = px_row_src;
        px_dst = px_row_des;
        for (x = 0; x < des_rect.width; ++x) {
            src_a = px_src->a / 255.0 * src->opacity;
            a = (1.0 - src_a) * px_dst->a / 255.0;
            out_r = px_dst->r * a + px_src->r * src_a;
            out_g = px_dst->g * a + px_src->g * src_a;
            out_b = px_dst->b * a + px_src->b * src_a;
            out_a = src_a + a;
            if (out_a > 0) {
                out_r /= out_a;
                out_g /= out_a;
                out_b /= out_a;
            }
            px_dst->r = (uint8_t)(out_r + 0.5);
            px_dst->g = (uint8_t)(out_g + 0.5);
            px_dst->b = (uint8_t)(out_b + 0.5);
            px_dst->a = (uint8_t)(255.0 * out_a + 0.5);
            ++px_src;
            ++px_dst;
        }
        px_row_des += dst->width;
        px_row_src += src->width;
    }
}

static void pd_canvas_mix_argb(pd_canvas_t *dest, pd_rect_t des_rect,
              const pd_canvas_t *src, int src_x, int src_y)
{
    int x, y;
    uint8_t a;
    pd_color_t *px_src, *px_dest;
    pd_color_t *px_row_src, *px_row_des;
    px_row_src = src->argb + src_y * src->width + src_x;
    px_row_des = dest->argb + des_rect.y * dest->width + des_rect.x;
    if (src->opacity < 1.0) {
        goto mix_with_opacity;
    }
    for (y = 0; y < des_rect.height; ++y) {
        px_src = px_row_src;
        px_dest = px_row_des;
        for (x = 0; x < des_rect.width; ++x) {
            PIXEL_BLEND(px_dest, px_src, px_src->a);
            ++px_src;
            ++px_dest;
        }
        px_row_des += dest->width;
        px_row_src += src->width;
    }
    return;

mix_with_opacity:
    for (y = 0; y < des_rect.height; ++y) {
        px_src = px_row_src;
        px_dest = px_row_des;
        for (x = 0; x < des_rect.width; ++x) {
            a = (uint8_t)(px_src->a * src->opacity);
            PIXEL_BLEND(px_dest, px_src, a);
            ++px_src;
            ++px_dest;
        }
        px_row_des += dest->width;
        px_row_src += src->width;
    }
}

static int pd_canvas_replace_argb(pd_canvas_t *des, pd_rect_t des_rect,
                 const pd_canvas_t *src, int src_x, int src_y)
{
    int x, y, row_size;
    pd_color_t *px_row_src, *px_row_des, *px_src, *px_des;
    px_row_src = src->argb + src_y * src->width + src_x;
    px_row_des = des->argb + des_rect.y * des->width + des_rect.x;
    if (src->opacity - 1.0f < 0.01f) {
        row_size = sizeof(pd_color_t) * des_rect.width;
        for (y = 0; y < des_rect.height; ++y) {
            memcpy(px_row_des, px_row_src, row_size);
            px_row_src += src->width;
            px_row_des += des->width;
        }
        return 0;
    }
    for (y = 0; y < des_rect.height; ++y) {
        px_src = px_row_src;
        px_des = px_row_des;
        for (x = 0; x < des_rect.width; ++x) {
            px_des->b = px_src->b;
            px_des->g = px_src->g;
            px_des->r = px_src->r;
            px_des->a = (uint8_t)(src->opacity * px_src->a);
        }
        px_row_src += src->width;
        px_row_des += des->width;
    }
    return 0;
}

static int pd_canvas_honrizontal_flip_argb(pd_context_t *context, pd_context_t *context_buff)
{
    pd_canvas_t* canvas = context->source;
    pd_canvas_t* buff = context_buff->source;
    int x, y;
    pd_rect_t rect = context->rect;
    pd_color_t *pixel_src, *pixel_des;

    if (!pd_canvas_is_valid(canvas)) {
        return -1;
    }
    buff->opacity = canvas->opacity;
    buff->color_type = canvas->color_type;
    if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
        return -2;
    }

    for (y = 0; y < rect.height; ++y) {
        pixel_des = buff->argb + y * buff->width;
        pixel_src = canvas->argb + (rect.y + y) * canvas->width;
        pixel_src += rect.x + rect.width - 1;
        for (x = 0; x < rect.width; ++x) {
            *pixel_des++ = *pixel_src--;
        }
    }
    return 0;
}

static int pd_canvas_vertical_flip_argb(pd_context_t *context, pd_context_t *context_buff)
{
    pd_canvas_t* canvas = context->source;
    pd_canvas_t* buff = context_buff->source;
    int y;
    pd_rect_t rect = context->rect;
    uint8_t *byte_src, *byte_des;

    if (!pd_canvas_is_valid(canvas)) {
        return -1;
    }
    buff->opacity = canvas->opacity;
    buff->color_type = canvas->color_type;
    if (0 != pd_canvas_create(buff, rect.width, rect.height)) {
        return -2;
    }
    byte_src = canvas->bytes;
    byte_src += (rect.y + rect.height - 1) * canvas->bytes_per_row;
    byte_src += rect.x * canvas->bytes_per_pixel;
    byte_des = buff->bytes;
    for (y = 0; y < rect.height; ++y) {
        memcpy(byte_des, byte_src, buff->bytes_per_row);
        byte_src -= canvas->bytes_per_row;
        byte_des += buff->bytes_per_row;
    }
    return 0;
}

static int pd_canvas_fill_rect_argb(pd_context_t* context_ptr, pd_color_t color,
                  pd_rect_t rect, pd_bool_t with_alpha)
{
    int x, y;
    pd_color_t *pixel, *pixel_row;
    if (!pd_context_is_valid(context_ptr)) {
        return -1;
    }
    
    pixel_row = context_ptr->source->argb + rect.y * context_ptr->source->width + rect.x;
    if (with_alpha) {
        for (y = 0; y < rect.height; ++y) {
            pixel = pixel_row;
            for (x = 0; x < rect.width; ++x) {
                *pixel++ = color;
            }
            pixel_row += context_ptr->source->width;
        }
    } else {
        for (y = 0; y < rect.height; ++y) {
            pixel = pixel_row;
            for (x = 0; x < rect.width; ++x) {
                color.alpha = pixel->alpha;
                *pixel++ = color;
            }
            pixel_row += context_ptr->source->width;
        }
    }
    return 0;
}

INLINE uint8_t pd_canvas_billinear_resamping_core(uint8_t a, uint8_t b, uint8_t c,
                        uint8_t d, float dx, float dy)
{
    return (uint8_t)(a * (1 - dx) * (1 - dy) + b * (dx) * (1 - dy) +
             c * (dy) * (1 - dx) + d * (dx * dy));
}

/*-------------------------------- End ARGB --------------------------------*/

int pd_canvas_create(pd_canvas_t *canvas, unsigned width, unsigned height)
{
    size_t size;
    if (width > 10000 || height > 10000) {
        abort();
    }
    if (width < 1 || height < 1) {
        pd_canvas_free(canvas);
        return -1;
    }
    canvas->bytes_per_pixel = pd_get_pixel_size(canvas->color_type);
    canvas->bytes_per_row = canvas->bytes_per_pixel * width;
    size = canvas->bytes_per_row * height;
    if (pd_canvas_is_valid(canvas)) {
        /* 如果现有图形尺寸大于要创建的图形的尺寸，直接改尺寸即可 */
        if (canvas->mem_size >= size) {
            memset(canvas->bytes, 0, canvas->mem_size);
            canvas->width = width;
            canvas->height = height;
            return 0;
        }
        pd_canvas_free(canvas);
    }
    canvas->mem_size = size;
    canvas->bytes = calloc(1, size);
    if (!canvas->bytes) {
        canvas->width = 0;
        canvas->height = 0;
        return -2;
    }
    canvas->width = width;
    canvas->height = height;
    return 0;
}

pd_bool_t pd_context_is_valid(const pd_context_t* context_ptr){
    if(context_ptr->source != NULL && context_ptr->source->bytes != NULL &&
        context_ptr->source->height > 0 && context_ptr->source->width > 0)
        return TRUE;
    return FALSE;
}

pd_bool_t pd_canvas_is_valid(const pd_canvas_t *canvas)
{
    return canvas->bytes && canvas->height > 0 && canvas->width > 0;
}

void pd_canvas_copy(pd_context_t *context_des, const pd_context_t *context)
{
    pd_canvas_t *des = context_des->source;
    const pd_canvas_t *canvas;
    if (!des || !pd_context_is_valid(context)) {
        return;
    }
    canvas = context->source;
    des->color_type = canvas->color_type;
    /* 创建合适尺寸的Graph */
    pd_canvas_create(des, canvas->width, canvas->height);
    pd_canvas_replace(context_des, context, 0, 0);
    des->opacity = canvas->opacity;
}

void pd_canvas_free(pd_canvas_t *canvas)
{
    /* 解除引用 */
    if (canvas->bytes) {
        // free(canvas->bytes);
        canvas->bytes = NULL;
    }
    canvas->width = 0;
    canvas->height = 0;
    canvas->mem_size = 0;
}

void pd_context_free(pd_context_t *context)
{
    /* 解除引用 */
    context->is_valid = FALSE;
    context->is_writable = FALSE;
    context->left = 0;
    context->top = 0;
    context->rect.x = 0;
    context->rect.y = 0;
    context->rect.height = 0;
    context->rect.width = 0;
    pd_canvas_free(context->source);
}

int pd_canvas_quote_read_only(pd_context_t* self, const pd_context_t* source,
            const pd_rect_t *rect)
{
    pd_rect_t quote_rect;
    if (!rect) {
        quote_rect.x = 0;
        quote_rect.y = 0;
        quote_rect.width = source->source->width;
        quote_rect.height = source->source->height;
    } else {
        quote_rect = *rect;
    }

    if (!self->source) {
        pd_canvas_t canvas;
        pd_canvas_init(&canvas);
        self->source = &canvas;
    }
    
    pd_rect_validate_area(&quote_rect, source->source->width, source->source->height);
    /* 如果引用源本身已经引用了另一个源 */
    if (source->is_valid) {
        quote_rect.x += source->left;
        quote_rect.y += source->top;
        // source = source->quote.source;       // Problem：source已经标记了const，还进行了操作
    }
    if (quote_rect.width <= 0 || quote_rect.height <= 0) {
        self->source->width = 0;
        self->source->height = 0;
        self->source->opacity = 1.0f;
        self->left = 0;
        self->top = 0;
        self->source->bytes = NULL;
        self->source = NULL;
        self->is_valid = FALSE;
        return -EINVAL;
    }
    self->source->opacity = 1.0;
    self->source->bytes = NULL;
    self->source->mem_size = 0;
    self->source->width = quote_rect.width;
    self->source->height = quote_rect.height;
    self->source->color_type = source->source->color_type;
    self->source->bytes_per_pixel = source->source->bytes_per_pixel;
    self->source->bytes_per_row = source->source->bytes_per_row;
    self->is_valid = TRUE;
    self->source_ro = source->source_ro;
    self->left = quote_rect.x;
    self->top = quote_rect.y;
    self->is_writable = FALSE;
    self->rect.x = quote_rect.x;
    self->rect.y = quote_rect.x;
    self->rect.width = quote_rect.width;
    self->rect.height = quote_rect.height;
    return 0;
}

int pd_context_quote(pd_context_t* self, pd_context_t* source, const pd_rect_t *rect)
{
    int ret = pd_canvas_quote_read_only(self, source, rect);
    self->is_writable = TRUE;
    return ret;
}

// to be deleted
// void pd_canvas_get_valid_rect(const pd_canvas_t *canvas, pd_rect_t *rect) 
// {
//     if (canvas->quote.is_valid) {
//         rect->x = canvas->quote.left;
//         rect->y = canvas->quote.top;
//     } else {
//         rect->x = 0;
//         rect->y = 0;
//     }
//     rect->width = canvas->width;
//     rect->height = canvas->height;
// }

int pd_canvas_set_alpha(pd_canvas_t *canvas, uint8_t *a, size_t size)
{
    size_t i;
    if (size > (size_t)(canvas->width * canvas->height)) {
        size = (size_t)(canvas->width * canvas->height);
    }
    if (canvas->color_type != PD_COLOR_TYPE_ARGB) {
        return -2;
    }
    for (i = 0; i < size; ++i) {
        canvas->argb[i].a = a[i];
    }
    return 0;
}

int pd_canvas_zoom(const pd_context_t *context, pd_context_t *context_buff, pd_bool_t keep_scale,
           int width, int height)
{
    pd_canvas_t* canvas = context->source;
    pd_canvas_t* buff = context_buff->source;

    pd_rect_t rect = context->rect;
    int x, y, src_x, src_y;
    double scale_x = 0.0, scale_y = 0.0;
    if (!pd_canvas_is_valid(canvas) || (width <= 0 && height <= 0)) {
        return -1;
    }
    if (width > 0) {
        scale_x = 1.0 * rect.width / width;
    }
    if (height > 0) {
        scale_y = 1.0 * rect.height / height;
    }
    if (width <= 0) {
        scale_x = scale_y;
        width = (int)(0.5 + 1.0 * canvas->width / scale_x);
    }
    if (height <= 0) {
        scale_y = scale_x;
        height = (int)(0.5 + 1.0 * canvas->height / scale_y);
    }
    /* 如果保持宽高比 */
    if (keep_scale) {
        if (scale_x < scale_y) {
            scale_y = scale_x;
        } else {
            scale_x = scale_y;
        }
    }
    buff->color_type = canvas->color_type;
    if (pd_canvas_create(buff, width, height) < 0) {
        return -2;
    }
    if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
        pd_color_t *px_src, *px_des, *px_row_src;
        for (y = 0; y < height; ++y) {
            src_y = (int)(y * scale_y);
            px_row_src = canvas->argb;
            px_row_src += (src_y + rect.y) * canvas->width + rect.x;
            px_des = buff->argb + y * width;
            for (x = 0; x < width; ++x) {
                src_x = (int)(x * scale_x);
                px_src = px_row_src + src_x;
                *px_des++ = *px_src;
            }
        }
    }
    return 0;
}

int pd_canvas_zoom_bilinear(const pd_context_t *context, pd_context_t *context_buff,
               pd_bool_t keep_scale, int width, int height)
{
    pd_canvas_t *canvas = context->source;
    pd_canvas_t *buff = context_buff->source;
    pd_rect_t rect;
    pd_color_t a, b, c, d, t_color;

    int x, y, i, j;
    float x_diff, y_diff;
    double scale_x = 0.0, scale_y = 0.0;

    if (canvas->color_type != PD_COLOR_TYPE_ARGB) {
        /* fall back to nearest scaling */
        return pd_canvas_zoom(context, context_buff, keep_scale, width, height);
    }
    if (!pd_canvas_is_valid(canvas) || (width <= 0 && height <= 0)) {
        return -1;
    }
    rect = context->rect;
    if (width > 0) {
        scale_x = 1.0 * rect.width / width;
    }
    if (height > 0) {
        scale_y = 1.0 * rect.height / height;
    }
    if (width <= 0) {
        scale_x = scale_y;
        width = (int)(0.5 + 1.0 * canvas->width / scale_x);
    }
    if (height <= 0) {
        scale_y = scale_x;
        height = (int)(0.5 + 1.0 * canvas->height / scale_y);
    }
    /* 如果保持宽高比 */
    if (keep_scale) {
        if (scale_x < scale_y) {
            scale_y = scale_x;
        } else {
            scale_x = scale_y;
        }
    }
    buff->color_type = canvas->color_type;
    if (pd_canvas_create(buff, width, height) < 0) {
        return -2;
    }
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            /*
             * Qmn = (m, n).
             * Qxy1 = (x2 - x) / (x2 - x1) * Q11 + (x - x1) / (x2
             * - x1) * Q21
             * Qxy2 = (x2 - x) / (x2 - x1) * Q12 + (x - x1) / (x2
             * - x1) * Q22
             * Qxy = (y2 - y) / (y2 - y1) * Qxy1 + (y - y1) / (y2 -
             * y1) * Qxy2
             */
            x = (int)(scale_x * j);
            y = (int)(scale_y * i);
            x_diff = (float)((scale_x * j) - x);
            y_diff = (float)((scale_y * i) - y);
            pd_canvas_get_pixel(canvas, x + rect.x + 0, y + rect.y + 0,
                       a);
            pd_canvas_get_pixel(canvas, x + rect.x + 1, y + rect.y + 0,
                       b);
            pd_canvas_get_pixel(canvas, x + rect.x + 0, y + rect.y + 1,
                       c);
            pd_canvas_get_pixel(canvas, x + rect.x + 1, y + rect.y + 1,
                       d);
            t_color.b = pd_canvas_billinear_resamping_core(
                a.b, b.b, c.b, d.b, x_diff, y_diff);
            t_color.g = pd_canvas_billinear_resamping_core(
                a.g, b.g, c.g, d.g, x_diff, y_diff);
            t_color.r = pd_canvas_billinear_resamping_core(
                a.r, b.r, c.r, d.r, x_diff, y_diff);
            t_color.a = pd_canvas_billinear_resamping_core(
                a.a, b.a, c.a, d.a, x_diff, y_diff);
            pd_canvas_set_pixel(buff, j, i, t_color);
        }
    }
    return 0;
}

int pd_canvas_cut(const pd_context_t *context, pd_rect_t rect, pd_context_t *context_buff)
{
    pd_canvas_t *canvas = context->source;
    pd_canvas_t *buff = context_buff->source;
    if (!pd_canvas_is_valid(canvas)) {
        return -2;
    }
    pd_rect_validate_area(&rect, canvas->width, canvas->height);
    if (rect.width <= 0 || rect.height <= 0) {
        return -3;
    }
    switch (canvas->color_type) {
    case PD_COLOR_TYPE_ARGB:
        return pd_canvas_cut_argb(canvas, rect, buff);
    default:
        break;
    }
    return -4;
}

int pd_canvas_horizontal_flip(pd_context_t *context, pd_context_t *buff)
{
    switch (context->source->color_type) {
    case PD_COLOR_TYPE_ARGB:
        return pd_canvas_honrizontal_flip_argb(context, buff);
    default:
        break;
    }
    return -1;
}
#define pd_canvas_hflip pd_canvas_horizontal_flip

int pd_canvas_vertical_flip(pd_context_t *context, pd_context_t *buff)
{
    switch (context->source->color_type) {
    case PD_COLOR_TYPE_ARGB:
        return pd_canvas_vertical_flip_argb(context, buff);
    default:
        break;
    }
    return -1;
}
#define pd_canvas_vflip pd_canvas_vertical_flip

int pd_canvas_fill_rect(pd_context_t* context, pd_color_t color, pd_rect_t *rect,
           pd_bool_t with_alpha)
{
    pd_rect_t rect2;
    if (rect) {
        rect2 = *rect;
    } else {
        rect2.x = rect2.y = 0;
        rect2.width = context->source->width;
        rect2.height = context->source->height;
    }
    return pd_canvas_fill_rect_argb(context, color, rect2, with_alpha);
}

int pd_canvas_fill_alpha(pd_context_t* context, uint8_t alpha)
{
    pd_canvas_t *canvas = context->source;
    int x, y;
    pd_rect_t rect = context->rect;
    pd_color_t *pixel, *pixel_row;

    if (!pd_canvas_is_valid(canvas)) {
        return -1;
    }
    pixel_row = canvas->argb + rect.y * canvas->width + rect.x;
    for (y = 0; y < rect.height; ++y) {
        pixel = pixel_row;
        for (x = 0; x < rect.width; ++x) {
            pixel->alpha = alpha;
            ++pixel;
        }
        pixel_row += canvas->width;
    }
    return 0;
}

int pd_canvas_tile(pd_context_t *context_buff, const pd_context_t* context, pd_bool_t replace,
           pd_bool_t with_alpha)
{
    pd_canvas_t *buff = context_buff->source;
    pd_canvas_t *canvas = context->source;
    int ret = 0;
    unsigned x, y;

    if (!pd_canvas_is_valid(canvas) || !pd_canvas_is_valid(buff)) {
        return -1;
    }
    for (y = 0; y < buff->height; y += canvas->height) {
        for (x = 0; x < buff->width; x += canvas->width) {
            if (replace) {
                ret += pd_canvas_replace(context_buff, context, y, x);
                continue;
            }
            ret += pd_canvas_mix(context_buff, context, y, x, with_alpha);
        }
    }
    return ret;
}

typedef void (*MixerPtr)(pd_canvas_t *, pd_rect_t, const pd_canvas_t *, int, int);

int pd_canvas_mix(pd_context_t *context_back, const pd_context_t *context_fore, int left, int top,
          pd_bool_t with_alpha)
{
    pd_canvas_t *back = context_back->source;
    pd_canvas_t *fore = context_fore->source;
    pd_context_t *w_slot;
    pd_rect_t r_rect, w_rect;
    MixerPtr mixer = NULL;

    if (!context_back->is_writable || !pd_canvas_is_valid(fore)) {
        return -1;
    }
    w_rect.x = left;
    w_rect.y = top;
    w_rect.width = fore->width;
    w_rect.height = fore->height;
    pd_rect_get_cut_area(back->width, back->height, w_rect, &r_rect);
    w_rect.x += r_rect.x;
    w_rect.y += r_rect.y;
    w_rect.width = r_rect.width;
    w_rect.height = r_rect.height;
    pd_context_quote(w_slot, context_back, &w_rect);
    /* 获取实际操作区域 */
    w_rect = w_slot->rect;
    r_rect = context_fore->rect;
    if (w_rect.width <= 0 || w_rect.height <= 0 || r_rect.width <= 0 ||
        r_rect.height <= 0) {
        return -2;
    }
    top = r_rect.y;
    left = r_rect.x;
    switch (fore->color_type) {
    case PD_COLOR_TYPE_ARGB:
        if (back->color_type == PD_COLOR_TYPE_ARGB) {
            if (with_alpha) {
                mixer = pd_canvas_mix_argb_with_alpha;
            } else {
                mixer = pd_canvas_mix_argb;
            }
        }
    default:
        break;
    }
    if (mixer) {
        mixer(back, w_rect, fore, left, top);
        return 0;
    }
    return -3;
}

int pd_canvas_replace(pd_context_t *context_back, const pd_context_t *context_fore, int left, int top)
{
    pd_canvas_t *back = context_back->source;
    pd_canvas_t *fore = context_fore->source;
    pd_canvas_t write_slot;
    pd_context_t *write_context;
    pd_context_init(write_context);
    pd_canvas_init(&write_slot);
    write_context->source = &write_slot;
    pd_rect_t read_rect, write_rect;

    if (!context_back->is_writable || !pd_canvas_is_valid(fore)) {
        return -1;
    }
    write_rect.x = left;
    write_rect.y = top;
    write_rect.width = fore->width;
    write_rect.height = fore->height;
    pd_context_quote(write_context, context_back, &write_rect);
    write_rect = write_context->rect;
    read_rect = context_fore->rect;
    if (write_rect.width <= 0 || write_rect.height <= 0 ||
        read_rect.width <= 0 || read_rect.height <= 0) {
        return -2;
    }
    left = read_rect.x;
    top = read_rect.y;
    switch (fore->color_type) {
    case PD_COLOR_TYPE_ARGB:
        pd_canvas_replace_argb(back, write_rect, fore, left, top);
    default:
        break;
    }
    return -1;
}
