#include "SystemController.h"

namespace Prism::Core
{
	SystemController::SystemController(Ref<Window> win)
		:
		m_Window(win),
		m_GuiIO(&ImGui::GetIO())
	{		
	}

	bool SystemController::ImGuiWantKeyboardCapture()
	{
		return m_GuiIO->WantCaptureKeyboard;
	}

	bool SystemController::ImGuiWantMouseCapture()
	{
		return m_GuiIO->WantCaptureMouse;
	}

	void SystemController::DepthTest(bool enabled) const
	{
		if (enabled)
		{
			glEnable(GL_DEPTH_TEST);
		} else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
	
	void SystemController::DrawWireframe(bool enabled) const
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void SystemController::ShouldCullFaces(bool enabled) const
	{
		if (enabled)
		{
			glEnable(GL_CULL_FACE);
		} else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void SystemController::CullFaceOrder(GLenum order)
	{
		glCullFace(order);
	}

	void SystemController::EnableCursor() const
	{
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void SystemController::HideCursor() const
	{
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void SystemController::DisableCursor() const
	{
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}
