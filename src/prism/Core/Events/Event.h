#pragma once

#include <glm/vec2.hpp>
#include "Keyboard.h"
#include "Mouse.h"

namespace Prism
{	
	enum class EventType
	{
		None = 0,
		KeyPressed, KeyDown, KeyReleased,
		MouseButtonPressed, MouseButtonDown,MouseButtonReleased, MouseMove,
		WindowResize
	};

#define PR_EVENT(type) EventType GetEventType() const override { return type; } \
						static EventType GetStaticType() { return type; }
	
	class Event
	{
	public:
		//virtual EventType GetEventType() const { return EventType::None; };
		virtual EventType GetEventType() const = 0;
	};

	class EventHandler
	{
	public:
		EventHandler(Event& e)
			:
			m_Evt(e) {}

		template<typename T, typename F,
			typename = typename std::enable_if<std::is_base_of<Event, T>::value>::type>
		bool Handle(const F& func)
		{
			if (m_Evt.GetEventType() == T::GetStaticType())
			{
				func(static_cast<T&>(m_Evt));
				return true;
			}
			return false;
		}
	private:
		Event& m_Evt;
	};
}
