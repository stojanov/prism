#include "LayerSystem.h"


#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


namespace Prism::Core
{
	LayerSystem::LayerSystem()
	{
		
	}

	
	LayerSystem::LayerSystem(SharedContextRef ctx)
		:
		m_Ctx(ctx)
	{
		
	}

	void LayerSystem::PopLayer()
	{
		PR_ASSERT(m_Layers.size() > 0, "Layer stack size must have at least 1 layer.");
		m_Layers[m_Layers.size() - 1]->OnDetach();
		m_Layers.pop_back();
		
	}

	void LayerSystem::PopOverlay()
	{
		PR_ASSERT(m_Overlays.size() > 0, "Layer stack size must have at least 1 layer.");
		m_Overlays[m_Overlays.size() - 1]->OnDetach();
		m_Overlays.pop_back();
	}
	
	void LayerSystem::OnSystemEvent(Event& e)
	{
		for (const auto& overlay : m_Overlays)
		{
			if (overlay->ShouldRespondToEvents())
			{
				overlay->OnSystemEvent(e);
			}
		}
		for (const auto& layer : m_Layers)
		{
			if (layer->ShouldRespondToEvents())
			{
				layer->OnSystemEvent(e);
			}
		}
	}

	void LayerSystem::Update(float dt)
	{
		for (const auto& overlay : m_Overlays)
		{
			overlay->OnUpdate(dt);
		}
		for (const auto& layer : m_Layers)
		{
			layer->OnUpdate(dt);
		}
	}

	void LayerSystem::Draw()
	{
		static constexpr auto LayerDrawFunc = [](const Ptr<ILayer>& layer)
		{
			layer->OnDraw();
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			layer->OnGuiDraw();

			// TODO: Update imgui window size
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		};
		
		for (const auto& layer : m_Layers)
		{
			LayerDrawFunc(layer);
		}
		for (const auto& overlay : m_Overlays)
		{
			LayerDrawFunc(overlay);
		}
	}
}
