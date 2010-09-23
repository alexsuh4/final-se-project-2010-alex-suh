#include "Thread.h"

Thread::Thread() {}
/**
	Thread entry point
	DO NOT OVERRIDE
*/
int Thread::Start(void * arg)
{
   Arg(arg); // store user data
   pthread_attr_t att;
   pthread_attr_init(&att);
   pthread_attr_setdetachstate(&att,PTHREAD_CREATE_DETACHED);
   int code = pthread_create( & ThreadId_, &att, Thread::EntryPoint, (void*) this);
   pthread_attr_destroy(&att);
   return code;
}
/**
internal exceution
*/
int Thread::Run(void * arg)
{
   Setup();
   Execute( arg );
   return 0;
}

/**
static pthread wrapper to be accomodated for c++
*/
void * Thread::EntryPoint(void * pthis)
{
   Thread * pt = (Thread*)pthis;
   pt->Run( pt->Arg() );
   return NULL;
}
/**
thread desctructor - not implemented
*/
Thread::~Thread()
{

}
