#include "ProcGenTerrain.h"

#include "ext/scalar_constants.hpp"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/Core/Events/WindowEvents.h"
#include "prism/Interfaces/Camera/FPSCameraController.h"

using namespace Prism;

ProcGenTerrain::ProcGenTerrain(Core::SharedContextRef ctx, const std::string& name)
	:
	ILayer(ctx, name)
{
	
}

ProcGenTerrain::~ProcGenTerrain()
{

}

void ProcGenTerrain::OnAttach()
{
	m_Ctx->Tasks->GetWorker("glcontext")->QueueTask([this]()
	{
		m_TerrainCPU.CreateMesh(); 
		m_TerrainGPU.CreateMesh();
	}); 

	if (!m_Ctx->Assets.Shaders->LoadAsset("procgen-terrain",
		{ "res/procgen-terrain.vert", "res/procgen-terrain.frag" }
	))
	{
		m_ShaderCPU = m_Ctx->Assets.Shaders->Get("procgen-terrain");
		PR_INFO("Procgen terrain CPU Shader Loaded");
	}

	if (!m_Ctx->Assets.Shaders->LoadAsset("gpu-procgen-terrain",
		{ "res/gpu-procgen-terrain.vert", "res/gpu-procgen-terrain.frag" }
	))
	{
		m_ShaderGPU = m_Ctx->Assets.Shaders->Get("gpu-procgen-terrain");
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

	m_Noise.SetScale(0.004);

	m_TerrainCPU.SetHeightFunc([this, height](int x, int y, int w, int h)
		{
			static constexpr float pi = glm::pi<float>();

			x -= w / 2;
			y -= h / 2;

			float dX = (x * 1.f / w) * pi;
			float dY = (y * 1.f / h) * pi;

			float z = (glm::sin(10 * ((dX * dX) + (dY * dY))) / 10) * height;

			return z;
			//return m_Noise.Fractal2(x, y) * height;
		});

	m_TerrainCPU.BakeMap();
}

void ProcGenTerrain::OnDetach()
{
	
}

void ProcGenTerrain::OnSystemEvent(Event& e)
{
	EventHandler handler(e);

	m_Camera.OnSystemEvent(e);

	handler.Handle<WindowResizeEvent>([this](WindowResizeEvent& evt)
	{
		m_Camera.UpdatePerspective(90, evt.GetWidth(), evt.GetHeight(), 0.1f, 2048.f);
	});

	handler.Handle<KeyPressedEvent>([this] (KeyPressedEvent& evt)
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

void ProcGenTerrain::OnUpdate(float dt)
{
	m_Camera.ShouldLock(m_CameraLocked);
	m_Camera.OnUpdate(dt);
}

void ProcGenTerrain::OnGuiDraw()
{
	auto& io = ImGui::GetIO();

	ImGui::BeginMainMenuBar();
	if (ImGui::Button("Render GPU"))
	{
		m_RenderingGPU = !m_RenderingGPU;
	}
	ImGui::MenuItem("Info", 0, &m_ShowInfo);
	ImGui::MenuItem("Shader", 0, &m_ShowCPUOptions);
	
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

	if (m_RenderingGPU)
	{
		ImGui::Begin("GPU Render Options");

		ImGui::SliderFloat("Elevation", &m_GPUElevation, 0, 100);
		 
		if (ImGui::CollapsingHeader("Noise options"))
		{
			ImGui::Text("Currently using %s", m_GPUUsingSimplex ? "Simplex Noise" : "Value Noise");
			ImGui::SameLine();
			if (ImGui::Button(m_GPUUsingSimplex ? "Value Noise" : "Simplex Noise"))
			{
				m_GPUUsingSimplex = !m_GPUUsingSimplex;
			}
			ImGui::SliderInt("Octaves", &m_GPUOctaves, 1, 10);
			ImGui::SliderFloat("Scale", &m_GPUScale, 0, 1);
			ImGui::SliderFloat("Scale Multiplier", &m_GPUScaleMultiplier, 0, 1);
			ImGui::SliderFloat("Persistance", &m_GPUPersistance, 0, 1);
			ImGui::SliderFloat2("Offset", (float*)&m_GPUOffset, -50, 50);
		}

		ImGui::End();
	}

	if (!m_RenderingGPU)
	{
		ImGui::Begin("CPU Render Options");


		ImGui::End();
	}
}

void ProcGenTerrain::OnDraw()
{
	if (!m_RenderingGPU)
	{
		m_ShaderCPU->Bind();
		m_ShaderCPU->SetFloat("height", 25.f);
		m_ShaderCPU->SetMat4("transform", m_TerrainCPU.GetTransform());
		m_ShaderCPU->SetMat4("projectedView", m_Camera.GetProjectedView());
		m_TerrainCPU.Render();
	}
	else
	{
		m_ShaderGPU->Bind();
		m_ShaderGPU->SetInt("using_simplex", m_GPUUsingSimplex);
		m_ShaderGPU->SetFloat("noise_persistence", m_GPUPersistance);
		m_ShaderGPU->SetInt("noise_octaves", m_GPUOctaves);
		m_ShaderGPU->SetFloat("noise_scale", m_GPUScale * m_GPUScaleMultiplier);
		m_ShaderGPU->SetFloat2("noise_offset", m_GPUOffset);
		m_ShaderGPU->SetFloat("elevation", m_GPUElevation);

		m_ShaderGPU->SetMat4("transform", m_TerrainGPU.GetTransform());
		m_ShaderGPU->SetMat4("projectedView", m_Camera.GetProjectedView());
		m_TerrainGPU.Render();
	}
}


