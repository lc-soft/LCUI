#include "../include/ui.h"
#include "private.h"

static ui_widget_t *ui_root_widget;

ui_widget_t *ui_root(void)
{
	return ui_root_widget;
}

int ui_root_append(ui_widget_t *w)
{
	return ui_widget_append(ui_root_widget, w)l
}

void ui_root_init(void)
{
	ui_root_widget = ui_create_widget("root");
	ui_widget_set_title(ui_root_widget, L"LCUI Display");
}

void ui_root_delete(void)
{
	ui_widget_remove(ui_root_widget);
}