#pragma once

#include "Event.h"
#include "glm/vec2.hpp"

namespace Prism
{
	template<EventType Type>
	class MouseButtonEvent : public Event
	{
	public:
		PR_EVENT(Type)
			MouseButtonEvent(Mouse::Button btn) : m_Btn(btn) {}
		Mouse::Button GetKey() const { return m_Btn; }
	private:
		Mouse::Button m_Btn;
	};

	using MouseButtonDownEvent = MouseButtonEvent<EventType::MouseButtonDown>;
	using MouseButtonPressedEvent = MouseButtonEvent<EventType::MouseButtonPressed>;
	using MouseButtonReleasedEvent = MouseButtonEvent<EventType::MouseButtonReleased>;

	class MouseMoveEvent : public Event
	{
	public:
		PR_EVENT(EventType::MouseMoved)

			MouseMoveEvent(float x, float y)
			:
			m_Pos(x, y)
		{}

		MouseMoveEvent(const glm::vec2& pos)
			:
			m_Pos(pos)
		{}

		const glm::vec2& GetPosition() const { return m_Pos; }
	private:
		glm::vec2 m_Pos;
	};
}
