/*
 * app/components/demo-provider.c - Shared interactive behaviour for
 * code-demo widgets.
 *
 * One DemoProvider is mounted as the parent prototype of every
 * `app/examples/<name>/index.tsx` demo (lcui-cli emits
 * `ui_create_widget_prototype("..._index", "demo_provider")`).
 * As the parent prototype, the `init` / `destroy` callbacks declared
 * here apply to every demo automatically; the per-example index.c only
 * needs to wire up its preview widget.
 *
 * The widget tree authored in TSX looks like:
 *
 *   <DemoProvider>                             -> class="demo"
 *     <widget $ref="preview" />                -> class="demo-preview"
 *     <widget class="code-block">              -> wrapper card
 *       <widget class="code-block-header">     -> flex row
 *         <widget class="demo-files">          -> inline-block file tabs
 *           <text class="demo-tab" data-language="..." data-value="..." />
 *         </widget>
 *         <widget class="demo-languages">      -> inline-block lang tabs
 *           <text class="demo-tab" data-value="c|xml|tsx" />
 *         </widget>
 *         <CodeBlockCopy />                    -> single copy button
 *       </widget>
 *       <widget class="code-block-body" data-language="..." data-file="..."
 * data-source="..."> <text class="code-line" />*
 *       </widget>
 *       ... more (language, file) bodies as flat siblings ...
 *     </widget>
 *   </DemoProvider>
 *
 * State-driven model:
 *   1. Click / ready handlers write to `that->language` and
 *      `that->file` (the single source of truth), then call
 *      `demo_provider_update(w)`.
 *   2. `demo_provider_update` reads state and renders the DOM:
 *      - Step 1: activate the language tab matching state.
 *      - Step 2: show/hide file tabs by language, activate the file
 *        tab matching state.  When the file doesn't match any
 *        visible tab, fall back to the first file of the current
 *        language and write back into `that->file` so the state
 *        always reflects the UI truth.
 *      - Step 3: show the code-block body matching (language, file),
 *        hide the rest.
 *
 * Pointer ownership: `language` and `file` point at tab widget
 * `data-value` attributes.  LCUI widget attributes are stable for
 * the widget lifetime, so no copy is needed.
 */

#include <string.h>
#include "demo-provider.tsx.h"
#include "demo-provider.h"

typedef struct {
        demo_provider_react_t base;
        const char *language;
        const char *file;
} demo_provider_t;

/* -- helpers ------------------------------------------------------------- */

static ui_widget_t *find_child_by_class(ui_widget_t *parent, const char *cls)
{
        ui_widget_t *child = ui_widget_get_child(parent, 0);

        while (child) {
                if (ui_widget_has_class(child, cls)) {
                        return child;
                }
                child = ui_widget_next(child);
        }
        return NULL;
}

/* -- state-driven renderer ----------------------------------------------- */

void demo_provider_update(ui_widget_t *w)
{
        demo_provider_t *that = ui_widget_get_data(w, demo_provider_proto);
        const char *language = that->language;
        const char *file = that->file;
        ui_widget_t *wrapper;
        ui_widget_t *header;
        ui_widget_t *languages;
        ui_widget_t *files;
        ui_widget_t *child;
        ui_widget_t *first_for_lang = NULL;
        int file_matched = 0;

        demo_provider_react_update(w);

        /* Step 0: locate containers. */
        wrapper = find_child_by_class(w, "code-block");
        header = find_child_by_class(wrapper, "code-block-header");
        languages = find_child_by_class(header, "demo-languages");
        files = find_child_by_class(header, "demo-files");

        /* Step 1: activate the language tab matching `language`. */
        for (child = ui_widget_get_child(languages, 0); child;
             child = ui_widget_next(child)) {
                const char *v = ui_widget_get_attr(child, "data-value");
                if (v && strcmp(v, language) == 0) {
                        ui_widget_add_class(child, "active");
                } else {
                        ui_widget_remove_class(child, "active");
                }
        }

        /* Step 2: show/hide file tabs by language, activate the one
         * matching `file`.  When no tab matches, remember the first
         * file belonging to the current language and activate it. */
        for (child = ui_widget_get_child(files, 0); child;
             child = ui_widget_next(child)) {
                const char *la = ui_widget_get_attr(child, "data-language");
                const char *va = ui_widget_get_attr(child, "data-value");
                if (la && strcmp(la, language) == 0) {
                        ui_widget_show(child);
                        if (!first_for_lang) {
                                first_for_lang = child;
                        }
                        if (file && va && strcmp(va, file) == 0) {
                                ui_widget_add_class(child, "active");
                                file_matched = 1;
                        } else {
                                ui_widget_remove_class(child, "active");
                        }
                } else {
                        ui_widget_hide(child);
                        ui_widget_remove_class(child, "active");
                }
        }

        /* Fallback: file didn't match — adopt the first file of the
         * current language and write back into state so the next
         * render is consistent. */
        if (!file_matched && first_for_lang) {
                that->file = ui_widget_get_attr(first_for_lang, "data-value");
                file = that->file;
                ui_widget_add_class(first_for_lang, "active");
        }

        /* Step 3: show the code-block body matching (language, file)
         * and hide every other body. */
        for (child = ui_widget_get_child(wrapper, 0); child;
             child = ui_widget_next(child)) {
                const char *la = ui_widget_get_attr(child, "data-language");
                const char *fi = ui_widget_get_attr(child, "data-file");
                if (la && fi && ui_widget_has_class(child, "code-block-body") &&
                    strcmp(la, language) == 0 && strcmp(fi, file) == 0) {
                        ui_widget_show(child);
                        ui_widget_add_class(child, "active");
                } else if (fi) {
                        ui_widget_hide(child);
                        ui_widget_remove_class(child, "active");
                }
        }
}

/* -- event handlers ------------------------------------------------------ */

static void on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        demo_provider_t *that = ui_widget_get_data(w, demo_provider_proto);
        ui_widget_t *target = e->target;
        ui_widget_t *parent = target->parent;

        if (ui_widget_has_class(parent, "demo-languages")) {
                that->language = ui_widget_get_attr(target, "data-value");
                that->file = NULL;
                demo_provider_update(w);
                return;
        }
        if (ui_widget_has_class(parent, "demo-files")) {
                that->language = ui_widget_get_attr(target, "data-language");
                that->file = ui_widget_get_attr(target, "data-value");
                demo_provider_update(w);
                return;
        }
}

static void on_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        demo_provider_t *that = ui_widget_get_data(w, demo_provider_proto);
        ui_widget_t *wrapper = find_child_by_class(w, "code-block");
        ui_widget_t *header = find_child_by_class(wrapper, "code-block-header");
        ui_widget_t *languages = find_child_by_class(header, "demo-languages");
        ui_widget_t *first_tab = ui_widget_get_child(languages, 0);

        that->language = ui_widget_get_attr(first_tab, "data-value");
        that->file = NULL;
        demo_provider_update(w);
}

/* -- prototype glue ------------------------------------------------------ */

static void demo_provider_init(ui_widget_t *w)
{
        demo_provider_t *that;

        that =
            ui_widget_add_data(w, demo_provider_proto, sizeof(demo_provider_t));
        that->language = NULL;
        that->file = NULL;
        demo_provider_react_init(w);
        ui_widget_on(w, "click", on_click, NULL);
        ui_widget_on(w, "ready", on_ready, NULL);
}

static void demo_provider_destroy(ui_widget_t *w)
{
        demo_provider_react_destroy(w);
}

ui_widget_t *ui_create_demo_provider(void)
{
        return ui_create_widget_with_prototype(demo_provider_proto);
}

void ui_register_demo_provider(void)
{
        demo_provider_init_prototype();
        demo_provider_proto->init = demo_provider_init;
        demo_provider_proto->destroy = demo_provider_destroy;
}
