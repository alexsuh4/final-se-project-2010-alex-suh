#ifndef _THREAD_HH
#define _THREAD_HH
#ifndef _MSC_VER // NOT win32 enviroment (probably)
	#include <pthread.h>
	#include "unistd.h"
#else
	#include "winThread.h"	// win32 enviroment (probably)
#endif
/**
	from 
		<a href="http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-18.html">here</a>
	basic c thread wrapper for c++
*/
class Thread
{
   public:
      Thread();
	///force destructor implementation by derived class
	virtual ~Thread();
	/** 
		<p>
		This function provides the means to create the thread and start it going. 
		The argument arg provides a way for user data to be passed into the thread.
		Start() creates the thread by calling the operating system thread creation function.
		</p><br/>
		<b>DO NOT OVERRIDE THIS METHOD<b>
 	*/    
      int Start(void * arg);
   protected:
	///	This is a protected function that should never be tampered with.
      int Run(void * arg);
	///	This function serves as the entry point to the thread. It simply casts pthis to Thread 
      static void * EntryPoint(void*);
      /**
		This function is called after the thread has been created but before Execute() is called. 
		If you override this function, remember to call the parent class Execute().
	*/
      virtual void Setup() {/*implemented by user*/};
      /**
		 must override this function to provide your functionality.
	*/
      virtual void Execute(void*) {/*implemented by user*/};
      /**
		Arguments getter
	*/
      void * Arg() const {return Arg_;}
        /**
		Arguments setter
	*/
      void Arg(void* a){Arg_ = a;}
   private:
	///thread id
      pthread_t ThreadId_;
	///thread arguments
      void * Arg_;

};



#endif
