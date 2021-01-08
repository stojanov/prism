#pragma once

#include <GLFW/glfw3.h>

#include "SystemEventManager.h"

namespace Prism::Core
{
	class Window
	{
	public:
		Window();
		virtual ~Window();

		void Init();
		void Create(int w, int h, const char* name);
		void Loop();
		SystemEventManager& GetInputManager() { return m_InputEventManager; }

		GLFWwindow* GetNativeWindow() const { return m_WindowPtr;  }
	private:
		int m_Width, m_Height;
		GLFWwindow* m_WindowPtr{ nullptr };
		SystemEventManager m_InputEventManager;
	};
}
