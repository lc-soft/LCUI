#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_THREAD_H
#ifdef LCUI_THREAD_PTHREAD

typedef struct _arglist {
	void (*func)(void*);
	void *arg;
} arglist;

static void *run_thread(void *arg)
{
	arglist *ptr;
	ptr = (arglist*)arg;
	ptr->func( ptr->arg );
	free( ptr );
	pthread_exit(NULL);
}

int _LCUIThread_Create( LCUI_Thread *thread, void(*func)(void*), void *arg )
{
	int ret;
	arglist *list;
	
	list = malloc( sizeof(arglist) );
	if( !list ) {
		return -1;
	}
	list->func = func;
	list->arg = arg;
	ret = pthread_create( thread, NULL, run_thread, list );
	if( ret != 0 ) {
		free( list );
	}
	return ret;
}

LCUI_Thread LCUIThread_SelfID( void )
{
	return pthread_self();
}

void _LCUIThread_Exit( void *retval )
{
	pthread_exit( retval );
}

void _LCUIThread_Cancel( LCUI_Thread thread )
{
#ifdef PTHREAD_CANCEL_ASYNCHRONOUS
	pthread_cancel( thread );
#else
	pthread_kill( thread, SIGKILL );
#endif
}

int _LCUIThread_Join( LCUI_Thread thread, void **retval )
{
	return pthread_join( thread, retval );
}

#ifdef need_test_this_code
void test_thread(void *arg)
{
	char *ret_str;
	ret_str = malloc(200);
	sprintf( ret_str, "my tid is %lu", LCUIThread_SelfID() );
	printf( "[TID: %lu] %s\n", LCUIThread_SelfID(), (char*)arg );
	_LCUIThread_Exit( ret_str );
}

int main()
{
	LCUI_Thread thread[2];
	char *str="Hello, World.";
	char *ret_str[2];
	
	_LCUIThread_Create( &thread[0], test_thread, str );
	_LCUIThread_Create( &thread[1], test_thread, str );
	_LCUIThread_Join( thread[0], (void*)&ret_str[0] );
	_LCUIThread_Join( thread[1], (void*)&ret_str[1] );
	printf("ret_str[0]: %s\n", ret_str[0]);
	printf("ret_str[1]: %s\n", ret_str[1]);
	free(ret_str[0]);
	free(ret_str[1]);
	return 0;
}
#endif
#endif
