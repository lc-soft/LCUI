/*
 * widget_base.h -- The widget base operation set.
 *
 * Copyright (c) 2018-2020, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LCUI_WIDGET_BASE_H
#define LCUI_WIDGET_BASE_H

#include <LCUI/util/strlist.h>
#include <LCUI/gui/css_library.h>

LCUI_BEGIN_HEADER

/* clang-format off */

/** 部件样式 */
typedef struct LCUI_WidgetStyle {
	LCUI_BOOL visible;
	LCUI_BOOL focusable;
	LCUI_SizingRule width_sizing;
	LCUI_SizingRule height_sizing;
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
	LCUI_FlexBoxLayoutStyle flex;
	int pointer_events;
} LCUI_WidgetStyle;

typedef struct LCUI_WidgetActualStyleRec_ {
	float x, y;
	LCUI_Rect canvas_box;
	LCUI_Rect border_box;
	LCUI_Rect padding_box;
	LCUI_Rect content_box;
	LCUI_Border border;
	LCUI_BoxShadow shadow;
	LCUI_Background background;
} LCUI_WidgetActualStyleRec, *LCUI_WidgetActualStyle;

/** 部件任务类型，按照任务的依赖顺序排列 */
typedef enum LCUI_WidgetTaskType {
	LCUI_WTASK_REFRESH_STYLE,	/**< 刷新部件全部样式 */
	LCUI_WTASK_UPDATE_STYLE,	/**< 更新部件自定义样式 */
	LCUI_WTASK_TITLE,
	LCUI_WTASK_PROPS,		/**< 更新一些属性 */
	LCUI_WTASK_BOX_SIZING,
	LCUI_WTASK_PADDING,
	LCUI_WTASK_MARGIN,
	LCUI_WTASK_VISIBLE,
	LCUI_WTASK_DISPLAY,
	LCUI_WTASK_FLEX,
	LCUI_WTASK_SHADOW,
	LCUI_WTASK_BORDER,
	LCUI_WTASK_BACKGROUND,
	LCUI_WTASK_POSITION,
	LCUI_WTASK_RESIZE,
	LCUI_WTASK_ZINDEX,
	LCUI_WTASK_OPACITY,
	LCUI_WTASK_REFLOW,
	LCUI_WTASK_USER,
	LCUI_WTASK_TOTAL_NUM
} LCUI_WidgetTaskType;

/** See more: https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Box_Model */
typedef struct LCUI_WidgetBoxModelRec_ {
	LCUI_RectF content;
	LCUI_RectF padding;
	LCUI_RectF border;
	LCUI_RectF canvas;
	LCUI_RectF outer;
} LCUI_WidgetBoxModelRec, *LCUI_WidgetBoxModel;

typedef struct LCUI_WidgetTaskRec_ {
	/** Should update for self? */
	LCUI_BOOL for_self;

	/** Should update for children? */
	LCUI_BOOL for_children;

	/** Should skip the property sync of bound surface? */
	LCUI_BOOL skip_surface_props_sync;

	/** States of tasks */
	LCUI_BOOL states[LCUI_WTASK_TOTAL_NUM];
} LCUI_WidgetTaskRec;

/** 部件状态 */
typedef enum LCUI_WidgetState {
	LCUI_WSTATE_CREATED = 0,
	LCUI_WSTATE_UPDATED,
	LCUI_WSTATE_LAYOUTED,
	LCUI_WSTATE_READY,
	LCUI_WSTATE_NORMAL,
	LCUI_WSTATE_DELETED,
} LCUI_WidgetState;

typedef struct LCUI_WidgetRec_* LCUI_Widget;
typedef struct LCUI_WidgetPrototypeRec_ *LCUI_WidgetPrototype;
typedef const struct LCUI_WidgetPrototypeRec_ *LCUI_WidgetPrototypeC;

typedef void(*LCUI_WidgetFunction)(LCUI_Widget);
typedef void(*LCUI_WidgetTaskHandler)(LCUI_Widget, int);
typedef void(*LCUI_WidgetSizeGetter)(LCUI_Widget, float*, float*, LCUI_LayoutRule);
typedef void(*LCUI_WidgetSizeSetter)(LCUI_Widget, float, float);
typedef void(*LCUI_WidgetAttrSetter)(LCUI_Widget, const char*, const char*);
typedef void(*LCUI_WidgetTextSetter)(LCUI_Widget, const char*);
typedef void(*LCUI_WidgetPropertyBinder)(LCUI_Widget, const char*, LCUI_Object);
typedef void(*LCUI_WidgetPainter)(LCUI_Widget, LCUI_PaintContext,
				  LCUI_WidgetActualStyle);

typedef struct LCUI_WidgetPrototypeRec_ {
	char *name;
	LCUI_WidgetFunction init;
	LCUI_WidgetFunction refresh;
	LCUI_WidgetFunction destroy;
	LCUI_WidgetFunction update;
	LCUI_WidgetTaskHandler runtask;
	LCUI_WidgetAttrSetter setattr;
	LCUI_WidgetTextSetter settext;
	LCUI_WidgetPropertyBinder bindprop;
	LCUI_WidgetSizeGetter autosize;
	LCUI_WidgetSizeSetter resize;
	LCUI_WidgetPainter paint;
	LCUI_WidgetPrototype proto;
} LCUI_WidgetPrototypeRec;

typedef struct LCUI_WidgetDataEntryRec_ {
	void *data;
	LCUI_WidgetPrototype proto;
} LCUI_WidgetDataEntryRec;

typedef struct LCUI_WidgetData_ {
	unsigned length;
	LCUI_WidgetDataEntryRec *list;
} LCUI_WidgetData;

/* clang-format on */

typedef struct LCUI_WidgetRulesRec_ {
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
	 * Widget_GenerateHash() to generate a hash value for the children
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
	void (*on_update_progress)(LCUI_Widget, size_t);
} LCUI_WidgetRulesRec, *LCUI_WidgetRules;

/* clang-format off */

typedef struct LCUI_WidgetRulesDataRec_ {
	LCUI_WidgetRulesRec rules;
	Dict *style_cache;
	size_t default_max_update_count;
	size_t progress;
} LCUI_WidgetRulesDataRec, *LCUI_WidgetRulesData;

typedef struct LCUI_WidgetAttributeRec_ {
	char *name;
	struct {
		int type;
		void(*destructor)(void*);
		union {
			char *string;
			void *data;
		};
	} value;
} LCUI_WidgetAttributeRec, *LCUI_WidgetAttribute;

typedef enum LCUI_InvalidAreaType_ {
	LCUI_INVALID_AREA_TYPE_NONE,
	LCUI_INVALID_AREA_TYPE_CUSTOM,
	LCUI_INVALID_AREA_TYPE_PADDING_BOX,
	LCUI_INVALID_AREA_TYPE_BORDER_BOX,
	LCUI_INVALID_AREA_TYPE_CANVAS_BOX
} LCUI_InvalidAreaType;

typedef struct LCUI_WidgetRec_ {
	unsigned hash;
	LCUI_WidgetState state;

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
	LCUI_WidgetBoxModelRec box;

	LCUI_StyleSheet style;
	LCUI_StyleList custom_style;
	LCUI_CachedStyleSheet inherited_style;
	LCUI_WidgetStyle computed_style;

	/** Some data bound to the prototype */
	LCUI_WidgetData data;

	/**
	 * Prototype chain
	 * It is used to implement the inheritance of widgets,
	 * Just like prototype chain in JavaScript
	 */
	LCUI_WidgetPrototypeC proto;

	/** Update task context */
	LCUI_WidgetTaskRec task;
	LCUI_WidgetRules rules;
	LCUI_EventTrigger trigger;

	/** Invalid area (Dirty Rectangle) */
	LCUI_RectF invalid_area;
	LCUI_InvalidAreaType invalid_area_type;
	LCUI_BOOL has_child_invalid_area;
	
	/** Parent widget */
	LCUI_Widget parent;

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
} LCUI_WidgetRec;

/* clang-format on */

#define Widget_SetStyle(W, K, VAL, TYPE)      \
	do {                                  \
		LCUI_Style _s;                \
		_s = Widget_GetStyle(W, K);   \
		_s->is_valid = TRUE;          \
		_s->type = LCUI_STYPE_##TYPE; \
		_s->val_##TYPE = VAL;         \
		Widget_AddTaskByStyle(W, K);  \
	} while (0)

#define Widget_CheckStyleType(W, K, T) CheckStyleType((W)->style, K, T)

#define Widget_HasAbsolutePosition(W) \
	((W)->computed_style.position == SV_ABSOLUTE)

#define Widget_HasBlockDisplay(W) ((W)->computed_style.display == SV_BLOCK)

#define Widget_HasFlexDisplay(W) ((W)->computed_style.display == SV_FLEX)

#define Widget_HasInlineBlockDisplay(W) \
	((W)->computed_style.display == SV_INLINE_BLOCK)

#define Widget_HasFillAvailableWidth(W)                             \
	((Widget_HasBlockDisplay(W) || Widget_HasFlexDisplay(W)) && \
	 !Widget_HasAbsolutePosition(W))

#define Widget_HasScaleSize(W)                         \
	(Widget_CheckStyleType(W, key_width, SCALE) || \
	 Widget_CheckStyleType(W, key_height, SCALE))

INLINE LCUI_BOOL Widget_IsFlexLayoutStyleWorks(LCUI_Widget w)
{
	return Widget_HasFlexDisplay(w) ||
	       (!Widget_HasAbsolutePosition(w) && w->parent &&
		Widget_HasFlexDisplay(w->parent));
}

LCUI_API float Widget_ComputeXMetric(LCUI_Widget w, int key);

LCUI_API float Widget_ComputeYMetric(LCUI_Widget w, int key);

LCUI_API LCUI_BOOL Widget_HasAutoStyle(LCUI_Widget w, int key);

LCUI_API LCUI_Widget LCUIWidget_GetRoot(void);

LCUI_API LCUI_Widget LCUIWidget_GetById(const char *idstr);

/** Create a widget by prototype */
LCUI_API LCUI_Widget LCUIWidget_NewWithPrototype(LCUI_WidgetPrototypeC proto);

/** Create a widget by type name */
LCUI_API LCUI_Widget LCUIWidget_New(const char *type_name);

/** Execute destruction task */
LCUI_API void Widget_ExecDestroy(LCUI_Widget w);

/** Mark a Widget needs to be destroyed */
LCUI_API void Widget_Destroy(LCUI_Widget w);

LCUI_API void Widget_GetOffset(LCUI_Widget w, LCUI_Widget parent,
			       float *offset_x, float *offset_y);

LCUI_API void LCUIWidget_InitImageLoader(void);

LCUI_API void LCUIWidget_FreeImageLoader(void);

LCUI_API void Widget_InitBackground(LCUI_Widget w);

LCUI_API void Widget_DestroyBackground(LCUI_Widget w);

LCUI_API void Widget_ComputeBackgroundStyle(LCUI_Widget widget);

LCUI_API void Widget_PaintBakcground(LCUI_Widget w, LCUI_PaintContext paint,
				     LCUI_WidgetActualStyle style);

LCUI_API void Widget_ComputeBackground(LCUI_Widget w, LCUI_Background *out);

LCUI_API float Widget_GetCanvasWidth(LCUI_Widget widget);

LCUI_API float Widget_GetCanvasHeight(LCUI_Widget widget);

LCUI_API float Widget_GetBoxShadowOffsetX(LCUI_Widget w);

LCUI_API float Widget_GetBoxShadowOffsetY(LCUI_Widget w);

LCUI_API float Widget_GetCanvasWidth(LCUI_Widget w);

LCUI_API float Widget_GetCanvasWidth(LCUI_Widget w);

LCUI_API void Widget_ComputeBoxShadow(LCUI_Widget w, LCUI_BoxShadow *out);

LCUI_API void Widget_ComputeBoxShadowStyle(LCUI_Widget w);

LCUI_API void Widget_PaintBoxShadow(LCUI_Widget w, LCUI_PaintContext paint,
				    LCUI_WidgetActualStyle style);

LCUI_API int Widget_Top(LCUI_Widget w);

LCUI_API void Widget_SortChildrenShow(LCUI_Widget w);

LCUI_API void Widget_SetTitleW(LCUI_Widget w, const wchar_t *title);

LCUI_API void Widget_AddState(LCUI_Widget w, LCUI_WidgetState state);

/** Check whether the widget is in the visible area */
LCUI_API LCUI_BOOL Widget_InVisibleArea(LCUI_Widget w);

/** Set widget updating rules */
LCUI_API int Widget_SetRules(LCUI_Widget w, const LCUI_WidgetRulesRec *rules);

/** Set widget content text */
LCUI_API void Widget_SetText(LCUI_Widget w, const char *text);

/* Bind an object to a widget property, and the widget property is automatically
 * updated when the value of the object changes */
LCUI_API void Widget_BindProperty(LCUI_Widget w, const char *name,
				  LCUI_Object value);

LCUI_API void Widget_UpdateBoxPosition(LCUI_Widget w);

LCUI_API void Widget_UpdateCanvasBox(LCUI_Widget w);

LCUI_API void Widget_UpdateBoxSize(LCUI_Widget w);

LCUI_API size_t LCUIWidget_ClearTrash(void);

LCUI_API void LCUIWidget_InitBase(void);

LCUI_API void LCUIWidget_FreeRoot(void);

LCUI_API void LCUIWidget_FreeBase(void);

LCUI_END_HEADER

#endif
