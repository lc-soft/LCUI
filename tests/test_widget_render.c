/*
 * tests/test_widget_render.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <pandagl.h>

int main(void)
{
	int ret;
	pd_canvas_t canvas;
	ui_widget_t *root, *box, *txt;
	pd_context_t paint;
	pd_rect_t area = { 40, 40, 320, 240 };

	lcui_init();

	root = ui_create_widget(NULL);
	box = ui_create_widget(NULL);
	txt = ui_create_widget("text");

	/* 创建一块灰色的画板 */
	pd_canvas_init(&canvas);
	pd_canvas_create(&canvas, 320, 240);
	pd_canvas_fill(&canvas, pd_rgb(240, 240, 240));

	/* 初始化一个绘制实例，绘制区域为整个画板 */
	paint.with_alpha = false;
	paint.rect.width = 320;
	paint.rect.height = 320;
	paint.rect.x = paint.rect.y = 0;
	pd_canvas_quote(&paint.canvas, &canvas, &area);

	/* 设定基本的样式和内容 */
	ui_widget_set_style_string(box, "padding", "20px");
	ui_widget_set_style_string(box, "border", "1px solid rgb(201,230,242)");
	ui_widget_set_style_string(box, "background-color", "rgb(242,249,252)");
	ui_text_set_content_w(txt, L"[size=24px]这是一段测试文本[/size]");
	ui_widget_append(box, txt);
	ui_widget_append(root, box);
	/* 更新部件，此处的更新顺序必须是父级到子级 */
	ui_widget_update(root);
	ui_widget_update(box);
	ui_widget_update(txt);

	/* 渲染部件 */
	ui_widget_render(box, &paint);
	ret = pd_write_png_file("test_widget_render.png", &canvas);
	pd_canvas_destroy(&canvas);

	lcui_destroy();
	return ret;
}
