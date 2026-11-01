/*
 * tests/integration/test_accordion.c: -- Accordion widget tests
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI.h>
#include <ctest-custom.h>

static void test_family_factories(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *item = ui_create_accordion_item();
        ui_widget_t *trigger = ui_create_accordion_trigger();
        ui_widget_t *content = ui_create_accordion_content();

        ctest_equal_str("accordion type", accordion->type, "accordion");
        ctest_equal_str("item type", item->type, "accordion-item");
        ctest_equal_str("trigger type", trigger->type, "accordion-trigger");
        ctest_equal_str("content type", content->type, "accordion-content");
        ctest_equal_bool("root defaults to single mode",
                         ui_accordion_get_multiple(accordion), false);
        ctest_equal_bool("item starts closed", ui_accordion_item_get_open(item),
                         false);
        ctest_equal_bool("item starts with closed status",
                         ui_widget_has_status(item, "closed"), true);

        ui_widget_destroy(accordion);
        ui_widget_destroy(item);
        ui_widget_destroy(trigger);
        ui_widget_destroy(content);
        ui_clear_trash();
}

static ui_widget_t *make_item(const char *value, ui_widget_t **trigger_out)
{
        ui_widget_t *item = ui_create_accordion_item();
        ui_widget_t *trigger = ui_create_accordion_trigger();
        ui_widget_t *content = ui_create_accordion_content();

        ui_accordion_item_set_value(item, value);
        ui_widget_append(item, trigger);
        ui_widget_append(item, content);
        if (trigger_out) {
                *trigger_out = trigger;
        }
        return item;
}

static int change_count;
static char changed_value[128];
static int item_open_count;
static int item_close_count;

static void on_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *value = arg;

        change_count++;
        strncpy(changed_value, value ? value : "", sizeof(changed_value) - 1);
        changed_value[sizeof(changed_value) - 1] = '\0';
}

static void on_item_open(ui_widget_t *w, ui_event_t *e, void *arg)
{
        item_open_count++;
}

static void on_item_close(ui_widget_t *w, ui_event_t *e, void *arg)
{
        item_close_count++;
}

static void on_mutating_open(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_accordion_item_set_value(w, "mutated");
}

static void reset_events(void)
{
        change_count = 0;
        changed_value[0] = '\0';
        item_open_count = 0;
        item_close_count = 0;
}

static ui_widget_t *find_by_class(ui_widget_t *w, const char *class_name)
{
        ui_widget_t *child;
        ui_widget_t *found;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_widget_has_class(child, class_name)) {
                        return child;
                }
                found = find_by_class(child, class_name);
                if (found) {
                        return found;
                }
        }
        return NULL;
}

static int count_by_class(ui_widget_t *w, const char *class_name)
{
        ui_widget_t *child;
        int count = 0;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_widget_has_class(child, class_name)) {
                        count++;
                }
                count += count_by_class(child, class_name);
        }
        return count;
}

static ui_widget_t *last_child(ui_widget_t *w)
{
        ui_widget_t *child;
        ui_widget_t *last = NULL;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                last = child;
        }
        return last;
}

static void test_default_value(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);
        ui_widget_t *content = ui_widget_get_child(b, 1);
        ui_widget_t *body = ui_create_widget("text");

        ui_text_set_content(body, "A longer body that should wrap and make the "
                                  "open item grow to fit its content.");
        ui_widget_append(content, body);
        ui_widget_on(accordion, "change", on_change, NULL);
        reset_events();
        ui_widget_set_attr(accordion, "default-value", "b");
        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();
        ctest_equal_int("default-value initialization is silent", change_count,
                        0);

        ctest_equal_bool("default-value opens the matching item",
                         ui_accordion_item_get_open(b), true);
        ctest_equal_bool("default-value leaves others closed",
                         ui_accordion_item_get_open(a), false);
        ctest_equal_str("root value reflects the open item",
                        ui_accordion_get_value(accordion), "b");
        ctest_equal_bool("is_open finds the open item",
                         ui_accordion_is_open(accordion, "b"), true);
        ctest_equal_bool("open item content is visible",
                         ui_widget_is_visible(content), true);
        ctest_equal_bool("open item content has height",
                         content->border_box.height > 0, true);
        ctest_equal_bool("open item grows to fit content",
                         b->border_box.height >= content->border_box.height,
                         true);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_item_setter_and_value(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);

        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        ui_update();
        ui_update();
        reset_events();

        ui_accordion_item_set_open(a, true);
        ctest_equal_bool("setter opens the item", ui_accordion_item_get_open(a),
                         true);
        ctest_equal_str("setter updates root value",
                        ui_accordion_get_value(accordion), "a");
        ctest_equal_int("setter emits one change", change_count, 1);
        ctest_equal_str("setter change payload", changed_value, "a");

        ui_accordion_set_value(accordion, "b");
        ctest_equal_bool("set_value opens b", ui_accordion_item_get_open(b),
                         true);
        ctest_equal_bool("single mode closes a", ui_accordion_item_get_open(a),
                         false);
        ctest_equal_str("set_value updates root value",
                        ui_accordion_get_value(accordion), "b");
        ctest_equal_int("set_value stays silent", change_count, 1);

        ui_accordion_set_multiple(accordion, true);
        ui_accordion_set_value(accordion, "a,b");
        ctest_equal_bool(
            "multiple keeps both open",
            ui_accordion_item_get_open(a) && ui_accordion_item_get_open(b),
            true);
        ctest_equal_str("multiple value order is document order",
                        ui_accordion_get_value(accordion), "a,b");
        ctest_equal_bool("multiple getter",
                         ui_accordion_get_multiple(accordion), true);

        ui_accordion_set_multiple(accordion, false);
        ctest_equal_bool("turning multiple off keeps the first open",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_bool("turning multiple off closes the rest",
                         ui_accordion_item_get_open(b), false);
        ctest_equal_str("turning multiple off rebuilds value",
                        ui_accordion_get_value(accordion), "a");

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_keyless_and_duplicate_values(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *keyless = make_item("", NULL);
        ui_widget_t *first = make_item("x", NULL);
        ui_widget_t *second = make_item("x", NULL);

        ui_widget_append(accordion, keyless);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        ui_update();
        ui_update();
        reset_events();

        ui_accordion_item_set_open(keyless, true);
        ctest_equal_str("keyless change payload is empty", changed_value, "");
        ctest_equal_str("keyless item is omitted from value",
                        ui_accordion_get_value(accordion), "");

        ui_widget_append(accordion, first);
        ui_widget_append(accordion, second);
        ui_update();
        ui_update();
        ui_accordion_set_value(accordion, "x");
        ctest_equal_bool("single mode opens the first duplicate",
                         ui_accordion_item_get_open(first), true);
        ctest_equal_bool("single mode skips the second duplicate",
                         ui_accordion_item_get_open(second), false);
        ctest_equal_str("duplicate value appears once",
                        ui_accordion_get_value(accordion), "x");

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_empty_and_late_items(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);
        ui_widget_t *c;

        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        ui_update();
        ui_update();
        reset_events();

        ui_accordion_set_value(accordion, "a");
        ui_accordion_set_value(accordion, "");
        ctest_equal_bool("empty value closes a", ui_accordion_item_get_open(a),
                         false);
        ctest_equal_str("empty value clears the list",
                        ui_accordion_get_value(accordion), "");
        ctest_equal_int("empty value is silent", change_count, 0);

        c = make_item("c", NULL);
        ui_widget_append(accordion, c);
        ui_update();
        ui_update();
        ui_accordion_set_value(accordion, "c");
        ctest_equal_bool("late item opens", ui_accordion_item_get_open(c),
                         true);
        ctest_equal_str("late item enters value",
                        ui_accordion_get_value(accordion), "c");

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_value_parsing(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *keyless = make_item("", NULL);
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);

        ui_widget_append(accordion, keyless);
        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();

        ui_accordion_set_value(accordion, " ");
        ctest_equal_bool("whitespace value keeps keyless closed",
                         ui_accordion_item_get_open(keyless), false);
        ui_accordion_set_value(accordion, ",");
        ctest_equal_bool("comma value keeps keyless closed",
                         ui_accordion_item_get_open(keyless), false);
        ui_accordion_set_value(accordion, "a,");
        ctest_equal_bool("trailing comma opens the named item",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_bool("trailing comma keeps keyless closed",
                         ui_accordion_item_get_open(keyless), false);

        ui_accordion_set_value(accordion, " b , a ");
        ctest_equal_bool("single mode opens the first listed item",
                         ui_accordion_item_get_open(b), true);
        ctest_equal_bool("single mode ignores later listed items",
                         ui_accordion_item_get_open(a), false);
        ctest_equal_str("single mode rebuilds document order value",
                        ui_accordion_get_value(accordion), "b");
        ctest_equal_bool("is_open is false for a closed item",
                         ui_accordion_is_open(accordion, "a"), false);
        ctest_equal_bool("is_open is false for an unknown item",
                         ui_accordion_is_open(accordion, "missing"), false);

        ui_accordion_set_value(accordion, "unknown");
        ctest_equal_str("unknown value closes everything",
                        ui_accordion_get_value(accordion), "");
        ctest_equal_bool("unknown value leaves a closed",
                         ui_accordion_item_get_open(a), false);
        ctest_equal_bool("unknown value leaves b closed",
                         ui_accordion_item_get_open(b), false);

        ui_accordion_set_value(accordion, "missing,a");
        ctest_equal_bool("single mode skips values without an item",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_str("single mode skips values without an item and rebuilds",
                        ui_accordion_get_value(accordion), "a");

        ui_accordion_set_multiple(accordion, true);
        ui_accordion_set_value(accordion, "a, ,b");
        ctest_equal_bool("multiple skips empty segments and opens a",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_bool("multiple skips empty segments and opens b",
                         ui_accordion_item_get_open(b), true);
        ctest_equal_bool("multiple keeps keyless closed",
                         ui_accordion_item_get_open(keyless), false);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_value_precedence(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);

        ui_widget_set_attr(accordion, "default-value", "b");
        ui_widget_set_attr(accordion, "value", "a");
        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();

        ctest_equal_bool("value wins over default-value",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_bool("default-value is ignored when value exists",
                         ui_accordion_item_get_open(b), false);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_item_open_attribute(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *item = make_item("a", NULL);

        ui_widget_set_attr(item, "open", "true");
        ui_widget_append(accordion, item);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        ui_widget_on(item, "open", on_item_open, NULL);
        ui_widget_on(item, "close", on_item_close, NULL);
        reset_events();
        ui_update();
        ui_update();

        ctest_equal_bool("item open attribute applies before initialization",
                         ui_accordion_item_get_open(item), true);
        ctest_equal_int("initial item open is silent", change_count, 0);
        ctest_equal_int("initial item open emits no item event",
                        item_open_count, 0);
        ctest_equal_str("initial item open fills root value",
                        ui_accordion_get_value(accordion), "a");

        ui_widget_set_attr(item, "open", "false");
        ctest_equal_bool("open attribute closes after initialization",
                         ui_accordion_item_get_open(item), false);
        ctest_equal_int("open attribute emits close", item_close_count, 1);
        ctest_equal_int("open attribute emits change", change_count, 1);
        ctest_equal_str("open attribute change payload", changed_value, "a");

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_change_payload_snapshot(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *item = make_item("a", NULL);

        ui_widget_append(accordion, item);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        ui_widget_on(item, "open", on_mutating_open, NULL);
        reset_events();
        ui_update();
        ui_update();

        ui_accordion_item_set_open(item, true);
        ctest_equal_str("change keeps the value from before the item event",
                        changed_value, "a");
        ctest_equal_str("open listener mutation still updates the root value",
                        ui_accordion_get_value(accordion), "mutated");

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_wrapped_items(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *wrapper = ui_create_widget(NULL);
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);

        ui_widget_set_attr(accordion, "default-value", "b");
        ui_widget_append(wrapper, a);
        ui_widget_append(wrapper, b);
        ui_widget_append(accordion, wrapper);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();

        ctest_equal_bool("wrapped item opens from default-value",
                         ui_accordion_item_get_open(b), true);
        ctest_equal_bool("wrapped sibling stays closed",
                         ui_accordion_item_get_open(a), false);
        ctest_equal_bool("wrapped last item gets the last status",
                         ui_widget_has_status(b, "last"), true);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static bool has_last_item(ui_widget_t *w)
{
        ui_widget_t *child;
        size_t i;

        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_widget_has_status(child, "last") ||
                    has_last_item(child)) {
                        return true;
                }
        }
        return false;
}

static void test_last_status(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);
        ui_widget_t *c = make_item("c", NULL);

        ctest_equal_bool("empty accordion has no last item",
                         has_last_item(accordion), false);

        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();
        ctest_equal_bool("first item is not last",
                         ui_widget_has_status(a, "last"), false);
        ctest_equal_bool("second item is last", ui_widget_has_status(b, "last"),
                         true);

        ui_widget_append(accordion, c);
        ui_update();
        ui_update();
        ctest_equal_bool("previous last loses the status",
                         ui_widget_has_status(b, "last"), false);
        ctest_equal_bool("new item becomes last",
                         ui_widget_has_status(c, "last"), true);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_flex_parent_layout(void)
{
        ui_widget_t *preview = ui_create_widget(NULL);
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *trigger;
        ui_widget_t *item = make_item("a", &trigger);
        ui_widget_t *content = ui_widget_get_child(item, 1);
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *body = ui_create_widget("text");

        ui_widget_set_style_string(preview, "display", "flex");
        ui_widget_set_style_string(preview, "align-items", "center");
        ui_widget_set_style_string(preview, "justify-content", "center");
        ui_widget_set_style_string(preview, "min-height", "128px");
        ui_text_set_content(label, "A trigger");
        ui_text_set_content(body,
                            "A body long enough to force the open item to "
                            "grow inside a flex parent.");
        ui_widget_append(trigger, label);
        ui_widget_append(content, body);
        ui_widget_set_attr(accordion, "default-value", "a");
        ui_widget_set_style_string(accordion, "width", "512px");
        ui_widget_append(accordion, item);
        ui_widget_append(preview, accordion);
        ui_widget_append(ui_root(), preview);
        ui_update();
        ui_update();

        ctest_equal_bool("open content is visible in a flex parent",
                         ui_widget_is_visible(content), true);
        ctest_equal_bool("open item grows in a flex parent",
                         item->border_box.height >= content->border_box.height,
                         true);

        ui_widget_destroy(preview);
        ui_clear_trash();
}

static void test_nested_scan(void)
{
        ui_widget_t *outer = ui_create_accordion();
        ui_widget_t *outer_item = make_item("outer", NULL);
        ui_widget_t *outer_content = ui_widget_get_child(outer_item, 1);
        ui_widget_t *inner = ui_create_accordion();
        ui_widget_t *inner_item = make_item("inner", NULL);

        ui_widget_append(inner, inner_item);
        ui_widget_append(outer_content, inner);
        ui_widget_append(outer, outer_item);
        ui_widget_append(ui_root(), outer);
        ui_update();
        ui_update();

        ui_accordion_item_set_open(inner_item, true);
        ctest_equal_str("inner value stays in the inner root",
                        ui_accordion_get_value(inner), "inner");
        ctest_equal_str("outer value ignores the nested item",
                        ui_accordion_get_value(outer), "");
        ctest_equal_bool("outer last stays on its own item",
                         ui_widget_has_status(outer_item, "last"), true);
        ctest_equal_bool("inner root marks its own last",
                         ui_widget_has_status(inner_item, "last"), true);

        ui_widget_destroy(outer);
        ui_clear_trash();
}

static void test_item_events_and_single_mode(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *a = make_item("a", NULL);
        ui_widget_t *b = make_item("b", NULL);

        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        ui_widget_on(a, "open", on_item_open, NULL);
        ui_widget_on(a, "close", on_item_close, NULL);
        ui_widget_on(b, "open", on_item_open, NULL);
        ui_update();
        ui_update();
        reset_events();

        ui_accordion_item_set_open(a, true);
        ctest_equal_int("opening a emits one item open", item_open_count, 1);
        ctest_equal_int("opening a emits no item close", item_close_count, 0);

        ui_accordion_item_set_open(b, true);
        ctest_equal_int("opening b emits one item open", item_open_count, 2);
        ctest_equal_int("auto-closing a is silent for item events",
                        item_close_count, 0);
        ctest_equal_str("change payload is the newly opened item",
                        changed_value, "b");
        ctest_equal_int("two changes total", change_count, 2);

        ui_accordion_item_set_open(a, true);
        ctest_equal_bool("a stays open", ui_accordion_item_get_open(a), true);
        ctest_equal_int("opening a after b emits one more change", change_count,
                        3);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void emit_click(ui_widget_t *target)
{
        ui_event_t e;

        ui_event_init(&e, "click");
        e.target = target;
        ui_widget_emit_event(target, e, NULL);
}

static void emit_keydown(ui_widget_t *target, int code)
{
        ui_event_t e;

        ui_event_init(&e, "keydown");
        e.target = target;
        e.key.code = code;
        ui_widget_emit_event(target, e, NULL);
}

static void test_multiple_click(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *trigger_a;
        ui_widget_t *trigger_b;
        ui_widget_t *a = make_item("a", &trigger_a);
        ui_widget_t *b = make_item("b", &trigger_b);

        ui_accordion_set_multiple(accordion, true);
        ui_widget_append(accordion, a);
        ui_widget_append(accordion, b);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        reset_events();
        ui_update();
        ui_update();

        emit_click(trigger_a);
        ctest_equal_bool("multiple opens the first clicked item",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_bool("multiple keeps the sibling closed",
                         ui_accordion_item_get_open(b), false);
        ctest_equal_str("multiple first payload", changed_value, "a");

        emit_click(trigger_b);
        ctest_equal_bool("multiple opens the second item",
                         ui_accordion_item_get_open(b), true);
        ctest_equal_bool("multiple keeps the first item open",
                         ui_accordion_item_get_open(a), true);
        ctest_equal_str("multiple second payload", changed_value, "b");
        ctest_equal_int("multiple emits one change per click", change_count, 2);

        emit_click(trigger_a);
        ctest_equal_bool("multiple closes only the clicked item",
                         ui_accordion_item_get_open(a), false);
        ctest_equal_bool("multiple leaves the other item open",
                         ui_accordion_item_get_open(b), true);
        ctest_equal_int("multiple close emits one change", change_count, 3);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_trigger_click(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *trigger;
        ui_widget_t *item = make_item("a", &trigger);
        ui_widget_t *content = ui_widget_get_child(item, 1);
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *body = ui_create_widget("text");
        ui_widget_t *icon;

        ui_text_set_content(label, "Toggle");
        ui_text_set_content(body,
                            "A longer body that should make the item grow "
                            "after it is opened by a click.");
        ui_widget_append(trigger, label);
        ui_widget_append(content, body);
        ui_widget_append(accordion, item);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        reset_events();
        ui_update();
        ui_update();
        icon = find_by_class(trigger, "accordion-trigger-icon");
        ctest_equal_bool("trigger has an icon to click", icon != NULL, true);

        emit_click(label);
        ui_update();
        ctest_equal_bool("clicking the trigger opens the item",
                         ui_accordion_item_get_open(item), true);
        ctest_equal_bool("clicking open grows the item to fit content",
                         item->border_box.height >= content->border_box.height,
                         true);
        ctest_equal_int("clicking the trigger emits one change", change_count,
                        1);
        ctest_equal_str("clicking the trigger change payload", changed_value,
                        "a");

        emit_click(content);
        ctest_equal_bool("clicking content does not toggle",
                         ui_accordion_item_get_open(item), true);
        ctest_equal_int("clicking content emits no change", change_count, 1);

        if (icon) {
                emit_click(icon);
                ctest_equal_bool("clicking the chevron toggles the item",
                                 ui_accordion_item_get_open(item), false);
                ctest_equal_int("clicking the chevron emits one change",
                                change_count, 2);
        }

        emit_click(label);
        ctest_equal_bool("clicking the trigger again opens the item",
                         ui_accordion_item_get_open(item), true);
        ctest_equal_int("third click emits one more change", change_count, 3);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_keyboard_and_disabled(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *trigger;
        ui_widget_t *item = make_item("a", &trigger);
        ui_widget_t *content = ui_widget_get_child(item, 1);

        ui_widget_append(accordion, item);
        ui_widget_append(ui_root(), accordion);
        ui_widget_on(accordion, "change", on_change, NULL);
        reset_events();
        ui_update();
        ui_update();

        emit_keydown(trigger, KEY_ENTER);
        ctest_equal_bool("Enter opens the item",
                         ui_accordion_item_get_open(item), true);
        ctest_equal_int("Enter emits one change", change_count, 1);
        ctest_equal_str("Enter change payload", changed_value, "a");

        emit_keydown(content, KEY_SPACE);
        ctest_equal_bool("Space on content does not toggle",
                         ui_accordion_item_get_open(item), true);
        ctest_equal_int("Space on content emits no change", change_count, 1);

        emit_keydown(trigger, KEY_SPACE);
        ctest_equal_bool("Space closes the item",
                         ui_accordion_item_get_open(item), false);
        ctest_equal_int("Space emits one more change", change_count, 2);

        emit_keydown(trigger, KEY_ESCAPE);
        ctest_equal_bool("Escape does not toggle",
                         ui_accordion_item_get_open(item), false);

        ui_widget_set_disabled(item, true);
        emit_click(trigger);
        emit_keydown(trigger, KEY_ENTER);
        ctest_equal_bool("disabled item ignores click and keyboard",
                         ui_accordion_item_get_open(item), false);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_data_trigger(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *item = ui_create_accordion_item();
        ui_widget_t *trigger = ui_create_widget(NULL);
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *content = ui_create_accordion_content();
        ui_widget_t *wrapper;

        ui_accordion_item_set_value(item, "custom");
        ui_text_set_content(label, "Custom");
        ui_widget_set_attr(trigger, "data-accordion-trigger", "true");
        wrapper = ui_create_widget(NULL);
        ui_widget_append(wrapper, label);
        ui_widget_append(trigger, wrapper);
        ui_widget_append(item, trigger);
        ui_widget_append(item, content);
        ui_widget_append(accordion, item);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();

        emit_click(label);
        ctest_equal_bool("data trigger opens the item",
                         ui_accordion_item_get_open(item), true);

        emit_keydown(label, KEY_SPACE);
        ctest_equal_bool("data trigger handles Space",
                         ui_accordion_item_get_open(item), false);

        emit_keydown(label, KEY_ENTER);
        ctest_equal_bool("data trigger handles Enter",
                         ui_accordion_item_get_open(item), true);

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

static void test_nested_click(void)
{
        ui_widget_t *outer = ui_create_accordion();
        ui_widget_t *outer_trigger;
        ui_widget_t *outer_item = make_item("outer", &outer_trigger);
        ui_widget_t *outer_content = ui_widget_get_child(outer_item, 1);
        ui_widget_t *inner = ui_create_accordion();
        ui_widget_t *inner_trigger;
        ui_widget_t *inner_item = make_item("inner", &inner_trigger);

        ui_widget_append(inner, inner_item);
        ui_widget_append(outer_content, inner);
        ui_widget_append(outer, outer_item);
        ui_widget_append(ui_root(), outer);
        ui_update();
        ui_update();

        emit_click(inner_trigger);
        ctest_equal_bool("inner trigger opens the inner item",
                         ui_accordion_item_get_open(inner_item), true);
        ctest_equal_bool("inner trigger leaves the outer item closed",
                         ui_accordion_item_get_open(outer_item), false);

        emit_click(outer_trigger);
        ctest_equal_bool("outer trigger opens the outer item",
                         ui_accordion_item_get_open(outer_item), true);

        emit_keydown(inner_trigger, KEY_SPACE);
        ctest_equal_bool("inner keyboard closes the inner item",
                         ui_accordion_item_get_open(inner_item), false);
        ctest_equal_bool("inner keyboard leaves the outer item open",
                         ui_accordion_item_get_open(outer_item), true);

        ui_widget_destroy(outer);
        ui_clear_trash();
}

static void test_nested_chevron_scope(void)
{
        ui_widget_t *outer = ui_create_accordion();
        ui_widget_t *outer_item = ui_create_accordion_item();
        ui_widget_t *outer_content = ui_create_accordion_content();
        ui_widget_t *outer_trigger = ui_create_accordion_trigger();
        ui_widget_t *outer_icon;
        ui_widget_t *inner = ui_create_accordion();
        ui_widget_t *inner_trigger;
        ui_widget_t *inner_item = make_item("inner", &inner_trigger);
        ui_widget_t *inner_icon;

        ui_accordion_item_set_value(outer_item, "outer");
        ui_widget_append(inner, inner_item);
        ui_widget_append(outer_content, inner);
        /* Content first: a recursive trigger search would pick the nested
           trigger before the outer one. */
        ui_widget_append(outer_item, outer_content);
        ui_widget_append(outer_item, outer_trigger);
        ui_widget_append(outer, outer_item);
        ui_widget_append(ui_root(), outer);
        ui_update();
        ui_update();

        outer_icon = find_by_class(outer_trigger, "accordion-trigger-icon");
        inner_icon = find_by_class(inner_trigger, "accordion-trigger-icon");
        ui_accordion_item_set_open(outer_item, true);
        ui_update();

        ctest_equal_bool("outer trigger follows the outer item",
                         outer_icon && ui_widget_has_class(
                                           outer_icon, "fui-icon-chevron-up"),
                         true);
        ctest_equal_bool("outer change leaves the nested icon closed",
                         inner_icon && ui_widget_has_class(
                                           inner_icon, "fui-icon-chevron-down"),
                         true);

        ui_widget_destroy(outer);
        ui_clear_trash();
}

static void test_chevron(void)
{
        ui_widget_t *accordion = ui_create_accordion();
        ui_widget_t *trigger;
        ui_widget_t *item = make_item("a", &trigger);
        ui_widget_t *icon;
        ui_widget_t *item2;
        ui_widget_t *trigger2;
        ui_widget_t *item3;
        ui_widget_t *trigger3;
        ui_widget_t *content3;

        ui_widget_append(accordion, item);
        ui_widget_append(ui_root(), accordion);
        ui_update();
        ui_update();

        icon = find_by_class(trigger, "accordion-trigger-icon");
        ctest_equal_bool("standard trigger creates an icon", icon != NULL,
                         true);
        ctest_equal_int("standard trigger has exactly one icon",
                        count_by_class(trigger, "accordion-trigger-icon"), 1);
        if (!icon) {
                ui_widget_destroy(accordion);
                ui_clear_trash();
                return;
        }
        {
                ui_widget_t *long_label = ui_create_widget("text");

                ui_text_set_content(long_label,
                                    "What are your shipping options?");
                ui_widget_append(trigger, long_label);
        }
        ui_update();
        ctest_equal_bool("icon starts as chevron-down",
                         ui_widget_has_class(icon, "fui-icon-chevron-down"),
                         true);

        ui_accordion_item_set_open(item, true);
        ctest_equal_bool("open item uses chevron-up",
                         ui_widget_has_class(icon, "fui-icon-chevron-up"),
                         true);

        ui_accordion_item_set_open(item, false);
        ctest_equal_bool("closed item restores chevron-down",
                         ui_widget_has_class(icon, "fui-icon-chevron-down"),
                         true);

        trigger2 = ui_create_accordion_trigger();
        item2 = ui_create_accordion_item();
        ui_widget_set_attr(trigger2, "chevron", "false");
        ui_widget_append(item2, trigger2);
        ui_widget_append(accordion, item2);
        ui_update();
        ctest_equal_bool(
            "chevron=false removes the icon",
            find_by_class(trigger2, "accordion-trigger-icon") == NULL, true);

        ui_widget_set_attr(trigger2, "chevron", "true");
        ui_update();
        icon = find_by_class(trigger2, "accordion-trigger-icon");
        ctest_equal_bool("chevron=true recreates the icon", icon != NULL, true);

        ui_widget_append(trigger2, ui_create_widget("text"));
        ui_update();
        ctest_equal_bool("appending content keeps the icon last",
                         last_child(trigger2) == icon, true);

        trigger3 = ui_create_widget(NULL);
        content3 = ui_create_accordion_content();
        item3 = ui_create_accordion_item();
        ui_widget_set_attr(trigger3, "data-accordion-trigger", "true");
        ui_widget_append(item3, trigger3);
        ui_widget_append(item3, content3);
        ui_widget_append(accordion, item3);
        ui_update();
        ctest_equal_bool("data trigger does not get an auto icon",
                         find_by_class(item3, "accordion-trigger-icon") == NULL,
                         true);

        {
                ui_widget_t *standalone = ui_create_accordion_trigger();
                ui_widget_t *standalone_icon =
                    find_by_class(standalone, "accordion-trigger-icon");

                ui_widget_append(ui_root(), standalone);
                ui_widget_append(standalone, ui_create_widget("text"));
                ui_update();
                ui_update();
                ctest_equal_bool("standalone trigger keeps the icon last",
                                 last_child(standalone) == standalone_icon,
                                 true);
                ui_widget_destroy(standalone);
        }

        ui_widget_destroy(accordion);
        ui_clear_trash();
}

void test_accordion(void)
{
        lcui_init();
        ctest_describe("accordion factories", test_family_factories);
        ctest_describe("accordion default value", test_default_value);
        ctest_describe("accordion setter and value",
                       test_item_setter_and_value);
        ctest_describe("accordion keyless and duplicate",
                       test_keyless_and_duplicate_values);
        ctest_describe("accordion empty and late items",
                       test_empty_and_late_items);
        ctest_describe("accordion value parsing", test_value_parsing);
        ctest_describe("accordion value precedence", test_value_precedence);
        ctest_describe("accordion item open attribute",
                       test_item_open_attribute);
        ctest_describe("accordion change payload snapshot",
                       test_change_payload_snapshot);
        ctest_describe("accordion wrapped items", test_wrapped_items);
        ctest_describe("accordion last status", test_last_status);
        ctest_describe("accordion flex parent layout", test_flex_parent_layout);
        ctest_describe("accordion nested scan", test_nested_scan);
        ctest_describe("accordion item events",
                       test_item_events_and_single_mode);
        ctest_describe("accordion multiple click", test_multiple_click);
        ctest_describe("accordion trigger click", test_trigger_click);
        ctest_describe("accordion keyboard", test_keyboard_and_disabled);
        ctest_describe("accordion data trigger", test_data_trigger);
        ctest_describe("accordion nested click", test_nested_click);
        ctest_describe("accordion nested chevron scope",
                       test_nested_chevron_scope);
        ctest_describe("accordion chevron", test_chevron);
        lcui_destroy();
}
