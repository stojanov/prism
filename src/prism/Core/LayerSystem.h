#pragma once

#include <vector>
#include <unordered_map>

#include "Pointers.h"
#include "Layer.h"

namespace Prism::Core
{
	class LayerSystem
	{
	public:
		LayerSystem();

		template<typename T, typename = typename std::enable_if<std::is_base_of<Layer, T>::value>::type>
		void CreateLayer(const std::string& name)
		{
			auto LayerPtr = new T();
			LayerPtr->OnAttach();
			m_LayerPositions.emplace(name, m_Layers.size());
			m_Layers.emplace_back(dynamic_cast<Layer*>(LayerPtr));
			m_Layers[m_Layers.size() - 1]->OnAttach();
			m_LastInserts.layer = std::move(name);
		}

		template<typename T, typename = typename std::enable_if<std::is_base_of<Layer, T>::value>::type>
		void CreateOverlay(const std::string& name)
		{
			auto LayerPtr = new T();
			LayerPtr->OnAttach();
			m_LayerPositions.emplace(name, m_Layers.size());
			m_Overlays.emplace_back(dynamic_cast<Layer*>(LayerPtr));
			m_Overlays[m_Layers.size() - 1]->OnAttach();
			m_LastInserts.overlay = std::move(name);
		}
		
		void PopOverlay();
		void PopLayer();

		void Update(float dt);
		void Draw();
		void OnSystemEvent(Event& e);
		// void OnGameEvent();
		
		void RemoveLayer(const std::string& name);
		void RemoveOverlay(const std::string& name);
	private:
		std::vector<Layer*> m_Overlays;
		std::vector<Layer*> m_Layers;
		std::unordered_map<std::string, int> m_LayerPositions;

		// Temp
		struct
		{
			std::string layer;
			std::string overlay;
		} m_LastInserts;
	};
}