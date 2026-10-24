/*
 * tests/integration/test_attachment.c: -- Attachment widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <wchar.h>
#include <LCUI.h>
#include <ctest-custom.h>

static ui_widget_t *find_by_class(ui_widget_t *parent, const char *class_name)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(parent, index)); ++index) {
                if (ui_widget_has_class(child, class_name)) {
                        return child;
                }
        }
        return NULL;
}

static void test_parts(void)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *media = ui_create_attachment_media();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *description = ui_create_attachment_description();
        ui_widget_t *actions = ui_create_attachment_actions();
        ui_widget_t *action = ui_create_attachment_action();
        ui_widget_t *trigger = ui_create_attachment_trigger();
        ui_widget_t *group = ui_create_attachment_group();

        ui_widget_append(attachment, media);
        ui_widget_append(attachment, content);
        ui_widget_append(content, title);
        ui_widget_append(content, description);
        ui_widget_append(attachment, actions);
        ui_widget_append(actions, action);
        ui_widget_append(attachment, trigger);

        ctest_equal_bool(
            "should accept media",
            ui_check_widget_type(ui_widget_get_child(attachment, 0),
                                 "attachment-media"),
            true);
        ctest_equal_bool(
            "should accept content",
            ui_check_widget_type(ui_widget_get_child(attachment, 1),
                                 "attachment-content"),
            true);
        ctest_equal_bool("should accept title",
                         ui_check_widget_type(ui_widget_get_child(content, 0),
                                              "attachment-title"),
                         true);
        ctest_equal_bool("should accept description",
                         ui_check_widget_type(ui_widget_get_child(content, 1),
                                              "attachment-description"),
                         true);
        ctest_equal_bool(
            "should accept actions",
            ui_check_widget_type(ui_widget_get_child(attachment, 2),
                                 "attachment-actions"),
            true);
        ctest_equal_bool("should create an action button",
                         ui_check_widget_type(action, "button"), true);
        ctest_equal_bool(
            "should accept a trigger",
            ui_check_widget_type(ui_widget_get_child(attachment, 3),
                                 "attachment-trigger"),
            true);
        ctest_equal_bool("should mark an attachment with a trigger",
                         ui_widget_has_class(attachment, "has-trigger"), true);
        ctest_equal_bool("should create a group",
                         ui_check_widget_type(group, "attachment-group"), true);

        ui_widget_destroy(attachment);
        ui_widget_destroy(group);
}

static void test_state(void)
{
        ui_widget_t *attachment = ui_create_attachment();

        ctest_equal_int("should default to the done state",
                        ui_attachment_get_state(attachment),
                        UI_ATTACHMENT_STATE_DONE);

        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_IDLE);
        ctest_equal_bool("should add the idle status",
                         ui_widget_has_status(attachment, "idle"), true);
        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_UPLOADING);
        ctest_equal_bool("should remove the previous state status",
                         ui_widget_has_status(attachment, "idle"), false);
        ctest_equal_bool("should add the uploading status",
                         ui_widget_has_status(attachment, "uploading"), true);
        ctest_equal_int("should store the uploading state",
                        ui_attachment_get_state(attachment),
                        UI_ATTACHMENT_STATE_UPLOADING);

        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_PROCESSING);
        ctest_equal_bool("should add the processing status",
                         ui_widget_has_status(attachment, "processing"), true);
        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_ERROR);
        ctest_equal_bool("should add the error status",
                         ui_widget_has_status(attachment, "error"), true);
        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_DONE);
        ctest_equal_bool("should clear the error status",
                         ui_widget_has_status(attachment, "error"), false);
        ctest_equal_bool("should add the done status",
                         ui_widget_has_status(attachment, "done"), true);

        ui_widget_destroy(attachment);
}

static void test_size_and_orientation(void)
{
        ui_widget_t *attachment = ui_create_attachment();

        ui_attachment_set_size(attachment, UI_ATTACHMENT_SIZE_SM);
        ctest_equal_bool("should add the sm size status",
                         ui_widget_has_status(attachment, "sm"), true);
        ui_attachment_set_size(attachment, UI_ATTACHMENT_SIZE_XS);
        ctest_equal_bool("should replace the previous size status",
                         ui_widget_has_status(attachment, "sm"), false);
        ctest_equal_bool("should add the xs size status",
                         ui_widget_has_status(attachment, "xs"), true);
        ctest_equal_int("should store the xs size",
                        ui_attachment_get_size(attachment),
                        UI_ATTACHMENT_SIZE_XS);
        ui_attachment_set_size(attachment, UI_ATTACHMENT_SIZE_DEFAULT);
        ctest_equal_bool("should clear size status for default",
                         ui_widget_has_status(attachment, "xs"), false);

        ui_attachment_set_orientation(attachment,
                                      UI_ATTACHMENT_ORIENTATION_VERTICAL);
        ctest_equal_bool("should add the vertical orientation status",
                         ui_widget_has_status(attachment, "vertical"), true);
        ctest_equal_int("should store the vertical orientation",
                        ui_attachment_get_orientation(attachment),
                        UI_ATTACHMENT_ORIENTATION_VERTICAL);
        ui_attachment_set_orientation(attachment,
                                      UI_ATTACHMENT_ORIENTATION_HORIZONTAL);
        ctest_equal_bool("should clear the vertical orientation status",
                         ui_widget_has_status(attachment, "vertical"), false);

        ui_widget_destroy(attachment);
}

static void test_media(void)
{
        ui_widget_t *media = ui_create_attachment_media();

        ctest_equal_bool("should default to the icon media variant",
                         ui_widget_has_status(media, "icon"), true);
        ui_attachment_media_set_variant(media,
                                        UI_ATTACHMENT_MEDIA_VARIANT_IMAGE);
        ctest_equal_bool("should replace the icon media status",
                         ui_widget_has_status(media, "icon"), false);
        ctest_equal_bool("should add the image media status",
                         ui_widget_has_status(media, "image"), true);
        ctest_equal_int("should store the image media variant",
                        ui_attachment_media_get_variant(media),
                        UI_ATTACHMENT_MEDIA_VARIANT_IMAGE);

        ui_widget_destroy(media);
}

static void test_loading_shimmer(void)
{
        ui_widget_t *attachment = ui_create_attachment();
        ui_widget_t *content = ui_create_attachment_content();
        ui_widget_t *title = ui_create_attachment_title();
        ui_widget_t *loading;

        ui_text_set_content(title, "sales-dashboard.pdf");
        ui_widget_append(content, title);
        ui_widget_append(attachment, content);
        ui_widget_append(ui_root(), attachment);
        ui_update();

        loading = find_by_class(content, "attachment-loading");
        ctest_equal_bool("should not create a loading shimmer for done",
                         loading == NULL, true);

        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_UPLOADING);
        ui_update();
        loading = find_by_class(content, "attachment-loading");
        ctest_equal_bool("should create a loading shimmer while uploading",
                         loading != NULL, true);
        if (loading) {
                ctest_equal_bool("loading shimmer should be a shimmer",
                                 ui_check_widget_type(loading, "shimmer"),
                                 true);
                ctest_equal_bool("loading shimmer should be visible",
                                 ui_widget_is_visible(loading), true);
                ui_widget_t *shimmer_content = ui_widget_get_child(loading, 0);
                ui_widget_t *loading_text =
                    shimmer_content ? ui_widget_get_child(shimmer_content, 0)
                                    : NULL;
                ctest_equal_bool(
                    "loading shimmer should carry a text copy",
                    loading_text && ui_check_widget_type(loading_text, "text"),
                    true);
                if (loading_text) {
                        wchar_t buffer[64];
                        ctest_equal_int("loading text should match the title",
                                        (int)ui_text_get_content_w(loading_text,
                                                                   buffer, 64),
                                        (int)wcslen(L"sales-dashboard.pdf"));
                }
        }
        ctest_equal_bool(
            "uploading title should be hidden",
            title->computed_style.type_bits.visibility == CSS_VISIBILITY_HIDDEN,
            true);

        ui_attachment_set_state(attachment, UI_ATTACHMENT_STATE_DONE);
        ui_update();
        if (loading) {
                ctest_equal_bool("loading shimmer should hide when done",
                                 ui_widget_is_visible(loading), false);
        }
        ctest_equal_bool("done title should be visible",
                         title->computed_style.type_bits.visibility ==
                             CSS_VISIBILITY_VISIBLE,
                         true);

        ui_widget_destroy(attachment);
}

static void test_composition_helpers(void)
{
        ui_widget_t *attachment = ui_create_attachment();

        ui_widget_append(ui_root(), attachment);
        ui_update();
        ui_attachment_set_title(attachment, "report.pdf");
        ui_attachment_set_description(attachment, "PDF - 2.4 MB");
        ui_update();

        ctest_equal_bool(
            "should create content on demand",
            ui_check_widget_type(ui_widget_get_child(attachment, 0),
                                 "attachment-content"),
            true);

        ui_widget_destroy(attachment);
}

void test_attachment(void)
{
        lcui_init();
        ctest_describe("attachment parts", test_parts);
        ctest_describe("attachment state", test_state);
        ctest_describe("attachment size and orientation",
                       test_size_and_orientation);
        ctest_describe("attachment media", test_media);
        ctest_describe("attachment loading shimmer", test_loading_shimmer);
        ctest_describe("attachment composition helpers",
                       test_composition_helpers);
        lcui_destroy();
}
