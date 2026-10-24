/*
 * src/widgets/card.c: -- Card widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI/widgets/card.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>

static ui_widget_prototype_t *ui_card_prototype;
static ui_widget_prototype_t *ui_card_header_prototype;
static ui_widget_prototype_t *ui_card_title_prototype;
static ui_widget_prototype_t *ui_card_description_prototype;
static ui_widget_prototype_t *ui_card_action_prototype;
static ui_widget_prototype_t *ui_card_content_prototype;
static ui_widget_prototype_t *ui_card_footer_prototype;

static const char *ui_card_size_names[] = {
        "default",
        "sm",
};

static const char *ui_card_css = "\
card {\
  display: flex;\
  flex-direction: column;\
}\
card-header {\
  display: flex;\
  position: relative;\
  flex-direction: column;\
}\
card-title {\
  min-width: 0;\
}\
card-action {\
  display: flex;\
  position: absolute;\
  top: 0;\
  right: 0;\
  flex-shrink: 0;\
}\
card-content {\
  display: block;\
}\
card-footer {\
  display: flex;\
  align-items: center;\
}\
";

static void ui_card_action_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
        if (w->parent && w->parent->proto == ui_card_header_prototype) {
                ui_widget_add_class(w->parent, "has-card-action");
        }
}

static void ui_card_action_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "link", ui_card_action_on_link, NULL);
}

static void ui_card_on_set_attr(ui_widget_t *w, const char *name,
                                const char *value)
{
        if (strcmp(name, "size") != 0) {
                return;
        }
        ui_widget_remove_status(w, "sm");
        if (value && strcmp(value, "sm") == 0) {
                ui_widget_add_status(w, "sm");
        }
}

ui_widget_t *ui_create_card(void)
{
        return ui_create_widget_with_prototype(ui_card_prototype);
}

ui_widget_t *ui_create_card_header(void)
{
        return ui_create_widget_with_prototype(ui_card_header_prototype);
}

ui_widget_t *ui_create_card_title(void)
{
        return ui_create_widget_with_prototype(ui_card_title_prototype);
}

ui_widget_t *ui_create_card_description(void)
{
        return ui_create_widget_with_prototype(ui_card_description_prototype);
}

ui_widget_t *ui_create_card_action(void)
{
        return ui_create_widget_with_prototype(ui_card_action_prototype);
}

ui_widget_t *ui_create_card_content(void)
{
        return ui_create_widget_with_prototype(ui_card_content_prototype);
}

ui_widget_t *ui_create_card_footer(void)
{
        return ui_create_widget_with_prototype(ui_card_footer_prototype);
}

void ui_card_set_size(ui_widget_t *card, ui_card_size_t size)
{
        if (size < UI_CARD_SIZE_DEFAULT || size > UI_CARD_SIZE_SM) {
                size = UI_CARD_SIZE_DEFAULT;
        }
        ui_widget_set_attr(card, "size", ui_card_size_names[size]);
}

ui_card_size_t ui_card_get_size(ui_widget_t *card)
{
        const char *value = ui_widget_get_attr(card, "size");

        if (value && strcmp(value, "sm") == 0) {
                return UI_CARD_SIZE_SM;
        }
        return UI_CARD_SIZE_DEFAULT;
}

void ui_register_card(void)
{
        ui_card_prototype = ui_create_widget_prototype("card", NULL);
        ui_card_header_prototype =
            ui_create_widget_prototype("card-header", NULL);
        ui_card_title_prototype =
            ui_create_widget_prototype("card-title", "text");
        ui_card_description_prototype =
            ui_create_widget_prototype("card-description", "text");
        ui_card_action_prototype =
            ui_create_widget_prototype("card-action", NULL);
        ui_card_content_prototype =
            ui_create_widget_prototype("card-content", NULL);
        ui_card_footer_prototype =
            ui_create_widget_prototype("card-footer", NULL);
        ui_card_prototype->setattr = ui_card_on_set_attr;
        ui_card_action_prototype->init = ui_card_action_on_init;
        ui_load_css_string(ui_card_css, __FILE__);
}
