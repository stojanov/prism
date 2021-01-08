#include "SystemController.h"

namespace Prism::Core
{
	SystemController::SystemController(GLFWwindow* win)
		:
		m_Window(win)
	{		
	}

	void SystemController::EnableCursor() const
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void SystemController::HideCursor() const
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void SystemController::DisableCursor() const
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
