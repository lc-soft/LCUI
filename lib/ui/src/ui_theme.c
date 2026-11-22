/*
 * lib/ui/src/ui_theme.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pandagl.h>
#include <ui/base.h>
#include <ui/css.h>
#include <ui/theme.h>
#include "../../../src/styles/accent-palette.h"
#include "../../../src/styles/accent-rules.h"
#include "../../../src/styles/accents/accent-themes-api.h"

#define ACCENT_NAME_MAX 64
#define ACCENT_CSS_MAX 65536
#define ACCENT_CSS_SPACE "ui-theme-accent"
#define ACCENT_SPACE_MAX (ACCENT_NAME_MAX + 8)

typedef struct theme_color {
        pd_color_t color;
        float alpha;
} theme_color_t;

typedef struct theme_tokens {
        theme_color_t light[8];
        theme_color_t dark[8];
} theme_tokens_t;

static char current_accent[ACCENT_NAME_MAX] = "default";
static char current_class[ACCENT_NAME_MAX] = "";
static char loaded_space[ACCENT_SPACE_MAX] = "";

static void unload_accent_css(void)
{
        if (loaded_space[0] != 0) {
                ui_unload_css_string(loaded_space);
                loaded_space[0] = 0;
        }
}

static const ui_theme_accent_t *find_accent(const char *name)
{
        size_t i;

        for (i = 0; i < sizeof(ui_theme_accents) / sizeof(ui_theme_accents[0]);
             ++i) {
                if (strcmp(ui_theme_accents[i].name, name) == 0) {
                        return &ui_theme_accents[i];
                }
        }
        return NULL;
}

static int parse_hex(const char *hex, pd_color_t *color)
{
        unsigned r, g, b;

        if (!hex) {
                return -1;
        }
        if (hex[0] == '#') {
                ++hex;
        }
        if (strlen(hex) != 6 || strspn(hex, "0123456789abcdefABCDEF") != 6) {
                return -1;
        }
        if (sscanf(hex, "%2X%2X%2X", &r, &g, &b) != 3) {
                return -1;
        }
        *color = pd_color(255, (uint8_t)r, (uint8_t)g, (uint8_t)b);
        return 0;
}

static pd_color_t mix_color(pd_color_t a, pd_color_t b, float ratio)
{
        pd_color_t color;

        color.a = 255;
        color.r = (uint8_t)roundf(a.r * (1 - ratio) + b.r * ratio);
        color.g = (uint8_t)roundf(a.g * (1 - ratio) + b.g * ratio);
        color.b = (uint8_t)roundf(a.b * (1 - ratio) + b.b * ratio);
        return color;
}

static float channel_luminance(uint8_t channel)
{
        float value = channel / 255.0f;

        if (value <= 0.04045f) {
                return value / 12.92f;
        }
        return powf((value + 0.055f) / 1.055f, 2.4f);
}

static float relative_luminance(pd_color_t color)
{
        return 0.2126f * channel_luminance(color.r) +
               0.7152f * channel_luminance(color.g) +
               0.0722f * channel_luminance(color.b);
}

static pd_color_t foreground_for(pd_color_t color)
{
        if (relative_luminance(color) > 0.5f) {
                return pd_color(255, 0x17, 0x17, 0x17);
        }
        return pd_color(255, 0xfa, 0xfa, 0xfa);
}

static void set_token(theme_tokens_t *tokens, int dark, ui_theme_token_t token,
                      pd_color_t color, float alpha)
{
        theme_color_t *entry =
            dark ? &tokens->dark[token] : &tokens->light[token];

        entry->color = color;
        entry->alpha = alpha;
}

static void derive_tokens(theme_tokens_t *tokens, pd_color_t light,
                          pd_color_t dark)
{
        pd_color_t light_foreground = foreground_for(light);
        pd_color_t dark_foreground = foreground_for(dark);

        set_token(tokens, 0, UI_THEME_TOKEN_PRIMARY, light, 1.0f);
        set_token(tokens, 0, UI_THEME_TOKEN_PRIMARY_FOREGROUND,
                  light_foreground, 1.0f);
        set_token(tokens, 0, UI_THEME_TOKEN_ACCENT, light, 0.1f);
        set_token(tokens, 0, UI_THEME_TOKEN_ACCENT_FOREGROUND, light, 1.0f);
        set_token(tokens, 0, UI_THEME_TOKEN_SIDEBAR_PRIMARY, light, 1.0f);
        set_token(tokens, 0, UI_THEME_TOKEN_SIDEBAR_PRIMARY_FOREGROUND,
                  light_foreground, 1.0f);
        set_token(tokens, 0, UI_THEME_TOKEN_SIDEBAR_ACCENT, light, 0.1f);
        set_token(tokens, 0, UI_THEME_TOKEN_SIDEBAR_ACCENT_FOREGROUND, light,
                  1.0f);

        set_token(tokens, 1, UI_THEME_TOKEN_PRIMARY, dark, 1.0f);
        set_token(tokens, 1, UI_THEME_TOKEN_PRIMARY_FOREGROUND, dark_foreground,
                  1.0f);
        set_token(tokens, 1, UI_THEME_TOKEN_ACCENT, dark, 0.15f);
        set_token(tokens, 1, UI_THEME_TOKEN_ACCENT_FOREGROUND, dark, 1.0f);
        set_token(tokens, 1, UI_THEME_TOKEN_SIDEBAR_PRIMARY, dark, 1.0f);
        set_token(tokens, 1, UI_THEME_TOKEN_SIDEBAR_PRIMARY_FOREGROUND,
                  dark_foreground, 1.0f);
        set_token(tokens, 1, UI_THEME_TOKEN_SIDEBAR_ACCENT, dark, 0.15f);
        set_token(tokens, 1, UI_THEME_TOKEN_SIDEBAR_ACCENT_FOREGROUND, dark,
                  1.0f);
}

static theme_color_t *token_color(theme_tokens_t *tokens,
                                  ui_theme_token_t token, int dark)
{
        return dark ? &tokens->dark[token] : &tokens->light[token];
}

static void format_color(theme_color_t *color, char *buf, size_t size)
{
        snprintf(buf, size, "rgba(%u, %u, %u, %.2f)", color->color.r,
                 color->color.g, color->color.b, color->alpha);
}

static void format_rule_value(const ui_theme_rule_t *rule, theme_color_t *color,
                              char *buf, size_t size)
{
        if (rule->shadow > 0) {
                snprintf(buf, size, "0 0 0 %dpx rgba(%u, %u, %u, %.2f)",
                         rule->shadow, color->color.r, color->color.g,
                         color->color.b, color->alpha);
                return;
        }
        format_color(color, buf, size);
}

static int append_rule(char *css, size_t *length, int dark,
                       const char *selector, const char *property,
                       const char *value)
{
        const char *prefix = dark ? ".accent-custom.dark" : ".accent-custom";
        size_t remaining;
        int written;

        if (*length >= ACCENT_CSS_MAX) {
                return -1;
        }
        remaining = ACCENT_CSS_MAX - *length;
        written =
            snprintf(css + *length, remaining, "%s %s {\n  %s: %s;\n}\n\n",
                     prefix, selector, property, value);
        if (written < 0 || (size_t)written >= remaining) {
                return -1;
        }
        *length += (size_t)written;
        return 0;
}

static void remove_current_class(void)
{
        if (current_class[0] != 0) {
                ui_widget_remove_class(ui_root(), current_class);
                current_class[0] = 0;
        }
}

static void set_current_class(const char *class_name)
{
        remove_current_class();
        snprintf(current_class, sizeof(current_class), "%s", class_name);
        ui_widget_add_class(ui_root(), current_class);
}

int ui_theme_set_accent(const char *name)
{
        const ui_theme_accent_t *accent;
        char class_name[ACCENT_NAME_MAX];

        if (!name || strcmp(name, "default") == 0) {
                unload_accent_css();
                remove_current_class();
                snprintf(current_accent, sizeof(current_accent), "default");
                return 0;
        }
        accent = find_accent(name);
        if (!accent) {
                return -1;
        }
        snprintf(class_name, sizeof(class_name), "accent-%s", accent->name);
        unload_accent_css();
        if (!ui_load_accent_theme(accent->name)) {
                remove_current_class();
                snprintf(current_accent, sizeof(current_accent), "default");
                return -1;
        }
        snprintf(loaded_space, sizeof(loaded_space), "%s.css", accent->name);
        set_current_class(class_name);
        snprintf(current_accent, sizeof(current_accent), "%s", accent->name);
        return 0;
}

int ui_theme_set_accent_color(const char *hex)
{
        pd_color_t light;
        pd_color_t dark;
        theme_tokens_t tokens;
        char *css;
        char value[64];
        size_t length = 0;
        size_t i;

        if (parse_hex(hex, &light) != 0) {
                return -1;
        }
        dark = mix_color(light, pd_color(255, 0xff, 0xff, 0xff), 0.2f);
        derive_tokens(&tokens, light, dark);

        css = calloc(1, ACCENT_CSS_MAX);
        if (!css) {
                return -1;
        }
        for (i = 0; i < sizeof(ui_theme_rules) / sizeof(ui_theme_rules[0]);
             ++i) {
                const ui_theme_rule_t *rule = &ui_theme_rules[i];
                theme_color_t color =
                    *token_color(&tokens, rule->token, rule->dark);

                if (rule->alpha >= 0) {
                        color.alpha = rule->alpha / 255.0f;
                }
                format_rule_value(rule, &color, value, sizeof(value));
                if (append_rule(css, &length, rule->dark, rule->selector,
                                rule->property, value) != 0) {
                        free(css);
                        return -1;
                }
        }
        unload_accent_css();
        ui_load_css_string(css, ACCENT_CSS_SPACE);
        free(css);
        snprintf(loaded_space, sizeof(loaded_space), "%s", ACCENT_CSS_SPACE);

        set_current_class("accent-custom");
        snprintf(current_accent, sizeof(current_accent), "#%02x%02x%02x",
                 light.r, light.g, light.b);
        return 0;
}

const char *ui_theme_get_accent(void)
{
        return current_accent;
}
