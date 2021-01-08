#include "Window.h"
#include <stdlib.h>
#include "Pointers.h"

#include <chrono>
#include <iostream>
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

		m_InputEventManager = SystemEventManager(m_WindowPtr);
		
		PR_INFO("Gpu - {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		PR_INFO("Driver - {0}", glGetString(GL_VERSION));
		PR_INFO("Shader Version - {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	void Window::Loop()
	{
		glm::vec4 clearColor{ 0.0f, 0.8f, 0.0f, 0.0f };

		auto StartTime = std::chrono::high_resolution_clock::now();
		auto LastFrameTime = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(m_WindowPtr)) 
		{
			StartTime = std::chrono::high_resolution_clock::now();
			float dt = std::chrono::duration_cast<std::chrono::milliseconds>(LastFrameTime - StartTime).count();
			glfwGetWindowSize(m_WindowPtr, &m_Width, &m_Height);

			glViewport(0, 0, m_Width, m_Height);
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(m_WindowPtr);
			LastFrameTime = std::chrono::high_resolution_clock::now();
		}
	}
}
