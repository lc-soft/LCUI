# border 优化方案

## 设计思路

LCUI 中，将绘制 border 分为四个角落，各设置一个函数进行绘制。显然这种方法导致代码过于冗长。对于 border 需要进行代码重复利用。

Border 优化的整体思想是只保留使用一个角落的绘制代码（选取左上角的）。border_paint 绘制流程如下：
- 左上角绘制时，依然像 LCUI 中的一样，直接将位图传入。
- 在其他三个角落绘制时，先根据这个角落的宽和高，新建一个 buffer 用于缓存该角落的位图，并将原始 canvas 中的像素值复制进该位图（因为 draw_border_corner 中会根据 border 的内外改变原有的像素值，所以必须传入原有像素值）。
- 在draw_border_corner 中，通过传入的参数进行绘制。
- 在 border_paint 中，获得更新后的 buffer 像素矩阵缓存。三个角落的像素值，对 buffer 中数据根据位置进行水平翻转、竖直翻转等操作，写回入 canvas 中。

## 伪代码

```c++
int pd_border_paint(const pd_border_t *border, const pd_rect_t *box,
		 pd_paint_context paint) {
    // 计算各个角落的宽与高，初始化数据

    // 绘制左上角
    ...  // 计算绘制区域、判断绘制条件
    pd_graph_quote(&canvas, &paint->canvas, &rect);
    draw_border_top_left(&canvas, bound_left, bound_top, &border->top,
				  &border->left, border->top_left_radius);

    /*---- 绘制右上角 ----*/

    ...  // 计算绘制区域、判断绘制条件

    // 创建 canvas_buffer
    pd_canvas_t canvas_buffer;
    pd_canvas_init(&canvas_buffer);
    pd_canvas_create(width, height);

    // 向 canvas_buffer 中赋值，需要水平翻转
    pd_color_t *src_color_ptr;
    pd_color_t *dst_color_ptr;
    for (int y = 0; y < rect.height; ++y) {
        dst_color_ptr = pd_canvas_get_pixel_pointer(&canvas_buffer, 0, y);
        src_color_ptr = pd_canvas_get_pixel_pointer(&canvas, rect.x + rect.width, rect.y);
        for (int x = 0; x < rect.width; ++x,
              ++src_color_ptr, --dst_color_ptr) {
            dst_color_ptr->value = src_color_ptr->value;
        }
    }

    // 利用 draw_top_left 进行绘制
    draw_border_top_left(&canvas_buffer, bound_right, bound_top, &border->top,
				  &border->right, border->top_right_radius);

    // 将数据写回 canvas
    for (int y = 0; y < rect.height; ++y) {
        dst_color_ptr = pd_canvas_get_pixel_pointer(&canvas_buffer, 0, y);
        src_color_ptr = pd_canvas_get_pixel_pointer(&canvas, rect.x + rect.width, rect.y);
        for (int x = 0; x < rect.width; ++x,
              ++src_color_ptr, --dst_color_ptr) {
            dst_color_ptr->value = src_color_ptr->value;
        }
    }

    // 绘制左下角与右下角，类似右上角
}

```
