#pragma once

namespace Prism::Core
{

	class Timestep
	{
	public:
		Timestep(float dt = 0.f)
			:
				m_Time(dt)
		{}

		float GetSeconds() { return m_Time; }
		float GetMiliseconds() { return m_Time; }
		
	private:
		float m_Time { 0.f };
	};
	
}