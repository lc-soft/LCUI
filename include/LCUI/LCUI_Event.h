#ifndef __LCUI_EVENT_H__
#define __LCUI_EVENT_H__

#include <inttypes.h>

typedef struct {
	uint8_t type;
	int key_code;
} LCUI_KeyboardEvent;

typedef struct {
	uint8_t type;
} LCUI_QuitEvent;

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
	LCUI_QuitEvent quit;
	LCUI_UserEvent user;
} LCUI_Event;

void AppEventQueue_Init( LCUI_App *app );

BOOL _LCUI_PollEvent( LCUI_App *app, LCUI_Event *event );

BOOL LCUI_PollEvent( LCUI_Event *event );

BOOL _LCUI_PushEvent( LCUI_App *app, LCUI_Event *event );

BOOL LCUI_PushEvent( LCUI_Event *event );

#endif
