#include "Window.h"
#include <stdlib.h>
#include "Pointers.h"

#include <chrono>
#include <iostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "prism/System/Debug.h"

namespace Prism::Core
{
	Window::Window()
	{
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_WindowPtr);
	}

	int Window::GetWidth() const
	{
		return m_Data.Width;
	}

	int Window::GetHeight() const 
	{
		return m_Data.Height;
	}

	void Window::BindWindow()
	{
		glfwMakeContextCurrent(m_WindowPtr);
	}
	
	void Window::Create(int w, int h, const char* name)
	{
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		m_LoadingContext = glfwCreateWindow(1, 1, "Loading Context", NULL, NULL);

		if (m_LoadingContext == NULL)
		{
			PR_CORE_ERROR("Coulding creating loading context!");
			PR_ASSERT(0, "Error creating loading context");
		}
		
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
		m_WindowPtr = glfwCreateWindow(w, h, name, NULL, m_LoadingContext);

		if (m_WindowPtr == NULL)
		{
			PR_CORE_ERROR("Coulding creating window!");
			PR_ASSERT(0, "Error creating window");
		}

		glfwMakeContextCurrent(m_WindowPtr);

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			exit(-1);

		glfwSetWindowUserPointer(m_WindowPtr, (void*) &m_Data);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_WindowPtr, true);
		ImGui_ImplOpenGL3_Init("#version 400");

		m_InputEventManager = { m_WindowPtr };
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(m_WindowPtr, true);
	}

	void Window::ProcessEvents()
	{
		m_InputEventManager.ProcessEvents();
	}
	
	void Window::SetEventCallback(EventCallback callback)
	{
		m_Data.OnEvent = callback;
	}
}
