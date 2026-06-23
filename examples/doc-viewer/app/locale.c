#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <LCUI.h>
#include <i18n.h>
#include "locale.h"

#define SETTINGS_SECTION "app"
#define SETTINGS_KEY "locale"
#define DEFAULT_LOCALE "en"

/* Locales the doc-viewer ships.  Used to validate both persisted values
 * and OS-detected values, and as the router path-prefix vocabulary. */
static const char *supported_locales[] = { "en", "zh-CN", NULL };

/* Returns true when `needle` is a member of supported_locales. */
static bool is_supported(const char *value)
{
        const char **it;

        for (it = supported_locales; *it; ++it) {
                if (strcmp(value, *it) == 0) {
                        return true;
                }
        }
        return false;
}

/* Match a detected system locale string (Windows GetUserDefaultLocaleName
 * produces BCP-47 like "zh-CN"; POSIX $LANG is normalized by lib/i18n to
 * the same shape, e.g. "zh_CN" -> "zh-CN") against supported_locales.
 * A language-only match counts: "zh-TW" falls back to "zh-CN" since
 * zh-TW is not shipped; both '-' and '_' are accepted as separators. */
static const char *match_supported_locale(const char *detected)
{
        const char **it;

        if (!detected || !*detected) {
                return NULL;
        }
        for (it = supported_locales; *it; ++it) {
                if (strcmp(detected, *it) == 0) {
                        return *it;
                }
        }
        for (it = supported_locales; *it; ++it) {
                size_t prefix_len = strcspn(*it, "-");
                char sep = detected[prefix_len];
                if (strncmp(detected, *it, prefix_len) == 0 &&
                    (sep == 0 || sep == '-' || sep == '_')) {
                        return *it;
                }
        }
        return NULL;
}

/* Copy `value` into a stable static buffer so callers can hold the
 * pointer past the next call.  Caller is responsible for `free`-ing
 * the lcui_settings-owned `value` buffer. */
static const char *stash_locale(const char *value)
{
        static char buf[32];
        size_t n = strlen(value);

        if (n >= sizeof(buf)) {
                n = sizeof(buf) - 1;
        }
        memcpy(buf, value, n);
        buf[n] = 0;
        return buf;
}

const char *app_get_locale(void)
{
        char *saved = NULL;
        const char *matched;

        if (!lcui_settings_is_loaded()) {
                lcui_settings_load();
        }
        if (lcui_settings_get_string(SETTINGS_SECTION, SETTINGS_KEY, &saved) &&
            saved) {
                matched = match_supported_locale(saved);
                free(saved);
                if (matched) {
                        return stash_locale(matched);
                }
        }

        /* No usable persisted locale: ask the operating system. */
        wchar_t wbuf[32];
        size_t wlen = i18n_detect_language(wbuf, 32);
        if (wlen > 0 && wlen < 32) {
                char nbuf[32];
                size_t i;
                for (i = 0; i < wlen; ++i) {
                        nbuf[i] = (char)wbuf[i];
                }
                nbuf[i] = 0;
                matched = match_supported_locale(nbuf);
                if (matched) {
                        return matched;
                }
        }

        return DEFAULT_LOCALE;
}

void app_set_locale(const char *locale)
{
        if (!locale || !is_supported(locale)) {
                return;
        }
        if (!lcui_settings_is_loaded()) {
                return;
        }
        lcui_settings_set_string(SETTINGS_SECTION, SETTINGS_KEY, locale);
}
