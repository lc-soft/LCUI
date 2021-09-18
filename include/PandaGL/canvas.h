// Copyright to be defined

#ifndef PANDAGL_INCLUDE_PANDAGL_CANVAS_H
#define PANDAGL_INCLUDE_PANDAGL_CANVAS_H

#include <PandaGL/types.h>
#include <PandaGL/build.h>

/* 解除RGB宏 */
#ifdef RGB
#undef RGB
#endif

/* 将两个像素点的颜色值进行alpha混合 */
#define _ALPHA_BLEND(__back__, __fore__, __alpha__) \
    ((((__fore__ - __back__) * (__alpha__)) >> 8) + __back__)

#define ALPHA_BLEND(__back__, __fore__, __alpha__)                      \
    {                                                               \
        __back__ = _ALPHA_BLEND(__back__, __fore__, __alpha__); \
    }

#define PIXEL_BLEND(px1, px2, a)                    \
    {                                           \
        ALPHA_BLEND((px1)->r, (px2)->r, a); \
        ALPHA_BLEND((px1)->g, (px2)->g, a); \
        ALPHA_BLEND((px1)->b, (px2)->b, a); \
    }

INLINE void pd_canvas_set_pixel(pd_canvas_t* canvas,
          int x, int y, pd_color_t color) {
    if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
        canvas->argb[canvas->width * y + x] = color;
    }
}
INLINE void pd_canvas_set_pixel_alpha(pd_canvas_t* canvas,
          int x, int y, uint8_t alpha) {
    canvas->argb[canvas->width * y + x].alpha = alpha;
}

INLINE void pd_canvas_get_pixel(pd_canvas_t* canvas, int x,
          int y, pd_color_t color) {
    if (canvas->color_type == PD_COLOR_TYPE_ARGB) {
        color = canvas->argb[canvas->width * (y % canvas->height) +
                (x % canvas->width)];
    }
}

INLINE pd_color_t* pd_canvas_get_pixel_pointer(pd_canvas_t* canvas, int x, int y) {
    return (canvas->argb + canvas->width * y + x);
}

/*
 * Pixel over operator with alpha channel
 * See more: https://en.wikipedia.org/wiki/Alpha_compositing
 */
INLINE void pd_over_pixel(pd_color_t *dst, const pd_color_t *src)
{
    /*
     * Original formula:
     *   Co = (Ca * aa + Cb * ab * (1 - aa)) / (aa + ab * (1 - aa))
     *   ao = aa + ab * (1 - aa)
     *
     * Variable full name:
     *   Co => colorOut
     *   Ca => colorA
     *   Cb => colorB
     *   aa => colorA.alpha
     *   ab => colorB.alpha
     *   ao => colorOut.alpha
     *
     * The formula used in the code:
     *   ai = ab * (1 - aa)
     *   Co = (Ca * aa + Cb * ai) / (aa + ai)
     *   ao = aa + ai
     */
    double src_a = src->a / 255.0;
    double a = (1.0 - src_a) * dst->a / 255.0;
    double out_a = src_a + a;

    if (out_a > 0) {
        src_a /= out_a;
        a /= out_a;
    }
    dst->r = (uint8_t)(src->r * src_a + dst->r * a);
    dst->g = (uint8_t)(src->g * src_a + dst->g * a);
    dst->b = (uint8_t)(src->b * src_a + dst->b * a);
    dst->a = (uint8_t)(255.0 * out_a);

    /* If it is assumed that all color values are premultiplied by their
     * alpha values, we can rewrite the equation for output color as:

        const double a = 1.0 - src->a / 255.0;
        dst->r = src->r + (uint8_t)(dst->r * a);
        dst->g = src->g + (uint8_t)(dst->g * a);
        dst->b = src->b + (uint8_t)(dst->b * a);
        dst->a = src->a + (uint8_t)(dst->a * a);
    */
}

PD_API void pd_canvas_print_info(pd_canvas_t *canvas);

/**
 * create a context and a new source:pd_canvas_t* inside
 */
PD_API void pd_context_create(pd_context_t* context_ptr);

/**
 * create a new context with null variable source:pd_canvas_t*
 */
PD_API void pd_context_init(pd_context_t* context_ptr);

PD_API void pd_canvas_init(pd_canvas_t *canvas);

PD_API pd_color_t pd_rgb(uint8_t r, uint8_t g, uint8_t b);

PD_API pd_color_t pd_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

PD_API int pd_canvas_create(pd_canvas_t *canvas, unsigned width, unsigned height);

PD_API void pd_canvas_copy(pd_context_t *context_des, const pd_context_t *context);

PD_API void pd_canvas_free(pd_canvas_t *canvas);

PD_API void pd_context_free(pd_context_t *context);

/**
 * 为图像创建一个引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
PD_API int pd_context_quote(pd_context_t* self, pd_context_t* source,
             const pd_rect_t *rect);

/**
 * 为图像创建一个只读引用
 * @param self 用于存放图像引用的缓存区
 * @param source 引用的源图像
 * &param rect 引用的区域，若为NULL，则引用整个图像
 */
PD_API int pd_canvas_quote_read_only(pd_context_t* self, const pd_context_t* source,
                 const pd_rect_t *rect);

PD_API pd_bool_t pd_context_is_valid(const pd_context_t* context_ptr);

PD_API pd_bool_t pd_canvas_is_valid(const pd_canvas_t *canvas);

// PD_API void pd_canvas_get_valid_rect(const pd_canvas_t *canvas, pd_rect_t *rect);  //to be deleted

PD_API int pd_canvas_set_alpha(pd_canvas_t *canvas, uint8_t *a, size_t size);

PD_API int pd_canvas_zoom(const pd_context_t *context, pd_context_t *context_buff,
                pd_bool_t keep_scale, int width, int height);

PD_API int pd_canvas_zoom_bilinear(const pd_context_t *context, pd_context_t *context_buff,
               pd_bool_t keep_scale, int width, int height);

PD_API int pd_canvas_cut(const pd_context_t *context, pd_rect_t rect,
                pd_context_t *context_buff);

PD_API int pd_canvas_horizontal_flip(pd_context_t *context, pd_context_t *buff);

PD_API int pd_canvas_vertical_flip(pd_context_t *context, pd_context_t *buff);

/**
 * 用颜色填充一块区域
 * @param[in][out] context 需要填充的图层的上下文
 * @param[in] color 颜色
 * @param[in] rect 区域，若值为 NULL，则填充整个图层
 * @param[in] with_alpha 是否需要处理alpha通道
 */
PD_API int pd_canvas_fill_rect(pd_context_t* context, pd_color_t color,
            pd_rect_t *rect, pd_bool_t with_alpha);

PD_API int pd_canvas_fill_alpha(pd_context_t* context, uint8_t alpha);

PD_API int pd_canvas_tile(pd_context_t *context_buff, const pd_context_t* context,
            pd_bool_t replace, pd_bool_t with_alpha);

/**
 * 混合两张图层
 * 将前景图混合到背景图上
 * @param[in][out] context_back 背景图层的上下文
 * @param[in] context_fore 前景图层的上下文
 * @param[in] left 前景图层的左边距
 * @param[in] top 前景图层的上边距
 * @param[in] with_alpha 是否需要处理alpha通道
 */
PD_API int pd_canvas_mix(pd_context_t *context_back, const pd_context_t *context_fore,
            int left, int top, pd_bool_t with_alpha);

PD_API int pd_canvas_replace(pd_context_t *context_back,
            const pd_context_t *context_fore, int left, int top);


INLINE pd_bool_t pd_context_is_writable(pd_context_t* context_ptr) {
    return (pd_context_is_valid(context_ptr) &&
      context_ptr->is_writable);
}


#endif  // !PANDAGL_INCLUDE_PANDAGL_CANVAS_H
