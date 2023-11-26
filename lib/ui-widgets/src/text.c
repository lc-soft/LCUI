/*
 * text.c -- TextView widget for display text.
 *
 * Copyright (c) 2018-2020, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
                return TRUE;
        }
        return FALSE;
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
        float scale = ui_metrics.scale;

        list_create(&rects);
        pd_text_update(txt->layer, &rects);
        for (list_each(node, &rects)) {
                ui_convert_rect(node->data, &rect, 1.0f / scale);
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
        content_changed = (!txt->style.content != !style.content);
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
        txt->task.update_content = true;
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
                        txt->task.update_content = FALSE;
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
        txt->task.update_content = FALSE;
        txt->task.content = NULL;
        txt->content = NULL;
        txt->trimming = TRUE;
        txt->layer = pd_text_create();
        pd_text_set_autowrap(txt->layer, TRUE);
        pd_text_set_multiline(txt->layer, TRUE);
        pd_text_set_style_tag(txt->layer, TRUE);
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

static void ui_text_on_auto_size(ui_widget_t *w, float *width, float *height,
                                 ui_layout_rule_t rule)
{
        float max_width, max_height;
        ui_text_t *txt = ui_widget_get_data(w, ui_text.prototype);
        list_t rects;

        if (w->parent &&
            IS_CSS_FIXED_LENGTH(&w->parent->computed_style, width)) {
                max_width = w->parent->content_box.width -
                            (w->border_box.width - w->content_box.width);
        } else {
                max_width = 0;
        }
        switch (rule) {
        case UI_LAYOUT_RULE_FIXED_WIDTH:
                max_width = w->content_box.width;
                max_height = 0;
                break;
        case UI_LAYOUT_RULE_FIXED_HEIGHT:
                max_height = w->content_box.height;
                break;
        case UI_LAYOUT_RULE_FIXED:
                max_width = w->content_box.width;
                max_height = w->content_box.height;
                break;
        default:
                max_height = 0;
                break;
        }
        list_create(&rects);
        pd_text_set_fixed_size(txt->layer, 0, 0);
        pd_text_set_max_size(txt->layer, ui_compute(max_width),
                             ui_compute(max_height));
        pd_text_update(txt->layer, &rects);
        *width = pd_text_get_width(txt->layer) / ui_metrics.scale;
        *height = pd_text_get_height(txt->layer) / ui_metrics.scale;
        pd_rects_clear(&rects);
}

static void ui_text_on_resize(ui_widget_t *w, float width, float height)
{
        float scale = ui_metrics.scale;
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
                ui_convert_rect(node->data, &rect, 1.0f / scale);
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
        ;

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

        if (ui_widget_has_class(w, "fui-icon-regular")) {
                _DEBUG_MSG("set content\n");
        }
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
        txt->task.update_content = TRUE;
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
        if (wstr) {
                free(wstr);
        }
        return ret;
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
        ui_text.prototype->autosize = ui_text_on_auto_size;
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
