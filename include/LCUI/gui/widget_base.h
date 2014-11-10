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

/** 如果没有包含 widget_build.h 头文件 */
#ifndef __LCUI_WIDGET_BUILD_H__

typedef struct LCUI_WidgetBase* LCUI_Widget;

#endif

#ifdef $
#undef $
#endif

/** 
 * $ 可以看成是 self，部件操作函数的第一个参数类型都是LCUI_Widget，想用
 * 一个字符的来代替，以减少源代码的字符量，再加上又想试试 $，于是就用它了
 */
#define $ LCUI_Widget

/** 边框风格 */
enum BorderStyle {
	BORDER_NONE,	/**< 无边框 */
	BORDER_SOLID,	/**< 实线 */
	BORDER_DOTTED,	/**< 点状 */
	BORDER_DOUBLE,	/**< 双线 */
	BORDER_DASHED,	/**< 虚线 */
};

/** 部件停靠类型 */
enum DockType {
	DOCK_NONE,
	DOCK_TOP,
	DOCK_LEFT,
	DOCK_RIGHT,
	DOCK_FILL,
	DOCK_BOTTOM
};

typedef struct LCUI_WidgetBase {
	int width, height;		/**< 部件区域大小，包括边框和内边距占用区域 */
	int innerWidth, innerHeight;	/**< 内部区域大小，不包括内边距占用区域 */
	int outerWidth, outerHeight;	/**< 外部区域大小，包括边框和阴影占用区域 */
	
	void (*css)($, const char*, const char*);
	void (*dock)($, int);
	void (*focus)($, LCUI_BOOL);
	void (*show)($, LCUI_BOOL);
	void (*shadow)($, int, int, int, LCUI_Color);
	void (*setName)($, const char*);
	void (*setOpacity)($, float);
	void (*setStyleClassName)($, const char*);

	LCUI_BOOL (*isVisible)($);
	
	LCUI_Widget (*at)($, int, int);

	struct {
		void (*top)($, int);
		void (*right)($, int);
		void (*bottom)($, int);
		void (*left)($, int);
		void (*all)($, int, int, int, int);
	} margin, padding;

	struct {
		void (*top)(int, int, LCUI_Color);
		void (*right)(int, int, LCUI_Color);
		void (*bottom)(int, int, LCUI_Color);
		void (*left)(int, int, LCUI_Color);
		void (*all)(int, int, LCUI_Color);
		struct {
			void (*topLeft)(int);
			void (*topRight)(int);
			void (*bottomLeft)(int);
			void (*bottomRight)(int);
			void (*all)(int);
		} radius;
	} border;

	struct {
		void (*color)($, LCUI_Color);
		void (*image)($, LCUI_Graph);
		void (*repeat)($, LCUI_BOOL, LCUI_BOOL);
		void (*position)($, const char*, const char*);
		void (*size)($, const char*, const char*);
		void (*origin)($, int);
	} background;

	struct {
		void (*scale)($, float, float);
		void (*rotate)($, float);
	} transform;
} LCUI_WidgetBase;

#undef $

/** 上面用完 $，接着用 $ 定义个宏，为函数加前缀名 */
#define $(FUNC_NAME) Widget_##FUNC_NAME

extern LCUI_Widget LCUIRootWidget;	/**< 根级部件 */

/** 新建一个GUI部件 */
LCUI_API LCUI_Widget $(New)( const char *type_name );

/* 获取当前点命中的最上层可见部件 */
LCUI_API LCUI_Widget $(At)( LCUI_Widget widget, int x, int y );

#undef $

#endif
