/*
 * widget_base.h -- The widget base operation set.
 *
 * Copyright (c) 2018-2019, Liu chao <lc-soft@live.cn> All rights reserved.
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
	LCUI_BOOL visible;			/**< 是否可见 */
	LCUI_BOOL focusable;			/**< 是否能够得到焦点 */
	float min_width, min_height;		/**< 最小尺寸 */
	float max_width, max_height;		/**< 最大尺寸 */
	float left, top;			/**< 左边界、顶边界的偏移距离 */
	float right, bottom;			/**< 右边界、底边界的偏移距离 */
	int z_index;				/**< 堆叠顺序，该值越高，部件显示得越靠前 */
	float opacity;				/**< 不透明度，有效范围从 0.0 （完全透明）到 1.0（完全不透明） */
	LCUI_StyleValue position;		/**< 定位方式 */
	LCUI_StyleValue display;		/**< 显示方式，决定以何种布局显示该部件 */
	LCUI_StyleValue box_sizing;		/**< 以何种方式计算宽度和高度 */
	LCUI_StyleValue vertical_align;		/**< 垂直对齐方式 */
	LCUI_BoundBox margin;			/**< 外边距 */
	LCUI_BoundBox padding;			/**< 内边距 */
	LCUI_BorderStyle border;		/**< 边框 */
	LCUI_BoxShadowStyle shadow;		/**< 盒形阴影 */
	LCUI_BackgroundStyle background;	/**< 背景 */
	LCUI_FlexLayoutStyle flex;		/**< 弹性布局相关样式 */
	int pointer_events;			/**< 事件的处理方式 */
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
	LCUI_WTASK_SHADOW,
	LCUI_WTASK_BORDER,
	LCUI_WTASK_BACKGROUND,
	LCUI_WTASK_LAYOUT,
	LCUI_WTASK_RESIZE,
	LCUI_WTASK_POSITION,
	LCUI_WTASK_ZINDEX,
	LCUI_WTASK_OPACITY,
	LCUI_WTASK_USER,
	LCUI_WTASK_TOTAL_NUM
} LCUI_WidgetTaskType;

typedef struct LCUI_WidgetBoxModelRec_ {
	LCUI_RectF content;	/**< 内容框的区域 */
	LCUI_RectF padding;	/**< 内边距框的区域 */
	LCUI_RectF border;	/**< 边框盒的区域，包括内边距框和内容框区域 */
	LCUI_RectF outer;	/**< 外边距框的区域，包括边框盒和外边距框区域 */
	LCUI_RectF canvas;	/**< 图层的区域，包括边框盒和阴影区域 */
} LCUI_WidgetBoxModelRec, *LCUI_WidgetBoxModel;

typedef struct LCUI_WidgetTaskBoxRec_ {
	/** update for self */

	LCUI_BOOL for_self;

	/** update for children */
	LCUI_BOOL for_children;

	/** skip the property synchronization of bound surface */
	LCUI_BOOL skip_surface_props_sync;

	/** states of tasks */
	LCUI_BOOL states[LCUI_WTASK_TOTAL_NUM];
} LCUI_WidgetTaskBoxRec;

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
typedef void(*LCUI_WidgetResizer)(LCUI_Widget, float*, float*);
typedef void(*LCUI_WidgetAttrSetter)(LCUI_Widget, const char*, const char*);
typedef void(*LCUI_WidgetTextSetter)(LCUI_Widget, const char*);
typedef void(*LCUI_WidgetPropertyBinder)(LCUI_Widget, const char*, LCUI_Object);
typedef void(*LCUI_WidgetPainter)(LCUI_Widget, LCUI_PaintContext,
				  LCUI_WidgetActualStyle);

/** 部件原型数据结构 */
typedef struct LCUI_WidgetPrototypeRec_ {
	char *name;				/**< 名称 */
	LCUI_WidgetFunction init;		/**< 构造函数  */
	LCUI_WidgetFunction refresh;		/**< 数据刷新函数 */
	LCUI_WidgetFunction destroy;		/**< 析构函数 */
	LCUI_WidgetFunction update;		/**< 样式处理函数 */
	LCUI_WidgetFunction runtask;		/**< 自定义任务处理函数 */
	LCUI_WidgetAttrSetter setattr;		/**< 属性设置函数 */
	LCUI_WidgetTextSetter settext;		/**< 文本内容设置函数 */
	LCUI_WidgetPropertyBinder bindprop;	/**< 属性绑定函数 */
	LCUI_WidgetResizer autosize;		/**< 内容尺寸计算函数 */
	LCUI_WidgetPainter paint;		/**< 绘制函数 */
	LCUI_WidgetPrototype proto;		/**< 父级原型 */
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

/** 部件结构 */
typedef struct LCUI_WidgetRec_ {
	unsigned		hash;			/**< 哈希值 */
	LCUI_WidgetState	state;			/**< 状态 */
	float			x, y;			/**< 当前坐标（由 origin 计算而来） */
	float			origin_x, origin_y;	/**< 当前布局下计算出的坐标 */
	float			width, height;		/**< 部件区域大小，包括边框和内边距占用区域 */
	size_t			index;			/**< 部件索引位置 */
	char			*id;			/**< ID */
	char			*type;			/**< 类型 */
	strlist_t		classes;		/**< 类列表 */
	strlist_t		status;			/**< 状态列表 */
	wchar_t			*title;			/**< 标题 */
	LCUI_Rect2F		padding;		/**< 内边距框 */
	LCUI_Rect2F		margin;			/**< 外边距框 */
	LCUI_WidgetBoxModelRec	box;			/**< 部件的各个区域信息 */
	LCUI_StyleSheet		style;			/**< 当前完整样式表 */
	LCUI_StyleList		custom_style;		/**< 自定义样式表 */
	LCUI_CachedStyleSheet	inherited_style;	/**< 通过继承得到的样式表 */
	LCUI_WidgetStyle	computed_style;		/**< 已经计算的样式数据 */
	LCUI_Widget		parent;			/**< 父部件 */
	LinkedList		children;		/**< 子部件 */
	LinkedList		children_show;		/**< 子部件的堆叠顺序记录，由顶到底 */
	LCUI_WidgetData		data;			/**< 私有数据 */
	Dict			*attributes;		/**< 属性记录 */
	LCUI_WidgetPrototypeC	proto;			/**< 原型 */
	LCUI_EventTrigger	trigger;		/**< 事件触发器 */
	LCUI_WidgetTaskBoxRec	task;			/**< 任务记录 */
	LCUI_WidgetRules	rules;			/**< 更新部件时采用的规则 */
	LCUI_BOOL		event_blocked;		/**< 是否阻止自己和子级部件的事件处理 */
	LCUI_BOOL		disabled;		/**< 是否禁用 */
	LinkedListNode		node;			/**< 在部件链表中的结点 */
	LinkedListNode		node_show;		/**< 在部件显示链表中的结点 */
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

#define Widget_HasParentDependentWidth(W)              \
	(Widget_CheckStyleType(W, key_width, scale) && \
	 !Widget_HasStaticWidthParent(W))

LCUI_API float Widget_ComputeXMetric(LCUI_Widget w, int key);

LCUI_API float Widget_ComputeYMetric(LCUI_Widget w, int key);

/** 部件是否有值为自动（默认）的样式 */
LCUI_API LCUI_BOOL Widget_HasAutoStyle(LCUI_Widget w, int key);

/** 父级部件有可直接获取的静态宽度 */
LCUI_API LCUI_BOOL Widget_HasStaticWidthParent(LCUI_Widget widget);

/** 如果部件具有自适应内容的宽度 */
LCUI_API LCUI_BOOL Widget_HasFitContentWidth(LCUI_Widget w);

/** 获取根级部件 */
LCUI_API LCUI_Widget LCUIWidget_GetRoot(void);

/** 获取指定ID的部件 */
LCUI_API LCUI_Widget LCUIWidget_GetById(const char *idstr);

/** Create a widget by prototype */
LCUI_API LCUI_Widget LCUIWidget_NewWithPrototype(LCUI_WidgetPrototypeC proto);

/** Create a widget by type name */
LCUI_API LCUI_Widget LCUIWidget_New(const char *type_name);

/** Execute destruction task */
LCUI_API void Widget_ExecDestroy(LCUI_Widget w);

/** Mark a Widget needs to be destroyed */
LCUI_API void Widget_Destroy(LCUI_Widget w);

/** 获取相对于父级指定部件的 XY 坐标 */
LCUI_API void Widget_GetOffset(LCUI_Widget w, LCUI_Widget parent,
			       float *offset_x, float *offset_y);

/** 初始化图片加载器，用于加载部件背景图 */
LCUI_API void LCUIWidget_InitImageLoader(void);

/** 退出并销毁图片加载器 */
LCUI_API void LCUIWidget_FreeImageLoader(void);

/** 初始化部件背景样式 */
LCUI_API void Widget_InitBackground(LCUI_Widget w);

/** 销毁部件背景相关数据 */
LCUI_API void Widget_DestroyBackground(LCUI_Widget w);

/** 更新部件背景样式 */
LCUI_API void Widget_ComputeBackgroundStyle(LCUI_Widget widget);

/** 绘制部件背景 */
LCUI_API void Widget_PaintBakcground(LCUI_Widget w, LCUI_PaintContext paint,
				     LCUI_WidgetActualStyle style);

/** 计算部件背景样式的实际值 */
LCUI_API void Widget_ComputeBackground(LCUI_Widget w, LCUI_Background *out);

/** 计算部件边框样式的实际值 */
LCUI_API void Widget_ComputeBorder(LCUI_Widget w, LCUI_Border *out);

LCUI_API void Widget_ComputeBorderStyle(LCUI_Widget w);

/** 绘制部件边框 */
LCUI_API void Widget_PaintBorder(LCUI_Widget w, LCUI_PaintContext paint,
				 LCUI_WidgetActualStyle style);

LCUI_API void Widget_CropContent(LCUI_Widget w, LCUI_PaintContext paint,
				 LCUI_WidgetActualStyle style);

LCUI_API float Widget_GetCanvasWidth(LCUI_Widget widget);

LCUI_API float Widget_GetCanvasHeight(LCUI_Widget widget);

LCUI_API float Widget_GetLimitedWidth(LCUI_Widget w, float width);

LCUI_API float Widget_GetLimitedHeight(LCUI_Widget w, float height);

LCUI_API void Widget_AutoSize(LCUI_Widget w);

LCUI_API void Widget_ComputeSizeStyle(LCUI_Widget w);

/** 根据阴影参数获取部件区域的横向偏移距离 */
LCUI_API float Widget_GetBoxShadowOffsetX(LCUI_Widget w);

/** 根据阴影参数获取部件区域的纵向偏移距离 */
LCUI_API float Widget_GetBoxShadowOffsetY(LCUI_Widget w);

/** 获取部件在添加阴影后的高度 */
LCUI_API float Widget_GetCanvasWidth(LCUI_Widget w);

/** 获取部件在添加阴影后的宽度 */
LCUI_API float Widget_GetCanvasWidth(LCUI_Widget w);

/** 计算部件阴影样式的实际值 */
LCUI_API void Widget_ComputeBoxShadow(LCUI_Widget w, LCUI_BoxShadow *out);

LCUI_API void Widget_ComputeBoxShadowStyle(LCUI_Widget w);

/** 绘制部件阴影 */
LCUI_API void Widget_PaintBoxShadow(LCUI_Widget w, LCUI_PaintContext paint,
				    LCUI_WidgetActualStyle style);

/** 更新可见性 */
/** 设置部件为顶级部件 */
LCUI_API int Widget_Top(LCUI_Widget w);

LCUI_API void Widget_SortChildrenShow(LCUI_Widget w);

/** 设置部件标题 */
LCUI_API void Widget_SetTitleW(LCUI_Widget w, const wchar_t *title);

/** 为部件添加状态 */
LCUI_API void Widget_AddState(LCUI_Widget w, LCUI_WidgetState state);

/** Check whether the widget is in the visible area */
LCUI_API LCUI_BOOL Widget_InVisibleArea(LCUI_Widget w);

/** Generate a hash for a widget to identify it and siblings */
LCUI_API void Widget_GenerateSelfHash(LCUI_Widget w);

/** Generate hash values for a widget and its children */
LCUI_API void Widget_GenerateHash(LCUI_Widget w);

LCUI_API size_t Widget_SetHashList(LCUI_Widget w, unsigned *hash_list,
				   size_t len);

LCUI_API size_t Widget_GetHashList(LCUI_Widget w, unsigned *hash_list,
				   size_t maxlen);

/** Set widget updating rules */
LCUI_API int Widget_SetRules(LCUI_Widget w, const LCUI_WidgetRulesRec *rules);

/** Set widget content text */
LCUI_API void Widget_SetText(LCUI_Widget w, const char *text);

/* Bind an object to a widget property, and the widget property is automatically
 * updated when the value of the object changes */
LCUI_API void Widget_BindProperty(LCUI_Widget w, const char *name,
				  LCUI_Object value);

/** 计算部件的最大宽度 */
LCUI_API float Widget_ComputeMaxWidth(LCUI_Widget w);

/** 计算部件可利用的宽度 */
LCUI_API float Widget_ComputeFillAvailableWidth(LCUI_Widget w);

/** 计算部件的最大内容宽度 */
LCUI_API float Widget_ComputeMaxContentWidth(LCUI_Widget w);

/** 计算部件的最大可用宽度 */
LCUI_API float Widget_ComputeMaxAvaliableWidth(LCUI_Widget widget);

LCUI_API void Widget_ComputeLimitSize(LCUI_Widget w);

LCUI_API size_t LCUIWidget_ClearTrash(void);

LCUI_API void LCUIWidget_InitBase(void);

LCUI_API void LCUIWidget_FreeRoot(void);

LCUI_API void LCUIWidget_FreeBase(void);

LCUI_END_HEADER

#endif
