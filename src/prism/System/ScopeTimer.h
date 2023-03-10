#pragma once
#include "Time.h"

namespace Prism::System::Time
{
	template<Duration duration>
	class Scope
	{
	public:
		Scope(const std::string& name)
			:
			m_Name(name),
			m_Start(Clock::now())
		{
		}

		~Scope()
		{
			PR_CORE_INFO("(Timer) {0}\t\t{1}{2}",
				m_Name,
				DurationCast<duration>(Clock::now() - m_Start).count(), 
				GetDurationAsString<duration>()
			);
		}
	private:
		std::string m_Name;
		TimePoint m_Start;
	};
}

#ifdef PRISM_DEBUG
#define PR_SCOPE_TIMER_NS(name) auto PR_TIMER_NS = ::Prism::System::Time::Scope<::Prism::System::Time::Nanoseconds>(name);
#define PR_SCOPE_TIMER_US(name) auto PR_TIMER_US = ::Prism::System::Time::Scope<::Prism::System::Time::Microseconds>(name);
#define PR_SCOPE_TIMER_MS(name) auto PR_TIMER_MS = ::Prism::System::Time::Scope<::Prism::System::Time::Miliseconds>(name);
#define PR_SCOPE_TIMER_S(name) auto PR_TIMER_S = ::Prism::System::Time::Scope<::Prism::System::Time::Seconds>(name);
#else
#define PR_SCOPE_TIMER_NS(name) ({;});
#define PR_SCOPE_TIMER_MS(name) ({;});
#define PR_SCOPE_TIMER_US(name) ({;});
#define PR_SCOPE_TIMER_S(name) ({;});
#endif 
