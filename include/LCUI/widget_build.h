/* ***************************************************************************
 * widget_build.h -- system APIs for GUI widget.
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
 * widget_build.h -- GUI部件的系统操作函数，主要用于widget开发。
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

#ifndef __LCUI_WIDGET_BUILD_H__
#define __LCUI_WIDGET_BUILD_H__

#include <LCUI/graph.h>

LCUI_BEGIN_HEADER

/** 部件样式 */
typedef struct LCUI_WidgetStyle {
	LCUI_BOOL visible;		/**< 是否可见 */
	int position;			/**< 定位方式 */
	int box_sizing;			/**< 以何种方式计算宽度和高度 */
	int left, top;			/**< 水平、垂直坐标偏移量 */
	int z_index;			/**< 堆叠顺序，该值越高，部件显示得越靠前 */
	float opacity;			/**< 不透明度，有效范围从 0.0 （完全透明）到 1.0（完全不透明） */
	LCUI_StyleVar x, y;	/**< 当前平面坐标 */

	union {
		LCUI_StyleVar w, width;	/**< 部件区域宽度 */
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

	struct {
		struct {
			float x, y;
		} scale;		/**< 2D 缩放 */
		float rotate;		/**< 2D 旋转角度 */
	} transform;

} LCUI_WidgetStyle;

typedef struct LCUI_WidgetFull* LCUI_Widget;

#include <LCUI/gui/widget_base.h>
#include <LCUI/gui/widget_task.h>

/** 部件结构（完整版） */
struct LCUI_WidgetFull {
	LCUI_WidgetBase		base;		/**< 基础数据及操作集 */
	LCUI_WidgetStyle	style;		/**< 样式 */
	char			*type_name;	/**< 类型名称 */
	LCUI_BOOL		autosize;	/**< 指定是否自动调整自身的大小，以适应内容的大小 */
	LCUI_BOOL		focus;		/**< 指定该部件是否需要焦点 */
	LCUI_Widget		focus_widget;	/**< 获得焦点的子部件 */

	LCUI_Widget		parent;		/**< 父部件 */
	LinkedList		children;	/**< 子部件 */
	LinkedList		children_show;	/**< 子部件的堆叠顺序记录，由顶到底 */
	LCUI_EventBox		event;		/**< 事件记录 */
	LCUI_WidgetTaskBox	task;		/**< 任务记录 */
	LCUI_DirtyRectList	dirty_rects;	/**< 记录无效区域（脏矩形） */
	LCUI_GraphLayer		glayer;		/**< 图层 */
	void			*private_data;	/**< 私有数据 */
};

LCUI_END_HEADER

#include <LCUI/gui/widget_library.h>
#include <LCUI/gui/widget_event.h>
#include <LCUI/gui/widget_paint.h>

#endif
