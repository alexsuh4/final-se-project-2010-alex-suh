#ifndef _WINTHREAD_H
#define _WINTHREAD_H
#include <windows.h>
#include <Winbase.h>
#define pthread_t HANDLE
///wrapper for win32 Sleep
unsigned int sleep(int seconds);
#define pthread_t HANDLE
#define pthread_attr_t int
#define PTHREAD_CREATE_DETACHED 1
//wrapper for win32 CreateThread
int pthread_create(pthread_t *thread, const pthread_attr_t *attr , void *(*start_routine)(void*) , void *arg);
///have no effect as win32 have no detachble threads
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
///not used as using default attributes for win threads
int pthread_attr_init(pthread_attr_t *attr);
///not used as using default attributes for win threads
int pthread_attr_destroy(pthread_attr_t *attr);
#endif