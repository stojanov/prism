#include "TerrainChunk.h"

#include <prism/System/ScopeTimer.h>
#include "ext/scalar_constants.hpp"

TerrainChunk::TerrainChunk(int width, int height)
	:
	m_Width(width),
	m_Height(height)
{
	m_ColorBuffer = m_Mesh.CreateNewVertexBuffer({
		{ Prism::Gl::ShaderDataType::Float3, "color"},
	});

	m_NormalBuffer = m_Mesh.CreateNewVertexBuffer({
		{ Prism::Gl::ShaderDataType::Float3, "normals"},
	});

	m_UVBuffer = m_Mesh.CreateNewVertexBuffer({
		{ Prism::Gl::ShaderDataType::Float2, "uv"}
	});

	_Allocate();
	_CreateMesh();
}

void TerrainChunk::_Allocate()
{
	m_Mesh.AllocateVertexBuffer(0, m_Width * m_Height);
	m_Mesh.AllocateVertexBuffer(m_ColorBuffer, m_Width * m_Height);
	m_Mesh.AllocateVertexBuffer(m_NormalBuffer, m_Width * m_Height);
	m_Mesh.AllocateVertexBuffer(m_UVBuffer, m_Width * m_Height);
	m_Mesh.AllocateIndexBuffer((m_Width - 1) * (m_Height - 1) * 6);
}

void TerrainChunk::SetHeightFunc(std::function<float(int, int, int, int)> func)
{
	m_HeightFunc = func;
}

void TerrainChunk::BakeMap()
{
	PR_SCOPE_TIMER_US("Generating Height Map");
	if (!m_HeightFunc)
	{
		PR_CORE_WARN("Height function not found");
		return;
	}

	m_HeightMap.resize(m_Width * m_Height);

	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			m_HeightMap[_Idx(x, y)] = m_HeightFunc(x, y, m_Width, m_Height);
		}
	}
}

void TerrainChunk::_CreateMesh()
{
	PR_SCOPE_TIMER_US("Mesh Creation");
	m_MeshReady = false;

	uint32_t VertexCount = 0;
	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			float u = 1.f * x / m_Width;
			float v = 1.f * y / m_Height;

			m_Mesh.AddVertex(glm::vec3{ x, 0.f, y });
			m_Mesh.AddVertex(m_UVBuffer, glm::vec2{ u, v });
			m_Mesh.AddVertex(m_ColorBuffer, glm::vec3{ 1.f, 0.f, 0.f });
			m_Mesh.AddVertex(m_NormalBuffer, glm::vec3{ 0.f, 0.0f, 0.f });

			if (x < m_Width - 1 && y < m_Height - 1)
			{
				m_Mesh.ConnectVertices(VertexCount, VertexCount + m_Width + 1, VertexCount + m_Width);
				m_Mesh.ConnectVertices(VertexCount + m_Width + 1, VertexCount, VertexCount + 1);
			}

			VertexCount++;
		}
	}

	m_Mesh.Flush();

	m_MeshReady = true;
}

void TerrainChunk::UpdateMesh(bool shouldUseFunc)
{
	PR_SCOPE_TIMER_US("Updating Mesh");

	if (!shouldUseFunc && m_HeightMap.size() != m_Width * m_Height)
	{
		PR_CORE_ERROR("(TerrainChunk) Cannot use height map looks like it's not generated yet!");
		return;
	}

	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			float z;
			if (shouldUseFunc)
			{
				z = m_HeightFunc(x, y, m_Width, m_Height);
			}
			else
			{
				z = m_HeightMap[_Idx(x, y)];
			}
			m_Mesh.OverwriteVertex(0, _Idx(x, y), glm::vec3{ x, z, y });
		}
	}

	m_Mesh.Flush();
}

void TerrainChunk::Render()
{
	m_Mesh.DrawIndexed();
}
