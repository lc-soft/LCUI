#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI.h>
#include <router.h>
#include "navbar.tsx.h"
#include "navbar.h"
#include "route-titles.h"
#include "../locale.h"

typedef struct {
        navbar_react_t base;
        router_t *router;
        router_watcher_t *watcher;
        char *current_locale;
} navbar_t;

static navbar_t *navbar_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, navbar_proto);
}

/* Locale persistence lives in app/locale.c so that the resolution
 * rules (saved -> OS-detected -> default) are shared with main.c.
 * The navbar only owns the click-driven *switch* path; persistence is
 * delegated to app_set_locale. */

/* -----------------------------------------------------------------------
 * Locale switch: three independent steps -- apply UI state (root class +
 * button active), persist to settings, navigate the router.  The
 * on_click handler orchestrates them via navbar_set_locale.
 * --------------------------------------------------------------------- */

static void navbar_apply_locale(ui_widget_t *w, const char *locale)
{
        navbar_t *that = navbar_get(w);
        ui_widget_t *buttons[2] = {
                that->base.refs.locale_en,
                that->base.refs.locale_zh,
        };
        const char *button_locales[2] = { "en", "zh-CN" };

        if (that->current_locale) {
                ui_widget_remove_class(ui_root(), that->current_locale);
        }
        ui_widget_add_class(ui_root(), locale);
        for (int i = 0; i < 2; ++i) {
                if (!buttons[i]) {
                        continue;
                }
                if (strcmp(button_locales[i], locale) == 0) {
                        ui_widget_add_class(buttons[i], "active");
                } else {
                        ui_widget_remove_class(buttons[i], "active");
                }
        }
}

static void navbar_save_locale(const char *locale)
{
        app_set_locale(locale);
}

static void navbar_navigate_to(ui_widget_t *w, const char *locale)
{
        navbar_t *that = navbar_get(w);
        const router_route_t *current;
        const char *full_path;
        const char *suffix;
        char new_path[256];
        router_location_t *location;

        if (!that->router) {
                return;
        }
        current = router_get_current_route(that->router);
        if (!current) {
                return;
        }
        full_path = router_route_get_full_path(current);
        if (!full_path || full_path[0] != '/') {
                return;
        }
        suffix = strchr(full_path + 1, '/');
        if (suffix) {
                snprintf(new_path, sizeof(new_path), "/%s%s", locale, suffix);
        } else {
                snprintf(new_path, sizeof(new_path), "/%s/overview/quick-start",
                         locale);
        }
        location = router_location_create(NULL, new_path);
        router_replace(that->router, location);
        router_location_destroy(location);
}

static void navbar_set_locale(ui_widget_t *w, const char *locale)
{
        navbar_t *that = navbar_get(w);

        if (!locale || locale[0] == 0) {
                return;
        }
        if (that->current_locale && strcmp(that->current_locale, locale) == 0) {
                return;
        }
        navbar_apply_locale(w, locale);
        navbar_save_locale(locale);
        navbar_navigate_to(w, locale);
        free(that->current_locale);
        that->current_locale = y_strdup(locale);
}

/* -----------------------------------------------------------------------
 * Click handler wired up by the TSX.  `arg` is the clicked button; we
 * read its data-locale attribute and forward to navbar_set_locale.
 * --------------------------------------------------------------------- */

static void navbar_on_locale_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *locale = ui_widget_get_attr(w, "data-locale");
        if (!locale) {
                return;
        }
        navbar_set_locale(w->parent->parent->parent, locale);
}

/* -----------------------------------------------------------------------
 * Route tracking: keep the path label and window title in sync with the
 * current router location.
 * --------------------------------------------------------------------- */

static void navbar_apply_route(ui_widget_t *w, const char *path)
{
        navbar_t *that = navbar_get(w);
        const wchar_t *title;
        wchar_t buf[256];

        if (path && that->base.refs.path) {
                ui_text_set_content(that->base.refs.path, path);
        }
        title = route_title_for(path);
        if (title && *title) {
                swprintf(buf, 256, L"%ls \u00b7 LCUI", title);
        } else {
                wcscpy(buf, L"LCUI");
        }
        ui_widget_set_title(ui_root(), buf);
}

static void navbar_on_route_changed(void *data, const router_route_t *to,
                                    const router_route_t *from)
{
        ui_widget_t *w = data;
        (void)from;
        if (!to) {
                return;
        }
        navbar_apply_route(w, router_route_get_full_path(to));
}

static void navbar_init(ui_widget_t *w)
{
        navbar_t *that;
        const router_route_t *current;
        const char *locale;

        ui_widget_add_data(w, navbar_proto, sizeof(navbar_t));
        navbar_react_init(w);
        that = navbar_get(w);
        that->current_locale = NULL;

        /* lcui-cli only emits a click binding for the *first* ref that
         * uses a given onClick name.  The EN button already has its
         * binding wired up by navbar_load_template; mirror it onto the
         * zh-CN button so both share the same click handler.  The
         * handler reads data-locale off the clicked widget, so both
         * paths flow through navbar_set_locale. */
        if (that->base.refs.locale_zh) {
                ui_widget_on(that->base.refs.locale_zh, "click",
                             navbar_on_locale_click, w);
        }

        that->router = router_get_by_name("AppRouter");
        if (that->router) {
                that->watcher =
                    router_watch(that->router, navbar_on_route_changed, w);
                current = router_get_current_route(that->router);
                if (current) {
                        navbar_apply_route(w,
                                           router_route_get_full_path(current));
                }
        }
        locale = app_get_locale();
        that->current_locale = y_strdup(locale);
        navbar_apply_locale(w, locale);
        navbar_update(w);
}

static void navbar_destroy(ui_widget_t *w)
{
        navbar_t *that = navbar_get(w);
        if (that && that->router && that->watcher) {
                router_unwatch(that->router, that->watcher);
                that->watcher = NULL;
        }
        if (that && that->current_locale) {
                free(that->current_locale);
                that->current_locale = NULL;
        }
        navbar_react_destroy(w);
}

void navbar_update(ui_widget_t *w)
{
        navbar_react_update(w);
}

ui_widget_t *ui_create_navbar(void)
{
        return ui_create_widget_with_prototype(navbar_proto);
}

void ui_register_navbar(void)
{
        navbar_init_prototype();
        navbar_proto->init = navbar_init;
        navbar_proto->destroy = navbar_destroy;
}
