/* ***************************************************************************
 * widget_base.h -- the widget base operation set.
 *
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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

#ifndef __LCUI_WIDGET_BASE_H__
#define __LCUI_WIDGET_BASE_H__

LCUI_BEGIN_HEADER

/** 如果没有包含 widget_build.h 头文件 */
#ifndef __LCUI_WIDGET_BUILD_H__
typedef struct LCUI_WidgetBase* LCUI_Widget;
#endif

/** 部件样式 */
typedef struct LCUI_WidgetStyle {
	LCUI_BOOL visible;		/**< 是否可见 */
	int left, top;			/**< 左边界、顶边界的偏移距离 */
	int right, bottom;		/**< 右边界、底边界的偏移距离 */
	int z_index;			/**< 堆叠顺序，该值越高，部件显示得越靠前 */
	float opacity;			/**< 不透明度，有效范围从 0.0 （完全透明）到 1.0（完全不透明） */
	LCUI_StyleValue position;	/**< 定位方式 */
	LCUI_StyleValue float_mode;	/**< 浮动模式 */
	LCUI_StyleValue display;	/**< 显示方式，决定以何种布局显示该部件 */
	LCUI_StyleValue box_sizing;	/**< 以何种方式计算宽度和高度 */
	union {
		LCUI_Style w, width;	/**< 部件区域宽度 */
	};
	union {
		LCUI_Style h, height;	/**< 部件区域高度 */
	};
	struct {
		LCUI_Style top, right, bottom, left;
	} margin, padding;		/**< 外边距, 内边距 */
	LCUI_Background background;	/**< 背景 */
	LCUI_BoxShadow shadow;		/**< 阴影 */
	LCUI_Border border;		/**< 边框 */
	int pointer_events;		/**< 事件的处理方式 */
} LCUI_WidgetStyle;

/** 样式属性名 */
enum LCUI_StyleKeyName {
	key_position_start,
	key_left,
	key_right,
	key_top,
	key_bottom,
	key_position,
	key_position_end,
	key_display_start,
	key_visible,
	key_display,
	key_display_end,
	key_z_index,
	key_opacity,
	key_box_sizing,
	key_width,
	key_height,
	key_margin_top,
	key_margin_right,
	key_margin_bottom,
	key_margin_left,
	key_padding_top,
	key_padding_right,
	key_padding_bottom,
	key_padding_left,
	key_border_start,
	key_border_color,
	key_border_style,
	key_border_width,
	key_border_top_width,
	key_border_top_style,
	key_border_top_color,
	key_border_right_width,
	key_border_right_style,
	key_border_right_color,
	key_border_bottom_width,
	key_border_bottom_style,
	key_border_bottom_color,
	key_border_left_width,
	key_border_left_style,
	key_border_left_color,
	key_border_top_left_radius,
	key_border_top_right_radius,
	key_border_bottom_left_radius,
	key_border_bottom_right_radius,
	key_border_end,
	key_background_start,
	key_background_color,
	key_background_image,
	key_background_size,
	key_background_size_width,
	key_background_size_height,
	key_background_repeat,
	key_background_repeat_x,
	key_background_repeat_y,
	key_background_position,
	key_background_position_x,
	key_background_position_y,
	key_background_origin,
	key_background_end,
	key_box_shadow_start,
	key_box_shadow_x,
	key_box_shadow_y,
	key_box_shadow_spread,
	key_box_shadow_blur,
	key_box_shadow_color,
	key_box_shadow_end,
	key_pointer_events,
	STYLE_KEY_TOTAL
};

typedef struct LCUI_StyleSheetRec_ {
	LCUI_Style *sheet;
	int length;
} LCUI_StyleSheetRec, *LCUI_StyleSheet;

typedef struct LCUI_SelectorNodeRec_ {
	char *id;
	char *type;
	char *class_name;
	char *pseudo_class_name;
} LCUI_SelectorNodeRec, *LCUI_SelectorNode;

typedef struct LCUI_SelectorRec_ {
	LCUI_SelectorNode *list;	/**< 选择器结点列表 */
	int length;			/**< 选择器结点长度 */
	int rank;			/**< 权值，决定优先级 */
	int batch_num;			/**< 批次号 */
} LCUI_SelectorRec, *LCUI_Selector;

#define SetStyle(S, NAME, VAL, TYPE)	S->sheet[NAME].is_valid = TRUE, \
					S->sheet[NAME].is_changed = TRUE, \
					S->sheet[NAME].type = SVT_##TYPE, \
					S->sheet[NAME].val_##TYPE = VAL

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
	WTT_OPACITY,
	WTT_BODY,
	WTT_REFRESH,
	WTT_CACHE_STYLE,	/**< 缓存当前样式 */
	WTT_USER,
	WTT_DESTROY
};

#define WTT_TOTAL_NUM (WTT_DESTROY+1)

#ifndef __IN_WIDGET_TASK_SOURCE_FILE__
typedef void* LCUI_WidgetTaskBox;
#else
typedef struct LCUI_WidgetTaskBoxRec_* LCUI_WidgetTaskBox;
#endif

typedef struct LCUI_WidgetBoxRect {
	LCUI_Rect content;	/**< 内容框的区域 */
	LCUI_Rect border;	/**< 边框盒的区域，包括内边距框和内容框区域 */
	LCUI_Rect outer;	/**< 外边框的区域，包括边框盒和外边距框区域 */
	LCUI_Rect graph;	/**< 图层的区域，包括边框盒和阴影区域 */
} LCUI_WidgetBoxRect;

typedef struct LCUI_WidgetRec_* LCUI_Widget;

/** 部件结构 */
typedef struct LCUI_WidgetRec_ {
	int			x, y;			/**< 部件当前坐标 */
	int			width, height;		/**< 部件区域大小，包括边框和内边距占用区域 */
	char			*id;			/**< ID */
	char			*type;			/**< 类型 */
	char			**classes;		/**< 类列表 */
	char			**status;		/**< 伪类列表 */
	wchar_t			*title;			/**< 标题 */
	LCUI_Rect2		padding;		/**< 内边距框 */
	LCUI_Rect2		margin;			/**< 外边距框 */
	LCUI_WidgetBoxRect	box;			/**< 部件的各个区域信息 */
	LCUI_StyleSheet		style;			/**< 当前完整样式表 */
	LCUI_StyleSheet		cached_style;		/**< 已缓存的完整样式表 */
	LCUI_StyleSheet		inherited_style;	/**< 通过继承得到的样式表 */
	LCUI_StyleSheet		custom_style;		/**< 自定义样式表 */
	LCUI_WidgetStyle	computed_style;		/**< 已经计算的样式数据 */
	LCUI_Widget		parent;			/**< 父部件 */
	LinkedList		children;		/**< 子部件 */
	LinkedList		children_show;		/**< 子部件的堆叠顺序记录，由顶到底 */

	void			*private_data;		/**< 私有数据 */
	void			*extend_data;		/**< 扩展数据 */

	LCUI_Graph		graph;			/**< 位图缓存 */
	LCUI_Mutex		mutex;			/**< 互斥锁 */
	LCUI_EventBox		event;			/**< 事件记录 */
	LCUI_WidgetTaskBox	task;			/**< 任务记录 */
	LinkedList		dirty_rects;		/**< 记录无效区域（脏矩形） */
	LCUI_BOOL		has_dirty_child;	/**< 标志，指示子级部件是否有无效区域 */
} LCUI_WidgetRec;


#define Widget_GetNode(w) (LinkedListNode*)(((char*)w) + sizeof(LCUI_WidgetRec))
#define Widget_GetShowNode(w) (LinkedListNode*)(((char*)w) + sizeof(LCUI_WidgetRec) + sizeof(LinkedListNode))
#define Widget_NewPrivateData(w, type) (type*)(w->private_data = malloc(sizeof(type)))

/** 获取根级部件 */
LCUI_API LCUI_Widget LCUIWidget_GetRoot(void);

/** 获取指定ID的部件 */
LCUI_API LCUI_Widget LCUIWidget_GetById( const char *idstr );

/** 新建一个GUI部件 */
LCUI_API LCUI_Widget LCUIWidget_New( const char *type_name );

/** 直接销毁部件 */
LCUI_API void Widget_ExecDestroy( LCUI_Widget *w );

/** 销毁部件 */
LCUI_API void Widget_Destroy( LCUI_Widget *w );

/** 追加子部件 */
LCUI_API int Widget_Append( LCUI_Widget container, LCUI_Widget widget );

/** 移除部件，并将其子级部件转移至父部件内 */
LCUI_API int Widget_Unwrap( LCUI_Widget *widget );

/** 获取当前点命中的最上层可见部件 */
LCUI_API LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y );

/** 刷新部件的边框 */
LCUI_API void Widget_FlushBorder( LCUI_Widget w );

/** 刷新部件的矩形阴影 */
LCUI_API void Widget_FlushBoxShadow( LCUI_Widget w );
/** 刷新可见性 */
LCUI_API void Widget_FlushVisibility( LCUI_Widget w );

/** 设置部件为顶级部件 */
LCUI_API int Widget_Top( LCUI_Widget w );

/** 获取在部件列表中的位置 */
LCUI_API int Widget_GetIndex( LCUI_Widget w );

/** 刷新堆叠顺序 */
LCUI_API void Widget_FlushZIndex( LCUI_Widget w );

/** 刷新位置 */
LCUI_API void Widget_FlushPosition( LCUI_Widget w );

/** 刷新尺寸 */
LCUI_API void Widget_FlushSize( LCUI_Widget w );

/** 刷新各项属性 */
LCUI_API void Widget_FlushProps( LCUI_Widget w );

/** 处理部件中当前积累的任务 */
LCUI_API int Widget_Flush( LCUI_Widget w );

/** 计算部件通过继承得到的样式表 */
LCUI_API int Widget_ComputeInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss );

/** 更新当前部件的样式 */
LCUI_API void Widget_UpdateStyle( LCUI_Widget w, LCUI_BOOL is_update_all );

/** 直接更新当前部件的样式 */
LCUI_API void Widget_FlushStyle( LCUI_Widget w, LCUI_BOOL is_update_all );

/** 设置部件标题 */
LCUI_API void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title );

/** 设置部件ID */
LCUI_API int Widget_SetId( LCUI_Widget w, const char *idstr );

/** 设置内边距 */
LCUI_API void Widget_SetPadding( LCUI_Widget w, int top, int right, int bottom, int left );

/** 设置外边距 */
LCUI_API void Widget_SetMargin( LCUI_Widget w, int top, int right, int bottom, int left );

/** 移动部件位置 */
LCUI_API void Widget_Move( LCUI_Widget w, int top, int left );

/** 调整部件尺寸 */
LCUI_API void Widget_Resize( LCUI_Widget w, int width, int height );

LCUI_API void Widget_Show( LCUI_Widget w );

LCUI_API void Widget_Hide( LCUI_Widget w );

LCUI_API void Widget_Lock( LCUI_Widget w );

LCUI_API void Widget_Unlock( LCUI_Widget w );

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

/** 更新子部件的布局 */
LCUI_API void Widget_UpdateLayout( LCUI_Widget w );

/** 从部件中移除一个状态 */
int Widget_RemoveStatus( LCUI_Widget w, const char *status_name );

/** 打印部件树 */
LCUI_API void Widget_PrintTree( LCUI_Widget w );

void LCUI_InitWidget(void);

void LCUI_ExitWidget(void);

/** 计算背景样式 */
void Widget_ComputeBackgroundStyle( LCUI_Widget widget );

#endif

