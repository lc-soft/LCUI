﻿#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/painter.h>

int test_paint_background_color(void)
{
        pd_canvas_t canvas;
        pd_color_t gray = RGB(240, 240, 240);
        pd_color_t green = RGB(102, 204, 0);
        pd_rect_t rect = { 200, 100, 400, 300 };
        pd_background_t bg = { 0 };
        pd_paint_context_t* paint;

        pd_canvas_init(&canvas);
        pd_canvas_create(&canvas, 800, 600);
        pd_canvas_fill_rect(&canvas, gray, NULL, FALSE);
        // 设置背景色
        bg.color = green;
        // 创建绘制上下文
        paint = pd_painter_begin(&canvas, &rect);
        // 绘制背景
        pd_background_paint(&bg, &rect, paint);
        LCUI_WritePNGFile("test_paint_background_color.png", &canvas);
        pd_painter_end(paint);
        pd_canvas_free(&canvas);
        return 0;
}

int test_paint_background_image(void)
{
        pd_canvas_t canvas;
        pd_canvas_t image;
        pd_color_t gray = RGB(240, 240, 240);
        pd_color_t green = RGB(102, 204, 0);
        pd_rect_t rect = { 200, 100, 400, 300 };
        pd_background_t bg = { 0 };
        pd_paint_context_t* paint;

        pd_canvas_init(&canvas);
        pd_canvas_init(&image);
        pd_canvas_create(&canvas, 800, 600);
        pd_canvas_fill_rect(&canvas, gray, NULL, FALSE);
        // 读取背景图片
        if (LCUI_ReadImageFile("test_image_reader.png", &image) != 0) {
                return -1;
        }
        // 设置背景色
        bg.color = green;
        // 设置背景图
        bg.image = &image;
        bg.size.width = image.width;
        bg.size.height = image.height;
        // 创建绘制上下文
        paint = pd_painter_begin(&canvas, &rect);
        // 绘制背景
        pd_background_paint(&bg, &rect, paint);
        LCUI_WritePNGFile("test_paint_background_image.png", &canvas);
        pd_painter_end(paint);
        pd_canvas_free(&image);
        pd_canvas_free(&canvas);
        return 0;
}

int test_paint_background_image_with_size(void)
{
        pd_canvas_t canvas;
        pd_canvas_t image;
        pd_color_t gray = RGB(240, 240, 240);
        pd_color_t green = RGB(102, 204, 0);
        pd_rect_t rect = { 200, 100, 400, 300 };
        pd_background_t bg = { 0 };
        pd_paint_context_t* paint;

        pd_canvas_init(&canvas);
        pd_canvas_init(&image);
        pd_canvas_create(&canvas, 800, 600);
        pd_canvas_fill_rect(&canvas, gray, NULL, FALSE);
        // 读取背景图片
        if (LCUI_ReadImageFile("test_image_reader.png", &image) != 0) {
                return -1;
        }
        // 设置背景色
        bg.color = green;
        // 设置背景图
        bg.image = &image;
        // 将背景图设置成与背景区域相同的尺寸
        bg.size.width = rect.width;
        bg.size.height = rect.height;
        // 创建绘制上下文
        paint = pd_painter_begin(&canvas, &rect);
        // 绘制背景
        pd_background_paint(&bg, &rect, paint);
        LCUI_WritePNGFile("test_paint_background_image_with_size.png", &canvas);
        pd_painter_end(paint);
        pd_canvas_free(&image);
        pd_canvas_free(&canvas);
        return 0;
}

int test_paint_background_image_with_position(void)
{
        pd_canvas_t canvas;
        pd_canvas_t image;
        pd_color_t gray = RGB(240, 240, 240);
        pd_color_t green = RGB(102, 204, 0);
        pd_rect_t rect = { 200, 100, 400, 300 };
        pd_background_t bg = { 0 };
        pd_paint_context_t* paint;

        pd_canvas_init(&canvas);
        pd_canvas_init(&image);
        pd_canvas_create(&canvas, 800, 600);
        pd_canvas_fill_rect(&canvas, gray, NULL, FALSE);
        // 读取背景图片
        if (LCUI_ReadImageFile("test_image_reader.png", &image) != 0) {
                return -1;
        }
        // 设置背景色
        bg.color = green;
        // 设置背景图
        bg.image = &image;
        bg.size.width = image.width;
        bg.size.height = image.height;
        // 让背景图居中
        bg.position.x = (rect.width - image.width) / 2;
        bg.position.y = (rect.height - image.height) / 2;
        // 创建绘制上下文
        paint = pd_painter_begin(&canvas, &rect);
        // 绘制背景
        pd_background_paint(&bg, &rect, paint);
        LCUI_WritePNGFile("test_paint_background_image_with_position.png",
                          &canvas);
        pd_painter_end(paint);
        pd_canvas_free(&image);
        pd_canvas_free(&canvas);
        return 0;
}

int main(void)
{
        test_paint_background_color();
        test_paint_background_image();
        test_paint_background_image_with_size();
        test_paint_background_image_with_position();
        return 0;
}
