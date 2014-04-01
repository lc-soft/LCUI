#ifndef __LCUI_WIDGET_LIBRARY_H__
#define __LCUI_WIDGET_LIBRARY_H__

LCUI_BEGIN_HEADER

/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/
LCUI_API int WidgetFunc_Add(	const char *type_name,
				void (*widget_func)(LCUI_Widget*),
				WidgetFuncType func_type );

/* 获取指定类型部件的类型ID */
LCUI_API LCUI_ID WidgetType_GetID( const char *widget_type );

/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/
LCUI_API int WidgetType_Add( const char *type_name );

/* 初始化部件库 */
LCUI_API void WidgetLib_Init( LCUI_Queue *w_lib );

/* 删除指定部件类型的相关数据 */
LCUI_API int WidgetType_Delete( const char *type_name );

/* 获取指定类型ID的类型名称 */
LCUI_API int WidgetType_GetNameByID( LCUI_ID type_id, char *widget_type );

/* 获取指定部件类型ID的函数的函数指针 */
LCUI_API WidgetCallBackFunc WidgetFunc_GetByID( LCUI_ID id,
						WidgetFuncType func_type );

/* 获取指定类型名的部件的函数指针 */
LCUI_API WidgetCallBackFunc WidgetFunc_Get(	const char *widget_type,
						WidgetFuncType func_type );

/* 检测指定部件类型是否有效 */
LCUI_API LCUI_BOOL WidgetType_IsValid( const char *widget_type );

/* 调用指定类型的部件函数 */
LCUI_API int WidgetFunc_Call( LCUI_Widget *widget, WidgetFuncType type );

/* 为程序的部件库添加默认的部件类型 */
LCUI_API void Register_DefaultWidgetType(void);

LCUI_END_HEADER

#endif
