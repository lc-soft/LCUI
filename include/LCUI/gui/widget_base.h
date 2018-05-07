/*
 * widget_base.h -- The widget base operation set.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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

#include <LCUI/gui/css_library.h>

LCUI_BEGIN_HEADER

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
	LCUI_WTASK_RESIZE_WITH_SURFACE,
	LCUI_WTASK_POSITION,
	LCUI_WTASK_ZINDEX,
	LCUI_WTASK_OPACITY,
	LCUI_WTASK_BODY,
	LCUI_WTASK_REFRESH,
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
	LCUI_BOOL for_self;			/**< 标志，指示当前部件是否有待处理的任务 */
	LCUI_BOOL for_children;			/**< 标志，指示是否有待处理的子级部件 */
	LCUI_BOOL buffer[LCUI_WTASK_TOTAL_NUM];	/**< 记录缓存 */
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

typedef void( *LCUI_WidgetFunction )(LCUI_Widget);
typedef void( *LCUI_WidgetResizer )(LCUI_Widget, float*, float*);
typedef void( *LCUI_WidgetAttrSetter )(LCUI_Widget, const char*, const char*);
typedef void( *LCUI_WidgetTextSetter )(LCUI_Widget, const char*);
typedef void( *LCUI_WidgetPainter )(LCUI_Widget, LCUI_PaintContext,
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
	LCUI_WidgetResizer autosize;		/**< 内容尺寸计算函数 */
	LCUI_WidgetPainter paint;		/**< 绘制函数 */
	LCUI_WidgetPrototype proto;		/**< 父级原型 */
} LCUI_WidgetPrototypeRec;

typedef struct LCUI_WidgetDataEntryRec_ {
	void *data;
	LCUI_WidgetPrototype proto;
} LCUI_WidgetDataEntryRec;

typedef struct LCUI_WidgetData_ {
	uint_t length;
	LCUI_WidgetDataEntryRec *list;
} LCUI_WidgetData;

typedef struct LCUI_WidgetAttributeRec_ {
	char *name;
	struct {
		int type;
		void (*destructor)(void*);
		union {
			char *string;
			void *data;
		};
	} value;
} LCUI_WidgetAttributeRec, *LCUI_WidgetAttribute;

/** 部件结构 */
typedef struct LCUI_WidgetRec_ {
	LCUI_WidgetState	state;			/**< 状态 */
	float			x, y;			/**< 当前坐标（由 origin 计算而来） */
	float			origin_x, origin_y;	/**< 当前布局下计算出的坐标 */
	float			width, height;		/**< 部件区域大小，包括边框和内边距占用区域 */
	uint_t			index;			/**< 部件索引位置 */
	char			*id;			/**< ID */
	char			*type;			/**< 类型 */
	char			**classes;		/**< 类列表 */
	char			**status;		/**< 状态列表 */
	wchar_t			*title;			/**< 标题 */
	LCUI_Rect2F		padding;		/**< 内边距框 */
	LCUI_Rect2F		margin;			/**< 外边距框 */
	LCUI_WidgetBoxModelRec	box;			/**< 部件的各个区域信息 */
	LCUI_StyleSheet		style;			/**< 当前完整样式表 */
	LCUI_StyleSheet		custom_style;		/**< 自定义样式表 */
	LCUI_StyleSheet		inherited_style;	/**< 通过继承得到的样式表 */
	LCUI_WidgetStyle	computed_style;		/**< 已经计算的样式数据 */
	LCUI_Widget		parent;			/**< 父部件 */
	LinkedList		children;		/**< 子部件 */
	LinkedList		children_show;		/**< 子部件的堆叠顺序记录，由顶到底 */
	LCUI_WidgetData		data;			/**< 私有数据 */
	Dict			*attributes;		/**< 属性记录 */
	LCUI_WidgetPrototypeC	proto;			/**< 原型 */
	LCUI_EventTrigger	trigger;		/**< 事件触发器 */
	LCUI_WidgetTaskBoxRec	task;			/**< 任务记录 */
	LCUI_BOOL		event_blocked;		/**< 是否阻止自己和子级部件的事件处理 */
	LCUI_BOOL		disabled;		/**< 是否禁用 */
	LinkedListNode		node;			/**< 在部件链表中的结点 */
	LinkedListNode		node_show;		/**< 在部件显示链表中的结点 */
} LCUI_WidgetRec;

#define Widget_SetStyle(W, K, V, T) SetStyle((W)->custom_style, K, V, T)
#define Widget_UnsetStyle(W, K) UnsetStyle((W)->custom_style, K)
#define Widget_CheckStyleType(W, K, T) CheckStyleType((W)->style, K, T)
#define Widget_CheckStyleValue(W, K, T) CheckStyleValue((W)->style, K, T)
#define Widget_CheckStyleValid(W, K) W->style->sheet[K].is_valid

#define Widget_HasAbsolutePosition(W) \
((W)->computed_style.position == SV_ABSOLUTE)

#define Widget_HasBlockDisplay(W) \
((W)->computed_style.display == SV_BLOCK)

#define Widget_HasFlexDisplay(W) \
((W)->computed_style.display == SV_FLEX)

#define Widget_HasInlineBlockDisplay(W) \
((W)->computed_style.display == SV_INLINE_BLOCK)

#define Widget_HasFillAvailableWidth(W) \
((Widget_HasBlockDisplay( W ) || Widget_HasFlexDisplay( W )) && \
!Widget_HasAbsolutePosition( W ))

#define Widget_HasScaleSize(W) \
(Widget_CheckStyleType( W, key_width, SCALE ) ||\
Widget_CheckStyleType( W, key_height, SCALE ))

#define Widget_HasParentDependentWidth(W) \
	(Widget_CheckStyleType( W, key_width, scale ) &&\
	!Widget_HasStaticWidthParent( W ))

/** 部件是否有值为自动（默认）的样式 */
LCUI_API LCUI_BOOL Widget_HasAutoStyle( LCUI_Widget w, int key );

/** 父级部件有可直接获取的静态宽度 */
LCUI_API LCUI_BOOL Widget_HasStaticWidthParent( LCUI_Widget widget );

/** 如果部件具有自适应内容的宽度 */
LCUI_API LCUI_BOOL Widget_HasFitContentWidth( LCUI_Widget w );

/** 获取根级部件 */
LCUI_API LCUI_Widget LCUIWidget_GetRoot(void);

/** 获取指定ID的部件 */
LCUI_API LCUI_Widget LCUIWidget_GetById( const char *idstr );

/** 新建一个GUI部件 */
LCUI_API LCUI_Widget LCUIWidget_New( const char *type_name );

/** 直接销毁部件 */
LCUI_API void Widget_ExecDestroy( LCUI_Widget w );

/** 销毁部件 */
LCUI_API void Widget_Destroy( LCUI_Widget w );

/** 将部件与子部件列表断开链接 */
LCUI_API int Widget_Unlink( LCUI_Widget widget );

/** 向子部件列表追加部件 */
LCUI_API int Widget_Append( LCUI_Widget container, LCUI_Widget widget );

/** 将部件插入到子部件列表的开头处 */
LCUI_API int Widget_Prepend( LCUI_Widget parent, LCUI_Widget widget );

/** 移除部件，并将其子级部件转移至父部件内 */
LCUI_API int Widget_Unwrap( LCUI_Widget widget );

/** 清空部件内的子级部件 */
LCUI_API void Widget_Empty( LCUI_Widget widget );

/** 获取上一个部件 */
LCUI_API LCUI_Widget Widget_GetPrev( LCUI_Widget w );

/** 获取下一个部件 */
LCUI_API LCUI_Widget Widget_GetNext( LCUI_Widget w );

/** 获取一个子部件 */
LCUI_API LCUI_Widget Widget_GetChild( LCUI_Widget w, size_t index );

/** 获取当前点命中的最上层可见部件 */
LCUI_API LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y );

/** 获取相对于父级指定部件的 XY 坐标 */
LCUI_API void Widget_GetOffset( LCUI_Widget w, LCUI_Widget parent,
				float *offset_x, float *offset_y );

/** 初始化图片加载器，用于加载部件背景图 */
LCUI_API void LCUIWidget_InitImageLoader( void );

/** 退出并销毁图片加载器 */
LCUI_API void LCUIWidget_FreeImageLoader( void );

/** 初始化部件背景样式 */
LCUI_API void Widget_InitBackground( LCUI_Widget w );

/** 销毁部件背景相关数据 */
LCUI_API void Widget_DestroyBackground( LCUI_Widget w );

/** 更新部件背景样式 */
LCUI_API void Widget_UpdateBackground( LCUI_Widget widget );

/** 绘制部件背景 */
LCUI_API void Widget_PaintBakcground( LCUI_Widget w, LCUI_PaintContext paint,
				      LCUI_WidgetActualStyle style );

/** 计算部件背景样式的实际值 */
LCUI_API void Widget_ComputeBackground( LCUI_Widget w, LCUI_Background *out );

/** 更新部件边框样式 */
LCUI_API void Widget_UpdateBorder( LCUI_Widget w );

/** 计算部件边框样式的实际值 */
LCUI_API void Widget_ComputeBorder( LCUI_Widget w, LCUI_Border *out );

/** 绘制部件边框 */
LCUI_API void Widget_PaintBorder( LCUI_Widget w, LCUI_PaintContext paint,
				  LCUI_WidgetActualStyle style );

LCUI_API float Widget_GetCanvasWidth( LCUI_Widget widget );

LCUI_API float Widget_GetCanvasHeight( LCUI_Widget widget );

LCUI_API float Widget_GetLimitedWidth( LCUI_Widget w, float width );

LCUI_API float Widget_GetLimitedHeight( LCUI_Widget w, float height );

LCUI_API void Widget_AutoSize( LCUI_Widget w );

/** 根据阴影参数获取部件区域的横向偏移距离 */
LCUI_API float Widget_GetBoxShadowOffsetX( LCUI_Widget w );

/** 根据阴影参数获取部件区域的纵向偏移距离 */
LCUI_API float Widget_GetBoxShadowOffsetY( LCUI_Widget w );

/** 获取部件在添加阴影后的高度 */
LCUI_API float Widget_GetCanvasWidth( LCUI_Widget w );

/** 获取部件在添加阴影后的宽度 */
LCUI_API float Widget_GetCanvasWidth( LCUI_Widget w );

/** 更新部件矩形阴影样式 */
LCUI_API void Widget_UpdateBoxShadow( LCUI_Widget w );

/** 计算部件阴影样式的实际值 */
LCUI_API void Widget_ComputeBoxShadow( LCUI_Widget w, LCUI_BoxShadow *out );

/** 绘制部件阴影 */
LCUI_API void Widget_PaintBoxShadow( LCUI_Widget w, LCUI_PaintContext paint,
				     LCUI_WidgetActualStyle style );

/** 更新可见性 */
LCUI_API void Widget_UpdateVisibility( LCUI_Widget w );

/** 更新显示方式 */
LCUI_API void Widget_UpdateDisplay( LCUI_Widget w );

/** 设置部件为顶级部件 */
LCUI_API int Widget_Top( LCUI_Widget w );

/** 刷新堆叠顺序 */
LCUI_API void Widget_UpdateZIndex( LCUI_Widget w );

LCUI_API void Widget_ExecUpdateZIndex( LCUI_Widget w );

/** 刷新位置 */
LCUI_API void Widget_UpdatePosition( LCUI_Widget w );

/** 刷新外间距 */
LCUI_API void Widget_UpdateMargin( LCUI_Widget w );

/** 刷新尺寸 */
LCUI_API void Widget_UpdateSize( LCUI_Widget w );

/** 刷新部件尺寸以及与之绑定的表面的尺寸 */
LCUI_API void Widget_UpdateSizeWithSurface( LCUI_Widget w );

/** 刷新各项属性 */
LCUI_API void Widget_UpdateProps( LCUI_Widget w );

/** 更新透明度 */
LCUI_API void Widget_UpdateOpacity( LCUI_Widget w );

/** 设置部件标题 */
LCUI_API void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title );

/** 设置部件ID */
LCUI_API int Widget_SetId( LCUI_Widget w, const char *idstr );

/** 为部件添加状态 */
LCUI_API void Widget_AddState( LCUI_Widget w, LCUI_WidgetState state );

/** 为部件设置属性 */
LCUI_API int Widget_SetAttributeEx( LCUI_Widget w, const char *name, void *value,
				    int value_type, void( *value_destructor )(void*) );

/** 为部件设置属性（字符串版） */
LCUI_API int Widget_SetAttribute( LCUI_Widget w, const char *name, const char *value );

/** 获取部件属性 */
LCUI_API const char *Widget_GetAttribute( LCUI_Widget w, const char *name );

/** 判断部件类型 */
LCUI_API LCUI_BOOL Widget_CheckType( LCUI_Widget w, const char *type );

/** 判断部件原型 */
LCUI_API LCUI_BOOL Widget_CheckPrototype( LCUI_Widget w, LCUI_WidgetPrototypeC proto );

/** 为部件添加一个类 */
LCUI_API int Widget_AddClass( LCUI_Widget w, const char *class_name );

/** 判断部件是否包含指定的类 */
LCUI_API LCUI_BOOL Widget_HasClass( LCUI_Widget w, const char *class_name );

/** 从部件中移除一个类 */
LCUI_API int Widget_RemoveClass( LCUI_Widget w, const char *class_name );

/** 为部件添加一个状态 */
LCUI_API int Widget_AddStatus( LCUI_Widget w, const char *status_name );

/** 判断部件是否包含指定的状态 */
LCUI_API LCUI_BOOL Widget_HasStatus( LCUI_Widget w, const char *status_name );

/** 设置部件是否禁用 */
LCUI_API void Widget_SetDisabled( LCUI_Widget w, LCUI_BOOL disabled );

/** 计算部件的最大宽度 */
LCUI_API float Widget_ComputeMaxWidth( LCUI_Widget w );

/** 计算部件可利用的宽度 */
LCUI_API float Widget_ComputeFillAvailableWidth( LCUI_Widget w );

/** 计算部件的最大内容宽度 */
LCUI_API float Widget_ComputeMaxContentWidth( LCUI_Widget w );

/** 计算部件的最大可用宽度 */
LCUI_API float Widget_ComputeMaxAvaliableWidth( LCUI_Widget widget );

LCUI_API void Widget_ComputeLimitSize( LCUI_Widget w );

/** 从部件中移除一个状态 */
LCUI_API int Widget_RemoveStatus( LCUI_Widget w, const char *status_name );

/** 打印部件树 */
LCUI_API void Widget_PrintTree( LCUI_Widget w );

LCUI_API void LCUIWidget_InitBase( void );

LCUI_API void LCUIWidget_FreeRoot( void );

LCUI_API void LCUIWidget_FreeBase( void );

LCUI_END_HEADER

#endif
