#pragma once

#include "prism/Core/Events/Event.h"

namespace Prism::Renderer
{
	class CameraController
	{
	public:
		virtual void OnSystemEvent(Event& e) = 0;
		virtual void Update(float dt) = 0;
	};
}