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
			m_End = Clock::now();
			PR_CORE_INFO("(Timer) {0}\t\t{1}{2}",
				m_Name,
				DurationCast<duration>(Clock::now() - m_Start), 
				GetDurationAsString<duration>()
			);
		}
	private:
		std::string m_Name;
		TimePoint m_Start;
		TimePoint m_End;
	};
}

