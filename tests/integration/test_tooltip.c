/*
 * tests/integration/test_tooltip.c: -- Tooltip widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <LCUI.h>
#include <ctest-custom.h>

static ui_widget_t *find_inner(ui_widget_t *content);

static void test_structure(void)
{
        ui_widget_t *tooltip = ui_create_tooltip();
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *content = ui_create_tooltip_content();

        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, content);
        ui_root_append(tooltip);
        ui_update();

        ctest_equal_bool("tooltip should create trigger prototype",
                         ui_check_widget_type(trigger, "tooltip-trigger"),
                         true);
        ctest_equal_bool("tooltip should create content prototype",
                         ui_check_widget_type(content, "tooltip-content"),
                         true);
        ctest_equal_int("delay should default to 0",
                        ui_tooltip_get_delay(tooltip), 0);
        ui_tooltip_set_delay(tooltip, 120);
        ctest_equal_int("delay should be writable",
                        ui_tooltip_get_delay(tooltip), 120);
        ctest_equal_bool("tooltip should start closed",
                         ui_tooltip_is_open(tooltip), false);

        ui_widget_destroy(tooltip);
}

static ui_widget_t *find_tooltip_content(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, "tooltip-content")) {
                        return child;
                }
                found = find_tooltip_content(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *find_portal_tooltip_content(void)
{
        ui_widget_t *root = ui_get_portal_root();

        return root ? find_tooltip_content(root) : NULL;
}

static void test_portal_structure(void)
{
        ui_widget_t *tooltip = ui_create_tooltip();
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *content = ui_create_tooltip_content();
        ui_widget_t *arrow;
        int i;

        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, content);
        ui_root_append(tooltip);
        ui_update();

        ctest_equal_bool("content should be reparented to portal root",
                         content->parent == ui_get_portal_root(), true);
        arrow = NULL;
        for (i = 0; (arrow = ui_widget_get_child(content, i)); ++i) {
                if (ui_widget_has_class(arrow, "tooltip-arrow")) {
                        break;
                }
        }
        ctest_equal_bool("content should own an arrow", arrow != NULL, true);

        ui_tooltip_content_set_side(content, UI_SIDE_LEFT);
        ui_tooltip_content_set_align(content, UI_ALIGN_END);
        ui_tooltip_content_set_side_offset(content, 8);
        ui_tooltip_content_set_align_offset(content, -4);
        ctest_equal_str("side setter should store attr",
                        ui_widget_get_attr(content, "side"), "left");
        ctest_equal_str("align setter should store attr",
                        ui_widget_get_attr(content, "align"), "end");
        ctest_equal_str("side offset setter should store attr",
                        ui_widget_get_attr(content, "side-offset"), "8");
        ctest_equal_str("align offset setter should store attr",
                        ui_widget_get_attr(content, "align-offset"), "-4");

        ui_tooltip_content_set_side(content, (ui_tooltip_side_t)99);
        ctest_equal_str("invalid enum side should fall back to top",
                        ui_widget_get_attr(content, "side"), "top");
        ui_tooltip_content_set_align(content, (ui_tooltip_align_t)99);
        ctest_equal_str("invalid enum align should fall back to center",
                        ui_widget_get_attr(content, "align"), "center");

        ui_widget_destroy(tooltip);
}

static void test_destroy_orders(void)
{
        ui_widget_t *tooltip;
        ui_widget_t *content;

        tooltip = ui_create_tooltip();
        ui_widget_append(tooltip, ui_create_tooltip_trigger());
        content = ui_create_tooltip_content();
        ui_widget_append(tooltip, content);
        ui_root_append(tooltip);
        ui_update();
        ui_widget_destroy(content);
        ui_widget_destroy(tooltip);
        ui_update();
        ctest_equal_bool("content-first destroy should not leave content",
                         find_portal_tooltip_content() == NULL, true);

        tooltip = ui_create_tooltip();
        ui_widget_append(tooltip, ui_create_tooltip_trigger());
        ui_widget_append(tooltip, ui_create_tooltip_content());
        ui_root_append(tooltip);
        ui_update();
        ui_widget_destroy(tooltip);
        ui_update();
        ctest_equal_bool("owner-first destroy should clean portal content",
                         find_portal_tooltip_content() == NULL, true);
}

static void test_show_hide_position(void)
{
        ui_widget_t *tooltip = ui_create_tooltip();
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_tooltip_content();
        ui_widget_t *label = ui_create_widget("text");
        float tx, ty, cx, cy;

        ui_widget_set_style_string(trigger, "margin-left", "200px");
        ui_widget_set_style_string(trigger, "margin-top", "200px");
        ui_widget_set_style_string(button, "width", "80px");
        ui_widget_set_style_string(button, "height", "32px");
        ui_text_set_content(label, "Add to library");
        ui_widget_append(trigger, button);
        ui_widget_append(content, label);
        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, content);
        ui_root_append(tooltip);
        ui_update();

        ui_tooltip_show(tooltip);
        ui_update();
        ui_update(); /* let the portal settle after the content's first layout
                      */
        ctest_equal_bool("show should open tooltip",
                         ui_tooltip_is_open(tooltip), true);
        ctest_equal_bool("show should reveal content",
                         ui_widget_is_visible(content), true);
        ui_widget_get_offset(trigger, ui_root(), &tx, &ty);
        ui_widget_get_offset(content, ui_root(), &cx, &cy);
        ctest_equal_bool("side=top content should sit above trigger",
                         cy + content->border_box.height <= ty + 1.0f, true);
        ctest_equal_bool("align=center should center content on trigger",
                         fabsf((cx + content->border_box.width / 2) -
                               (tx + trigger->border_box.width / 2)) <= 1.5f,
                         true);

        ui_tooltip_content_set_side(content, UI_SIDE_BOTTOM);
        ui_tooltip_show(tooltip);
        ui_update();
        ui_update();
        ui_widget_get_offset(content, ui_root(), &cx, &cy);
        ctest_equal_bool("side=bottom content should sit below trigger",
                         cy >= ty + trigger->border_box.height - 1.0f, true);

        ui_tooltip_hide(tooltip);
        ctest_equal_bool("hide should close tooltip",
                         ui_tooltip_is_open(tooltip), false);

        ui_widget_destroy(tooltip);
}

static void hover_at(float x, float y)
{
        ui_event_t e = { 0 };

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x = x;
        e.mouse.y = y;
        ui_dispatch_event(&e);
        ui_update();
        ui_update();
}

static void emit_escape(ui_widget_t *w)
{
        ui_event_t e = { 0 };

        ui_event_init(&e, "keydown");
        e.key.code = KEY_ESCAPE;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
}

static bool widget_is_descendant(ui_widget_t *ancestor, ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (w == ancestor) {
                        return true;
                }
        }
        return false;
}

static void test_hover_focus(void)
{
        ui_widget_t *tooltip = ui_create_tooltip();
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *content = ui_create_tooltip_content();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *other = ui_create_button();

        ui_widget_set_style_string(trigger, "margin-left", "200px");
        ui_widget_set_style_string(trigger, "margin-top", "200px");
        ui_widget_set_style_string(button, "width", "80px");
        ui_widget_set_style_string(button, "height", "32px");
        ui_widget_set_style_string(other, "margin-left", "400px");
        ui_widget_set_style_string(other, "margin-top", "200px");
        ui_icon_set_name(icon, "checkmark");
        ui_text_set_content(label, "Add to library");
        ui_widget_append(button, icon);
        ui_widget_append(trigger, button);
        ui_widget_append(content, label);
        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, content);
        ui_root_append(tooltip);
        ui_root_append(other);
        ui_update();

        hover_at(240, 216);
        ctest_equal_bool("hover should open", ui_tooltip_is_open(tooltip),
                         true);
        ctest_equal_bool("open tooltip should keep the trigger hovered",
                         widget_is_descendant(button, ui_get_mouseover_in(ui_root())),
                         true);

        hover_at(230, 210);
        ctest_equal_bool("hovering a child should keep it open",
                         ui_tooltip_is_open(tooltip), true);

        hover_at(20, 20);
        ctest_equal_bool("leaving trigger should close",
                         ui_tooltip_is_open(tooltip), false);

        ui_set_focus(button);
        ui_update();
        ctest_equal_bool("focus should open", ui_tooltip_is_open(tooltip),
                         true);
        ui_set_focus(other);
        ui_update();
        ctest_equal_bool("blur should close", ui_tooltip_is_open(tooltip),
                         false);

        ui_set_focus(button);
        ui_update();
        emit_escape(button);
        ctest_equal_bool("escape should close", ui_tooltip_is_open(tooltip),
                         false);

        ui_tooltip_set_delay(tooltip, 100);
        hover_at(240, 216);
        ctest_equal_bool("delay>0 should not open immediately",
                         ui_tooltip_is_open(tooltip), false);
        ui_tooltip_hide(tooltip);
        ui_update();
        ctest_equal_bool("hide should cancel pending show",
                         ui_tooltip_is_open(tooltip), false);

        ui_tooltip_set_delay(tooltip, 0);
        hover_at(20, 20);
        hover_at(240, 216);
        ctest_equal_bool("delay=0 should open immediately",
                         ui_tooltip_is_open(tooltip), true);

        ui_widget_destroy(tooltip);
        ui_widget_destroy(other);
}

static void test_loose_binding(void)
{
        ui_widget_t *trigger = ui_create_widget("button");
        ui_widget_t *trigger2 = ui_create_widget("button");
        ui_widget_t *content = ui_create_tooltip_content();
        ui_widget_t *label = ui_create_widget("text");

        ui_widget_set_attr(trigger, "data-tooltip-trigger", "save");
        ui_widget_set_attr(trigger2, "data-tooltip-trigger", "save");
        ui_widget_set_attr(content, "data-tooltip-target", "save");
        ui_widget_set_style_string(trigger, "position", "absolute");
        ui_widget_set_style_string(trigger, "left", "200px");
        ui_widget_set_style_string(trigger, "top", "200px");
        ui_widget_set_style_string(trigger, "width", "80px");
        ui_widget_set_style_string(trigger, "height", "32px");
        ui_widget_set_style_string(trigger2, "position", "absolute");
        ui_widget_set_style_string(trigger2, "left", "400px");
        ui_widget_set_style_string(trigger2, "top", "200px");
        ui_widget_set_style_string(trigger2, "width", "80px");
        ui_widget_set_style_string(trigger2, "height", "32px");
        ui_text_set_content(label, "Save changes");
        ui_widget_append(content, label);
        ui_root_append(trigger);
        ui_root_append(trigger2);
        ui_root_append(content);
        ui_update();

        hover_at(240, 216);
        ctest_equal_bool("loose trigger should move content to portal",
                         content->parent == ui_get_portal_root(), true);
        ctest_equal_bool("loose trigger should show content",
                         ui_widget_is_visible(content), true);
        hover_at(20, 20);
        ctest_equal_bool("leaving loose trigger should hide content",
                         ui_widget_is_visible(content), false);

        hover_at(440, 216);
        ctest_equal_bool("second trigger with same key should reuse content",
                         ui_widget_is_visible(content), true);

        ui_widget_destroy(trigger);
        ui_widget_destroy(trigger2);
}

static ui_widget_t *find_visible_content(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, "tooltip-content") &&
                    ui_widget_is_visible(child)) {
                        return child;
                }
                found = find_visible_content(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static ui_widget_t *find_visible_tooltip_content(void)
{
        ui_widget_t *root = ui_get_portal_root();

        return root ? find_visible_content(root) : NULL;
}

static ui_widget_t *find_text_child(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_type(child, "text")) {
                        return child;
                }
                found = find_text_child(child);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static void test_attribute_tooltip(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *other = ui_create_button();
        ui_widget_t *content;
        float cx, cy;
        wchar_t text[64];

        ui_widget_set_attr(button, "data-tooltip", "Add to library");
        ui_widget_set_attr(button, "data-tooltip-side", "left");
        ui_widget_set_attr(button, "data-tooltip-side-offset", "8");
        ui_widget_set_attr(button, "data-tooltip-delay", "0");
        ui_widget_set_attr(other, "data-tooltip", "Other");
        ui_widget_set_style_string(button, "position", "absolute");
        ui_widget_set_style_string(button, "left", "200px");
        ui_widget_set_style_string(button, "top", "200px");
        ui_widget_set_style_string(button, "width", "80px");
        ui_widget_set_style_string(button, "height", "32px");
        ui_widget_set_style_string(other, "position", "absolute");
        ui_widget_set_style_string(other, "left", "400px");
        ui_widget_set_style_string(other, "top", "200px");
        ui_widget_set_style_string(other, "width", "80px");
        ui_widget_set_style_string(other, "height", "32px");
        ui_root_append(button);
        ui_root_append(other);
        ui_update();

        hover_at(240, 216);
        ui_update(); /* settle the attribute tooltip position */
        content = find_visible_tooltip_content();
        ctest_equal_bool("attribute should create a content", content != NULL,
                         true);
        if (content) {
                ui_text_get_content_w(find_text_child(content), text, 64);
                ctest_equal_bool("attribute should use data-tooltip text",
                                 wcscmp(text, L"Add to library") == 0, true);
                ctest_equal_bool(
                    "attribute should apply data-tooltip-side",
                    ui_widget_has_class(content, "tooltip-side-left"), true);
                ui_widget_get_offset(content, ui_root(), &cx, &cy);
                ctest_equal_bool(
                    "attribute should apply data-tooltip-side-offset",
                    cx + content->border_box.width <= 200 - 8 + 1, true);
        }

        hover_at(440, 216);
        ctest_equal_bool("previous attribute target should hide",
                         content ? ui_widget_is_visible(content) : false,
                         false);

        ui_widget_set_attr(button, "data-tooltip", "");
        hover_at(240, 216);
        ctest_equal_bool("empty data-tooltip should not show",
                         find_visible_tooltip_content() == NULL, true);

        ui_widget_set_attr(button, "data-tooltip", "Invalid side");
        ui_widget_set_attr(button, "data-tooltip-side", "diagonal");
        ui_widget_set_attr(button, "data-tooltip-delay", "abc");
        hover_at(20, 20);
        hover_at(240, 216);
        content = find_visible_tooltip_content();
        ctest_equal_bool(
            "invalid side should fall back to top",
            content && ui_widget_has_class(content, "tooltip-side-top"), true);
        ctest_equal_bool("invalid delay should fall back to 0",
                         content ? ui_widget_is_visible(content) : false, true);

        ui_widget_destroy(button);
        ui_update();
        hover_at(20, 20);
        hover_at(440, 216);
        ctest_equal_bool("other target should still work after destroy",
                         find_visible_tooltip_content() != NULL, true);

        ui_widget_destroy(other);
}

static void test_attach_api(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content;
        wchar_t text[64];

        ui_widget_set_style_string(button, "position", "absolute");
        ui_widget_set_style_string(button, "left", "200px");
        ui_widget_set_style_string(button, "top", "200px");
        ui_widget_set_style_string(button, "width", "80px");
        ui_widget_set_style_string(button, "height", "32px");
        ui_tooltip_attach(button, "Save");
        ctest_equal_str("attach should store data-tooltip",
                        ui_widget_get_attr(button, "data-tooltip"), "Save");
        ui_root_append(button);
        ui_update();

        hover_at(240, 216);
        content = find_visible_tooltip_content();
        ctest_equal_bool("attach should show a content", content != NULL, true);
        if (content) {
                ui_text_get_content_w(find_text_child(content), text, 64);
                ctest_equal_bool("attach should show the text",
                                 wcscmp(text, L"Save") == 0, true);
        }

        ui_tooltip_attach(button, "Open");
        ui_update();
        ui_update();
        ui_update();
        content = find_visible_tooltip_content();
        ctest_equal_bool("attach should keep the visible content",
                         content != NULL, true);
        if (content) {
                ui_text_get_content_w(find_text_child(content), text, 64);
                ctest_equal_bool("attach should update the visible text",
                                 wcscmp(text, L"Open") == 0, true);
        }

        ui_tooltip_attach(button, NULL);
        ui_update();
        ctest_equal_bool("attach NULL should clear data-tooltip",
                         ui_widget_get_attr(button, "data-tooltip") == NULL,
                         true);
        ctest_equal_bool("attach NULL should hide the content",
                         find_visible_tooltip_content() == NULL, true);
        hover_at(20, 20);
        hover_at(240, 216);
        ctest_equal_bool("attach NULL should not show again",
                         find_visible_tooltip_content() == NULL, true);

        ui_widget_destroy(button);
}

static void test_attach_options(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content;
        ui_tooltip_options_t options;
        wchar_t text[64];

        ui_tooltip_options_init(&options);
        ctest_equal_int("options should default delay", options.delay, 0);
        ctest_equal_int("options should default side", (int)options.side,
                        UI_SIDE_TOP);
        ctest_equal_int("options should default align", (int)options.align,
                        UI_ALIGN_CENTER);
        ctest_equal_int("options should default side offset",
                        options.side_offset, 4);
        ctest_equal_int("options should default align offset",
                        options.align_offset, 0);

        options.content = "Options";
        options.delay = 0;
        options.side = UI_SIDE_LEFT;
        options.align = UI_ALIGN_END;
        options.side_offset = 8;
        options.align_offset = -4;
        ui_tooltip_attach_ex(button, &options);
        ctest_equal_str("attach_ex should store side",
                        ui_widget_get_attr(button, "data-tooltip-side"),
                        "left");
        ctest_equal_str("attach_ex should store align",
                        ui_widget_get_attr(button, "data-tooltip-align"),
                        "end");
        ctest_equal_str("attach_ex should store side offset",
                        ui_widget_get_attr(button, "data-tooltip-side-offset"),
                        "8");
        ctest_equal_str("attach_ex should store align offset",
                        ui_widget_get_attr(button, "data-tooltip-align-offset"),
                        "-4");
        ui_widget_set_style_string(button, "position", "absolute");
        ui_widget_set_style_string(button, "left", "200px");
        ui_widget_set_style_string(button, "top", "200px");
        ui_widget_set_style_string(button, "width", "80px");
        ui_widget_set_style_string(button, "height", "32px");
        ui_root_append(button);
        ui_update();

        hover_at(240, 216);
        content = find_visible_tooltip_content();
        ctest_equal_bool("attach_ex should show a content", content != NULL,
                         true);
        if (content) {
                ui_text_get_content_w(find_text_child(content), text, 64);
                ctest_equal_bool("attach_ex should show the text",
                                 wcscmp(text, L"Options") == 0, true);
                ctest_equal_str("content should use side",
                                ui_widget_get_attr(content, "side"), "left");
                ctest_equal_str("content should use align",
                                ui_widget_get_attr(content, "align"), "end");
                ctest_equal_str("content should use side offset",
                                ui_widget_get_attr(content, "side-offset"),
                                "8");
                ctest_equal_str("content should use align offset",
                                ui_widget_get_attr(content, "align-offset"),
                                "-4");
                ctest_equal_bool(
                    "content should use left side class",
                    ui_widget_has_class(content, "tooltip-side-left"), true);
        }

        ui_tooltip_attach_ex(button, NULL);
        ctest_equal_str("attach_ex NULL should keep attrs",
                        ui_widget_get_attr(button, "data-tooltip"), "Options");
        ui_widget_destroy(button);
}

static ui_widget_t *find_arrow(ui_widget_t *content)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (ui_widget_has_class(child, "tooltip-arrow")) {
                        return child;
                }
        }
        return NULL;
}

static ui_widget_t *find_inner(ui_widget_t *content)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (ui_widget_has_class(child, "tooltip-inner")) {
                        return child;
                }
        }
        return NULL;
}

static void test_arrow_geometry(void)
{
        ui_widget_t *tooltip = ui_create_tooltip();
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *content = ui_create_tooltip_content();
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *arrow;
        ui_widget_t *inner;

        ui_text_set_content(label, "Add to library");
        ui_widget_append(trigger, ui_create_button());
        ui_widget_append(content, label);
        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, content);
        ui_root_append(tooltip);
        ui_update();

        ui_tooltip_content_set_side(content, UI_SIDE_TOP);
        ui_tooltip_show(tooltip);
        ui_update();
        ui_update();
        arrow = find_arrow(content);
        inner = find_inner(content);
        ctest_equal_bool("top tooltip should have an arrow", arrow != NULL,
                         true);
        ctest_equal_bool("top tooltip should have an inner bubble",
                         inner != NULL, true);
        ctest_equal_bool("content should shrink-wrap the inner bubble",
                         inner && content->border_box.width <=
                                      inner->border_box.width + 6.0f,
                         true);
        ui_widget_set_attr(content, "side", "diagonal");
        ui_tooltip_show(tooltip);
        ui_update();
        ctest_equal_bool("invalid side attr should fall back to top",
                         ui_widget_has_class(content, "tooltip-side-top"),
                         true);

        ui_tooltip_content_set_side(content, UI_SIDE_BOTTOM);
        ui_tooltip_show(tooltip);
        ui_update();
        ctest_equal_bool("bottom tooltip should keep its arrow",
                         find_arrow(content) != NULL, true);

        ui_tooltip_content_set_side(content, UI_SIDE_LEFT);
        ui_tooltip_show(tooltip);
        ui_update();
        ctest_equal_bool("left tooltip should keep its arrow",
                         find_arrow(content) != NULL, true);

        ui_tooltip_content_set_side(content, UI_SIDE_RIGHT);
        ui_tooltip_show(tooltip);
        ui_update();
        ctest_equal_bool("right tooltip should keep its arrow",
                         find_arrow(content) != NULL, true);

        ui_widget_destroy(tooltip);
}

static void add_side_tooltip(ui_widget_t *parent, ui_tooltip_side_t side,
                             const char *label)
{
        ui_widget_t *tooltip = ui_create_tooltip();
        ui_widget_t *trigger = ui_create_tooltip_trigger();
        ui_widget_t *content = ui_create_tooltip_content();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *text = ui_create_widget("text");

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_text(button, label);
        ui_widget_append(trigger, button);
        ui_text_set_content(text, "Add to library");
        ui_widget_append(content, text);
        ui_tooltip_content_set_side(content, side);
        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, content);
        ui_widget_append(parent, tooltip);
}

static void test_sides_layout(void)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_t *tooltip;
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *content;
        ui_widget_t *inner;
        float x, y;

        ui_widget_add_class(row, "tooltip-sides-demo");
        add_side_tooltip(row, UI_SIDE_LEFT, "Left");
        add_side_tooltip(row, UI_SIDE_TOP, "Top");
        add_side_tooltip(row, UI_SIDE_BOTTOM, "Bottom");
        add_side_tooltip(row, UI_SIDE_RIGHT, "Right");
        ui_root_append(row);
        ui_update();

        tooltip = ui_widget_get_child(row, 0);
        trigger = ui_widget_get_child(tooltip, 0);
        button = ui_widget_get_child(trigger, 0);
        ui_widget_get_offset(button, ui_root(), &x, &y);
        hover_at(x + button->border_box.width / 2,
                 y + button->border_box.height / 2);
        ui_update();

        content = find_visible_tooltip_content();
        inner = content ? find_inner(content) : NULL;
        ctest_equal_bool("sides tooltip should open", content != NULL, true);
        ctest_equal_bool("sides tooltip should be visible",
                         content ? ui_widget_is_visible(content) : false, true);
        ctest_equal_bool("sides tooltip should have an inner bubble",
                         inner != NULL, true);
        ctest_equal_bool("sides tooltip text should be laid out",
                         inner && inner->border_box.width > 0 &&
                             inner->border_box.height > 0,
                         true);
        {
                ui_widget_t *text = inner ? find_text_child(inner) : NULL;

                ctest_equal_int(
                    "sides tooltip text should use the bubble color",
                    text ? text->computed_style.color : 0,
                    inner ? inner->computed_style.color : 0);
        }

        ui_widget_destroy(row);
}

static void test_tooltip_in_dialog(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content;
        ui_widget_t *overlay;
        ui_widget_t *tooltip;
        ui_widget_t *trigger;
        ui_widget_t *button;
        ui_widget_t *tip_content;
        ui_widget_t *tip_text;
        ui_event_t ev = { 0 };
        float x, y;

        ui_dialog_build_children(dialog);
        ui_root_append(dialog);
        ui_update();
        ui_dialog_open(dialog);
        ui_update();

        content = ui_dialog_get_content(dialog);
        tooltip = ui_create_tooltip();
        trigger = ui_create_tooltip_trigger();
        button = ui_create_button();
        tip_content = ui_create_tooltip_content();
        tip_text = ui_create_widget("text");
        ui_button_set_text(button, "Hover");
        ui_text_set_content(tip_text, "Inside dialog");
        ui_widget_append(trigger, button);
        ui_widget_append(tip_content, tip_text);
        ui_widget_append(tooltip, trigger);
        ui_widget_append(tooltip, tip_content);
        ui_widget_set_style_string(tooltip, "position", "absolute");
        ui_widget_set_style_string(tooltip, "left", "40px");
        ui_widget_set_style_string(tooltip, "top", "80px");
        ui_widget_append(content, tooltip);
        ui_update();

        ui_widget_get_offset(button, ui_root(), &x, &y);
        hover_at(x + button->border_box.width / 2,
                 y + button->border_box.height / 2);
        ctest_equal_bool("tooltip inside dialog should open",
                         ui_tooltip_is_open(tooltip), true);
        ctest_equal_bool("dialog target should keep hover",
                         widget_is_descendant(button, ui_get_mouseover_in(ui_root())),
                         true);

        overlay = content->parent;
        ui_event_init(&ev, "click");
        ui_widget_emit_event(overlay, ev, NULL);
        ui_update();
        ctest_equal_bool("overlay click should still close the dialog",
                         ui_dialog_is_open(dialog), 0);

        ui_widget_destroy(dialog);
}

static void test_shared_placement_types(void)
{
        ui_side_t side = UI_SIDE_RIGHT;
        ui_align_t align = UI_ALIGN_END;
        ui_tooltip_side_t tooltip_side = side;
        ui_tooltip_align_t tooltip_align = align;

        ctest_equal_int("shared side should map to tooltip side",
                        (int)tooltip_side, (int)UI_SIDE_RIGHT);
        ctest_equal_int("shared align should map to tooltip align",
                        (int)tooltip_align, (int)UI_ALIGN_END);
}

void test_tooltip(void)
{
        lcui_init();
        ctest_describe("tooltip structure", test_structure);
        ctest_describe("tooltip portal structure", test_portal_structure);
        ctest_describe("tooltip destroy orders", test_destroy_orders);
        ctest_describe("tooltip show and position", test_show_hide_position);
        ctest_describe("tooltip hover and focus", test_hover_focus);
        ctest_describe("tooltip loose binding", test_loose_binding);
        ctest_describe("tooltip attribute", test_attribute_tooltip);
        ctest_describe("tooltip attach", test_attach_api);
        ctest_describe("tooltip attach options", test_attach_options);
        ctest_describe("tooltip arrow geometry", test_arrow_geometry);
        ctest_describe("tooltip sides layout", test_sides_layout);
        ctest_describe("tooltip in dialog", test_tooltip_in_dialog);
        ctest_describe("tooltip placement types", test_shared_placement_types);
        lcui_destroy();
}
