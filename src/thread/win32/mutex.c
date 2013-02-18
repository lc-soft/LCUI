#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_THREAD_H

#ifdef LCUI_THREAD_WIN32

int LCUIMutex_Init( LCUI_Mutex *mutex )
{
	*mutex = CreateMutex(NULL, FALSE, NULL);
	return 0;
}

/* Free the mutex */
void LCUIMutex_Destroy( LCUI_Mutex *mutex )
{
	CloseHandle( *mutex );
}

/* Lock the mutex */
int LCUIMutex_Lock( LCUI_Mutex *mutex )
{
	if ( WaitForSingleObject( *mutex, INFINITE ) == WAIT_FAILED ) {
		printf("Couldn't wait on mutex\n");
		return -1;
	}
	return 0;
}

/* Unlock the mutex */
int LCUIMutex_UnLock( LCUI_Mutex *mutex )
{
	if ( ReleaseMutex( *mutex ) == FALSE ) {
		printf("Couldn't release mutex\n");
		return -1;
	}
	return 0;
}
#endif
