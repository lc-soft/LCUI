# CSS 已计算样式

- 开始日期：2023-04-08
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：[#287](https://github.com/lc-soft/LCUI/pull/287)

## 概括

重新设计 CSS 样式计算流程和相关数据存储方式，将 UI 库中的部分样式计算逻辑移入 CSS 库中实现。

## 基本示例

以下是 UI 部件的计算流程示例：

```c
css_computed_style_t *s = &w->specified_style;
css_style_decl_t *style;

css_computed_style_destroy(s);
if (w->custom_style) {
        style = css_style_decl_create();
        css_style_decl_merge(style, w->custom_style);
        css_style_decl_merge(style, w->matched_style);
        css_cascade_style(style, s);
        css_style_decl_destroy(style);
} else {
        css_cascade_style(w->matched_style, s);
}
w->computed_style = *s;
ui_widget_compute_style(w);
```

先清空之前的计算结果，然后层叠（Cascade）已匹配样式和自定义样式，最后对层叠结果进行计算，得出已计算样式。

2.x 版本中的 `LCUI_Widget` 结构体成员 `style` 和 `computed_style` 已统一改用 `css_computed_style_t` 类型，并重命名为 `specified_style` 和 `computed_style`。

## 动机

UI 库中包含了部分 CSS 计算逻辑，例如：width、height、flex-grow 等属性的应用值计算，这有违单一责任原则，应该将 CSS 属性计算移动到 CSS 库内，以让 CSS 库的功能更完备。

另一个方面，UI 部件的 `computed_style` 和 `style` 成员的内存占用比较大：`computed_style` 占用 336 字节，`style` 成员占用 8 字节，其中每个 CSS 属性值占用 16 字节，共有 68 个属性值，也就是共占用 336 + 8 + 16 * 68 = 1432 字节，需要优化。

## 详细设计

参考 [LibCSS](http://www.netsurf-browser.org/projects/libcss/) 的设计，更改样式计算流程为：

1. 层叠已匹配的样式和自定义样式，计算每个属性的指定值，得出指定样式（`specified_style`）。
1. 计算每个属性的实际值，得出已计算样式（`computed_style`）。

内存优化方面，调整已计算样式的数据结构，以比特位为最小粒度为 CSS 属性值分配存储空间，例如：

```c
typedef struct css_computed_style_t {
	struct css_type_bits_t {
		uint8_t display : 5;
		uint8_t box_sizing : 2;
		uint8_t visibility : 4;
		uint8_t vertical_align : 4;
		uint8_t pointer_events : 2;
		uint8_t position : 3;
                ...
        } type_bits;

	struct css_unit_bits_t {
		css_unit_t left : 4;
		css_unit_t right : 4;
		css_unit_t top : 4;
		css_unit_t bottom : 4;
		css_unit_t width : 4;
		css_unit_t height : 4;
                ...
        } unit_bits;

	css_numeric_value_t z_index;
	css_numeric_value_t opacity;

	css_numeric_value_t left;
	css_numeric_value_t right;
	css_numeric_value_t top;
	css_numeric_value_t bottom;
        ...
}
```

以目前的 CSS 属性数量，`css_computed_style_t` 占用 288 字节，相比修改前减少了 `1432 - 288 * 2 = 856` 字节。

为方便使用属性值，可为部分 CSS 属性提供 `css_computed_` 开头的辅助函数，例如：

```c
LIBCSS_PUBLIC uint8_t css_computed_display(const css_computed_style_t *s);

LIBCSS_PUBLIC uint8_t css_computed_width(const css_computed_style_t *s,
                                         css_numeric_value_t *value,
                                         css_unit_t *unit);

LIBCSS_PUBLIC uint8_t css_computed_height(const css_computed_style_t *s,
                                          css_numeric_value_t *value,
                                          css_unit_t *unit);
```

用法如下：

```c
css_number_value_t value;
css_unit_t unit;

switch (css_computed_width(&style, &value, &unit)) {
case CSS_WIDTH_AUTO:
        // ...
        break;
case CSS_WIDTH_SET:
        if (unit == CSS_UNIT_PERCENT) {
                // ...
        }
        break;
default:
        break;
}
```

还可以添加一些常用的 CSS 值操作相关的工具函数宏，例如：

```c
#define IS_CSS_LENGTH(S, PROP_KEY) (S)->type_bits.PROP_KEY == CSS_LENGTH_SET

#define IS_CSS_FIXED_LENGTH(S, PROP_KEY)              \
	((S)->type_bits.PROP_KEY == CSS_LENGTH_SET && \
	 (S)->unit_bits.PROP_KEY == CSS_UNIT_PX)
```

## 缺点

每次样式计算都是计算全部属性的值，影响性能。

## 备选方案

改用 libcss 库。不建议采用此方案，因为 libcss 的用法与 LCUI 现有的 CSS 库的用法相差较大。

## 采用策略

这是个破坏性改动，包含数据结构和函数的改动，涉及 UI 库和 CSS 库。

数据结构的改动主要是 `LCUI_Widget`：

```diff
- LCUI_StyleList custom_style;
- LCUI_CachedStyleSheet inherited_style;
- LCUI_WidgetStyle computed_style;
+ css_style_decl_t *custom_style;
+ const css_style_decl_t *matched_style;
+ css_computed_style_t specified_style;
+ css_computed_style_t computed_style;
```

UI 库内涉及样式读写的代码都需要重构，包括布局计算、绘制、鼠标事件等。
