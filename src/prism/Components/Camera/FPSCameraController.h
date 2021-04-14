#pragma once
#include "glm/glm.hpp"

#include "IController.h"
#include "prism/Renderer/PerspectiveCamera.h"
#include "prism/Core/Events/MouseEvents.h"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/Core/Events/WindowEvents.h"

namespace Prism::Renderer
{
	template<typename T>
	class FPSCameraController : public ICameraController
	{
	public:
		FPSCameraController(T* camera)
			:
			m_Camera(camera)
		{
		}
		virtual ~FPSCameraController() = default;

		void ShouldLock(bool lck) override
		{
			m_IsLocked = lck;
		}
		
		void OnSystemEvent(Event& e) override
		{
			if (m_IsLocked)
			{
				return;
			}
			EventHandler evt(e);
			
			evt.Handle<MouseMoveEvent>([this](MouseMoveEvent& e)
			{
				if (m_ResetDelta)
				{
					m_LastMousePosition = e.GetPosition();
					m_ResetDelta = false;
				}
				auto rate = (e.GetPosition() - m_LastMousePosition) * m_MouseRotateSens;
				
				m_Rotation -= glm::clamp(rate, -1.f * m_MaxRotationRate, m_MaxRotationRate);
				m_LastMousePosition = e.GetPosition();
				//PR_INFO("Camera Rotation: {0}, {1}", m_Rotation.x, m_Rotation.y);
			});
			
			evt.Handle<KeyDownEvent>([this](KeyDownEvent& e)
				{
					auto key = e.GetKey();
				
					if (key == Keyboard::W)
					{
						m_Position.x += m_MoveSpeed;
					}
					if (key == Keyboard::S)
					{
						m_Position.x -= m_MoveSpeed;
					}
					if (key == Keyboard::A)
					{
						m_Position.y -= m_MoveSpeed;
					}
					if (key == Keyboard::D)
					{
						m_Position.y += m_MoveSpeed;
					}
				});
			if constexpr (std::is_same_v<T, PerspectiveCamera>)
			{
				evt.Handle<WindowResizeEvent>([this](WindowResizeEvent& e)
					{
						m_Camera->UpdateRatio(e.GetWidth(), e.GetHeight());
					});
			}
		}

		void SetMoveSpeed(float s) override
		{
			m_MoveSpeed = s;
		}

		void ResetDelta() override
		{
			m_ResetDelta = true;
		}
		
		void Update(float dt) override
		{
			m_Camera->OffsetRotation(m_Rotation * dt);
			
			m_Camera->MoveZ(m_Position.x * dt);
			m_Camera->MoveX(m_Position.y * dt);
			
			m_Position = { 0.f, 0.f };
			m_Rotation = { 0.f, 0.f };
		}
	private:
		T* m_Camera;
		bool m_ResetDelta{ false };
		bool m_IsLocked{ false };
		bool m_IsMouseActive{ false };
		float m_MoveSpeed = 0.9;
		float m_MouseRotateSens = 0.3;
		glm::vec2 m_LastMousePosition{ 0.f, 0.f };
		glm::vec2 m_Rotation{ 0.f, 0.f };
		glm::vec2 m_Position{ 0.f, 0.f };
		glm::vec2 m_MaxRotationRate{ 35.f, 35.f };
		bool m_ShouldRotate{ false };
		bool m_MouseDown{ false };
		bool m_ShouldMove{ false };
	};
}
