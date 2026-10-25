/*
 * tests/integration/test_icon_centering.c: -- 图标在 flex 容器内的居中
 *
 * 目的：验证 flex 容器的 items-center / justify-center 能正确居中子项，
 * 以及 icon 部件的盒高度是否被异常撑高。容器只用普通 widget + className
 * 搭建，不依赖任何具体部件的自带样式。
 *
 * 三组断言：
 *   A) 纯 flex 引擎：固定尺寸的普通子项在 flex 容器内精确居中。
 *   B) icon 盒高度：icon 的布局高度是否等于预期行盒（是否被异常撑高）。
 *   C) icon 盒居中：真实 icon 部件在容器内上下/左右留白是否对称
 *      （对称则布局 OK，偏移来自字形 baseline）。
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include <ctest-custom.h>

#define EPS 3.0f
#define MEDIA_SIZE 32.0f

static const char TEST_CSS[] = ".test-media {\n"
                               "  display: flex;\n"
                               "  align-items: center;\n"
                               "  justify-content: center;\n"
                               "  width: 32px;\n"
                               "  height: 32px;\n"
                               "}\n"
                               "icon {\n"
                               "  line-height: 1;\n"
                               "}\n"
                               ".test-box {\n"
                               "  width: 24px;\n"
                               "  height: 24px;\n"
                               "  background-color: #000000;\n"
                               "}\n";

/* 子项相对容器内容区的四边留白，用于判断居中是否对称 */
static void measure_gaps(ui_widget_t *container, ui_widget_t *child, float *dx,
                         float *dy)
{
        float left = child->border_box.x;
        float top = child->border_box.y;
        float right = container->content_box.width -
                      (child->border_box.x + child->border_box.width);
        float bottom = container->content_box.height -
                       (child->border_box.y + child->border_box.height);
        *dx = left - right;
        *dy = top - bottom;
}

static ui_widget_t *create_container_with(ui_widget_t *child)
{
        ui_widget_t *container = ui_create_widget(NULL);

        ui_widget_add_class(container, "test-media");
        ui_widget_append(container, child);
        ui_root_append(container);
        return container;
}

static void test_flex_centering_sized_box(void)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *container;
        float dx, dy;

        ui_widget_add_class(box, "test-box");
        container = create_container_with(box);
        ui_update();

        measure_gaps(container, box, &dx, &dy);
        ctest_equal_bool("flex should center child horizontally",
                         dx > -EPS && dx < EPS, true);
        ctest_equal_bool("flex should center child vertically",
                         dy > -EPS && dy < EPS, true);
        ctest_equal_int("child should be 24px tall inside container",
                        (int)(box->border_box.height + 0.5f), 24);

        ui_widget_destroy(container);
}

static void test_icon_box_height(void)
{
        ui_widget_t *icon = ui_create_fluent_icon("checkmark");
        ui_widget_t *container;

        container = create_container_with(icon);
        ui_update();

        /* 不显式设 font-size，让它按真实继承链取默认字号；line-height:1 时
         * 盒高应 ≈ 字号。若被下划线/额外度量撑高，则会明显大于字号行盒。 */
        ctest_equal_bool("icon height should not be inflated (fits container)",
                         icon->border_box.height > 0.0f &&
                             icon->border_box.height < MEDIA_SIZE + EPS,
                         true);

        ui_widget_destroy(container);
}

static void test_icon_vertical_centering(void)
{
        ui_widget_t *icon = ui_create_fluent_icon("checkmark");
        ui_widget_t *container;
        float dx, dy;

        container = create_container_with(icon);
        ui_update();

        measure_gaps(container, icon, &dx, &dy);
        ctest_equal_bool("icon box should be horizontally centered",
                         dx > -EPS && dx < EPS, true);
        /* 若这条失败（dy 明显偏离 0），说明 flex 对 text 子的高度/对齐估算
         * 有问题；若通过，则布局居中正确，字形上下偏移来自字体 baseline。 */
        ctest_equal_bool("icon box should be vertically centered",
                         dy > -EPS && dy < EPS, true);

        ui_widget_destroy(container);
}

void test_icon_centering(void)
{
        lcui_init();
        ui_load_css_string(TEST_CSS, "test_icon_centering.css");

        ctest_describe("flex centering (sized box)",
                       test_flex_centering_sized_box);
        ctest_describe("icon box height", test_icon_box_height);
        ctest_describe("icon vertical centering", test_icon_vertical_centering);

        lcui_destroy();
}
