/*
 * tests/integration/test_alert_dialog.c
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
#include <string.h>

static void ad_emit_click(ui_widget_t *emitter, ui_widget_t *target)
{
        ui_event_t event;

        ui_event_init(&event, "click");
        event.target = target;
        ui_widget_emit_event(emitter, event, NULL);
}

static size_t ad_count_children(ui_widget_t *w)
{
        size_t n = 0;

        while (ui_widget_get_child(w, n)) {
                ++n;
        }
        return n;
}

typedef struct ad_fixture {
        ui_widget_t *dialog;
        ui_widget_t *portal;
        ui_widget_t *overlay;
        ui_widget_t *content;
        ui_widget_t *cancel;
        ui_widget_t *action;
} ad_fixture_t;

static void ad_fixture_setup(ad_fixture_t *fx)
{
        fx->dialog = ui_create_alert_dialog();
        ui_widget_append(ui_root(), fx->dialog);
        ui_dialog_build_children(fx->dialog);
        fx->portal = ui_widget_get_child(fx->dialog, 1);
        fx->overlay = ui_portal_get_content(fx->portal);
        fx->content = ui_dialog_get_content(fx->dialog);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();
        ui_dialog_open(fx->dialog);
        ui_update();
        fx->cancel = NULL;
        fx->action = NULL;
        {
                ui_widget_t *footer = NULL;
                ui_widget_t *child;
                size_t i;
                for (i = 0; (child = ui_widget_get_child(fx->content, i));
                     ++i) {
                        if (child->type &&
                            strcmp(child->type, "alert-dialog-footer") == 0) {
                                footer = child;
                        }
                }
                if (footer) {
                        for (i = 0; (child = ui_widget_get_child(footer, i));
                             ++i) {
                                if (child->type &&
                                    strcmp(child->type,
                                           "alert-dialog-cancel") == 0) {
                                        fx->cancel = child;
                                }
                                if (child->type &&
                                    strcmp(child->type,
                                           "alert-dialog-action") == 0) {
                                        fx->action = child;
                                }
                        }
                }
        }
}

static void ad_fixture_teardown(ad_fixture_t *fx)
{
        ui_widget_remove(fx->dialog);
        ui_clear_trash();
}

static void test_alert_dialog_structure(void)
{
        ad_fixture_t fx;
        ui_widget_t *child;
        size_t i;
        int has_close = 0;

        ad_fixture_setup(&fx);
        ctest_equal_int("overlay should be mounted on portal root",
                        fx.overlay->parent == ui_get_portal_root(), 1);
        ctest_equal_int("alert dialog should be open",
                        ui_dialog_is_open(fx.dialog), 1);
        for (i = 0; (child = ui_widget_get_child(fx.content, i)); ++i) {
                if (child->type && strcmp(child->type, "dialog-close") == 0) {
                        has_close = 1;
                }
        }
        ctest_equal_int("alert dialog should not inject close", has_close, 0);
        ctest_equal_int("default cancel button should exist", fx.cancel != NULL,
                        1);
        ctest_equal_int("default action button should exist", fx.action != NULL,
                        1);
        ctest_equal_int(
            "overlay should use the alert-dialog type",
            fx.overlay->type != NULL &&
                strcmp(fx.overlay->type, "alert-dialog-overlay") == 0,
            1);
        ctest_equal_int(
            "content should use the alert-dialog type",
            fx.content->type != NULL &&
                strcmp(fx.content->type, "alert-dialog-content") == 0,
            1);
        ui_alert_dialog_content_set_size(fx.content, "sm");
        ctest_equal_int("size should apply on the default subtree",
                        ui_widget_has_status(fx.content, "sm"), 1);
        ad_fixture_teardown(&fx);
}

static void test_alert_dialog_not_dismissable(void)
{
        ad_fixture_t fx;
        ui_event_t event;

        ad_fixture_setup(&fx);

        ui_event_init(&event, "keydown");
        event.target = fx.action;
        event.key.code = KEY_ESCAPE;
        ui_widget_emit_event(fx.action, event, NULL);
        ctest_equal_int("escape should not close alert dialog",
                        ui_dialog_is_open(fx.dialog), 1);

        ad_emit_click(fx.overlay, fx.overlay);
        ctest_equal_int("overlay click should not close alert dialog",
                        ui_dialog_is_open(fx.dialog), 1);

        ui_event_init(&event, "portal-outside");
        event.target = fx.portal;
        ui_widget_emit_event(fx.portal, event, NULL);
        ctest_equal_int("portal-outside should not close alert dialog",
                        ui_dialog_is_open(fx.dialog), 1);

        ui_dialog_close(fx.dialog);
        ctest_equal_int("explicit close should close alert dialog",
                        ui_dialog_is_open(fx.dialog), 0);
        ad_fixture_teardown(&fx);
}

static int ad_action_count;
static int ad_cancel_count;
static int ad_close_count;
static char ad_last_event[32];

static void on_ad_action(ui_widget_t *w, ui_event_t *e, void *arg)
{
        strcpy(ad_last_event, "action");
        ++ad_action_count;
}

static void on_ad_cancel(ui_widget_t *w, ui_event_t *e, void *arg)
{
        strcpy(ad_last_event, "cancel");
        ++ad_cancel_count;
}

static void on_ad_close(ui_widget_t *w, ui_event_t *e, void *arg)
{
        strcpy(ad_last_event, "close");
        ++ad_close_count;
}

static void test_alert_dialog_cancel_action_events(void)
{
        ad_fixture_t fx;

        ad_fixture_setup(&fx);
        ad_action_count = 0;
        ad_cancel_count = 0;
        ad_close_count = 0;
        ad_last_event[0] = '\0';
        ui_widget_on(fx.dialog, "action", on_ad_action, NULL);
        ui_widget_on(fx.dialog, "cancel", on_ad_cancel, NULL);
        ui_widget_on(fx.dialog, "close", on_ad_close, NULL);

        ad_emit_click(fx.cancel, fx.cancel);
        ctest_equal_int("cancel click should close",
                        ui_dialog_is_open(fx.dialog), 0);
        ctest_equal_int("cancel should emit close once", ad_close_count, 1);
        ctest_equal_int("cancel should emit cancel once", ad_cancel_count, 1);
        ctest_equal_str("cancel should emit after close", ad_last_event,
                        "cancel");

        ad_fixture_teardown(&fx);

        ad_fixture_setup(&fx);
        ad_action_count = 0;
        ad_cancel_count = 0;
        ad_close_count = 0;
        ad_last_event[0] = '\0';
        ui_widget_on(fx.dialog, "action", on_ad_action, NULL);
        ui_widget_on(fx.dialog, "cancel", on_ad_cancel, NULL);
        ui_widget_on(fx.dialog, "close", on_ad_close, NULL);
        ad_emit_click(fx.action, fx.action);
        ctest_equal_int("action click should close",
                        ui_dialog_is_open(fx.dialog), 0);
        ctest_equal_int("action should emit close once", ad_close_count, 1);
        ctest_equal_int("action should emit action once", ad_action_count, 1);
        ctest_equal_str("action should emit after close", ad_last_event,
                        "action");
        ad_fixture_teardown(&fx);
}

static void test_alert_dialog_loose_actions(void)
{
        ui_widget_t *trigger = ui_create_widget("button");
        ui_widget_t *overlay;
        ui_widget_t *content;
        ui_widget_t *cancel;
        ui_widget_t *action;

        ui_widget_set_attr(trigger, "data-dialog-trigger", "delete");
        overlay = ui_create_alert_dialog_overlay();
        ui_widget_set_attr(overlay, "data-dialog-target", "delete");
        content = ui_create_alert_dialog_content();
        cancel = ui_create_widget("button");
        ui_widget_set_attr(cancel, "data-dialog-cancel", "delete");
        action = ui_create_widget("button");
        ui_widget_set_attr(action, "data-dialog-action", "delete");
        ui_widget_append(content, cancel);
        ui_widget_append(content, action);
        ui_widget_append(overlay, content);
        ui_widget_append(ui_root(), trigger);
        ui_widget_append(ui_root(), overlay);
        ui_widget_resize(ui_root(), 512, 400);
        ui_update();

        ad_action_count = 0;
        ad_cancel_count = 0;
        ad_close_count = 0;
        ui_widget_on(ui_root(), "action", on_ad_action, NULL);
        ui_widget_on(ui_root(), "cancel", on_ad_cancel, NULL);
        ui_widget_on(ui_root(), "close", on_ad_close, NULL);
        ad_emit_click(trigger, trigger);
        ui_update();
        ctest_equal_int("loose alert overlay should mount",
                        overlay->parent == ui_get_portal_root(), 1);
        ctest_equal_int("loose alert overlay should show",
                        overlay->border_box.width > 0, 1);
        ad_emit_click(action, action);
        ui_update();
        ctest_equal_int("loose action should emit once", ad_action_count, 1);
        ctest_equal_int(
            "loose action should close overlay",
            overlay->computed_style.type_bits.display == CSS_DISPLAY_NONE, 1);
        ad_emit_click(trigger, trigger);
        ui_update();
        ad_emit_click(cancel, cancel);
        ctest_equal_int("loose cancel should emit once", ad_cancel_count, 1);
        ui_widget_off(ui_root(), "action", on_ad_action, NULL);
        ui_widget_off(ui_root(), "cancel", on_ad_cancel, NULL);
        ui_widget_off(ui_root(), "close", on_ad_close, NULL);

        ui_widget_remove(trigger);
        ui_widget_remove(overlay);
        ui_clear_trash();
}

static void test_alert_dialog_size(void)
{
        ui_widget_t *content = ui_create_alert_dialog_content();

        ui_widget_append(ui_root(), content);
        ui_alert_dialog_content_set_size(content, "sm");
        ctest_equal_int("sm size should add status",
                        ui_widget_has_status(content, "sm"), 1);
        ui_widget_set_attr(content, "size", "default");
        ctest_equal_int("default size should clear status",
                        !ui_widget_has_status(content, "sm"), 1);
        ui_widget_set_attr(content, "size", "sm");
        ctest_equal_int("attr path should add status",
                        ui_widget_has_status(content, "sm"), 1);
        ui_widget_remove(content);
        ui_clear_trash();
}

static void test_alert_dialog_convenience_api(void)
{
        ui_widget_t *alert = ui_create_alert_dialog();
        ui_widget_t *content, *header;
        ui_widget_t *media;

        ui_widget_append(ui_root(), alert);
        ui_dialog_build_children(alert);
        ui_dialog_set_trigger_text(alert, "Confirm");
        ui_dialog_set_title(alert, "Delete item?");
        ui_dialog_set_description(alert, "This cannot be undone.");
        ui_dialog_set_action_text(alert, "Delete");
        {
                ui_widget_t *footer =
                    ui_widget_get_child(ui_dialog_get_content(alert), 1);
                ui_widget_set_attr(ui_widget_get_child(footer, 1), "variant",
                                   "destructive");
        }
        media = ui_create_alert_dialog_media();
        ui_alert_dialog_set_media(alert, media);
        ui_alert_dialog_set_size(alert, "sm");
        ui_update();
        content = ui_dialog_get_content(alert);
        ctest_equal_int("alert size should map to status",
                        ui_widget_has_status(content, "sm"), 1);
        header = ui_widget_get_child(content, 0);
        ctest_equal_int("media should be prepended to the header",
                        ui_widget_get_child(header, 0) == media, 1);
        ctest_equal_int("title stays after prepended media",
                        ui_widget_get_child(header, 1) != NULL, 1);
        ui_widget_remove(alert);
        ui_clear_trash();
}

static void test_alert_dialog_header_alignment(void)
{
        ui_widget_t *alert = ui_create_alert_dialog();
        ui_widget_t *content, *header;

        ui_widget_append(ui_root(), alert);
        ui_dialog_build_children(alert);
        ui_update();
        content = ui_dialog_get_content(alert);
        header = ui_widget_get_child(content, 0);
        ctest_equal_int("default header should left-align items",
                        header->computed_style.type_bits.align_items !=
                            CSS_ALIGN_ITEMS_CENTER,
                        1);
        ctest_equal_int("default header text should be left aligned",
                        header->computed_style.type_bits.text_align !=
                            CSS_TEXT_ALIGN_CENTER,
                        1);
        ui_alert_dialog_set_size(alert, "sm");
        ui_widget_request_refresh_style(header);
        ui_update();
        ctest_equal_int("sm header should center items",
                        header->computed_style.type_bits.align_items ==
                            CSS_ALIGN_ITEMS_CENTER,
                        1);
        ctest_equal_int("sm header text should be centered",
                        header->computed_style.type_bits.text_align ==
                            CSS_TEXT_ALIGN_CENTER,
                        1);
        ui_widget_remove(alert);
        ui_clear_trash();
}
void test_alert_dialog(void)
{
        lcui_init();
        ctest_describe("alert-dialog structure", test_alert_dialog_structure);
        ctest_describe("alert-dialog dismissal",
                       test_alert_dialog_not_dismissable);
        ctest_describe("alert-dialog confirm flow",
                       test_alert_dialog_cancel_action_events);
        ctest_describe("alert-dialog loose actions",
                       test_alert_dialog_loose_actions);
        ctest_describe("alert-dialog size", test_alert_dialog_size);
        ctest_describe("alert-dialog header alignment",
                       test_alert_dialog_header_alignment);
        ctest_describe("alert-dialog convenience api",
                       test_alert_dialog_convenience_api);
        lcui_destroy();
}
