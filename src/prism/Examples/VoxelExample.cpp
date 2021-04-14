#include "VoxelExample.h"


#include "glm/ext/matrix_transform.hpp"
#include "prism/Components/Camera/CameraEditorController.h"
#include "prism/Components/Camera/FPSCameraController.h"
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
		m_Camera.OffsetRotation({ 0.f, 45.f });
		
		m_Camera.AttachController<Renderer::FPSCameraController<Renderer::PerspectiveCamera>>();
		m_Camera.GetController()->SetMoveSpeed(32);
		m_Ctx->Assets.Shaders->LoadAsset("baseshader", { "res/voxel.vert", "res/voxel.frag" });
		//m_Ctx->Assets.Textures->LoadAsset("dirt", { "res/dirt.jpg "});
		m_Shader = m_Ctx->Assets.Shaders->Get("baseshader");

		m_Noise.setScale(0.025);
		
		m_ChunkManager.PopulationFunction([this](int x, int y)
			{
				return (m_Noise.Fractal2(x, y) + 1) / 2;
			});

		m_ChunkManager.SetRadius(5);
		m_ChunkManager.Initial({0, 0});
		
		m_CameraLocked = true;
	}

	void VoxelExample::OnDetach()
	{

	}

	void VoxelExample::OnSystemEvent(Event& e)
	{
		EventHandler handler(e);

		m_Camera.OnSystemEvent(e);
		CLASSEVENT(handler, KeyPressedEvent)
		{
			switch (e.GetKey())
			{
			case Keyboard::P:
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
		ImGui::MenuItem("Chunk Controls", 0, &m_ShowChunkCtrls);
		ImGui::MenuItem("Controls", 0, &m_ShowBaseCtrls);
		ImGui::EndMainMenuBar();

		
		if (m_ShowChunkCtrls)
		{
			ImGui::Begin("Chunks");
			for (auto& chunk : m_ChunkData)
			{
				ImGui::BulletText("%.2f\t\t%.2f\t\t%.2f", chunk.offset.x, chunk.offset.y, chunk.offset.z);
			}
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
