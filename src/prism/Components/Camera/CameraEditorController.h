#pragma once

#include "glm/glm.hpp"

#include "IController.h"
#include "prism/Renderer/PerspectiveCamera.h"
#include "prism/Core/Events/MouseEvents.h"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/Core/Events/WindowEvents.h"
#include "prism/System/Log.h"

namespace Prism::Renderer
{
	template<typename T>
	class CameraEditorController: public ICameraController
	{
	public:
		CameraEditorController(T* camera)
			:
			m_Camera(camera)
		{
		}
		virtual ~CameraEditorController() = default;

		void ShouldLock(bool lck) override
		{
			m_IsLocked = lck;
		}

		void SetMoveSpeed(float s) override
		{
			m_MouseMoveSens = s;
		}
		
		void OnSystemEvent(Event& e) override
		{
			if (m_IsLocked)
			{
				return;
			}
			EventHandler evt(e);

			CLASSEVENT(evt, MouseScrollEvent)
			{
				m_Position.y += e.GetPosition().y * m_MouseMoveSens * 25;
			});
			
			CLASSEVENT(evt, MouseButtonDownEvent)
			{
				const auto pos = e.GetPosition();
				
				if (e.GetKey() == Mouse::Button::SCROLL)
				{
					m_Position.x -= (pos.x - m_LastMousePosition.x) * m_MouseMoveSens;
					m_Position.z += (pos.y - m_LastMousePosition.y) * m_MouseMoveSens;
				} else
				{
					if (m_ShouldRotate)
					{
						m_Rotation.x += (pos.x - m_LastMousePosition.x) * m_MouseRotateSens;
						m_Rotation.y += (pos.y - m_LastMousePosition.y) * m_MouseRotateSens;
					}
				}

				m_LastMousePosition = e.GetPosition();
			});
			
			CLASSEVENT(evt, MouseButtonPressedEvent)
			{
				if (e.GetKey() == Mouse::Button::SCROLL)
				{
					m_IsMouseActive = true;
				}
				m_LastMousePosition = e.GetPosition();
			});
			CLASSEVENT(evt, MouseButtonReleasedEvent)
			{
				if (e.GetKey() == Mouse::Button::SCROLL)
				{
					m_IsMouseActive = false;
				}
			});
			CLASSEVENT(evt, KeyPressedEvent)
			{
				m_ShouldRotate = e.GetKey() == Keyboard::LALT;
			});
			CLASSEVENT(evt, KeyReleasedEvent)
			{
				m_ShouldRotate = e.GetKey() != Keyboard::LALT;
			});
			
			if constexpr (std::is_same<T, PerspectiveCamera>::value)
			{
				CLASSEVENT(evt, WindowResizeEvent)
				{
					m_Camera->UpdateRatio(e.GetWidth(), e.GetHeight());
				});
			}
		}

		void ResetDelta() override
		{
			
		}
		
		void Update(float dt) override
		{
			m_Camera->OffsetRotation(m_Rotation * dt);
			m_Camera->MoveX(m_Position.x * dt);
			m_Camera->MoveZ(m_Position.y * dt);
			m_Camera->MoveY(m_Position.z * dt);

			m_Position = { 0.f, 0.f, 0.f };
			m_Rotation = { 0.f, 0.f };
		}
	private:
		T* m_Camera;
		bool m_IsLocked{ false };
		bool m_IsMouseActive{ false };
		float m_MouseMoveSens = 0.9;
		float m_MouseRotateSens = 0.3;
		glm::vec2 m_LastMousePosition{ 0.f, 0.f };
		glm::vec2 m_Rotation{ 0.f, 0.f};
		glm::vec3 m_Position{ 0.f, 0.f, 0.f };
		bool m_ShouldRotate{ false };
		bool m_MouseDown{ false };
		bool m_ShouldMove{ false };
	};
}
