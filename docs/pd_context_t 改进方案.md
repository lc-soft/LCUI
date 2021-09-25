# pd_context_t 改进方案

## 设计思路

由于 LCUI_GraphQuote 与 LCUI_PaintContextRec 都用于记录 LCUI_Graph 的上下文相关信息，所以希望将这两个数据结构进行合并为 pd_context_t 一个结构体，在每个 pd_context_t 中都保存有一个 pd_canvas_t 的指针，保存绘制的位图数据。

在数据结构上，把 LCUI_GraphQuote 和 LCUI_PaintContextRec 的成员变量进行合并。将 LCUI_GraphQuote 的 top 和 left 变量融合入 LCUI_PaintContextRec 的 rect 变量中，分别用 rect.x rect.y 表示。

由于 pd_context_t 的更改，在 graph 中也有一些函数需要更改。比如 pd_graph_get_valid_rect 需要更改传入参数为 pd_context_t\*，返回 pd_context_t.rect，或者直接使用 pd_context_t.rect 代替此函数；graph_horiz_flip_rgb 等函数需要将传入参数从 pd_canvas_t\* 更改为 pd_context_t\*，并修改内部逻辑。

## 工作流程讲解

在原本 LCUI 的绘制流程中，部分函数将 LCUI_GraphQuote 与 LCUI_Graph 绑定，在绘制函数中传入 LCUI_Graph，在绘制函数内部通过 LCUI_Graph 的指针获取 Quote。

更改后，绘制时，在 pd_context_t 中保存绘制的上下文、绘制区域、以及与其 pd_canvas_t 的连接信息。在绘制函数的参数中，由原先的传入 LCUI_Graph 更改为传入 pd_context_t。在绘制函数内部，取消获得 LCUI_GraphQuote 与 validarea 步骤。validarea 可以直接用 pd_context_t.rect 获得。

## 示例代码

更改后，pd_context_t 和 pd_canvas_t (替代原有的 LCUI_Graph 结构体)的伪代码如下：
```c++
typedef struct pd_canvas_t_ pd_canvas_t;

typedef struct pd_context_t {  
    pd_rect_t rect;       /**< 允许绘制的区域 */
    pd_bool_t is_valid;
    pd_bool_t is_writable;
    pd_bool_t with_alpha; /**< 绘制时是否需要处理 alpha 通道 */
    union {
        pd_canvas_t *source;
        const pd_canvas_t *source_ro;
    };
} pd_context_t;

struct pd_canvas_t_ {
    unsigned width;
    unsigned height;
    union {
        uchar_t *bytes;
        pd_color_t *argb;
    };
    pd_color_type color_type;
    unsigned bytes_per_pixel;
    unsigned bytes_per_row;
    float opacity;
    size_t mem_size;
    uchar_t *palette;
};
```

简单地，以 graph_horiz_flip_rgb 功能为例，目前函数传入参数为 pd_canvas_t \*graph，修改为 pd_context_t \*context。

```c++
static int graph_horiz_flip_rgb(const pd_context_t *context, pd_context_t *buff)
{
	int x, y;
	size_t n_bytes;
	pd_rect_t rect;
	uchar_t *byte_src, *byte_des;

	buff->rect = context->rect;
	buff->with_alpha = context->with_alpha;
	buff->is_valid = context->is_valid;
	buff->is_writable = context->is_writable;

	if (!pd_context_is_valid(context)) {
		return -1;
	}
	rect = context->rect;
	buff->source->color_type = context->source->color_type;
	if (0 != pd_graph_create(buff, rect.width, rect.height)) {
		return -2;
	}

	for (y = 0; y < rect.height; ++y) {
		byte_des = buff->source->bytes + y * buff->source->bytes_per_row;
		n_bytes = (rect.y + y) * context->source->bytes_per_row;
		n_bytes += (rect.x + rect.width - 1) * 3;
		byte_src = buff->source->bytes + n_bytes;
		for (x = 0; x < rect.width; ++x) {
			*byte_des++ = *byte_src--;
			*byte_des++ = *byte_src--;
			*byte_des++ = *byte_src--;
		}
	}
	return 0;
}
```
