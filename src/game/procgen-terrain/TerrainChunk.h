#pragma once

#include "prism/Renderer/DynamicMesh.h"

class TerrainChunk
{
public:
	TerrainChunk(int width, int height);

	void SetHeightFunc(std::function<float(int, int, int, int)> func);
	void BakeMap();

	void CreateMesh();
	void UpdateMesh(bool shouldUseFunc = false);
	void Render();

	const glm::mat4& GetTransform()
	{
		return m_Transform;
	}

private:
	void _Allocate();
	

	bool _CheckBounds(int x, int y)
	{
		return (x > m_Width - 1) || (x < 0) || (y > m_Height - 1) || (y < 0);
	}

	int _Idx(int x, int y)
	{
		return m_Width * x + y;
	}

	std::vector<float> m_HeightMap;
	std::atomic_bool m_MeshReady{ false };
	Prism::Renderer::DynamicMesh m_Mesh;

	std::function<float(int, int, int, int)> m_HeightFunc{ nullptr };
	// Will use the world pos and tranform in case i want to implement infinite terrain
	glm::vec3 m_WorldPosition{ 0.f, 0.f, 0.f }; // World position
	glm::mat4 m_Transform{ 1.f };
	int m_Width;
	int m_Height;
	uint32_t m_OffsetBuffer;
	uint32_t m_ColorBuffer;
	uint32_t m_NormalBuffer;
	uint32_t m_UVBuffer;
};