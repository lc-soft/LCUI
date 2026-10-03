#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <LCUI.h>
#include "icon-browser.tsx.h"
#include "icon-browser.h"

typedef struct {
        icon_browser_react_t base;
        char *search;
        char *selected_name;
        char *selected_component;
        char *selected_class;
        char *selected_size_class;
        bool selected_filled_only;
        int selected_size;
        int search_timer_id;
} icon_browser_t;

static icon_browser_t *icon_browser_get(ui_widget_t *w)
{
        while (w && !ui_widget_has_class(w, "icon-library-browser")) {
                w = w->parent;
        }
        return w ? ui_widget_get_data(w, icon_browser_proto) : NULL;
}

static bool icon_browser_name_matches(const char *name, const char *query)
{
        const unsigned char *name_char;
        const unsigned char *query_char;

        if (!query || !query[0]) {
                return true;
        }
        for (; *name; ++name) {
                name_char = (const unsigned char *)name;
                query_char = (const unsigned char *)query;
                while (*name_char && *query_char &&
                       tolower(*name_char) == tolower(*query_char)) {
                        ++name_char;
                        ++query_char;
                }
                if (!*query_char) {
                        return true;
                }
        }
        return false;
}

static void icon_browser_apply_search(void *arg)
{
        icon_browser_t *that = arg;
        ui_widget_t *item;
        const char *name;
        size_t i;

        that->search_timer_id = 0;
        for (i = 0; (item = ui_widget_get_child(that->base.refs.icon_list, i));
             ++i) {
                name = ui_widget_get_attr(item, "name");
                ui_widget_set_style_string(
                    item, "display",
                    icon_browser_name_matches(name, that->search) ? "flex"
                                                                  : "none");
        }
}

static void icon_browser_on_search_change(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        icon_browser_t *that = icon_browser_get(w);
        wchar_t wide_search[256] = { 0 };
        char search[1024] = { 0 };
        size_t wide_length;

        wide_length = ui_textinput_get_text_w(w, 0, 255, wide_search);
        wide_search[wide_length] = L'\0';
        encode_utf8(search, wide_search, sizeof(search));
        free(that->search);
        that->search = y_strdup(search);
        if (that->search_timer_id) {
                ptk_clear_timeout(that->search_timer_id);
        }
        that->search_timer_id =
            ptk_set_timeout(1000, icon_browser_apply_search, that);
}

static void icon_browser_on_select(ui_widget_t *w, ui_event_t *e, void *arg)
{
        icon_browser_t *that = icon_browser_get(w);
        const char *name = ui_widget_get_attr(w, "name");
        const char *component = ui_widget_get_attr(w, "data-component-name");
        const char *filled_only = ui_widget_get_attr(w, "data-filled-only");
        const char *preview_size = ui_widget_get_attr(w, "data-preview-size");
        char class_name[256];
        char size_class[64];

        free(that->selected_name);
        that->selected_name = y_strdup(name);
        free(that->selected_component);
        that->selected_component = y_strdup(component);
        that->selected_filled_only =
            filled_only && strcmp(filled_only, "true") == 0;
        that->selected_size = preview_size ? atoi(preview_size) : 24;
        ui_text_set_content(that->base.refs.detail_name, name);
        if (that->selected_class) {
                ui_widget_remove_class(that->base.refs.detail_icon,
                                       that->selected_class);
                free(that->selected_class);
        }
        if (that->selected_size_class) {
                ui_widget_remove_class(that->base.refs.detail_icon,
                                       that->selected_size_class);
                free(that->selected_size_class);
                that->selected_size_class = NULL;
        }
        ui_widget_remove_class(that->base.refs.detail_icon, "fui-icon-filled");
        snprintf(class_name, sizeof(class_name), "fui-icon-%s", name);
        that->selected_class = y_strdup(class_name);
        ui_widget_add_class(that->base.refs.detail_icon, that->selected_class);
        if (that->selected_filled_only) {
                ui_widget_add_class(that->base.refs.detail_icon,
                                    "fui-icon-filled");
        }
        if (that->selected_size != 24) {
                snprintf(size_class, sizeof(size_class), "fui-icon-glyph-%d",
                         that->selected_size);
                that->selected_size_class = y_strdup(size_class);
                ui_widget_add_class(that->base.refs.detail_icon,
                                    that->selected_size_class);
        }
        ui_widget_set_style_string(that->base.refs.detail, "display", "flex");
}

static void icon_browser_on_close(ui_widget_t *w, ui_event_t *e, void *arg)
{
        icon_browser_t *that = icon_browser_get(w);
        ui_widget_set_style_string(that->base.refs.detail, "display", "none");
}

static void icon_browser_copy_text(const char *text)
{
        size_t text_length = strlen(text);
        wchar_t *wide_text = malloc(sizeof(wchar_t) * (text_length + 1));
        size_t wide_length = decode_utf8(wide_text, text, text_length + 1);

        ptk_clipboard_set_text(wide_text, wide_length);
        free(wide_text);
}

static void icon_browser_on_copy(ui_widget_t *w, ui_event_t *e, void *arg)
{
        icon_browser_t *that = icon_browser_get(w);
        const char *format = ui_widget_get_attr(w, "data-copy-format");
        char snippet[1024];

        if (!that->selected_name) {
                return;
        }
        if (strcmp(format, "c") == 0) {
                snprintf(snippet, sizeof(snippet),
                         "ui_widget_t *icon = ui_create_fluent_icon(\"%s\");",
                         that->selected_name);
                if (that->selected_filled_only) {
                        snprintf(snippet + strlen(snippet),
                                 sizeof(snippet) - strlen(snippet),
                                 "\nui_icon_set_filled(icon, true);");
                }
                if (that->selected_size != 24) {
                        snprintf(snippet + strlen(snippet),
                                 sizeof(snippet) - strlen(snippet),
                                 "\nui_icon_set_size(icon, %d);",
                                 that->selected_size);
                }
                snprintf(snippet + strlen(snippet),
                         sizeof(snippet) - strlen(snippet),
                         "\nui_widget_append(parent, icon);");
        } else if (strcmp(format, "jsx") == 0) {
                if (that->selected_filled_only && that->selected_size != 24) {
                        snprintf(
                            snippet, sizeof(snippet),
                            "import { %s } from \"@lcui/fluent-icons\";\n\n"
                            "<%s size={%d} fontSize={24} />",
                            that->selected_component, that->selected_component,
                            that->selected_size);
                } else {
                        snprintf(
                            snippet, sizeof(snippet),
                            "import { %s } from \"@lcui/fluent-icons\";\n\n"
                            "<%s />",
                            that->selected_component, that->selected_component);
                }
        } else {
                if (that->selected_filled_only && that->selected_size != 24) {
                        snprintf(snippet, sizeof(snippet),
                                 "<icon name=\"%s\" "
                                 "filled=\"true\" size=\"%d\" />",
                                 that->selected_name, that->selected_size);
                } else if (that->selected_filled_only) {
                        snprintf(snippet, sizeof(snippet),
                                 "<icon name=\"%s\" "
                                 "filled=\"true\" />",
                                 that->selected_name);
                } else {
                        snprintf(snippet, sizeof(snippet),
                                 "<icon name=\"%s\" />", that->selected_name);
                }
        }
        icon_browser_copy_text(snippet);
}

static void icon_browser_update_locale(icon_browser_t *that, const char *locale)
{
        bool is_english = locale && strcmp(locale, "en") == 0;

        ui_textinput_set_placeholder(
            that->base.refs.search,
            is_english ? "Search icons..." : "搜索图标名称...");
        ui_text_set_content(that->base.refs.copy_c,
                            is_english ? "Copy C code" : "复制 C 代码");
        ui_text_set_content(that->base.refs.copy_jsx,
                            is_english ? "Copy JSX" : "复制 JSX");
        ui_text_set_content(that->base.refs.copy_xml,
                            is_english ? "Copy XML" : "复制 XML");
}

static void icon_browser_on_set_attr(ui_widget_t *w, const char *name,
                                     const char *value)
{
        icon_browser_t *that = ui_widget_get_data(w, icon_browser_proto);

        if (strcmp(name, "locale") == 0) {
                icon_browser_update_locale(that, value);
        }
}

static void icon_browser_init(ui_widget_t *w)
{
        icon_browser_t *that =
            ui_widget_add_data(w, icon_browser_proto, sizeof(icon_browser_t));

        icon_browser_react_init(w);
        that->search = y_strdup("");
        that->selected_name = NULL;
        that->selected_component = NULL;
        that->selected_class = y_strdup("fui-icon-checkmark");
        that->selected_size_class = NULL;
        that->selected_filled_only = false;
        that->selected_size = 24;
        that->search_timer_id = 0;
        ui_widget_set_style_string(that->base.refs.detail, "display", "none");
}

static void icon_browser_destroy(ui_widget_t *w)
{
        icon_browser_t *that = ui_widget_get_data(w, icon_browser_proto);

        if (that->search_timer_id) {
                ptk_clear_timeout(that->search_timer_id);
        }
        free(that->search);
        free(that->selected_name);
        free(that->selected_component);
        free(that->selected_class);
        free(that->selected_size_class);
        icon_browser_react_destroy(w);
}

ui_widget_t *ui_create_icon_browser(void)
{
        return ui_create_widget_with_prototype(icon_browser_proto);
}

void ui_register_icon_browser(void)
{
        icon_browser_init_prototype();
        icon_browser_proto->init = icon_browser_init;
        icon_browser_proto->destroy = icon_browser_destroy;
        icon_browser_proto->setattr = icon_browser_on_set_attr;
}
