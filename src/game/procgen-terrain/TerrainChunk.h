#pragma once

#include "prism/Renderer/DynamicMesh.h"

class TerrainChunk
{
public:
	TerrainChunk(int width, int height);

	void SetHeightFunc(std::function<float(int, int)> func);
	void CreateTerrain();
	void UpdateMesh();
	void Render();

	const glm::mat4& GetTransform()
	{
		return m_Transform;
	}

private:
	void __CreateMesh();
	void __UpdateNormals();

	std::atomic_bool m_MeshReady{ false };
	Prism::Renderer::DynamicMesh m_Mesh;

	std::function<float(int, int)> m_HeightFunc;
	// Will use the world pos and tranform in case i want to implement infinite terrain
	glm::vec3 m_WorldPosition{ 0.f, 0.f, 0.f }; // World position
	glm::mat4 m_Transform{ 1.f };
	int m_Width;
	int m_Height;
	uint32_t m_ColorBuffer;
	uint32_t m_NormalBuffer;
	uint32_t m_UVBuffer;
};