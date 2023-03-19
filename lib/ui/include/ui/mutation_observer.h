#ifndef LIB_UI_INCLUDE_UI_MUTATION_OBSERVER_H
#define LIB_UI_INCLUDE_UI_MUTATION_OBSERVER_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

typedef list_t ui_mutation_list_t;
typedef struct ui_mutation_observer_t ui_mutation_observer_t;

typedef enum ui_mutation_record_type_t {
	UI_MUTATION_RECORD_TYPE_NONE,
	UI_MUTATION_RECORD_TYPE_ATTRIBUTES,
	UI_MUTATION_RECORD_TYPE_PROPERTIES,
	UI_MUTATION_RECORD_TYPE_CHILD_LIST,
} ui_mutation_record_type_t;

typedef struct ui_mutation_record_t {
	ui_mutation_record_type_t type;
	ui_widget_t *target;
	list_t added_widgets;
	list_t removed_widgets;
	char *attribute_name;
	char *property_name;
} ui_mutation_record_t;

typedef void (*ui_mutation_observer_callback_t)(ui_mutation_list_t *,
						ui_mutation_observer_t *,
						void *);

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MutationObserverInit
 */
typedef struct ui_mutation_observer_init_t {
	bool child_list;
	bool subtree;
	bool properties;
	bool attributes;
} ui_mutation_observer_init_t;

LIBUI_PUBLIC ui_mutation_record_t *ui_mutation_record_create(
    ui_widget_t *widget, ui_mutation_record_type_t type);

LIBUI_PUBLIC ui_mutation_record_t *ui_mutation_record_duplicate(
    ui_mutation_record_t *source);

LIBUI_PUBLIC void ui_mutation_record_destroy(ui_mutation_record_t *mutation);

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver
 */
LIBUI_PUBLIC ui_mutation_observer_t *ui_mutation_observer_create(
    ui_mutation_observer_callback_t callback, void *callback_arg);

LIBUI_PUBLIC int ui_mutation_observer_observe(
    ui_mutation_observer_t *observer, ui_widget_t *w,
    ui_mutation_observer_init_t options);

LIBUI_PUBLIC void ui_mutation_observer_disconnect(
    ui_mutation_observer_t *observer);
LIBUI_PUBLIC void ui_mutation_observer_destroy(
    ui_mutation_observer_t *observer);
LIBUI_PUBLIC void ui_process_mutation_observers(void);
LIBUI_PUBLIC void ui_mutation_observer_add_record(
    ui_mutation_observer_t *observer, ui_mutation_record_t *record);

LIBUI_END_DECLS

#endif
