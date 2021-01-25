#pragma once

#include "Core/LayerSystem.h"
#include "Core/SharedContext.h"
#include "Core/Window.h"

// TODO: Add State management
namespace Prism
{
	class Application
	{
	public:
		Application(int w, int h, const char* name);

		template<typename T>
		void CreateLayer(const std::string& name)
		{
			m_Layers.CreateLayer<T>(name);
		}

		template<typename T>
		void CreateOverlay(const std::string& name)
		{
			m_Layers.CreateOverlay<T>(name);
		}
		
		void Run();
	private:
		float m_FixedDt{ 0.16f };

		void OnEvent(Event& e);
		void Loop();

		Core::LayerSystem m_Layers;
		Ref<Core::SharedContext> m_Context;
		Ref<Core::Window> m_Window;
		bool m_WindowActive{ false };
		bool m_Wireframe{ false };
	};
}


