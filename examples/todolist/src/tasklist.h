#include <stdbool.h>
#include <yutil.h>

typedef struct task_t {
        int id;
        bool is_completed;
        wchar_t *name;
} task_t;

typedef list_t tasklist_t;

void tasklist_init(tasklist_t *list);

task_t *tasklist_append(tasklist_t *list, const wchar_t *name,
                        bool is_completed);

task_t *tasklist_find(tasklist_t *list, int id);

bool tasklist_remove(tasklist_t *list, int id);

void tasklist_empty(tasklist_t *list);

void tasklist_filter(tasklist_t *list, int status, list_t *filtered_list);
