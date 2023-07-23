#include <LCUI.h>
#include <LCUI/main.h>
#include <time.h>
#include "ui_tasklist.h"

static tasklist_t tasks;

void update_title(void)
{
        char str[64];
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        strftime(str, 64, "%a %b %d %Y", tm);
        ui_text_set_content(ui_get_widget("title"), str);
}

void update_count(void)
{
        wchar_t text[32];

        swprintf(text, 32, tasks.length > 1 ? L"%u tasks" : L"%u task",
                 tasks.length);
        ui_text_set_content_w(ui_get_widget("count"), text);
}

void update_filter_status(const char *status)
{
        ui_widget_t *child;
        const char *attr;

        if (strcmp(status, "active") == 0) {
                ui_tasklist_filter(0);
        } else if (strcmp(status, "completed") == 0) {
                ui_tasklist_filter(1);
        } else {
                ui_tasklist_filter(2);
        }
        for (child = ui_widget_get_child(ui_get_widget("filters"), 0);
             child != NULL; child = ui_widget_next(child)) {
                attr = ui_widget_get_attr(child, "data-value");
                if (attr != NULL && strcmp(attr, status) == 0) {
                        ui_widget_add_class(child, "is-active");
                } else {
                        ui_widget_remove_class(child, "is-active");
                }
        }
        update_count();
}

void on_input_keydown(ui_widget_t *w, ui_event_t *e, void *arg)
{
        wchar_t name[256];

        if (e->key.code == KEY_ENTER) {
                ui_textedit_get_text_w(w, 0, 255, name);
                ui_tasklist_append(name, false);
                ui_textedit_clear_text(w);
        }
}

void on_filter_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *status = ui_widget_get_attr(e->target, "data-value");

        if (status != NULL) {
                update_filter_status(status);
        }
}

int main(int argc, char **argv)
{
        ui_widget_t *pack;

        lcui_init();
        pack = ui_load_xml_file("todolist.xml");
        if (!pack) {
                return -1;
        }
        tasklist_append(&tasks, L"Download LCUI source code", true);
        tasklist_append(&tasks, L"Build LCUI", true);
        tasklist_append(&tasks, L"Read LCUI tutorials", false);
        tasklist_append(&tasks, L"Create my LCUI application", false);

        ui_root_append(pack);
        ui_widget_unwrap(pack);
        ui_widget_set_title(ui_root(), L"Todo list");
        ui_tasklist_init(ui_get_widget("list"), &tasks);
        ui_widget_on(ui_get_widget("input"), "keydown", on_input_keydown, NULL);
        ui_widget_on(ui_get_widget("filters"), "click", on_filter_click, NULL);
        ui_widget_on(ui_get_widget("list"), "update",
                     (ui_event_handler_t)update_count, NULL);

        update_title();
        update_filter_status("all");
        return lcui_main();
}
