#ifndef __LCUI_WIDGET_PRIVATE_H__
#define __LCUI_WIDGET_PRIVATE_H__

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
} LCUI_WidgetFull;

#endif
