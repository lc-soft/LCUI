#include "ui_tasklist.h"
#include <stdio.h>
#include <ui_widgets.h>

static tasklist_t *tasklist;
static ui_widget_t *ui_tasklist;

ui_widget_t *ui_taskitem_create(task_t *task)
{
        char id[32] = { 0 };
        ui_widget_t *item = ui_create_widget("text");
        ui_widget_t *status = ui_create_widget(NULL);
        ui_widget_t *del = ui_create_widget(NULL);
        ui_widget_t *name = ui_create_widget("text");
        snprintf(id, 32, "%u", task->id);
        ui_text_set_content_w(name, task->name);
        ui_widget_set_attr(item, "data-id", id);
        ui_widget_add_class(item, "task-item");
        if (task->is_completed) {
                ui_widget_add_class(item, "is-completed");
        }
        ui_widget_add_class(status, "task-status");
        ui_widget_add_class(del, "task-delete");
        ui_widget_append(item, status);
        ui_widget_append(item, name);
        ui_widget_append(item, del);
        return item;
}

void ui_tasklist_filter(int status)
{
        list_node_t *node;
        list_t filtered_list;
        tasklist_init(&filtered_list);
        tasklist_filter(tasklist, status, &filtered_list);
        ui_widget_empty(ui_tasklist);
        for (list_each(node, &filtered_list)) {
                ui_widget_append(ui_tasklist, ui_taskitem_create(node->data));
        }
}

void ui_tasklist_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        int id;
        const char *id_str;
        task_t *task;
        ui_widget_t *item;
        ui_event_t ev;

        // 找到当前事件目标所属的任务部件
        for (item = e->target; !ui_widget_has_class(item, "task-item");
             item = item->parent)
                ;
        id_str = ui_widget_get_attr(item, "data-id");
        if (!id_str || sscanf(id_str, "%d", &id) != 1) {
                return;
        }
        if (ui_widget_has_class(e->target, "task-status")) {
                task = tasklist_find(tasklist, id);
                if (task != NULL) {
                        task->is_completed = !task->is_completed;
                        if (task->is_completed) {
                                ui_widget_add_class(item, "is-completed");
                        } else {
                                ui_widget_remove_class(item, "is-completed");
                        }
                }
        } else if (ui_widget_has_class(e->target, "task-delete")) {
                tasklist_remove(tasklist, id);
                ui_event_init(&ev, "update");
                ui_widget_emit_event(item->parent, ev, NULL);
                ui_widget_remove(item);
        }
}

void ui_tasklist_init(ui_widget_t *w, tasklist_t *data)
{
        ui_tasklist = w;
        tasklist = data;
        ui_widget_on(w, "click", ui_tasklist_on_click, NULL);
}

void ui_tasklist_append(const wchar_t *name, bool is_completed)
{
        ui_widget_append(ui_tasklist, ui_taskitem_create(tasklist_append(
                                          tasklist, name, is_completed)));
}
