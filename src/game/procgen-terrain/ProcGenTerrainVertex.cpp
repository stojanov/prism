#include "ProcGenTerrainVertex.h"

#include "ext/scalar_constants.hpp"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/Core/Events/WindowEvents.h"
#include "prism/Interfaces/Camera/FPSCameraController.h"

using namespace Prism;

ProcGenTerrainVertex::ProcGenTerrainVertex(Core::SharedContextRef ctx, const std::string& name)
	:
	ILayer(ctx, name)
{

}

ProcGenTerrainVertex::~ProcGenTerrainVertex()
{

}

void ProcGenTerrainVertex::OnAttach()
{
	m_Ctx->Tasks->GetWorker("glcontext")->QueueTask([this]()
		{
			m_TerrainGPU.CreateMesh();
		});

	if (!m_Ctx->Assets.Shaders->LoadAsset("gpu-procgen-terrain",
		{ "res/gpu-procgen-terrain.vert", "res/gpu-procgen-terrain.frag" }
	))
	{
		m_Shader = m_Ctx->Assets.Shaders->Get("gpu-procgen-terrain");
		PR_INFO("Procgen terrain GPU Shader Loaded");
	}

	{
		const auto& ctrl = m_Camera.AttachController<Renderer::FPSCameraController<Renderer::PerspectiveCamera>>();
		ctrl->SetMoveSpeed(m_CameraMoveSpeed);
		ctrl->SetRotationSpeed(m_CameraRotationSpeed);
	}

	m_Camera.SetPosition({ 0.f, 3.f, 0.f });
	m_Camera.SetLookAt({ 0.f, 0.f, 0.f });

	float height = 100.f;
	// sin(10(x^2+y^2))/10

	/*
	m_Modes.resize(m_MaxNoiseLayers, { 0 });
	m_UsingSimplex.resize(m_MaxNoiseLayers, { false });
	m_Scales.resize(m_MaxNoiseLayers, { 0.1f });
	m_ScaleMultipliers.resize(m_MaxNoiseLayers, { 1.f });
	m_OffsetsX.resize(m_MaxNoiseLayers, { 0.f });
	m_OffsetsY.resize(m_MaxNoiseLayers, { 0.f });
	*/
	m_NoiseLayerCount = 0;
}

void ProcGenTerrainVertex::OnDetach()
{

}

void ProcGenTerrainVertex::OnSystemEvent(Event& e)
{
	EventHandler handler(e);

	m_Camera.OnSystemEvent(e);

	handler.Handle<WindowResizeEvent>([this](WindowResizeEvent& evt)
		{
			m_Camera.UpdatePerspective(90, evt.GetWidth(), evt.GetHeight(), 0.1f, 2048.f);
		});

	handler.Handle<KeyPressedEvent>([this](KeyPressedEvent& evt)
		{
			switch (evt.GetKey())
			{
			case Keyboard::F2:
				if (m_CameraLocked)
				{
					m_Ctx->SystemOptions->DisableCursor();
				}
				else
				{
					m_Ctx->SystemOptions->EnableCursor();
				}
				m_CameraLocked = !m_CameraLocked;
				break;

			}
		});
}

void ProcGenTerrainVertex::OnUpdate(float dt)
{
	m_Camera.ShouldLock(m_CameraLocked);
	m_Camera.OnUpdate(dt);
}

void ProcGenTerrainVertex::OnGuiDraw()
{
	auto& io = ImGui::GetIO();

	ImGui::BeginMainMenuBar();

	ImGui::MenuItem("Info", 0, &m_ShowInfo);
	ImGui::MenuItem("Shader", 0, &m_ShaderOptions);

	ImGui::EndMainMenuBar();

	if (m_ShowInfo)
	{
		ImGui::Begin("Options");
		ImGui::Text("Use (WASD) to move around the world");
		ImGui::Text("Use (F2) to active the camera move");
		ImGui::Text("Toggle Wireframe = F1");
		ImGui::Text("Toggle Camera = F2");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		auto& pos = m_Camera.GetPosition();
		auto& rot = m_Camera.GetRotation();
		ImGui::Text("Camera Position: (%.2f %.2f %.2f)", pos.x, pos.y, pos.z);
		ImGui::Text("Camera Rotaton: (%.2f %.2f)", rot.x, rot.y);

		ImGui::End();
	}

	if (m_ShaderOptions)
	{
		ImGui::Begin("GPU Render Options");

		ImGui::SliderFloat("Elevation", &m_Elevation, 0, 100);

		if (ImGui::Button("Add Noise Layer") && m_NoiseLayerCount < m_MaxNoiseLayers)
		{
			m_Modes[m_NoiseLayerCount] = 0;
			m_UsingSimplex[m_NoiseLayerCount] = false;
			m_Scales[m_NoiseLayerCount] = 0.1f;
			m_ScaleMultipliers[m_NoiseLayerCount] = 1.f;
			m_OffsetsX[m_NoiseLayerCount] = 0.f;
			m_OffsetsY[m_NoiseLayerCount] = 0.f;
			m_NoiseLayerCount++;
		}

		for (int i = 0; i < m_NoiseLayerCount; i++)
		{
			char noiseTitle[100];
			char scale[20];
			char scalemulti[20];
			char offsetx[20];
			char offsety[20];
			char rmbtn[20];

			sprintf_s(noiseTitle, "Noise Layer: %d", i);
			sprintf_s(scale, "Scale : %d", i);
			sprintf_s(scalemulti, "Scale Multi: %d", i);
			sprintf_s(offsetx, "Offset X: %d", i);
			sprintf_s(offsety, "Offset Y: %d", i);
			sprintf_s(rmbtn, "Remove Layer %d", i);

			if (ImGui::CollapsingHeader(noiseTitle))
			{
				ImGui::Text("Currently using %s", m_UsingSimplex[i] ? "Simplex Noise" : "Value Noise");
				ImGui::SameLine(0.f, 0.f);
				if (ImGui::Button(m_UsingSimplex[i] ? "Value Noise" : "Simplex Noise"))
				{
					m_UsingSimplex[i] = m_UsingSimplex[i] == 0 ? 1 : 0;
				}
				ImGui::SliderFloat(scale, &m_Scales[i], 0, 1);
				ImGui::SliderFloat(scalemulti, &m_ScaleMultipliers[i], 0, 1);
				ImGui::SliderFloat(offsetx, (float*)&m_OffsetsX[i], -50, 50);
				ImGui::SliderFloat(offsety, (float*)&m_OffsetsY[i], -50, 50);

				if (ImGui::Button(rmbtn))
				{
					for (int j = i; j < m_NoiseLayerCount - 1; j++)
					{
						m_Modes[j] = m_Modes[j + 1];
						m_UsingSimplex[j] = m_UsingSimplex[j + 1];
						m_Scales[j] = m_Scales[j + 1];
						m_ScaleMultipliers[j] = m_ScaleMultipliers[j + 1];
						m_OffsetsX[j] = m_OffsetsX[j + 1];
						m_OffsetsY[j] = m_OffsetsY[j + 1];
					}

					m_NoiseLayerCount--;
					break;
				}
			}
		}

		ImGui::End();
	}
}
 
void ProcGenTerrainVertex::OnDraw()
{
	m_Shader->Bind();
	m_Shader->SetInt("layer_count", m_NoiseLayerCount);
	m_Shader->SetIntV("mode", m_MaxNoiseLayers, m_Modes);
	m_Shader->SetIntV("using_simplex", m_MaxNoiseLayers, m_UsingSimplex);
	m_Shader->SetFloatV("noise_scale", m_MaxNoiseLayers, m_Scales);
	m_Shader->SetFloatV("noise_scale_m", m_MaxNoiseLayers, m_ScaleMultipliers);
	m_Shader->SetFloatV("noise_offset_x", m_MaxNoiseLayers,m_OffsetsX);
	m_Shader->SetFloatV("noise_offset_y", m_MaxNoiseLayers, m_OffsetsY);
	m_Shader->SetFloat("elevation", m_Elevation);

	m_Shader->SetMat4("transform", m_TerrainGPU.GetTransform());
	m_Shader->SetMat4("projectedView", m_Camera.GetProjectedView());
	m_TerrainGPU.Render();
}


