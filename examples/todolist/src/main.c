// Reference from: https://codepen.io/knyttneve/details/mddGVjB
#include <stdio.h>
#include <LCUI.h>
#include <platform/main.h>

typedef struct task_t {
        unsigned id;
        wchar_t *name;
        const char *status;
} task_t;

struct todolist_app_t {
        unsigned id;
        list_t tasks;
} app = { 0 };

ui_widget_t *ui_task_item_create(task_t *task)
{
        char id[32] = { 0 };
        ui_widget_t *item = ui_create_widget("text");
        ui_widget_t *status = ui_create_widget(NULL);
        ui_widget_t *del = ui_create_widget(NULL);
        ui_widget_t *name = ui_create_widget("text");
        snprintf(id, 32, "%u", task->id);
        ui_text_set_content_w(name, task->name);
        ui_widget_set_attribute(item, "data-id", id);
        ui_widget_add_class(item, "task-item");
        if (strcmp(task->status, "completed") == 0) {
                ui_widget_add_class(item, "is-completed");
        }
        ui_widget_add_class(status, "task-status");
        ui_widget_add_class(del, "task-delete");
        ui_widget_append(item, status);
        ui_widget_append(item, name);
        ui_widget_append(item, del);
        return item;
}

void ui_todolist_update_count(void)
{
        wchar_t text[32];
        ui_widget_t *count = ui_get_widget("count");

        swprintf(text, 32, app.tasks.length > 1 ? L"%u tasks" : L"%u task",
                 app.tasks.length);
        ui_text_set_content_w(count, text);
}

void update_filter_status(ui_widget_t *w, void *activeStatus)
{
        const char *status = ui_widget_get_attribute_value(w, "data-value");
        if (status && strcmp(status, activeStatus) == 0) {
                ui_widget_add_class(w, "is-active");
        } else {
                ui_widget_remove_class(w, "is-active");
        }
}

void ui_todolist_filter(const char *status)
{
        task_t *task;
        list_node_t *node;
        ui_widget_t *list = ui_get_widget("list");

        ui_widget_empty(list);
        for (list_each(node, &app.tasks)) {
                task = node->data;
                if (strcmp(status, "all") != 0 &&
                    strcmp(task->status, status) != 0) {
                        continue;
                }
                ui_widget_append(list, ui_task_item_create(task));
        }
        ui_widget_each(ui_get_widget("filters"), update_filter_status,
                       (void *)status);
        ui_todolist_update_count();
}

void ui_todolist_add(const wchar_t *name, const char *status)
{
        task_t *task = malloc(sizeof(task_t));

        task->id = ++app.id;
        task->name = wcsdup2(name);
        task->status = status ? status : "active";
        list_append(&app.tasks, task);
        ui_widget_append(ui_get_widget("list"), ui_task_item_create(task));
        ui_todolist_update_count();
}

void on_input_keydown(ui_widget_t *w, ui_event_t *e, void *arg)
{
        wchar_t name[256];

        if (e->key.code == KEY_ENTER) {
                ui_textedit_get_text_w(w, 0, 255, name);
                ui_todolist_add(name, "active");
                ui_textedit_clear_text(w);
        }
}

void on_filter_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *status = ui_widget_get_attribute_value(e->target, "data-value");

        if (status) {
                ui_todolist_filter(status);
        }
}

void on_task_list_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *id_str;
        unsigned id;
        task_t *task;
        list_node_t *node;
        ui_widget_t *item = e->target->parent;

        for (item = e->target; !ui_widget_has_class(item, "task-item");
             item = item->parent)
                ;
        id_str = ui_widget_get_attribute_value(item, "data-id");
        if (!id_str || sscanf(id_str, "%u", &id) != 1) {
                return;
        }
        if (ui_widget_has_class(e->target, "task-delete")) {
                ui_widget_remove(item);
                for (list_each(node, &app.tasks)) {
                        task = node->data;
                        if (task->id == id) {
                                list_delete_node(&app.tasks, node);
                                break;
                        }
                }
                ui_todolist_update_count();
                return;
        }
        if (!ui_widget_has_class(e->target, "task-status")) {
                return;
        }
        for (list_each(node, &app.tasks)) {
                task = node->data;
                if (task->id != id) {
                        continue;
                }
                if (strcmp(task->status, "completed") == 0) {
                        task->status = "active";
                        ui_widget_remove_class(item, "is-completed");
                        break;
                }
                task->status = "completed";
                ui_widget_add_class(item, "is-completed");
                break;
        }
}

void ui_todolist_init(void)
{
        ui_widget_on(ui_get_widget("input"), "keydown", on_input_keydown, NULL, NULL);
        ui_widget_on(ui_get_widget("filters"), "click", on_filter_click, NULL, NULL);
        ui_widget_on(ui_get_widget("list"), "click", on_task_list_click, NULL, NULL);
        ui_todolist_filter("all");
}

int main(int argc, char **argv)
{
        ui_widget_t *pack;

        lcui_init();
        pack = ui_load_xml_file("todolist.xml");
        if (!pack) {
                return -1;
        }
        ui_root_append(pack);
        ui_widget_unwrap(pack);
        ui_widget_set_title(ui_root(), L"Todo list");
        ui_todolist_init();
        ui_todolist_add(L"Download LCUI source code", "completed");
        ui_todolist_add(L"Build LCUI", "completed");
        ui_todolist_add(L"Read LCUI tutorials", "active");
        ui_todolist_add(L"Create my LCUI application", "active");
        return lcui_main();
}
