/*
 * app/components/code-block-copy.c - Click handler for the Copy
 * button rendered by code-block-copy.tsx.
 *
 * The button lives inside `.code-block-body`, which itself sits
 * under the demo wrapper `.code-block` (demo case) or directly
 * under `.code-block` (mdx case).  On click, the handler checks
 * whether the body widget has a `data-source` attribute (demo case:
 * the attribute is on the body); if not, it walks up to the
 * `.code-block` ancestor and uses its `data-source`.  The raw UTF-8
 * source is then looked up in the generated code-snippets table
 * and written as wide characters to the system clipboard.
 *
 * After a successful copy the `copied` class is added to the button
 * widget and a 1.5s timeout removes it again.  global.css flips the
 * visibility of the inner `.copy-icon` / `.check-icon` based on that
 * class, so callers see the checkmark until the timer fires.
 * Back-to-back clicks (same or different button) cancel any pending
 * revert before starting a new timer, so the feedback always lines
 * up with the most recent action.
 */

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <LCUI.h>
#include <LCUI/widgets.h>
#include <ptk/clipboard.h>
#include <ptk/events.h>
#include <yutil/charset.h>
#include "code-block-copy.tsx.h"
#include "code-block-copy.h"
#include "code-snippets.h"

typedef struct {
        code_block_copy_react_t base;
} code_block_copy_t;

typedef struct {
        ui_widget_t *button;
        int timer_id;
} copy_feedback_t;

static copy_feedback_t g_feedback;

static ui_widget_t *find_ancestor_with_class(ui_widget_t *w, const char *cls)
{
        while (w) {
                if (ui_widget_has_class(w, cls)) {
                        return w;
                }
                w = w->parent;
        }
        return NULL;
}

static void revert_copied_class(void *arg)
{
        copy_feedback_t *fb = arg;
        if (fb->button) {
                ui_widget_remove_class(fb->button, "copied");
        }
        fb->button = NULL;
        fb->timer_id = 0;
}

static void code_block_copy_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *block;
        const char *id;
        const char *src;
        size_t len;
        size_t wlen;
        wchar_t *wbuf;

        block = find_ancestor_with_class(w, "code-block-body");
        id = ui_widget_get_attr(block, "data-source");
        if (!id) {
                block = find_ancestor_with_class(w, "code-block");
                id = ui_widget_get_attr(block, "data-source");
        }
        src = code_snippet_for(id);

        len = strlen(src);
        wbuf = malloc(sizeof(wchar_t) * (len + 1));
        wlen = decode_utf8(wbuf, src, len + 1);
        ptk_clipboard_set_text(wbuf, wlen);
        free(wbuf);

        if (g_feedback.timer_id) {
                ptk_clear_timeout(g_feedback.timer_id);
                if (g_feedback.button && g_feedback.button != w) {
                        ui_widget_remove_class(g_feedback.button, "copied");
                }
        }
        ui_widget_add_class(w, "copied");
        g_feedback.button = w;
        g_feedback.timer_id =
            ptk_set_timeout(1500, revert_copied_class, &g_feedback);
}

static void code_block_copy_init(ui_widget_t *w)
{
        ui_widget_add_data(w, code_block_copy_proto, sizeof(code_block_copy_t));
        code_block_copy_react_init(w);
}

static void code_block_copy_destroy(ui_widget_t *w)
{
        code_block_copy_react_destroy(w);
}

ui_widget_t *ui_create_code_block_copy(void)
{
        return ui_create_widget_with_prototype(code_block_copy_proto);
}

void ui_register_code_block_copy(void)
{
        code_block_copy_init_prototype();
        code_block_copy_proto->init = code_block_copy_init;
        code_block_copy_proto->destroy = code_block_copy_destroy;
}
