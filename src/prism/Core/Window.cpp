#include "Window.h"
#include <stdlib.h>
#include "Pointers.h"

#include <chrono>
#include <iostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "glm/vec4.hpp"
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

	void Window::Init()
	{		
		PR_ASSERT(glfwInit(), "Coudn't initialize glfw")
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	
	void Window::Create(int w, int h, const char* name)
	{
		m_WindowPtr = glfwCreateWindow(w, h, name, NULL, NULL);

		if (m_WindowPtr == NULL)
			exit(-1);

		glfwMakeContextCurrent(m_WindowPtr);

		if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
			exit(-1);

		m_InputEventManager = { m_WindowPtr };

		glfwSetWindowUserPointer(m_WindowPtr, (void*) &m_Data);

		ImGui::CreateContext();

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_WindowPtr, true);
		ImGui_ImplOpenGL3_Init("#version 400");
		
		PR_INFO("Gpu - {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		PR_INFO("Driver - {0}", glGetString(GL_VERSION));
		PR_INFO("Shader Version - {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
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
