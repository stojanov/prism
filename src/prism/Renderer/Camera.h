#pragma once
#include "glm/vec3.hpp"

#include "prism/Core/Events/Event.h"

namespace Prism::Renderer
{
	class BaseCamera
	{
	public:
		virtual ~BaseCamera() = default;
		virtual void MoveHorizontally(float x) = 0;
		virtual void MoveVertically(float y) = 0;
		virtual void OffsetXPosition(float x) = 0;
		virtual void OffsetYPosition(float y) = 0;
		virtual void OffsetZPosition(float z) = 0;
		virtual void OffsetPosition(const glm::vec3& position) = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual void OnSystemEvent(Event& e) = 0;
		
		virtual void SetVerticalRotation(float angle) = 0;
		virtual void SetHorizontalRotation(float angle) = 0;
		virtual void OffsetVerticalRotation(float angle) = 0;
		virtual void OffsetHorizontalRotation(float angle) = 0;
	};
}
