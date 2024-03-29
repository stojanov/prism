#include "VoxelExample.h"


#include <ext/matrix_transform.hpp>
#include "prism/Interfaces/Camera/CameraEditorController.h"
#include "prism/Interfaces/Camera/FPSCameraController.h"
#include "prism/System/ScopeTimer.h"

namespace Prism::Examples
{
	class MainMapper : public Voxel::ChunkMapper
	{
	public:
		MainMapper(Ref<Renderer::TextureAtlas> textureAtlas)
			: m_Atlas(std::move(textureAtlas))
		{
			
		}

		void Map(const MappingData& data, int x, int y, int height, FaceSide side) override
		{
			glm::vec3 clr = { 1.f, 1.f, 0.f };

			data.ColorFn(clr);

			auto sample = data.MaxHeight / 3;

			if (height > sample * 1.2)
			{
				if (side == FaceSide::TOP)
				{
					data.TextureCoordFn(m_Atlas->GetTextureVec(0, 0));
				}
				else
				{
					data.TextureCoordFn(m_Atlas->GetTextureVec(3, 0));
				}
			}
			else if (height <= sample * 1.2 && height > sample)
			{
				data.TextureCoordFn(m_Atlas->GetTextureVec(2, 0));
			}
			else
			{
				data.TextureCoordFn(m_Atlas->GetTextureVec(3, 4));
			}
		}

	private:
		Ref<Renderer::TextureAtlas> m_Atlas;
	};

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
		m_Camera.SetPosition({ 100.f, 320.f, 0.f });
		m_Camera.OffsetRotation({ 0.f, 85.f });

		m_Camera.AttachController<Renderer::FPSCameraController<Renderer::PerspectiveCamera>>();
		m_Camera.GetController()->SetMoveSpeed(m_CameraMoveSpeed);


		m_Ctx->assets.Shaders->LoadAsset("baseshader", { "res/voxel.vert", "res/voxel.frag" });
		m_Ctx->assets.Textures->LoadAsset("voxel-texture", { "res/voxel-texture-atlas-2.png" });

		m_Shader = m_Ctx->assets.Shaders->Get("baseshader");
		m_VoxelAtlas = MakeRef<Renderer::TextureAtlas>(m_Ctx->assets.Textures->Get("voxel-texture"));

		m_VoxelAtlas->SetInstanceSize({ 16.f,16.f });

		m_Noise.SetScale(m_NoiseScale);
		m_Noise.SetPersistance(m_Persistance);

		m_Noise.SetOctaves(m_Octaves);
		m_ChunkManager.AttachChunkMapper(MakeRef<MainMapper>(m_VoxelAtlas));

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
					m_Ctx->systemOptions->DisableCursor();
					m_CameraLocked = !m_CameraLocked;
				} else
				{
					m_CameraLocked = !m_CameraLocked;
					m_Ctx->systemOptions->EnableCursor();
				}
				break;
			}
		});

        CLASSEVENT(handler, WindowResizeEvent)
        {
            m_Camera.UpdatePerspective(90, e.GetWidth(), e.GetHeight(), 0.1f, 2048.f);
        });
	}

	void VoxelExample::OnGuiDraw()
	{
		auto io = ImGui::GetIO();
		m_CursorOverGui = io.WantCaptureMouse;
		
		ImGui::BeginMainMenuBar();
		ImGui::MenuItem("Controls", 0, &m_ShowBaseCtrls);
		ImGui::MenuItem("Options", 0, &m_ShowDescripton);
		ImGui::MenuItem("World Gen", 0, &m_ShowWorldGen);
		ImGui::EndMainMenuBar();
		
		if (m_ShowDescripton)
		{
			ImGui::Begin("Options");
			ImGui::Text("Use (WASD) to move around the world");
			ImGui::Text("Use (F2) to active the camera move");
			ImGui::SliderFloat("Mouse Sensitivty ", &m_CameraRotationSpeed, 0, 1);
			ImGui::SliderFloat("Camera Move Speed", &m_CameraMoveSpeed, 0, 100);
			ImGui::Text("Toggle Wireframe = F1");
			ImGui::Text("Toggle Camera = F2");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

		if (m_ShowWorldGen)
		{
			ImGui::Begin("World Generation");

			ImGui::SliderFloat("Scale", &m_NoiseScale, 0.001, 2);
			ImGui::SliderFloat("Scale Multiplier", &m_NoiseScaleMultiplier, 0, 2);
			ImGui::Text("Noise Scale %.3f", m_NoiseScale * m_NoiseScaleMultiplier);
			ImGui::SliderInt("Noise Octaves", &m_Octaves, 1, 10);
			ImGui::SliderFloat("Noise Persistance", &m_Persistance, 0, 1);
			// ImGui::SliderInt("Height", &m_Height, 8, 64); WIP
			ImGui::InputInt("Noise seed", &m_Seed);


			if (ImGui::Button("Reseed noise (from seed)"))
			{
				m_Noise.ReseedNoise((unsigned) abs(m_Seed));
			}
			ImGui::SameLine();
			if (ImGui::Button("Reseed nosie (random)"))
			{
				m_Noise.ReseedNoise();
			}
			ImGui::SameLine();
			if (ImGui::Button("Generate World"))
			{
				PR_INFO("Generating World");

				m_Noise.SetOctaves(m_Octaves);
				m_Noise.SetPersistance(m_Persistance);
				m_Noise.SetScale((Math::prdecimal)m_NoiseScale * m_NoiseScaleMultiplier);

				//m_ChunkManager.SetHeight(m_Height);
				m_ChunkManager.Clear();
				m_ChunkManager.GenerateFromPosition(m_Camera.GetPosition());
			}
			ImGui::Text("Please don't run the generate world while the meshes are being generated!");
			ImGui::End();
		}
	}

	void VoxelExample::OnUpdate(float dt)
	{
		m_Camera.ShouldLock(m_CameraLocked);
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
			m_VoxelAtlas->Bind(0);
			m_Shader->Bind();
			m_Shader->SetInt("textureAtlas", 0);
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
