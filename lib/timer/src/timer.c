#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <timer.h>
#include <yutil.h>

static struct lcui_timers_t {
	timer_list_t *list;
	thread_mutex_t mutex;
} lcui_timers = { 0 };

int lcui_destroy_timer(int timer_id)
{
	int ret;
	thread_mutex_lock(&lcui_timers.mutex);
	ret = timer_destroy(lcui_timers.list, timer_id);
	thread_mutex_unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_pause_timer(int timer_id)
{
	int ret;
	thread_mutex_lock(&lcui_timers.mutex);
	ret = timer_pause(lcui_timers.list, timer_id);
	thread_mutex_unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_continue_timer(int timer_id)
{
	int ret;
	thread_mutex_lock(&lcui_timers.mutex);
	ret = timer_continue(lcui_timers.list, timer_id);
	thread_mutex_unlock(&lcui_timers.mutex);
	return ret;
}

int lcui_reset_timer(int timer_id, long int n_ms)
{
	int ret;
	thread_mutex_lock(&lcui_timers.mutex);
	ret = timer_reset(lcui_timers.list, timer_id, n_ms);
	thread_mutex_unlock(&lcui_timers.mutex);
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
	thread_mutex_lock(&lcui_timers.mutex);
	count = timer_list_process(lcui_timers.list);
	thread_mutex_unlock(&lcui_timers.mutex);
	return count;
}

void lcui_init_timers(void)
{
	thread_mutex_init(&lcui_timers.mutex);
	lcui_timers.list = timer_list_create();
}

void lcui_destroy_timers(void)
{
	if (!lcui_timers.list) {
		return;
	}
	timer_list_destroy(lcui_timers.list);
	thread_mutex_destroy(&lcui_timers.mutex);
	lcui_timers.list = NULL;
}
