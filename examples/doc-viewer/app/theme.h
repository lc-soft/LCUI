#ifndef APP_THEME_H
#define APP_THEME_H

/*
 * app/theme.h - Color theme resolution and persistence.
 *
 * The doc-viewer ships a "light" and a "dark" theme.  The current theme
 * is persisted through lcui_settings under [app] theme; unknown values
 * fall back to "light".
 */

/* Returns "light" or "dark". */
const char *app_get_theme(void);

/* Persists the given theme. Unknown values are stored as "light". */
void app_set_theme(const char *theme);

/* Applies the persisted theme to ui_root() (adds/removes the `dark` class). */
void app_apply_theme(void);

/* Returns the persisted accent name, or a custom "#RRGGBB" value. */
const char *app_get_accent(void);

/* Persists the accent name or custom color. */
void app_set_accent(const char *accent);

/* Applies the persisted accent to the UI theme. */
void app_apply_accent(void);

#endif
