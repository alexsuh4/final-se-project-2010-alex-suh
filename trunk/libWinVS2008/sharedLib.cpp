/**
This file contains implemetaion of functions used to port 
shared library functionality between Unix like systems (posix complaiant ) and win32 
Prepared as part of Final project for Bsc Degree of SE.
Alex Suchman 09/2010
*/

#include "sharedLib.h"

void *dlsym(void *handle, const char *name)
{
	return (void*)GetProcAddress((HMODULE)handle,(LPCSTR)name);
}

void *dlopen(const char *file, int mode)
{
	return (void*)LoadLibrary( (LPCSTR)file );
}
char *errMsg="NO ERROR";
char *dlerror(void)
{
	return errMsg;
}

int dlclose(void *handle)
{
	return (FreeLibrary ((HMODULE)handle)!=0);
}
