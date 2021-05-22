#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "prism/System/Debug.h"

namespace Prism::Renderer
{
	PerspectiveCamera::PerspectiveCamera() = default;
	PerspectiveCamera::~PerspectiveCamera() = default;

	PerspectiveCamera::PerspectiveCamera(float FOV, int width, int height, float clipPlaneNear, float clipPlaneFar)
		:
		m_Fov(FOV),
		m_ClipNear(clipPlaneNear),
		m_ClipFar(clipPlaneFar)
	{
		m_Projection = glm::perspective(glm::radians(FOV), width * 1.0f / height, clipPlaneNear, clipPlaneFar);
	}

	void PerspectiveCamera::ShouldLock(bool lck)
	{
		if (m_HasContoller)
		{
			m_Controller->ShouldLock(lck);
		}
	}

	void PerspectiveCamera::SetLookAt(const glm::vec3& LookAt)
	{
		m_LookAt = LookAt;
	}
	
	void PerspectiveCamera::UpdatePerspective(float fov, int width, int height, float clipPlaneNear, float clipPlaneFar)
	{
		m_Projection = glm::perspective(glm::radians(fov), width * 1.f / height, clipPlaneNear, clipPlaneFar);
	}

	void PerspectiveCamera::UpdateRatio(int width, int height)
	{
		m_Projection = glm::perspective(glm::radians(m_Fov), width * 1.f / height, m_ClipNear, m_ClipFar);
	}

	void PerspectiveCamera::OffsetRotation(const glm::vec2& rot)
	{	
		m_RotationDelta += rot;
	}

	void PerspectiveCamera::Rotate(const glm::vec2& rot)
	{
		m_Rotation = rot;
	}

	void PerspectiveCamera::MoveZ(float speed)
	{
		m_DPosition += m_Direction * speed;
	}
	
	void PerspectiveCamera::MoveX(float speed)
	{
		m_DPosition += glm::normalize(glm::cross(m_Direction, m_Up)) * speed;
	}

	void PerspectiveCamera::MoveY(float speed)
	{
		m_DPosition += glm::normalize(m_Up) * speed;
	}
	
	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}
	void PerspectiveCamera::OffsetPosition(const glm::vec3& position)
	{
		m_Position += position;
	}
	
	void PerspectiveCamera::OnSystemEvent(Event& e)
	{
		if (m_HasContoller)
		{
			m_Controller->OnSystemEvent(e);
		}	
	}

	void PerspectiveCamera::_RotateCamera()
	{
	}
	
	void PerspectiveCamera::OnUpdate(float dt)
	{
		if (m_HasContoller)
		{
			m_Controller->Update(dt);
		}

		//PR_INFO("Camera rotation {0}, {1}", m_RotationDelta.x, m_RotationDelta.y);
	
		m_Position += m_DPosition;
		m_LookAt += m_DPosition;

		glm::vec3 axis = glm::cross(m_Direction, m_Up);
		glm::quat pitch = glm::angleAxis(m_RotationDelta.y, axis);
		glm::quat heading = glm::angleAxis(m_RotationDelta.x, m_Up);

		glm::quat rotation = glm::normalize(glm::cross(pitch, heading));

		m_Direction = glm::rotate(rotation, m_Direction);

		m_LookAt = m_Position + m_Direction;
		m_View = glm::lookAt(m_Position, m_LookAt, m_Up);
		
		m_ProjectedView = m_Projection * m_View;
		m_DPosition = { 0.f, 0.f, 0.f };
		m_Rotation += m_RotationDelta;
		m_RotationDelta = { 0.f, 0.f };
	}
}
