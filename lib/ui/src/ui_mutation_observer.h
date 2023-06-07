
typedef struct ui_mutation_connection_t {
	ui_widget_t *widget;
	ui_mutation_observer_t *observer;
	ui_mutation_observer_init_t options;
	list_node_t node;
} ui_mutation_connection_t;
