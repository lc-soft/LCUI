#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_THREAD_H
#ifdef LCUI_THREAD_PTHREAD

/* init the mutex */
int LCUIMutex_Init( LCUI_Mutex *mutex )
{
	return pthread_mutex_init( mutex, NULL );
}

/* Free the mutex */
void LCUIMutex_Destroy( LCUI_Mutex *mutex )
{
	pthread_mutex_destroy( mutex );
}

/* Lock the mutex */
int LCUIMutex_Lock( LCUI_Mutex *mutex )
{
	return pthread_mutex_lock( mutex );
}

/* Unlock the mutex */
int LCUIMutex_UnLock( LCUI_Mutex *mutex )
{
	return pthread_mutex_unlock( mutex );
}
#endif
