#pragma once

#include <vector>
#include <unordered_map>

#include "Pointers.h"
#include "prism/Interfaces/ILayer.h"

namespace Prism::Core
{
	class LayerSystem
	{
	public:
		LayerSystem();
		LayerSystem(SharedContextRef ctx);

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ILayer, T>>>
		void CreateLayer(const std::string& name)
		{
			auto LayerPtr = MakePtr<T>(m_Ctx, name);
			LayerPtr->OnAttach();
			m_Layers.emplace_back(DynamicPtrCast<ILayer>(std::move(LayerPtr)));
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ILayer, T>>>
		void CreateOverlay(const std::string& name)
		{
			auto LayerPtr = new T(m_Ctx, name);
			LayerPtr->OnAttach();
			m_Overlays.emplace_back(DynamicPtrCast<ILayer>(std::move(LayerPtr)));
		}
		
		void PopOverlay();
		void PopLayer();

		void Update(float dt);
		void Draw();
		void OnSystemEvent(Event& e);
		// void OnGameEvent();
		
	private:
		SharedContextRef m_Ctx;
		std::vector<Ptr<ILayer>> m_Overlays;
		std::vector<Ptr<ILayer>> m_Layers;
	};
}
