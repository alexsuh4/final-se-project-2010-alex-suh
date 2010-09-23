#include "posix_for_windows.h"
void pthread_mutex_init(pthread_mutex_t *mutex_hanlde,int * ignorable_var)
{
	*mutex_hanlde = CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);
}

void pthread_mutex_destroy(pthread_mutex_t *mutex_hanlde)
{
	CloseHandle(*mutex_hanlde);
}

DWORD pthread_mutex_lock(pthread_mutex_t *mutex_handle)
{
	return WaitForSingleObject( 
            *mutex_handle,    // handle to mutex
            INFINITE);  // no time-out interval
}

BOOL pthread_mutex_unlock(pthread_mutex_t *mutex_handle)
{
	return ReleaseMutex(*mutex_handle); // no time-out interval
}

