#include "VoxelExample.h"


#include <ext/matrix_transform.hpp>
#include "prism/Interfaces/Camera/CameraEditorController.h"
#include "prism/Interfaces/Camera/FPSCameraController.h"
#include "prism/System/ScopeTimer.h"

namespace Prism::Examples
{
	VoxelExample::VoxelExample(Core::SharedContextRef ctx, const std::string& name)
		:
		ILayer(ctx, name)
	{
		
	}

	VoxelExample::~VoxelExample()
	{
		
	}

	void VoxelExample::OnAttach()
	{
		m_Camera.SetPosition({ 100.f, 85.f, 0.f });
		m_Camera.OffsetRotation({ 0.f, 85.f });
		
		m_Camera.AttachController<Renderer::FPSCameraController<Renderer::PerspectiveCamera>>();
		m_Camera.GetController()->SetMoveSpeed(m_CameraMoveSpeed);
		m_Ctx->Assets.Shaders->LoadAsset("baseshader", { "res/voxel.vert", "res/voxel.frag" });
		m_Shader = m_Ctx->Assets.Shaders->Get("baseshader");

		m_Noise.setScale(0.025);
		
		m_ChunkManager.PopulationFunction([this](int x, int y)
			{
				return (m_Noise.Fractal2(x, y) + 1) / 2;
			});

		m_ChunkManager.SetRadius(20);
		
		m_CameraLocked = true;
	}

	void VoxelExample::OnDetach()
	{

	}

	void VoxelExample::OnSystemEvent(Event& e)
	{
		EventHandler handler(e);

		m_Camera.OnSystemEvent(e);
		CLASSEVENT(handler, KeyReleasedEvent)
		{
			switch (e.GetKey())
			{
			case Keyboard::LSHIFT:
				m_CameraMoveSpeedMulti = 1.f;
				break;
			}
		});
		CLASSEVENT(handler, KeyPressedEvent)
		{
			switch (e.GetKey())
			{
			case Keyboard::LSHIFT:
				m_CameraMoveSpeedMulti = 3.f;
				break;
			case Keyboard::F2:
				if (m_CameraLocked)
				{
					m_Camera.GetController()->ResetDelta();
					m_Ctx->SystemOptions->DisableCursor();
					m_CameraLocked = !m_CameraLocked;
				} else
				{
					m_CameraLocked = !m_CameraLocked;
					m_Ctx->SystemOptions->EnableCursor();
				}
				break;
			}
		});
	}

	void VoxelExample::OnGuiDraw()
	{
		auto io = ImGui::GetIO();
		m_CursorOverGui = io.WantCaptureMouse;
		
		ImGui::BeginMainMenuBar();
		ImGui::MenuItem("Controls", 0, &m_ShowBaseCtrls);
		ImGui::MenuItem("Options", 0, &m_ShowDescripton);
		ImGui::EndMainMenuBar();
		
		if (m_ShowDescripton)
		{
			ImGui::Begin("Options");
			ImGui::SliderFloat("Mouse Sensitivty ", &m_CameraRotationSpeed, 0, 1);
			ImGui::SliderFloat("Camera Move Speed", &m_CameraMoveSpeed, 0, 100);
			ImGui::Text("Toggle Wireframe = F1");
			ImGui::Text("Toggle Camera = F2");
			ImGui::End();
		}

		if (m_ShowBaseCtrls)
		{
			ImGui::Begin("Controls");
			ImGui::SliderFloat3("Light Position", (float*)&m_LightPosition, -200, 200);
			ImGui::ColorEdit3("Light Color", (float*)&m_LightClr);
			ImGui::SliderFloat("Light Intensity", &m_LightIntensity, 0, 5);
			ImGui::End();	
		}
	}

	void VoxelExample::OnUpdate(float dt)
	{
		m_Camera.ShouldLock(m_CameraLocked || m_CursorOverGui);
		m_Camera.OnUpdate(dt);
		m_Camera.GetController()->SetMoveSpeed(m_CameraMoveSpeed * m_CameraMoveSpeedMulti);
		m_Camera.GetController()->SetRotationSpeed(m_CameraRotationSpeed);
		m_ChunkManager.MoveXY(m_Camera.GetPosition());
	}

	void VoxelExample::OnDraw()
	{
		for (auto& [pos, chunk] : m_ChunkManager)
		{
			if (!chunk->MeshReady())
			{
				continue;
			}

			chunk->SendToGpu();
			m_Shader->Bind();
			m_Shader->SetMat4("transform", chunk->GetTransform());
			m_Shader->SetInt("tex", 0);
			m_Shader->SetFloat3("lightPos", m_LightPosition);
			m_Shader->SetFloat("lightIntens", m_LightIntensity);
			m_Shader->SetFloat3("lightClr", m_LightClr);
			m_Shader->SetMat4("projectedview", m_Camera.GetProjectedView());
			chunk->Render();
		}
	}
}
