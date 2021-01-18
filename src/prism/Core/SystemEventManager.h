#pragma once

#include <array>

#include "prism/System/Debug.h"
#include "Events/Event.h"
#include "GLFW/glfw3.h"
#include "WindowData.h"

namespace Prism::Core
{
	// Queue Events so it doesn't block the whole game on event spawn
	class SystemEventManager
	{
	public:
		SystemEventManager();
		SystemEventManager(GLFWwindow* win);

		void PollEvents();

		void UseRawMouseInput(bool isEnabled) const;

		bool IsKeyDown(Keyboard::Key key) { return m_PressedKeys[key]; }
		bool IsMouseDown(Mouse::Button btn)
		{
			return btn == Mouse::Button::LEFT ? m_MouseState.LeftDown : m_MouseState.RightDown;
		}

		void ShouldSpawnKeyDown(bool isEnabled);
		void ShouldSpawnKeyReleased(bool isEnabled);
		void ShouldSpawnMouseButtonDown(bool isEnabled);
		void ShouldSpawnMouseButtonReleased(bool isEnabled);

		void SetEventCallback(const EventCallback& callback);
		
		void ProcessEvents();
	private:
		void _ProcessEvents();
		void _PushEvent(Event& e);
		void _ProcessKeyboardEvents();
		void _ProcessMouseEvents();

		EventCallback m_EvtCallback;

		struct ButtonStates
		{
			bool KeyDown{ true };
			bool KeyReleased{ true };
			bool MouseButtonDown{ true };
			bool MouseButtonReleased{ true };
		} m_ButtonStates;
		
		struct MouseState
		{
			bool LeftDown{ false };
			bool RightDown{ false };
			double MouseXPos{ 0 };
			double MouseYPos{ 0 };
		} m_MouseState;

		
		GLFWwindow* m_Window; // temp
		bool m_MouseReleasedEvt{ false };
		std::array<bool, 350> m_PressedKeys;
		//std::unordered_map<int, bool> m_PressedKeys; TODO: Maybe use an unordered_map, for now std::array for fast access
		//std::queue<Event> m_PolledEvents;
	};
}
