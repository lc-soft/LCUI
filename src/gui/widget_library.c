#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H
#include LC_WIDGET_H

typedef struct _WidgetTypeData {
	LCUI_String type_name;
	LCUI_ID type_id;
	LCUI_Queue func;
}
WidgetTypeData;

static LCUI_Queue widget_type_library;

static WidgetTypeData *
WidgetType_FindByID( LCUI_Queue *type_lib, LCUI_ID type_id )
{
	int i, n;
	WidgetTypeData *data;

	n = Queue_GetTotal( type_lib );
	for(i=0; i<n; ++i) {
		data = (WidgetTypeData *)Queue_Get( type_lib, i );
		if( !data || type_id != data->type_id ) {
			continue;
		}
		return data;
	}
	return NULL;
}
/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/
LCUI_API int WidgetFunc_Add(	const char *type_name,
				void (*widget_func)(LCUI_Widget*),
				WidgetFuncType func_type )
{
	WidgetTypeData *data;
	LCUI_Func *temp_func, func_data;
	int total, i;

	data = WidgetType_FindByID( &widget_type_library, WidgetType_GetID(type_name));
	if( !data ) {
		return -2;
	}
	
	func_data.id = func_type;
	func_data.func = (CallBackFunc)widget_func;
	func_data.destroy_arg[0] = FALSE;
	func_data.destroy_arg[1] = FALSE;
	func_data.arg[0] = NULL;
	func_data.arg[1] = NULL;

	total = Queue_GetTotal( &data->func );
	for(i=0; i<total; i++) {
		temp_func = (LCUI_Func*)Queue_Get( &data->func, i );
		if(temp_func->id == func_type) {
			Queue_Replace( &data->func, i, &func_data );
			return 0;
		}
	}
	Queue_Add( &data->func, &func_data );
	return 0;
}

/* 获取指定类型部件的类型ID */
LCUI_API LCUI_ID WidgetType_GetID( const char *widget_type )
{
	return BKDRHash( widget_type );
}

/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/
LCUI_API int WidgetType_Add( const char *type_name )
{
	WidgetTypeData *data, new_data;
	/* 生成类型ID */
	new_data.type_id = WidgetType_GetID( type_name );
	data = WidgetType_FindByID( &widget_type_library, new_data.type_id );
	if( data ) {
		_DEBUG_MSG("the widget type is already registered\n");
		return -1;
	}

	/* 初始化部件函数列表 */
	Queue_Init( &new_data.func, sizeof(LCUI_Func), NULL );
	/* 保存类型名 */
	LCUIString_Init( &new_data.type_name );
	_LCUIString_Copy( &new_data.type_name, type_name );
	Queue_Add( &widget_type_library, &new_data );
	return 0;
}

/* 移除部件类型数据 */
static void WidgetType_Destroy(void *arg)
{
	WidgetTypeData *wd = (WidgetTypeData*)arg;
	Queue_Destroy(&wd->func);
}

extern void RegisterWindow(void);
extern void RegisterLabel(void);
extern void Register_Button(void);
extern void Register_PictureBox(void);
extern void Register_ProgressBar(void);
extern void Register_Menu(void);
extern void Register_CheckBox(void);
extern void Register_RadioButton(void);
extern void Register_ActiveBox(void);
extern void RegisterTextBox(void);
extern void Register_ScrollBar(void);

/* 初始化部件类型库 */
void LCUIWidgetTypeLibrary_Init(void)
{
	Queue_Init( &widget_type_library, sizeof(WidgetTypeData),
						WidgetType_Destroy );
	/* 为程序的部件库添加默认的部件类型 */
	WidgetType_Add(NULL);
	RegisterWindow();
	RegisterLabel();
	Register_Button();
	Register_PictureBox();
	Register_ProgressBar();
	Register_Menu();
	Register_CheckBox();
	Register_RadioButton();
	Register_ActiveBox();
	RegisterTextBox();
	Register_ScrollBar();
}

/* 删除指定部件类型的相关数据 */
LCUI_API int WidgetType_Delete( const char *type_name )
{
	int i, n;
	WidgetTypeData *wd;
	LCUI_ID type_id;

	type_id = WidgetType_GetID( type_name );
	n = Queue_GetTotal( &widget_type_library );
	for(i=0; i<n; ++i) {
		wd = (WidgetTypeData *)Queue_Get( &widget_type_library, i );
		if( wd && wd->type_id == type_id ) {
			return Queue_Delete( &widget_type_library, i );
		}
	}
	return -1;
}

/* 获取指定类型ID的类型名称 */
LCUI_API int WidgetType_GetNameByID( LCUI_ID type_id, char *widget_type )
{
	WidgetTypeData *data;
	data = WidgetType_FindByID( &widget_type_library, type_id );
	if( !data ) {
		return -1;
	}
	strcpy( widget_type, data->type_name.string );
	return 0;
}

/* 获取指定部件类型ID的函数的函数指针 */
LCUI_API WidgetCallBackFunc 
WidgetFunc_GetByID( LCUI_ID id, WidgetFuncType func_type )
{
	LCUI_Func *f;
	WidgetTypeData *data;
	int total, i;

	data = WidgetType_FindByID( &widget_type_library, id );
	if( data == NULL ) {
		return NULL;
	}
	total = Queue_GetTotal( &data->func );
	for(i=0; i<total; i++) {
		f = (LCUI_Func*)Queue_Get( &data->func, i );
		if(f->id == func_type) {
			return (WidgetCallBackFunc)f->func;
		}
	}
	return NULL;
}

/* 获取指定类型名的部件的函数指针 */
LCUI_API WidgetCallBackFunc 
WidgetFunc_Get(	const char *widget_type, WidgetFuncType func_type )
{
	return WidgetFunc_GetByID( BKDRHash(widget_type), func_type );
}

/* 检测指定部件类型是否有效 */
LCUI_API LCUI_BOOL WidgetType_IsValid( const char *widget_type )
{
	int i, n;
	WidgetTypeData *wd;

	n = Queue_GetTotal( &widget_type_library );
	for(i=0; i<n; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&widget_type_library, i);
		if( wd == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp(&wd->type_name, widget_type) == 0) {
			return TRUE;
		}
	}

	return FALSE;
}

/* 调用指定类型的部件函数 */
LCUI_API int WidgetFunc_Call( LCUI_Widget *widget, WidgetFuncType type )
{
	void (*func)(LCUI_Widget*);
	func = WidgetFunc_GetByID( widget->type_id, type );
	if( func ) {
		func( widget );
		return 0;
	}
	return -1;
}
