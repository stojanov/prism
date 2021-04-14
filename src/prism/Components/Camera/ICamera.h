#pragma once
#include "glm/vec3.hpp"

#include "prism/Core/Events/Event.h"

namespace Prism::Renderer
{
	class IBaseCamera
	{
	public:
		virtual ~IBaseCamera() = default;
		virtual void OffsetPosition(const glm::vec3& position) = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual void ShouldLock(bool lck) = 0;
		virtual void OnSystemEvent(Event& e) = 0;

		virtual void OffsetRotation(const glm::vec2& rot) = 0;
		virtual void Rotate(const glm::vec2& rot) = 0;
	};
}
