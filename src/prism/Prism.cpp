#include "Prism.h"

#include <iomanip>
#include <iostream>

#include "Core/Events/KeyEvents.h"
#include "Core/Events/MouseEvents.h"
#include "Core/Events/WindowEvents.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Prism
{
	Application::Application(int w, int h, const char* name)
	{
		PR_ASSERT(glfwInit(), "Coudn't initialize glfw")

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		Log::Init();
		m_Window = MakeRef<Core::Window>();
		m_Window->Init();
		m_Window->Create(w, h, name); // Temp, TODO: Add fullscreen support
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_WindowActive = true;

		m_Context = Core::CreateSharedContext(m_Window);
		m_Context->SystemController->DepthTest(true);

		m_Layers = { m_Context };
		
		PR_CORE_INFO("Gpu - {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		PR_CORE_INFO("Driver - {0}", glGetString(GL_VERSION));
		PR_CORE_INFO("Shader Version - {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	void Application::OnEvent(Event& e)
	{
		EventHandler evt(e);

		CLASSEVENT(evt, KeyPressedEvent)
		{
			// PR_INFO("KEY: {0}", (char)e.GetKey());

			switch (e.GetKey())
			{
			case Keyboard::Escape:
				m_Window->Close();
				m_WindowActive = false;
				break;
			case Keyboard::F1:
				m_Wireframe = !m_Wireframe;
				break;
			default:
				break;
			}
		});

		CLASSEVENT(evt, WindowCloseEvent)
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

		GLFWwindow* WndPtr = m_Window->GetNativeWindow();
		
		auto StartTime = std::chrono::high_resolution_clock::now();
		auto LastFrameTime = StartTime;
		
		while (m_WindowActive)
		{
			StartTime = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(StartTime - LastFrameTime).count();
			LastFrameTime = StartTime;
			float dt = ms * 1.f / 1000; // TODO: move into it's own class

			m_Context->SystemController->DrawWireframe(m_Wireframe);
			
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Window->ProcessEvents();
			
			if (dt > m_FixedDt)
			{
				while (dt > m_FixedDt) 
				{
					m_Layers.Update(dt);
					dt -= m_FixedDt;
				}	
			} else
			{
				m_Layers.Update(dt);
			}
			
			m_Layers.Update(dt);
			m_Layers.Draw();

			glfwSwapBuffers(WndPtr);
		}
	}


}
