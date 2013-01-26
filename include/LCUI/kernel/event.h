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

LCUI_BEGIN_HEADER

void LCUI_EventsInit( void );

void LCUI_DestroyEvents( void );

BOOL LCUI_PollEvent( LCUI_Event *event );

void LCUI_StopEventThread( void );

int LCUI_StartEventThread( void );

BOOL LCUI_PushEvent( LCUI_Event *event );

LCUI_END_HEADER

#endif
