#include "SystemEventManager.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

namespace Prism::Core
{
	SystemEventManager::SystemEventManager() 
	{
		m_PressedKeys.fill(false);
	}

	SystemEventManager::SystemEventManager(GLFWwindow* win)
		: m_Window(win)
	{
		m_PressedKeys.fill(false);

		glfwSetWindowSizeCallback(win, [](GLFWwindow* win, int width, int height)
		{
			
		});
	}
	
	void SystemEventManager::UseRawMouseInput(bool isEnabled) const
	{
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, isEnabled);
	}

	void SystemEventManager::ShouldSpawnKeyDown(bool isEnabled)
	{
		m_ButtonStates.KeyDown = isEnabled;
	}

	void SystemEventManager::ShouldSpawnKeyReleased(bool isEnabled)
	{
		m_ButtonStates.KeyReleased = isEnabled;
	}
	
	void SystemEventManager::ShouldSpawnMouseButtonDown(bool isEnabled)
	{
		m_ButtonStates.MouseButtonDown = isEnabled;
	}

	void SystemEventManager::ShouldSpawnMouseButtonReleased(bool isEnabled)
	{
		m_ButtonStates.MouseButtonReleased = isEnabled;
	}

	void SystemEventManager::PollEvents() 
	{
		glfwPollEvents();
	}

	void SystemEventManager::SetEventCallback(const EventCallback& callback)
	{
		m_EvtCallback = callback;
	}
	
	void SystemEventManager::_ProcessEvents()
	{
		_ProcessMouseEvents();
		_ProcessKeyboardEvents();
	}

	void SystemEventManager::_ProcessKeyboardEvents()
	{	
		static auto HandleKey = [this](uint16_t key)
		{
			auto glKeyEvent = glfwGetKey(m_Window, key);
			if (glKeyEvent == GLFW_PRESS)
			{
				if (!m_PressedKeys[key])
				{
					m_PressedKeys[key] = true;
					_PushEvent(KeyPressedEvent ((Keyboard::Key)key));
				}
				
				if (m_ButtonStates.KeyDown)
				{
					_PushEvent(KeyDownEvent ((Keyboard::Key)key));
				}
			}
			else if (this->m_PressedKeys[key] && glKeyEvent == GLFW_RELEASE)
			{
				m_PressedKeys[key] = false;
				_PushEvent(KeyReleasedEvent((Keyboard::Key)key));
			}
		};

		for (uint16_t key = GLFW_KEY_0; key <= GLFW_KEY_WORLD_2; key++)
			HandleKey(key);

		for (uint16_t key = GLFW_KEY_ESCAPE; key <= GLFW_KEY_MENU; key++)
			HandleKey(key);

		HandleKey(GLFW_KEY_SPACE);
	}

	void SystemEventManager::_ProcessMouseEvents()
	{
		static double xpos = 0, ypos = 0;
		glfwGetCursorPos(m_Window, &xpos, &ypos);

		if (xpos != m_MouseState.MouseXPos || ypos != m_MouseState.MouseYPos) 
		{
			_PushEvent(MouseMoveEvent(Mouse::Button::NONE, m_MouseState.MouseXPos = xpos, m_MouseState.MouseYPos = ypos));
		}
	
		static auto OnMouseClick = [this](uint8_t glMouseBtn, Mouse::Button mouseBtn)
		{
			bool& KeyDownRef = (mouseBtn == Mouse::Button::LEFT ? m_MouseState.LeftDown : m_MouseState.RightDown);
			
			if (auto _localBtnState = glfwGetMouseButton(m_Window, glMouseBtn); _localBtnState == GLFW_PRESS)
			{
				if (!KeyDownRef)
				{
					_PushEvent(MouseButtonPressedEvent(mouseBtn, m_MouseState.MouseXPos, m_MouseState.MouseYPos));
					KeyDownRef = true;
				}
				if (m_ButtonStates.MouseButtonDown)
				{
					_PushEvent(MouseButtonDownEvent(mouseBtn, m_MouseState.MouseXPos, m_MouseState.MouseYPos));
				}
			}
			else if (KeyDownRef && _localBtnState == GLFW_RELEASE)
			{
				KeyDownRef = false;
				if (m_ButtonStates.MouseButtonReleased)
				{
					_PushEvent(MouseButtonReleasedEvent(mouseBtn, m_MouseState.MouseXPos, m_MouseState.MouseYPos));
				}
			}
		};

		OnMouseClick(GLFW_MOUSE_BUTTON_LEFT, Mouse::Button::LEFT);
		OnMouseClick(GLFW_MOUSE_BUTTON_RIGHT, Mouse::Button::RIGHT);
	}
	
	void SystemEventManager::_PushEvent(Event& e)
	{
		m_EvtCallback(e);
		//m_PolledEvents.push(e);
	}

	void SystemEventManager::ProcessEvents()
	{
		PollEvents();
		_ProcessEvents();
	}

}
