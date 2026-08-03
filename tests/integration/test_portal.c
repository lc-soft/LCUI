/*
 * tests/integration/test_portal.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>

static void test_root_is_lazy(void)
{
        ui_widget_t *portal;

        portal = ui_create_portal();
        ctest_equal_int("portal root should stay lazy",
                        ui_widget_get_child(ui_root(), 0) == NULL, 1);
        ui_widget_destroy(portal);
}

static void test_content_is_mounted_to_root(void)
{
        ui_widget_t *owner;
        ui_widget_t *portal;
        ui_widget_t *content;
        ui_widget_t *root;

        owner = ui_create_widget(NULL);
        portal = ui_create_portal();
        content = ui_create_widget(NULL);
        ui_widget_append(owner, portal);
        ui_widget_append(ui_root(), owner);
        ui_portal_set_content(portal, content);
        root = ui_get_portal_root();

        ctest_equal_int("portal should remain under owner",
                        portal->parent == owner, 1);
        ctest_equal_int("content should move under portal root",
                        content->parent == root, 1);
        ctest_equal_int("portal should return its content",
                        ui_portal_get_content(portal) == content, 1);

        ui_widget_destroy(owner);
        ui_clear_trash();
}

static int g_portal_click_count;
static int g_portal_outside_count;

static void on_portal_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_portal_click_count;
}

static void on_portal_outside(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_portal_outside_count;
}

static void test_root_forwards_content_click(void)
{
        ui_widget_t *owner;
        ui_widget_t *portal;
        ui_widget_t *content;
        ui_event_t event;

        owner = ui_create_widget(NULL);
        portal = ui_create_portal();
        content = ui_create_widget(NULL);
        ui_widget_append(owner, portal);
        ui_widget_append(ui_root(), owner);
        ui_portal_set_content(portal, content);
        ui_widget_on(owner, "click", on_portal_click, NULL);

        g_portal_click_count = 0;
        ui_event_init(&event, "click");
        ui_widget_emit_event(content, event, NULL);

        ctest_equal_int("content click should reach owner once",
                        g_portal_click_count, 1);

        ui_widget_destroy(owner);
        ui_clear_trash();
}

static void test_root_dispatches_outside_event(void)
{
        ui_widget_t *portal;
        ui_widget_t *content;
        ui_event_t event;

        portal = ui_create_portal();
        content = ui_create_widget(NULL);
        ui_widget_append(ui_root(), portal);
        ui_portal_set_content(portal, content);
        ui_portal_open(portal);
        ui_widget_on(portal, "portal-outside", on_portal_outside, NULL);

        g_portal_outside_count = 0;
        ui_event_init(&event, "click");
        event.target = ui_get_portal_root();
        ui_widget_emit_event(ui_get_portal_root(), event, NULL);

        ctest_equal_int("root blank click should dispatch outside event",
                        g_portal_outside_count, 1);

        ui_widget_destroy(portal);
        ui_clear_trash();
}

void test_portal(void)
{
        lcui_init();
        ctest_describe("portal root lazy creation", test_root_is_lazy);
        ctest_describe("portal content mounting",
                       test_content_is_mounted_to_root);
        ctest_describe("portal content click forwarding",
                       test_root_forwards_content_click);
        ctest_describe("portal outside event",
                       test_root_dispatches_outside_event);
        lcui_destroy();
}
