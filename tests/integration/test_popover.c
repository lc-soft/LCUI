/*
 * tests/integration/test_popover.c: -- Popover widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <string.h>
#include <wchar.h>
#include <LCUI.h>
#include <ctest-custom.h>

static ui_widget_t *find_by_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, type)) {
                        return child;
                }
                found = find_by_type(child, type);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static int count_by_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        int count = 0;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, type)) {
                        ++count;
                }
                count += count_by_type(child, type);
        }
        return count;
}

static void test_factories_and_build(void)
{
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *trigger;
        ui_widget_t *content;
        ui_widget_t *header;
        ui_widget_t *title;
        ui_widget_t *description;
        ui_widget_t *button;
        wchar_t text[64];

        ui_popover_build_children(popover);
        trigger = find_by_type(popover, "popover-trigger");
        content = ui_popover_get_content(popover);
        header = find_by_type(content, "popover-header");
        title = find_by_type(content, "popover-title");
        description = find_by_type(content, "popover-description");
        button = find_by_type(trigger, "button");

        ctest_equal_bool("build should create a trigger", trigger != NULL,
                         true);
        ctest_equal_bool("build should create a content", content != NULL,
                         true);
        ctest_equal_bool("build should create a header", header != NULL, true);
        ctest_equal_bool("build should create a title", title != NULL, true);
        ctest_equal_bool("build should create a description",
                         description != NULL, true);
        ctest_equal_bool("build should create a trigger button", button != NULL,
                         true);

        if (button) {
                ui_text_get_content_w(button, text, 64);
                ctest_equal_bool("trigger button should use the default text",
                                 wcscmp(text, L"Open popover") == 0, true);
        }
        if (title) {
                ui_text_get_content_w(title, text, 64);
                ctest_equal_bool("title should use the default text",
                                 wcscmp(text, L"Untitled popover") == 0, true);
        }
        if (description) {
                ui_text_get_content_w(description, text, 64);
                ctest_equal_bool(
                    "description should use the default text",
                    wcscmp(text, L"Add supporting text here.") == 0, true);
        }

        ui_popover_build_children(popover);
        ctest_equal_int("build should keep one trigger",
                        count_by_type(popover, "popover-trigger"), 1);
        ctest_equal_int("build should keep one content",
                        count_by_type(popover, "popover-content"), 1);
        ctest_equal_int("build should keep one header",
                        count_by_type(popover, "popover-header"), 1);
        ctest_equal_int("build should keep one title",
                        count_by_type(popover, "popover-title"), 1);
        ctest_equal_int("build should keep one description",
                        count_by_type(popover, "popover-description"), 1);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_custom_structure_is_preserved(void)
{
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *trigger = ui_create_popover_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_popover_content();
        ui_widget_t *header = ui_create_popover_header();
        ui_widget_t *title = ui_create_popover_title();
        ui_widget_t *description = ui_create_popover_description();
        wchar_t text[64];

        ui_button_set_text(button, "Custom");
        ui_text_set_content(title, "Custom title");
        ui_text_set_content(description, "Custom description");
        ui_widget_append(trigger, button);
        ui_widget_append(header, title);
        ui_widget_append(header, description);
        ui_widget_append(content, header);
        ui_widget_append(popover, trigger);
        ui_widget_append(popover, content);

        ui_popover_build_children(popover);

        ctest_equal_int("custom build should keep one trigger",
                        count_by_type(popover, "popover-trigger"), 1);
        ctest_equal_int("custom build should keep one content",
                        count_by_type(popover, "popover-content"), 1);
        ctest_equal_int("custom build should keep one header",
                        count_by_type(popover, "popover-header"), 1);
        ctest_equal_int("custom build should keep one title",
                        count_by_type(popover, "popover-title"), 1);
        ctest_equal_int("custom build should keep one description",
                        count_by_type(popover, "popover-description"), 1);
        ui_text_get_content_w(button, text, 64);
        ctest_equal_bool("custom trigger text should be preserved",
                         wcscmp(text, L"Custom") == 0, true);
        ui_text_get_content_w(title, text, 64);
        ctest_equal_bool("custom title should be preserved",
                         wcscmp(text, L"Custom title") == 0, true);
        ui_text_get_content_w(description, text, 64);
        ctest_equal_bool("custom description should be preserved",
                         wcscmp(text, L"Custom description") == 0, true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_text_setters(void)
{
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *content;
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *title;
        ui_widget_t *description;
        wchar_t text[64];

        ui_popover_build_children(popover);
        ui_root_append(popover);
        ui_update();
        content = ui_popover_get_content(popover);
        trigger = find_by_type(popover, "popover-trigger");
        button = find_by_type(trigger, "button");
        title = find_by_type(content, "popover-title");
        description = find_by_type(content, "popover-description");

        ui_popover_set_trigger_text(popover, "Details");
        ui_popover_set_title(popover, "Dimensions");
        ui_popover_set_description(popover, "Set the layer size.");

        ui_text_get_content_w(button, text, 64);
        ctest_equal_bool("trigger text setter should update the button",
                         wcscmp(text, L"Details") == 0, true);
        ui_text_get_content_w(title, text, 64);
        ctest_equal_bool("title setter should update the title",
                         wcscmp(text, L"Dimensions") == 0, true);
        ui_text_get_content_w(description, text, 64);
        ctest_equal_bool("description setter should update the description",
                         wcscmp(text, L"Set the layer size.") == 0, true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static int g_popover_open_count;
static int g_popover_close_count;

static void on_popover_open(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_popover_open_count;
}

static void on_popover_close(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_popover_close_count;
}

static void click_widget(ui_widget_t *w)
{
        ui_event_t event;

        ui_event_init(&event, "click");
        ui_widget_emit_event(w, event, NULL);
}

static void settle_popover(void)
{
        ui_update();
        ui_update();
        ui_update();
}

static ui_widget_t *find_trigger_button(ui_widget_t *popover)
{
        ui_widget_t *trigger = find_by_type(popover, "popover-trigger");

        return trigger ? find_by_type(trigger, "button") : NULL;
}

static ui_widget_t *create_built_popover(void)
{
        ui_widget_t *popover = ui_create_popover();

        ui_popover_build_children(popover);
        ui_root_append(popover);
        ui_update();
        return popover;
}

static void test_open_close_api(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);

        ui_popover_open(popover);
        ctest_equal_bool("open should set state", ui_popover_is_open(popover),
                         true);
        ctest_equal_str("open should sync the open attr",
                        ui_widget_get_attr(popover, "open"), "true");
        settle_popover();
        ctest_equal_bool("open should reveal the content",
                         ui_widget_is_visible(content), true);
        ctest_equal_int("open content should accept pointer events",
                        content->computed_style.type_bits.pointer_events,
                        CSS_POINTER_EVENTS_AUTO);

        ui_popover_close(popover);
        ctest_equal_bool("close should clear state",
                         ui_popover_is_open(popover), false);
        ctest_equal_str("close should sync the open attr",
                        ui_widget_get_attr(popover, "open"), "false");
        ui_update();
        ctest_equal_bool("close should hide the content",
                         ui_widget_is_visible(content), false);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_trigger_toggles(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *button = find_trigger_button(popover);

        click_widget(button);
        ui_update();
        ctest_equal_bool("trigger click should open",
                         ui_popover_is_open(popover), true);

        click_widget(button);
        ui_update();
        ctest_equal_bool("second trigger click should close",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_open_attr_and_events(void)
{
        ui_widget_t *popover = create_built_popover();

        ui_widget_on(popover, "open", on_popover_open, NULL);
        ui_widget_on(popover, "close", on_popover_close, NULL);
        g_popover_open_count = 0;
        g_popover_close_count = 0;

        ui_widget_set_attr(popover, "open", "true");
        ui_update();
        ctest_equal_bool("open attr should open", ui_popover_is_open(popover),
                         true);
        ctest_equal_int("open attr should emit once", g_popover_open_count, 1);

        ui_widget_set_attr(popover, "open", "true");
        ui_update();
        ctest_equal_int("repeating open should not emit again",
                        g_popover_open_count, 1);

        ui_popover_set_open(popover, true);
        ctest_equal_int("same open value should not emit", g_popover_open_count,
                        1);

        ui_widget_set_attr(popover, "open", "false");
        ui_update();
        ctest_equal_bool("close attr should close", ui_popover_is_open(popover),
                         false);
        ctest_equal_int("close attr should emit once", g_popover_close_count,
                        1);

        ui_popover_set_open(popover, false);
        ctest_equal_int("same close value should not emit",
                        g_popover_close_count, 1);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_open_attr_before_update(void)
{
        ui_widget_t *popover = ui_create_popover();

        ui_popover_build_children(popover);
        ui_root_append(popover);
        ui_widget_set_attr(popover, "open", "true");
        ui_update();

        ctest_equal_bool("open attr should survive the first update",
                         ui_popover_is_open(popover), true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_open_without_content(void)
{
        ui_widget_t *popover = ui_create_popover();

        ui_root_append(popover);
        ui_widget_set_attr(popover, "open", "true");
        ui_update();

        ctest_equal_bool("popover without content should stay closed",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_single_active(void)
{
        ui_widget_t *first = create_built_popover();
        ui_widget_t *second = create_built_popover();

        ui_popover_open(first);
        ctest_equal_bool("first popover should open", ui_popover_is_open(first),
                         true);

        ui_popover_open(second);
        ctest_equal_bool("opening the second should close the first",
                         ui_popover_is_open(first), false);
        ctest_equal_bool("second popover should open",
                         ui_popover_is_open(second), true);

        ui_widget_destroy(first);
        ui_widget_destroy(second);
        ui_clear_trash();
}

static int g_outside_click_count;

static void on_outside_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++g_outside_click_count;
}

static void on_noop_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
}

static void test_outside_click_through_closes(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *outside = ui_create_button();

        ui_root_append(outside);
        ui_update();
        ui_widget_on(outside, "click", on_outside_click, NULL);
        ui_popover_open(popover);
        g_outside_click_count = 0;

        click_widget(outside);
        ctest_equal_int("outside click should reach the outside button",
                        g_outside_click_count, 1);
        ctest_equal_bool("outside click should close the popover",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_widget_destroy(outside);
        ui_clear_trash();
}

static void test_outside_real_click_closes(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *outer = ui_create_widget(NULL);
        ui_widget_t *outside = ui_create_button();
        ui_event_t event;

        ui_widget_set_style_string(outer, "position", "absolute");
        ui_widget_set_style_string(outer, "left", "400px");
        ui_widget_set_style_string(outer, "top", "400px");
        ui_widget_set_style_string(outer, "width", "80px");
        ui_widget_set_style_string(outer, "height", "32px");
        ui_widget_set_style_string(outside, "width", "80px");
        ui_widget_set_style_string(outside, "height", "32px");
        ui_widget_append(outer, outside);
        ui_root_append(outer);
        ui_update();
        ui_widget_on(outer, "click", on_noop_click, NULL);
        ui_widget_on(outside, "click", on_outside_click, NULL);
        ui_popover_open(popover);
        settle_popover();
        g_outside_click_count = 0;

        memset(&event, 0, sizeof(event));
        event.type = UI_EVENT_MOUSEDOWN;
        event.mouse.x = 440;
        event.mouse.y = 416;
        ui_dispatch_event(&event);
        event.type = UI_EVENT_MOUSEUP;
        ui_dispatch_event(&event);
        ui_update();

        ctest_equal_int("real outside click should reach the button",
                        g_outside_click_count, 1);
        ctest_equal_bool("real outside click should close",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_widget_destroy(outer);
        ui_clear_trash();
}

static void test_outside_click_in_demo_area(void)
{
        ui_widget_t *demo = ui_create_widget(NULL);
        ui_widget_t *preview = ui_create_widget(NULL);
        ui_widget_t *popover = ui_create_popover();
        ui_event_t event;

        ui_widget_set_style_string(demo, "position", "absolute");
        ui_widget_set_style_string(demo, "left", "0");
        ui_widget_set_style_string(demo, "top", "0");
        ui_widget_set_style_string(demo, "width", "600px");
        ui_widget_set_style_string(demo, "height", "300px");
        ui_widget_set_style_string(preview, "width", "600px");
        ui_widget_set_style_string(preview, "height", "300px");
        ui_widget_append(demo, preview);
        ui_root_append(demo);
        ui_popover_build_children(popover);
        ui_widget_set_style_string(popover, "position", "absolute");
        ui_widget_set_style_string(popover, "left", "200px");
        ui_widget_set_style_string(popover, "top", "40px");
        ui_widget_append(preview, popover);
        ui_update();
        ui_widget_on(demo, "click", on_noop_click, NULL);
        ui_popover_open(popover);
        settle_popover();

        memset(&event, 0, sizeof(event));
        event.type = UI_EVENT_MOUSEDOWN;
        event.mouse.x = 50;
        event.mouse.y = 50;
        ui_dispatch_event(&event);
        event.type = UI_EVENT_MOUSEUP;
        ui_dispatch_event(&event);
        ui_update();

        ctest_equal_bool("clicking the demo background should close",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(demo);
        ui_clear_trash();
}

static void test_scroll_repositions(void)
{
        ui_widget_t *container = ui_create_scrollarea();
        ui_widget_t *scroll_content = ui_create_scrollarea_content();
        ui_widget_t *popover = ui_create_popover();
        ui_widget_t *content;
        ui_widget_t *trigger;
        float cx0, cy0, cx1, cy1, tx0, ty0, tx1, ty1;

        ui_widget_set_style_string(container, "position", "absolute");
        ui_widget_set_style_string(container, "left", "0");
        ui_widget_set_style_string(container, "top", "0");
        ui_widget_set_style_string(container, "width", "300px");
        ui_widget_set_style_string(container, "height", "200px");
        ui_widget_set_style_string(scroll_content, "height", "600px");
        ui_widget_append(container, scroll_content);
        ui_root_append(container);

        ui_popover_build_children(popover);
        ui_popover_set_title(popover, "Scrolled");
        ui_widget_set_style_string(popover, "position", "absolute");
        ui_widget_set_style_string(popover, "left", "20px");
        ui_widget_set_style_string(popover, "top", "60px");
        ui_widget_append(scroll_content, popover);
        ui_update();

        trigger = find_by_type(popover, "popover-trigger");
        content = ui_popover_get_content(popover);
        ui_popover_open(popover);
        settle_popover();
        ui_widget_get_offset(content, ui_root(), &cx0, &cy0);
        ui_widget_get_offset(trigger, ui_root(), &tx0, &ty0);

        ui_scrollarea_set_scroll_top(container, 50);
        ui_update();
        ui_update();
        ui_widget_get_offset(content, ui_root(), &cx1, &cy1);
        ui_widget_get_offset(trigger, ui_root(), &tx1, &ty1);

        ctest_equal_bool("scroll should move the trigger", ty1 < ty0 - 1, true);
        ctest_equal_bool("content should follow the scrolled trigger",
                         fabsf((cy1 - cy0) - (ty1 - ty0)) <= 1, true);
        (void)cx0;
        (void)cx1;
        (void)tx0;
        (void)tx1;

        ui_widget_destroy(container);
        ui_clear_trash();
}

static void test_content_click_keeps_open(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *button = ui_create_button();

        ui_widget_append(content, button);
        ui_popover_open(popover);
        click_widget(button);
        ctest_equal_bool("clicking content should keep the popover open",
                         ui_popover_is_open(popover), true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_switch_triggers_in_one_click(void)
{
        ui_widget_t *first = create_built_popover();
        ui_widget_t *second = create_built_popover();
        ui_widget_t *second_button = find_trigger_button(second);

        ui_popover_open(first);
        click_widget(second_button);
        ui_update();
        ctest_equal_bool("switching triggers should close the first",
                         ui_popover_is_open(first), false);
        ctest_equal_bool("switching triggers should open the second",
                         ui_popover_is_open(second), true);

        ui_widget_destroy(first);
        ui_widget_destroy(second);
        ui_clear_trash();
}

static void test_escape_closes(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_event_t event;

        ui_popover_open(popover);
        ui_event_init(&event, "keydown");
        event.key.code = KEY_ESCAPE;
        ui_widget_emit_event(ui_root(), event, NULL);

        ctest_equal_bool("escape should close the popover",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_focus_first_focusable(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *trigger_button = find_trigger_button(popover);
        ui_widget_t *first = ui_create_button();
        ui_widget_t *second = ui_create_button();

        ui_widget_append(content, first);
        ui_widget_append(content, second);
        ui_set_focus(trigger_button);
        ui_popover_open(popover);
        settle_popover();

        ctest_equal_bool("open should focus the first focusable descendant",
                         ui_get_focus_in(ui_root()) == first, true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_focus_without_focusable(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *trigger_button = find_trigger_button(popover);

        ui_set_focus(trigger_button);
        ui_popover_open(popover);
        settle_popover();

        ctest_equal_bool("content without focusable should keep trigger focus",
                         ui_get_focus_in(ui_root()) == trigger_button, true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_escape_restores_focus(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *trigger_button = find_trigger_button(popover);
        ui_widget_t *button = ui_create_button();
        ui_event_t event;

        ui_widget_append(content, button);
        ui_set_focus(trigger_button);
        ui_popover_open(popover);
        settle_popover();
        ctest_equal_bool("content button should receive focus",
                         ui_get_focus_in(ui_root()) == button, true);

        ui_event_init(&event, "keydown");
        event.key.code = KEY_ESCAPE;
        ui_widget_emit_event(ui_root(), event, NULL);

        ctest_equal_bool("escape should close", ui_popover_is_open(popover),
                         false);
        ctest_equal_bool("escape should restore trigger focus",
                         ui_get_focus_in(ui_root()) == trigger_button, true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_outside_click_keeps_focus(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *outside = ui_create_button();

        ui_root_append(outside);
        ui_update();
        ui_popover_open(popover);
        settle_popover();
        ui_set_focus(outside);
        click_widget(outside);

        ctest_equal_bool("outside click should close",
                         ui_popover_is_open(popover), false);
        ctest_equal_bool("outside click should keep outside focus",
                         ui_get_focus_in(ui_root()) == outside, true);

        ui_widget_destroy(popover);
        ui_widget_destroy(outside);
        ui_clear_trash();
}

static ui_widget_t *find_portal(ui_widget_t *popover)
{
        return find_by_type(popover, "portal");
}

static void test_default_placement(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *trigger = find_by_type(popover, "popover-trigger");
        ui_widget_t *portal;
        float cx, cy, tx, ty;

        ui_popover_open(popover);
        settle_popover();
        portal = find_portal(popover);
        ui_widget_get_offset(content, ui_root(), &cx, &cy);
        ui_widget_get_offset(trigger, ui_root(), &tx, &ty);

        ctest_equal_str("default side should be bottom",
                        ui_widget_get_attr(portal, "side"), "bottom");
        ctest_equal_str("default align should be center",
                        ui_widget_get_attr(portal, "align"), "center");
        ctest_equal_str("default side offset should be 4",
                        ui_widget_get_attr(portal, "side-offset"), "4");
        ctest_equal_str("default align offset should be 0",
                        ui_widget_get_attr(portal, "align-offset"), "0");
        ctest_equal_bool("default content should sit below the trigger",
                         cy >= ty + trigger->border_box.height + 4 - 1, true);
        (void)cx;

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_side_align_offsets(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *trigger = find_by_type(popover, "popover-trigger");
        ui_widget_t *portal;
        float cx, cy, tx, ty;

        ui_popover_content_set_side(content, UI_SIDE_RIGHT);
        ui_popover_content_set_align(content, UI_ALIGN_END);
        ui_popover_content_set_side_offset(content, 8);
        ui_popover_content_set_align_offset(content, -4);
        ui_popover_open(popover);
        settle_popover();
        portal = find_portal(popover);
        ui_widget_get_offset(content, ui_root(), &cx, &cy);
        ui_widget_get_offset(trigger, ui_root(), &tx, &ty);

        ctest_equal_str("side setter should reach the portal",
                        ui_widget_get_attr(portal, "side"), "right");
        ctest_equal_str("align setter should reach the portal",
                        ui_widget_get_attr(portal, "align"), "end");
        ctest_equal_str("side offset setter should reach the portal",
                        ui_widget_get_attr(portal, "side-offset"), "8");
        ctest_equal_str("align offset setter should reach the portal",
                        ui_widget_get_attr(portal, "align-offset"), "-4");
        ctest_equal_bool("right content should sit right of the trigger",
                         cx >= tx + trigger->border_box.width + 8 - 1, true);
        (void)cy;
        (void)ty;

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_invalid_content_attrs(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *portal;

        ui_widget_set_attr(content, "side", "diagonal");
        ui_widget_set_attr(content, "align", "middle");
        ui_popover_open(popover);
        settle_popover();
        portal = find_portal(popover);

        ctest_equal_str("invalid side should fall back to bottom",
                        ui_widget_get_attr(portal, "side"), "bottom");
        ctest_equal_str("invalid align should fall back to center",
                        ui_widget_get_attr(portal, "align"), "center");

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_zero_offset(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *portal;

        ui_popover_content_set_side_offset(content, 0);
        ui_popover_open(popover);
        settle_popover();
        portal = find_portal(popover);

        ctest_equal_str("zero side offset should be preserved",
                        ui_widget_get_attr(portal, "side-offset"), "0");

        ui_widget_destroy(popover);
        ui_clear_trash();
}

static void test_content_first_destroy(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);

        ui_popover_open(popover);
        ui_widget_destroy(content);
        ui_update();

        ctest_equal_bool("content destroy should close the popover",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_update();
        ui_clear_trash();
}

static void test_owner_first_destroy(void)
{
        ui_widget_t *first = create_built_popover();
        ui_widget_t *second;

        ui_popover_open(first);
        ui_widget_destroy(first);
        ui_update();

        second = create_built_popover();
        ui_popover_open(second);
        ctest_equal_bool("a new popover should work after owner destroy",
                         ui_popover_is_open(second), true);

        ui_widget_destroy(second);
        ui_update();
        ui_clear_trash();
}

static void test_trigger_destroy_while_open(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *trigger = find_by_type(popover, "popover-trigger");

        ui_popover_open(popover);
        ui_widget_destroy(trigger);
        ui_update();

        ctest_equal_bool("trigger destroy should close the popover",
                         ui_popover_is_open(popover), false);

        ui_widget_destroy(popover);
        ui_update();
        ui_clear_trash();
}

static void test_destroy_when_closed(void)
{
        ui_widget_t *popover = create_built_popover();

        ui_widget_destroy(popover);
        ui_update();
        ui_clear_trash();
}

static void test_content_layout(void)
{
        ui_widget_t *popover = create_built_popover();
        ui_widget_t *content = ui_popover_get_content(popover);
        ui_widget_t *header = find_by_type(content, "popover-header");
        ui_widget_t *title = find_by_type(content, "popover-title");
        ui_widget_t *description = find_by_type(content, "popover-description");
        ui_widget_t *button = ui_create_button();

        ui_widget_append(content, button);
        ui_popover_open(popover);
        settle_popover();

        ctest_equal_float("content should use the default width",
                          content->border_box.width, 288.0f);
        ctest_equal_bool("content should have height",
                         content->border_box.height > 0, true);
        ctest_equal_bool("header should have height",
                         header && header->border_box.height > 0, true);
        ctest_equal_bool("title should have height",
                         title && title->border_box.height > 0, true);
        ctest_equal_bool("description should have height",
                         description && description->border_box.height > 0,
                         true);
        ctest_equal_bool("button should have height",
                         button->border_box.height > 0, true);

        ui_widget_destroy(popover);
        ui_clear_trash();
}

void test_popover(void)
{
        lcui_init();
        ctest_describe("popover structure", test_factories_and_build);
        ctest_describe("popover custom structure",
                       test_custom_structure_is_preserved);
        ctest_describe("popover text setters", test_text_setters);
        ctest_describe("popover open and close", test_open_close_api);
        ctest_describe("popover trigger toggles", test_trigger_toggles);
        ctest_describe("popover open attr and events",
                       test_open_attr_and_events);
        ctest_describe("popover open attr before update",
                       test_open_attr_before_update);
        ctest_describe("popover open without content",
                       test_open_without_content);
        ctest_describe("popover single active", test_single_active);
        ctest_describe("popover outside click",
                       test_outside_click_through_closes);
        ctest_describe("popover real outside click",
                       test_outside_real_click_closes);
        ctest_describe("popover demo area outside click",
                       test_outside_click_in_demo_area);
        ctest_describe("popover scroll repositions", test_scroll_repositions);
        ctest_describe("popover content click", test_content_click_keeps_open);
        ctest_describe("popover trigger switching",
                       test_switch_triggers_in_one_click);
        ctest_describe("popover escape closes", test_escape_closes);
        ctest_describe("popover focus first focusable",
                       test_focus_first_focusable);
        ctest_describe("popover focus without focusable",
                       test_focus_without_focusable);
        ctest_describe("popover escape restores focus",
                       test_escape_restores_focus);
        ctest_describe("popover outside click keeps focus",
                       test_outside_click_keeps_focus);
        ctest_describe("popover default placement", test_default_placement);
        ctest_describe("popover side align offsets", test_side_align_offsets);
        ctest_describe("popover invalid content attrs",
                       test_invalid_content_attrs);
        ctest_describe("popover zero offset", test_zero_offset);
        ctest_describe("popover content first destroy",
                       test_content_first_destroy);
        ctest_describe("popover owner first destroy", test_owner_first_destroy);
        ctest_describe("popover trigger destroy",
                       test_trigger_destroy_while_open);
        ctest_describe("popover destroy when closed", test_destroy_when_closed);
        ctest_describe("popover content layout", test_content_layout);
        lcui_destroy();
}
