#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI.h>
#include "theme.h"

#define SETTINGS_SECTION "app"
#define SETTINGS_KEY "theme"
#define THEME_LIGHT "light"
#define THEME_DARK "dark"

static const char *normalize_theme(const char *theme)
{
        if (theme && strcmp(theme, THEME_DARK) == 0) {
                return THEME_DARK;
        }
        return THEME_LIGHT;
}

const char *app_get_theme(void)
{
        char *saved = NULL;
        const char *theme = THEME_LIGHT;

        if (!lcui_settings_is_loaded()) {
                lcui_settings_load();
        }
        if (lcui_settings_get_string(SETTINGS_SECTION, SETTINGS_KEY, &saved) &&
            saved) {
                theme = normalize_theme(saved);
                free(saved);
        }
        return theme;
}

void app_set_theme(const char *theme)
{
        if (!lcui_settings_is_loaded()) {
                return;
        }
        lcui_settings_set_string(SETTINGS_SECTION, SETTINGS_KEY,
                                 normalize_theme(theme));
        lcui_settings_flush();
}

void app_apply_theme(void)
{
        if (strcmp(app_get_theme(), THEME_DARK) == 0) {
                ui_widget_add_class(ui_root(), THEME_DARK);
        } else {
                ui_widget_remove_class(ui_root(), THEME_DARK);
        }
}

static char accent_value[64] = "neutral";

const char *app_get_accent(void)
{
        char *saved = NULL;

        if (!lcui_settings_is_loaded()) {
                lcui_settings_load();
        }
        if (lcui_settings_get_string(SETTINGS_SECTION, "accent", &saved) &&
            saved) {
                if (saved[0] != 0) {
                        snprintf(accent_value, sizeof(accent_value), "%s",
                                 saved);
                } else {
                        snprintf(accent_value, sizeof(accent_value), "neutral");
                }
                free(saved);
        }
        return accent_value;
}

void app_set_accent(const char *accent)
{
        if (!lcui_settings_is_loaded()) {
                return;
        }
        lcui_settings_set_string(SETTINGS_SECTION, "accent",
                                 accent && accent[0] ? accent : "neutral");
        lcui_settings_flush();
}

void app_apply_accent(void)
{
        const char *accent = app_get_accent();

        if (accent[0] == '#') {
                ui_theme_set_accent_color(accent);
        } else {
                ui_theme_set_accent(accent);
        }
}
