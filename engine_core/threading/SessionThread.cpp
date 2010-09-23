#include "Thread.h"
#include <iostream>
using namespace std;
/**
	Session Thread Implementation
*/
class SessionThread : public Thread 
{
	public:
		SessionThread() : Thread()
		{
			
		}
	protected:
		void Setup();
		void Execute(void*);
};

/**
	do any setup here
*/
void SessionThread::Setup()
{
        // Do any setup here
	std::cout<<"sesion SessionThread setup been called\n";
}
/**
	derived code goes here
*/
void SessionThread::Execute(void* arg)
{
        // Your code goes here
	std::cout<<"session SessionThread exceute been called\n";
	while(1)
	{
		std::cout<<"thread exceuting\n";
		sleep(1);
	}
}


#ifdef SESSION_THREAD_UNIT_TESTING
/**
unit testing entry point
*/
int main(int argc,char **argv)
{
	std::cout<<"compiled OK\n";
	SessionThread tr;
	
	tr.Start(NULL);
	std::cout<<"main is going to sleep";
	while(1)
	{
		sleep(10);
	}
	return 0;
	
}
#endif