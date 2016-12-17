/* ***************************************************************************
 * widget_base.h -- the widget base operation set.
 *
 * Copyright (C) 2012-2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * widget_base.h -- 部件的基本操作集。
 *
 * 版权所有 (C) 2012-2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#ifndef LCUI_WIDGET_BASE_H
#define LCUI_WIDGET_BASE_H

#include <LCUI/gui/css_library.h>
#include <LCUI/gui/css_parser.h>

LCUI_BEGIN_HEADER

/** 部件样式 */
typedef struct LCUI_WidgetStyle {
	LCUI_BOOL visible;		/**< 是否可见 */
	LCUI_BOOL focusable;		/**< 是否能够得到焦点 */
	int left, top;			/**< 左边界、顶边界的偏移距离 */
	int right, bottom;		/**< 右边界、底边界的偏移距离 */
	int z_index;			/**< 堆叠顺序，该值越高，部件显示得越靠前 */
	float opacity;			/**< 不透明度，有效范围从 0.0 （完全透明）到 1.0（完全不透明） */
	LCUI_StyleValue position;	/**< 定位方式 */
	LCUI_StyleValue display;	/**< 显示方式，决定以何种布局显示该部件 */
	LCUI_StyleValue box_sizing;	/**< 以何种方式计算宽度和高度 */
	LCUI_StyleValue vertical_align;	/**< 垂直对齐方式 */
	LCUI_BoundBox margin;		/**< 外边距 */
	LCUI_BoundBox padding;		/**< 内边距 */
	LCUI_Background background;	/**< 背景 */
	LCUI_BoxShadow shadow;		/**< 阴影 */
	LCUI_Border border;		/**< 边框 */
	int pointer_events;		/**< 事件的处理方式 */
} LCUI_WidgetStyle;

/** 部件任务类型，按照任务的依赖顺序排列 */
enum WidgetTaskType {
	WTT_REFRESH_STYLE,	/**< 刷新部件全部样式 */
	WTT_UPDATE_STYLE,	/**< 更新部件自定义样式 */
	WTT_TITLE,
	WTT_PROPS,		/**< 更新一些属性 */
	WTT_BOX_SIZING,
	WTT_PADDING,
	WTT_MARGIN,
	WTT_VISIBLE,
	WTT_SHADOW,
	WTT_BORDER,
	WTT_BACKGROUND,
	WTT_LAYOUT,
	WTT_RESIZE,
	WTT_POSITION,
	WTT_ZINDEX,
	WTT_OPACITY,
	WTT_BODY,
	WTT_REFRESH,
	WTT_USER,
	WTT_TOTAL_NUM
};

typedef struct LCUI_WidgetBoxRect {
	LCUI_Rect content;	/**< 内容框的区域 */
	LCUI_Rect padding;	/**< 内边距框的区域 */
	LCUI_Rect border;	/**< 边框盒的区域，包括内边距框和内容框区域 */
	LCUI_Rect outer;	/**< 外边距框的区域，包括边框盒和外边距框区域 */
	LCUI_Rect graph;	/**< 图层的区域，包括边框盒和阴影区域 */
} LCUI_WidgetBoxRect;

typedef struct LCUI_WidgetTaskBoxRec_ {
	LCUI_BOOL for_self;			/**< 标志，指示当前部件是否有待处理的任务 */
	LCUI_BOOL for_children;			/**< 标志，指示是否有待处理的子级部件 */
	LCUI_BOOL buffer[WTT_TOTAL_NUM];	/**< 记录缓存 */
} LCUI_WidgetTaskBoxRec;

/** 部件状态 */
enum LCUI_WidgetState {
	WSTATE_CREATED = 0,
	WSTATE_UPDATED,
	WSTATE_LAYOUTED,
	WSTATE_READY,
	WSTATE_NORMAL,
	WSTATE_DELETED,
};

typedef struct LCUI_WidgetRec_* LCUI_Widget;
typedef struct LCUI_WidgetPrototypeRec_ *LCUI_WidgetPrototype;
typedef const struct LCUI_WidgetPrototypeRec_ *LCUI_WidgetPrototypeC;

typedef void( *LCUI_WidgetFunction )(LCUI_Widget);
typedef void( *LCUI_WidgetResizer )(LCUI_Widget, int*, int*);
typedef void( *LCUI_WidgetAttrSetter )(LCUI_Widget, const char*, const char*);
typedef void( *LCUI_WidgetTextSetter )(LCUI_Widget, const char*);
typedef void( *LCUI_WidgetPainter )(LCUI_Widget, LCUI_PaintContext);

/** 部件原型数据结构 */
typedef struct LCUI_WidgetPrototypeRec_ {
	char *name;				/**< 名称 */
	LCUI_WidgetFunction init;		/**< 构造函数  */
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
	int			state;			/**< 状态 */
	int			x, y;			/**< 当前坐标（由 origin 计算而来） */
	int			origin_x, origin_y;	/**< 当前布局下计算出的坐标 */
	int			width, height;		/**< 部件区域大小，包括边框和内边距占用区域 */
	int			index;			/**< 部件索引位置 */
	char			*id;			/**< ID */
	char			*type;			/**< 类型 */
	char			**classes;		/**< 类列表 */
	char			**status;		/**< 状态列表 */
	wchar_t			*title;			/**< 标题 */
	LCUI_Rect2		padding;		/**< 内边距框 */
	LCUI_Rect2		margin;			/**< 外边距框 */
	LCUI_WidgetBoxRect	box;			/**< 部件的各个区域信息 */
	LCUI_StyleSheet		style;			/**< 当前完整样式表 */
	LCUI_StyleSheet		custom_style;		/**< 自定义样式表 */
	LCUI_StyleSheet		inherited_style;	/**< 通过继承得到的样式表 */
	LCUI_WidgetStyle	computed_style;		/**< 已经计算的样式数据 */
	LCUI_Widget		parent;			/**< 父部件 */
	LinkedList		children;		/**< 子部件 */
	LinkedList		children_show;		/**< 子部件的堆叠顺序记录，由顶到底 */
	LCUI_WidgetData		data;			/**< 私有数据 */
	Dict			*attributes;
	LCUI_WidgetPrototypeC	proto;			/**< 原型 */
	LCUI_BOOL		enable_graph;		/**< 是否启用位图缓存 */
	LCUI_Graph		graph;			/**< 位图缓存 */
	LCUI_Mutex		mutex;			/**< 互斥锁 */
	LCUI_EventTrigger	trigger;		/**< 事件触发器 */
	LCUI_WidgetTaskBoxRec	task;			/**< 任务记录 */
	LinkedList		dirty_rects;		/**< 记录无效区域（脏矩形） */
	LCUI_BOOL		has_dirty_child;	/**< 子级部件是否有无效区域 */
	LCUI_BOOL		layout_locked;		/**< 子级部件布局是否已锁定 */
	LCUI_BOOL		event_blocked;		/**< 是否阻止自己和子级部件的事件处理 */
	LCUI_BOOL		disabled;		/**< 是否禁用 */
} LCUI_WidgetRec;

#define Widget_GetNode(w) (LinkedListNode*)(((char*)w) + sizeof(LCUI_WidgetRec))
#define Widget_GetShowNode(w) (LinkedListNode*)(((char*)w) + sizeof(LCUI_WidgetRec) + sizeof(LinkedListNode))
#define Widget_NewPrivateData(w, type) (type*)(w->private_data = malloc(sizeof(type)))
#define Widget_SetStyle(W, K, V, T) SetStyle((W)->custom_style, K, V, T)
#define Widget_UnsetStyle(W, K) UnsetStyle((W)->custom_style, K)

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

/** 获取当前点命中的最上层可见部件 */
LCUI_API LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y );

/** 获取相对于父级指定部件的 XY 坐标 */
LCUI_API void Widget_GetAbsXY( LCUI_Widget w, LCUI_Widget parent, int *x, int *y );

/** 更新部件背景样式 */
LCUI_API void Widget_UpdateBackground( LCUI_Widget widget );

/** 刷新部件的边框 */
LCUI_API void Widget_UpdateBorder( LCUI_Widget w );

/** 刷新部件的矩形阴影 */
LCUI_API void Widget_UpdateBoxShadow( LCUI_Widget w );

/** 刷新可见性 */
LCUI_API void Widget_UpdateVisibility( LCUI_Widget w );

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

/** 刷新各项属性 */
LCUI_API void Widget_UpdateProps( LCUI_Widget w );

/** 更新透明度 */
LCUI_API void Widget_UpdateOpacity( LCUI_Widget w );

/** 设置部件标题 */
LCUI_API void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title );

/** 设置部件ID */
LCUI_API int Widget_SetId( LCUI_Widget w, const char *idstr );

/** 设置边框 */
LCUI_API void Widget_SetBorder( LCUI_Widget w, int width, int style, LCUI_Color clr );

/** 设置内边距 */
LCUI_API void Widget_SetPadding( LCUI_Widget w, int top, int right, int bottom, int left );

/** 设置外边距 */
LCUI_API void Widget_SetMargin( LCUI_Widget w, int top, int right, int bottom, int left );

/** 移动部件位置 */
LCUI_API void Widget_Move( LCUI_Widget w, int left, int top );

/** 调整部件尺寸 */
LCUI_API void Widget_Resize( LCUI_Widget w, int width, int height );

LCUI_API void Widget_Show( LCUI_Widget w );

LCUI_API void Widget_Hide( LCUI_Widget w );

LCUI_API void Widget_Lock( LCUI_Widget w );

LCUI_API void Widget_Unlock( LCUI_Widget w );

/** 为部件设置属性 */
LCUI_API int Widget_SetAttributeEx( LCUI_Widget w, const char *name, void *value,
				    int value_type, void( *value_destructor )(void*) );

/** 为部件设置属性（字符串版） */
LCUI_API int Widget_SetAttribute( LCUI_Widget w, const char *name, const char *value );

/** 获取部件属性 */
LCUI_API const char *Widget_GetAttribute( LCUI_Widget w, const char *name );

/** 判断部件类型 */
LCUI_API LCUI_BOOL Widget_CheckType( LCUI_Widget w, const char *type );

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
LCUI_API int Widget_ComputeMaxWidth( LCUI_Widget w );

/** 锁定子部件的布局，让 LCUI 不自动更新布局 */
LCUI_API void Widget_LockLayout( LCUI_Widget w );

/** 解除锁定子部件的布局 */
LCUI_API void Widget_UnlockLayout( LCUI_Widget w );

LCUI_API void Widget_UpdateLayout( LCUI_Widget w );

/** 更新子部件的布局 */
LCUI_API void Widget_UpdateLayout( LCUI_Widget w );

LCUI_API void Widget_ExecUpdateLayout( LCUI_Widget w );

/** 从部件中移除一个状态 */
int Widget_RemoveStatus( LCUI_Widget w, const char *status_name );

/** 打印部件树 */
LCUI_API void Widget_PrintTree( LCUI_Widget w );

void LCUI_InitWidget( void );

void LCUI_ExitWidget( void );

LCUI_END_HEADER

#endif
