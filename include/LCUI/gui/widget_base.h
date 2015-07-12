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
	int position;			/**< 定位方式 */
	int left, top;			/**< 左边界、顶边界的偏移距离 */
	int right, bottom;		/**< 右边界、底边界的偏移距离 */
	int z_index;			/**< 堆叠顺序，该值越高，部件显示得越靠前 */
	float opacity;			/**< 不透明度，有效范围从 0.0 （完全透明）到 1.0（完全不透明） */
	LCUI_StyleVar x, y;		/**< 当前平面坐标 */
	int box_sizing;			/**< 以何种方式计算宽度和高度 */

	union {
		LCUI_StyleVar w, width;		/**< 部件区域宽度 */
	};
	union {
		LCUI_StyleVar h, height;	/**< 部件区域高度 */
	};

	struct {
		LCUI_StyleVar top, right, bottom, left;
	} margin, padding;		/**< 外边距, 内边距 */

	LCUI_Background background;	/**< 背景 */
	LCUI_BoxShadow shadow;		/**< 阴影 */
	LCUI_Border border;		/**< 边框 */
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
	key_visible,
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
	STYLE_KEY_TOTAL
};

typedef struct LCUI_Style {
	LCUI_BOOL is_valid;
	LCUI_BOOL is_changed;
	LCUI_StyleVarType type;
	union {
		int value;
		int value_px;
		int value_pt;
		int value_style;
		float value_scale;
		float scale;
		LCUI_BOOL value_boolean;
		LCUI_Color value_color;
		LCUI_Color color;
		LCUI_Graph *value_image;
		LCUI_Graph *image;
	};
} LCUI_Style;

typedef LCUI_Style *LCUI_StyleSheet;

typedef struct LCUI_SelectorNodeRec_ {
	char *id;
	char *type;
	char *class_name;
	char *pseudo_class_name;
} *LCUI_SelectorNode, **LCUI_Selector;

#define SetStyle(S, NAME, VAL, TYPE)	S[NAME].is_valid = TRUE, \
					S[NAME].is_changed = TRUE, \
					S[NAME].type = SVT_##TYPE, \
					S[NAME].value_##TYPE = VAL

typedef struct LCUI_WidgetBase {
	int x, y;			/**< 部件当前坐标 */
	int width, height;		/**< 部件区域大小，包括边框和内边距占用区域 */
	struct LCUI_WidgetBoxRect {
		LCUI_Rect content;	/**< 内容框的区域 */
		LCUI_Rect border;	/**< 边框盒的区域，包括内边距框和内容框区域 */
		LCUI_Rect outer;	/**< 外边框的区域，包括边框盒和外边距框区域 */
		LCUI_Rect graph;	/**< 图层的区域，包括边框盒和阴影区域 */
	} box;				/**< 部件的各个区域信息 */
	LCUI_Rect2 padding;		/**< 内边距框 */
	LCUI_Rect2 margin;		/**< 外边距框 */
	LCUI_StyleSheet style;		/**< 自定义样式表 */
	LCUI_StyleSheet css;		/**< 应用在部件上的完整样式表 */
} LCUI_WidgetBase;

/** 为函数加前缀名 */
#define $(FUNC_NAME) Widget_##FUNC_NAME

#define Widget_NewPrivateData(w, type) (type*)(w->private_data = malloc(sizeof(type)))

/** 一般来说部件占用的矩形区域看上去是指边框盒区域 */
#define Widget_GetRect Widget_GetBorderRect

extern LCUI_Widget LCUIRootWidget;

LCUI_API LCUI_Widget LCUIWidget_GetRoot(void);

/** 新建一个GUI部件 */
LCUI_API LCUI_Widget LCUIWidget_New( const char *type_name );

/** 追加子部件 */
LCUI_API int Widget_Append( LCUI_Widget container, LCUI_Widget widget );

/** 获取当前点命中的最上层可见部件 */
LCUI_API LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y );

/** 设置部件为顶级部件 */
LCUI_API int Widget_Top( LCUI_Widget w );

/** 更新位图尺寸 */
void Widget_UpdateGraphBox( LCUI_Widget w );

/** 计算位置 */
void Widget_ComputePosition( LCUI_Widget w );

/** 计算尺寸 */
void Widget_ComputeSize( LCUI_Widget w );

/** 计算部件通过继承得到的样式表 */
LCUI_API int Widget_ComputeInheritStyle( LCUI_Widget w, LCUI_StyleSheet out_ss );

/** 设置部件标题 */
LCUI_API void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title );

/** 移动部件位置 */
LCUI_API void Widget_Move( LCUI_Widget w, int top, int left );

LCUI_API void Widget_Resize( LCUI_Widget w, int width, int height );

LCUI_API void Widget_Show( LCUI_Widget w );

LCUI_API void Widget_Hide( LCUI_Widget w );

LCUI_API void Widget_Lock( LCUI_Widget w );

LCUI_API void Widget_Unlock( LCUI_Widget w );

void LCUI_InitWidget(void);

void LCUI_ExitWidget(void);

#undef $

#endif

