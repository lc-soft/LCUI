#include <stdint.h>
#include <stdio.h>
#include "test.h"
#include "libtest.h"
#include "yutil/keywords.h"
#include "yutil/timer.h"
#include "yutil/time.h"

#ifdef _WIN32
#include <windows.h>
#include <process.h> /* _beginthread, _endthread */
static HANDLE mutex;
#endif

static timer_list_t *timer_list = NULL;
static int count = 0;

/* Unused arguments generate annoying warnings... */
#define arg_not_used(V) ((void)V)

void on_time_out(void *arg)
{
	int *timer_id = (int *)arg;

	it_b("check timer_destroy()", timer_destroy(timer_list, *timer_id) == 0,
	     true);
	timer_list_destroy(timer_list);
	count = -1;
#ifdef _WIN32
	_endthread();
#endif
}

void on_interval(void *arg)
{
	arg_not_used(arg);
	count++;
}

void process(void *ignored)
{
	arg_not_used(ignored);
	while (1) {
#ifdef _WIN32
		WaitForSingleObject(mutex, INFINITE);
		timer_list_process(timer_list);
		ReleaseMutex(mutex);
		sleep_ms(20L);
#endif
	}
}

void test_timer(void)
{
#ifdef _WIN32
	int timer_id;
	int ret = count;

	mutex = CreateMutex(NULL, false, NULL);

	it_b("check timer_list_new()",
	     (timer_list = timer_list_create()) != NULL, true);
	it_b("check timer_list_add_interval()",
	     (timer_id =
		  timer_list_add_interval(timer_list, 10, on_interval, 0)) == 1,
	     true);

	it_b("check timer_list_add_timeout()",
	     timer_list_add_timeout(timer_list, 250, on_time_out, &timer_id) ==
		 2,
	     true);

	// process
	_beginthread(process, 0, NULL);
	sleep_ms(50L);

	// test pause
	WaitForSingleObject(mutex, INFINITE);
	timer_pause(timer_list, timer_id);
	ReleaseMutex(mutex);
	ret = count;
	sleep_ms(50L);
	it_b("check timer_pause()", (ret != 0) && (ret == count), true);

	// test continue
	WaitForSingleObject(mutex, INFINITE);
	timer_continue(timer_list, timer_id);
	ReleaseMutex(mutex);
	sleep_ms(50L);
	it_b("check timer_continue()", ret < count, true);

	count = 0;
	// test reset
	WaitForSingleObject(mutex, INFINITE);
	timer_reset(timer_list, timer_id, 70);
	ReleaseMutex(mutex);
	sleep_ms(50L);
	it_i("check timer_reset()", count, 0);

	sleep_ms(70L);
	it_b("check timer_list_process()", count == -1, true);
#endif
}