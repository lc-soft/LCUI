#include <string.h>
#include <stdio.h>
#include <LCUI.h>
#include <ctest-custom.h>

typedef struct TestWorkerRec_ {
	char data[32];
	int data_count;
	LCUI_BOOL cancel;
	LCUI_BOOL active;
	thread_cond_t cond;
	thread_mutex_t mutex;
	thread_t thread;
} TestWorkerRec, *TestWorker;

static void TestWorker_Thread(void *arg)
{
	TestWorker worker = arg;

	worker->cancel = FALSE;
	worker->active = TRUE;
	worker->data_count = 0;
	thread_mutex_lock(&worker->mutex);
	while (!worker->cancel && worker->data_count < 20) {
		logger_debug("waiting...\n");
		thread_cond_wait(&worker->cond, &worker->mutex);
		logger_debug("get data: %s\n", worker->data);
		worker->data_count += 1;
	}
	thread_mutex_unlock(&worker->mutex);
	logger_debug("count: %d\n", worker->data_count);
	worker->active = FALSE;
	thread_exit(NULL);
}

static void TestWorker_Send(TestWorker worker, const char *data)
{
	thread_mutex_lock(&worker->mutex);
	strcpy(worker->data, data);
	thread_cond_signal(&worker->cond);
	thread_mutex_unlock(&worker->mutex);
}

static void TestWorker_Init(TestWorker worker)
{
	thread_mutex_init(&worker->mutex);
	thread_cond_init(&worker->cond);
}

static void TestWorker_Destroy(TestWorker worker)
{
	thread_mutex_lock(&worker->mutex);
	worker->cancel = TRUE;
	thread_cond_signal(&worker->cond);
	thread_mutex_unlock(&worker->mutex);
	thread_join(worker->thread, NULL);
	thread_mutex_destroy(&worker->mutex);
	thread_cond_destroy(&worker->cond);
}

void test_thread(void)
{
	TestWorkerRec worker;

	TestWorker_Init(&worker);
	thread_create(&worker.thread, TestWorker_Thread, &worker);
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
	ctest_equal_int("check worker data count", worker.data_count, 7);
	ctest_equal_bool("check worker is no longer active", worker.active, FALSE);
}
