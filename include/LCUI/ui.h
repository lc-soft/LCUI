#ifndef LIB_UI_INCLUDE_UI_H
#define LIB_UI_INCLUDE_UI_H

#include <LCUI/def.h>
#include <yutil.h>
#include <LCUI/css/def.h>
#include <pandagl/types.h>
#include <LCUI/css/library.h>

LCUI_BEGIN_HEADER

// Types

typedef struct ui_rect_t {
	float x, y, width, height;
} ui_rect_t;

typedef struct ui_area_t {
	float left, top, right, bottom;
} ui_area_t;

typedef css_metrics_t ui_metrics_t;

typedef enum ui_density_Level_t {
	UI_DENSITY_LEVEL_SMALL,
	UI_DENSITY_LEVEL_NORMAL,
	UI_DENSITY_LEVEL_LARGE,
	UI_DENSITY_LEVEL_BIG
} ui_density_Level_t;

typedef enum ui_sizing_rule_t {
	UI_SIZING_RULE_NONE,
	UI_SIZING_RULE_FIXED,
	UI_SIZING_RULE_FILL,
	UI_SIZING_RULE_PERCENT,
	UI_SIZING_RULE_FIT_CONTENT
} ui_sizing_rule_t;

typedef enum ui_layout_rule_t {
	UI_LAYOUT_RULE_AUTO,
	UI_LAYOUT_RULE_MAX_CONTENT,
	UI_LAYOUT_RULE_FIXED_WIDTH = 0b11,
	UI_LAYOUT_RULE_FIXED_HEIGHT = 0b101,
	UI_LAYOUT_RULE_FIXED = 0b111
} ui_layout_rule_t;

typedef enum ui_widget_state_t {
	UI_WIDGET_STATE_CREATED = 0,
	UI_WIDGET_STATE_UPDATED,
	UI_WIDGET_STATE_LAYOUTED,
	UI_WIDGET_STATE_READY,
	UI_WIDGET_STATE_NORMAL,
	UI_WIDGET_STATE_DELETED,
} ui_widget_state_t;

typedef enum ui_dirty_rect_type_t {
	UI_DIRTY_RECT_TYPE_NONE,
	UI_DIRTY_RECT_TYPE_CUSTOM,
	UI_DIRTY_RECT_TYPE_PADDING_BOX,
	UI_DIRTY_RECT_TYPE_BORDER_BOX,
	UI_DIRTY_RECT_TYPE_CANVAS_BOX
} ui_dirty_rect_type_t;

typedef struct ui_border_style_t_ {
	struct {
		int style;
		float width;
		pd_color_t color;
	} top, right, bottom, left;
	float top_left_radius;
	float top_right_radius;
	float bottom_left_radius;
	float bottom_right_radius;
} ui_border_style_t;

typedef struct ui_widget_attribute_t_ {
	char *name;
	struct {
		int type;
		void (*destructor)(void *);
		union {
			char *string;
			void *data;
		};
	} value;
} ui_widget_attribute_t;

typedef struct ui_widget_actual_style_t_ {
	float x, y;
	pd_rect_t canvas_box;
	pd_rect_t border_box;
	pd_rect_t padding_box;
	pd_rect_t content_box;
} ui_widget_actual_style_t;

typedef enum ui_task_type_t {
	UI_TASK_REFRESH_STYLE,
	UI_TASK_UPDATE_STYLE,
	UI_TASK_REFLOW,
	UI_TASK_USER,
	UI_TASK_TOTAL_NUM
} ui_task_type_t;

typedef struct ui_event_listener_t ui_event_listener_t;
typedef struct ui_widget_t ui_widget_t;
typedef list_t ui_widget_listeners_t;

typedef void (*ui_widget_function_t)(ui_widget_t *);
typedef void (*ui_widget_task_handler_t)(ui_widget_t *, int);
typedef void (*ui_widget_size_getter_t)(ui_widget_t *, float *, float *,
					ui_layout_rule_t);
typedef void (*ui_widget_size_setter_t)(ui_widget_t *, float, float);
typedef void (*ui_widget_attr_setter_t)(ui_widget_t *, const char *,
					const char *);
typedef void (*ui_widget_text_setter_t)(ui_widget_t *, const char *);
typedef void (*ui_widget_painter_t)(ui_widget_t *, pd_context_t *,
				    ui_widget_actual_style_t *);
typedef struct ui_widget_prototype_t ui_widget_prototype_t;

typedef struct ui_widget_rules_t {
	/**
	 * Suspend update if the current widget is not visible or is
	 * completely covered by other widgets
	 */
	LCUI_BOOL only_on_visible;

	/**
	 * First update the children in the visible area
	 * If your widget has a lot of children and you want to see the
	 * children who are currently seeing the priority update, we recommend
	 * enabling this rule.
	 */
	LCUI_BOOL first_update_visible_children;

	/**
	 * Cache the stylesheets of children to improve the query speed of
	 * the stylesheet.
	 * If this rule is enabled, we recommend that you manually call
	 * ui_widget_generate_hash() to generate a hash value for the children
	 * of the widget.
	 */
	LCUI_BOOL cache_children_style;

	/** Refresh the style of all child widgets if the status has changed */
	LCUI_BOOL ignore_status_change;

	/** Refresh the style of all child widgets if the classes has changed */
	LCUI_BOOL ignore_classes_change;

	/**
	 * Maximum number of children updated at each update
	 * values:
	 * 0 - Update all children at once
	 * -1  - Automatically calculates the appropriate maximum number
	 * N  - Custom maximum number
	 */
	int max_update_children_count;

	/** Limit the number of children rendered  */
	unsigned max_render_children_count;

	/** A callback function on update progress */
	void (*on_update_progress)(ui_widget_t *, size_t);
} ui_widget_rules_t;

typedef struct ui_widget_update_t {
	/** Should update for self? */
	LCUI_BOOL for_self;

	/** Should update for children? */
	LCUI_BOOL for_children;

	/** Should skip the property sync of bound surface? */
	LCUI_BOOL skip_surface_props_sync;

	/** States of tasks */
	LCUI_BOOL states[UI_TASK_TOTAL_NUM];
} ui_widget_update_t;

typedef struct ui_profile_t {
	long time;
	size_t update_count;
	size_t refresh_count;
	size_t layout_count;
	size_t user_task_count;
	size_t destroy_count;
	size_t destroy_time;
} ui_profile_t;

struct ui_widget_prototype_t {
	char *name;
	ui_widget_function_t init;
	ui_widget_function_t refresh;
	ui_widget_function_t destroy;
	ui_widget_function_t update;
	ui_widget_task_handler_t runtask;
	ui_widget_attr_setter_t setattr;
	ui_widget_text_setter_t settext;
	ui_widget_size_getter_t autosize;
	ui_widget_size_setter_t resize;
	ui_widget_painter_t paint;
	ui_widget_prototype_t *proto;
};

typedef struct ui_widget_data_entry_t {
	void *data;
	ui_widget_prototype_t *proto;
} ui_widget_data_entry_t;

typedef struct ui_widget_data_t {
	unsigned length;
	ui_widget_data_entry_t *list;
} ui_widget_data_t;

// Event begin

typedef enum ui_event_type_t {
	UI_EVENT_NONE,
	UI_EVENT_LINK,
	UI_EVENT_UNLINK,
	UI_EVENT_READY,
	UI_EVENT_DESTROY,
	UI_EVENT_FOCUS,
	UI_EVENT_BLUR,
	UI_EVENT_AFTERLAYOUT,
	UI_EVENT_KEYDOWN,
	UI_EVENT_KEYUP,
	UI_EVENT_KEYPRESS,
	UI_EVENT_TEXTINPUT,

	UI_EVENT_MOUSEOVER,
	UI_EVENT_MOUSEMOVE,
	UI_EVENT_MOUSEOUT,
	UI_EVENT_MOUSEDOWN,
	UI_EVENT_MOUSEUP,
	UI_EVENT_WHEEL,
	UI_EVENT_CLICK,
	UI_EVENT_DBLCLICK,
	UI_EVENT_TOUCH,
	UI_EVENT_TOUCHDOWN,
	UI_EVENT_TOUCHUP,
	UI_EVENT_TOUCHMOVE,
	UI_EVENT_PASTE,

	UI_EVENT_FONT_FACE_LOAD,
	UI_EVENT_USER
} ui_event_type_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent
 */
typedef struct ui_keyboard_event_t {
	int code;
	LCUI_BOOL alt_key;
	LCUI_BOOL ctrl_key;
	LCUI_BOOL shift_key;
	LCUI_BOOL meta_key;
	LCUI_BOOL is_composing;
} ui_keyboard_event_t;

typedef struct ui_touch_point_t {
	float x;
	float y;
	int id;
	int state;
	LCUI_BOOL is_primary;
} ui_touch_point_t;

typedef struct ui_touch_event_t {
	unsigned n_points;
	ui_touch_point_t *points;
} ui_touch_event_t;

typedef struct ui_paint_event_t {
	pd_rect_t rect;
} ui_paint_event_t;

typedef struct ui_textinput_event_t {
	wchar_t *text;
	size_t length;
} ui_textinput_event_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent
 */
typedef struct ui_mouse_event_t {
	float x;
	float y;
	int button;
} ui_mouse_event_t;

typedef enum ui_wheel_delta_mode_t {
	UI_WHEEL_DELTA_PIXEL = 0,
	UI_WHEEL_DELTA_LINE,
	UI_WHEEL_DELTA_PAGE
} ui_wheel_delta_mode_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/WheelEvent
 */
typedef struct ui_wheel_event_t {
	int delta_x;
	int delta_y;
	int delta_z;
	int delta_mode;
} ui_wheel_event_t;

typedef struct ui_event_t ui_event_t;
typedef void (*ui_event_handler_t)(ui_widget_t *, ui_event_t *, void *);
typedef void (*ui_event_arg_destructor_t)(void *);

struct ui_event_t {
	uint32_t type;           /**< 事件类型标识号 */
	void *data;              /**< 附加数据 */
	ui_widget_t *target;     /**< 触发事件的部件 */
	LCUI_BOOL cancel_bubble; /**< 是否取消事件冒泡 */
	union {
		ui_mouse_event_t mouse;
		ui_wheel_event_t wheel;
		ui_keyboard_event_t key;
		ui_touch_event_t touch;
		ui_textinput_event_t text;
	};
};

// Event end

// MutationObserver begin

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
	LCUI_BOOL child_list;
	LCUI_BOOL subtree;
	LCUI_BOOL properties;
	LCUI_BOOL attributes;
} ui_mutation_observer_init_t;

LCUI_API ui_mutation_record_t *ui_mutation_record_create(
    ui_widget_t *widget, ui_mutation_record_type_t type);

LCUI_API ui_mutation_record_t *ui_mutation_record_duplicate(
    ui_mutation_record_t *source);

LCUI_API void ui_mutation_record_destroy(ui_mutation_record_t *mutation);

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver
 */
LCUI_API ui_mutation_observer_t *ui_mutation_observer_create(
    ui_mutation_observer_callback_t callback, void *callback_arg);

LCUI_API int ui_mutation_observer_observe(ui_mutation_observer_t *observer,
					  ui_widget_t *w,
					  ui_mutation_observer_init_t options);

LCUI_API void ui_mutation_observer_disconnect(ui_mutation_observer_t *observer);
LCUI_API void ui_mutation_observer_destroy(ui_mutation_observer_t *observer);
LCUI_API void ui_process_mutation_observers(void);
LCUI_API void ui_mutation_observer_add_record(ui_mutation_observer_t *observer,
					      ui_mutation_record_t *record);

// MutationObserver end

typedef struct ui_widget_extra_data_t {
	ui_widget_listeners_t listeners;
	list_t observer_connections;
	ui_widget_rules_t rules;
	size_t default_max_update_count;
	size_t update_progress;
	dict_t *style_cache;
} ui_widget_extra_data_t;

struct ui_widget_t {
	unsigned hash;
	ui_widget_state_t state;

	char *id;
	char *type;
	int tab_index;
	strlist_t classes;
	strlist_t status;
	wchar_t *title;
	dict_t *attributes;
	LCUI_BOOL disabled;
	LCUI_BOOL event_blocked;

	/**
	 * Coordinates calculated by the layout system
	 * The position of the rectangular boxes is calculated based on it
	 */
	float layout_x, layout_y;

	/**
	 * A box’s “ideal” size in a given axis when given infinite available
	 * space. See more: https://drafts.csswg.org/css-sizing-3/#max-content
	 */
	float max_content_width, max_content_height;

	/** See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Box_Model */
	ui_rect_t content_box;
	ui_rect_t padding_box;
	ui_rect_t border_box;
	ui_rect_t canvas_box;
	ui_rect_t outer_box;

	css_style_decl_t *custom_style;
	const css_style_decl_t *matched_style;
	css_computed_style_t specified_style;
	css_computed_style_t computed_style;

	/** Some data bound to the prototype */
	ui_widget_data_t data;
	ui_widget_extra_data_t *extra;

	/**
	 * Prototype chain
	 * It is used to implement the inheritance of widgets,
	 * Just like prototype chain in JavaScript
	 */
	const ui_widget_prototype_t *proto;

	ui_widget_update_t update;

	ui_rect_t dirty_rect;
	ui_dirty_rect_type_t dirty_rect_type;
	LCUI_BOOL has_child_dirty_rect;

	/** Parent widget */
	ui_widget_t *parent;

	/** List of child widgets */
	list_t children;

	/**
	 * List of child widgets in descending order by z-index
	 * @see
	 *https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Positioning/Understanding_z_index/The_stacking_context
	 **/
	list_t stacking_context;

	/**
	 * Position in the parent->children
	 * this == list_get(&this->parent->children, this.index)
	 */
	size_t index;

	/**
	 * Node in the parent->children
	 * &this->node == list_get_node(&this->parent->children, this.index)
	 */
	list_node_t node;

	/** Node in the parent->children_shoa */
	list_node_t node_show;
};

// Base

LCUI_API void ui_init(void);
LCUI_API void ui_destroy(void);
LCUI_API int ui_dispatch_event(ui_event_t *e);
LCUI_API void ui_process_events(void);

// Metrics

LCUI_API ui_metrics_t ui_metrics;

INLINE int ui_compute(float value)
{
	return (int)(ui_metrics.scale * value);
}

INLINE void ui_compute_rect(pd_rect_t *actual_rect, const ui_rect_t *rect)
{
	actual_rect->x = ui_compute(rect->x);
	actual_rect->y = ui_compute(rect->y);
	actual_rect->width = ui_compute(rect->width);
	actual_rect->height = ui_compute(rect->height);
}

/** 设置密度 */
LCUI_API void ui_set_density(float density);

/** 设置缩放密度 */
LCUI_API void ui_set_scaled_density(float density);

/** 设置密度等级 */
LCUI_API void ui_set_density_level(ui_density_Level_t level);

/** 设置缩放密度等级 */
LCUI_API void ui_set_scaled_density_level(ui_density_Level_t level);

/** 设置 DPI */
LCUI_API void ui_set_dpi(float dpi);

/** 设置全局缩放比例 */
LCUI_API void ui_set_scale(float scale);

// Base

LCUI_API ui_widget_t *ui_create_widget(const char *type);
LCUI_API ui_widget_t *ui_create_widget_with_prototype(
    const ui_widget_prototype_t *proto);
LCUI_API void ui_widget_destroy(ui_widget_t *w);
LCUI_API void ui_widget_set_title(ui_widget_t *w, const wchar_t *title);
LCUI_API void ui_widget_set_text(ui_widget_t *w, const char *text);
LCUI_API void ui_widget_get_offset(ui_widget_t *w, ui_widget_t *parent,
				   float *offset_x, float *offset_y);
LCUI_API LCUI_BOOL ui_widget_in_viewport(ui_widget_t *w);

// Root

LCUI_API ui_widget_t *ui_root(void);
LCUI_API int ui_root_append(ui_widget_t *w);

// Id

LCUI_API ui_widget_t *ui_get_widget(const char *id);
LCUI_API int ui_widget_set_id(ui_widget_t *w, const char *idstr);

// Prototype

LCUI_API ui_widget_prototype_t *ui_create_widget_prototype(
    const char *name, const char *parent_name);
LCUI_API ui_widget_prototype_t *ui_get_widget_prototype(const char *name);
LCUI_API LCUI_BOOL ui_check_widget_type(ui_widget_t *w, const char *type);
LCUI_API LCUI_BOOL
ui_check_widget_prototype(ui_widget_t *w, const ui_widget_prototype_t *proto);
LCUI_API void *ui_widget_get_data(ui_widget_t *widget,
				  ui_widget_prototype_t *proto);
LCUI_API void *ui_widget_add_data(ui_widget_t *widget,
				  ui_widget_prototype_t *proto,
				  size_t data_size);

// Extra Data

LCUI_API ui_widget_extra_data_t *ui_create_extra_data(ui_widget_t *widget);

INLINE ui_widget_extra_data_t *ui_widget_use_extra_data(ui_widget_t *widget)
{
	return widget->extra ? widget->extra : ui_create_extra_data(widget);
}

// Attributes

LCUI_API int ui_widget_set_attribute_ex(ui_widget_t *w, const char *name,
					void *value, int value_type,
					void (*value_destructor)(void *));
LCUI_API int ui_widget_set_attribute(ui_widget_t *w, const char *name,
				     const char *value);
LCUI_API const char *ui_widget_get_attribute_value(ui_widget_t *w,
						   const char *name);

// Classes

LCUI_API int ui_widget_add_class(ui_widget_t *w, const char *class_name);
LCUI_API LCUI_BOOL ui_widget_has_class(ui_widget_t *w, const char *class_name);
LCUI_API int ui_widget_remove_class(ui_widget_t *w, const char *class_name);

// Status

LCUI_API int ui_widget_add_status(ui_widget_t *w, const char *status_name);
LCUI_API LCUI_BOOL ui_widget_has_status(ui_widget_t *w,
					const char *status_name);
LCUI_API int ui_widget_remove_status(ui_widget_t *w, const char *status_name);
LCUI_API void ui_widget_update_status(ui_widget_t *widget);
LCUI_API void ui_widget_set_disabled(ui_widget_t *w, LCUI_BOOL disabled);

// Tree

LCUI_API void ui_widget_remove(ui_widget_t *w);
LCUI_API void ui_widget_empty(ui_widget_t *w);
LCUI_API int ui_widget_append(ui_widget_t *parent, ui_widget_t *widget);
LCUI_API int ui_widget_prepend(ui_widget_t *parent, ui_widget_t *widget);
LCUI_API int ui_widget_unwrap(ui_widget_t *widget);
LCUI_API int ui_widget_unlink(ui_widget_t *w);
LCUI_API ui_widget_t *ui_widget_prev(ui_widget_t *w);
LCUI_API ui_widget_t *ui_widget_next(ui_widget_t *w);
LCUI_API ui_widget_t *ui_widget_get_child(ui_widget_t *w, size_t index);
LCUI_API size_t ui_widget_each(ui_widget_t *w,
			       void (*callback)(ui_widget_t *, void *),
			       void *arg);
LCUI_API ui_widget_t *ui_widget_at(ui_widget_t *widget, int ix, int iy);
LCUI_API void ui_print_tree(ui_widget_t *w);

// Style
LCUI_API void ui_widget_get_style(ui_widget_t *w, int key,
				  css_style_value_t *value);
LCUI_API void ui_widget_set_style(ui_widget_t *w, int key,
				  const css_style_value_t *value);
LCUI_API int ui_widget_unset_style(ui_widget_t *w, int key);
LCUI_API css_selector_node_t *ui_widget_create_selector_node(ui_widget_t *w);
LCUI_API css_selector_t *ui_widget_create_selector(ui_widget_t *w);
LCUI_API size_t ui_widget_get_children_style_changes(ui_widget_t *w, int type,
						     const char *name);
LCUI_API void ui_widget_print_stylesheet(ui_widget_t *w);
LCUI_API size_t ui_widget_get_children_style_changes(ui_widget_t *w, int type,
						     const char *name);
LCUI_API void ui_widget_update_children_style(ui_widget_t *w);
LCUI_API void ui_widget_refresh_children_style(ui_widget_t *w);
LCUI_API int ui_widget_set_style_string(ui_widget_t *w, const char *property,
					const char *css_text);
LCUI_API void ui_widget_set_style_unit_value(ui_widget_t *w, int key,
					     css_numeric_value_t value,
					     css_unit_t unit);
LCUI_API void ui_widget_set_style_keyword_value(ui_widget_t *w, int key,
						css_keyword_value_t value);
LCUI_API void ui_widget_set_style_color_value(ui_widget_t *w, int key,
					      css_color_value_t value);
LCUI_API void ui_widget_set_style_numeric_value(ui_widget_t *w, int key,
						css_numeric_value_t value);
LCUI_API void ui_widget_compute_style(ui_widget_t *w);
LCUI_API void ui_widget_force_update_style(ui_widget_t *w);
LCUI_API void ui_widget_force_refresh_style(ui_widget_t *w);

// Updater

LCUI_API size_t ui_widget_update(ui_widget_t *w);
LCUI_API size_t ui_update(void);
LCUI_API void ui_refresh_style(void);

// Helper

LCUI_API LCUI_BOOL ui_widget_is_visible(ui_widget_t *w);
LCUI_API void ui_widget_move(ui_widget_t *w, float left, float top);
LCUI_API void ui_widget_resize(ui_widget_t *w, float width, float height);
LCUI_API void ui_widget_show(ui_widget_t *w);
LCUI_API void ui_widget_hide(ui_widget_t *w);
LCUI_API ui_widget_t *ui_widget_get_closest(ui_widget_t *w, const char *type);
LCUI_API dict_t *ui_widget_collect_references(ui_widget_t *w);

// Hash

LCUI_API void ui_widget_generate_self_hash(ui_widget_t *widget);
LCUI_API void ui_widget_generate_hash(ui_widget_t *w);
LCUI_API size_t ui_widget_export_hash(ui_widget_t *w, unsigned *hash_list,
				      size_t len);
LCUI_API size_t ui_widget_import_hash(ui_widget_t *w, unsigned *hash_list,
				      size_t maxlen);

// Layout

/**
 * 执行布局前的准备操作
 * 重置布局相关属性，以让它们在布局时被重新计算
 * @param rule 父级组件所使用的布局规则
 */
LCUI_API void ui_widget_prepare_reflow(ui_widget_t *w, ui_layout_rule_t rule);

LCUI_API void ui_widget_reset_size(ui_widget_t *w);

LCUI_API void ui_widget_reflow(ui_widget_t *w);

// Renderer

LCUI_API LCUI_BOOL ui_widget_mark_dirty_rect(ui_widget_t *w, ui_rect_t *in_rect,
					     int box_type);
LCUI_API size_t ui_widget_get_dirty_rects(ui_widget_t *w, list_t *rects);
LCUI_API size_t ui_widget_render(ui_widget_t *w, pd_context_t *paint);

// Updater

LCUI_API void ui_widget_set_rules(ui_widget_t *w,
				  const ui_widget_rules_t *rules);
LCUI_API void ui_widget_add_task_for_children(ui_widget_t *widget,
					      ui_task_type_t task);
LCUI_API void ui_widget_add_task(ui_widget_t *widget, int task);

INLINE void ui_widget_refresh_style(ui_widget_t *w)
{
	ui_widget_add_task(w, UI_TASK_REFRESH_STYLE);
}

INLINE void ui_widget_update_style(ui_widget_t *w)
{
	ui_widget_add_task(w, UI_TASK_UPDATE_STYLE);
}

// Observer
LCUI_API LCUI_BOOL ui_widget_has_observer(ui_widget_t *widget,
					  ui_mutation_record_type_t type);

LCUI_API int ui_widget_add_mutation_recrod(ui_widget_t *widget,
					   ui_mutation_record_t *record);

// Logger

LCUI_API int ui_logger_log(logger_level_e level, ui_widget_t *w,
			   const char *fmt, ...);

#define ui_debug(W, ...) ui_logger_log(LOGGER_LEVEL_DEBUG, W, ##__VA_ARGS__)

// Events

/** 设置阻止部件及其子级部件的事件 */
INLINE void ui_widget_block_event(ui_widget_t *w, LCUI_BOOL block)
{
	w->event_blocked = block;
}

/** 触发事件，让事件处理器在主循环中调用 */
LCUI_API int ui_widget_post_event(ui_widget_t *w, const ui_event_t *e,
				  void *arg,
				  ui_event_arg_destructor_t destroy_arg);

/** 触发事件，直接调用事件处理器 */
LCUI_API int ui_widget_emit_event(ui_widget_t *w, ui_event_t e, void *arg);

/** 自动分配一个可用的事件标识号 */
LCUI_API int ui_alloc_event_id(void);

/** 设置与事件标识号对应的名称 */
LCUI_API int ui_set_event_id(int event_id, const char *event_name);

/** 获取与事件标识号对应的名称 */
LCUI_API const char *ui_get_event_name(int event_id);

/** 获取与事件名称对应的标识号 */
LCUI_API int ui_get_event_id(const char *event_name);

LCUI_API void ui_event_init(ui_event_t *e, const char *name);

LCUI_API void ui_event_destroy(ui_event_t *e);

/**
 * 添加部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_id 事件标识号
 * @param[in] handler 事件处理函数
 * @param[in] data 事件处理函数的附加数据
 * @param[in] destroy_arg 数据的销毁函数
 * @return 成功则返回 0，失败返回负数
 */
LCUI_API ui_event_listener_t *ui_widget_add_event_listener(
    ui_widget_t *widget, int event_id, ui_event_handler_t handler, void *data,
    void (*destroy_arg)(void *));

/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 与事件绑定的函数
 * @return 成功则返回 0，失败返回负数
 */
LCUI_API int ui_widget_remove_event_listener(ui_widget_t *w, int event_id,
					     ui_event_handler_t handler,
					     void *data);

/**
 * 添加部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 事件处理函数
 * @param[in] data 事件处理函数的附加数据
 * @param[in] destroy_arg 数据的销毁函数
 * @return 返回已移除的事件监听器数量
 */
LCUI_API ui_event_listener_t *ui_widget_on(ui_widget_t *widget,
					   const char *event_name,
					   ui_event_handler_t handler,
					   void *data,
					   void (*destroy_arg)(void *));

/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 与事件绑定的函数
 * @return 成功则返回 0，失败返回负数
 */
LCUI_API int ui_widget_off(ui_widget_t *widget, const char *event_name,
			   ui_event_handler_t handler, void *data);

INLINE int ui_emit_event(ui_event_t e, void *arg)
{
	return ui_widget_emit_event(ui_root(), e, arg);
}

INLINE int ui_post_event(const ui_event_t *e, void *arg,
			 ui_event_arg_destructor_t destroy_arg)
{
	return ui_widget_post_event(ui_root(), e, arg, destroy_arg);
}

INLINE ui_event_listener_t *ui_add_event_listener(
    ui_widget_t *widget, int event_id, ui_event_handler_t handler, void *arg,
    ui_event_arg_destructor_t destroy_arg)
{
	return ui_widget_add_event_listener(ui_root(), event_id, handler, arg,
					    destroy_arg);
}

INLINE int ui_remove_event_listener(ui_widget_t *w, int event_id,
				    ui_event_handler_t handler, void *data)
{
	return ui_widget_remove_event_listener(ui_root(), event_id, handler,
					       data);
}

INLINE ui_event_listener_t *ui_on_event(const char *event_name,
					ui_event_handler_t handler, void *data,
					void (*destroy_arg)(void *))
{
	return ui_widget_on(ui_root(), event_name, handler, data, destroy_arg);
}

INLINE int ui_off_event(const char *event_name, ui_event_handler_t handler,
			void *data)
{
	return ui_widget_off(ui_root(), event_name, handler, data);
}

/** 清除事件对象，通常在部件销毁时调用该函数，以避免部件销毁后还有事件发送给它
 */
LCUI_API void ui_clear_event_target(ui_widget_t *widget);

/** get current focused widget */
LCUI_API ui_widget_t *ui_get_focus(void);

/** 将一个部件设置为焦点 */
LCUI_API int ui_set_focus(ui_widget_t *widget);

/** 停止部件的事件传播 */
LCUI_API int ui_widget_stop_event_propagation(ui_widget_t *widget);

/** 为部件设置鼠标捕获，设置后将捕获全局范围内的鼠标事件 */
LCUI_API void ui_widget_set_mouse_capture(ui_widget_t *w);

/** 为部件解除鼠标捕获 */
LCUI_API void ui_widget_release_mouse_capture(ui_widget_t *w);

/**
 * 为部件设置触点捕获，设置后将捕获全局范围内的触点事件
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则捕获全部触点
 * @returns 设置成功返回 0，如果其它部件已经捕获该触点则返回 -1
 */
LCUI_API int ui_widget_set_touch_capture(ui_widget_t *w, int point_id);

/**
 * 为部件解除触点捕获
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则解除全部触点的捕获
 */
LCUI_API int ui_widget_release_touch_capture(ui_widget_t *w, int point_id);

LCUI_API void ui_widget_destroy_listeners(ui_widget_t *w);

// CSS

LCUI_API int ui_load_css_file(const char *filepath);
LCUI_API size_t ui_load_css_string(const char *str, const char *space);

// Image Loader

#define ui_image_on_event ui_image_add_event_listener
#define ui_image_off_event ui_image_remove_event_listener

typedef struct ui_image_t ui_image_t;
typedef void (*ui_image_event_handler_t)(ui_image_t *, void *);

LCUI_API void ui_init_image_loader(void);
LCUI_API void ui_destroy_image_loader(void);
LCUI_API void ui_process_image_events(void);
LCUI_API ui_image_t *ui_load_image(const char *path);
LCUI_API pd_canvas_t *ui_image_get_data(ui_image_t *image);
LCUI_API const char *ui_image_get_path(ui_image_t *image);
LCUI_API void ui_image_add_ref(ui_image_t *image);
LCUI_API void ui_image_remove_ref(ui_image_t *image);
LCUI_API int ui_image_add_event_listener(ui_image_t *image,
					 ui_image_event_handler_t handler,
					 void *data);
LCUI_API int ui_image_remove_event_listener(ui_image_t *image,
					    ui_image_event_handler_t handler,
					    void *data);

// CSS Font Style

#define UI_DEFAULT_FONT_SIZE 14
#define UI_DEFAULT_FONT_COLOR 0xff333333
#define UI_MIN_FONT_SIZE 12
#define UI_LINE_HEIGHT_SCALE 1.42857143

typedef struct ui_text_style_t {
	int font_size;
	int line_height;
	int *font_ids;
	char *font_family;
	wchar_t *content;
	pd_color_t color;
	uint8_t font_style;
	uint8_t font_weight;
	uint8_t text_align;
	uint8_t white_space;
	uint8_t word_break;
} ui_text_style_t;

LCUI_API void ui_text_style_init(ui_text_style_t *fs);

LCUI_API void ui_text_style_destroy(ui_text_style_t *fs);

LCUI_API LCUI_BOOL ui_text_style_is_equal(const ui_text_style_t *a,
					  const ui_text_style_t *b);

LCUI_API void ui_compute_text_style(ui_text_style_t *fs,
				    const css_computed_style_t *ss);

// Rect

LCUI_API void ui_convert_rect(const pd_rect_t *rect, ui_rect_t *rectf,
			      float scale);

LCUI_API LCUI_BOOL ui_rect_correct(ui_rect_t *rect, float container_width,
				   float container_height);

INLINE LCUI_BOOL ui_rect_has_point(ui_rect_t *rect, float x, float y)
{
	return x >= rect->x && y >= rect->y && x < rect->x + rect->width &&
	       y < rect->y + rect->height;
}

INLINE LCUI_BOOL ui_rect_is_equal(const ui_rect_t *a, const ui_rect_t *b)
{
	return (int)(100 * (a->x - b->x)) == 0 &&
	       (int)(100 * (a->y - b->y)) == 0 &&
	       (int)(100 * (a->width - b->width)) == 0 &&
	       (int)(100 * (a->height - b->height)) == 0;
}

LCUI_API LCUI_BOOL ui_rect_is_cover(const ui_rect_t *a, const ui_rect_t *b);

INLINE LCUI_BOOL ui_rect_is_include(ui_rect_t *a, ui_rect_t *b)
{
	return (b->x >= a->x && b->x + b->width <= a->x + a->width &&
		b->y >= a->y && b->y + b->height <= a->y + a->height);
}

LCUI_API LCUI_BOOL ui_rect_overlap(const ui_rect_t *a, const ui_rect_t *b,
				   ui_rect_t *overlapping_rect);

LCUI_API void ui_rect_merge(ui_rect_t *merged_rect, const ui_rect_t *a,
			    const ui_rect_t *b);

LCUI_END_HEADER

#endif
