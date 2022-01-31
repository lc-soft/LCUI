#ifndef LIB_UI_INCLUDE_UI_H
#define LIB_UI_INCLUDE_UI_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include <LCUI/util.h>
#include <LCUI/css/def.h>
#include <LCUI/css/library.h>

LCUI_BEGIN_HEADER

// Types

typedef struct ui_metrics_t {
	float dpi;
	float density;
	float scaled_density;
	float scale;
} ui_metrics_t;

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
	UI_LAYOUT_RULE_FIXED_WIDTH,
	UI_LAYOUT_RULE_FIXED_HEIGHT,
	UI_LAYOUT_RULE_FIXED
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

typedef struct ui_boxshadow_style_t_ {
	float x, y;
	float blur;
	float spread;
	pd_color_t color;
} ui_boxshadow_style_t;

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

typedef struct ui_background_position_t {
	LCUI_BOOL using_value;
	union {
		struct {
			css_unit_value_t x, y;
		};
		int value;
	};
} ui_background_position_t;

typedef struct ui_background_size_t {
	LCUI_BOOL using_value;
	union {
		struct {
			css_unit_value_t width, height;
		};
		int value;
	};
} ui_background_size_t;

typedef struct ui_background_style_t {
	pd_canvas_t *image;
	pd_color_t color;
	struct {
		LCUI_BOOL x, y;
	} repeat;
	ui_background_position_t position;
	ui_background_size_t size;
} ui_background_style_t;

typedef struct ui_widget_attribute_t_ {
	char *name;
	struct {
		int type;
		void(*destructor)(void*);
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
	pd_border_t border;
	pd_boxshadow_t shadow;
	pd_background_t background;
} ui_widget_actual_style_t;

typedef enum ui_task_type_t {
	UI_TASK_REFRESH_STYLE,
	UI_TASK_UPDATE_STYLE,
	UI_TASK_TITLE,
	UI_TASK_PROPS,
	UI_TASK_BOX_SIZING,
	UI_TASK_PADDING,
	UI_TASK_MARGIN,
	UI_TASK_VISIBLE,
	UI_TASK_DISPLAY,
	UI_TASK_FLEX,
	UI_TASK_SHADOW,
	UI_TASK_BORDER,
	UI_TASK_BACKGROUND,
	UI_TASK_POSITION,
	UI_TASK_RESIZE,
	UI_TASK_ZINDEX,
	UI_TASK_OPACITY,
	UI_TASK_REFLOW,
	UI_TASK_USER,
	UI_TASK_TOTAL_NUM
} ui_task_type_t;

typedef struct ui_widget_t ui_widget_t;
typedef list_t ui_widget_listeners_t;

typedef void(*ui_widget_function_t)(ui_widget_t*);
typedef void(*ui_widget_task_handler_t)(ui_widget_t*, int);
typedef void(*ui_widget_size_getter_t)(ui_widget_t*, float*, float*, ui_layout_rule_t);
typedef void(*ui_widget_size_setter_t)(ui_widget_t*, float, float);
typedef void(*ui_widget_attr_setter_t)(ui_widget_t*, const char*, const char*);
typedef void(*ui_widget_text_setter_t)(ui_widget_t*, const char*);
typedef void(*ui_widget_prop_binder_t)(ui_widget_t*, const char*, LCUI_Object);
typedef void(*ui_widget_painter_t)(ui_widget_t*, pd_paint_context_t*,
				  ui_widget_actual_style_t*);
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
	void (*on_update_progress)(ui_widget_t*, size_t);
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

/** See more: https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Box_Model */
typedef struct ui_widget_box_model_t {
	pd_rectf_t content;
	pd_rectf_t padding;
	pd_rectf_t border;
	pd_rectf_t canvas;
	pd_rectf_t outer;
} ui_widget_box_model_t;

typedef struct ui_flexbox_layout_style_t {
	/**
	 * The flex shrink factor of a flex item
	 * See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/flex-shrink
	 */
	float shrink;

	/* the flex grow factor of a flex item main size
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex-grow
	 */
	float grow;

	/**
	 * The initial main size of a flex item
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/flex-basis
	 */
	float basis;

	css_keyword_value_t wrap : 8;
	css_keyword_value_t direction : 8;

	/**
	 * Sets the align-self value on all direct children as a group
	 * See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/align-items
	 */
	css_keyword_value_t align_items : 8;

	/**
	 * Sets the distribution of space between and around content items along
	 * a flexbox's cross-axis
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/align-content
	 */
	css_keyword_value_t align_content : 8;

	/**
	 * Defines how the browser distributes space between and around content
	 * items along the main-axis of a flex container See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/justify-content
	 */
	css_keyword_value_t justify_content : 8;
} ui_flexbox_layout_style_t;

typedef struct ui_profile_t {
	clock_t time;
	size_t update_count;
	size_t refresh_count;
	size_t layout_count;
	size_t user_task_count;
	size_t destroy_count;
	size_t destroy_time;
} ui_profile_t;

typedef struct ui_widget_style_t {
	LCUI_BOOL visible;
	LCUI_BOOL focusable;
	ui_sizing_rule_t width_sizing;
	ui_sizing_rule_t height_sizing;
	float min_width;
	float min_height;
	float max_width;
	float max_height;
	float left;
	float top;
	float right;
	float bottom;
	int z_index;
	float opacity;
	css_keyword_value_t position;
	css_keyword_value_t display;
	css_keyword_value_t box_sizing;
	css_keyword_value_t vertical_align;
	ui_border_style_t border;
	ui_boxshadow_style_t shadow;
	ui_background_style_t background;
	ui_flexbox_layout_style_t flex;
	int pointer_events;
} ui_widget_style_t;

struct ui_widget_prototype_t {
	char *name;
	ui_widget_function_t init;
	ui_widget_function_t refresh;
	ui_widget_function_t destroy;
	ui_widget_function_t update;
	ui_widget_task_handler_t runtask;
	ui_widget_attr_setter_t setattr;
	ui_widget_text_setter_t settext;
	ui_widget_prop_binder_t bindprop;
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
	UI_EVENT_MOVE,
	UI_EVENT_RESIZE,
	UI_EVENT_SHOW,
	UI_EVENT_HIDE,
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
typedef void(*ui_event_handler_t)(ui_widget_t*, ui_event_t*, void*);

struct ui_event_t {
	uint32_t type;			/**< 事件类型标识号 */
	void *data;			/**< 附加数据 */
	ui_widget_t* target;		/**< 触发事件的部件 */
	LCUI_BOOL cancel_bubble;	/**< 是否取消事件冒泡 */
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
					   ui_mutation_observer_t *, void *);

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MutationObserverInit
 */
typedef struct ui_mutation_observer_init_t {
	LCUI_BOOL child_list;
	LCUI_BOOL subtree;
	LCUI_BOOL properties;
	LCUI_BOOL attributes;
} ui_mutation_observer_init_t;

LCUI_API ui_mutation_record_t *ui_mutation_record_create(ui_widget_t *widget,
						ui_mutation_record_type_t type);

LCUI_API ui_mutation_record_t *ui_mutation_record_duplicate(ui_mutation_record_t *source);

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
LCUI_API void ui_mutation_observer_add_record(ui_mutation_observer_t *observer, ui_mutation_record_t *record);

// MutationObserver end


typedef struct ui_widget_extra_data_t {
	ui_widget_listeners_t listeners;
	ui_mutation_observer_t *observer;
	ui_mutation_observer_init_t observer_options;
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
	 * Geometric parameters (readonly)
	 * Their values come from the box.border
	 */
	float x, y, width, height;

	/**
	 * A box’s “ideal” size in a given axis when given infinite available space.
	 * See more: https://drafts.csswg.org/css-sizing-3/#max-content
	 */
	float max_content_width, max_content_height;

	pd_rect_t2F padding;
	pd_rect_t2F margin;
	ui_widget_box_model_t box;

	css_style_decl_t *style;
	css_style_props_t *custom_style;
	const css_style_decl_t *matched_style;
	ui_widget_style_t computed_style;

	/** Some data bound to the prototype */
	ui_widget_data_t data;
	ui_widget_extra_data_t *extra;

	/**
	 * Prototype chain
	 * It is used to implement the inheritance of widgets,
	 * Just like prototype chain in JavaScript
	 */
	const ui_widget_prototype_t* proto;

	ui_widget_update_t update;

	pd_rectf_t dirty_rect;
	ui_dirty_rect_type_t dirty_rect_type;
	LCUI_BOOL has_child_dirty_rect;

	/** Parent widget */
	ui_widget_t* parent;

	/** List of child widgets */
	list_t children;

	/**
	 * List of child widgets in descending order by z-index
	 * @see https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Positioning/Understanding_z_index/The_stacking_context
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
LCUI_API int ui_dispatch_event(ui_event_t* e);
LCUI_API void ui_process_events(void);

// Trash

LCUI_API size_t ui_trash_clear(void);
LCUI_API void ui_trash_add(ui_widget_t* w);

// Metrics

/** 转换成单位为 px 的度量值 */
LCUI_API float ui_compute(float value, css_unit_t type);

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

LCUI_API const ui_metrics_t* ui_get_metrics(void);

/** 获取当前的全局缩放比例 */
INLINE float ui_get_scale(void)
{
	return ui_get_metrics()->scale;
}

INLINE int ui_compute_actual(float value, css_unit_t type)
{
	return y_iround(ui_compute(value, type) * ui_get_scale());
}

INLINE void ui_compute_rect_actual(pd_rect_t* dst, const pd_rectf_t* src)
{
	dst->x = y_iround(src->x * ui_get_scale());
	dst->y = y_iround(src->y * ui_get_scale());
	dst->width = y_iround(src->width * ui_get_scale());
	dst->height = y_iround(src->height * ui_get_scale());
}

// Base

LCUI_API ui_widget_t* ui_create_widget(const char* type);
LCUI_API ui_widget_t* ui_create_widget_with_prototype(
    const ui_widget_prototype_t* proto);
LCUI_API void ui_widget_destroy(ui_widget_t* w);
LCUI_API void ui_widget_set_title(ui_widget_t* w, const wchar_t* title);
LCUI_API void ui_widget_set_text(ui_widget_t* w, const char* text);
LCUI_API void ui_widget_bind_property(ui_widget_t* w, const char* name,
				      LCUI_Object value);
LCUI_API void ui_widget_get_offset(ui_widget_t* w, ui_widget_t* parent,
				   float* offset_x, float* offset_y);
LCUI_API LCUI_BOOL ui_widget_in_viewport(ui_widget_t* w);

// Root

LCUI_API ui_widget_t* ui_root(void);
LCUI_API int ui_root_append(ui_widget_t* w);

// Id

LCUI_API ui_widget_t* ui_get_widget(const char* id);
LCUI_API int ui_widget_set_id(ui_widget_t* w, const char* idstr);

// Prototype

LCUI_API ui_widget_prototype_t* ui_create_widget_prototype(
    const char* name, const char* parent_name);
LCUI_API ui_widget_prototype_t* ui_get_widget_prototype(const char* name);
LCUI_API LCUI_BOOL ui_check_widget_type(ui_widget_t* w, const char* type);
LCUI_API LCUI_BOOL
ui_check_widget_prototype(ui_widget_t* w, const ui_widget_prototype_t* proto);
LCUI_API void* ui_widget_get_data(ui_widget_t* widget,
				  ui_widget_prototype_t* proto);
LCUI_API void* ui_widget_add_data(ui_widget_t* widget,
				  ui_widget_prototype_t* proto,
				  size_t data_size);

// Extra Data

LCUI_API ui_widget_extra_data_t* ui_create_extra_data(ui_widget_t* widget);

INLINE ui_widget_extra_data_t* ui_widget_use_extra_data(ui_widget_t* widget)
{
	return widget->extra ? widget->extra : ui_create_extra_data(widget);
}

// Attributes

LCUI_API int ui_widget_set_attribute_ex(ui_widget_t* w, const char* name,
					void* value, int value_type,
					void (*value_destructor)(void*));
LCUI_API int ui_widget_set_attribute(ui_widget_t* w, const char* name,
				     const char* value);
LCUI_API const char* ui_widget_get_attribute_value(ui_widget_t* w,
						   const char* name);

// Classes

LCUI_API int ui_widget_add_class(ui_widget_t* w, const char* class_name);
LCUI_API LCUI_BOOL ui_widget_has_class(ui_widget_t* w, const char* class_name);
LCUI_API int ui_widget_remove_class(ui_widget_t* w, const char* class_name);

// Status

LCUI_API int ui_widget_add_status(ui_widget_t* w, const char* status_name);
LCUI_API LCUI_BOOL ui_widget_has_status(ui_widget_t* w,
					const char* status_name);
LCUI_API int ui_widget_remove_status(ui_widget_t* w, const char* status_name);
LCUI_API void ui_widget_update_status(ui_widget_t* widget);
LCUI_API void ui_widget_set_disabled(ui_widget_t* w, LCUI_BOOL disabled);

// Tree

LCUI_API void ui_widget_remove(ui_widget_t* w);
LCUI_API void ui_widget_empty(ui_widget_t* w);
LCUI_API int ui_widget_append(ui_widget_t* parent, ui_widget_t* widget);
LCUI_API int ui_widget_prepend(ui_widget_t* parent, ui_widget_t* widget);
LCUI_API int ui_widget_unwrap(ui_widget_t* widget);
LCUI_API int ui_widget_unlink(ui_widget_t* w);
LCUI_API ui_widget_t* ui_widget_prev(ui_widget_t* w);
LCUI_API ui_widget_t* ui_widget_next(ui_widget_t* w);
LCUI_API ui_widget_t* ui_widget_get_child(ui_widget_t* w, size_t index);
LCUI_API size_t ui_widget_each(ui_widget_t* w,
			       void (*callback)(ui_widget_t*, void*),
			       void* arg);
LCUI_API ui_widget_t* ui_widget_at(ui_widget_t* widget, int ix, int iy);
LCUI_API void ui_print_tree(ui_widget_t* w);

// Style

#define ui_widget_check_style_type(W, K, T) css_check_style_prop((W)->style, K, T)

#define ui_widget_set_style(W, K, VAL, TYPE)       \
	do {                                       \
		css_unit_value_t *_s;                     \
		_s = ui_widget_get_style(W, K);    \
		_s->is_valid = TRUE;               \
		_s->unit = CSS_UNIT_##TYPE;      \
		_s->val_##TYPE = VAL;              \
		ui_widget_add_task_by_style(W, K); \
	} while (0)

INLINE LCUI_BOOL ui_widget_is_visible(ui_widget_t* w)
{
	return w->computed_style.visible;
}

INLINE LCUI_BOOL ui_widget_check_style_valid(ui_widget_t* w, int key)
{
	return w->style && w->style->sheet[key].is_valid;
}

INLINE LCUI_BOOL ui_widget_has_auto_style(ui_widget_t* w, int key)
{
	return !ui_widget_check_style_valid(w, key) ||
	       ui_widget_check_style_type(w, key, AUTO);
}

LCUI_API css_selector_node_t *ui_widget_create_selector_node(ui_widget_t* w);
LCUI_API css_selector_t *ui_widget_create_selector(ui_widget_t* w);
LCUI_API size_t ui_widget_get_children_style_changes(ui_widget_t* w, int type,
						     const char* name);
LCUI_API void ui_widget_print_stylesheet(ui_widget_t* w);
LCUI_API size_t ui_widget_get_children_style_changes(ui_widget_t* w, int type,
						     const char* name);
LCUI_API void ui_widget_update_children_style(ui_widget_t* w);
LCUI_API void ui_widget_refresh_children_style(ui_widget_t* w);
LCUI_API void ui_widget_set_style_string(ui_widget_t* w, const char* name,
					 const char* value);
LCUI_API void ui_widget_add_task_by_style(ui_widget_t* w, int key);
LCUI_API void ui_widget_force_update_style(ui_widget_t* w);
LCUI_API void ui_widget_force_refresh_style(ui_widget_t* w);

// Updater

LCUI_API size_t ui_widget_update(ui_widget_t* w);
LCUI_API size_t ui_update(void);
LCUI_API void ui_refresh_style(void);

// Helper

INLINE float padding_x(ui_widget_t* w)
{
	return w->padding.left + w->padding.right;
}

INLINE float padding_y(ui_widget_t* w)
{
	return w->padding.top + w->padding.bottom;
}

INLINE float border_x(ui_widget_t* w)
{
	return w->computed_style.border.left.width +
	       w->computed_style.border.right.width;
}

INLINE float border_y(ui_widget_t* w)
{
	return w->computed_style.border.top.width +
	       w->computed_style.border.bottom.width;
}

INLINE float margin_x(ui_widget_t* w)
{
	return w->margin.left + w->margin.right;
}

INLINE float margin_y(ui_widget_t* w)
{
	return w->margin.top + w->margin.bottom;
}

INLINE float to_border_box_width(ui_widget_t* w, float content_width)
{
	return content_width + padding_x(w) + border_x(w);
}

INLINE float to_border_box_height(ui_widget_t* w, float content_height)
{
	return content_height + padding_y(w) + border_y(w);
}

INLINE float ui_widget_get_limited_width(ui_widget_t* w, float width)
{
	if (w->computed_style.max_width > -1 &&
	    width > w->computed_style.max_width) {
		width = w->computed_style.max_width;
	}
	if (w->computed_style.min_width > -1 &&
	    width < w->computed_style.min_width) {
		width = w->computed_style.min_width;
	}
	return width;
}

INLINE float ui_widget_get_limited_height(ui_widget_t* w, float height)
{
	if (w->computed_style.max_height > -1 &&
	    height > w->computed_style.max_height) {
		height = w->computed_style.max_height;
	}
	if (w->computed_style.min_height > -1 &&
	    height < w->computed_style.min_height) {
		height = w->computed_style.min_height;
	}
	return height;
}

LCUI_API void ui_widget_set_padding(ui_widget_t* w, float top, float right,
				    float bottom, float left);
LCUI_API void ui_widget_set_margin(ui_widget_t* w, float top, float right,
				   float bottom, float left);
LCUI_API void ui_widget_set_border_color(ui_widget_t* w, pd_color_t color);
LCUI_API void ui_widget_set_border_width(ui_widget_t* w, float width);
LCUI_API void ui_widget_set_border_style(ui_widget_t* w, int style);
LCUI_API void ui_widget_set_border(ui_widget_t* w, float width, int style,
				   pd_color_t color);
LCUI_API void ui_widget_set_box_shadow(ui_widget_t* w, float x, float y,
				       float blur, pd_color_t color);
LCUI_API void ui_widget_move(ui_widget_t* w, float left, float top);
LCUI_API void ui_widget_resize(ui_widget_t* w, float width, float height);
LCUI_API css_unit_value_t *ui_widget_get_style(ui_widget_t* w, int key);
LCUI_API int ui_widget_unset_style(ui_widget_t* w, int key);
LCUI_API css_unit_value_t *ui_widget_get_matched_style(ui_widget_t* w, int key);
LCUI_API void ui_widget_set_visibility(ui_widget_t* w, const char* value);

INLINE void ui_widget_set_visible(ui_widget_t* w)
{
	ui_widget_set_visibility(w, "visible");
}

INLINE void ui_widget_set_hidden(ui_widget_t* w)
{
	ui_widget_set_visibility(w, "hidden");
}

LCUI_API void ui_widget_show(ui_widget_t* w);
LCUI_API void ui_widget_hide(ui_widget_t* w);
LCUI_API void ui_widget_set_position(ui_widget_t* w, css_keyword_value_t position);
LCUI_API void ui_widget_set_opacity(ui_widget_t* w, float opacity);
LCUI_API void ui_widget_set_box_sizing(ui_widget_t* w, css_keyword_value_t sizing);
LCUI_API ui_widget_t* ui_widget_get_closest(ui_widget_t* w, const char* type);
LCUI_API dict_t* ui_widget_collect_references(ui_widget_t* w);

// Hash

LCUI_API void ui_widget_generate_self_hash(ui_widget_t* widget);
LCUI_API void ui_widget_generate_hash(ui_widget_t* w);
LCUI_API size_t ui_widget_export_hash(ui_widget_t* w, unsigned* hash_list,
				      size_t len);
LCUI_API size_t ui_widget_import_hash(ui_widget_t* w, unsigned* hash_list,
				      size_t maxlen);

// Layout

LCUI_API void ui_widget_reflow(ui_widget_t* w, ui_layout_rule_t rule);
LCUI_API LCUI_BOOL ui_widget_auto_reflow(ui_widget_t* w, ui_layout_rule_t rule);

// Renderer

LCUI_API LCUI_BOOL ui_widget_mark_dirty_rect(ui_widget_t* w,
					     pd_rectf_t* in_rect, int box_type);
LCUI_API size_t ui_widget_get_dirty_rects(ui_widget_t* w, list_t* rects);
LCUI_API size_t ui_widget_render(ui_widget_t* w, pd_paint_context_t* paint);

// Updater

LCUI_API void ui_widget_set_rules(ui_widget_t* w,
				  const ui_widget_rules_t* rules);
LCUI_API void ui_widget_add_task_for_children(ui_widget_t* widget,
					      ui_task_type_t task);
LCUI_API void ui_widget_add_task(ui_widget_t* widget, int task);

INLINE void ui_widget_refresh_style(ui_widget_t* w)
{
	ui_widget_add_task(w, UI_TASK_REFRESH_STYLE);
}

INLINE void ui_widget_update_style(ui_widget_t* w)
{
	ui_widget_add_task(w, UI_TASK_UPDATE_STYLE);
}

// Observer
LCUI_API LCUI_BOOL ui_widget_has_observer(ui_widget_t* widget,
					  ui_mutation_record_type_t type);

LCUI_API int ui_widget_add_mutation_recrod(ui_widget_t* widget,
					   ui_mutation_record_t* record);

// Logger

LCUI_API int ui_logger_log(logger_level_e level, ui_widget_t *w, const char *fmt, ...);

#define ui_debug(W, ...) ui_logger_log(LOGGER_LEVEL_DEBUG, W, ##__VA_ARGS__)

// Events

/** 设置阻止部件及其子级部件的事件 */
INLINE void ui_widget_block_event(ui_widget_t* w, LCUI_BOOL block)
{
	w->event_blocked = block;
}

/** 触发事件，让事件处理器在主循环中调用 */
LCUI_API int ui_widget_post_event(ui_widget_t* w, const ui_event_t* e,
				  void* arg, void (*destroy_data)(void*));

/** 触发事件，直接调用事件处理器 */
LCUI_API int ui_widget_emit_event(ui_widget_t* w, ui_event_t e, void* arg);

/** 自动分配一个可用的事件标识号 */
LCUI_API int ui_alloc_event_id(void);

/** 设置与事件标识号对应的名称 */
LCUI_API int ui_set_event_id(int event_id, const char* event_name);

/** 获取与事件标识号对应的名称 */
LCUI_API const char* ui_get_event_name(int event_id);

/** 获取与事件名称对应的标识号 */
LCUI_API int ui_get_event_id(const char* event_name);

LCUI_API void ui_event_init(ui_event_t* e, const char* name);

LCUI_API void ui_event_destroy(ui_event_t* e);

/**
 * 添加部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_id 事件标识号
 * @param[in] handler 事件处理函数
 * @param[in] data 事件处理函数的附加数据
 * @param[in] destroy_data 数据的销毁函数
 * @return 成功则返回 0，失败返回负数
 */
LCUI_API int ui_widget_add_event_listener(ui_widget_t* widget, int event_id,
					  ui_event_handler_t handler,
					  void* data,
					  void (*destroy_data)(void*));

LCUI_API int ui_widget_remove_event_listener(ui_widget_t* w, int event_id,
					     ui_event_handler_t handler);
/**
 * 添加部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 事件处理函数
 * @param[in] data 事件处理函数的附加数据
 * @param[in] destroy_data 数据的销毁函数
 * @return 返回已移除的事件监听器数量
 */
LCUI_API int ui_widget_on(ui_widget_t* widget, const char* event_name,
			  ui_event_handler_t handler, void* data,
			  void (*destroy_data)(void*));

/**
 * 解除部件事件绑定
 * @param[in] widget 目标部件
 * @param[in] event_name 事件名称
 * @param[in] handler 与事件绑定的函数
 * @return 成功则返回 0，失败返回负数
 */
LCUI_API int ui_widget_off(ui_widget_t* widget, const char* event_name,
			   ui_event_handler_t handler);

INLINE int ui_emit_event(ui_event_t e, void* arg)
{
	return ui_widget_emit_event(ui_root(), e, arg);
}

INLINE int ui_post_event(const ui_event_t* e, void* data,
			 void (*destroy_data)(void*))
{
	return ui_widget_post_event(ui_root(), e, data, destroy_data);
}

INLINE int ui_add_event_listener(ui_widget_t* widget, int event_id,
				 ui_event_handler_t handler, void* data,
				 void (*destroy_data)(void*))
{
	return ui_widget_add_event_listener(ui_root(), event_id, handler, data,
					    destroy_data);
}

INLINE int ui_remove_event_listener(ui_widget_t* w, int event_id,
				    ui_event_handler_t handler)
{
	return ui_widget_remove_event_listener(ui_root(), event_id, handler);
}

INLINE int ui_on_event(const char* event_name, ui_event_handler_t handler,
		       void* data, void (*destroy_data)(void*))
{
	return ui_widget_on(ui_root(), event_name, handler, data, destroy_data);
}

INLINE int ui_off_event(const char* event_name, ui_event_handler_t handler)
{
	return ui_widget_off(ui_root(), event_name, handler);
}

/** 清除事件对象，通常在部件销毁时调用该函数，以避免部件销毁后还有事件发送给它
 */
LCUI_API void ui_clear_event_target(ui_widget_t* widget);

/** get current focused widget */
LCUI_API ui_widget_t* ui_get_focus(void);

/** 将一个部件设置为焦点 */
LCUI_API int ui_set_focus(ui_widget_t* widget);

/** 停止部件的事件传播 */
LCUI_API int ui_widget_stop_event_propagation(ui_widget_t* widget);

/** 为部件设置鼠标捕获，设置后将捕获全局范围内的鼠标事件 */
LCUI_API void ui_widget_set_mouse_capture(ui_widget_t* w);

/** 为部件解除鼠标捕获 */
LCUI_API void ui_widget_release_mouse_capture(ui_widget_t* w);

/**
 * 为部件设置触点捕获，设置后将捕获全局范围内的触点事件
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则捕获全部触点
 * @returns 设置成功返回 0，如果其它部件已经捕获该触点则返回 -1
 */
LCUI_API int ui_widget_set_touch_capture(ui_widget_t* w, int point_id);

/**
 * 为部件解除触点捕获
 * @param[in] w 部件
 * @param[in] point_id 触点ID，当值为 -1 时则解除全部触点的捕获
 */
LCUI_API int ui_widget_release_touch_capture(ui_widget_t* w, int point_id);

LCUI_API void ui_widget_destroy_listeners(ui_widget_t* w);

// CSS

LCUI_API int ui_load_css_file(const char* filepath);
LCUI_API size_t ui_load_css_string(const char* str, const char* space);

// Image Loader

#define ui_image_on_event ui_image_add_event_listener
#define ui_image_off_event ui_image_remove_event_listener

typedef struct ui_image_t ui_image_t;
typedef void (*ui_image_event_handler_t)(ui_image_t*, void*);

LCUI_API void ui_init_image_loader(void);
LCUI_API void ui_destroy_image_loader(void);
LCUI_API void ui_process_image_events(void);
LCUI_API ui_image_t* ui_load_image(const char* path);
LCUI_API pd_canvas_t* ui_image_get_data(ui_image_t* image);
LCUI_API const char* ui_image_get_path(ui_image_t* image);
LCUI_API void ui_image_add_ref(ui_image_t* image);
LCUI_API void ui_image_remove_ref(ui_image_t* image);
LCUI_API int ui_image_add_event_listener(ui_image_t* image,
					 ui_image_event_handler_t handler,
					 void* data);
LCUI_API int ui_image_remove_event_listener(ui_image_t* image,
					    ui_image_event_handler_t handler,
					    void* data);

LCUI_END_HEADER

#endif
