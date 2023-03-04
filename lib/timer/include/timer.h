
#ifndef LCUI_TIMER_H
#define LCUI_TIMER_H

#define LIBTIMER_VERSION "3.0.0-a"
#define LIBTIMER_VERSION_MAJOR 3
#define LIBTIMER_VERSION_MINOR 0
#define LIBTIMER_VERSION_ALTER 0
#define LIBTIMER_STATIC_BUILD 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBTIMER_PUBLIC
#if defined(_MSC_VER) && !defined(LIBTIMER_STATIC_BUILD)
#ifdef LIBTIMER_DLL_EXPORT
#define LIBTIMER_PUBLIC __declspec(dllexport)
#else
#define LIBTIMER_PUBLIC __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
#define LIBTIMER_PUBLIC extern __attribute__((visibility("default")))
#else
#define LIBTIMER_PUBLIC extern
#endif
#endif

LIBTIMER_PUBLIC int lcui_destroy_timer(int timer_id);
LIBTIMER_PUBLIC int lcui_pause_timer(int timer_id);
LIBTIMER_PUBLIC int lcui_continue_timer(int timer_id);
LIBTIMER_PUBLIC int lcui_reset_timer(int timer_id, long int n_ms);
LIBTIMER_PUBLIC int lcui_set_timeout(long int n_ms, void (*callback)(void *), void *arg);
LIBTIMER_PUBLIC int lcui_set_interval(long int n_ms, void (*callback)(void *), void *arg);
LIBTIMER_PUBLIC size_t lcui_process_timers(void);
LIBTIMER_PUBLIC void lcui_init_timers(void);
LIBTIMER_PUBLIC void lcui_destroy_timers(void);

#ifdef __cplusplus
}
#endif

#endif
