#include <LCUI_Build.h>
#include LC_LCUI_H
#ifdef LCUI_THREAD_PTHREAD

int thread_mutex_init( mutex_t *mutex )
{
	return pthread_mutex_init( mutex, NULL );
}

int thread_mutex_destroy( mutex_t *mutex )
{
	return pthread_mutex_destroy( mutex );
}

int thread_mutex_lock( mutex_t *mutex )
{
	return pthread_mutex_lock( mutex );
}

int thread_mutex_unlock( mutex_t *mutex )
{
	return pthread_mutex_unlock( mutex );
}
#endif
