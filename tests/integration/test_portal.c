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

#include <math.h>
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

static void test_closed_content_is_hidden(void)
{
        ui_widget_t *open_portal = ui_create_portal();
        ui_widget_t *open_content = ui_create_widget(NULL);
        ui_widget_t *closed_portal = ui_create_portal();
        ui_widget_t *closed_content = ui_create_widget(NULL);

        ui_widget_append(ui_root(), open_portal);
        ui_widget_append(ui_root(), closed_portal);
        ui_portal_set_content(open_portal, open_content);
        ui_portal_set_content(closed_portal, closed_content);
        ui_portal_open(open_portal);
        ui_update();

        ctest_equal_int("open portal content should be displayed",
                        open_content->computed_style.type_bits.display,
                        CSS_DISPLAY_BLOCK);
        ctest_equal_int("closed portal content should stay hidden",
                        closed_content->computed_style.type_bits.display,
                        CSS_DISPLAY_NONE);

        ui_widget_destroy(closed_portal);
        ui_widget_destroy(open_portal);
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

static int g_underlay_click_count;

static void on_underlay_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_underlay_click_count;
}

static void emit_click(ui_widget_t *w)
{
        ui_event_t event;

        ui_event_init(&event, "click");
        ui_widget_emit_event(w, event, NULL);
}

static void test_passthrough_root_pointer_events(void)
{
        ui_widget_t *portal = ui_create_portal();
        ui_widget_t *content = ui_create_widget(NULL);

        ui_widget_append(ui_root(), portal);
        ui_portal_set_content(portal, content);
        ui_portal_set_passthrough(portal, true);
        ui_portal_open(portal);
        ui_update();

        ctest_equal_int(
            "passthrough portal should disable root pointer events",
            ui_get_portal_root()->computed_style.type_bits.pointer_events,
            CSS_POINTER_EVENTS_NONE);

        ui_widget_destroy(portal);
        ui_clear_trash();
}

static void test_click_through_outside_close(void)
{
        ui_widget_t *portal = ui_create_portal();
        ui_widget_t *anchor = ui_create_widget("button");
        ui_widget_t *underlay = ui_create_widget("button");
        ui_widget_t *content = ui_create_widget(NULL);

        ui_widget_append(ui_root(), portal);
        ui_widget_append(ui_root(), anchor);
        ui_widget_append(ui_root(), underlay);
        ui_portal_set_content(portal, content);
        ui_portal_set_anchor(portal, anchor);
        ui_portal_set_passthrough(portal, true);
        ui_portal_set_outside_close(portal, true);
        ui_portal_open(portal);
        ui_widget_on(portal, "portal-outside", on_portal_outside, NULL);
        ui_widget_on(underlay, "click", on_underlay_click, NULL);

        g_portal_outside_count = 0;
        g_underlay_click_count = 0;
        emit_click(underlay);
        ctest_equal_int("outside click should reach the underlay",
                        g_underlay_click_count, 1);
        ctest_equal_int("outside click should emit portal-outside",
                        g_portal_outside_count, 1);

        ui_portal_open(portal);
        g_portal_outside_count = 0;
        emit_click(content);
        ctest_equal_int("content click should not emit outside",
                        g_portal_outside_count, 0);

        emit_click(anchor);
        ctest_equal_int("anchor click should not emit outside",
                        g_portal_outside_count, 0);

        ui_widget_destroy(portal);
        ui_widget_destroy(anchor);
        ui_widget_destroy(underlay);
        ui_clear_trash();
}

static void test_passthrough_without_outside_close(void)
{
        ui_widget_t *portal = ui_create_portal();
        ui_widget_t *underlay = ui_create_widget("button");
        ui_widget_t *content = ui_create_widget(NULL);

        ui_widget_append(ui_root(), portal);
        ui_widget_append(ui_root(), underlay);
        ui_portal_set_content(portal, content);
        ui_portal_set_passthrough(portal, true);
        ui_portal_open(portal);
        ui_widget_on(portal, "portal-outside", on_portal_outside, NULL);

        g_portal_outside_count = 0;
        emit_click(underlay);
        ctest_equal_int("passthrough without outside close should not emit",
                        g_portal_outside_count, 0);

        ui_widget_destroy(portal);
        ui_widget_destroy(underlay);
        ui_clear_trash();
}

static void test_capturing_content_closes_other_outside_close(void)
{
        ui_widget_t *outside = ui_create_portal();
        ui_widget_t *outside_content = ui_create_widget(NULL);
        ui_widget_t *capturing = ui_create_portal();
        ui_widget_t *capturing_owner = ui_create_widget(NULL);
        ui_widget_t *capturing_anchor = ui_create_widget("button");
        ui_widget_t *capturing_content = ui_create_widget(NULL);

        ui_widget_append(ui_root(), outside);
        ui_portal_set_content(outside, outside_content);
        ui_portal_set_passthrough(outside, true);
        ui_portal_set_outside_close(outside, true);

        ui_widget_append(ui_root(), capturing_owner);
        ui_widget_append(capturing_owner, capturing);
        ui_widget_append(ui_root(), capturing_anchor);
        ui_portal_set_content(capturing, capturing_content);
        ui_portal_set_anchor(capturing, capturing_anchor);

        ui_portal_open(outside);
        ui_portal_open(capturing);
        ui_update();
        ctest_equal_bool("capturing content is portaled",
                         capturing_content->parent == ui_get_portal_root(),
                         true);
        ctest_equal_bool("capturing portal is owned",
                         capturing->parent == capturing_owner, true);
        ctest_equal_bool("capturing content is registered",
                         ui_portal_get_content(capturing) == capturing_content,
                         true);
        ui_widget_on(capturing_owner, "click", on_portal_click, NULL);
        ui_widget_on(outside, "portal-outside", on_portal_outside, NULL);

        g_portal_click_count = 0;
        g_portal_outside_count = 0;
        emit_click(capturing_content);
        ctest_equal_int("capturing content click should reach owner",
                        g_portal_click_count, 1);
        ctest_equal_int(
            "capturing content click should close other outside portal",
            g_portal_outside_count, 1);

        ui_widget_destroy(capturing_owner);
        ui_widget_destroy(capturing_anchor);
        ui_widget_destroy(outside);
        ui_clear_trash();
}

static void test_follow_anchor_scroll(void)
{
        ui_widget_t *container = ui_create_scrollarea();
        ui_widget_t *scroll_content = ui_create_scrollarea_content();
        ui_widget_t *anchor = ui_create_button();
        ui_widget_t *portal = ui_create_portal();
        ui_widget_t *content = ui_create_widget(NULL);
        float cx0, cy0, cx1, cy1, ax0, ay0, ax1, ay1;

        ui_widget_set_style_string(content, "position", "absolute");

        ui_widget_set_style_string(container, "position", "absolute");
        ui_widget_set_style_string(container, "left", "0");
        ui_widget_set_style_string(container, "top", "0");
        ui_widget_set_style_string(container, "width", "300px");
        ui_widget_set_style_string(container, "height", "200px");
        ui_widget_set_style_string(scroll_content, "height", "600px");
        ui_widget_set_style_string(anchor, "position", "absolute");
        ui_widget_set_style_string(anchor, "left", "20px");
        ui_widget_set_style_string(anchor, "top", "60px");
        ui_widget_set_style_string(anchor, "width", "80px");
        ui_widget_set_style_string(anchor, "height", "32px");
        ui_widget_append(scroll_content, anchor);
        ui_widget_append(container, scroll_content);
        ui_root_append(container);

        ui_widget_append(ui_root(), portal);
        ui_portal_set_content(portal, content);
        ui_portal_set_anchor(portal, anchor);
        ui_portal_set_follow_anchor(portal, true);
        ui_portal_open(portal);
        ui_update();
        ui_widget_get_offset(content, ui_root(), &cx0, &cy0);
        ui_widget_get_offset(anchor, ui_root(), &ax0, &ay0);

        ui_scrollarea_set_scroll_top(container, 50);
        ui_update();
        ui_widget_get_offset(content, ui_root(), &cx1, &cy1);
        ui_widget_get_offset(anchor, ui_root(), &ax1, &ay1);

        ctest_equal_bool("portal follow should see the anchor move",
                         ay1 < ay0 - 1, true);
        ctest_equal_bool("portal content should follow the anchor",
                         fabsf((cy1 - cy0) - (ay1 - ay0)) <= 1, true);
        (void)cx0;
        (void)cx1;
        (void)ax0;
        (void)ax1;

        ui_widget_destroy(portal);
        ui_widget_destroy(container);
        ui_clear_trash();
}

void test_portal(void)
{
        lcui_init();
        ctest_describe("portal root lazy creation", test_root_is_lazy);
        ctest_describe("portal content mounting",
                       test_content_is_mounted_to_root);
        ctest_describe("portal closed content visibility",
                       test_closed_content_is_hidden);
        ctest_describe("portal content click forwarding",
                       test_root_forwards_content_click);
        ctest_describe("portal outside event",
                       test_root_dispatches_outside_event);
        ctest_describe("portal passthrough pointer events",
                       test_passthrough_root_pointer_events);
        ctest_describe("portal click-through outside close",
                       test_click_through_outside_close);
        ctest_describe("portal passthrough without outside close",
                       test_passthrough_without_outside_close);
        ctest_describe("portal capturing content outside propagation",
                       test_capturing_content_closes_other_outside_close);
        ctest_describe("portal follow anchor scroll",
                       test_follow_anchor_scroll);
        lcui_destroy();
}
