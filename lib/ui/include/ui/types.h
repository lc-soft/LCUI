#ifndef LIB_UI_INCLUDE_UI_TPYES_H
#define LIB_UI_INCLUDE_UI_TPYES_H

#include <stdbool.h>
#include <yutil.h>
#include <css/types.h>
#include <pandagl/types.h>

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

typedef enum ui_box_type_t {
	UI_BOX_TYPE_CONTENT_BOX,
	UI_BOX_TYPE_PADDING_BOX,
	UI_BOX_TYPE_BORDER_BOX,
	UI_BOX_TYPE_GRAPH_BOX
} ui_box_type_t;

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
	bool only_on_visible;

	/**
	 * First update the children in the visible area
	 * If your widget has a lot of children and you want to see the
	 * children who are currently seeing the priority update, we recommend
	 * enabling this rule.
	 */
	bool first_update_visible_children;

	/**
	 * Cache the stylesheets of children to improve the query speed of
	 * the stylesheet.
	 * If this rule is enabled, we recommend that you manually call
	 * ui_widget_generate_hash() to generate a hash value for the children
	 * of the widget.
	 */
	bool cache_children_style;

	/** Refresh the style of all child widgets if the status has changed */
	bool ignore_status_change;

	/** Refresh the style of all child widgets if the classes has changed */
	bool ignore_classes_change;

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
	bool for_self;

	/** Should update for children? */
	bool for_children;

	/** Should skip the property sync of bound surface? */
	bool skip_surface_props_sync;

	/** States of tasks */
	bool states[UI_TASK_TOTAL_NUM];
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

typedef enum ui_mouse_button_code_t {
	UI_MOUSE_BUTTON_MAIN,
	UI_MOUSE_BUTTON_AUXILIARY,
	UI_MOUSE_BUTTON_SECNODARY,
	UI_MOUSE_BUTTON_FORTH_BUTTON,
	UI_MOUSE_BUTTON_FIFTH_BUTTON
} ui_mouse_button_code_t;

#define UI_MOUSE_BUTTON_LEFT UI_MOUSE_BUTTON_MAIN
#define UI_MOUSE_BUTTON_MIDDLE UI_MOUSE_BUTTON_AUXILIARY
#define UI_MOUSE_BUTTON_WHEEL UI_MOUSE_BUTTON_AUXILIARY
#define UI_MOUSE_BUTTON_RIGHT UI_MOUSE_BUTTON_SECNODARY

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent
 */
typedef struct ui_keyboard_event_t {
	int code;
	bool alt_key;
	bool ctrl_key;
	bool shift_key;
	bool meta_key;
	bool is_composing;
} ui_keyboard_event_t;

typedef struct ui_touch_point_t {
	float x;
	float y;
	int id;
	int state;
	bool is_primary;
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
	uint32_t type;       /**< 事件类型标识号 */
	void *data;          /**< 附加数据 */
	ui_widget_t *target; /**< 触发事件的部件 */
	bool cancel_bubble;  /**< 是否取消事件冒泡 */
	union {
		ui_mouse_event_t mouse;
		ui_wheel_event_t wheel;
		ui_keyboard_event_t key;
		ui_touch_event_t touch;
		ui_textinput_event_t text;
	};
};

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
	bool disabled;
	bool event_blocked;

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
	bool has_child_dirty_rect;

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

#endif
