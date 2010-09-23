#ifndef LIBNETWORK_MODULE_HH_INCLUDED
#define LIBNETWORK_MODULE_HH_INCLUDED

#ifndef _MSC_VER
//unix like system
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#else
//win32 system
#include "../../portability/posix_Sockets_windows.h"

#endif
#include "../threading/Thread.h"
#include <string>
/**
	network communication module
*/
class network
{
	private :
		int server_sockfd;	//self socket descriptor
		int portno;			//listening port
		char is_stop;		//stop flag for listening loop
		char stop_msg[100];//indicate message for server to stop
	public :
		/// initalize server at specified port
		network(int _portno);
		
		/// start listening in specified port
		void start_server();
		
		// stop listening loop
		void stop_server();
		
		/// method to be called when connection socket been established 
		virtual void handleReq(int sockedfd);
		
		///virtual destructor
		virtual ~network();
};



#endif
