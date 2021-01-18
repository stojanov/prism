
#include "glm/glm.hpp"

#include "GLFW/glfw3.h"
#include "Controller.h"
#include "prism/Renderer/PerspectiveCamera.h"
#include "prism/Core/Events/MouseEvents.h"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/Core/Events/WindowEvents.h"
#include "prism/System/Log.h"

namespace Prism::Renderer
{
	template<typename T>
	class FPSCameraController : public CameraController
	{
	public:
		FPSCameraController(T* camera)
			:
			m_Camera(camera)
		{
		}
		virtual ~FPSCameraController() = default;

		void OnSystemEvent(Event& e) override
		{
			EventHandler evt(e);

			evt.Handle<MouseButtonDownEvent>([this](MouseButtonDownEvent& e)
			{
				m_Rotation += (e.GetPosition() - m_LastMousePosition) * m_MouseRotateSens;
				m_LastMousePosition = e.GetPosition();
			});
			
			evt.Handle<KeyDownEvent>([this](KeyDownEvent& e)
				{
					auto key = e.GetKey();
				
					if (key == Keyboard::W)
					{
						m_Position.x += 0.9;
					}
					if (key == Keyboard::S)
					{
						m_Position.x -= 0.9;
					}
					if (key == Keyboard::A)
					{
						m_Position.y += 0.9;
					}
					if (key == Keyboard::D)
					{
						m_Position.y -= 0.9;
					}
				});
			evt.Handle<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
				{
					m_LastMousePosition = e.GetPosition();
					m_IsMouseActive = true;
				});
			evt.Handle<MouseButtonReleasedEvent>([this](auto e)
				{
					m_IsMouseActive = false;
				});
			evt.Handle<KeyPressedEvent>([this](KeyPressedEvent& e)
				{
					m_ShouldRotate = e.GetKey() == Keyboard::LALT;
				});
			evt.Handle<KeyReleasedEvent>([this](KeyReleasedEvent& e)
				{
					m_ShouldRotate = e.GetKey() != Keyboard::LALT;
				});
			if constexpr (std::is_same<T, PerspectiveCamera>::value)
			{
				evt.Handle<WindowResizeEvent>([this](WindowResizeEvent& e)
					{
						m_Camera->UpdateRatio(e.GetWidth(), e.GetHeight());
					});
			}
		}

		void Update(float dt) override
		{
			m_Camera->SetVerticalRotation(m_Rotation.y * dt);
			m_Camera->SetHorizontalRotation(m_Rotation.x * dt);
			
			m_Camera->MoveZ(m_Position.x * dt);
			m_Camera->MoveX(m_Position.y * dt);
			
			m_Position = { 0.f, 0.f };
			m_Rotation = { 0.f, 0.f };
		}
	private:
		T* m_Camera;
		bool m_IsMouseActive{ false };
		float m_MouseMoveSens = 0.9;
		float m_MouseRotateSens = 0.3;
		glm::vec2 m_LastMousePosition{ 0.f, 0.f };
		glm::vec2 m_Rotation{ 0.f, 0.f };
		glm::vec2 m_Position{ 0.f, 0.f };
		bool m_ShouldRotate{ false };
		bool m_MouseDown{ false };
		bool m_ShouldMove{ false };
	};
}
