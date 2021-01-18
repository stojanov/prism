#include "Prism.h"


#include <iomanip>
#include <iostream>



#include "Core/Events/WindowEvents.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Prism
{
	Application::Application(int w, int h, const char* name)
	{
		Log::Init();
		m_Window.Init();
		m_Window.Create(w, h, name); // Temp, TODO: Add fullscreen support
		m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_WindowActive = true;
	}

	void Application::OnEvent(Event& e)
	{
		EventHandler evt(e);

		evt.Handle<WindowCloseEvent>([this](Event& e)
		{
			m_WindowActive = false;
		});

		m_Layers.OnSystemEvent(e);
	}
	
	void Application::Run()
	{
		Loop();
	}

	void Application::Loop()
	{
		glm::vec4 clearColor{ 0.5f, 0.0f, 0.3f, 0.0f };

		GLFWwindow* WndPtr = m_Window.GetNativeWindow();
		
		auto StartTime = std::chrono::high_resolution_clock::now();
		auto LastFrameTime = StartTime;

		//glEnable(GL_DEPTH_TEST);
		
		while (m_WindowActive)
		{
			StartTime = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(StartTime - LastFrameTime).count();
			LastFrameTime = std::chrono::high_resolution_clock::now();
			float dt = ms * 1.f / 1000; // TODO: move into it's own class

			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Window.ProcessEvents();
			m_Layers.Update(dt);
			m_Layers.Draw();

			glfwSwapBuffers(WndPtr);
		}
	}


}
