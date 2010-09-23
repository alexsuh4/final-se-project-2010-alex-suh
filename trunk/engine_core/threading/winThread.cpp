#include "winThread.h"

unsigned int sleep(int seconds)
{
	Sleep(seconds*1000);
	return 0;
}

int pthread_create(pthread_t *thread , const pthread_attr_t *attr , void *(*start_routine)(void*) , void *arg)
{
	int newThreadId=-1;
	
	
	*thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)start_routine,arg,0,(LPDWORD)(&newThreadId) );
	return newThreadId;
}

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	return 0;
}

int pthread_attr_init(pthread_attr_t *attr)
{
	return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
	return 0;
}
