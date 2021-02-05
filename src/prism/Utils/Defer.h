#pragma once
#include <functional>

namespace Prism::Utils
{
	class Defer
	{
	public:
		Defer(const std::function<void()>& callback)
			:
			m_Callback(callback)
		{
		}

		~Defer()
		{
			m_Callback();
		}
		
	private:
		std::function<void()> m_Callback;
	};
}
