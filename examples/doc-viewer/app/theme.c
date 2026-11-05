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
