#ifndef _POSIX_SOCKETS_WINDOWS_H
#define _POSIX_SOCKETS_WINDOWS_H
/**
this file acts as a thin layer on top of win32 function to port
a tiny subset of posix socket functions into win32 enviroment
*/
#include <windows.h>
#include <sys/types.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <share.h>

#define ssize_t unsigned int
#define socklen_t int
//ssize_t write(int fildes, const void *buf, size_t nbyte)
//{
//	return _write(fildes,buf,nbyte);
//}
//ssize_t read(int fildes, void *buf, size_t nbyte)
//{
//	return _read(filedes,buf,nbytes);
//}
//int close( int fd )
//{
//	return _close(fd );
//}

#endif