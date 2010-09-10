/**************************************************
this file is intended to provide simple , cross platform
(win32 / Unix like) shared library (DLL) loading capacities
Alex Suchman 09/09/2010
**************************************************/
#ifndef _SHARD_LIB_WIN_H
#define _SHARD_LIB_WIN_H

#include <iostream>
#include <cstdio>
#include <stdlib.h>

#ifndef _MSC_VER// non windows system (assume Unix -like posix conforming system)
	#include <dlfcn.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <signal.h>
#else
	#include <windows.h> // win32  system
#endif


void *dlsym(void *handle, const char *name);

void *dlopen(const char *file, int mode);

char *dlerror(void);

int dlclose(void *handle);

#define RTLD_LAZY 1

#endif