#ifndef __LCUI_KERNEL_EVENT_H__
#define __LCUI_KERNEL_EVENT_H__

#include <inttypes.h>

typedef struct {
	uint8_t type;
	int key_code;
} LCUI_KeyboardEvent;

typedef struct {
	uint8_t type;
	int code;
	void *data1;
	void *data2;
} LCUI_UserEvent;

typedef struct{
	uint8_t type;
	uint8_t state;
	uint16_t x, y;
	uint16_t xrel, yrel;
} LCUI_MouseMotionEvent;

typedef struct {
	uint8_t type;
	uint8_t button;
	uint8_t state;
	uint16_t x, y;
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
	uint8_t type;
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

/* 事件队列初始化 */
void LCUI_EventsInit( void );

/* 销毁事件队列 */
void LCUI_DestroyEvents( void );

/* 从事件队列中获取事件 */
BOOL LCUI_PollEvent( LCUI_Event *event );

/* 停用事件线程 */
void LCUI_StopEventThread( void );

/* 启动事件线程 */
int LCUI_StartEventThread( void );

/* 添加事件至事件队列中 */
BOOL LCUI_PushEvent( LCUI_Event *event );

/* 初始化事件槽记录 */
void EventSlots_Init( LCUI_Queue *slots );

/* 将函数指针以及两个参数，转换成LCUI_Func类型，保存至p_buff指向的缓冲区中 */
BOOL Get_FuncData(	LCUI_Func *p_buff, 
			void (*func) (),
			void *arg1, void *arg2 );

/* 根据事件的ID，获取与该事件关联的事件槽 */
LCUI_EventSlot *
EventSlots_Find( LCUI_Queue *slots, int event_id );

/* 添加事件槽与事件的关联记录 */
int EventSlots_Add( LCUI_Queue *slots, int event_id, LCUI_Func *func );

/* 
 * 功能：将指定ID的事件分发到已注册的回调函数
 * 说明：本函数会将事件槽记录中与指定ID的事件关联的回调函数 添加至程序的任务队列
 * */
int EventSlots_DispatchEvent( LCUI_Queue *slots, int event_id );

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
