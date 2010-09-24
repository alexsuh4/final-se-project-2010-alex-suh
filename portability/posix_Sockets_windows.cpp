#ifdef _MSC_VER
//if under windows
#include "posix_Sockets_windows.h"
int read(int fd, void *buf, size_t nbyte)
{
	return recv(fd,(char*)buf,nbyte,NULL);
}
ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	return send(fildes,(const char*)buf,nbyte,NULL);
}
int close(int fildes)
{
	return closesocket(fildes);
}

#endif

