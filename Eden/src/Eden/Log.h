#pragma once

#include "Eden/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Eden {

	class EDEN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#ifdef EDEN_LOGGING
	// Log macros
	#define EDEN_TRACE(...)   ::Eden::Log::GetLogger()->trace(__VA_ARGS__)
	#define EDEN_INFO(...)    ::Eden::Log::GetLogger()->info(__VA_ARGS__)
	#define EDEN_WARN(...)    ::Eden::Log::GetLogger()->warn(__VA_ARGS__)
	#define EDEN_ERROR(...)   ::Eden::Log::GetLogger()->error(__VA_ARGS__)
	#define EDEN_FATAL(...)   ::Eden::Log::GetLogger()->fatal(__VA_ARGS__)
	#define EDEN_ASSERT(x, ...) { if(!(x)) {EDEN_ERROR("Assertion Failed: {0}", __VA_ARGS__); exit(0);} }
#else
	#define EDEN_TRACE(...)
	#define EDEN_INFO(...)
	#define EDEN_WARN(...)
	#define EDEN_ERROR(...)
	#define EDEN_FATAL(...)
	#define EDEN_ASSERT(x, ...)
#endif
