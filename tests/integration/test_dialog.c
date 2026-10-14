/*
 * tests/integration/test_dialog.c
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
#include <math.h>
#include <string.h>

static void emit_click(ui_widget_t *emitter, ui_widget_t *target)
{
        ui_event_t event;

        ui_event_init(&event, "click");
        event.target = target;
        ui_widget_emit_event(emitter, event, NULL);
}

static void test_dialog_structure(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *trigger;
        ui_widget_t *portal;
        ui_widget_t *overlay;
        ui_widget_t *content;
        ui_widget_t *button;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_update();

        trigger = ui_widget_get_child(dialog, 0);
        portal = ui_widget_get_child(dialog, 1);
        button = ui_widget_get_child(trigger, 0);
        overlay = ui_portal_get_content(portal);
        content = ui_dialog_get_content(dialog);

        ctest_equal_int("build should create a trigger", trigger != NULL, 1);
        ctest_equal_int("trigger should contain a real button",
                        button && button->proto != NULL, 1);
        ctest_equal_int(
            "trigger button should have a non-zero layout",
            button->border_box.width > 0 && button->border_box.height > 0, 1);
        ctest_equal_int("build should create a portal", portal != NULL, 1);
        ctest_equal_int("portal should manage the overlay",
                        overlay != NULL && content != NULL, 1);
        ctest_equal_int("overlay should leave the owner tree after update",
                        overlay->parent == ui_get_portal_root(), 1);
        ctest_equal_int("content should stay inside the overlay",
                        content->parent == overlay, 1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_open_close(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *portal;
        ui_widget_t *overlay;
        ui_widget_t *content;
        float delta;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();

        portal = ui_widget_get_child(dialog, 1);
        overlay = ui_portal_get_content(portal);
        content = ui_dialog_get_content(dialog);
        ui_dialog_open(dialog);
        ui_update();

        ctest_equal_int("open should report the open state",
                        ui_dialog_is_open(dialog), 1);
        ctest_equal_int("overlay should be mounted on the portal root",
                        overlay->parent == ui_get_portal_root(), 1);
        ctest_equal_float("overlay should cover the root width",
                          overlay->border_box.width,
                          ui_root()->border_box.width);
        ctest_equal_float("overlay should cover the root height",
                          overlay->border_box.height,
                          ui_root()->border_box.height);
        delta = (content->border_box.x + content->border_box.width / 2) -
                ui_root()->border_box.width / 2;
        ctest_equal_int("content should be horizontally centered",
                        fabsf(delta) < 2.0f, 1);
        ctest_equal_int("content width should respect max-width",
                        content->border_box.width <= 385.0f, 1);

        ui_dialog_close(dialog);
        ui_update();
        ctest_equal_int("close should report the closed state",
                        ui_dialog_is_open(dialog), 0);
        ctest_equal_int(
            "closed overlay should be hidden",
            overlay->computed_style.type_bits.display == CSS_DISPLAY_NONE, 1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_loose_association(void)
{
        ui_widget_t *trigger = ui_create_widget("button");
        ui_widget_t *overlay;
        ui_widget_t *content;

        ui_widget_set_attr(trigger, "data-dialog-trigger", "profile");
        overlay = ui_create_dialog_overlay();
        ui_widget_set_attr(overlay, "data-dialog-target", "profile");
        content = ui_create_dialog_content();
        ui_widget_append(overlay, content);
        ui_widget_append(ui_root(), trigger);
        ui_widget_append(ui_root(), overlay);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();

        emit_click(trigger, trigger);
        ui_update();
        ctest_equal_int("loose trigger should mount the targeted overlay",
                        overlay->parent == ui_get_portal_root(), 1);
        ctest_equal_int("loose trigger should show the overlay",
                        overlay->border_box.width > 0, 1);

        ui_widget_remove(trigger);
        ui_widget_remove(overlay);
        ui_clear_trash();
}

static void emit_keydown(ui_widget_t *target, int code)
{
        ui_event_t event;

        ui_event_init(&event, "keydown");
        event.target = target;
        event.key.code = (unsigned char)code;
        ui_widget_emit_event(target, event, NULL);
}

static size_t count_children(ui_widget_t *w)
{
        size_t n = 0;

        while (ui_widget_get_child(w, n)) {
                ++n;
        }
        return n;
}

static bool has_ancestor(ui_widget_t *w, ui_widget_t *ancestor)
{
        while (w) {
                if (w == ancestor) {
                        return true;
                }
                w = w->parent;
        }
        return false;
}

typedef struct dialog_fixture {
        ui_widget_t *dialog;
        ui_widget_t *portal;
        ui_widget_t *overlay;
        ui_widget_t *content;
        ui_widget_t *trigger_btn;
        ui_widget_t *close_btn;
} dialog_fixture_t;

static void dialog_fixture_setup(dialog_fixture_t *fx)
{
        fx->dialog = ui_create_dialog();
        ui_widget_append(ui_root(), fx->dialog);
        ui_dialog_build_children(fx->dialog);
        fx->portal = ui_widget_get_child(fx->dialog, 1);
        fx->overlay = ui_portal_get_content(fx->portal);
        fx->content = ui_dialog_get_content(fx->dialog);
        fx->trigger_btn =
            ui_widget_get_child(ui_widget_get_child(fx->dialog, 0), 0);
        fx->close_btn = ui_create_dialog_close();
        ui_widget_append(fx->content, fx->close_btn);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();
        ui_dialog_open(fx->dialog);
        ui_update();
}

static void dialog_fixture_teardown(dialog_fixture_t *fx)
{
        ui_widget_remove(fx->dialog);
        ui_clear_trash();
}

static void test_dialog_close_interactions(void)
{
        dialog_fixture_t fx;
        ui_widget_t *footer;
        ui_widget_t *inner;
        ui_event_t outside;

        dialog_fixture_setup(&fx);
        footer = ui_create_dialog_footer();
        inner = ui_create_widget("button");
        ui_widget_append(footer, inner);
        ui_widget_append(fx.content, footer);
        ui_update();

        emit_click(fx.overlay, inner);
        ctest_equal_int("click inside content should stay open",
                        ui_dialog_is_open(fx.dialog), 1);
        emit_click(fx.overlay, fx.overlay);
        ctest_equal_int("overlay click should close",
                        ui_dialog_is_open(fx.dialog), 0);

        ui_dialog_open(fx.dialog);
        ui_update();
        emit_keydown(fx.close_btn, KEY_ESCAPE);
        ctest_equal_int("escape should close", ui_dialog_is_open(fx.dialog), 0);

        ui_dialog_open(fx.dialog);
        ui_update();
        emit_click(fx.close_btn, fx.close_btn);
        ctest_equal_int("close button should close",
                        ui_dialog_is_open(fx.dialog), 0);

        emit_click(fx.trigger_btn, fx.trigger_btn);
        ui_update();
        ctest_equal_int("trigger should reopen after overlay close",
                        ui_dialog_is_open(fx.dialog), 1);
        ctest_equal_int("reopened overlay should stay on portal root",
                        fx.overlay->parent == ui_get_portal_root(), 1);

        ui_event_init(&outside, "portal-outside");
        outside.target = fx.portal;
        ui_widget_emit_event(fx.portal, outside, NULL);
        ctest_equal_int("portal-outside should close",
                        ui_dialog_is_open(fx.dialog), 0);
        dialog_fixture_teardown(&fx);
}

static void test_dialog_close_attr_binding(void)
{
        dialog_fixture_t fx;
        ui_widget_t *plain;

        dialog_fixture_setup(&fx);
        plain = ui_create_widget("button");
        ui_widget_set_attr(plain, "data-dialog-close", "true");
        ui_widget_append(fx.content, plain);
        ui_dialog_build_children(fx.dialog);
        ui_update();
        emit_click(plain, plain);
        ctest_equal_int("data-dialog-close node should close",
                        ui_dialog_is_open(fx.dialog), 0);
        dialog_fixture_teardown(&fx);
}

static void test_dialog_focus(void)
{
        dialog_fixture_t fx;

        dialog_fixture_setup(&fx);
        ctest_equal_int("trigger button should be focusable",
                        fx.trigger_btn->tab_index >= 0, 1);
        ctest_equal_int("close button should be focusable",
                        fx.close_btn->tab_index >= 0, 1);
        ui_dialog_close(fx.dialog);
        ui_update();
        ui_set_focus(fx.trigger_btn);
        ui_dialog_open(fx.dialog);
        ui_update();
        ctest_equal_int(
            "open should focus inside content",
            ui_get_focus() != NULL && has_ancestor(ui_get_focus(), fx.content),
            1);
        ui_dialog_close(fx.dialog);
        ui_update();
        ctest_equal_int("close should restore focus",
                        ui_get_focus() == fx.trigger_btn, 1);
        dialog_fixture_teardown(&fx);
}

static int dialog_open_count;
static int dialog_close_count;

static void on_dialog_open_count(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++dialog_open_count;
}

static void on_dialog_close_count(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++dialog_close_count;
}

static void test_dialog_events(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *portal;
        ui_widget_t *overlay;

        dialog_open_count = 0;
        dialog_close_count = 0;
        ui_widget_on(dialog, "open", on_dialog_open_count, NULL);
        ui_widget_on(dialog, "close", on_dialog_close_count, NULL);
        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        portal = ui_widget_get_child(dialog, 1);
        overlay = ui_portal_get_content(portal);
        ui_update();
        ui_dialog_open(dialog);
        ctest_equal_int("open should emit open event", dialog_open_count, 1);
        ui_dialog_open(dialog);
        ctest_equal_int("repeated open is a no-op", dialog_open_count, 1);
        ui_dialog_close(dialog);
        ctest_equal_int("close should emit close event", dialog_close_count, 1);
        ui_dialog_open(dialog);
        ui_update();
        emit_click(overlay, overlay);
        ctest_equal_int("overlay close also emits close", dialog_close_count,
                        2);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_bind_appended_children(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *trigger = ui_create_dialog_trigger();
        ui_widget_t *overlay = ui_create_dialog_overlay();
        ui_widget_t *content = ui_create_dialog_content();
        ui_widget_t *portal = ui_create_portal();

        ui_widget_append(ui_root(), dialog);
        ui_update();
        ui_widget_append(overlay, content);
        ui_widget_append(portal, overlay);
        ui_widget_append(dialog, trigger);
        ui_widget_append(dialog, portal);
        ui_dialog_build_children(dialog);
        ui_dialog_open(dialog);
        ui_update();
        ctest_equal_int("appended overlay should mount on open",
                        overlay->parent == ui_get_portal_root(), 1);
        ctest_equal_int("appended structure should be open",
                        ui_dialog_is_open(dialog), 1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_destroy_orders(void)
{
        dialog_fixture_t fx;
        size_t children_before;

        children_before = count_children(ui_get_portal_root());
        dialog_fixture_setup(&fx);
        ctest_equal_int(
            "setup should mount one overlay",
            count_children(ui_get_portal_root()) == children_before + 1, 1);
        ui_widget_destroy(fx.dialog);
        ui_update();
        ui_clear_trash();
        ctest_equal_int("owner-first destroy should remove the overlay",
                        count_children(ui_get_portal_root()) == children_before,
                        1);

        dialog_fixture_setup(&fx);
        children_before = count_children(ui_get_portal_root());
        ui_widget_remove(fx.overlay);
        ui_clear_trash();
        ui_update();
        ctest_equal_int("content-first destroy should reset open state",
                        ui_dialog_is_open(fx.dialog), 0);
        ctest_equal_int(
            "content-first destroy should unmount overlay",
            count_children(ui_get_portal_root()) == children_before - 1, 1);
        ui_dialog_open(fx.dialog);
        ui_update();
        ctest_equal_int("reopen without content is a safe no-op",
                        ui_dialog_is_open(fx.dialog), 0);
        ui_dialog_build_children(fx.dialog);
        ui_dialog_open(fx.dialog);
        ui_update();
        ctest_equal_int("build_children should rebuild the structure",
                        ui_dialog_is_open(fx.dialog), 1);
        dialog_fixture_teardown(&fx);
}

static void test_dialog_default_subtree(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content;
        ui_widget_t *child;
        ui_widget_t *header = NULL;
        ui_widget_t *title = NULL;
        ui_widget_t *description = NULL;
        ui_widget_t *footer = NULL;
        ui_widget_t *close = NULL;
        wchar_t text[64];
        size_t i;
        size_t n;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_update();

        content = ui_dialog_get_content(dialog);
        n = count_children(content);
        for (i = 0; i < n; ++i) {
                child = ui_widget_get_child(content, i);
                if (child->type && strcmp(child->type, "dialog-header") == 0) {
                        header = child;
                } else if (child->type &&
                           strcmp(child->type, "dialog-footer") == 0) {
                        footer = child;
                } else if (child->type &&
                           ui_widget_has_class(child, "dialog-close")) {
                        close = child;
                } else if (child->type &&
                           strcmp(child->type, "dialog-title") == 0) {
                        title = child;
                } else if (child->type &&
                           strcmp(child->type, "dialog-description") == 0) {
                        description = child;
                }
        }
        if (header) {
                size_t j;
                for (j = 0; j < count_children(header); ++j) {
                        child = ui_widget_get_child(header, j);
                        if (child->type &&
                            strcmp(child->type, "dialog-title") == 0) {
                                title = child;
                        }
                        if (child->type &&
                            strcmp(child->type, "dialog-description") == 0) {
                                description = child;
                        }
                }
        }
        ctest_equal_int("default header should exist", header != NULL, 1);
        ctest_equal_int("default title should exist", title != NULL, 1);
        ctest_equal_int("default description should exist", description != NULL,
                        1);
        ctest_equal_int("default footer should exist", footer != NULL, 1);
        ctest_equal_int("default close should exist", close != NULL, 1);
        ctest_equal_int("default title text should be set",
                        title != NULL &&
                            ui_text_get_content_w(
                                title, text, sizeof(text) / sizeof(*text)) &&
                            wcslen(text) > 0,
                        1);
        ctest_equal_int("default footer should have two buttons",
                        footer != NULL && count_children(footer) == 2, 1);
        ctest_equal_int("default close should be focusable",
                        close != NULL && close->tab_index >= 0, 1);

        n = count_children(content);
        ui_dialog_build_children(dialog);
        ctest_equal_int("second build should not re-inject nodes",
                        count_children(content) == n, 1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_geometry(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content;
        ui_widget_t *footer = NULL;
        ui_widget_t *child;
        size_t i;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();
        ui_dialog_open(dialog);
        ui_update();

        content = ui_dialog_get_content(dialog);
        ctest_equal_int("content should respect max width",
                        content->border_box.width <= 385.0f, 1);
        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (child->type && strcmp(child->type, "dialog-footer") == 0) {
                        footer = child;
                }
        }
        ctest_equal_int("footer should exist for geometry", footer != NULL, 1);
        ctest_equal_int("footer should be full-bleed via negative margins",
                        footer != NULL &&
                            fabsf(footer->border_box.width -
                                  (content->border_box.width - 2.0f)) < 1.5f &&
                            fabsf(footer->border_box.x - 1.0f) < 1.5f,
                        1);
        if (footer) {
                ui_widget_t *last = NULL;
                size_t j;
                for (j = 0; (child = ui_widget_get_child(footer, j)); ++j) {
                        last = child;
                }
                ctest_equal_int(
                    "last footer button should sit at footer right edge",
                    last != NULL &&
                        fabsf(last->border_box.x + last->border_box.width -
                              (footer->border_box.width - 16.0f)) < 1.0f,
                    1);
        }
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_overlay_isolation(void)
{
        ui_widget_t *d1 = ui_create_dialog();
        ui_widget_t *d2 = ui_create_dialog();
        ui_widget_t *o1;
        ui_widget_t *o2;

        ui_widget_append(ui_root(), d1);
        ui_widget_append(ui_root(), d2);
        ui_dialog_build_children(d1);
        ui_dialog_build_children(d2);
        o1 = ui_portal_get_content(ui_widget_get_child(d1, 1));
        o2 = ui_portal_get_content(ui_widget_get_child(d2, 1));
        ui_update();

        ctest_equal_int(
            "closed overlay should default to hidden",
            o1->computed_style.type_bits.display == CSS_DISPLAY_NONE &&
                o2->computed_style.type_bits.display == CSS_DISPLAY_NONE,
            1);

        ui_dialog_open(d1);
        ui_update();
        ctest_equal_int(
            "open should show only its own overlay",
            o1->computed_style.type_bits.display != CSS_DISPLAY_NONE &&
                o2->computed_style.type_bits.display == CSS_DISPLAY_NONE,
            1);

        ui_dialog_open(d2);
        ui_update();
        ctest_equal_int(
            "both open should show both overlays",
            o1->computed_style.type_bits.display != CSS_DISPLAY_NONE &&
                o2->computed_style.type_bits.display != CSS_DISPLAY_NONE,
            1);

        ui_dialog_close(d1);
        ui_update();
        ctest_equal_int(
            "closing one keeps the other visible",
            o1->computed_style.type_bits.display == CSS_DISPLAY_NONE &&
                o2->computed_style.type_bits.display != CSS_DISPLAY_NONE,
            1);

        ui_widget_remove(d1);
        ui_widget_remove(d2);
        ui_clear_trash();
}

static void test_dialog_loose_scans_require_keys(void)
{
        ui_widget_t *trigger = ui_create_dialog_trigger();
        ui_widget_t *overlay_a = ui_create_dialog_overlay();
        ui_widget_t *overlay_b = ui_create_dialog_overlay();
        ui_widget_t *content_a = ui_create_dialog_content();
        ui_widget_t *content_b = ui_create_dialog_content();
        ui_widget_t *button = ui_create_widget("button");

        /* A stray typed trigger with no association key must not be
         * hijacked by loose controllers resolved from other keys. */
        ui_widget_set_attr(overlay_a, "data-dialog-target", "alpha");
        ui_widget_set_attr(overlay_b, "data-dialog-target", "beta");
        ui_widget_append(trigger, button);
        ui_widget_append(overlay_a, content_a);
        ui_widget_append(overlay_b, content_b);
        ui_widget_append(ui_root(), trigger);
        ui_widget_append(ui_root(), overlay_a);
        ui_widget_append(ui_root(), overlay_b);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();

        emit_click(button, button);
        ui_update();
        ctest_equal_int(
            "keyless stray trigger should not show overlays",
            overlay_a->computed_style.type_bits.display == CSS_DISPLAY_NONE &&
                overlay_b->computed_style.type_bits.display == CSS_DISPLAY_NONE,
            1);

        ui_widget_remove(trigger);
        ui_widget_remove(overlay_a);
        ui_widget_remove(overlay_b);
        ui_clear_trash();
}

static void test_dialog_content_destroy_then_rebuild(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content;
        ui_widget_t *portal;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        portal = ui_widget_get_child(dialog, 1);
        ui_dialog_open(dialog);
        ui_update();
        content = ui_dialog_get_content(dialog);
        ui_widget_remove(content);
        ui_clear_trash();
        ui_update();
        ctest_equal_int("destroyed content should clear the getter",
                        ui_dialog_get_content(dialog) == NULL, 1);
        ui_dialog_open(dialog);
        ui_update();
        ctest_equal_int("open after content loss is a safe no-op",
                        ui_dialog_is_open(dialog), 0);
        ui_dialog_build_children(dialog);
        ui_dialog_open(dialog);
        ui_update();
        ctest_equal_int("rebuild should restore the dialog",
                        ui_dialog_is_open(dialog) &&
                            ui_dialog_get_content(dialog) != NULL &&
                            ui_portal_get_content(portal) != NULL,
                        1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_deferred_typed_close(void)
{
        dialog_fixture_t fx;
        ui_widget_t *close2;

        dialog_fixture_setup(&fx);
        close2 = ui_create_dialog_close();
        ui_widget_append(fx.content, close2);
        ui_update();
        emit_click(close2, close2);
        ui_update();
        ctest_equal_int("deferred typed close should bind via update rescan",
                        ui_dialog_is_open(fx.dialog), 0);
        dialog_fixture_teardown(&fx);
}

static void test_dialog_loose_registry_exit(void)
{
        ui_widget_t *trigger = ui_create_widget("button");
        ui_widget_t *overlay;
        ui_widget_t *content;

        /* Regression: the loose controller is released with the widgets
         * module at lcui_destroy(), which runs before the widget tree is
         * destroyed.  Listeners on the still-attached overlay/portal must
         * not outlive the controller data.  Leave every widget attached;
         * the exit ordering itself is the test. */
        ui_widget_set_attr(trigger, "data-dialog-trigger", "exitcheck");
        overlay = ui_create_dialog_overlay();
        ui_widget_set_attr(overlay, "data-dialog-target", "exitcheck");
        content = ui_create_dialog_content();
        ui_widget_append(overlay, content);
        ui_widget_append(ui_root(), trigger);
        ui_widget_append(ui_root(), overlay);
        ui_update();
        emit_click(trigger, trigger);
        ui_update();
        ctest_equal_int("loose dialog should be open before exit",
                        overlay->parent == ui_get_portal_root(), 1);
}

static void test_dialog_show_close_button(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content;
        ui_widget_t *child;
        int has_close = 0;
        size_t i;

        ui_dialog_show_close_button(dialog, false);
        ctest_equal_int("show close button should be readable as off",
                        ui_dialog_shows_close_button(dialog), 0);
        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_update();
        content = ui_dialog_get_content(dialog);
        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (child->type && ui_widget_has_class(child, "dialog-close")) {
                        has_close = 1;
                }
        }
        ctest_equal_int("content should have no close button", has_close, 0);
        ui_dialog_show_close_button(dialog, true);
        ui_dialog_build_children(dialog);
        has_close = 0;
        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (child->type && ui_widget_has_class(child, "dialog-close")) {
                        has_close = 1;
                }
        }
        ctest_equal_int("re-enabled close button should be injected", has_close,
                        1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void text_of(ui_widget_t *w, wchar_t *buf, size_t n)
{
        ui_text_get_content_w(w, buf, n);
}

static void test_dialog_slot_setters(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content, *header, *footer, *trigger, *button;
        wchar_t buf[64];

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_dialog_set_trigger_text(dialog, "Open It");
        ui_dialog_set_title(dialog, "Slot Title");
        ui_dialog_set_description(dialog, "Slot Description");
        ui_dialog_set_cancel_text(dialog, "Nope");
        ui_dialog_set_action_text(dialog, "Yep");
        ui_update();

        content = ui_dialog_get_content(dialog);
        header = ui_widget_get_child(content, 0);
        text_of(ui_widget_get_child(header, 0), buf, 64);
        ctest_equal_int("set_title should write the title slot",
                        wcscmp(buf, L"Slot Title") == 0, 1);
        text_of(ui_widget_get_child(header, 1), buf, 64);
        ctest_equal_int("set_description should write the description slot",
                        wcscmp(buf, L"Slot Description") == 0, 1);
        footer = ui_widget_get_child(content, 1);
        text_of(ui_widget_get_child(footer, 0), buf, 64);
        ctest_equal_int("set_cancel_text should write the first footer button",
                        wcscmp(buf, L"Nope") == 0, 1);
        text_of(ui_widget_get_child(footer, 1), buf, 64);
        ctest_equal_int("set_action_text should write the last footer button",
                        wcscmp(buf, L"Yep") == 0, 1);
        trigger = ui_widget_get_child(dialog, 0);
        button = ui_widget_get_child(trigger, 0);
        text_of(button, buf, 64);
        ctest_equal_int("set_trigger_text should write the trigger button",
                        wcscmp(buf, L"Open It") == 0, 1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static void test_dialog_close_is_ghost_button(void)
{
        ui_widget_t *close = ui_create_dialog_close();

        ui_widget_append(ui_root(), close);
        ctest_equal_int("dialog-close should carry the ghost variant status",
                        ui_widget_has_status(close, "ghost"), 1);
        ui_widget_remove(close);
        ui_clear_trash();
}

static void test_dialog_append_body(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *content, *child, *body, *footer;
        size_t header_i = 0, body_i = 0, footer_i = 0;
        size_t i;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        body = ui_create_widget(NULL);
        ui_widget_append(body, ui_create_widget("text"));
        ui_dialog_append_body(dialog, body);
        ui_update();
        content = ui_dialog_get_content(dialog);
        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (child->type && strcmp(child->type, "dialog-header") == 0) {
                        header_i = i;
                }
                if (child == body) {
                        body_i = i;
                }
                if (child->type && strcmp(child->type, "dialog-footer") == 0) {
                        footer_i = i;
                        footer = child;
                }
        }
        ctest_equal_int("body should insert between header and footer",
                        header_i < body_i && body_i < footer_i, 1);
        ctest_equal_int("footer button getter should return action button",
                        ui_dialog_get_footer_button(dialog, true) ==
                            ui_widget_get_child(footer, 1),
                        1);
        ctest_equal_int("cancel button getter should return first button",
                        ui_dialog_get_footer_button(dialog, false) ==
                            ui_widget_get_child(footer, 0),
                        1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

static ui_widget_t *find_close_child(ui_widget_t *content)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                if (ui_widget_has_class(child, "dialog-close")) {
                        return child;
                }
        }
        return NULL;
}

static void test_dialog_close_active_stays_absolute(void)
{
        ui_widget_t *dialog = ui_create_dialog();
        ui_widget_t *close;

        ui_widget_append(ui_root(), dialog);
        ui_dialog_build_children(dialog);
        ui_update();
        close = find_close_child(ui_dialog_get_content(dialog));
        ctest_equal_int("close node should exist", close != NULL, 1);
        ctest_equal_int(
            "close should be absolutely positioned",
            close->computed_style.type_bits.position == CSS_POSITION_ABSOLUTE,
            1);
        ui_widget_add_status(close, "active");
        ui_widget_request_refresh_style(close);
        ui_update();
        ctest_equal_int(
            "close should stay absolute while active",
            close->computed_style.type_bits.position == CSS_POSITION_ABSOLUTE,
            1);
        ui_widget_remove(dialog);
        ui_clear_trash();
}

void test_dialog(void)
{
        lcui_init();
        ctest_describe("dialog structure", test_dialog_structure);
        ctest_describe("dialog open close", test_dialog_open_close);
        ctest_describe("dialog loose association",
                       test_dialog_loose_association);
        ctest_describe("dialog close interactions",
                       test_dialog_close_interactions);
        ctest_describe("dialog close attr binding",
                       test_dialog_close_attr_binding);
        ctest_describe("dialog focus", test_dialog_focus);
        ctest_describe("dialog default subtree", test_dialog_default_subtree);
        ctest_describe("dialog show close button",
                       test_dialog_show_close_button);
        ctest_describe("dialog slot setters", test_dialog_slot_setters);
        ctest_describe("dialog append body", test_dialog_append_body);
        ctest_describe("dialog close ghost variant",
                       test_dialog_close_is_ghost_button);
        ctest_describe("dialog close active stays absolute",
                       test_dialog_close_active_stays_absolute);
        ctest_describe("dialog visual geometry", test_dialog_geometry);
        ctest_describe("dialog overlay isolation",
                       test_dialog_overlay_isolation);
        ctest_describe("dialog events", test_dialog_events);
        ctest_describe("dialog deferred structure",
                       test_dialog_bind_appended_children);
        ctest_describe("dialog destroy orders", test_dialog_destroy_orders);
        ctest_describe("dialog loose scans require keys",
                       test_dialog_loose_scans_require_keys);
        ctest_describe("dialog content destroy rebuild",
                       test_dialog_content_destroy_then_rebuild);
        ctest_describe("dialog deferred typed close",
                       test_dialog_deferred_typed_close);
        ctest_describe("dialog loose registry exit",
                       test_dialog_loose_registry_exit);
        lcui_destroy();
}
