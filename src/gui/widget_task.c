#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/rbtree.h>

#include "widget_private.h"
#include <LCUI/widget.h>

typedef struct LCUI_WidgetTaskRec {
	int type;				/**< 任务类型 */
	LCUI_BOOL hasTask;			/**< 是否有任务 */
	LCUI_WidgetTask data;			/**< 任务数据 */
	void (*taskHandler)(LCUI_WidgetTask);	/**< 任务处理函数 */
	void (*dataDestructor)(void*);		/**< 自定义任务的数据析构函数 */
} LCUI_WidgetTaskRec;

/** 注册一个任务类型，并指定ID */
int Widget_RegisterTaskWithId( LCUI_Widget *widget, const char *task_name, int id )
{

}

/** 注册一个任务类型 */
int Widget_RegisterTask( LCUI_Widget *widget, const char *task_name )
{

}

int Widget_SetTaskHandler( LCUI_Widget *widget, const char *task_name,
					 void (*func)(LCUI_WidgetTask*) )
{

}

/** 添加任务 */
int Widget_AddTask( const char *task_name, LCUI_WidgetTask *data )
{

}
