#pragma once

#include "Core/LayerSystem.h"
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
		void OnEvent(Event& e);
		void Loop();

		Core::LayerSystem m_Layers;
		Core::Window m_Window;
		bool m_WindowActive{ false };
	};
}


