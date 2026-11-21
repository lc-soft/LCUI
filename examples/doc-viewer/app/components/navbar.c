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
#include "../theme.h"

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

        if (that->current_locale) {
                ui_widget_remove_class(ui_root(), that->current_locale);
        }
        ui_widget_add_class(ui_root(), locale);
        if (that->base.refs.lang_group) {
                ui_widget_set_attr(that->base.refs.lang_group, "value", locale);
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
                snprintf(new_path, sizeof(new_path),
                         "/%s/docs/overview/quick-start", locale);
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

static ui_widget_t *navbar_owner(ui_widget_t *w)
{
        return ui_widget_get_closest(w, "navbar");
}

static void navbar_on_locale_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *item = arg;
        ui_widget_t *navbar = navbar_owner(w);
        const char *locale = item ? ui_widget_get_attr(item, "value") : NULL;

        if (!locale || !navbar) {
                return;
        }
        navbar_set_locale(navbar, locale);
}

static void navbar_on_theme_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        if (strcmp(app_get_theme(), "dark") == 0) {
                app_set_theme("light");
        } else {
                app_set_theme("dark");
        }
        app_apply_theme();
}

static ui_widget_t *navbar_find_by_class(ui_widget_t *w, const char *class_name)
{
        list_node_t *node;
        ui_widget_t *child;
        ui_widget_t *found;

        if (ui_widget_has_class(w, class_name)) {
                return w;
        }
        list_for_each(node, &w->children)
        {
                child = node->data;
                found = navbar_find_by_class(child, class_name);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static void navbar_sync_accent(void)
{
        ui_widget_t *palette =
            navbar_find_by_class(ui_root(), "accent-palette");
        ui_widget_t *item;
        const char *current = app_get_accent();
        bool selected;

        if (!palette) {
                return;
        }
        for (item = ui_widget_get_child(palette, 0); item;
             item = ui_widget_next(item)) {
                const char *name = ui_widget_get_attr(item, "data-accent");

                selected = name && strcmp(name, current) == 0;
                if (selected && !ui_widget_has_class(item, "active")) {
                        ui_widget_add_class(item, "active");
                } else if (!selected && ui_widget_has_class(item, "active")) {
                        ui_widget_remove_class(item, "active");
                }
        }
}

/* The dropdown content only becomes reachable from the root after the
 * first update, so the initial selection is applied once the navbar has
 * been laid out instead of during init. */
static void navbar_on_afterlayout(ui_widget_t *w, ui_event_t *e, void *arg)
{
        navbar_sync_accent();
}

static void navbar_on_accent_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *accent = ui_widget_get_attr(w, "data-accent");

        if (!accent) {
                return;
        }
        app_set_accent(accent);
        app_apply_accent();
        navbar_sync_accent();
}

static void navbar_replace(ui_widget_t *navbar, const char *path)
{
        navbar_t *that = navbar_get(navbar);
        router_location_t *location;

        if (!that || !that->router) {
                return;
        }
        location = router_location_create(NULL, path);
        router_replace(that->router, location);
        router_location_destroy(location);
}

static void navbar_on_nav_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *navbar = arg ? arg : navbar_owner(w);
        navbar_t *that;
        const char *nav = ui_widget_get_attr(w, "data-nav");
        const char *locale;
        char path[160];

        if (!nav || !navbar) {
                return;
        }
        that = navbar_get(navbar);
        locale = that && that->current_locale ? that->current_locale
                                              : app_get_locale();
        if (strcmp(nav, "home") == 0) {
                snprintf(path, sizeof(path), "/%s/home", locale);
        } else if (strcmp(nav, "docs") == 0) {
                snprintf(path, sizeof(path), "/%s/docs/overview/quick-start",
                         locale);
        } else if (strcmp(nav, "widgets") == 0) {
                snprintf(path, sizeof(path), "/%s/docs/widgets", locale);
        } else {
                return;
        }
        navbar_replace(navbar, path);
}

/* -----------------------------------------------------------------------
 * Route tracking: keep the path label and window title in sync with the
 * current router location.
 * --------------------------------------------------------------------- */

/* The docs are generated per locale, but the sidebar and other shared
 * widgets render both languages and rely on the root locale class.  Keep
 * that class in sync with the locale segment of the current route so a
 * direct visit to /zh-CN/... or /en/... shows the matching language. */
static void navbar_sync_locale_from_path(ui_widget_t *w, const char *path)
{
        navbar_t *that = navbar_get(w);
        const char *start;
        const char *slash;
        char locale[16];
        size_t length;

        if (!path || path[0] != '/') {
                return;
        }
        start = path + 1;
        slash = strchr(start, '/');
        if (!slash) {
                return;
        }
        length = (size_t)(slash - start);
        if (length == 0 || length >= sizeof(locale)) {
                return;
        }
        memcpy(locale, start, length);
        locale[length] = 0;
        if (strcmp(locale, "en") != 0 && strcmp(locale, "zh-CN") != 0) {
                return;
        }
        navbar_apply_locale(w, locale);
        free(that->current_locale);
        that->current_locale = y_strdup(locale);
}

static void navbar_apply_route(ui_widget_t *w, const char *path)
{
        navbar_t *that = navbar_get(w);
        const wchar_t *title;
        const char *last_segment;
        wchar_t buf[256];

        navbar_sync_locale_from_path(w, path);
        if (path && that->base.refs.path) {
                ui_text_set_content(that->base.refs.path, path);
        }
        last_segment = path ? strrchr(path, '/') : NULL;
        if (last_segment && strcmp(last_segment, "/home") == 0) {
                /* Mirror the product page titles used by documentation sites:
                 * the home title is the product name plus its tagline. */
                wcscpy(buf,
                       L"LCUI - The C library for building user interfaces");
        } else {
                title = route_title_for(path);
                if (title && *title) {
                        swprintf(buf, 256, L"%ls \u00b7 LCUI", title);
                } else {
                        wcscpy(buf, L"LCUI");
                }
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

        /* lcui-cli binds every ref that declares an onClick handler, so the
         * nav buttons and theme button are already wired. The language menu
         * reports selection through a radio-group value-change event. */
        if (that->base.refs.locale_menu) {
                ui_widget_on(that->base.refs.locale_menu, "value-change",
                             navbar_on_locale_change, w);
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
        ui_widget_on(w, "afterlayout", navbar_on_afterlayout, NULL);
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

static void navbar_on_resize(ui_widget_t *w, float width, float height)
{
        if (width < 700.0f) {
                ui_widget_add_class(w, "compact");
        } else {
                ui_widget_remove_class(w, "compact");
        }
}

void ui_register_navbar(void)
{
        navbar_init_prototype();
        navbar_proto->init = navbar_init;
        navbar_proto->destroy = navbar_destroy;
        navbar_proto->resize = navbar_on_resize;
}
