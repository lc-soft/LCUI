#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_ERROR_H

static LCUI_Queue events;
static LCUI_BOOL active = FALSE;
static LCUI_Thread eventloop_thread = -1;

/* 事件队列初始化 */
static void LCUI_EventsInit( void )
{
	Queue_Init( &events, sizeof(LCUI_Event), NULL );
}

/* 销毁事件队列 */
static void LCUI_DestroyEvents( void )
{
	Destroy_Queue( &events );
}

/* 从事件队列中获取事件 */
LCUI_BOOL LCUI_PollEvent( LCUI_Event *event )
{
	LCUI_Event *tmp;
	
	if( !active ) {
		return FALSE;
	}
	
	Queue_Lock( &events );
	tmp = Queue_Get( &events, 0 );
	if( !tmp ) {
		Queue_UnLock( &events );
		return FALSE;
	}
	*event = *tmp;
	Queue_Delete( &events, 0 );
	Queue_UnLock( &events );
	return TRUE;
}

/* 将系统事件分发到已注册的回调函数 */
static void 
LCUI_DispatchSystemEvent( LCUI_Event *event )
{
	int i, n;
	LCUI_EventSlot *slot;
	LCUI_Task *task;
	LCUI_Event event_buff;
	
	slot = EventSlots_Find( &LCUI_Sys.sys_event_slots, event->type);
	if( !slot ) {
		return;
	}
	n = Queue_Get_Total( &slot->func_data );
	for(i=0; i<n; ++i) {
		task = Queue_Get( &slot->func_data, i );
		if( !task || !task->func ) {
			continue;
		}
		/* 备份一次，避免回调函数修改事件数据 */
		event_buff = *event;
		switch( event_buff.type ) {
			case LCUI_KEYDOWN:
			case LCUI_KEYUP:
				task->func(	&event_buff.key, 
						task->arg[1] );
				break;
			case LCUI_MOUSEMOTION:
				task->func(	&event_buff.motion,
						task->arg[1] );
				break;
			case LCUI_MOUSEBUTTONDOWN:
			case LCUI_MOUSEBUTTONUP:
				task->func(	&event_buff.button,
						task->arg[1] );
				break;
			default: break;
		}
	}
}

/* 将用户事件分发到已注册的回调函数 */
static void 
LCUI_DispatchUserEvent( LCUI_Event *event )
{
	int i, n;
	LCUI_EventSlot *slot;
	LCUI_Task *task;
	LCUI_UserEvent user_event;
	
	slot = EventSlots_Find(	&LCUI_Sys.user_event_slots, 
				event->user.code );
	if( !slot ) {
		return;
	}
	n = Queue_Get_Total( &slot->func_data );
	for(i=0; i<n; ++i) {
		task = Queue_Get( &slot->func_data, i );
		if( !task || !task->func ) {
			continue;
		}
		user_event = event->user;
		task->func( user_event.data1, user_event.data2 );
	}
}

/* 事件循环处理 */
static void LCUI_EventLoop( void *unused )
{
	LCUI_Event event;
	int delay_time = 1;
	
	while( active ) {
		if( LCUI_PollEvent( &event ) ) {
			delay_time = 1;
			switch( event.type ) {
			case LCUI_KEYDOWN:
			case LCUI_KEYUP:
			case LCUI_MOUSEMOTION:
			case LCUI_MOUSEBUTTONDOWN:
			case LCUI_MOUSEBUTTONUP:
				LCUI_DispatchSystemEvent( &event );
				break;
			case LCUI_USEREVENT:
				LCUI_DispatchUserEvent( &event );
				break;
			}
		} else {
			if( delay_time <= 15 ) {
				delay_time += 1;
			}
			LCUI_MSleep( delay_time );
		}
	}
	_LCUIThread_Exit( NULL );
}

/* 停用事件线程 */
static void LCUI_StopEventThread( void )
{
	if( !active ) {
		return;
	}
	active = FALSE;
	_LCUIThread_Join( eventloop_thread, NULL );
}

/* 启动事件线程 */
static int LCUI_StartEventThread( void )
{
	LCUI_StopEventThread();
	active = TRUE;
	return _LCUIThread_Create( &eventloop_thread, LCUI_EventLoop, NULL );
}

/* 初始化事件模块 */
void LCUIModule_Event_Init( void )
{
	EventSlots_Init( &LCUI_Sys.sys_event_slots );
	EventSlots_Init( &LCUI_Sys.user_event_slots );
	LCUI_EventsInit();
	LCUI_StartEventThread();
}

/* 停用事件模块 */
void LCUIModule_Event_End( void )
{
	LCUI_StopEventThread();
	LCUI_DestroyEvents();
	Destroy_Queue( &LCUI_Sys.sys_event_slots );
	Destroy_Queue( &LCUI_Sys.user_event_slots );
}

/* 添加事件至事件队列中 */
LCUI_BOOL LCUI_PushEvent( LCUI_Event *event )
{
	if( !active ) {
		return FALSE;
	}
	Queue_Lock( &events );
	if(Queue_Add( &events, event ) < 0) {
		Queue_UnLock( &events );
		return FALSE;
	}
	Queue_UnLock( &events );
	return TRUE;
}

/* 销毁事件槽数据 */
static void Destroy_EventSlot( void *arg )
{
	LCUI_EventSlot *slot;
	slot = (LCUI_EventSlot*)arg;
	if( slot ) {
		Destroy_Queue( &slot->func_data );
	}
}

/* 初始化事件槽记录 */
void EventSlots_Init( LCUI_Queue *slots )
{
	Queue_Init( slots, sizeof(LCUI_EventSlot), Destroy_EventSlot);
}

/* 将函数指针以及两个参数，转换成LCUI_Func类型，保存至p_buff指向的缓冲区中 */
LCUI_BOOL Get_FuncData(	LCUI_Func *p_buff, 
			void (*func) (),
			void *arg1, void *arg2 )
{
	LCUI_App *app;
	app = LCUIApp_GetSelf();
	
	if( !app ) {
		printf("%s(): %s", __FUNCTION__, APP_ERROR_UNRECORDED_APP);
		return FALSE;
	}
	
	p_buff->id = app->id;
	p_buff->func = func;
	/* 
	 * 只是保存了指向参数的指针，如果该参数是局部变量，在声明它的函数退出后，该变量
	 * 的空间可能会无法访问。
	 *  */
	p_buff->arg[0] = arg1;	
	p_buff->arg[1] = arg2;
	p_buff->destroy_arg[0] = FALSE;
	p_buff->destroy_arg[1] = FALSE;
	return TRUE;
}

/* 根据事件的ID，获取与该事件关联的事件槽 */
LCUI_EventSlot *
EventSlots_Find( LCUI_Queue *slots, int event_id )
{
	int i, total;  
	LCUI_EventSlot *slot;
	
	total = Queue_Get_Total( slots );
	if (total <= 0) {
		return NULL;
	}
	for (i = 0; i < total; ++i) {
		slot = Queue_Get( slots, i );
		if( slot->id == event_id ) {
			return slot;
		}
	}
	return NULL;
}

/* 添加事件槽与事件的关联记录 */
int EventSlots_Add( LCUI_Queue *slots, int event_id, LCUI_Func *func )
{
	LCUI_EventSlot *slot;
	
	slot = EventSlots_Find( slots, event_id );
	if ( !slot ) {
		LCUI_EventSlot new_slot;
		
		new_slot.id = event_id;
		Queue_Init( &new_slot.func_data, sizeof(LCUI_Func), NULL);
		Queue_Add( &new_slot.func_data, func );
		if( Queue_Add( slots, &new_slot ) < 0 ) {
			return -1;
		}
	} else {
		if( Queue_Add( &slot->func_data, func ) < 0 ) {
			return -1;
		}
	}
	return 0;
}

/* 将回调函数与键盘按键事件进行连接 */
int LCUI_KeyboardEvent_Connect( 
		void (*func)(LCUI_KeyboardEvent*, void*), 
		void *arg )
{
	LCUI_Func func_data;
	if( !Get_FuncData( &func_data, func, NULL, arg ) ) {
		return -1;
	}
	return EventSlots_Add(	&LCUI_Sys.sys_event_slots, 
				LCUI_KEYDOWN, &func_data );
}

/* 将回调函数与鼠标移动事件进行连接 */
int LCUI_MouseMotionEvent_Connect( 
		void (*func)(LCUI_MouseMotionEvent*, void*), 
		void *arg )
{
	LCUI_Func func_data;
	if( !Get_FuncData( &func_data, func, NULL, arg ) ) {
		return -1;
	}
	return EventSlots_Add(	&LCUI_Sys.sys_event_slots, 
				LCUI_MOUSEMOTION, &func_data );
}

/* 将回调函数与鼠标按键事件进行连接 */
int LCUI_MouseButtonEvent_Connect( 
		void (*func)(LCUI_MouseButtonEvent*, void*), 
		void *arg )
{
	int ret = 0;
	LCUI_Func func_data;
	
	if( !Get_FuncData( &func_data, func, NULL, arg ) ) {
		return -1;
	}
	ret += EventSlots_Add(	&LCUI_Sys.sys_event_slots, 
				LCUI_MOUSEBUTTONDOWN, &func_data );
	ret += EventSlots_Add(	&LCUI_Sys.sys_event_slots, 
				LCUI_MOUSEBUTTONUP, &func_data );
	return ret>=0?0:-1;
}

/* 将回调函数与用户自定义的事件进行连接 */
int LCUI_UserEvent_Connect( int event_id, void (*func)(void*, void*) )
{
	LCUI_Func func_data;
	if( !Get_FuncData( &func_data, func, NULL, NULL ) ) {
		return -1;
	}
	return EventSlots_Add(	&LCUI_Sys.user_event_slots, 
				LCUI_USEREVENT, &func_data );
}
