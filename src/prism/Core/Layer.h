#pragma once

namespace Prism {
	class Event;
}

namespace Prism::Core
{
	class Layer
	{
	public:
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDraw() = 0;
		virtual void OnSystemEvent(Event& e) = 0;
		//virtual void OnGameEvent();
	};
}
