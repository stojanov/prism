#pragma once

#include "prism/Core/Events/Event.h"

namespace Prism::Renderer
{
	class ICameraController
	{
	public:
		virtual ~ICameraController() = default;
		virtual void SetMoveSpeed(float s) = 0;
		virtual void ShouldLock(bool lck) = 0;
		virtual void OnSystemEvent(Event& e) = 0;
		virtual void Update(float dt) = 0;
		virtual void ResetDelta() = 0;
	};
}