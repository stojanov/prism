#include "PerspectiveCamera.h"

#include <type_traits>

#include "CameraController.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "prism/System/Debug.h"

namespace Prism::Renderer
{
	PerspectiveCamera::PerspectiveCamera(float FOV, int width, int height, float clipPlaneNear, float clipPlaneFar)
	{
		m_Projection = glm::perspective(glm::radians(FOV), width * 1.0f / height, clipPlaneNear, clipPlaneFar);
	}

	void PerspectiveCamera::MoveVertically(float speed)
	{
		m_DPosition += m_Up * speed;
	}

	void PerspectiveCamera::MoveHorizontally(float speed)
	{
		m_DPosition += glm::normalize(glm::cross(m_Direction, m_Up)) * speed;
	}
	
	void PerspectiveCamera::OffsetXPosition(float x) 
	{
		m_Position.x += x;
	}

	void PerspectiveCamera::OffsetYPosition(float y)
	{
		m_Position.y += y;
	}

	void PerspectiveCamera::OffsetZPosition(float z)
	{
		m_Position.z += z;
	}
	
	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}
	
	void PerspectiveCamera::OffsetPosition(const glm::vec3& position)
	{
		m_Position += position;
	}

	void PerspectiveCamera::SetVerticalRotation(float angle)
	{
		m_RotationDelta.x = angle;
	}

	void PerspectiveCamera::OffsetVerticalRotation(float angle)
	{
		m_RotationDelta.x += angle;
	}

	void PerspectiveCamera::SetHorizontalRotation(float angle)
	{
		m_RotationDelta.y = angle;
	}

	void PerspectiveCamera::OffsetHorizontalRotation(float angle)
	{
		m_RotationDelta.y += angle;
	}

	void PerspectiveCamera::OnSystemEvent(Event& e)
	{
		if (m_HasContoller)
		{
			m_Controller->OnSystemEvent(e);
		}	
	}
	
	void PerspectiveCamera::Update()
	{
		if (m_HasContoller)
		{
			m_Controller->Update(0.013);
		}

		m_Position += m_DPosition;
		PR_WARN("{0},{1},{2} {3},{4},{5}", m_DPosition.x, m_DPosition.y, m_DPosition.z, m_Position.x, m_Position.y, m_Position.z);
		m_Direction = glm::normalize(m_LookAt - m_Position);

		glm::vec3 axis = glm::cross(m_Direction, m_Up);
		glm::quat pitch = glm::angleAxis(m_RotationDelta.x, axis);
		glm::quat heading = glm::angleAxis(m_RotationDelta.y, m_Up);

		glm::quat rotation = glm::normalize(glm::cross(pitch, heading));

		m_Direction = glm::rotate(rotation, m_Direction);
		m_Up = glm::rotate(rotation, m_Up);
		
		m_LookAt = m_Position + m_Direction;
		PR_ERROR("{0},{1},{2} {3},{4},{5}", m_Position.x, m_Position.y, m_Position.z, m_Direction.x, m_Direction.y, m_Direction.z);
		m_View = glm::lookAt(m_Position, m_LookAt, m_Up);
		
		m_ProjectedView = m_Projection * m_View;

		//m_RotationDelta = { 0.f, 0.f }; 
		m_DPosition = { 0.f, 0.f, 0.f };
	}
}
