#ifndef _POSIX_SOCKETS_WINDOWS_H
#ifdef _MSC_VER
#define _POSIX_SOCKETS_WINDOWS_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define ssize_t unsigned int
#define socklen_t int

int read(int fd, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);
int close(int fildes);

#endif
#endif