#pragma once

// Vector emplace Overlays on top, Layers in lastinsertedindex

#include <vector>
#include <unordered_map>

#include "Pointers.h"
#include "Layer.h"
#include "Events/Event.h"

namespace Prism::Core
{
	class LayerSystem
	{
	public:
		LayerSystem();

		template<typename T, typename = typename std::enable_if<std::is_base_of<Layer, T>::value>::type>
		void CreateLayer(const std::string& name)
		{
			m_LayerPositions.insert(name, m_Layers.size());
			m_Layers.emplace_back(MakePtr<T>());
			m_Layers[m_Layers.size() - 1]->OnAttach();
			m_LastInserts.layer = std::move(name);
		}

		template<typename T, typename = typename std::enable_if<std::is_base_of<Layer, T>::value>::type>
		void CreateOverlay(const std::string& name)
		{
			m_LayerPositions.insert(name, m_Layers.size());
			m_Overlays.emplace_back(MakePtr<T>());
			m_Overlays[m_Layers.size() - 1]->OnAttach();
			m_LastInserts.overlay = std::move(name);
		}
		
		void PopOverlay();
		void PopLayer();

		void Update();
		void Draw();
		void OnSystemEvent(Event& e);
		// void OnGameEvent();
		
		void RemoveLayer(const std::string& name);
		void RemoveOverlay(const std::string& name);
	private:
		std::vector<Ptr<Layer>> m_Overlays;
		std::vector<Ptr<Layer>> m_Layers; // temp <Ref<Layer>>
		std::unordered_map<std::string, int> m_LayerPositions;

		struct
		{
			std::string layer;
			std::string overlay;
		} m_LastInserts;
	};
}