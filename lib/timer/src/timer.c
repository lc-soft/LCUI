#include <stdio.h>
#include <stdlib.h>
#include <LCUI/thread.h>
#include <LCUI/timer.h>
#include <LCUI/util.h>

static struct lcui_timers_t {
	timer_list_t *list;
	LCUI_Mutex mutex;
} lcui_timers = { 0 };

int lcui_destroy_timer(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&lcui_timers.mutex);
	ret = timer_destroy(lcui_timers.list, timer_id);
	LCUIMutex_Unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_pause_timer(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&lcui_timers.mutex);
	ret = timer_pause(lcui_timers.list, timer_id);
	LCUIMutex_Unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_continue_timer(int timer_id)
{
	int ret;
	LCUIMutex_Lock(&lcui_timers.mutex);
	ret = timer_continue(lcui_timers.list, timer_id);
	LCUIMutex_Unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_reset_timer(int timer_id, long int n_ms)
{
	int ret;
	LCUIMutex_Lock(&lcui_timers.mutex);
	ret = timer_reset(lcui_timers.list, timer_id, n_ms);
	LCUIMutex_Unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_set_timeout(long int n_ms, void (*callback)(void *), void *arg)
{
	return timer_list_add_timeout(lcui_timers.list, n_ms, callback, arg);
}

int lcui_set_interval(long int n_ms, void (*callback)(void *), void *arg)
{
	return timer_list_add_interval(lcui_timers.list, n_ms, callback, arg);
}

size_t lcui_process_timers(void)
{
	size_t count = 0;
	LCUIMutex_Lock(&lcui_timers.mutex);
	count = timer_list_process(lcui_timers.list);
	LCUIMutex_Unlock(&lcui_timers.mutex);
	return count;
}

void lcui_init_timers(void)
{
	LCUIMutex_Init(&lcui_timers.mutex);
	lcui_timers.list = timer_list_create();
}

void lcui_destroy_timers(void)
{
	if (!lcui_timers.list) {
		return;
	}
	timer_list_destroy(lcui_timers.list);
	LCUIMutex_Destroy(&lcui_timers.mutex);
	lcui_timers.list = NULL;
}
