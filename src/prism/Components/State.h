#pragma once

namespace Prism
{
	class Event;

	class State
	{
	public:
		virtual ~State() = default;
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnDraw() = 0;
		virtual void OnSystemEvent(Event& e) = 0;
		//virtual void OnGameEvent();
	private:
	};
}