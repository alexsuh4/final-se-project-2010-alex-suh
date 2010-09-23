/**
This file contains implemetaion of functions used to port 
shared library functionality between Unix like systems (posix complaiant ) and win32 
Prepared as part of Final project for Bsc Degree of SE.
Alex Suchman 09/2010
*/

#include "sharedLib.h"

void *dlsym(void *handle, const char *name)
{
	FARPROC result=GetProcAddress((HMODULE)handle,(LPCSTR)name);
	return (void*)result;
}

void *dlopen(const char *file, int mode)
{

	HMODULE modResult= LoadLibrary( (LPCSTR)file );
	return (void*)modResult;
}
char *errMsg="NO ERROR";
char *dlerror(void)
{
	ErrorGeneric();
	return NULL;
}

int dlclose(void *handle)
{
	return (FreeLibrary ((HMODULE)handle)!=0);
}



void Error(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 
	if (dw==0)
		return;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf);
	printf((char*)lpMsgBuf);
	printf("\n");
}


void ErrorGeneric()
{
	Error(" Error ");
}