#pragma once
#include <type_traits>

#include "Camera.h"
#include "CameraController.h"
#include "glm/glm.hpp"
#include "prism/System/Debug.h"

namespace Prism::Renderer
{
	class PerspectiveCamera: BaseCamera
	{
	public:
		PerspectiveCamera();
		PerspectiveCamera(float FOV, int width, int height, float clipPlaneNear, float clipPlaneFar);
		virtual ~PerspectiveCamera() = default;

		void MoveHorizontally(float speed) override;
		void MoveVertically(float speed) override;
		
		void OffsetXPosition(float x) override;
		void OffsetYPosition(float y) override;
		void OffsetZPosition(float z) override;
		void OffsetPosition(const glm::vec3& position) override;
		void SetPosition(const glm::vec3& position) override;

		void SetVerticalRotation(float angle) override;
		void SetHorizontalRotation(float angle) override;
		void OffsetVerticalRotation(float angle) override;
		void OffsetHorizontalRotation(float angle) override;

		void OnSystemEvent(Event& e) override;
		
		void Update();

		template<typename T, typename = typename std::enable_if<std::is_base_of<CameraController, T>::value>::type>
		void AttachController()
		{
			PR_ASSERT(!m_HasContoller, "Camera can only have 1 Controller!");
			m_HasContoller = true;
			//m_Controller = DynamicPtrCast<Controller>(MakePtr<T>(this));
			m_Controller = dynamic_cast<CameraController*>(new T(this));
		}

		const glm::mat4& GetProjectedView() const { return m_ProjectedView; }
		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
	private:
		bool m_HasContoller{ false };
		CameraController* m_Controller;
		glm::mat4 m_View;
		glm::mat4 m_Projection;
		glm::mat4 m_ProjectedView;

		glm::vec3 m_DPosition{ 0.f, 0.f, 0.f };
		glm::vec2 m_RotationDelta{ 0.f, 0.f };
		glm::vec3 m_Position{ 0.f, 0.f, -3.f };
		glm::vec3 m_LookAt{ 0.f, 0.f, 5.f };
		glm::vec3 m_Up{ 0.f, 1.f, 0.f };
		glm::vec3 m_Direction;
	};
}
