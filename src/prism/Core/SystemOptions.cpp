#include "SystemOptions.h"

#include <glad/glad.h>

namespace Prism::Core
{
	SystemOptions::SystemOptions(Ref<Window> win)
		:
		m_Window(win),
		m_GuiIO(&ImGui::GetIO())
	{		
	}

	bool SystemOptions::ImGuiWantKeyboardCapture()
	{
		return m_GuiIO->WantCaptureKeyboard;
	}

	bool SystemOptions::ImGuiWantMouseCapture()
	{
		return m_GuiIO->WantCaptureMouse;
	}

	void SystemOptions::EnableCursor() const
	{
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void SystemOptions::HideCursor() const
	{
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void SystemOptions::DisableCursor() const
	{
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
