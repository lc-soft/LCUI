/*
 * lib/ui-widgets/src/text.c: -- TextView widget for display text.
 *
 * Copyright (c) 2018-2024, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pandagl.h>
#include <css.h>
#include <ui_widgets/text.h>
#include "textstyle.h"

typedef struct ui_text_task_t {
        wchar_t *content;
        bool update_content;
} ui_text_task_t;

typedef struct ui_text_t_ {
        wchar_t *content;
        bool trimming;
        ui_widget_t *widget;
        pd_text_t *layer;
        ui_text_style_t style;
        ui_text_task_t task;
        list_node_t node;
} ui_text_t;

static struct ui_text_module_t {
        list_t list;
        ui_widget_prototype_t *prototype;
} ui_text;

static bool parse_boolean(const char *str)
{
        if (strcmp(str, "on") == 0 && strcmp(str, "true") == 0 &&
            strcmp(str, "yes") == 0 && strcmp(str, "1") == 0) {
                return true;
        }
        return false;
}

static void ui_text_on_parse_attr(ui_widget_t *w, const char *name,
                                  const char *value)
{
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        if (strcmp(name, "trimming") == 0) {
                if (parse_boolean(value) != txt->trimming) {
                        txt->trimming = !txt->trimming;
                        ui_text_set_content_w(w, txt->content);
                }
                return;
        }
        if (strcmp(name, "multiline") == 0) {
                bool enable = parse_boolean(value);
                if (enable != txt->layer->mulitiline_enabled) {
                        ui_text_set_multiline(w, enable);
                }
        }
}

static void ui_text_on_parse_text(ui_widget_t *w, const char *text)
{
        ui_text_set_content(w, text);
}

static void ui_text_on_update_content(ui_widget_t *w)
{
        list_t rects;
        list_node_t *node;
        ui_rect_t rect;
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);
        float scale = ui_get_actual_scale();

        list_create(&rects);
        pd_text_update(txt->layer, &rects);
        for (list_each(node, &rects)) {
                ui_rect_from_pd_rect(&rect, node->data, scale);
                ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
        }
        pd_rects_clear(&rects);
        ui_widget_request_reflow(w);
}

static void ui_text_on_update_style(ui_widget_t *w)
{
        ui_text_style_t style;
        pd_text_style_t text_style;
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);
        bool content_changed;

        ui_text_style_init(&style);
        ui_compute_text_style(&style, &w->computed_style);
        if (ui_text_style_is_equal(&style, &txt->style)) {
                ui_text_style_destroy(&style);
                return;
        }
        content_changed = style.content || txt->style.content;
        convert_font_style_to_text_style(&style, &text_style);
        pd_text_set_align(txt->layer, style.text_align);
        pd_text_set_line_height(txt->layer, style.line_height);
        pd_text_set_autowrap(txt->layer,
                             style.white_space != CSS_WHITE_SPACE_NOWRAP);
        pd_text_set_word_break(txt->layer,
                               style.word_break == CSS_WORD_BREAK_BREAK_ALL
                                   ? PD_WORD_BREAK_BREAK_ALL
                                   : PD_WORD_BREAK_NORMAL);
        if (content_changed) {
                ui_text_set_content_w(w, style.content);
        }
        pd_text_set_style(txt->layer, &text_style);
        ui_text_style_destroy(&txt->style);
        pd_text_style_destroy(&text_style);
        txt->style = style;
}

static void ui_text_on_update(ui_widget_t *w, ui_task_type_t task)
{
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        switch (task) {
        case UI_TASK_UPDATE_STYLE:
                ui_text_on_update_style(w);
                break;
        case UI_TASK_AFTER_UPDATE:
                if (txt->task.update_content) {
                        pd_text_write(txt->layer, txt->task.content, NULL);
                        ui_text_on_update_content(w);
                        free(txt->task.content);
                        txt->task.content = NULL;
                        txt->task.update_content = false;
                }
        default:
                break;
        }
}

static void ui_text_on_init(ui_widget_t *w)
{
        ui_text_t *txt;

        txt = ui_widget_add_data(w, ui_text.prototype, sizeof(ui_text_t));
        txt->widget = w;
        txt->task.update_content = false;
        txt->task.content = NULL;
        txt->content = NULL;
        txt->trimming = true;
        txt->layer = pd_text_create();
        pd_text_set_autowrap(txt->layer, true);
        pd_text_set_multiline(txt->layer, true);
        pd_text_set_style_tag(txt->layer, true);
        ui_text_style_init(&txt->style);
        txt->node.data = txt;
        txt->node.prev = txt->node.next = NULL;
        list_append_node(&ui_text.list, &txt->node);
}

static void ui_text_on_destroy(ui_widget_t *w)
{
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        list_unlink(&ui_text.list, &txt->node);
        ui_text_style_destroy(&txt->style);
        pd_text_destroy(txt->layer);
        free(txt->content);
        if (txt->task.content) {
                free(txt->task.content);
                txt->task.content = NULL;
        }
}

static void ui_text_on_autosize(ui_widget_t *w, float *width, float *height)
{
        float max_width = 0, max_height = 0;
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);
        css_computed_style_t *s = &w->computed_style;
        list_t rects;

        if (IS_CSS_FIXED_LENGTH(s, width)) {
                max_width = css_width_to_content_box_width(s, s->width);
        } else if (ui_widget_get_max_width(w, &max_width)) {
                max_width = css_width_to_content_box_width(s, max_width);
        }
        if (IS_CSS_FIXED_LENGTH(s, height)) {
                max_height = css_height_to_content_box_height(s, s->height);
        } else if (ui_widget_get_max_height(w, &max_height)) {
                max_height = css_height_to_content_box_height(s, max_height);
        }
        list_create(&rects);
        pd_text_set_fixed_size(txt->layer, 0, 0);
        pd_text_set_max_size(txt->layer, ui_compute(max_width),
                             ui_compute(max_height));
        pd_text_update(txt->layer, &rects);
        *width = pd_text_get_width(txt->layer) / ui_get_actual_scale();
        *height = pd_text_get_height(txt->layer) / ui_get_actual_scale();
        pd_rects_clear(&rects);
}

static void ui_text_on_resize(ui_widget_t *w, float width, float height)
{
        float scale = ui_get_actual_scale();
        int fixed_width = (int)(width * scale);
        int fixed_height = (int)(height * scale);

        ui_rect_t rect;
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        list_t rects;
        list_node_t *node;

        list_create(&rects);
        pd_text_set_fixed_size(txt->layer, fixed_width, fixed_height);
        pd_text_set_max_size(txt->layer, fixed_width, fixed_height);
        pd_text_update(txt->layer, &rects);

        for (list_each(node, &rects)) {
                ui_rect_from_pd_rect(&rect, node->data, scale);
                ui_widget_mark_dirty_rect(w, &rect, UI_BOX_TYPE_CONTENT_BOX);
        }
        pd_rects_clear(&rects);
}

static void ui_text_on_paint(ui_widget_t *w, pd_context_t *paint,
                             ui_widget_actual_style_t *style)
{
        pd_pos_t pos;
        pd_canvas_t canvas;
        pd_rect_t content_rect, rect;
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        content_rect.width = style->content_box.width;
        content_rect.height = style->content_box.height;
        content_rect.x = style->content_box.x - style->canvas_box.x;
        content_rect.y = style->content_box.y - style->canvas_box.y;
        if (!pd_rect_overlap(&content_rect, &paint->rect, &rect)) {
                return;
        }
        pos.x = content_rect.x - rect.x;
        pos.y = content_rect.y - rect.y;
        rect.x -= paint->rect.x;
        rect.y -= paint->rect.y;
        pd_canvas_quote(&canvas, &paint->canvas, &rect);
        rect = paint->rect;
        rect.x -= content_rect.x;
        rect.y -= content_rect.y;
        pd_text_render_to(txt->layer, rect, pos, &canvas);
}

int ui_text_set_content_w(ui_widget_t *w, const wchar_t *text)
{
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);
        wchar_t *newtext = wcsdup2(text);

        if (!newtext) {
                return -ENOMEM;
        }
        if (txt->content) {
                free(txt->content);
        }
        txt->content = wcsdup2(text);
        if (!txt->content) {
                free(newtext);
                return -ENOMEM;
        }
        do {
                if (!text) {
                        newtext[0] = 0;
                        txt->content[0] = 0;
                        break;
                }
                wcscpy(txt->content, text);
                if (!txt->trimming) {
                        wcscpy(newtext, text);
                        break;
                }
                wcstrim(newtext, text, NULL);
        } while (0);
        if (txt->task.content) {
                free(txt->task.content);
        }
        txt->task.update_content = true;
        txt->task.content = newtext;
        ui_widget_request_update(w);
        return 0;
}

int ui_text_set_content(ui_widget_t *w, const char *utf8_text)
{
        int ret;
        wchar_t *wstr;
        size_t len = strlen(utf8_text) + 1;

        wstr = malloc(sizeof(wchar_t) * len);
        decode_utf8(wstr, utf8_text, len);
        ret = ui_text_set_content_w(w, wstr);
        free(wstr);
        return ret;
}

size_t ui_text_get_content_w(ui_widget_t *w, wchar_t *buf, size_t size)
{
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        if (buf && size > 0) {
                wcsncpy(buf, txt->content, size);
                buf[size - 1] = 0;
        }
        return wcslen(txt->content);
}

void ui_text_set_multiline(ui_widget_t *w, bool enable)
{
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);

        pd_text_set_multiline(txt->layer, enable);
        ui_widget_request_update(w);
}

static void text_on_font_face_load(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_text_t *txt;
        list_node_t *node;

        for (list_each(node, &ui_text.list)) {
                txt = node->data;
                if (txt->widget->state != UI_WIDGET_STATE_DELETED) {
                        ui_widget_request_refresh_style(txt->widget);
                }
        }
}

void ui_register_text(void)
{
        ui_text.prototype = ui_create_widget_prototype("text", NULL);
        ui_text.prototype->init = ui_text_on_init;
        ui_text.prototype->paint = ui_text_on_paint;
        ui_text.prototype->destroy = ui_text_on_destroy;
        ui_text.prototype->autosize = ui_text_on_autosize;
        ui_text.prototype->resize = ui_text_on_resize;
        ui_text.prototype->update = ui_text_on_update;
        ui_text.prototype->settext = ui_text_on_parse_text;
        ui_text.prototype->setattr = ui_text_on_parse_attr;
        list_create(&ui_text.list);
        ui_on_event("css_font_face_load", text_on_font_face_load, NULL);
}

void ui_unregister_text(void)
{
        list_destroy_without_node(&ui_text.list, NULL);
        ui_off_event("css_font_face_load", text_on_font_face_load, NULL);
}
