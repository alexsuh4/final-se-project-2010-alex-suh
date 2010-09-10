/***
	code for simple pthread_mutex operetions for windows
*/
#ifndef _PTHREAD_MUTEX_FOR_WINDOWS
#define _PTHREAD_MUTEX_FOR_WINDOWS
#include <windows.h>
typedef HANDLE pthread_mutex_t; 
void pthread_mutex_init(pthread_mutex_t *mutex_hanlde,int * ignorable_var);
void pthread_mutex_destroy(pthread_mutex_t *mutex_hanlde);
DWORD pthread_mutex_lock(pthread_mutex_t *mutex_handle);
BOOL pthread_mutex_unlock(pthread_mutex_t *mutex_handle);






#endif
