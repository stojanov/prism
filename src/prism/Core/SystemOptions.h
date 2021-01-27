#pragma once

#include "imgui.h"
#include "Pointers.h"
#include "Window.h"

namespace Prism::Core
{
	class SystemOptions
	{
	public:
		SystemOptions(Ref<Window> win);
		
		bool ImGuiWantMouseCapture();
		bool ImGuiWantKeyboardCapture();
		
		void EnableCursor() const;
		void HideCursor() const;
		void DisableCursor() const;
	private:
		Ref<Window> m_Window;
		ImGuiIO* m_GuiIO;
	};
}
