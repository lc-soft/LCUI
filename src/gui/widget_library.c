#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_ERROR_H

typedef struct _WidgetTypeData
{
	LCUI_String type_name;
	LCUI_ID type_id;
	LCUI_Queue func;
}
WidgetTypeData;

/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/
LCUI_API int 
WidgetFunc_Add(
	const char	*type_name,
	void		(*widget_func)(LCUI_Widget*),
	FuncType	func_type )
{
	LCUI_App *app;
	WidgetTypeData *temp;
	LCUI_Func *temp_func, func_data;
	int total, i, found = 0;

	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("%s(): %s", __FUNCTION__, APP_ERROR_UNRECORDED_APP);
		return -1;
	}

	total = Queue_GetTotal(&app->widget_lib);
	/* 遍历数据，找到对应的位置 */
	for(i = 0; i < total; ++i) {
		temp = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if( temp == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp( &temp->type_name, type_name) != 0) {
			continue;
		}
		total = Queue_GetTotal(&temp->func);
		for(i=0; i<total; i++) {
			temp_func = (LCUI_Func *)Queue_Get(&temp->func, i);
			if(temp_func->id == func_type) {
				found = 1;
				break;
			}
		}

		Get_FuncData(&func_data, (CallBackFunc)widget_func, NULL, NULL);
		func_data.id = func_type; /* 保存类型ID */
		if(found == 1) {/* 如果已经存在，就覆盖 */
			Queue_Replace(&temp->func, i, &func_data);
		} else {
			Queue_Add(&temp->func, &func_data);
		}
		return 0;
	}
	return -1;
}

/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/
LCUI_API int
WidgetType_Add( const char *type_name )
{
	int total, i;
	LCUI_App *app;
	WidgetTypeData *wd, new_wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		printf("WidgetType_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}

	//printf("WidgetType_Add(): add widget type: %s\n", type);
	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if(_LCUIString_Cmp(&wd->type_name, type_name) == 0) {
			//printf("WidgetType_Add(): the widget type is already registered\n");
			return -1;
		}
	}

	/* 初始化部件函数列表 */
	Queue_Init( &new_wd.func, sizeof(LCUI_Func), NULL );
	/* 用随机数作为类型ID */
	new_wd.type_id = rand();
	//printf("WidgetType_Add(): widget type id: %ld\n", new_wd.type_id);
	LCUIString_Init( &new_wd.type_name );
	_LCUIString_Copy( &new_wd.type_name, type_name );
	Queue_Add( &app->widget_lib, &new_wd );
	return 0;
}

static void WidgetType_Destroy(void *arg)
/* 功能：移除部件类型数据 */
{
	WidgetTypeData *wd = (WidgetTypeData*)arg;
	Queue_Destroy(&wd->func);
}

/* 功能：初始化部件库 */
LCUI_API void
WidgetLib_Init(LCUI_Queue *w_lib)
{
	Queue_Init(w_lib, sizeof(WidgetTypeData), WidgetType_Destroy);
}

/* 功能：删除指定部件类型的相关数据 */
LCUI_API int
WidgetType_Delete(const char *type)
{
	int total,  i;
	LCUI_App *app;
	WidgetTypeData *wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return -2;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		/* 如果类型一致 */
		if(_LCUIString_Cmp(&wd->type_name, type) == 0) {
			return Queue_Delete(&app->widget_lib, i);
		}
	}

	return -1;
}

/* 获取指定类型部件的类型ID */
LCUI_API LCUI_ID
WidgetType_GetID( const char *widget_type )
{
	int total, i;
	LCUI_App *app;
	WidgetTypeData *wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return -2;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if(_LCUIString_Cmp(&wd->type_name, widget_type) == 0) {
			return wd->type_id;
		}
	}

	return -1;
}

LCUI_API int
WidgetType_GetByID(LCUI_ID id, char *widget_type)
/* 功能：获取指定类型ID的类型名称 */
{
	int total, i;
	LCUI_App *app;
	WidgetTypeData *wd;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return -2;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id) {
			strcpy(widget_type, wd->type_name.string);
			return 0;
		}
	}

	return -1;
}

/* 获取指定部件类型ID的函数的函数指针 */
LCUI_API WidgetCallBackFunc
WidgetFunc_GetByID(LCUI_ID id, FuncType func_type)
{
	LCUI_App *app;
	LCUI_Func *f = NULL;
	WidgetTypeData *wd;
	int total, i, found = 0;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return NULL;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if( wd == NULL || wd->type_id != id ) {
			continue;
		}
		total = Queue_GetTotal(&wd->func);
		for(i=0; i<total; i++) {
			f = (LCUI_Func *)Queue_Get(&wd->func, i);
			if(f->id == func_type) {
				found = 1;
				break;
			}
		}
		break;
	}

	/* 如果已经存在 */
	if(found == 1) {
		return (WidgetCallBackFunc)f->func;
	}
	return NULL;
}

/* 获取指定类型名的部件的函数指针 */
LCUI_API WidgetCallBackFunc
WidgetFunc_Get(const char *widget_type, FuncType func_type )
{
	LCUI_App *app;
	int total, i, found = 0;
	WidgetTypeData *wd;
	LCUI_Func *f = NULL;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return NULL;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if( wd == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp(&wd->type_name, widget_type) != 0) {
			continue;
		}
		total = Queue_GetTotal(&wd->func);
		for(i=0; i<total; i++) {
			f = (LCUI_Func *)Queue_Get(&wd->func, i);
			if(f->id == func_type) {
				found = 1;
				break;
			}
		}
		break;
	}

	if(found == 1) {
		return (WidgetCallBackFunc)f->func;
	}
	return NULL;
}

/* 检测指定部件类型是否有效 */
LCUI_API int
WidgetType_Valid( const char *widget_type )
{
	int total, i;
	WidgetTypeData *wd;
	LCUI_App *app;

	app = LCUIApp_GetSelf();
	if( !app ) {
		return 0;
	}

	total = Queue_GetTotal(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData *)Queue_Get(&app->widget_lib, i);
		if( wd == NULL ) {
			continue;
		}
		if(_LCUIString_Cmp(&wd->type_name, widget_type) == 0) {
			return 1;
		}
	}

	return 0;
}

/* 调用指定类型的部件函数 */
LCUI_API void
WidgetFunc_Call( LCUI_Widget *widget, FuncType type )
{
	void (*func)(LCUI_Widget*);

	func = WidgetFunc_GetByID( widget->type_id, type );
	if( func != NULL ) {
		func( widget );
	}
}

extern void Register_Window(void);
extern void Register_Label(void);
extern void Register_Button(void);
extern void Register_PictureBox(void);
extern void Register_ProgressBar(void);
extern void Register_Menu(void);
extern void Register_CheckBox(void);
extern void Register_RadioButton(void);
extern void Register_ActiveBox(void);
extern void Register_TextBox(void);
extern void Register_ScrollBar(void);

LCUI_API void
Register_DefaultWidgetType()
/* 功能：为程序的部件库添加默认的部件类型 */
{
	WidgetType_Add(NULL);	/* 添加一个NULL类型的部件 */
	Register_Window();	/* 注册窗口部件 */
	Register_Label();	/* 注册文本标签部件 */
	Register_Button();	/* 注册按钮部件 */
	Register_PictureBox();	/* 注册图片盒子部件 */
	Register_ProgressBar();
	Register_Menu();
	Register_CheckBox();
	Register_RadioButton();
	Register_ActiveBox();
	Register_TextBox();
	Register_ScrollBar();
}
