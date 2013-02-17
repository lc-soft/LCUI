#include <LCUI_Build.h>
#include LC_LCUI_H 
#ifdef LCUI_THREAD_PTHREAD

thread_t thread_self( void )
{
	return pthread_self();
}

int thread_create(	thread_t *__newthread,
			void *(*__start_routine) (void *),
			void *__arg )
{
	return pthread_create ( __newthread, NULL, __start_routine, __arg);
}

int thread_cancel( thread_t __th )
{
	return pthread_cancel (__th);
}

int thread_join( thread_t __th, void **__thread_return )
{
	return pthread_join (__th, __thread_return);
}

void thread_exit( void *__retval )
{
	pthread_exit(__retval);
}
#endif
