#pragma once

#ifdef EDEN_PLATFORM_WINDOWS
	#ifdef EDEN_BUILD_DLL
		#define EDEN_API __declspec(dllexport)
	#else
		#define EDEN_API __declspec(dllimport)
	#endif
#elif EDEN_PLATFORM_LINUX
	#ifdef EDEN_BUILD_DLL
		#define EDEN_API __attribute__((visibility("default")))
	#else
		#define EDEN_API 
	#endif
#else
	#error Eden only supports Windows and Linux!
#endif
