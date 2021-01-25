#pragma once
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include "imgui.h"
#include "Pointers.h"
#include "Window.h"

namespace Prism::Core
{
	class SystemController
	{
	public:
		SystemController(Ref<Window> win);
		
		bool ImGuiWantMouseCapture();
		bool ImGuiWantKeyboardCapture();
		
		void DepthTest(bool enabled) const;
		void DrawWireframe(bool enabled) const;
		void ShouldCullFaces(bool enabled = true) const;
		void CullFaceOrder(GLenum order);
		
		void EnableCursor() const;
		void HideCursor() const;
		void DisableCursor() const;
	private:
		Ref<Window> m_Window;
		ImGuiIO* m_GuiIO;
	};
}
