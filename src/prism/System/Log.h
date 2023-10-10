#pragma once

#include "spdlog/spdlog.h"

namespace Prism
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLog() { return s_CoreLog;  }
		static std::shared_ptr<spdlog::logger>& GetClientLog() { return s_ClientLog; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLog;
		static std::shared_ptr<spdlog::logger> s_ClientLog;
	};
}

#define PR_TRACE(...)		::Prism::Log::GetClientLog()->trace(__VA_ARGS__)
#define PR_INFO(...)		::Prism::Log::GetClientLog()->info(__VA_ARGS__)
#define PR_WARN(...)		::Prism::Log::GetClientLog()->warn(__VA_ARGS__)
#define PR_ERROR(...)		::Prism::Log::GetClientLog()->error(__VA_ARGS__)
#define PR_CRITICAL(...)	::Prism::Log::GetClientLog()->critical(__VA_ARGS__)

#

#ifdef PRISM_DEBUG
#define PR_CORE_TRACE(...)		::Prism::Log::GetCoreLog()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...)		::Prism::Log::GetCoreLog()->info(__VA_ARGS__)
#define PR_CORE_WARN(...)		::Prism::Log::GetCoreLog()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...)		::Prism::Log::GetCoreLog()->error(__VA_ARGS__)
#define PR_CORE_CRITICAL(...)	::Prism::Log::GetCoreLog()->critical(__VA_ARGS__)
#else
#define PR_CORE_TRACE(...)		do {} while(0)
#define PR_CORE_INFO(...)		do {} while(0)
#define PR_CORE_WARN(...)		do {} while(0)
#define PR_CORE_ERROR(...)		do {} while(0)
#define PR_CORE_CRITICAL(...)	do {} while(0)
#endif