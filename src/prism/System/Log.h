#pragma once

#include "prism/Core/Pointers.h"
#include "spdlog/spdlog.h"

namespace Prism
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLog() { return s_CoreLog;  }
		static Ref<spdlog::logger>& GetClientLog() { return s_ClientLog; }
	private:
		static Ref<spdlog::logger> s_CoreLog;
		static Ref<spdlog::logger> s_ClientLog;
	};
}

#ifdef PRISM_DEBUG
#define PR_CORE_TRACE(...)		::Prism::Log::GetCoreLog()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...)		::Prism::Log::GetCoreLog()->info(__VA_ARGS__)
#define PR_CORE_WARN(...)		::Prism::Log::GetCoreLog()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...)		::Prism::Log::GetCoreLog()->error(__VA_ARGS__)
#define PR_CORE_CRITICAL(...)	::Prism::Log::GetCoreLog()->critical(__VA_ARGS__)
#else
#define PR_CORE_TRACE(...)		__noop
#define PR_CORE_INFO(...)		__noop
#define PR_CORE_WARN(...)		__noop
#define PR_CORE_ERROR(...)		__noop
#define PR_CORE_CRITICAL(...)	__noop
#endif

#define PR_TRACE(...)		::Prism::Log::GetClientLog()->trace(__VA_ARGS__)
#define PR_INFO(...)		::Prism::Log::GetClientLog()->info(__VA_ARGS__)
#define PR_WARN(...)		::Prism::Log::GetClientLog()->warn(__VA_ARGS__)
#define PR_ERROR(...)		::Prism::Log::GetClientLog()->error(__VA_ARGS__)
#define PR_CRITICAL(...)	::Prism::Log::GetClientLog()->critical(__VA_ARGS__)