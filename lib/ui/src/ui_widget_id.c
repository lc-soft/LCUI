
#include <LCUI.h>
#include <LCUI/thread.h>
#include "../include/ui.h"
#include "internal.h"

static dict_t* ui_widget_id_dict;
static LCUI_Mutex ui_widget_id_dict_mutex;

int ui_widget_destroy_id(ui_widget_t* w)
{
	list_t *list;
	list_node_t *node;

	if (!w->id) {
		return -1;
	}
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	list = dict_fetch_value(ui_widget_id_dict, w->id);
	if (!list) {
		LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
		return -2;
	}
	for (list_each(node, list)) {
		if (node->data == w) {
			free(w->id);
			w->id = NULL;
			list_unlink(list, node);
			free(node);
			LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
			return 0;
		}
	}
	LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
	return -3;
}

int ui_widget_set_id(ui_widget_t* w, const char *idstr)
{
	list_t *list;

	ui_widget_destroy_id(w);
	if (!idstr) {
		return -1;
	}
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	w->id = strdup2(idstr);
	if (!w->id) {
		goto error_exit;
	}
	list = dict_fetch_value(ui_widget_id_dict, w->id);
	if (!list) {
		list = malloc(sizeof(list_t));
		if (!list) {
			goto error_exit;
		}
		list_create(list);
		if (dict_add(ui_widget_id_dict, w->id, list) != 0) {
			free(list);
			goto error_exit;
		}
	}
	if (!list_append(list, w)) {
		goto error_exit;
	}
	LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
	return 0;

error_exit:
	LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
	if (w->id) {
		free(w->id);
		w->id = NULL;
	}
	return -2;
}

ui_widget_t* ui_get_widget(const char *id)
{
	list_t *list;
	ui_widget_t* w = NULL;

	if (!id) {
		return NULL;
	}
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	list = dict_fetch_value(ui_widget_id_dict, id);
	if (list) {
		w = list_get(list, 0);
	}
	LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
	return w;
}


static void ui_widget_id_dict_val_destructor(void *privdata, void *data)
{
	list_t *list = data;
	list_destroy(list, NULL);
	free(list);
}

void ui_init_widget_id(void)
{
	static dict_type_t type;
	LCUIMutex_Init(&ui_widget_id_dict_mutex);
	dict_init_string_copy_key_type(&type);
	type.val_destructor = ui_widget_id_dict_val_destructor;
	ui_widget_id_dict = dict_create(&type, NULL);
}

void ui_destroy_widget_id(void)
{
	dict_destroy(ui_widget_id_dict);
	LCUIMutex_Destroy(&ui_widget_id_dict_mutex);
	ui_widget_id_dict = NULL;
}
