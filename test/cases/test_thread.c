#include <string.h>
#include <stdio.h>
#include <LCUI.h>
#include <LCUI/thread.h>
#include <yutil.h>
#include "ctest.h"

typedef struct TestWorkerRec_ {
	char data[32];
	int data_count;
	LCUI_BOOL cancel;
	LCUI_BOOL active;
	LCUI_Cond cond;
	LCUI_Mutex mutex;
	LCUI_Thread thread;
} TestWorkerRec, *TestWorker;

static void TestWorker_Thread(void *arg)
{
	TestWorker worker = arg;

	worker->cancel = FALSE;
	worker->active = TRUE;
	worker->data_count = 0;
	LCUIMutex_Lock(&worker->mutex);
	while (!worker->cancel && worker->data_count < 20) {
		logger_debug("waiting...\n");
		LCUICond_Wait(&worker->cond, &worker->mutex);
		logger_debug("get data: %s\n", worker->data);
		worker->data_count += 1;
	}
	LCUIMutex_Unlock(&worker->mutex);
	logger_debug("count: %d\n", worker->data_count);
	worker->active = FALSE;
	LCUIThread_Exit(NULL);
}

static void TestWorker_Send(TestWorker worker, const char *data)
{
	LCUIMutex_Lock(&worker->mutex);
	strcpy(worker->data, data);
	LCUICond_Signal(&worker->cond);
	LCUIMutex_Unlock(&worker->mutex);
}

static void TestWorker_Init(TestWorker worker)
{
	LCUIMutex_Init(&worker->mutex);
	LCUICond_Init(&worker->cond);
}

static void TestWorker_Destroy(TestWorker worker)
{
	LCUIMutex_Lock(&worker->mutex);
	worker->cancel = TRUE;
	LCUICond_Signal(&worker->cond);
	LCUIMutex_Unlock(&worker->mutex);
	LCUIThread_Join(worker->thread, NULL);
	LCUIMutex_Destroy(&worker->mutex);
	LCUICond_Destroy(&worker->cond);
}

void test_thread(void)
{
	TestWorkerRec worker;

	TestWorker_Init(&worker);
	LCUIThread_Create(&worker.thread, TestWorker_Thread, &worker);
	sleep_ms(100);
	TestWorker_Send(&worker, "hello");
	sleep_ms(20);
	TestWorker_Send(&worker, "world");
	sleep_ms(100);
	TestWorker_Send(&worker, "this");
	sleep_ms(50);
	TestWorker_Send(&worker, "is");
	sleep_ms(100);
	TestWorker_Send(&worker, "test");
	sleep_ms(50);
	TestWorker_Send(&worker, "bye!");
	sleep_ms(100);
	TestWorker_Destroy(&worker);
	it_i("check worker data count", worker.data_count, 7);
	it_b("check worker is no longer active", worker.active, FALSE);
}
