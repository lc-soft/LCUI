/* ***************************************************************************
 * widget_build.h -- system APIs for GUI widget.
 *
 * Copyright (C) 2013-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2015 归属于 刘超 <lc-soft@live.cn>
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

typedef struct LCUI_WidgetFull* LCUI_Widget;

#include <LCUI/graph.h>
#include <LCUI/thread.h>
#include <LCUI/gui/widget_base.h>
#include <LCUI/gui/widget_task.h>
#include <LCUI/gui/widget_paint.h>
#include <LCUI/gui/widget_library.h>
#include <LCUI/gui/widget_event.h>
#include <LCUI/gui/widget_style.h>

LCUI_BEGIN_HEADER

/** 部件结构（完整版） */
struct LCUI_WidgetFull {
	LCUI_WidgetBase		base;			/**< 基础数据 */
	LCUI_WidgetStyle	style;			/**< 当前缓存的样式数据 */
	LCUI_StyleSheet		css;			/**< 当前缓存的样式表 */
	char			*id;			/**< ID */
	char			*type;			/**< 类型 */
	char			**classes;		/**< 类列表 */
	char			**pseudo_classes;	/**< 伪类列表 */
	wchar_t			*title;			/**< 标题 */

	LCUI_Widget		parent;			/**< 父部件 */
	LinkedList		children;		/**< 子部件 */
	LinkedList		children_show;		/**< 子部件的堆叠顺序记录，由顶到底 */
	LCUI_EventBox		event;			/**< 事件记录 */
	LCUI_WidgetTaskBox	task;			/**< 任务记录 */
	LCUI_DirtyRectList	dirty_rects;		/**< 记录无效区域（脏矩形） */
	LCUI_BOOL		has_dirty_child;	/**< 标志，指示子级部件是否有无效区域 */
	LCUI_Graph		graph;			/**< 位图缓存 */
	LCUI_Mutex		mutex;			/**< 互斥锁 */
	void			*private_data;		/**< 私有数据 */
};

LCUI_END_HEADER

#endif
