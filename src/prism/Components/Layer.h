#pragma once
#include <string>

#include "prism/Core/SharedContext.h"

namespace Prism {
	class Event;
}

namespace Prism
{
	class Layer
	{
	public:
		Layer(Ref<Core::SharedContext> ctx, const std::string& name);
		virtual ~Layer();
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnDraw() = 0;
		virtual void OnSystemEvent(Event& e) = 0;
		virtual void OnGuiDraw() = 0;
		//virtual void OnGameEvent();

		bool ShouldRespondToEvents() const { return m_ShouldRespondToEvents; }
		const std::string& GetName() const { return m_Name; }
	protected:
		Core::SharedContextRef m_Ctx;
		std::string m_Name;
		bool m_ShouldRespondToEvents{ true };
	};
}
