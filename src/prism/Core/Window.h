#pragma once

#include <GLFW/glfw3.h>

#include "SystemEventManager.h"
#include "WindowData.h"

namespace Prism::Core
{
	using UpdateCallback = std::function<void(float dt)>;
	using DrawCallback = std::function<void()>;
	
	class Window
	{
	public:
		Window();
		virtual ~Window();

		struct Size
		{
			int width;
			int height;
		};
		
		void Init();
		void Create(int w, int h, const char* name);
		SystemEventManager& GetInputManager() { return m_InputEventManager; }
		
		void ProcessEvents();
		void SetEventCallback(EventCallback callback);

		void Close();
		GLFWwindow* GetNativeWindow() const { return m_WindowPtr;  }
	private:
		int m_Width, m_Height;
		GLFWwindow* m_WindowPtr{ nullptr };
		SystemEventManager m_InputEventManager;
		WindowData m_Data;
	};
}
