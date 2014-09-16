/* ***************************************************************************
 * widget_base.h -- the widget base operation set.
 *
 * Copyright (C) 2012-2014 by
 * Liu Chao
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
 * 版权所有 (C) 2012-2014 归属于
 * 刘超
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

/** 边框风格 */
enum BorderStyle {
	BORDER_STYLE_NONE,	/**< 无边框 */
	BORDER_STYLE_SOLID,	/**< 实线 */
	BORDER_STYLE_DOTTED,	/**< 点状 */
	BORDER_STYLE_DOUBLE,	/**< 双线 */
	BORDER_STYLE_DASHED,	/**< 虚线 */
};

/** 部件停靠类型 */
enum DockType {
	DOCK_TYPE_NONE,
	DOCK_TYPE_TOP,
	DOCK_TYPE_LEFT,
	DOCK_TYPE_RIGHT,
	DOCK_TYPE_FILL,
	DOCK_TYPE_BOTTOM
};

/** 部件结构（仅包含样式），大部分是只读的，对其修改不会影响部件效果 */
typedef struct LCUI_WidgetLite {
	LCUI_BOOL visible;		/**< 是否可见 */
	int position;			/**< 定位方式 */
	int dock;			/**< 停靠位置 */
	StyleVar x, y;			/**< 当前坐标 */
	StyleVar offsetX, offsetY;	/**< 水平、垂直坐标偏移量 */
	union {
		StyleVar w, width;	/**< 部件区域宽度 */
	};
	union {
		StyleVar h, height;	/**< 部件区域高度 */
	};
	int inner_width, inner_height;	/**< 部件内部区域大小，除去内边距占用区域 */
	int outer_width, outer_height;	/**< 部件外部区域大小，包括边框和阴影占用区域 */

	struct {
		int top, right, bottom, left;
	} margin, padding;		/**< 外边距（暂不支持）, 内边距 */

	struct {
		LCUI_Graph image;	/**< 背景图 */
		LCUI_Color color;	/**< 背景色 */
	
		struct {
			LCUI_BOOL x, y;
		} repeat;		/**< 背景图是否重复 */

		int clip;		/**< 背景图的裁剪方式 */

		struct {
			StyleVar x, y;
		} position;		/**< 定位方式 */
		struct {
			StyleVar w, h;
		} size;
		int origin;		/**< 相对于何种位置进行定位 */
	} background;

	struct {
		int x, y;		/**< 位置 */
		int blur;		/**< 模糊距离 */
		int spread;		/**< 扩散大小 */
		LCUI_Color color;	/**< 颜色 */
	} shadow;			/**< 阴影 */

	struct {
		struct {
			int width;
			int style;
			LCUI_Color color;
		} top, right, bottom, left;
		unsigned int top_left_radius;
		unsigned int top_right_radius;
		unsigned int bottom_left_radius;
		unsigned int bottom_right_radius;
	} border;			/**< 边框 */

	float opacity;			/**< 不透明度，有效范围从 0.0 （完全透明）到 1.0（完全不透明） */
	struct {
		struct {
			float x, y;
		} scale;		/**< 2D 缩放 */
		float rotate;		/**< 2D 旋转角度 */
	} transform;

} LCUI_WidgetLite;

/** 如果没有包含 widget_build.h 头文件，则使用只包含样式的 LCUI_Widget */
#ifndef __LCUI_WIDGET_BUILD_H__
typedef LCUI_WidgetLite LCUI_Widget;
#endif

#endif
