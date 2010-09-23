


#include <cstring>
#include <iostream>
#include <string>
#include "libnetwork_module.h"
using namespace std;
/**
	network communication module
*/

//constructor only needs port number to listen to
const int _SERVER_VER = 2;
network::network(int _portno)
{
	std::cout<<"Server version "<<_SERVER_VER<<"initializing...\n";
	std::cout<<"network object  init started\n";
	strcpy(stop_msg,"bye");
	is_stop=0;
     struct sockaddr_in serv_addr;	//server address

	std::cout<<"setting port number";
	//retrieve port number
	portno=_portno;

	std::cout<<"creating socket\n";
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sockfd < 0)
		throw "ERROR opening socket";

	std::cout<<"socket created\n";;

	std::cout<<"init server address parameter\n";
	//init server address
	memset(&serv_addr,'\0',sizeof(serv_addr));

	std::cout<<"configure server address\n";
	//configure server address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	std::cout<<"binding socket\n";
	//bind socket

/*
	if (bind(server_sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
            char err_msg[1000];
            sprintf(err_msg,"binding failed!, please check port %d is not busy\n",portno);
			std::cout<<err_msg;
			throw err_msg;
			return;
	}
*/
    while (bind(server_sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        char err_msg[1000];
        sprintf(err_msg,"binding failed!, please check port %d is not busy\n",portno);
        std::cerr<<err_msg;
        std::cout<<"press N to abort or any key to retry...\n";
        char pressed_key;
        std::cin>>pressed_key;
        switch(pressed_key)
        {
            case 'n':
            case 'N':
                throw err_msg;
                break;
            default:
                break;
        }//switch N[n]o / default
    }//while loop

	std::cout<<"network initiated\n";
}
//start the server
void network::start_server()
{

	std::cout<<"waiting incoming connections\n";
	int
		newsockfd	,	//new socket descriptor
		clilen		;//length of client address
	struct sockaddr_in
		cli_addr;		//client address

	//error detection
	int err_count=0;
	int ERR_MAX=5;
	while(!is_stop)
	{
		///////////////////////////////
		//		server listening loop
		//////////////////////////////
		listen(server_sockfd,5);
		//compute size of client address
     		clilen = sizeof(cli_addr);
		//get client socket
		newsockfd = accept(server_sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)(&clilen));
		std::cout<<"recieved connection\n";
		if (newsockfd < 0)
		{
				std::cout<<"cannot recieve connection\n";
				//if accapt fails sleep some time and wait for next request
				if (err_count>=ERR_MAX)
				{
					std::cout<<"**succesive errors exceeded.closing server**\n";
					is_stop=1;
				}
				else
				{
					err_count++;
				}

				sleep(1);
				continue;
		}
		//if last one been succesful, reset fail counter
		err_count=0;

		//call handler method to be implemented by deriving class
		handleReq(newsockfd);


	}
	//close server socket
	close(server_sockfd);

}
//stop the server
void network::stop_server()
{
	std::cout<<"server stop method called\n";
	is_stop=1;
	std::cout<<"sending stop message to server!\n";

	//send_message(stop_msg,"localhost",portno); //<-not implemented yet
}

//called by server when succesful connection been recieved
//socketfd
void network::handleReq(int sockfd)
{
	//to be implemented by deriving class
}

network::~network()
{
}



