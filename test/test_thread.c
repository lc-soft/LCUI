#include <string.h>
#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include "test.h"

typedef struct TestWorkerRec_ {
	char data[32];
	int data_count;
	LCUI_BOOL active;
	LCUI_Cond cond;
	LCUI_Mutex mutex;
	LCUI_Thread thread;
} TestWorkerRec, *TestWorker;

static void TestWorker_Thread( void *arg )
{
	TestWorker worker = arg;

	worker->active = TRUE;
	worker->data_count = 0;
	LCUIMutex_Lock( &worker->mutex );
	while( worker->active ) {
		TEST_LOG( "waiting...\n" );
		LCUICond_Wait( &worker->cond, &worker->mutex );
		TEST_LOG( "get data: %s\n", worker->data );
		worker->data_count += 1;
	}
	LCUIMutex_Unlock( &worker->mutex );
	TEST_LOG( "count: %lu\n", worker->data_count );
}

static void TestWorker_Send( TestWorker worker, const char *data )
{
	LCUIMutex_Lock( &worker->mutex );
	strcpy( worker->data, data );
	LCUICond_Signal( &worker->cond );
	LCUIMutex_Unlock( &worker->mutex );
}

static void TestWorker_Init( TestWorker worker )
{	
	LCUIMutex_Init( &worker->mutex );
	LCUICond_Init( &worker->cond );
}

static void TestWorker_Destroy( TestWorker worker )
{
	LCUIMutex_Lock( &worker->mutex );
	worker->active = FALSE;
	LCUICond_Signal( &worker->cond );
	LCUIMutex_Unlock( &worker->mutex );
	LCUIThread_Join( worker->thread, NULL );
	LCUIMutex_Destroy( &worker->mutex );
	LCUICond_Destroy( &worker->cond );
}

int test_thread( void )
{
	int ret = 0;
	TestWorkerRec worker;

	TestWorker_Init( &worker );
	LCUIThread_Create( &worker.thread, TestWorker_Thread, &worker );
	LCUI_Sleep( 1 );
	TestWorker_Send( &worker, "hello" );
	LCUI_MSleep( 200 );
	TestWorker_Send( &worker, "world" );
	LCUI_Sleep( 1 );
	TestWorker_Send( &worker, "this" );
	LCUI_MSleep( 500 );
	TestWorker_Send( &worker, "is" );
	LCUI_Sleep( 1 );
	TestWorker_Send( &worker, "test" );
	LCUI_MSleep( 100 );
	TestWorker_Send( &worker, "bye!" );
	LCUI_Sleep( 1 );
	TestWorker_Destroy( &worker );
	CHECK( worker.data_count == 7 );
	return ret;
}
