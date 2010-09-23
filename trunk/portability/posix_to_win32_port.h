#ifndef _POSIX_TO_WIN32_PORT_H
#define _POSIX_TO_WIN32_PORT_H
	#ifdef _MSC_VER
		#include "../portability/posix_Sockets_windows.h"
		#include "../portability/sharedLib.h"
		#include "../portability/posix_common_utils_windows.h"
		#include "../portability/posix_for_windows.h"
	#endif
#endif