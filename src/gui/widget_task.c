#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/widget_build.h>

typedef struct LCUI_WidgetTaskPack {
	int type;				/**< 任务类型 */
	LCUI_BOOL is_valid;			/**< 是否有任务 */
	LCUI_WidgetTask *data;			/**< 任务数据 */
	void (*handler)(LCUI_WidgetTask*);	/**< 任务处理函数 */
	void (*data_destructor)(void*);		/**< 自定义任务的数据析构函数 */
} LCUI_WidgetTaskPack;

/** 注册一个任务类型，并指定ID */
int LCUIWidget_RegisterTaskWithId( LCUI_Widget widget, const char *task_name, int id )
{

}

/** 注册一个任务类型 */
int LCUIWidget_RegisterTask( LCUI_Widget widget, const char *task_name )
{

}

int LCUIWidget_SetTaskHandler( LCUI_Widget widget, const char *task_name,
					 void (*func)(LCUI_WidgetTask*) )
{

}

int LCUIWidget_SetTaskHandlerById( LCUI_Widget widget, int task_id,
					 void (*func)(LCUI_WidgetTask*) )
{

}

/** 添加任务 */
int Widget_AddTask( LCUI_Widget widget, const char *task_name, LCUI_WidgetTask *data )
{

}

int Widget_AddTaskById( LCUI_Widget widget, int id, LCUI_WidgetTask *data )
{

}
