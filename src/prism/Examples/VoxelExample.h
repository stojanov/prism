#pragma once

#include "prism/Components/ILayer.h"
#include "prism/Math/PerlinNoise.h"
#include "prism/Renderer/DynamicMesh.h"
#include "prism/Renderer/PerspectiveCamera.h"
#include "prism/Voxels/Chunk.h"
#include "prism/Voxels/ChunkManager.h"

namespace Prism::Examples
{
	class VoxelExample : public ILayer
	{
	public:
		struct ChunkData
		{
			glm::vec3 offset;
		};
		
		VoxelExample(Core::SharedContextRef ctx, const std::string& name);
		virtual ~VoxelExample();

		void OnAttach() override;
		void OnDetach() override;
		void OnDraw() override;
		void OnSystemEvent(Event& e) override;
		void OnGuiDraw() override;
		void OnUpdate(float dt) override;
	private:
		std::future<void> m_MeshGen;
		bool m_CursorOverGui{ false };

		// Hardcoded width and height for now
		Renderer::PerspectiveCamera m_Camera{ 90, 1280, 720, 0.1f, 2048.f };
		Voxel::ChunkManager m_ChunkManager{ m_Ctx, 16, 4 };
		Ref<Gl::Shader> m_Shader;
		Math::PerlinNoise m_Noise;
		VectorPtr<Voxel::Chunk> m_Chunks;
		std::vector<ChunkData> m_ChunkData;
		bool m_CameraLocked{ true };
		glm::vec3 m_LightPosition{ 0.f, -200.f, 200.f };
		glm::vec3 m_LightClr{ 0.1f, 0.9f, 0.6f };
		float m_LightIntensity{ 1.f };
		bool m_ShowChunkCtrls = false;
		bool m_ShowBaseCtrls = false;
	};
}
