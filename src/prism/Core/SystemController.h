#pragma once
#include "GLFW/glfw3.h"

namespace Prism::Core
{
	class SystemController
	{
	public:
		SystemController(GLFWwindow* win);
		
		void EnableCursor() const;
		void HideCursor() const;
		void DisableCursor() const;
	private:
		GLFWwindow* m_Window;
	};
}
