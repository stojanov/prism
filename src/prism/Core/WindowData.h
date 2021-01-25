#pragma once
#include <functional>
#include "Events/Event.h"

namespace Prism::Core
{
	using EventCallback = std::function<void(Event& e)>;
	
	struct WindowData
	{
		EventCallback OnEvent;
		int Width;
		int Height;
	};
}
