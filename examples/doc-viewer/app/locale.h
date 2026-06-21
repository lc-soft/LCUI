#ifndef APP_LOCALE_H
#define APP_LOCALE_H

/*
 * app/locale.h - Application-level locale resolution and persistence.
 *
 * Locale selection at startup follows this order:
 *   1. Locale persisted via lcui_settings under [app] locale.
 *   2. Locale detected from the operating system (Windows
 *      GetUserDefaultLocaleName, or $LANG on POSIX).  Detection is
 *      driven by lib/i18n's i18n_detect_language.  Detected values
 *      are matched against the locales shipped with the doc-viewer.
 *   3. The default locale ("en").
 *
 * The doc-viewer only ships a fixed set of locales; values outside
 * that set are rejected at every step.  Callers receive a pointer
 * into a static buffer that is overwritten on the next call.
 */

/* Returns the locale to use, picked from the shipped locale set. */
const char *app_get_locale(void);

/* Persists the given locale through lcui_settings.  No-op if the
 * settings doc has not been loaded. */
void app_set_locale(const char *locale);

#endif
