#pragma once

#include "glm/glm.hpp"
#include "CameraController.h"
#include "prism/Core/Events/MouseEvents.h"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/System/Log.h"

namespace Prism::Renderer
{
	template<typename T>
	class CameraEditorController: public CameraController
	{
	public:
		CameraEditorController(T* camera)
			:
			m_Camera(camera)
		{
			
		}

		void OnSystemEvent(Event& e) override
		{
			EventHandler evt(e);

			evt.Handle<MouseButtonDownEvent>([this](MouseButtonDownEvent& e) 
			{
				if (m_ShouldRotate)
				{
					auto dr = (e.GetPosition() - m_LastMousePosition) * m_MouseRotateSens;
					m_Rotation += (e.GetPosition() - m_LastMousePosition) * m_MouseRotateSens;
					//PR_WARN("ROTATING {0},{1} {2},{3}", m_Rotation.x, m_Rotation.y, dr.x, dr.y);
				} else
				{
					//PR_ERROR("MOVING");
					m_Position += (e.GetPosition() - m_LastMousePosition) * m_MouseMoveSens;
				}
				
				m_LastMousePosition = e.GetPosition();
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
				m_ShouldRotate = !(e.GetKey() == Keyboard::LALT);
			});
		}
		
		void Update(float dt) override
		{
			m_Camera->SetVerticalRotation(m_Rotation.y * dt);
			m_Camera->SetHorizontalRotation(m_Rotation.x * dt);
			m_Camera->MoveHorizontally(m_Position.x * dt);
			m_Camera->MoveVertically(m_Position.y * dt);

			m_Position = { 0.f, 0.f };
			m_Rotation = { 0.f, 0.f };
		}
	private:
		T* m_Camera;
		bool m_IsMouseActive{ false };
		float m_MouseMoveSens = 0.9;
		float m_MouseRotateSens = 0.3;
		glm::vec2 m_LastMousePosition{ 0.f, 0.f };
		glm::vec2 m_Rotation{ 0.f, 0.f};
		glm::vec2 m_Position{ 0.f, 0.f };
		bool m_ShouldRotate{ false };
		bool m_MouseDown{ false };
		bool m_ShouldMove{ false };
	};
}
