#pragma once

#include "Event.h"
#include "glm/vec2.hpp"

namespace Prism
{
	template<EventType Type>
	class MouseEvent : public Event
	{
	public:
		PR_EVENT(Type)
		MouseEvent(Mouse::Button btn, float x, float y)
			:
			m_Btn(btn),
			m_Pos(x, y)
		{}
		
		Mouse::Button GetKey() const { return m_Btn; }
		const glm::vec2& GetPosition() const { return m_Pos; }
	private:
		Mouse::Button m_Btn;
		glm::vec2 m_Pos;
	};

	using MouseButtonDownEvent = MouseEvent<EventType::MouseButtonDown>;
	using MouseButtonPressedEvent = MouseEvent<EventType::MouseButtonPressed>;
	using MouseButtonReleasedEvent = MouseEvent<EventType::MouseButtonReleased>;
	using MouseMoveEvent = MouseEvent<EventType::MouseMove>;

}
