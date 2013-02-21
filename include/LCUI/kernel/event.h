#ifndef __LCUI_KERNEL_EVENT_H__
#define __LCUI_KERNEL_EVENT_H__

typedef struct {
	unsigned char type;
	int key_code;
} LCUI_KeyboardEvent;

typedef struct {
	unsigned char type;
	int code;
	void *data1;
	void *data2;
} LCUI_UserEvent;

typedef struct{
	unsigned char type;
	unsigned char state;
	unsigned int x, y;
	unsigned int xrel, yrel;
} LCUI_MouseMotionEvent;

typedef struct {
	unsigned char type;
	unsigned char button;
	unsigned char state;
	unsigned int x, y;
} LCUI_MouseButtonEvent;

typedef enum {
	LCUI_KEYDOWN,
	LCUI_KEYUP,
	LCUI_MOUSEMOTION,
	LCUI_MOUSEBUTTONDOWN,
	LCUI_MOUSEBUTTONUP,
	LCUI_QUIT,
	LCUI_USEREVENT
} LCUI_EVENT_TYPE;

typedef union {
	unsigned char type;
	LCUI_KeyboardEvent key;
	LCUI_MouseMotionEvent motion;
	LCUI_MouseButtonEvent button;
	LCUI_UserEvent user;
} LCUI_Event;

typedef struct {
	int id;			/* 记录事件ID */
	LCUI_Queue func_data;	/* 记录被关联的回调函数数据 */
} LCUI_EventSlot;

LCUI_BEGIN_HEADER

/* 初始化事件模块 */
void LCUIModule_Event_Init( void );

/* 停用事件模块 */
void LCUIModule_Event_End( void );

/* 从事件队列中获取事件 */
LCUI_BOOL LCUI_PollEvent( LCUI_Event *event );

/* 添加事件至事件队列中 */
LCUI_BOOL LCUI_PushEvent( LCUI_Event *event );

/* 初始化事件槽记录 */
void EventSlots_Init( LCUI_Queue *slots );

/* 将函数指针以及两个参数，转换成LCUI_Func类型，保存至p_buff指向的缓冲区中 */
LCUI_BOOL Get_FuncData(	LCUI_Func *p_buff, 
			void (*func) (),
			void *arg1, void *arg2 );

/* 根据事件的ID，获取与该事件关联的事件槽 */
LCUI_EventSlot *
EventSlots_Find( LCUI_Queue *slots, int event_id );

/* 添加事件槽与事件的关联记录 */
int EventSlots_Add( LCUI_Queue *slots, int event_id, LCUI_Func *func );

/* 将回调函数与键盘按键事件进行连接 */
int LCUI_KeyboardEvent_Connect( 
		void (*func)(LCUI_KeyboardEvent*, void*), 
		void *arg );

/* 将回调函数与鼠标移动事件进行连接 */
int LCUI_MouseMotionEvent_Connect( 
		void (*func)(LCUI_MouseMotionEvent*, void*), 
		void *arg );

/* 将回调函数与鼠标按键事件进行连接 */
int LCUI_MouseButtonEvent_Connect( 
		void (*func)(LCUI_MouseButtonEvent*, void*), 
		void *arg );

/* 将回调函数与用户自定义的事件进行连接 */
int LCUI_UserEvent_Connect( int event_id, void (*func)(void*, void*) );

LCUI_END_HEADER

#endif
