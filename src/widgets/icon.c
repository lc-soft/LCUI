/*
 * src/widgets/icon.c: -- Icon widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT LICENSE file in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/widgets/icon.h>
#include <ui/css.h>
#include <ui/prototype.h>

typedef struct ui_icon {
        char *name;
        char *name_class;
        char *filled_class;
        char *size_class;
        int size;
        bool filled;
        ui_icon_library_t library;
} ui_icon_t;

static ui_widget_prototype_t *ui_icon_prototype;

/* Fluent icon glyphs have degenerate vertical metrics (hhea ascender = 1em,
 * descender = 0), and pandagl centers the line box by the font ascent/descent
 * box, so the ink renders slightly above the visual center (~0.5-2.5px
 * depending on the font size). Nudge fluent icons down 1px rather than
 * patching the font or the renderer. */
static const char *ui_icon_css = "\
icon,\
icon.fui-icon {\
  top: 1px;\
  display: inline-block;\
  position: relative;\
  line-height: 1;\
}";

static const char *ui_icon_library_names[] = {
        "fluent",
        "lucide",
};

static const char *ui_icon_library_classes[] = {
        "fui-icon",
        "lucide-icon",
};

static bool ui_icon_parse_bool(const char *value)
{
        return value &&
               (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
}

static void ui_icon_remove_class(ui_widget_t *icon, char **class_name)
{
        if (*class_name) {
                ui_widget_remove_class(icon, *class_name);
                free(*class_name);
                *class_name = NULL;
        }
}

static char *ui_icon_create_class(const char *prefix, const char *name)
{
        size_t length = strlen(prefix) + strlen(name) + 1;
        char *class_name = malloc(length);

        snprintf(class_name, length, "%s%s", prefix, name);
        return class_name;
}

static void ui_icon_update_classes(ui_widget_t *icon)
{
        ui_icon_t *that = ui_widget_get_data(icon, ui_icon_prototype);
        const char *font_class = ui_icon_library_classes[that->library];
        const char *name_prefix;
        const char *size_prefix;
        char size[16];

        ui_widget_remove_class(icon, "fui-icon");
        ui_widget_remove_class(icon, "lucide-icon");
        ui_icon_remove_class(icon, &that->name_class);
        ui_icon_remove_class(icon, &that->filled_class);
        ui_icon_remove_class(icon, &that->size_class);
        ui_widget_add_class(icon, font_class);
        if (!that->name || !that->name[0]) {
                return;
        }

        name_prefix = that->library == UI_ICON_LIBRARY_FLUENT ? "fui-icon-"
                                                              : "lucide-icon-";
        size_prefix = that->library == UI_ICON_LIBRARY_FLUENT
                          ? "fui-icon-glyph-"
                          : "lucide-icon-glyph-";
        that->name_class = ui_icon_create_class(name_prefix, that->name);
        ui_widget_add_class(icon, that->name_class);
        if (that->library == UI_ICON_LIBRARY_FLUENT && that->filled) {
                that->filled_class = y_strdup("fui-icon-filled");
                ui_widget_add_class(icon, that->filled_class);
        }
        if (that->size > 0 && that->size != 24) {
                snprintf(size, sizeof(size), "%d", that->size);
                that->size_class = ui_icon_create_class(size_prefix, size);
                ui_widget_add_class(icon, that->size_class);
        }
}

static void ui_icon_update_name(ui_widget_t *icon, const char *name)
{
        ui_icon_t *that = ui_widget_get_data(icon, ui_icon_prototype);

        free(that->name);
        that->name = name ? y_strdup(name) : NULL;
        ui_icon_update_classes(icon);
}

static void ui_icon_on_set_attr(ui_widget_t *icon, const char *name,
                                const char *value)
{
        ui_icon_t *that = ui_widget_get_data(icon, ui_icon_prototype);

        if (strcmp(name, "library") == 0) {
                that->library = value && strcmp(value, "lucide") == 0
                                    ? UI_ICON_LIBRARY_LUCIDE
                                    : UI_ICON_LIBRARY_FLUENT;
                ui_icon_update_classes(icon);
        } else if (strcmp(name, "name") == 0) {
                ui_icon_update_name(icon, value);
        } else if (strcmp(name, "size") == 0) {
                that->size = value ? atoi(value) : 0;
                if (that->size < 1) {
                        that->size = 0;
                }
                ui_icon_update_classes(icon);
        } else if (strcmp(name, "filled") == 0) {
                that->filled = ui_icon_parse_bool(value);
                ui_icon_update_classes(icon);
        }
}

static void ui_icon_on_init(ui_widget_t *icon)
{
        ui_icon_t *that;

        ui_icon_prototype->proto->init(icon);
        that = ui_widget_add_data(icon, ui_icon_prototype, sizeof(ui_icon_t));
        that->name = NULL;
        that->name_class = NULL;
        that->filled_class = NULL;
        that->size_class = NULL;
        that->size = 0;
        that->filled = false;
        that->library = UI_ICON_LIBRARY_FLUENT;
        ui_icon_update_classes(icon);
}

static void ui_icon_on_destroy(ui_widget_t *icon)
{
        ui_icon_t *that = ui_widget_get_data(icon, ui_icon_prototype);

        free(that->name);
        free(that->name_class);
        free(that->filled_class);
        free(that->size_class);
        ui_icon_prototype->proto->destroy(icon);
}

ui_widget_t *ui_create_icon(void)
{
        return ui_create_widget_with_prototype(ui_icon_prototype);
}

ui_widget_t *ui_create_fluent_icon(const char *name)
{
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, name);
        return icon;
}

void ui_icon_set_library(ui_widget_t *icon, ui_icon_library_t library)
{
        if (library < UI_ICON_LIBRARY_FLUENT ||
            library > UI_ICON_LIBRARY_LUCIDE) {
                library = UI_ICON_LIBRARY_FLUENT;
        }
        ui_widget_set_attr(icon, "library", ui_icon_library_names[library]);
}

void ui_icon_set_name(ui_widget_t *icon, const char *name)
{
        ui_widget_set_attr(icon, "name", name ? name : "");
}

void ui_icon_set_size(ui_widget_t *icon, int size)
{
        char value[16];

        snprintf(value, sizeof(value), "%d", size);
        ui_widget_set_attr(icon, "size", value);
}

void ui_icon_set_filled(ui_widget_t *icon, bool filled)
{
        ui_widget_set_attr(icon, "filled", filled ? "true" : "false");
}

void ui_register_icon(void)
{
        ui_icon_prototype = ui_create_widget_prototype("icon", "text");
        ui_icon_prototype->init = ui_icon_on_init;
        ui_icon_prototype->destroy = ui_icon_on_destroy;
        ui_icon_prototype->setattr = ui_icon_on_set_attr;
        ui_load_css_string(ui_icon_css, __FILE__);
}
