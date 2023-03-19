
#include <string.h>
#include <ui/base.h>
#include <ui/prototype.h>
#include "ui_widget_prototype.h"

static dict_t *ui_widget_prototype_dict;

static void widget_default_method(ui_widget_t *w)
{
}

static void widget_default_attr_setter(ui_widget_t *w, const char *name,
				       const char *value)
{
}

static void widget_default_text_setter(ui_widget_t *w, const char *text)
{
}

static void widget_default_size_getter(ui_widget_t *w, float *width,
				       float *height, ui_layout_rule_t rule)
{
}

static void widget_default_size_setter(ui_widget_t *w, float width,
				       float height)
{
}

static void widget_default_task_handler(ui_widget_t *w, int task)
{
}

static void widget_default_painter(ui_widget_t *w, pd_context_t *paint,
				   ui_widget_actual_style_t *style)
{
}

static ui_widget_prototype_t ui_default_widget_prototype = {
	.name = NULL,
	.init = widget_default_method,
	.refresh = widget_default_method,
	.destroy = widget_default_method,
	.update = widget_default_method,
	.runtask = widget_default_task_handler,
	.setattr = widget_default_attr_setter,
	.settext = widget_default_text_setter,
	.autosize = widget_default_size_getter,
	.resize = widget_default_size_setter,
	.paint = widget_default_painter
};

static void ui_widget_prototype_dict_val_destructor(void *privdata, void *data)
{
	ui_widget_prototype_t *proto = data;
	free(proto->name);
	free(proto);
}

ui_widget_prototype_t *ui_create_widget_prototype(const char *name,
						  const char *parent_name)
{
	ui_widget_prototype_t *proto;
	ui_widget_prototype_t *parent;

	if (dict_fetch_value(ui_widget_prototype_dict, name)) {
		return NULL;
	}
	proto = malloc(sizeof(ui_widget_prototype_t));
	if (parent_name) {
		parent =
		    dict_fetch_value(ui_widget_prototype_dict, parent_name);
		if (parent) {
			*proto = *parent;
			proto->proto = parent;
		} else {
			*proto = ui_default_widget_prototype;
		}
	} else {
		*proto = ui_default_widget_prototype;
	}
	proto->name = strdup2(name);
	if (dict_add(ui_widget_prototype_dict, proto->name, proto) == 0) {
		return proto;
	}
	free(proto->name);
	free(proto);
	return NULL;
}

ui_widget_prototype_t *ui_get_widget_prototype(const char *name)
{
	ui_widget_prototype_t *proto;

	if (!name) {
		return &ui_default_widget_prototype;
	}
	proto = dict_fetch_value(ui_widget_prototype_dict, name);
	if (!proto) {
		return &ui_default_widget_prototype;
	}
	return proto;
}

bool ui_check_widget_type(ui_widget_t *w, const char *type)
{
	const ui_widget_prototype_t *proto;

	if (!w || !w->type) {
		return false;
	}
	if (strcmp(w->type, type) == 0) {
		return true;
	}
	if (!w->proto) {
		return false;
	}
	for (proto = w->proto->proto; proto; proto = proto->proto) {
		if (strcmp(proto->name, type) == 0) {
			return true;
		}
	}
	return false;
}

bool ui_check_widget_prototype(ui_widget_t *w,
				    const ui_widget_prototype_t *proto)
{
	const ui_widget_prototype_t *p;
	for (p = w->proto; p; p = p->proto) {
		if (p == proto) {
			return true;
		}
	}
	return false;
}

void *ui_widget_get_data(ui_widget_t *widget, ui_widget_prototype_t *proto)
{
	unsigned i;
	if (!widget->data.list || !proto) {
		return NULL;
	}
	for (i = 0; i < widget->data.length; ++i) {
		if (widget->data.list[i].proto == proto) {
			return widget->data.list[i].data;
		}
	}
	return NULL;
}

void *ui_widget_add_data(ui_widget_t *widget, ui_widget_prototype_t *proto,
			 size_t data_size)
{
	void *data;
	ui_widget_data_entry_t *list;
	const size_t size = sizeof(ui_widget_data_entry_t);

	list = realloc(widget->data.list, size * (widget->data.length + 1));
	if (!list) {
		return NULL;
	}
	data = malloc(data_size);
	list[widget->data.length].data = data;
	list[widget->data.length].proto = proto;
	widget->data.list = list;
	widget->data.length += 1;
	return data;
}

void ui_widget_destroy_prototype(ui_widget_t *widget)
{
	if (widget->proto && widget->proto->destroy) {
		widget->proto->destroy(widget);
	}
	while (widget->data.length > 0) {
		widget->data.length -= 1;
		free(widget->data.list[widget->data.length].data);
	}
	if (widget->data.list) {
		free(widget->data.list);
	}
	if (widget->type && !widget->proto->name) {
		free(widget->type);
		widget->type = NULL;
	}
	widget->proto = NULL;
}

void ui_init_widget_prototype(void)
{
	static dict_type_t type;

	dict_init_string_key_type(&type);
	type.val_destructor = ui_widget_prototype_dict_val_destructor;
	ui_widget_prototype_dict = dict_create(&type, NULL);
}

void ui_destroy_widget_prototype(void)
{
	dict_destroy(ui_widget_prototype_dict);
	ui_widget_prototype_dict = NULL;
}
