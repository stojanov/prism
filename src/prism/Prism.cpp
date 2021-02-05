#include "Prism.h"

#include <iomanip>
#include "Core/Events/KeyEvents.h"
#include "Core/Events/MouseEvents.h"
#include "Core/Events/WindowEvents.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Core/AssetLoader.h"

namespace Prism
{
	Application::Application(int w, int h, const char* name)
	{
		PR_ASSERT(glfwInit(), "Coudn't initialize glfw");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		Log::Init();
		Ref<Core::Window> Window = MakeRef<Core::Window>();
		
		Window->Create(w, h, name); // Temp, TODO: Add fullscreen support
		Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_WindowActive = true;

		m_Context = Core::CreateSharedContext(Window);
		m_Context->RenderOptions->DepthTest(true);
		
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
				m_Context->Window->Close();
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
		glm::vec4 clearColor{ 0.07f, 0.0f, 0.1f, 0.0f };

		GLFWwindow* WndPtr = m_Context->Window->GetNativeWindow();
		
		auto StartTime = std::chrono::high_resolution_clock::now();
		auto LastFrameTime = StartTime;
		
		while (m_WindowActive)
		{
			StartTime = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(StartTime - LastFrameTime).count();
			LastFrameTime = StartTime;
			float dt = ms * 1.f / 1000; // TODO: move into it's own class

			m_Context->RenderOptions->DrawWireframe(m_Wireframe);
			
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Context->Window->ProcessEvents();
			
			if (dt > m_FixedDt)
			{
				while (dt > m_FixedDt) 
				{
					m_Layers.Update(m_FixedDt);
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

		m_Context->Tasks->Finish();
		
		exit(0);
	}


}
