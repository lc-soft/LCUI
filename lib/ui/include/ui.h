#ifndef LIB_UI_H
#define LIB_UI_H

#include <LCUI/util/strlist.h>
#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>

// Types

/* clang-format off */

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
	LCUI_WSTATE_CREATED = 0,
	LCUI_WSTATE_UPDATED,
	LCUI_WSTATE_LAYOUTED,
	LCUI_WSTATE_READY,
	LCUI_WSTATE_NORMAL,
	LCUI_WSTATE_DELETED,
} ui_widget_state_t;

typedef enum ui_dirty_rect_type_t {
	UI_DIRTY_RECT_TYPE_NONE,
	UI_DIRTY_RECT_TYPE_CUSTOM,
	UI_DIRTY_RECT_TYPE_PADDING_BOX,
	UI_DIRTY_RECT_TYPE_BORDER_BOX,
	UI_DIRTY_RECT_TYPE_CANVAS_BOX
} ui_dirty_rect_type_t;

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
	LCUI_Rect canvas_box;
	LCUI_Rect border_box;
	LCUI_Rect padding_box;
	LCUI_Rect content_box;
	LCUI_Border border;
	LCUI_BoxShadow shadow;
	LCUI_Background background;
} ui_widget_actual_style_t;

typedef enum ui_widget_task_type_t {
	UI_WIDGET_TASK_REFRESH_STYLE,
	UI_WIDGET_TASK_UPDATE_STYLE,
	UI_WIDGET_TASK_TITLE,
	UI_WIDGET_TASK_PROPS,
	UI_WIDGET_TASK_BOX_SIZING,
	UI_WIDGET_TASK_PADDING,
	UI_WIDGET_TASK_MARGIN,
	UI_WIDGET_TASK_VISIBLE,
	UI_WIDGET_TASK_DISPLAY,
	UI_WIDGET_TASK_FLEX,
	UI_WIDGET_TASK_SHADOW,
	UI_WIDGET_TASK_BORDER,
	UI_WIDGET_TASK_BACKGROUND,
	UI_WIDGET_TASK_POSITION,
	UI_WIDGET_TASK_RESIZE,
	UI_WIDGET_TASK_ZINDEX,
	UI_WIDGET_TASK_OPACITY,
	UI_WIDGET_TASK_REFLOW,
	UI_WIDGET_TASK_USER,
	UI_WIDGET_TASK_TOTAL_NUM
} ui_widget_task_type_t;

typedef struct ui_widget_task_t {
	/** Should update for self? */
	LCUI_BOOL for_self;

	/** Should update for children? */
	LCUI_BOOL for_children;

	/** Should skip the property sync of bound surface? */
	LCUI_BOOL skip_surface_props_sync;

	/** States of tasks */
	LCUI_BOOL states[UI_WIDGET_TASK_TOTAL_NUM];
} ui_widget_task_t;

/** See more: https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Box_Model */
typedef struct ui_widget_box_model_t {
	LCUI_RectF content;
	LCUI_RectF padding;
	LCUI_RectF border;
	LCUI_RectF canvas;
	LCUI_RectF outer;
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

	LCUI_StyleValue wrap : 8;
	LCUI_StyleValue direction : 8;

	/**
	 * Sets the align-self value on all direct children as a group
	 * See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/align-items
	 */
	LCUI_StyleValue align_items : 8;

	/**
	 * Sets the distribution of space between and around content items along
	 * a flexbox's cross-axis
	 * See more: https://developer.mozilla.org/en-US/docs/Web/CSS/align-content
	 */
	LCUI_StyleValue align_content : 8;

	/**
	 * Defines how the browser distributes space between and around content
	 * items along the main-axis of a flex container See more:
	 * https://developer.mozilla.org/en-US/docs/Web/CSS/justify-content
	 */
	LCUI_StyleValue justify_content : 8;
} ui_flexbox_layout_style_t;

typedef struct ui_widget_task_profile_t {
	clock_t time;
	size_t update_count;
	size_t refresh_count;
	size_t layout_count;
	size_t user_task_count;
	size_t destroy_count;
	size_t destroy_time;
} ui_widget_task_profile_t;

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
	LCUI_StyleValue position;
	LCUI_StyleValue display;
	LCUI_StyleValue box_sizing;
	LCUI_StyleValue vertical_align;
	LCUI_BorderStyle border;
	LCUI_BoxShadowStyle shadow;
	LCUI_BackgroundStyle background;
	ui_flexbox_layout_style_t flex;
	int pointer_events;
} ui_widget_style_t;

typedef struct ui_widget_t ui_widget_t;
typedef LinkedList ui_widget_listeners_t;

typedef void(*ui_widget_function_t)(ui_widget_t*);
typedef void(*ui_widget_task_handler_t)(ui_widget_t*, int);
typedef void(*ui_widget_size_getter_t)(ui_widget_t*, float*, float*, ui_layout_rule_t);
typedef void(*ui_widget_size_setter_t)(ui_widget_t*, float, float);
typedef void(*ui_widget_attr_setter_t)(ui_widget_t*, const char*, const char*);
typedef void(*ui_widget_text_setter_t)(ui_widget_t*, const char*);
typedef void(*ui_widget_prop_binder_t)(ui_widget_t*, const char*, LCUI_Object);
typedef void(*ui_widget_painter_t)(ui_widget_t*, LCUI_PaintContext,
				  ui_widget_actual_style_t*);

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
	 * -1 - Update all children at once
	 * 0  - Automatically calculates the appropriate maximum number
	 * N  - Custom maximum number
	 */
	int max_update_children_count;

	/** Limit the number of children rendered  */
	unsigned max_render_children_count;

	/** A callback function on update progress */
	void (*on_update_progress)(ui_widget_t*, size_t);
} ui_widget_rules_t;

typedef struct ui_widget_prototype_t {
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
} ui_widget_prototype_t;

typedef struct ui_widget_data_entry_t {
	void *data;
	ui_widget_prototype_t *proto;
} ui_widget_data_entry_t;

typedef struct ui_widget_data_t {
	unsigned length;
	ui_widget_data_entry_t *list;
} ui_widget_data_t;

/** 部件事件类型枚举 */
typedef enum ui_event_type_t {
	UI_EVENT_NONE,
	UI_EVENT_LINK,		/**< link widget node to the parent widget children list */
	UI_EVENT_UNLINK,		/**< unlink widget node from the parent widget children list */
	UI_EVENT_READY,		/**< after widget initial layout was completed */
	UI_EVENT_DESTROY,		/**< before destroy */
	UI_EVENT_MOVE,		/**< 在移动位置时 */
	UI_EVENT_RESIZE,		/**< 改变尺寸 */
	UI_EVENT_SHOW,		/**< 显示 */
	UI_EVENT_HIDE,		/**< 隐藏 */
	UI_EVENT_FOCUS,		/**< 获得焦点 */
	UI_EVENT_BLUR,		/**< 失去焦点 */
	UI_EVENT_AFTERLAYOUT,	/**< 在子部件布局完成后 */
	UI_EVENT_KEYDOWN,		/**< 按键按下 */
	UI_EVENT_KEYUP,		/**< 按键释放 */
	UI_EVENT_KEYPRESS,		/**< 按键字符输入 */
	UI_EVENT_TEXTINPUT,		/**< 文本输入 */

	UI_EVENT_MOUSEOVER,		/**< 鼠标在部件上 */
	UI_EVENT_MOUSEMOVE,		/**< 鼠标在部件上移动 */
	UI_EVENT_MOUSEOUT,		/**< 鼠标从部件上移开 */
	UI_EVENT_MOUSEDOWN,		/**< 鼠标按键按下 */
	UI_EVENT_MOUSEUP,		/**< 鼠标按键释放 */
	UI_EVENT_MOUSEWHEEL,		/**< 鼠标滚轮滚动时 */
	UI_EVENT_CLICK,		/**< 鼠标单击 */
	UI_EVENT_DBLCLICK,		/**< 鼠标双击 */
	UI_EVENT_TOUCH,		/**< 触控 */
	UI_EVENT_TOUCHDOWN,		/**< 触点按下 */
	UI_EVENT_TOUCHUP,		/**< 触点释放 */
	UI_EVENT_TOUCHMOVE,		/**< 触点移动 */

	UI_EVENT_TITLE,
	UI_EVENT_FONT_FACE_LOAD,
	UI_EVENT_SURFACE,
	UI_EVENT_USER
} ui_event_type_t;

/* 部件的事件数据结构和系统事件一样 */
typedef LCUI_MouseMotionEvent ui_widget_mouse_motion_event_t;
typedef LCUI_MouseButtonEvent ui_widget_mouse_button_event_t;
typedef LCUI_MouseWheelEvent ui_widget_mouse_wheel_event_t;
typedef LCUI_TextInputEvent ui_widget_text_input_event_t;
typedef LCUI_KeyboardEvent ui_widget_keyboard_event_t;
typedef LCUI_TouchEvent ui_widget_touch_event_t;
typedef struct ui_event_t ui_event_t;
typedef void(*ui_event_handler_t)(ui_widget_t*, ui_event_t*, void*);

/** 面向部件级的事件内容结构 */
typedef struct ui_event_t {
	uint32_t type;			/**< 事件类型标识号 */
	void *data;			/**< 附加数据 */
	ui_widget_t* target;		/**< 触发事件的部件 */
	LCUI_BOOL cancel_bubble;	/**< 是否取消事件冒泡 */
	union {
		ui_widget_mouse_motion_event_t motion;
		ui_widget_mouse_button_event_t button;
		ui_widget_mouse_wheel_event_t wheel;
		ui_widget_keyboard_event_t key;
		ui_widget_touch_event_t touch;
		ui_widget_text_input_event_t text;
	};
};

struct ui_widget_t {
	unsigned hash;
	ui_widget_state_t state;

	char *id;
	char *type;
	strlist_t classes;
	strlist_t status;
	wchar_t *title;
	Dict *attributes;
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

	LCUI_Rect2F padding;
	LCUI_Rect2F margin;
	ui_widget_box_model_t box;

	LCUI_StyleSheet style;
	LCUI_StyleList custom_style;
	LCUI_CachedStyleSheet matched_style;
	ui_widget_style_t computed_style;

	/** Some data bound to the prototype */
	ui_widget_data_t data;

	/**
	 * Prototype chain
	 * It is used to implement the inheritance of widgets,
	 * Just like prototype chain in JavaScript
	 */
	const ui_widget_prototype_t* proto;

	/** Update task context */
	ui_widget_task_t task;
	ui_widget_rules_t *rules;
	ui_widget_listeners_t *listeners;

	/** Invalid area (Dirty Rectangle) */
	LCUI_RectF invalid_area;
	ui_dirty_rect_type_t invalid_area_type;
	LCUI_BOOL has_child_invalid_area;

	/** Parent widget */
	ui_widget_t* parent;

	/** List of child widgets */
	LinkedList children;

	/** List of child widgets in descending order by z-index */
	LinkedList children_show;

	/**
	 * Position in the parent->children
	 * this == LinkedList_Get(&this->parent->children, this.index)
	 */
	size_t index;

	/**
	 * Node in the parent->children
	 * &this->node == LinkedList_GetNode(&this->parent->children, this.index)
	 */
	LinkedListNode node;

	/** Node in the parent->children_shoa */
	LinkedListNode node_show;
};

/* clang-format on */

// Metrics

/** 转换成单位为 px 的度量值 */
LCUI_API float ui_compute(float value, LCUI_StyleType type);

/** 将矩形中的度量值的单位转换为 px */
LCUI_API void ui_compute_rect_actual(LCUI_Rect* dst, const LCUI_RectF* src);

/** 转换成单位为 px 的实际度量值 */
LCUI_API int ui_compute_actual(float value, LCUI_StyleType type);

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

LCUI_API void ui_init_metrics(void);

LCUI_API const ui_metrics_t* ui_get_metrics(void);

/** 获取当前的全局缩放比例 */
INLINE float ui_get_scale(void)
{
	return ui_get_metrics()->scale;
}

INLINE int ui_compute_actual(float value, LCUI_StyleType type)
{
	return iround(ui_compute(value, type) * ui_get_scale());
}

INLINE void ui_compute_rect_actual(LCUI_Rect* dst, const LCUI_RectF* src)
{
	dst->x = iround(src->x * ui_get_scale());
	dst->y = iround(src->y * ui_get_scale());
	dst->width = iround(src->width * ui_get_scale());
	dst->height = iround(src->height * ui_get_scale());
}

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
LCUI_API ui_widget_t* ui_create_widget_with_prototype(
    const ui_widget_prototype_t* proto);
LCUI_API LCUI_BOOL ui_check_widget_type(ui_widget_t* w, const char* type);
LCUI_API LCUI_BOOL
ui_check_widget_prototype(ui_widget_t* w, const ui_widget_prototype_t* proto);
LCUI_API void* ui_widget_get_data(ui_widget_t* widget,
				  ui_widget_prototype_t proto);
LCUI_API void* ui_widget_add_data(ui_widget_t* widget,
				  ui_widget_prototype_t proto,
				  size_t data_size);

// Box

LCUI_API void ui_widget_update_box_size(ui_widget_t* w);
LCUI_API void ui_widget_update_canvas_box(ui_widget_t* w);
LCUI_API void ui_widget_update_box_position(ui_widget_t* w);

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

// Tree

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
LCUI_API void ui_widget_print_tree(ui_widget_t* w);

// Style

#define ui_widget_check_style_type(W, K, T) CheckStyleType((W)->style, K, T)

#define ui_widget_set_style(W, K, VAL, TYPE)    \
	do {                                    \
		LCUI_Style _s;                  \
		_s = ui_widget_get_style(W, K); \
		_s->is_valid = TRUE;            \
		_s->type = LCUI_STYPE_##TYPE;   \
		_s->val_##TYPE = VAL;           \
		Widget_AddTaskByStyle(W, K);    \
	} while (0)

LCUI_BOOL ui_widget_check_style_valid(ui_widget_t* w, int key)
{
	return w->style && w->style->sheet[key].is_valid;
}

INLINE LCUI_BOOL ui_widget_has_auto_style(ui_widget_t* w, int key)
{
	return !ui_widget_check_style_valid(w, key) ||
	       ui_widget_check_style_type(w, key, AUTO);
}

// Hash

LCUI_API void ui_widget_generate_self_hash(ui_widget_t* widget);
LCUI_API void ui_widget_generate_hash(ui_widget_t* w);
LCUI_API size_t ui_widget_export_hash(ui_widget_t* w, unsigned* hash_list,
				      size_t len);
LCUI_API size_t ui_widget_import_hash(ui_widget_t* w, unsigned* hash_list,
				      size_t maxlen);

// Task

// Events

/** 设置阻止部件及其子级部件的事件 */
INLINE ui_widget_block_event(ui_widget_t* w, LCUI_BOOL block)
{
	w->event_blocked = block;
}

/** 触发事件，让事件处理器在主循环中调用 */
LCUI_API int ui_widget_post_event(ui_widget_t* w, ui_event_t e, void* arg,
				  void (*destroy_data)(void*));

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

INLINE int ui_post_event(ui_event_t e, void* data,
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
	return ui_widget_on_event(ui_root(), event_name, handler, data,
				  destroy_data);
}

INLINE int ui_off_event(const char* event_name, ui_event_handler_t handler)
{
	return ui_widget_on_event(ui_root(), event_name, handler);
}

/**
 * 投递表面（surface）事件
 * 表面是与顶层部件绑定在一起的，只有当部件为顶层部件时，才能投递表面事件。
 * 表面事件主要用于让表面与部件同步一些数据，如：大小、位置、显示/隐藏。
 * @param event_type 事件类型
 * @param @sync_props 是否将部件的属性同步给表面
 */
LCUI_API int ui_widget_post_surface_event(ui_widget_t* w, int event_type,
					  LCUI_BOOL sync_props);

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

// Image Loader

#define ui_image_on_event ui_image_add_event_listener
#define ui_image_off_event ui_image_remove_event_listener

typedef struct ui_image_t ui_image_t;
typedef void (*ui_image_event_handler_t)(ui_image_t*, void*);

LCUI_API ui_image_t* ui_load_image(const char* path);
LCUI_API LCUI_Graph* ui_image_get_data(ui_image_t* image);
LCUI_API const char* ui_image_get_path(ui_image_t* image);
LCUI_API void ui_image_add_ref(ui_image_t* image);
LCUI_API void ui_image_remove_ref(ui_image_t* image);
LCUI_API int ui_image_add_event_listener(ui_image_t* image,
					 ui_image_event_handler_t handler,
					 void* data);
LCUI_API int ui_image_remove_event_listener(ui_image_t* image,
					    ui_image_event_handler_t handler,
					    void* data);

#endif
