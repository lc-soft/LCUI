/*
 * src/widgets/attachment.c: -- Attachment widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <stdlib.h>
#include <LCUI/widgets/attachment.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/dialog.h>
#include <LCUI/widgets/shimmer.h>
#include <LCUI/widgets/text.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>
#include <ui/style.h>

typedef struct ui_attachment {
        ui_widget_t *title;
        ui_widget_t *loading;
        bool loading_visible;
} ui_attachment_t;

static ui_widget_prototype_t *ui_attachment_prototype;
static ui_widget_prototype_t *ui_attachment_media_prototype;
static ui_widget_prototype_t *ui_attachment_content_prototype;
static ui_widget_prototype_t *ui_attachment_title_prototype;
static ui_widget_prototype_t *ui_attachment_description_prototype;
static ui_widget_prototype_t *ui_attachment_actions_prototype;
static ui_widget_prototype_t *ui_attachment_trigger_prototype;
static ui_widget_prototype_t *ui_attachment_group_prototype;

static const char *ui_attachment_state_names[] = {
        "idle", "uploading", "processing", "error", "done",
};

static const char *ui_attachment_size_names[] = {
        "default",
        "sm",
        "xs",
};

static const char *ui_attachment_orientation_names[] = {
        "horizontal",
        "vertical",
};

static const char *ui_attachment_media_variant_names[] = {
        "icon",
        "image",
};

static const char *ui_attachment_css = "\
attachment {\
  display: flex;\
  align-items: center;\
  position: relative;\
  gap: 8px;\
  box-sizing: border-box;\
  min-width: 0;\
  max-width: 100%;\
  flex-shrink: 0;\
  padding: 8px;\
  border-width: 1px;\
  border-style: solid;\
  border-color: transparent;\
}\
attachment:sm {\
  gap: 10px;\
  padding: 6px;\
}\
attachment:xs {\
  gap: 6px;\
  padding: 4px;\
}\
attachment:vertical {\
  flex-direction: column;\
  align-items: stretch;\
  width: 120px;\
  gap: 8px;\
}\
attachment-media {\
  display: flex;\
  align-items: center;\
  justify-content: center;\
  flex-shrink: 0;\
  width: 40px;\
  height: 40px;\
  border-radius: 8px;\
}\
attachment:sm attachment-media {\
  width: 32px;\
  height: 32px;\
}\
attachment:xs attachment-media {\
  width: 28px;\
  height: 28px;\
  border-radius: 6px;\
}\
attachment:vertical attachment-media {\
  width: 100%;\
  height: 104px;\
  border-radius: 12px;\
}\
attachment-media:icon {\
  font-size: 16px;\
}\
attachment-media:image {\
  background-size: cover;\
  background-position: center;\
  background-repeat: no-repeat;\
}\
attachment-content {\
  display: flex;\
  position: relative;\
  flex: 1;\
  flex-direction: column;\
  min-width: 0;\
}\
attachment:vertical attachment-content {\
  padding: 0 4px;\
}\
attachment:vertical attachment-title {\
  font-size: 11px;\
  line-height: 18px;\
}\
attachment:vertical attachment-description {\
  font-size: 10px;\
  line-height: 16px;\
}\
attachment-title {\
  display: block;\
  font-size: 14px;\
  font-weight: 500;\
  line-height: 20px;\
  white-space: nowrap;\
}\
attachment:sm attachment-title {\
  font-size: 12px;\
  line-height: 18px;\
}\
attachment:xs attachment-title {\
  font-size: 12px;\
  line-height: 18px;\
}\
attachment-description {\
  display: block;\
  margin-top: 2px;\
  font-size: 12px;\
  line-height: 18px;\
  white-space: nowrap;\
}\
attachment-actions {\
  display: flex;\
  position: relative;\
  align-items: center;\
  flex-shrink: 0;\
  gap: 4px;\
  z-index: 20;\
}\
attachment:vertical attachment-actions {\
  position: absolute;\
  top: 8px;\
  right: 8px;\
}\
attachment-trigger {\
  position: absolute;\
  left: 0;\
  top: 0;\
  right: 0;\
  bottom: 0;\
  z-index: 10;\
  padding: 0;\
  border: 0;\
  border-radius: 12px;\
  background: transparent;\
  cursor: pointer;\
}\
attachment-group {\
  display: flex;\
  gap: 12px;\
  min-width: 0;\
  max-width: 100%;\
}\
attachment .attachment-loading {\
  position: absolute;\
  left: 0;\
  top: 0;\
  width: 100%;\
  height: 20px;\
  z-index: 1;\
}\
attachment .attachment-loading-text {\
  display: block;\
  font-size: 14px;\
  font-weight: 500;\
  line-height: 20px;\
  white-space: nowrap;\
}\
attachment:sm .attachment-loading {\
  height: 18px;\
}\
attachment:sm .attachment-loading-text {\
  font-size: 12px;\
  line-height: 18px;\
}\
attachment:xs .attachment-loading {\
  height: 18px;\
}\
attachment:xs .attachment-loading-text {\
  font-size: 12px;\
  line-height: 18px;\
}\
attachment:vertical .attachment-loading {\
  height: 18px;\
}\
attachment:vertical .attachment-loading-text {\
  font-size: 11px;\
  line-height: 18px;\
}\
";

static void ui_attachment_refresh_loading(ui_widget_t *attachment);

// object 1: attribute and state helpers

static ui_attachment_t *ui_attachment_get_data(ui_widget_t *attachment)
{
        return ui_widget_get_data(attachment, ui_attachment_prototype);
}

static void ui_attachment_on_init(ui_widget_t *attachment)
{
        ui_attachment_t *data = ui_widget_add_data(
            attachment, ui_attachment_prototype, sizeof(ui_attachment_t));

        if (data) {
                memset(data, 0, sizeof(ui_attachment_t));
        }
}

static void ui_attachment_sync_status(ui_widget_t *w, const char *const *names,
                                      size_t count, const char *value)
{
        size_t index;

        for (index = 1; index < count; ++index) {
                ui_widget_remove_status(w, names[index]);
        }
        for (index = 1; index < count; ++index) {
                if (value && strcmp(value, names[index]) == 0) {
                        ui_widget_add_status(w, names[index]);
                        return;
                }
        }
}

static void ui_attachment_sync_state(ui_widget_t *attachment, const char *value)
{
        size_t index;
        bool matched = false;

        for (index = 0; index < sizeof(ui_attachment_state_names) /
                                    sizeof(ui_attachment_state_names[0]);
             ++index) {
                ui_widget_remove_status(attachment,
                                        ui_attachment_state_names[index]);
        }
        if (value) {
                for (index = 0;
                     index < sizeof(ui_attachment_state_names) /
                                 sizeof(ui_attachment_state_names[0]);
                     ++index) {
                        if (strcmp(value, ui_attachment_state_names[index]) ==
                            0) {
                                ui_widget_add_status(
                                    attachment,
                                    ui_attachment_state_names[index]);
                                matched = true;
                                break;
                        }
                }
        }
        if (!matched) {
                ui_widget_add_status(attachment, "done");
        }
}

static void ui_attachment_on_set_attr(ui_widget_t *attachment, const char *name,
                                      const char *value)
{
        if (strcmp(name, "state") == 0) {
                ui_attachment_sync_state(attachment, value);
        } else if (strcmp(name, "size") == 0) {
                ui_attachment_sync_status(
                    attachment, ui_attachment_size_names,
                    sizeof(ui_attachment_size_names) /
                        sizeof(ui_attachment_size_names[0]),
                    value);
        } else if (strcmp(name, "orientation") == 0) {
                ui_attachment_sync_status(
                    attachment, ui_attachment_orientation_names,
                    sizeof(ui_attachment_orientation_names) /
                        sizeof(ui_attachment_orientation_names[0]),
                    value);
        }
}

static void ui_attachment_media_on_set_attr(ui_widget_t *media,
                                            const char *name, const char *value)
{
        if (strcmp(name, "variant") == 0) {
                ui_widget_remove_status(media, "icon");
                ui_widget_remove_status(media, "image");
                if (value && strcmp(value, "image") == 0) {
                        ui_widget_add_status(media, "image");
                } else {
                        ui_widget_add_status(media, "icon");
                }
        } else if (strcmp(name, "src") == 0 && value && value[0]) {
                ui_widget_set_background_image_url(media, value);
        }
}

// object 2: child lookup and composition helpers

static ui_widget_t *ui_attachment_find_child(ui_widget_t *parent,
                                             ui_widget_prototype_t *prototype)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(parent, index)); ++index) {
                if (child->proto == prototype) {
                        return child;
                }
        }
        return NULL;
}

static ui_widget_t *ui_attachment_get_content(ui_widget_t *attachment)
{
        ui_widget_t *content = ui_attachment_find_child(
            attachment, ui_attachment_content_prototype);

        if (!content) {
                content = ui_create_widget_with_prototype(
                    ui_attachment_content_prototype);
                ui_widget_append(attachment, content);
        }
        return content;
}

static ui_widget_t *ui_attachment_get_content_child(
    ui_widget_t *attachment, ui_widget_prototype_t *prototype)
{
        ui_widget_t *content = ui_attachment_get_content(attachment);
        ui_widget_t *child = ui_attachment_find_child(content, prototype);

        if (!child) {
                child = ui_create_widget_with_prototype(prototype);
                ui_widget_append(content, child);
        }
        return child;
}

// object 3: loading shimmer

static ui_widget_t *ui_attachment_find_owner(ui_widget_t *w)
{
        while (w && !ui_check_widget_type(w, "attachment")) {
                w = w->parent;
        }
        return w;
}

static void ui_attachment_content_on_link(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        ui_widget_t *attachment = ui_attachment_find_owner(w->parent);

        if (attachment) {
                ui_attachment_refresh_loading(attachment);
        }
}

static void ui_attachment_content_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_attachment_content_on_link, NULL);
}

static void ui_attachment_trigger_on_link(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        if (w->parent && ui_check_widget_type(w->parent, "attachment")) {
                ui_widget_add_class(w->parent, "has-trigger");
        }
        if (ui_widget_get_attr(w, "data-dialog-trigger")) {
                ui_dialog_bind_data_trigger(w);
        }
}

static void ui_attachment_trigger_on_init(ui_widget_t *w)
{
        if (ui_attachment_trigger_prototype->proto &&
            ui_attachment_trigger_prototype->proto->init) {
                ui_attachment_trigger_prototype->proto->init(w);
        }
        ui_widget_on(w, "link", ui_attachment_trigger_on_link, NULL);
}

static ui_widget_t *ui_attachment_find_loading(ui_widget_t *content)
{
        ui_widget_t *child;
        size_t index;

        for (index = 0; (child = ui_widget_get_child(content, index));
             ++index) {
                if (ui_widget_has_class(child, "attachment-loading")) {
                        return child;
                }
        }
        return NULL;
}

static ui_widget_t *ui_attachment_create_loading(ui_widget_t *content)
{
        ui_widget_t *loading;
        ui_widget_t *shimmer_content;
        ui_widget_t *text;

        loading = ui_create_shimmer();
        ui_widget_add_class(loading, "attachment-loading");
        ui_shimmer_set_duration(loading, 1600);
        shimmer_content = ui_create_widget("shimmer-content");
        text = ui_create_widget("text");
        ui_widget_add_class(text, "attachment-loading-text");
        ui_widget_append(shimmer_content, text);
        ui_shimmer_set_content(loading, shimmer_content);
        ui_widget_append(content, loading);
        return loading;
}

static void ui_attachment_set_loading_text(ui_widget_t *loading,
                                           ui_widget_t *title)
{
        wchar_t *wstr;
        size_t length;
        ui_widget_t *content;
        ui_widget_t *text;

        content = ui_widget_get_child(loading, 0);
        if (!content) {
                return;
        }
        text = ui_widget_get_child(content, 0);
        if (!text || !ui_check_widget_type(text, "text")) {
                return;
        }
        length = ui_text_get_content_w(title, NULL, 0);
        wstr = malloc(sizeof(wchar_t) * (length + 1));
        if (!wstr) {
                return;
        }
        ui_text_get_content_w(title, wstr, length + 1);
        ui_text_set_content_w(text, wstr);
        free(wstr);
}

static void ui_attachment_refresh_loading(ui_widget_t *attachment)
{
        ui_attachment_t *data = ui_attachment_get_data(attachment);
        ui_widget_t *content;
        ui_widget_t *title;
        ui_widget_t *loading;
        bool should_load;

        if (!data) {
                return;
        }
        should_load = ui_widget_has_status(attachment, "uploading") ||
                      ui_widget_has_status(attachment, "processing");
        if (!should_load) {
                if (data->loading_visible && data->loading) {
                        ui_widget_set_style_string(data->loading, "display",
                                                   "none");
                        ui_widget_set_style_string(data->title, "visibility",
                                                   "visible");
                        data->loading_visible = false;
                }
                return;
        }
        if (data->loading_visible && data->loading) {
                return;
        }
        content = ui_attachment_find_child(attachment,
                                           ui_attachment_content_prototype);
        if (!content) {
                return;
        }
        title =
            ui_attachment_find_child(content, ui_attachment_title_prototype);
        if (!title) {
                return;
        }
        loading = ui_attachment_find_loading(content);
        if (!loading) {
                loading = ui_attachment_create_loading(content);
        }
        data->title = title;
        data->loading = loading;
        data->loading_visible = true;
        ui_attachment_set_loading_text(loading, title);
        ui_widget_set_style_string(loading, "display", "block");
        ui_widget_set_style_string(title, "visibility", "hidden");
}

// object 4: factories

ui_widget_t *ui_create_attachment(void)
{
        ui_widget_t *attachment =
            ui_create_widget_with_prototype(ui_attachment_prototype);

        ui_widget_add_status(attachment, "done");
        return attachment;
}

ui_widget_t *ui_create_attachment_media(void)
{
        ui_widget_t *media =
            ui_create_widget_with_prototype(ui_attachment_media_prototype);

        ui_widget_add_status(media, "icon");
        return media;
}

ui_widget_t *ui_create_attachment_content(void)
{
        return ui_create_widget_with_prototype(ui_attachment_content_prototype);
}

ui_widget_t *ui_create_attachment_title(void)
{
        return ui_create_widget_with_prototype(ui_attachment_title_prototype);
}

ui_widget_t *ui_create_attachment_description(void)
{
        return ui_create_widget_with_prototype(
            ui_attachment_description_prototype);
}

ui_widget_t *ui_create_attachment_actions(void)
{
        return ui_create_widget_with_prototype(ui_attachment_actions_prototype);
}

ui_widget_t *ui_create_attachment_action(void)
{
        ui_widget_t *action = ui_create_button();

        ui_button_set_variant(action, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(action, UI_BUTTON_SIZE_ICON_XS);
        ui_widget_add_class(action, "attachment-action");
        return action;
}

ui_widget_t *ui_create_attachment_trigger(void)
{
        return ui_create_widget_with_prototype(ui_attachment_trigger_prototype);
}

ui_widget_t *ui_create_attachment_group(void)
{
        return ui_create_widget_with_prototype(ui_attachment_group_prototype);
}

// object 5: public setters and getters

void ui_attachment_set_state(ui_widget_t *attachment,
                             ui_attachment_state_t state)
{
        if (state < UI_ATTACHMENT_STATE_IDLE ||
            state > UI_ATTACHMENT_STATE_DONE) {
                state = UI_ATTACHMENT_STATE_DONE;
        }
        ui_widget_set_attr(attachment, "state",
                           ui_attachment_state_names[state]);
        ui_attachment_refresh_loading(attachment);
}

ui_attachment_state_t ui_attachment_get_state(ui_widget_t *attachment)
{
        const char *value = ui_widget_get_attr(attachment, "state");
        size_t index;

        if (value) {
                for (index = 0;
                     index < sizeof(ui_attachment_state_names) /
                                 sizeof(ui_attachment_state_names[0]);
                     ++index) {
                        if (strcmp(value, ui_attachment_state_names[index]) ==
                            0) {
                                return (ui_attachment_state_t)index;
                        }
                }
        }
        return UI_ATTACHMENT_STATE_DONE;
}

void ui_attachment_set_size(ui_widget_t *attachment, ui_attachment_size_t size)
{
        if (size < UI_ATTACHMENT_SIZE_DEFAULT || size > UI_ATTACHMENT_SIZE_XS) {
                size = UI_ATTACHMENT_SIZE_DEFAULT;
        }
        ui_widget_set_attr(attachment, "size", ui_attachment_size_names[size]);
}

ui_attachment_size_t ui_attachment_get_size(ui_widget_t *attachment)
{
        const char *value = ui_widget_get_attr(attachment, "size");

        if (value) {
                if (strcmp(value, "sm") == 0) {
                        return UI_ATTACHMENT_SIZE_SM;
                }
                if (strcmp(value, "xs") == 0) {
                        return UI_ATTACHMENT_SIZE_XS;
                }
        }
        return UI_ATTACHMENT_SIZE_DEFAULT;
}

void ui_attachment_set_orientation(ui_widget_t *attachment,
                                   ui_attachment_orientation_t orientation)
{
        if (orientation < UI_ATTACHMENT_ORIENTATION_HORIZONTAL ||
            orientation > UI_ATTACHMENT_ORIENTATION_VERTICAL) {
                orientation = UI_ATTACHMENT_ORIENTATION_HORIZONTAL;
        }
        ui_widget_set_attr(attachment, "orientation",
                           ui_attachment_orientation_names[orientation]);
}

ui_attachment_orientation_t ui_attachment_get_orientation(
    ui_widget_t *attachment)
{
        const char *value = ui_widget_get_attr(attachment, "orientation");

        if (value && strcmp(value, "vertical") == 0) {
                return UI_ATTACHMENT_ORIENTATION_VERTICAL;
        }
        return UI_ATTACHMENT_ORIENTATION_HORIZONTAL;
}

void ui_attachment_media_set_variant(ui_widget_t *media,
                                     ui_attachment_media_variant_t variant)
{
        if (variant < UI_ATTACHMENT_MEDIA_VARIANT_ICON ||
            variant > UI_ATTACHMENT_MEDIA_VARIANT_IMAGE) {
                variant = UI_ATTACHMENT_MEDIA_VARIANT_ICON;
        }
        ui_widget_set_attr(media, "variant",
                           ui_attachment_media_variant_names[variant]);
}

ui_attachment_media_variant_t ui_attachment_media_get_variant(
    ui_widget_t *media)
{
        const char *value = ui_widget_get_attr(media, "variant");

        if (value && strcmp(value, "image") == 0) {
                return UI_ATTACHMENT_MEDIA_VARIANT_IMAGE;
        }
        return UI_ATTACHMENT_MEDIA_VARIANT_ICON;
}

void ui_attachment_media_set_image_url(ui_widget_t *media, const char *path)
{
        ui_attachment_media_set_variant(media,
                                        UI_ATTACHMENT_MEDIA_VARIANT_IMAGE);
        if (path && path[0]) {
                ui_widget_set_background_image_url(media, path);
        }
}

void ui_attachment_set_title(ui_widget_t *attachment, const char *text)
{
        ui_attachment_t *data = ui_attachment_get_data(attachment);
        ui_widget_t *title = ui_attachment_get_content_child(
            attachment, ui_attachment_title_prototype);

        ui_text_set_content(title, text);
        if (data && data->loading) {
                ui_attachment_set_loading_text(data->loading, title);
        } else {
                ui_attachment_refresh_loading(attachment);
        }
}

void ui_attachment_set_description(ui_widget_t *attachment, const char *text)
{
        ui_text_set_content(
            ui_attachment_get_content_child(
                attachment, ui_attachment_description_prototype),
            text);
}

// object 6: registration

void ui_register_attachment(void)
{
        ui_attachment_prototype =
            ui_create_widget_prototype("attachment", NULL);
        ui_attachment_media_prototype =
            ui_create_widget_prototype("attachment-media", NULL);
        ui_attachment_content_prototype =
            ui_create_widget_prototype("attachment-content", NULL);
        ui_attachment_title_prototype =
            ui_create_widget_prototype("attachment-title", "text");
        ui_attachment_description_prototype =
            ui_create_widget_prototype("attachment-description", "text");
        ui_attachment_actions_prototype =
            ui_create_widget_prototype("attachment-actions", NULL);
        ui_attachment_trigger_prototype =
            ui_create_widget_prototype("attachment-trigger", "button");
        ui_attachment_group_prototype =
            ui_create_widget_prototype("attachment-group", NULL);
        ui_attachment_prototype->init = ui_attachment_on_init;
        ui_attachment_prototype->setattr = ui_attachment_on_set_attr;
        ui_attachment_media_prototype->setattr =
            ui_attachment_media_on_set_attr;
        ui_attachment_content_prototype->init = ui_attachment_content_on_init;
        ui_attachment_trigger_prototype->init = ui_attachment_trigger_on_init;
        ui_load_css_string(ui_attachment_css, __FILE__);
}
