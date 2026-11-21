/*
 * tests/integration/test_breadcrumb.c: -- Breadcrumb widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static int count_children(ui_widget_t *w)
{
        int count = 0;

        while (ui_widget_get_child(w, count)) {
                count++;
        }
        return count;
}

static int count_items(ui_widget_t *list)
{
        ui_widget_t *item;
        size_t i;
        int count = 0;

        for (i = 0; (item = ui_widget_get_child(list, i)); ++i) {
                if (ui_widget_get_attr(item, "data-key") ||
                    ui_widget_get_attr(item, "data-path")) {
                        count++;
                }
        }
        return count;
}

static ui_widget_t *find_type(ui_widget_t *w, const char *type)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        if (!w) {
                return NULL;
        }
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (strcmp(child->type, type) == 0) {
                        return child;
                }
                found = find_type(child, type);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static const char *breadcrumb_test_css = ".bc-wide { width: 2000px; }"
                                         ".bc-narrow { width: 500px; }"
                                         ".bc-narrower { width: 200px; }"
                                         ".bc-tiny { width: 120px; }"
                                         ".bc-medium { width: 700px; }";

static const char *long_label(const char *segment, const char *path, void *arg)
{
        return "Very Long Breadcrumb Label";
}

static void set_test_width(ui_widget_t *breadcrumb, const char *cls)
{
        ui_widget_remove_class(breadcrumb, "bc-wide");
        ui_widget_remove_class(breadcrumb, "bc-narrow");
        ui_widget_remove_class(breadcrumb, "bc-narrower");
        ui_widget_remove_class(breadcrumb, "bc-tiny");
        ui_widget_remove_class(breadcrumb, "bc-medium");
        ui_widget_add_class(breadcrumb, cls);
        ui_update();
        ui_update();
}

static const char *changed_key;
static int change_count;

static void on_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        changed_key = arg;
        change_count++;
}

static void emit_click(ui_widget_t *target)
{
        ui_event_t e = { 0 };

        ui_event_init(&e, "click");
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static ui_widget_t *find_item(ui_widget_t *list, const char *key)
{
        ui_widget_t *item;
        const char *item_key;
        size_t i;

        for (i = 0; (item = ui_widget_get_child(list, i)); ++i) {
                item_key = ui_widget_get_attr(item, "data-key");
                if (!item_key) {
                        item_key = ui_widget_get_attr(item, "data-path");
                }
                if (item_key && strcmp(item_key, key) == 0) {
                        return item;
                }
        }
        return NULL;
}

static void test_family_factories(void)
{
        ui_widget_t *breadcrumb;
        ui_widget_t *list;
        ui_widget_t *item;
        ui_widget_t *link;
        ui_widget_t *page;
        ui_widget_t *separator;
        ui_widget_t *ellipsis;

        breadcrumb = ui_create_breadcrumb();
        list = ui_create_breadcrumb_list();
        item = ui_create_breadcrumb_item();
        link = ui_create_breadcrumb_link();
        page = ui_create_breadcrumb_page();
        separator = ui_create_breadcrumb_separator();
        ellipsis = ui_create_breadcrumb_ellipsis();

        ctest_equal_str("breadcrumb type", breadcrumb->type, "breadcrumb");
        ctest_equal_int("default list child count", count_children(breadcrumb),
                        1);
        ctest_equal_bool("default list exists",
                         find_type(breadcrumb, "breadcrumb-list") != NULL,
                         true);
        ctest_equal_str("list type", list->type, "breadcrumb-list");
        ctest_equal_str("item type", item->type, "breadcrumb-item");
        ctest_equal_str("link type", link->type, "breadcrumb-link");
        ctest_equal_str("page type", page->type, "breadcrumb-page");
        ctest_equal_str("separator type", separator->type,
                        "breadcrumb-separator");
        ctest_equal_str("ellipsis type", ellipsis->type, "breadcrumb-ellipsis");

        ui_widget_destroy(breadcrumb);
        ui_widget_destroy(list);
        ui_widget_destroy(item);
        ui_widget_destroy(link);
        ui_widget_destroy(page);
        ui_widget_destroy(separator);
        ui_widget_destroy(ellipsis);
        ui_clear_trash();
}

static const char *capture_segments[8];
static const char *capture_paths[8];
static char capture_path_storage[8][64];
static int capture_count;

static const char *label_provider(const char *segment, const char *path,
                                  void *arg)
{
        capture_segments[capture_count] = segment;
        strncpy(capture_path_storage[capture_count], path, 63);
        capture_path_storage[capture_count][63] = '\0';
        capture_paths[capture_count] = capture_path_storage[capture_count];
        capture_count++;
        return arg;
}

static void test_path_mode(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *list;

        capture_count = 0;
        ui_breadcrumb_build_from_path(breadcrumb, "/a/b/c", label_provider,
                                      "Label");
        ui_update();
        list = ui_widget_get_child(breadcrumb, 0);

        ctest_equal_int("three path nodes", count_items(list), 3);
        ctest_equal_int("provider called per node", capture_count, 3);
        ctest_equal_str("first node path", capture_paths[0], "/a");
        ctest_equal_str("second node path", capture_paths[1], "/a/b");
        ctest_equal_str("third node path", capture_paths[2], "/a/b/c");
        ctest_equal_str("last key is segment",
                        ui_breadcrumb_get_key(breadcrumb), "c");
        ctest_equal_str(
            "provider label is used",
            ui_widget_get_attr(ui_widget_get_child(list, 0), "data-label"),
            "Label");

        capture_count = 0;
        ui_breadcrumb_build_from_path(breadcrumb, "/", NULL, NULL);
        ui_update();
        list = ui_widget_get_child(breadcrumb, 0);
        ctest_equal_int("root path has one node", count_items(list), 1);
        ctest_equal_str("root key", ui_breadcrumb_get_key(breadcrumb), "/");

        ui_breadcrumb_build_from_path(breadcrumb, "", NULL, NULL);
        ui_update();
        ctest_equal_int("empty path clears nodes",
                        count_items(ui_widget_get_child(breadcrumb, 0)), 0);

        ui_breadcrumb_build_from_path(breadcrumb, "//a//b/", NULL, NULL);
        ui_update();
        list = ui_widget_get_child(breadcrumb, 0);
        ctest_equal_int("repeated slashes are ignored", count_items(list), 2);
        ctest_equal_str("relative key", ui_breadcrumb_get_key(breadcrumb), "b");

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static void test_append_custom(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *content = ui_create_button();
        ui_widget_t *list;
        ui_widget_t *item;

        ui_breadcrumb_append_item(breadcrumb, "a", "Alpha");
        ui_widget_set_attr(content, "data-key", "custom");
        ui_widget_set_attr(content, "data-label", "Custom Node");
        ui_breadcrumb_append(breadcrumb, content);

        list = ui_widget_get_child(breadcrumb, 0);
        item = find_item(list, "custom");
        ctest_equal_str("custom item wrapper", item->type, "breadcrumb-item");
        ctest_equal_str("custom key", ui_widget_get_attr(item, "data-key"),
                        "custom");
        ctest_equal_str("custom label", ui_widget_get_attr(item, "data-label"),
                        "Custom Node");

        change_count = 0;
        emit_click(content);
        ctest_equal_int("custom content does not emit change", change_count, 0);

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static int count_collapsed(ui_widget_t *list)
{
        ui_widget_t *item;
        size_t i;
        int count = 0;

        for (i = 0; (item = ui_widget_get_child(list, i)); ++i) {
                if (ui_widget_has_status(item, "collapsed")) {
                        count++;
                }
        }
        return count;
}

static void test_collapse(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *list;
        ui_widget_t *item;

        ui_breadcrumb_build_from_path(breadcrumb, "/a/b/c/d/e", long_label,
                                      NULL);
        ui_widget_append(ui_root(), breadcrumb);
        ctest_equal_int("default collapse-from",
                        ui_breadcrumb_get_collapse_from(breadcrumb), 1);
        ui_breadcrumb_set_collapse_from(breadcrumb, 2);
        ctest_equal_int("collapse-from setter stores value",
                        ui_breadcrumb_get_collapse_from(breadcrumb), 2);
        ui_breadcrumb_set_collapse_from(breadcrumb, 1);
        set_test_width(breadcrumb, "bc-wide");
        list = ui_widget_get_child(breadcrumb, 0);
        ctest_equal_int("wide keeps all items", count_items(list), 5);

        set_test_width(breadcrumb, "bc-narrow");
        item = find_item(list, "c");
        ctest_equal_bool("middle item collapsed",
                         ui_widget_has_status(item, "collapsed"), true);
        item = find_item(list, "e");
        ctest_equal_bool("last item never collapsed",
                         ui_widget_has_status(item, "collapsed"), false);

        set_test_width(breadcrumb, "bc-medium");
        {
                ui_widget_t *ellipsis =
                    find_type(breadcrumb, "breadcrumb-ellipsis");
                ui_widget_t *menu = ellipsis ? ellipsis->parent : NULL;
                ui_widget_t *portal = find_type(menu, "portal");
                ui_widget_t *content =
                    portal ? ui_portal_get_content(portal) : NULL;
                int collapsed = count_collapsed(list);

                ctest_equal_int("medium keeps trailing items", collapsed, 2);
                emit_click(ellipsis);
                ui_update();
                ctest_equal_int("ellipsis lists every hidden node",
                                content ? count_children(content) : -1,
                                collapsed);
                if (menu) {
                        ui_dropdown_menu_set_open(menu, false);
                }
        }

        set_test_width(breadcrumb, "bc-narrower");
        item = find_item(list, "b");
        ctest_equal_bool("collapse extends leftward",
                         ui_widget_has_status(item, "collapsed"), true);
        item = find_item(list, "a");
        ctest_equal_bool("first item may collapse",
                         ui_widget_has_status(item, "collapsed"), true);

        set_test_width(breadcrumb, "bc-wide");
        ctest_equal_bool(
            "widening expands items",
            ui_widget_has_status(find_item(list, "c"), "collapsed"), false);

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static void test_explicit_mode(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *list;
        ui_widget_t *item;
        ui_widget_t *link;
        ui_widget_t *text;
        ui_widget_t *ellipsis;
        ui_widget_t *menu;
        ui_widget_t *portal;
        ui_widget_t *content;
        ui_widget_t *keyless;
        ui_widget_t *child;
        size_t i;

        change_count = 0;
        changed_key = NULL;
        ui_widget_on(breadcrumb, "change", on_change, NULL);

        item = ui_create_breadcrumb_item();
        ui_widget_set_attr(item, "data-path", "/explicit");
        link = ui_create_breadcrumb_link();
        text = ui_create_widget("text");
        ui_text_set_content(text, "Explicit");
        ui_widget_append(link, text);
        ui_widget_append(item, link);
        ui_widget_append(item, ui_create_breadcrumb_separator());
        ui_widget_append(breadcrumb, item);

        item = ui_create_breadcrumb_item();
        ui_widget_set_attr(item, "data-path", "/explicit/tail");
        ui_widget_append(item, ui_create_breadcrumb_page());
        ui_widget_append(breadcrumb, item);

        ui_widget_append(ui_root(), breadcrumb);
        ui_breadcrumb_set_separator(breadcrumb, "circle");
        ui_update();

        list = ui_widget_get_child(breadcrumb, 0);
        ctest_equal_str("explicit list stays first", list->type,
                        "breadcrumb-list");
        ctest_equal_int("explicit item count", count_items(list), 2);
        ctest_equal_bool("explicit items moved into list",
                         ui_widget_get_child(breadcrumb, 1) == NULL, true);
        {
                ui_widget_t *separator = find_type(find_item(list, "/explicit"),
                                                   "breadcrumb-separator");
                ui_widget_t *icon =
                    separator ? ui_widget_get_child(separator, 0) : NULL;

                ctest_equal_bool(
                    "explicit separator themed",
                    icon && ui_widget_has_class(icon, "fui-icon-circle"), true);
        }

        link = find_type(find_item(list, "/explicit"), "breadcrumb-link");
        ctest_equal_bool("explicit link exists", link != NULL, true);
        if (link) {
                emit_click(link);
        }
        ctest_equal_int("explicit link emits", change_count, 1);
        ctest_equal_str("explicit key", changed_key, "/explicit");
        ctest_equal_str("explicit last key", ui_breadcrumb_get_key(breadcrumb),
                        "/explicit/tail");

        item = ui_create_breadcrumb_item();
        link = ui_create_breadcrumb_link();
        text = ui_create_widget("text");
        ui_text_set_content(text, "Very Long Unknown Node");
        ui_widget_append(link, text);
        ui_widget_append(item, link);
        ui_widget_append(item, ui_create_breadcrumb_separator());
        ui_widget_append(breadcrumb, item);

        {
                ui_widget_t *last = ui_create_breadcrumb_item();
                ui_widget_t *page = ui_create_breadcrumb_page();
                ui_widget_t *label = ui_create_widget("text");

                ui_widget_set_attr(last, "data-path", "/explicit/end");
                ui_text_set_content(label, "Very Long Final Page");
                ui_widget_append(page, label);
                ui_widget_append(last, page);
                ui_widget_append(breadcrumb, last);
        }
        ui_update();

        ctest_equal_bool("keyless link exists", link != NULL, true);
        ctest_equal_bool("keyless item moved into list",
                         ui_widget_get_child(breadcrumb, 1) == NULL, true);
        if (link) {
                emit_click(link);
        }
        ctest_equal_int("keyless link does not emit", change_count, 1);
        ctest_equal_str("keyless link keeps payload", changed_key, "/explicit");

        set_test_width(breadcrumb, "bc-tiny");
        list = ui_widget_get_child(breadcrumb, 0);
        ctest_equal_bool("keyless item collapses",
                         ui_widget_has_status(item, "collapsed"), true);
        ellipsis = find_type(breadcrumb, "breadcrumb-ellipsis");
        menu = ellipsis ? ellipsis->parent : NULL;
        portal = find_type(menu, "portal");
        content = portal ? ui_portal_get_content(portal) : NULL;
        emit_click(ellipsis);
        ui_update();
        ctest_equal_bool("ellipsis menu exists", content != NULL, true);
        if (content) {
                keyless = NULL;
                for (i = 0; (child = ui_widget_get_child(content, i)); ++i) {
                        if (!ui_widget_get_attr(child, "data-key")) {
                                keyless = child;
                                break;
                        }
                }
                ctest_equal_bool("keyless menu item exists", keyless != NULL,
                                 true);
                if (keyless) {
                        ctest_equal_bool("keyless menu item disabled",
                                         keyless->disabled, true);
                }
        }

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static void test_ellipsis_menu(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *ellipsis;
        ui_widget_t *menu;
        ui_widget_t *portal;
        ui_widget_t *content;
        ui_widget_t *item;

        change_count = 0;
        changed_key = NULL;
        ui_widget_on(breadcrumb, "change", on_change, NULL);
        ui_breadcrumb_build_from_path(breadcrumb, "/a/b/c/d/e", long_label,
                                      NULL);
        ui_widget_append(ui_root(), breadcrumb);
        set_test_width(breadcrumb, "bc-narrow");

        ellipsis = find_type(breadcrumb, "breadcrumb-ellipsis");
        ctest_equal_bool("ellipsis exists", ellipsis != NULL, true);
        menu = ellipsis->parent;
        portal = find_type(menu, "portal");
        content = portal ? ui_portal_get_content(portal) : NULL;
        emit_click(ellipsis);
        ui_update();
        ctest_equal_bool("ellipsis menu opens", ui_dropdown_menu_is_open(menu),
                         true);
        ctest_equal_int("menu lists collapsed nodes", count_children(content),
                        3);
        item = find_item(content, "b");
        ctest_equal_bool("menu item exists", item != NULL, true);
        emit_click(item);
        ctest_equal_str("menu item key", changed_key, "b");
        ctest_equal_int("menu item emits once", change_count, 1);
        ctest_equal_bool("menu closes after selection",
                         ui_dropdown_menu_is_open(menu), false);

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static void test_ellipsis_custom_label(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *content_widget = ui_create_button();
        ui_widget_t *ellipsis;
        ui_widget_t *menu;
        ui_widget_t *portal;
        ui_widget_t *dropdown_content;
        ui_widget_t *item;
        ui_widget_t *text;
        wchar_t wide[64];

        ui_widget_set_attr(content_widget, "data-key", "custom");
        ui_widget_set_attr(content_widget, "data-label", "Custom Node");
        ui_breadcrumb_append_item(breadcrumb, "a",
                                  "Very Long Breadcrumb Label");
        ui_breadcrumb_append(breadcrumb, content_widget);
        ui_breadcrumb_append_item(breadcrumb, "c",
                                  "Very Long Breadcrumb Label");
        ui_widget_append(ui_root(), breadcrumb);
        set_test_width(breadcrumb, "bc-narrower");

        ellipsis = find_type(breadcrumb, "breadcrumb-ellipsis");
        menu = ellipsis->parent;
        portal = find_type(menu, "portal");
        dropdown_content = portal ? ui_portal_get_content(portal) : NULL;
        emit_click(ellipsis);
        ui_update();
        item = find_item(dropdown_content, "custom");
        ctest_equal_bool("custom item is collapsed", item != NULL, true);
        text = find_type(item, "text");
        ui_text_get_content_w(text, wide, 64);
        ctest_equal_wcs("custom data-label used in menu", wide, L"Custom Node");

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static void test_change_event(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *list;
        ui_widget_t *link;

        change_count = 0;
        changed_key = NULL;
        ui_breadcrumb_append_item(breadcrumb, "a", "Alpha");
        ui_breadcrumb_append_item(breadcrumb, "b", "Beta");
        ui_breadcrumb_append_item(breadcrumb, "c", "Gamma");
        ui_widget_append(ui_root(), breadcrumb);
        ui_widget_on(breadcrumb, "change", on_change, NULL);
        ui_update();

        list = ui_widget_get_child(breadcrumb, 0);
        link = find_type(find_item(list, "a"), "breadcrumb-link");
        emit_click(link);
        ctest_equal_int("link click emits once", change_count, 1);
        ctest_equal_str("link key payload", changed_key, "a");

        emit_click(find_type(find_item(list, "c"), "breadcrumb-page"));
        ctest_equal_int("page click does not emit", change_count, 1);

        ui_widget_set_disabled(find_item(list, "a"), true);
        emit_click(link);
        ctest_equal_int("disabled item does not emit", change_count, 1);

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static void test_append_item(void)
{
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *list;
        ui_widget_t *item;
        ui_widget_t *separator;
        ui_widget_t *icon;

        ui_breadcrumb_append_item(breadcrumb, "a", "Alpha");
        ui_widget_append(ui_root(), breadcrumb);
        ui_update();

        list = ui_widget_get_child(breadcrumb, 0);
        ctest_equal_int("one item", count_items(list), 1);
        item = find_item(list, "a");
        ctest_equal_bool("first item is current page",
                         find_type(item, "breadcrumb-page") != NULL, true);

        ui_breadcrumb_append_item(breadcrumb, "b", "Beta");
        ui_update();
        ctest_equal_int("two items", count_items(list), 2);
        ctest_equal_bool(
            "previous page becomes link",
            find_type(find_item(list, "a"), "breadcrumb-link") != NULL, true);
        ctest_equal_bool(
            "new item is page",
            find_type(find_item(list, "b"), "breadcrumb-page") != NULL, true);
        ctest_equal_str("last key", ui_breadcrumb_get_key(breadcrumb), "b");

        separator = find_type(find_item(list, "a"), "breadcrumb-separator");
        ctest_equal_bool("previous item has a separator", separator != NULL,
                         true);
        icon = ui_widget_get_child(separator, 0);
        ctest_equal_bool("default separator icon",
                         ui_widget_has_class(icon, "fui-icon-chevron-right"),
                         true);

        ui_breadcrumb_set_separator(breadcrumb, "slash");
        ui_update();
        icon = ui_widget_get_child(separator, 0);
        ctest_equal_bool("custom separator icon",
                         ui_widget_has_class(icon, "fui-icon-slash"), true);

        ui_breadcrumb_append_item(breadcrumb, "plain", NULL);
        ui_update();
        ctest_equal_str(
            "NULL label falls back to key",
            ui_widget_get_attr(find_item(list, "plain"), "data-label"),
            "plain");

        ui_widget_destroy(breadcrumb);
        ui_clear_trash();
}

static ui_widget_t *demo_text(const char *content)
{
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, content);
        return text;
}

static ui_widget_t *demo_link_item(const char *path, const char *label)
{
        ui_widget_t *item = ui_create_breadcrumb_item();
        ui_widget_t *link = ui_create_breadcrumb_link();

        ui_widget_set_attr(item, "data-path", path);
        ui_widget_append(link, demo_text(label));
        ui_widget_append(item, link);
        ui_widget_append(item, ui_create_breadcrumb_separator());
        return item;
}

static ui_widget_t *demo_page_item(const char *path, const char *label)
{
        ui_widget_t *item = ui_create_breadcrumb_item();
        ui_widget_t *page = ui_create_breadcrumb_page();

        ui_widget_set_attr(item, "data-path", path);
        ui_widget_append(page, demo_text(label));
        ui_widget_append(item, page);
        return item;
}

static ui_widget_t *demo_menu_item(const char *label)
{
        ui_widget_t *item = ui_create_dropdown_menu_item();

        ui_widget_append(item, demo_text(label));
        return item;
}

static ui_widget_t *demo_ellipsis_item(void)
{
        ui_widget_t *item = ui_create_breadcrumb_item();
        ui_widget_t *menu = ui_create_dropdown_menu();
        ui_widget_t *trigger = ui_create_dropdown_menu_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *content = ui_create_dropdown_menu_content();
        ui_widget_t *group = ui_create_dropdown_menu_group();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_SM);
        ui_widget_append(button, ui_create_breadcrumb_ellipsis());
        ui_widget_append(trigger, button);
        ui_widget_append(group, demo_menu_item("Documentation"));
        ui_widget_append(group, demo_menu_item("Themes"));
        ui_widget_append(group, demo_menu_item("GitHub"));
        ui_widget_append(content, group);
        ui_widget_append(menu, trigger);
        ui_widget_append(menu, content);
        ui_widget_append(item, menu);
        ui_widget_append(item, ui_create_breadcrumb_separator());
        return item;
}

static void test_demo_layout(void)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *breadcrumb = ui_create_breadcrumb();
        ui_widget_t *list;
        char desc[128];

        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "align-items", "center");
        ui_widget_set_style_string(box, "width", "100%");
        ui_widget_append(breadcrumb, demo_link_item("/", "Home"));
        ui_widget_append(breadcrumb, demo_ellipsis_item());
        ui_widget_append(
            breadcrumb,
            demo_link_item("/Documentation/Themes/GitHub/Components",
                           "Components"));
        ui_widget_append(
            breadcrumb,
            demo_page_item("/Documentation/Themes/GitHub/Components/Breadcrumb",
                           "Breadcrumb"));
        ui_widget_append(box, breadcrumb);
        ui_widget_append(ui_root(), box);
        ui_update();
        ui_update();
        ui_update();

        list = ui_widget_get_child(breadcrumb, 0);
        snprintf(desc, sizeof(desc),
                 "demo breadcrumb should keep its full width "
                 "(width=%.1f, parent=%.1f)",
                 breadcrumb->border_box.width, box->border_box.width);
        ctest_equal_bool(
            desc,
            breadcrumb->border_box.width > 0.f &&
                breadcrumb->border_box.width <= box->border_box.width + 1.f,
            true);
        ctest_equal_bool(
            "demo breadcrumb list should fill the breadcrumb",
            fabsf(list->border_box.width - breadcrumb->border_box.width) <= 1.f,
            true);
        ctest_equal_bool("demo breadcrumb should keep all four items",
                         count_children(list) == 5, true);
        ctest_equal_bool("demo breadcrumb home should be visible",
                         ui_widget_get_child(list, 0)->border_box.width > 0.f,
                         true);
        ctest_equal_bool("demo breadcrumb ellipsis trigger should be visible",
                         ui_widget_get_child(list, 1)->border_box.width > 0.f,
                         true);
        ctest_equal_bool("demo breadcrumb components should be visible",
                         ui_widget_get_child(list, 2)->border_box.width > 0.f,
                         true);
        ctest_equal_bool(
            "demo built-in ellipsis should stay hidden",
            ui_widget_has_status(ui_widget_get_child(list, 3), "collapsed"),
            true);
        ctest_equal_bool("demo breadcrumb page should be visible",
                         ui_widget_get_child(list, 4)->border_box.width > 0.f,
                         true);

        change_count = 0;
        changed_key = NULL;
        ui_widget_on(breadcrumb, "change", on_change, NULL);
        emit_click(find_type(
            find_item(list, "/Documentation/Themes/GitHub/Components"),
            "breadcrumb-link"));
        ctest_equal_int("demo components click should emit once", change_count,
                        1);
        ctest_equal_str("demo components click should emit the full path",
                        changed_key, "/Documentation/Themes/GitHub/Components");

        ui_widget_destroy(box);
}

static void test_status_text_updates(void)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *status = ui_create_widget(NULL);
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *value = ui_create_widget("text");
        ui_widget_t *ref = ui_create_widget("text");
        float width0;

        ui_widget_set_style_string(box, "display", "flex");
        ui_widget_set_style_string(box, "flex-direction", "column");
        ui_widget_set_style_string(box, "align-items", "center");
        ui_widget_set_style_string(box, "width", "400px");
        ui_widget_set_style_string(status, "display", "flex");
        ui_widget_set_style_string(status, "align-items", "center");
        ui_widget_set_style_string(status, "gap", "6px");
        ui_text_set_content(label, "Selected:");
        ui_text_set_content(value, "-");
        ui_text_set_content(ref, "/components");

        ui_widget_append(status, label);
        ui_widget_append(status, value);
        ui_widget_append(box, status);
        ui_widget_append(ui_root(), box);
        ui_widget_append(ui_root(), ref);
        ui_update();
        width0 = status->border_box.width;

        ui_text_set_content(value, "/components");
        ui_update();
        ui_update();

        ctest_equal_bool("status text should expand with new content",
                         status->border_box.width > width0 + 20.f, true);
        ctest_equal_bool(
            "status text should stay on one line",
            value->content_box.height <= ref->content_box.height + 1.f, true);
        ctest_equal_bool("status text should not be clipped",
                         value->content_box.width > 50.f, true);

        ui_widget_destroy(box);
        ui_widget_destroy(ref);
}

void test_breadcrumb(void)
{
        lcui_init();
        ui_load_css_string(breadcrumb_test_css, "breadcrumb-test");
        ctest_describe("breadcrumb factories", test_family_factories);
        ctest_describe("breadcrumb append", test_append_item);
        ctest_describe("breadcrumb path", test_path_mode);
        ctest_describe("breadcrumb change", test_change_event);
        ctest_describe("breadcrumb custom append", test_append_custom);
        ctest_describe("breadcrumb collapse", test_collapse);
        ctest_describe("breadcrumb explicit", test_explicit_mode);
        ctest_describe("breadcrumb ellipsis", test_ellipsis_menu);
        ctest_describe("breadcrumb ellipsis label", test_ellipsis_custom_label);
        ctest_describe("breadcrumb demo layout", test_demo_layout);
        ctest_describe("breadcrumb status text update",
                       test_status_text_updates);
        lcui_destroy();
}
