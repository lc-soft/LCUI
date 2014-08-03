#ifndef __LCUI_WIDGET_BUILD_H__
#define __LCUI_WIDGET_BUILD_H__

LCUI_BEGIN_HEADER

/** 部件结构（完整版） */
typedef struct LCUI_WidgetFullRec_ {
	LCUI_WidgetLite style;		/**< 样式 */
	LCUI_BOOL autosize;		/**< 指定是否自动调整自身的大小，以适应内容的大小 */
	LCUI_BOOL focus;		/**< 指定该部件是否需要焦点 */
	LCUI_Widget *focusWidget;	/**< 获得焦点的子部件 */

	LCUI_Widget *parent;		/**< 父部件 */
	LinkedList children;		/**< 子部件 */
	LCUI_EventBox event;		/**< 事件记录 */
	LCUI_DirtyRectList dirtyRects;	/**< 记录无效区域（脏矩形） */
} LCUI_WidgetFull, LCUI_Widget;

typedef union LCUI_WidgetTask {
	/** 各种任务所需的数据 */
	union {
		struct {
			int x, y; 
		} move;				/**< 移动位置 */
		struct {
			int width, height;
		} resize;			/**< 调整大小 */
		struct {
			LCUI_BOOL visible;
		} show;				/**< 显示/隐藏 */
		void *data;			/**< 自定义任务数据 */
	};
	LCUI_Widget *target;			/**< 目标部件 */
} LCUI_WidgetTask;				/**< 部件任务 */

LCUI_END_HEADER

#include <LCUI/gui/widget.h>

#endif
