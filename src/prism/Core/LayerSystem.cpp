#include "LayerSystem.h"


namespace Prism::Core
{
	LayerSystem::LayerSystem()
	{
		
	}

	void LayerSystem::PopLayer()
	{
		m_Layers[m_Layers.size() - 1]->OnDetach();
		m_LayerPositions.erase(m_LastInserts.layer);
		m_Layers.pop_back();
	}

	void LayerSystem::PopOverlay()
	{
		m_Overlays[m_Layers.size() - 1]->OnDetach();
		m_LayerPositions.erase(m_LastInserts.overlay);
		m_Overlays.pop_back();
	}

	void LayerSystem::RemoveLayer(const std::string& name)
	{
		if (const auto& layerPos = m_LayerPositions.find(name); layerPos != m_LayerPositions.end())
		{
			m_Layers[layerPos->second]->OnDetach();
			m_Layers.erase(m_Layers.begin() + layerPos->second);
			m_LayerPositions.erase(layerPos);
		}
	}

	void LayerSystem::RemoveOverlay(const std::string& name)
	{
		if (const auto& overlayPos = m_LayerPositions.find(name); overlayPos != m_LayerPositions.end())
		{
			m_Overlays[overlayPos->second]->OnDetach();
			m_Overlays.erase(m_Overlays.begin() + overlayPos->second);
			m_LayerPositions.erase(overlayPos);
		}
	}

	void LayerSystem::OnSystemEvent(Event& e)
	{
		for (const auto& overlay : m_Overlays)
		{
			overlay->OnSystemEvent(e);
		}
		for (const auto& layer : m_Layers)
		{
			layer->OnSystemEvent(e);
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
		for (const auto& layer : m_Layers)
		{
			layer->OnDraw();
		}
		for (const auto& overlay : m_Overlays)
		{
			overlay->OnDraw();
		}
	}
}
