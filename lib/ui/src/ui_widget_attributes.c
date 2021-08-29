#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

static void ui_widget_attribute_destructor(void *privdata, void *data)
{
	ui_widget_attribute_t* attr = data;

	if (attr->value.destructor) {
		attr->value.destructor(attr->value.data);
	}
	free(attr->name);
	attr->name = NULL;
	attr->value.data = NULL;
	free(attr);
}

int ui_widget_set_attribute_ex(ui_widget_t* w, const char *name, void *value,
			  int value_type, void (*value_destructor)(void *))
{
	ui_widget_attribute_t* attr;
	static DictType dict_type;
	static LCUI_BOOL dict_type_available = FALSE;

	if (!dict_type_available) {
		Dict_InitStringKeyType(&dict_type);
		dict_type.valDestructor = ui_widget_attribute_destructor;
		dict_type_available = TRUE;
	}
	if (!w->attributes) {
		w->attributes = Dict_Create(&dict_type, NULL);
	}
	attr = Dict_FetchValue(w->attributes, name);
	if (attr) {
		if (attr->value.destructor) {
			attr->value.destructor(attr->value.data);
		}
	} else {
		attr = NEW(ui_widget_attribute_t, 1);
		attr->name = strdup2(name);
		Dict_Add(w->attributes, attr->name, attr);
	}
	attr->value.data = value;
	attr->value.type = value_type;
	attr->value.destructor = value_destructor;
	return 0;
}

int ui_widget_set_attribute(ui_widget_t* w, const char *name, const char *value)
{
	int ret;
	char *value_str = NULL;

	if (value) {
		value_str = strdup2(value);
		if (!value_str) {
			return -ENOMEM;
		}
		if (strcmp(name, "disabled") == 0) {
			if (!value || strcmp(value, "false") != 0) {
				Widget_SetDisabled(w, TRUE);
			} else {
				Widget_SetDisabled(w, FALSE);
			}
		}
		ret = ui_widget_set_attribute_ex(w, name, value_str,
					    LCUI_STYPE_STRING, free);
	} else {
		ret = ui_widget_set_attribute_ex(w, name, NULL, LCUI_STYPE_NONE,
					    NULL);
	}
	if (w->proto && w->proto->setattr) {
		w->proto->setattr(w, name, value_str);
	}
	return ret;
}

const char *ui_widget_get_attribute_value(ui_widget_t* w, const char *name)
{
	ui_widget_attribute_t* attr;
	if (!w->attributes) {
		return NULL;
	}
	attr = Dict_FetchValue(w->attributes, name);
	if (attr) {
		return attr->value.string;
	}
	return NULL;
}

void ui_widget_destroy_attributes(ui_widget_t* w)
{
	if (w->attributes) {
		Dict_Release(w->attributes);
	}
	w->attributes = NULL;
}
