#pragma once
#include <type_traits>
#include <glm.hpp>

#include "prism/Interfaces/Camera/ICamera.h"
#include "prism/Interfaces/Camera/IController.h"
#include "prism/Core/Pointers.h"
#include "prism/System/Debug.h"

namespace Prism::Renderer
{
	class PerspectiveCamera: IBaseCamera
	{
	public:
		PerspectiveCamera();
		PerspectiveCamera(float FOV, int width, int height, float clipPlaneNear, float clipPlaneFar);
		virtual ~PerspectiveCamera();
		
		void MoveX(float speed);
		void MoveY(float speed);
		void MoveZ(float speed);

		void ShouldLock(bool lck);

		void SetLookAt(const glm::vec3& LookAt);
		
		void OffsetPosition(const glm::vec3& position) override;
		void SetPosition(const glm::vec3& position) override;

		void UpdatePerspective(float fov, int width, int height, float clipPlaneNear, float clipPlaneFar);
		void UpdateRatio(int width, int height);

		void OffsetRotation(const glm::vec2& rot) override;
		void Rotate(const glm::vec2& rot) override;
		
		void OnSystemEvent(Event& e) override;
		void OnUpdate(float dt);

		glm::vec3& GetPosition()
		{
			return m_Position;
		}
		
		const std::unique_ptr<ICameraController>& GetController()
		{
			if (m_HasContoller)
			{
				return m_Controller;
			}
		}
		
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ICameraController, T>>>
		void AttachController()
		{
			PR_ASSERT(!m_HasContoller, "Camera can only have 1 Controller!");
			m_HasContoller = true;
			m_Controller = DynamicPtrCast<ICameraController>(MakePtr<T>(this));
		}

		void RemoveController()
		{
			PR_ASSERT(m_HasContoller, "Camera must have a controller to be removed");
			m_HasContoller = false;
			m_Controller.reset();
		}
		const glm::mat4& GetProjectedView() const { return m_ProjectedView; }
		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
	private:

		void _RotateCamera();
		
		bool m_HasContoller{ false };
		Ptr<ICameraController> m_Controller;
		glm::mat4 m_View;
		glm::mat4 m_Projection;
		glm::mat4 m_ProjectedView;
		float m_Fov;
		float m_ClipNear;
		float m_ClipFar;
		glm::vec3 m_DPosition{ 0.f, 0.f, 0.f };
		glm::vec2 m_RotationDelta{ 0.f, 0.f };
		glm::vec2 m_Rotation{ 0.f, 0.f };
		glm::vec3 m_Position{ 0.f, 0.f, -2.f };
		glm::vec3 m_LookAt{ 0.f, 0.f, 0.f };
		glm::vec3 m_Up{ 0.f, 1.f, 0.f };
		glm::vec3 m_Direction{ 0.f, 0.f, 1.f};
	};
}
