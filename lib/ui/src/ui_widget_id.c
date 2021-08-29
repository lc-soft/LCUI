
#include <LCUI.h>
#include <LCUI/thread.h>
#include "../include/ui.h"
#include "private.h"

static Dict* ui_widget_id_dict;
static LCUI_Mutex ui_widget_id_dict_mutex;

int ui_widget_destroy_id(ui_widget_t* w)
{
	int ret = 0;
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	LinkedList *list;
	LinkedListNode *node;

	if (!w->id) {
		return -1;
	}
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	list = Dict_FetchValue(ui_widget_id_dict, w->id);
	if (!list) {
		LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
		return -2;
	}
	for (LinkedList_Each(node, list)) {
		if (node->data == w) {
			free(w->id);
			w->id = NULL;
			LinkedList_Unlink(list, node);
			LinkedListNode_Delete(node);
			LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
			return 0;
		}
	}
	LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
	return -3;
}

int ui_widget_set_id(ui_widget_t* w, const char *idstr)
{
	LinkedList *list;

	ui_widget_destroy_id(w);
	if (!idstr) {
		return -1;
	}
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	w->id = strdup2(idstr);
	if (!w->id) {
		goto error_exit;
	}
	list = Dict_FetchValue(ui_widget_id_dict, w->id);
	if (!list) {
		list = malloc(sizeof(LinkedList));
		if (!list) {
			goto error_exit;
		}
		LinkedList_Init(list);
		if (Dict_Add(ui_widget_id_dict, w->id, list) != 0) {
			free(list);
			goto error_exit;
		}
	}
	if (!LinkedList_Append(list, w)) {
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
	LinkedList *list;
	ui_widget_t* w = NULL;

	if (!id) {
		return NULL;
	}
	LCUIMutex_Lock(&ui_widget_id_dict_mutex);
	list = Dict_FetchValue(ui_widget_id_dict, id);
	if (list) {
		w = LinkedList_Get(list, 0);
	}
	LCUIMutex_Unlock(&ui_widget_id_dict_mutex);
	return w;
}


static void ui_widget_id_dict_val_destructor(void *privdata, void *data)
{
	LinkedList *list = data;
	LinkedList_Clear(list, NULL);
	free(list);
}

void ui_init_widget_id(void)
{
	static DictType type;
	LCUIMutex_Init(&ui_widget_id_dict_mutex);
	Dict_InitStringCopyKeyType(&type);
	type.valDestructor = ui_widget_id_dict_val_destructor;
	ui_widget_id_dict = Dict_Create(&type, NULL);
}

void ui_destroy_widget_id(void)
{
	Dict_Release(ui_widget_id_dict);
	LCUIMutex_Destroy(&ui_widget_id_dict_mutex);
	ui_widget_id_dict = NULL;
}
