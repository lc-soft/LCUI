# PandaGL 图形库

- 开始日期：2022-04-11
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：[#292](https://github.com/lc-soft/LCUI/pull/292), [#281](https://github.com/lc-soft/LCUI/pull/281)

## 概括

将字体、文字处理、图像读写、图形绘制等功能模块整合为 PandaGL 图形库。

## 基本示例

```c
#include <pandagl.h>

int main(void)
{
	int ret;
	pd_canvas_t img;
	pd_pos_t pos = { 0, 80 };
	pd_rect_t area = { 0, 0, 320, 240 };
	pd_text_t *text = pd_text_create();
	pd_text_style_t style;

	/* 初始化字体处理功能 */
	pd_font_library_init();

	/* 创建一个图像，并使用灰色填充 */
	pd_canvas_init(&img);
	pd_canvas_create(&img, 320, 240);
	pd_canvas_fill(&img, RGB(240, 240, 240));

	/* 设置文本的字体大小 */
	pd_text_style_init(&style);
	style.pixel_size = 24;
	style.has_pixel_size = TRUE;

	/* 设置文本图层的固定尺寸、文本样式、文本内容、对齐方式 */
	pd_text_set_fixed_size(text, 320, 240);
	pd_text_set_style(text, &style);
	pd_text_set_align(text, PD_TEXT_ALIGN_CENTER);
	pd_text_write(text, L"这是一段测试文本\nHello, World!", NULL);
	pd_text_update(text, NULL);

	/* 将文本图层绘制到图像中，然后将图像写入至 png 文件中 */
	pd_text_render_to(text, area, pos, &img);
	ret = pd_write_png_file("test_string_render.png", &img);
	pd_canvas_destroy(&img);

	/* 释放字体处理功能相关资源 */
	pd_font_library_destroy();
	return ret;
}
```

## 动机

2.x 版本中的图形处理相关源文件比较分散，不利于查找和维护，而且按照 3.x 版本的架构设计思想，它们应该被整理为通用的图形库。

常见的开源图形库除了具备基本的图形绘制能力外，大都还具备文字渲染、图像读写能力，因此，新的图形库也应该具备这些能力。

## 详细设计

图形库命名为 PandaGL，标识符命名以 `pd_` 开头。

模块分为以下几类：

- 画布（Canvas）：提供像素数据操作和基本图形绘制能力。
- 字体库（FontLibrary）：提供字体文件加载、字形渲染能力。
- 文字（Text）：提供文字排版和渲染能力。
- 图像（Image）：提供图像文件读写能力。

由于字体库和图像模块都依赖第三方库，出于可裁剪性和库体积上的考虑，在构建配置中将它们配置为可选模块，示例：

```sh
# 禁用图像模块
xmake config --with-pandagl-image=n

# 禁用字体库模块
xmake config --with-pandagl-font=n

# 禁用文字模块
xmake config --with-pandagl-text=n
```

`LCUI_TextLayer` 重命名为 `pd_text_t`，删除 UTF-8 和 ANSI 版本的文本操作函数，仅保留宽字符版本，示例：

```diff
- /** 插入文本内容（UTF-8版） */
- LCUI_API int TextLayer_InsertText(LCUI_TextLayer layer, const char *utf8_str);
-
- /** 追加文本内容（宽字符版） */
- LCUI_API int TextLayer_AppendTextW(LCUI_TextLayer layer, const wchar_t *wstr,
- 				   LinkedList *tag_stack);
-
- /** 追加文本内容 */
- LCUI_API int TextLayer_AppendTextA(LCUI_TextLayer layer,
- 				   const char *ascii_text);
+ PD_PUBLIC int pd_text_append(pd_text_t *text, const wchar_t *wstr, list_t *tag_stack);
```

## 缺点

无。

## 备选方案

删除图形处理相关模块，直接用成熟的开源图形库。不建议采用此方案，因为工作量较大，还需要投入成本去学习其它图形库。

## 采用策略

这是个破坏性改动，需要对所有用到 PandaGL 功能的代码进行修改。由于主要改动都在命名上，这些文件通常只需要全局替换名称即可。

## 未解决的问题

出于函数参数复杂度问题上的考虑，`pd_text_write()`、`pd_text_append()`、`pd_text_insert()` 的第三个参数 `tag_stack` 用处不大，应该重新考虑 `tag_stack` 的实现方式，例如：内置在 `pd_text_t` 中。

字体库和文字模块的接口有待重新设计。
