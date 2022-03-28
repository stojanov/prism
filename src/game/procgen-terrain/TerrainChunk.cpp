#include "TerrainChunk.h"

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
		{ Prism::Gl::ShaderDataType::Float2, "texcoords"}
	});

	m_Mesh.AllocateVertexBuffer(0, m_Width * m_Height);
	m_Mesh.AllocateVertexBuffer(m_ColorBuffer, m_Width * m_Height);
	m_Mesh.AllocateVertexBuffer(m_NormalBuffer, m_Width * m_Height);
	m_Mesh.AllocateVertexBuffer(m_UVBuffer, m_Width * m_Height);
	m_Mesh.AllocateIndexBuffer((m_Width - 1) * (m_Height - 1) * 6);
}

void TerrainChunk::__CreateMesh()
{
	m_MeshReady = false;

	uint32_t VertexCount = 0;
	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			if (x < m_Width - 1 && x < m_Height - 1)
			{
				float u = 1.f * x / m_Width;
				float v = 1.f * y / m_Height;



				if (x < m_Width - 1 && y < m_Height - 1)
				{
					m_Mesh.ConnectVertices(VertexCount, VertexCount + m_Width + 1, VertexCount + m_Width);
					m_Mesh.ConnectVertices(VertexCount, VertexCount + 1, VertexCount + m_Width + 1);
				}
			}
		}
	}
}

void TerrainChunk::__UpdateNormals()
{
	static constexpr auto Idx = [](int w, int h)
	{
		return [w, h](int x, int y)
		{
			return 0;
		};
	};

	const auto _CalcIdx = Idx(m_Width, m_Height);

	_CalcIdx(1, 1);

	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			
		}
	}
}


void TerrainChunk::Render()
{
	
}
