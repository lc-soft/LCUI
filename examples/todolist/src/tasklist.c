#include "tasklist.h"
#include <stdbool.h>
#include <stdlib.h>

void task_destroy(task_t *task)
{
        free(task->name);
        free(task);
}

void tasklist_init(tasklist_t *list)
{
        list_create(list);
}

void tasklist_destroy(tasklist_t *list)
{
        list_destroy(list, (list_item_destructor_t)task_destroy);
}

task_t *tasklist_append(tasklist_t *list, const wchar_t *name,
                        bool is_completed)
{
        static int id = 1;
        task_t *task = malloc(sizeof(task_t));

        task->id = id++;
        task->name = wcsdup(name);
        task->is_completed = is_completed;
	list_append(list, task);
        return task;
}

task_t *tasklist_find(tasklist_t *list, int id)
{
        task_t *task;
        list_node_t *node;

        for (list_each(node, list)) {
                task = node->data;
                if (task->id == id) {
                        return task;
                }
        }
        return NULL;
}

bool tasklist_remove(tasklist_t *list, int id)
{
        list_node_t *node;

        for (list_each(node, list)) {
                if (((task_t *)node->data)->id == id) {
                        task_destroy(node->data);
                        list_delete_node(list, node);
                        return true;
                }
        }
        return false;
}

void tasklist_empty(tasklist_t *list)
{
        list_destroy(list, (list_item_destructor_t)task_destroy);
}

void tasklist_filter(tasklist_t *list, int status, list_t *filtered_list)
{
        list_node_t *node;

        for (list_each(node, list)) {
                if ((status == 0 && ((task_t *)node->data)->is_completed) ||
                    (status == 1 && !((task_t *)node->data)->is_completed)) {
                        continue;
                }
                list_append(filtered_list, node->data);
        }
}
