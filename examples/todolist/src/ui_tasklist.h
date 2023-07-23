#include <ui.h>
#include "tasklist.h"

void ui_tasklist_init(ui_widget_t *w, tasklist_t *data);

void ui_tasklist_filter(int status);

void ui_tasklist_append(const wchar_t *name, bool is_completed);
