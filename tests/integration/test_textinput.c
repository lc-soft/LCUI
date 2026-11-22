/*
 * tests/cases/test_textinput.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ctest-custom.h>
#include <math.h>

#define SEL_TEST_W 240
#define SEL_TEST_H 80

static bool pixel_equal(pd_color_t a, pd_color_t b)
{
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

static bool pixel_is_dark(pd_color_t c)
{
        return c.a > 0 && c.r < 128 && c.g < 128 && c.b < 128;
}

static void render_root_canvas(pd_canvas_t *canvas)
{
        pd_context_t paint;
        pd_rect_t rect = { 0, 0, SEL_TEST_W, SEL_TEST_H };

        pd_canvas_init(canvas);
        pd_canvas_create(canvas, SEL_TEST_W, SEL_TEST_H);
        pd_canvas_fill(canvas, pd_rgb(255, 255, 255));
        memset(&paint, 0, sizeof(paint));
        paint.rect = rect;
        pd_canvas_init(&paint.canvas);
        pd_canvas_quote(&paint.canvas, canvas, &rect);
        ui_widget_render(ui_root(), &paint);
}

static void scan_dark_bbox(const pd_canvas_t *canvas, pd_rect_t *bbox)
{
        int x, y;

        bbox->x = bbox->y = bbox->width = bbox->height = 0;
        for (y = 0; y < (int)canvas->height; ++y) {
                for (x = 0; x < (int)canvas->width; ++x) {
                        if (!pixel_is_dark(pd_canvas_get_pixel(
                                canvas, (unsigned)x, (unsigned)y))) {
                                continue;
                        }
                        if (bbox->width == 0) {
                                bbox->x = x;
                                bbox->y = y;
                                bbox->width = bbox->height = 1;
                        } else {
                                if (x < bbox->x) {
                                        bbox->width += bbox->x - x;
                                        bbox->x = x;
                                }
                                if (y < bbox->y) {
                                        bbox->height += bbox->y - y;
                                        bbox->y = y;
                                }
                                if (x >= bbox->x + bbox->width) {
                                        bbox->width = x - bbox->x + 1;
                                }
                                if (y >= bbox->y + bbox->height) {
                                        bbox->height = y - bbox->y + 1;
                                }
                        }
                }
        }
}

static void scan_diff_bbox(const pd_canvas_t *a, const pd_canvas_t *b,
                           pd_rect_t *bbox)
{
        int x, y;

        bbox->x = bbox->y = bbox->width = bbox->height = 0;
        for (y = 0; y < (int)a->height; ++y) {
                for (x = 0; x < (int)a->width; ++x) {
                        if (pixel_equal(pd_canvas_get_pixel(a, (unsigned)x,
                                                            (unsigned)y),
                                        pd_canvas_get_pixel(b, (unsigned)x,
                                                            (unsigned)y))) {
                                continue;
                        }
                        if (bbox->width == 0) {
                                bbox->x = x;
                                bbox->y = y;
                                bbox->width = bbox->height = 1;
                        } else {
                                if (x < bbox->x) {
                                        bbox->width += bbox->x - x;
                                        bbox->x = x;
                                }
                                if (y < bbox->y) {
                                        bbox->height += bbox->y - y;
                                        bbox->y = y;
                                }
                                if (x >= bbox->x + bbox->width) {
                                        bbox->width = x - bbox->x + 1;
                                }
                                if (y >= bbox->y + bbox->height) {
                                        bbox->height = y - bbox->y + 1;
                                }
                        }
                }
        }
}

static void check_selection_background_alignment(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        pd_canvas_t before, after;
        pd_rect_t glyphs, sel;
        ui_event_t e = { 0 };

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(w, "position", "absolute");
        ui_widget_set_style_string(w, "left", "20px");
        ui_widget_set_style_string(w, "top", "10px");
        ui_widget_set_style_string(w, "width", "180px");
        ui_textinput_set_text_w(w, L"abc");
        ui_root_append(w);
        ui_update();

        render_root_canvas(&before);
        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_A;
        e.key.ctrl_key = true;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        render_root_canvas(&after);

        scan_dark_bbox(&before, &glyphs);
        scan_diff_bbox(&before, &after, &sel);
        ctest_equal_bool("selection should start near the text start",
                         sel.x <= glyphs.x + 2, true);
        ctest_equal_bool("selection should end near the text end",
                         sel.x + sel.width >= glyphs.x + glyphs.width - 2,
                         true);
        ctest_equal_bool("selection should share the text row",
                         sel.y <= glyphs.y + 2 &&
                             sel.y + sel.height >= glyphs.y + glyphs.height - 2,
                         true);

        pd_canvas_destroy(&before);
        pd_canvas_destroy(&after);
        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_password_input(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        wchar_t wcs[64];

        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"secret");
        ui_widget_update(w);

        ctest_equal_bool("password input should keep the text length",
                         ui_textinput_get_text_length(w) == 6, true);
        ctest_equal_bool("password input should preserve the underlying text",
                         ui_textinput_get_text_w(w, 0, 64, wcs) == 6 &&
                             wcscmp(wcs, L"secret") == 0,
                         true);

        ui_textinput_set_password_char(w, 0);
        ui_widget_update(w);
        ctest_equal_bool(
            "disabling password masking should restore the plain text",
            ui_textinput_get_text_w(w, 0, 64, wcs) == 6 &&
                wcscmp(wcs, L"secret") == 0,
            true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_multiline_attr(void)
{
        ui_widget_t *single = ui_create_widget("textinput");
        ui_widget_t *multi = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_widget_set_attr(multi, "multiline", "true");
        ui_root_append(single);
        ui_root_append(multi);
        ui_update();

        ui_event_init(&e, "textinput");
        e.text.text = L"a\nb";
        e.text.length = 3;
        ui_widget_emit_event(single, e, NULL);
        ui_widget_emit_event(multi, e, NULL);
        ui_update();

        ctest_equal_bool("single-line input should drop newline characters",
                         ui_textinput_get_text_length(single) == 2, true);
        ctest_equal_bool("multiline input should keep newline characters",
                         ui_textinput_get_text_length(multi) == 3, true);

        ui_widget_destroy(single);
        ui_widget_destroy(multi);
        ui_clear_trash();
}

#define CARET_TEST_DPI 192.f

static ui_widget_t *create_scaled_textinput(void)
{
        ui_widget_t *w = ui_create_widget("textinput");

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_text_w(w, L"hello world");
        ui_root_append(w);
        ui_update();
        return w;
}

static void destroy_scaled_textinput(ui_widget_t *w)
{
        ui_widget_destroy(w);
        ui_clear_trash();
        ui_metrics.dpi = 96.f;
        ui_update();
}

static void check_multiline_enter_inserts_newline(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_widget_set_attr(w, "multiline", "true");
        ui_root_append(w);
        ui_update();

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_ENTER;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        ctest_equal_bool("enter should insert a line break in multiline mode",
                         ui_textinput_get_text_length(w) == 1, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_paste_newline_filter(void)
{
        ui_widget_t *single = ui_create_widget("textinput");
        ui_widget_t *multi = ui_create_widget("textinput");
        ui_event_t e = { 0 };
        ptk_clipboard_t clipboard = { 0 };
        wchar_t text[] = L"a\nb";

        ui_widget_set_attr(multi, "multiline", "true");
        ui_root_append(single);
        ui_root_append(multi);
        clipboard.text = text;
        clipboard.len = wcslen(text);
        ui_event_init(&e, "paste");
        ui_widget_emit_event(single, e, &clipboard);
        ui_widget_emit_event(multi, e, &clipboard);
        ui_update();

        ctest_equal_bool("single-line paste should drop line breaks",
                         ui_textinput_get_text_length(single) == 2, true);
        ctest_equal_bool("multiline paste should keep line breaks",
                         ui_textinput_get_text_length(multi) == 3, true);

        ui_widget_destroy(single);
        ui_widget_destroy(multi);
        ui_clear_trash();
}

static void check_multiline_autowrap(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_widget_t *caret;
        float expect_top;

        ui_widget_set_attr(w, "multiline", "true");
        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_prop_height, 120, CSS_UNIT_PX);
        ui_textinput_set_text_w(
            w, L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        ui_root_append(w);
        ui_update();

        caret = ui_widget_get_child(w, 0);
        ui_textinput_move_caret(w, 0, 0);
        ui_update();
        expect_top = caret->border_box.y;
        ui_textinput_move_caret(w, 1, 1);
        ui_update();
        ctest_equal_bool("long text should wrap into a second visual line",
                         caret->border_box.y > expect_top, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_edit_roundtrip_multiline(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        wchar_t buf[64];

        ui_widget_set_attr(w, "multiline", "true");
        ui_textinput_set_text_w(w, L"abc\ndef");
        ui_widget_update(w);

        ctest_equal_bool("multiline length should include the newline",
                         ui_textinput_get_text_length(w) == 7, true);
        ctest_equal_bool("multiline text should round-trip",
                         ui_textinput_get_text_w(w, 0, 64, buf) == 7 &&
                             wcscmp(buf, L"abc\ndef") == 0,
                         true);

        ui_textinput_move_caret(w, 1, 0);
        ui_textinput_insert_text_w(w, L"X");
        ui_widget_update(w);
        ctest_equal_bool("insert at line start should stay on line 1",
                         ui_textinput_get_text_w(w, 0, 64, buf) == 8 &&
                             wcscmp(buf, L"abc\nXdef") == 0,
                         true);

        ui_textinput_append_text_w(w, L"!");
        ui_widget_update(w);
        ctest_equal_bool("append should add text at the end",
                         ui_textinput_get_text_w(w, 0, 64, buf) == 9 &&
                             wcscmp(buf, L"abc\nXdef!") == 0,
                         true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_password_edit_roundtrip(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        wchar_t buf[64];

        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"secret");
        ui_widget_update(w);
        ui_textinput_move_caret(w, 0, 3);
        ui_textinput_insert_text_w(w, L"X");
        ui_widget_update(w);
        ctest_equal_bool("password input should keep the plaintext editable",
                         ui_textinput_get_text_w(w, 0, 64, buf) == 7 &&
                             wcscmp(buf, L"secXret") == 0,
                         true);
        ctest_equal_bool("password input length should match plaintext",
                         ui_textinput_get_text_length(w) == 7, true);

        ui_textinput_set_password_char(w, 0);
        ui_widget_update(w);
        ctest_equal_bool(
            "disabling password masking should keep the edited text",
            ui_textinput_get_text_w(w, 0, 64, buf) == 7 &&
                wcscmp(buf, L"secXret") == 0,
            true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_undo_redo(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };
        wchar_t buf[32];

        ui_textinput_set_text_w(w, L"abc");
        ui_widget_update(w);

        ui_event_init(&e, "textinput");
        e.text.text = L"d";
        e.text.length = 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("typed text should extend the value",
                         ui_textinput_get_text_w(w, 0, 32, buf) == 4 &&
                             wcscmp(buf, L"abcd") == 0,
                         true);

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_Z;
        e.key.ctrl_key = true;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("ctrl+z should undo the last input",
                         ui_textinput_get_text_w(w, 0, 32, buf) == 3 &&
                             wcscmp(buf, L"abc") == 0,
                         true);

        e.key.code = KEY_Y;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("ctrl+y should redo the input",
                         ui_textinput_get_text_w(w, 0, 32, buf) == 4 &&
                             wcscmp(buf, L"abcd") == 0,
                         true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_read_only(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_textinput_set_text_w(w, L"abc");
        ui_widget_update(w);
        ui_textinput_set_read_only(w, true);

        ui_event_init(&e, "textinput");
        e.text.text = L"d";
        e.text.length = 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("read-only textinput should ignore text input",
                         ui_textinput_get_text_length(w) == 3, true);

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_BACKSPACE;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("read-only textinput should ignore backspace",
                         ui_textinput_get_text_length(w) == 3, true);

        e.key.code = KEY_ENTER;
        e.key.shift_key = false;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("read-only textinput should ignore enter",
                         ui_textinput_get_text_length(w) == 3, true);

        ui_textinput_set_read_only(w, false);
        e.type = UI_EVENT_TEXTINPUT;
        e.text.text = L"d";
        e.text.length = 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_bool("editable textinput should accept text again",
                         ui_textinput_get_text_length(w) == 4, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static int textinput_change_count;

static void on_textinput_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++textinput_change_count;
}

static void check_password_copy_blocked(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"secret");
        ui_root_append(w);
        ui_update();

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_A;
        e.key.ctrl_key = true;
        ui_widget_emit_event(w, e, NULL);
        e.key.code = KEY_X;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        ctest_equal_bool("password cut should not remove or copy plaintext",
                         ui_textinput_get_text_length(w) == 6, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static int reentrant_change_count;

static void on_reentrant_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ++reentrant_change_count;
        if (reentrant_change_count == 1) {
                ui_textinput_append_text_w(w, L"!");
        }
}

static void check_change_reentrancy(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        wchar_t buf[16];

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_text_w(w, L"ab");
        ui_root_append(w);
        ui_update();
        ui_widget_on(w, "change", on_reentrant_change, NULL);
        reentrant_change_count = 0;

        ui_textinput_append_text_w(w, L"c");
        ui_update();
        ui_update();
        ctest_equal_int("change handler edits should emit another change",
                        reentrant_change_count, 2);
        ctest_equal_bool("change handler edits should be applied",
                         ui_textinput_get_text_w(w, 0, 16, buf) == 4 &&
                             wcscmp(buf, L"abc!") == 0,
                         true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_single_line_api_filter(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        wchar_t buf[16];

        ui_textinput_set_text_w(w, L"a\nb");
        ui_widget_update(w);
        ctest_equal_bool("single-line set_text_w should drop newlines",
                         ui_textinput_get_text_w(w, 0, 16, buf) == 2 &&
                             wcscmp(buf, L"ab") == 0,
                         true);

        ui_textinput_insert_text_w(w, L"c\rd");
        ui_widget_update(w);
        ctest_equal_bool("single-line insert_text_w should drop newlines",
                         ui_textinput_get_text_w(w, 0, 16, buf) == 4 &&
                             wcscmp(buf, L"abcd") == 0,
                         true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_noop_change(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_textinput_set_text_w(w, L"ab");
        ui_root_append(w);
        ui_update();
        ui_widget_on(w, "change", on_textinput_change, NULL);
        textinput_change_count = 0;

        ui_textinput_move_caret(w, 0, 0);
        ui_update();
        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_BACKSPACE;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        ctest_equal_int("backspace at text start should not emit change",
                        textinput_change_count, 0);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_page_down(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };
        wchar_t buf[64];

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_prop_height, 120, CSS_UNIT_PX);
        ui_textinput_enable_multiline(w, true);
        ui_textinput_set_text_w(w, L"0\n1\n2\n3\n4\n5\n6\n7");
        ui_root_append(w);
        ui_update();
        ui_textinput_move_caret(w, 0, 0);
        ui_update();

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_PAGEDOWN;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        ui_textinput_insert_text_w(w, L"X");
        ui_update();
        ctest_equal_bool(
            "page down should move the caret off the first line",
            ui_textinput_get_text_w(w, 0, 64, buf) == 16 && buf[0] != L'X',
            true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_parse_change_suppressed(void)
{
        ui_widget_t *w = ui_create_widget("textinput");

        ui_widget_on(w, "change", on_textinput_change, NULL);
        textinput_change_count = 0;
        ui_widget_set_text(w, "abc");
        ui_widget_update(w);
        ctest_equal_bool(
            "parsed initial text should not emit change",
            textinput_change_count == 0 && ui_textinput_get_text_length(w) == 3,
            true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_change_event(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_text_w(w, L"ab");
        ui_root_append(w);
        ui_update();
        ui_widget_on(w, "change", on_textinput_change, NULL);
        textinput_change_count = 0;

        ui_event_init(&e, "textinput");
        e.text.text = L"c";
        e.text.length = 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_update(w);
        ctest_equal_int("text input should emit one change event",
                        textinput_change_count, 1);

        ui_textinput_insert_text_w(w, L"d");
        ui_widget_update(w);
        ctest_equal_int("insert_text_w should emit one change event",
                        textinput_change_count, 2);

        ui_textinput_set_text_w(w, L"x");
        ui_widget_update(w);
        ctest_equal_int("set_text_w should emit one change event",
                        textinput_change_count, 3);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_edit_marks_content_dirty(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        list_t rects;

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_text_w(w, L"ab");
        ui_root_append(w);
        ui_update();
        list_create(&rects);
        ui_widget_get_dirty_rects(ui_root(), &rects);
        list_destroy(&rects, free);

        ui_textinput_insert_text_w(w, L"c");
        ui_widget_update(w);
        ctest_equal_bool(
            "editing should mark the text area dirty",
            w->rendering.dirty_rect_type != UI_DIRTY_RECT_TYPE_NONE, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_password_multiline_caret(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_widget_t *caret;
        float first_y, second_y;

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_widget_set_style_unit_value(w, css_prop_height, 120, CSS_UNIT_PX);
        ui_textinput_enable_multiline(w, true);
        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"a\nb");
        ui_root_append(w);
        ui_update();
        caret = ui_widget_get_child(w, 0);

        ui_textinput_move_caret(w, 0, 0);
        ui_update();
        first_y = caret->border_box.y;
        ui_textinput_move_caret(w, 1, 0);
        ui_update();
        second_y = caret->border_box.y;
        ctest_equal_bool(
            "password mask should keep line breaks for caret placement",
            second_y > first_y, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_password_pointer_mask_width(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_widget_t *caret;
        ui_event_t e = { 0 };
        wchar_t buf[16];
        float expect_x;

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"iiii");
        ui_root_append(w);
        ui_update();
        caret = ui_widget_get_child(w, 0);
        ui_textinput_move_caret(w, 0, 1);
        ui_update();
        expect_x = caret->border_box.x;
        ui_textinput_move_caret(w, 0, 0);
        ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.x = w->border_box.x + expect_x + 1;
        e.mouse.y = w->border_box.y + w->computed_style.padding_top + 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_release_mouse_capture(w);
        ui_update();
        ui_textinput_insert_text_w(w, L"X");
        ui_update();
        ctest_equal_bool("password click should hit the rendered mask column",
                         ui_textinput_get_text_w(w, 0, 16, buf) == 5 &&
                             wcscmp(buf, L"iXiii") == 0,
                         true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_password_pointer_mapping(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_widget_t *caret;
        ui_event_t e = { 0 };
        float expect_x;

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_password_char(w, L'*');
        ui_textinput_set_text_w(w, L"secret");
        ui_root_append(w);
        ui_update();
        caret = ui_widget_get_child(w, 0);
        ui_textinput_move_caret(w, 0, 3);
        ui_update();
        expect_x = caret->border_box.x;
        ui_textinput_move_caret(w, 0, 0);
        ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.x = w->border_box.x + expect_x + 1;
        e.mouse.y = w->border_box.y + w->computed_style.padding_top + 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_release_mouse_capture(w);
        ui_update();
        ctest_equal_bool("password click should keep the caret position",
                         fabsf(caret->border_box.x - expect_x) <= 1.f, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_drag_marks_content_dirty(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_event_t e = { 0 };
        list_t rects;

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_textinput_set_text_w(w, L"hello world");
        ui_root_append(w);
        ui_update();
        list_create(&rects);
        ui_widget_get_dirty_rects(ui_root(), &rects);
        list_destroy(&rects, free);

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.x = w->border_box.x + w->computed_style.padding_left + 2;
        e.mouse.y = w->border_box.y + w->computed_style.padding_top + 2;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_release_mouse_capture(w);
        ui_update();
        list_create(&rects);
        ui_widget_get_dirty_rects(ui_root(), &rects);
        list_destroy(&rects, free);

        e.type = UI_EVENT_MOUSEMOVE;
        e.mouse.x += 60;
        w->rendering.dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
        ui_widget_emit_event(w, e, NULL);
        ctest_equal_bool(
            "dragging should mark the text area dirty",
            w->rendering.dirty_rect_type != UI_DIRTY_RECT_TYPE_NONE, true);
        ui_update();

        ui_widget_destroy(w);
        ui_clear_trash();
}

static int count_diff_pixels_in_rect(const pd_canvas_t *a, const pd_canvas_t *b,
                                     pd_rect_t r)
{
        int x, y, count = 0;

        for (y = r.y; y < r.y + r.height; ++y) {
                for (x = r.x; x < r.x + r.width; ++x) {
                        if (y < 0 || x < 0 || y >= (int)a->height ||
                            x >= (int)a->width) {
                                continue;
                        }
                        if (!pixel_equal(pd_canvas_get_pixel(a, (unsigned)x,
                                                             (unsigned)y),
                                         pd_canvas_get_pixel(b, (unsigned)x,
                                                             (unsigned)y))) {
                                ++count;
                        }
                }
        }
        return count;
}

static int count_dark_pixels_in_rect(const pd_canvas_t *canvas, pd_rect_t r)
{
        int x, y, count = 0;

        for (y = r.y; y < r.y + r.height; ++y) {
                for (x = r.x; x < r.x + r.width; ++x) {
                        if (y < 0 || x < 0 || y >= (int)canvas->height ||
                            x >= (int)canvas->width) {
                                continue;
                        }
                        if (pixel_is_dark(pd_canvas_get_pixel(
                                canvas, (unsigned)x, (unsigned)y))) {
                                ++count;
                        }
                }
        }
        return count;
}

static void check_blur_keeps_text(void)
{
        ui_widget_t *a = ui_create_widget("textinput");
        ui_widget_t *b = ui_create_widget("textinput");
        pd_canvas_t canvas;
        pd_rect_t area;
        int dark_before, dark_after;

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(a, "position", "absolute");
        ui_widget_set_style_string(a, "left", "20px");
        ui_widget_set_style_string(a, "top", "5px");
        ui_widget_set_style_string(a, "width", "180px");
        ui_widget_set_style_string(b, "position", "absolute");
        ui_widget_set_style_string(b, "left", "20px");
        ui_widget_set_style_string(b, "top", "45px");
        ui_widget_set_style_string(b, "width", "180px");
        ui_textinput_set_text_w(a, L"abc");
        ui_textinput_set_text_w(b, L"def");
        ui_root_append(a);
        ui_root_append(b);
        ui_update();
        ui_set_focus(a);
        ui_update();
        render_root_canvas(&canvas);
        area.x = (int)a->content_box.x;
        area.y = (int)a->content_box.y;
        area.width = (int)a->content_box.width;
        area.height = (int)a->content_box.height;
        dark_before = count_dark_pixels_in_rect(&canvas, area);
        pd_canvas_destroy(&canvas);

        ui_set_focus(b);
        ui_update();
        render_root_canvas(&canvas);
        dark_after = count_dark_pixels_in_rect(&canvas, area);
        ctest_equal_bool("blur should keep the text visible",
                         dark_before > 0 && dark_after > 0, true);
        pd_canvas_destroy(&canvas);

        ui_widget_destroy(a);
        ui_widget_destroy(b);
        ui_clear_trash();
}

static void check_blur_keeps_long_text(void)
{
        ui_widget_t *a = ui_create_widget("textinput");
        ui_widget_t *b = ui_create_widget("textinput");
        pd_canvas_t canvas;
        pd_rect_t area;
        int dark_after;
        int i;

        ui_metrics.dpi = 192.f;
        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(a, "position", "absolute");
        ui_widget_set_style_string(a, "left", "20px");
        ui_widget_set_style_string(a, "top", "5px");
        ui_widget_set_style_string(a, "width", "180px");
        ui_widget_set_style_string(b, "position", "absolute");
        ui_widget_set_style_string(b, "left", "20px");
        ui_widget_set_style_string(b, "top", "45px");
        ui_widget_set_style_string(b, "width", "180px");
        for (i = 0; i < 40; ++i) {
                ui_textinput_append_text_w(a, L"W");
        }
        ui_root_append(a);
        ui_root_append(b);
        ui_update();
        ui_set_focus(a);
        ui_update();

        ui_set_focus(b);
        ui_update();
        render_root_canvas(&canvas);
        area.x = (int)a->content_box.x;
        area.y = (int)a->content_box.y;
        area.width = (int)a->content_box.width;
        area.height = (int)a->content_box.height;
        dark_after = count_dark_pixels_in_rect(&canvas, area);
        ctest_equal_bool("blur should keep long text visible", dark_after > 0,
                         true);
        pd_canvas_destroy(&canvas);

        ui_widget_destroy(a);
        ui_widget_destroy(b);
        ui_clear_trash();
        ui_metrics.dpi = 96.f;
        ui_update();
}

static void check_blur_keeps_text_with_placeholder(void)
{
        ui_widget_t *a = ui_create_widget("textinput");
        ui_widget_t *b = ui_create_widget("textinput");
        pd_canvas_t canvas;
        pd_rect_t area;
        int dark_after;

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(a, "position", "absolute");
        ui_widget_set_style_string(a, "left", "20px");
        ui_widget_set_style_string(a, "top", "5px");
        ui_widget_set_style_string(a, "width", "180px");
        ui_widget_set_style_string(b, "position", "absolute");
        ui_widget_set_style_string(b, "left", "20px");
        ui_widget_set_style_string(b, "top", "45px");
        ui_widget_set_style_string(b, "width", "180px");
        ui_textinput_set_placeholder(a, "Please input...");
        ui_textinput_set_text_w(a, L"abc");
        ui_root_append(a);
        ui_root_append(b);
        ui_update();
        ui_set_focus(a);
        ui_update();

        ui_set_focus(b);
        ui_update();
        render_root_canvas(&canvas);
        area.x = (int)a->content_box.x;
        area.y = (int)a->content_box.y;
        area.width = (int)a->content_box.width;
        area.height = (int)a->content_box.height;
        dark_after = count_dark_pixels_in_rect(&canvas, area);
        ctest_equal_bool("placeholder should not replace a blurred value",
                         dark_after > 0, true);
        pd_canvas_destroy(&canvas);

        ui_widget_destroy(a);
        ui_widget_destroy(b);
        ui_clear_trash();
}

static void check_blur_keeps_typed_text(void)
{
        ui_widget_t *a = ui_create_widget("textinput");
        ui_widget_t *b = ui_create_widget("textinput");
        pd_canvas_t canvas;
        pd_rect_t area;
        ui_event_t e = { 0 };
        int dark_after;

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(a, "position", "absolute");
        ui_widget_set_style_string(a, "left", "20px");
        ui_widget_set_style_string(a, "top", "5px");
        ui_widget_set_style_string(a, "width", "180px");
        ui_widget_set_style_string(b, "position", "absolute");
        ui_widget_set_style_string(b, "left", "20px");
        ui_widget_set_style_string(b, "top", "45px");
        ui_widget_set_style_string(b, "width", "180px");
        ui_root_append(a);
        ui_root_append(b);
        ui_update();
        ui_set_focus(a);
        ui_update();

        ui_event_init(&e, "textinput");
        e.text.text = L"typed";
        e.text.length = 5;
        ui_widget_emit_event(a, e, NULL);
        ui_update();

        ui_set_focus(b);
        ui_update();
        render_root_canvas(&canvas);
        area.x = (int)a->content_box.x;
        area.y = (int)a->content_box.y;
        area.width = (int)a->content_box.width;
        area.height = (int)a->content_box.height;
        dark_after = count_dark_pixels_in_rect(&canvas, area);
        ctest_equal_bool("blur should keep typed text visible", dark_after > 0,
                         true);
        pd_canvas_destroy(&canvas);

        ui_widget_destroy(a);
        ui_widget_destroy(b);
        ui_clear_trash();
}

static void check_blur_keeps_input_group_text(void)
{
        ui_widget_t *group = ui_create_input_group();
        ui_widget_t *input = ui_create_input_group_input();
        ui_widget_t *notes = ui_create_input_group_textarea();
        pd_canvas_t canvas;
        pd_rect_t area;
        int dark_after;

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(group, "position", "absolute");
        ui_widget_set_style_string(group, "left", "20px");
        ui_widget_set_style_string(group, "top", "5px");
        ui_widget_set_style_string(group, "width", "180px");
        ui_widget_set_style_string(notes, "position", "absolute");
        ui_widget_set_style_string(notes, "left", "20px");
        ui_widget_set_style_string(notes, "top", "45px");
        ui_widget_set_style_string(notes, "width", "180px");
        ui_textinput_set_text_w(input, L"typed");
        ui_widget_append(group, input);
        ui_root_append(group);
        ui_root_append(notes);
        ui_update();
        ui_set_focus(input);
        ui_update();

        ui_set_focus(notes);
        ui_update();
        render_root_canvas(&canvas);
        area.x = (int)group->border_box.x;
        area.y = (int)group->border_box.y;
        area.width = (int)group->border_box.width;
        area.height = (int)group->border_box.height;
        dark_after = count_dark_pixels_in_rect(&canvas, area);
        ctest_equal_bool("input-group blur should keep text visible",
                         dark_after > 0, true);
        pd_canvas_destroy(&canvas);

        ui_widget_destroy(group);
        ui_widget_destroy(notes);
        ui_clear_trash();
}

static void check_selection_cleared_on_blur(void)
{
        ui_widget_t *a = ui_create_widget("textinput");
        ui_widget_t *b = ui_create_widget("textinput");
        pd_canvas_t before, after;
        pd_rect_t area;
        ui_event_t e = { 0 };
        int changed;

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(a, "position", "absolute");
        ui_widget_set_style_string(a, "left", "20px");
        ui_widget_set_style_string(a, "top", "5px");
        ui_widget_set_style_string(a, "width", "180px");
        ui_widget_set_style_string(b, "position", "absolute");
        ui_widget_set_style_string(b, "left", "20px");
        ui_widget_set_style_string(b, "top", "45px");
        ui_widget_set_style_string(b, "width", "180px");
        ui_textinput_enable_multiline(a, true);
        ui_textinput_set_text_w(a, L"abc");
        ui_textinput_set_text_w(b, L"def");
        ui_root_append(a);
        ui_root_append(b);
        ui_update();

        ui_set_focus(a);
        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_A;
        e.key.ctrl_key = true;
        ui_widget_emit_event(a, e, NULL);
        ui_update();
        render_root_canvas(&before);

        ui_set_focus(b);
        ui_update();
        render_root_canvas(&after);
        area.x = (int)a->content_box.x;
        area.y = (int)a->content_box.y;
        area.width = (int)a->content_box.width;
        area.height = (int)a->content_box.height;
        changed = count_diff_pixels_in_rect(&before, &after, area);
        ctest_equal_bool("blur should clear the text selection", changed > 100,
                         true);
        ctest_equal_bool("blur should keep the selected text visible",
                         count_dark_pixels_in_rect(&after, area) > 0, true);

        pd_canvas_destroy(&before);
        pd_canvas_destroy(&after);
        ui_widget_destroy(a);
        ui_widget_destroy(b);
        ui_clear_trash();
}

static void check_selection_css_class(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        pd_canvas_t before, after;
        pd_rect_t diff;
        pd_color_t pixel;
        ui_event_t e = { 0 };

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(w, "position", "absolute");
        ui_widget_set_style_string(w, "left", "20px");
        ui_widget_set_style_string(w, "top", "10px");
        ui_widget_set_style_string(w, "width", "180px");
        ui_textinput_set_text_w(w, L"abc");
        ui_root_append(w);
        ui_update();

        ui_load_css_string("textinput .selection { background-color: #00ff00; "
                           "color: #ff0000; }",
                           "test-selection-css");
        ui_refresh_style();
        ui_update();
        render_root_canvas(&before);

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_A;
        e.key.ctrl_key = true;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        render_root_canvas(&after);
        scan_diff_bbox(&before, &after, &diff);
        pixel = pd_canvas_get_pixel(&after, (unsigned)(diff.x + 1),
                                    (unsigned)(diff.y + 2));
        ctest_equal_bool("selection class should tint the selection",
                         pixel.g > 200 && pixel.r < 100 && pixel.b < 100, true);
        {
                int x, y, red = 0;

                for (y = diff.y; y < diff.y + diff.height; ++y) {
                        for (x = diff.x; x < diff.x + diff.width; ++x) {
                                pd_color_t p = pd_canvas_get_pixel(
                                    &after, (unsigned)x, (unsigned)y);
                                if (p.r > 200 && p.g < 80 && p.b < 80) {
                                        ++red;
                                }
                        }
                }
                ctest_equal_bool(
                    "selection class should let CSS set the text color",
                    red > 0, true);
        }

        ui_unload_css_string("test-selection-css");
        ui_refresh_style();
        ui_update();
        pd_canvas_destroy(&before);
        pd_canvas_destroy(&after);
        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_selection_accent_color(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        pd_canvas_t before, after;
        pd_rect_t diff;
        pd_color_t pixel;
        ui_event_t e = { 0 };

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(w, "position", "absolute");
        ui_widget_set_style_string(w, "left", "20px");
        ui_widget_set_style_string(w, "top", "10px");
        ui_widget_set_style_string(w, "width", "180px");
        ui_textinput_set_text_w(w, L"abc");
        ui_root_append(w);

        ui_theme_set_accent("blue");
        ui_refresh_style();
        ui_update();
        render_root_canvas(&before);

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_A;
        e.key.ctrl_key = true;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        render_root_canvas(&after);
        scan_diff_bbox(&before, &after, &diff);
        pixel = pd_canvas_get_pixel(&after, (unsigned)(diff.x + 1),
                                    (unsigned)(diff.y + 2));
        ctest_equal_bool("accent should tint the selection background",
                         pixel.b > pixel.r + 20, true);

        ui_theme_set_accent(NULL);
        ui_refresh_style();
        ui_update();
        pd_canvas_destroy(&before);
        pd_canvas_destroy(&after);
        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_selection_default_colors(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_widget_t *selection;

        ui_root_append(w);
        ui_update();
        selection = ui_widget_get_child(w, 1);
        ctest_equal_bool("default selection background should be solid primary",
                         selection->computed_style.background_color ==
                             css_color(255, 0x17, 0x17, 0x17),
                         true);
        ctest_equal_bool(
            "default selection text should use primary foreground",
            selection->computed_style.color == css_color(255, 0xfa, 0xfa, 0xfa),
            true);

        ui_widget_add_class(ui_root(), "dark");
        ui_update();
        ctest_equal_bool("dark selection background should be light",
                         selection->computed_style.background_color ==
                             css_color(255, 0xfa, 0xfa, 0xfa),
                         true);
        ctest_equal_bool(
            "dark selection text should be dark",
            selection->computed_style.color == css_color(255, 0x17, 0x17, 0x17),
            true);
        ui_widget_remove_class(ui_root(), "dark");
        ui_update();

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_selection_color_attr(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        pd_canvas_t before, after;
        pd_rect_t diff;
        pd_color_t pixel;
        ui_event_t e = { 0 };

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(w, "position", "absolute");
        ui_widget_set_style_string(w, "left", "20px");
        ui_widget_set_style_string(w, "top", "10px");
        ui_widget_set_style_string(w, "width", "180px");
        ui_widget_set_attr(w, "selection-color", "#00ff00");
        ui_textinput_set_text_w(w, L"abc");
        ui_root_append(w);
        ui_update();
        render_root_canvas(&before);

        e.type = UI_EVENT_KEYDOWN;
        e.key.code = KEY_A;
        e.key.ctrl_key = true;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        render_root_canvas(&after);
        scan_diff_bbox(&before, &after, &diff);
        pixel = pd_canvas_get_pixel(&after, (unsigned)(diff.x + 1),
                                    (unsigned)(diff.y + 2));
        ctest_equal_bool("selection-color attribute should tint the selection",
                         pixel.g > 200 && pixel.r < 100 && pixel.b < 100, true);

        pd_canvas_destroy(&before);
        pd_canvas_destroy(&after);
        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_first_char_caret(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        ui_widget_t *caret;
        ui_event_t e = { 0 };
        float empty_x, typed_x;

        ui_widget_set_style_unit_value(w, css_prop_width, 200, CSS_UNIT_PX);
        ui_root_append(w);
        ui_update();
        ui_set_focus(w);
        ui_update();
        caret = ui_widget_get_child(w, 0);
        empty_x = caret->border_box.x;

        ui_event_init(&e, "textinput");
        e.text.text = L"a";
        e.text.length = 1;
        ui_widget_emit_event(w, e, NULL);
        ui_update();
        typed_x = caret->border_box.x;
        ctest_equal_bool("caret should move after the first typed character",
                         typed_x > empty_x + 1.f, true);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_collapsed_layout_keeps_text(void)
{
        ui_widget_t *w = ui_create_widget("textinput");
        pd_canvas_t before, after;
        pd_rect_t bbox_before, bbox_after;

        ui_widget_resize(ui_root(), SEL_TEST_W, SEL_TEST_H);
        ui_widget_set_style_string(w, "position", "absolute");
        ui_widget_set_style_string(w, "left", "20px");
        ui_widget_set_style_string(w, "top", "10px");
        ui_widget_set_style_string(w, "width", "180px");
        ui_textinput_set_text_w(w, L"hello");
        ui_root_append(w);
        ui_update();

        render_root_canvas(&before);
        scan_dark_bbox(&before, &bbox_before);
        ctest_equal_bool("text should be visible before the collapse",
                         bbox_before.width > 0, true);

        /* A reflow can deliver a caret update while the widget reports a
         * zero-sized content box.  That must not queue a scroll offset which
         * the next update pass applies before the layout grows back. */
        {
                float width = w->content_box.width;
                float height = w->content_box.height;

                w->content_box.width = 0;
                w->content_box.height = 0;
                ui_textinput_move_caret(w, 0, 5);
                ui_widget_update(w);
                w->content_box.width = width;
                w->content_box.height = height;
        }

        render_root_canvas(&after);
        scan_dark_bbox(&after, &bbox_after);
        ctest_equal_bool("collapsed layout should not hide the text",
                         bbox_after.width > 0, true);

        pd_canvas_destroy(&before);
        pd_canvas_destroy(&after);

        ui_widget_destroy(w);
        ui_clear_trash();
}

static void check_caret_geometry_with_stale_metrics(void)
{
        ui_widget_t *w, *caret;
        float expect_x, expect_height;

        ui_metrics.dpi = CARET_TEST_DPI;
        w = create_scaled_textinput();
        caret = ui_widget_get_child(w, 0);
        ui_textinput_move_caret(w, 0, 5);
        ui_update();
        expect_x = caret->border_box.x;
        expect_height = caret->border_box.height;
        ctest_equal_bool("scaled caret should have a visible height",
                         expect_height > 0, true);

        /* 事件分发发生在 ui_server_update() 之外，此时全局 dpi 还是默认值 96，
         * caret 计算必须使用文本层的实际排版比例，不能跟随这个临时值 */
        ui_metrics.dpi = 96.f;
        ui_textinput_move_caret(w, 0, 5);
        ui_metrics.dpi = CARET_TEST_DPI;
        ui_update();
        ctest_equal_float("caret x should not apply the scale twice",
                          caret->border_box.x, expect_x);
        ctest_equal_float("caret height should not apply the scale twice",
                          caret->border_box.height, expect_height);
        destroy_scaled_textinput(w);
}

static void check_caret_pointer_mapping_at_scale(void)
{
        ui_widget_t *w, *caret;
        ui_event_t e = { 0 };
        float expect_x;

        ui_metrics.dpi = CARET_TEST_DPI;
        w = create_scaled_textinput();
        caret = ui_widget_get_child(w, 0);
        ui_textinput_move_caret(w, 0, 5);
        ui_update();
        expect_x = caret->border_box.x;
        ui_textinput_move_caret(w, 0, 0);
        ui_update();

        /* 模拟一次在高 DPI 窗口内、以逻辑坐标发出的点击 */
        ui_metrics.dpi = 96.f;
        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.x = w->border_box.x + expect_x + 1;
        e.mouse.y = w->border_box.y + w->computed_style.padding_top + 1;
        ui_widget_emit_event(w, e, NULL);
        ui_widget_release_mouse_capture(w);
        ui_metrics.dpi = CARET_TEST_DPI;
        ui_update();
        ctest_equal_float("pointer should select the same caret column",
                          caret->border_box.x, expect_x);
        destroy_scaled_textinput(w);
}

void test_textinput(void)
{
        ui_widget_t *w;
        wchar_t wcs[64];

        lcui_init();

        w = ui_create_widget("textinput");

        ui_textinput_set_text_w(w, L"hello");
        ui_widget_update(w);
        ctest_equal_bool("should match text length after set_text_w",
                         ui_textinput_get_text_length(w) == wcslen(L"hello"),
                         true);
        ctest_equal_bool(
            "should retrieve full text after set_text_w",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), true);
        ctest_equal_bool(
            "should retrieve identical wide string after set_text_w",
            wcscmp(L"hello", wcs) == 0, true);

        ui_widget_set_text(w, "hello");
        ui_widget_update(w);
        ctest_equal_bool("should match text length after set_text",
                         ui_textinput_get_text_length(w) == wcslen(L"hello"),
                         true);
        ctest_equal_bool(
            "should retrieve full text after set_text",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello"), true);
        ctest_equal_bool("should retrieve identical wide string after set_text",
                         wcscmp(L"hello", wcs) == 0, true);

        ui_textinput_append_text_w(w, L"world");
        ui_widget_update(w);
        ctest_equal_bool(
            "should match text length after append_text_w",
            ui_textinput_get_text_length(w) == wcslen(L"helloworld"), true);
        ctest_equal_bool(
            "should retrieve appended text via get_text_w",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"helloworld"),
            true);
        ctest_equal_bool("should retrieve identical appended wide string",
                         wcscmp(L"helloworld", wcs) == 0, true);

        ui_textinput_move_caret(w, 0, 5);
        ui_textinput_insert_text_w(w, L", ");
        ui_widget_update(w);
        ui_textinput_move_caret(w, 0, 100);
        ui_textinput_insert_text_w(w, L"!");
        ui_widget_update(w);
        ctest_equal_bool(
            "should match text length after inserting text",
            ui_textinput_get_text_length(w) == wcslen(L"hello, world!"), true);
        ctest_equal_bool(
            "should retrieve inserted text via get_text_w",
            ui_textinput_get_text_w(w, 0, 64, wcs) == wcslen(L"hello, world!"),
            true);
        ctest_equal_bool("should retrieve identical inserted wide string",
                         wcscmp(L"hello, world!", wcs) == 0, true);
        ui_widget_destroy(w);
        check_password_input();
        check_multiline_attr();
        ctest_describe("multiline editing",
                       check_multiline_enter_inserts_newline);
        ctest_describe("multiline autowrap", check_multiline_autowrap);
        ctest_describe("paste newline filter", check_paste_newline_filter);
        ctest_describe("edit roundtrip", check_edit_roundtrip_multiline);
        ctest_describe("password edit roundtrip",
                       check_password_edit_roundtrip);
        ctest_describe("undo redo", check_undo_redo);
        ctest_describe("read only", check_read_only);
        ctest_describe("password pointer", check_password_pointer_mapping);
        ctest_describe("password pointer mask width",
                       check_password_pointer_mask_width);
        ctest_describe("drag dirty", check_drag_marks_content_dirty);
        ctest_describe("selection alignment",
                       check_selection_background_alignment);
        ctest_describe("selection blur", check_selection_cleared_on_blur);
        ctest_describe("blur keeps text", check_blur_keeps_text);
        ctest_describe("blur keeps typed text", check_blur_keeps_typed_text);
        ctest_describe("blur keeps input-group text",
                       check_blur_keeps_input_group_text);
        ctest_describe("blur keeps long text", check_blur_keeps_long_text);
        ctest_describe("blur keeps placeholder text",
                       check_blur_keeps_text_with_placeholder);
        ctest_describe("selection color", check_selection_color_attr);
        ctest_describe("selection default colors",
                       check_selection_default_colors);
        ctest_describe("selection css", check_selection_css_class);
        ctest_describe("selection accent", check_selection_accent_color);
        ctest_describe("first char caret", check_first_char_caret);
        ctest_describe("edit dirty", check_edit_marks_content_dirty);
        ctest_describe("change event", check_change_event);
        ctest_describe("parse change suppression",
                       check_parse_change_suppressed);
        ctest_describe("single-line api filter", check_single_line_api_filter);
        ctest_describe("noop change", check_noop_change);
        ctest_describe("page down", check_page_down);
        ctest_describe("change reentrancy", check_change_reentrancy);
        ctest_describe("password copy", check_password_copy_blocked);
        ctest_describe("password multiline caret",
                       check_password_multiline_caret);
        ctest_describe("collapsed layout keeps text",
                       check_collapsed_layout_keeps_text);
        ctest_describe("caret scale", check_caret_geometry_with_stale_metrics);
        ctest_describe("caret pointer scale",
                       check_caret_pointer_mapping_at_scale);
        lcui_destroy();
}
