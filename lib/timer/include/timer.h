
#ifndef LCUI_TIMER_H
#define LCUI_TIMER_H

#include <LCUI/header.h>
#include <stddef.h>

LCUI_BEGIN_HEADER

LCUI_API int lcui_destroy_timer(int timer_id);
LCUI_API int lcui_pause_timer(int timer_id);
LCUI_API int lcui_continue_timer(int timer_id);
LCUI_API int lcui_reset_timer(int timer_id, long int n_ms);
LCUI_API int lcui_set_timeout(long int n_ms, void (*callback)(void *), void *arg);
LCUI_API int lcui_set_interval(long int n_ms, void (*callback)(void *), void *arg);
LCUI_API size_t lcui_process_timers(void);
LCUI_API void lcui_init_timers(void);
LCUI_API void lcui_destroy_timers(void);

LCUI_END_HEADER

#endif
