#include "Chunk.h"

#include <ext/matrix_transform.hpp>

#include "prism/System/ScopeTimer.h"
namespace std
{
	// TEMPPPP!!!
	template<>
	struct hash<Prism::Voxel::Vec2>
	{
		size_t operator()(const Prism::Voxel::Vec2& vec) const noexcept
		{
			hash<int> hasher;
			auto a = hasher(vec.x);
			auto b = hasher(vec.y);
			return b + 0x9e3779b9 + (a << 6) + (a >> 2);
		}
	};
}

namespace Prism::Voxel
{
	class DefaultMapper : public Voxel::ChunkMapper
	{
	public:

		void Map(const MappingData& data, int x, int y, int height, FaceSide side) override
		{
			auto color2 = glm::vec3{ 1.f, 0.f, 1.f };
			data.ColorFn(color2);

			auto coords = std::array<glm::vec2, 4>();

			data.TextureCoordFn(coords);
		}
	};

	Chunk::Chunk(int Size, int blockSize)
		:
		m_BlockSize(blockSize),
		m_XSize(Size),
		m_YSize(Size),
		m_ZSize(Size),
		m_Height(Size)
	{
		m_Mesh = MakePtr<MeshType>();
		m_DebugMesh = MakePtr<MeshType>();

		m_NormalBuffer = m_Mesh->CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "normal" }
		});

		m_ColorBuffer = m_Mesh->CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "color" }
		});

		m_TextureCoordBuffer = m_Mesh->CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float2, "TexCoord" }
		});

		m_DebugMesh->CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "color"}
		});

		m_MappingData.MinHeight = 0;

		m_MappingData.ColorFn = [this](const glm::vec3& color)
		{
			_PassVertParam(m_ColorBuffer, color);
		};

		m_MappingData.TextureCoordFn = [this](std::array<glm::vec2, 4> texCoords)
		{
			for (auto& coords: texCoords)
			{
				m_Mesh->AddVertex(m_TextureCoordBuffer, coords);
			}
		};

		HookMapper(MakeRef<DefaultMapper>());
	}

	// Allocation is in another function in order to
	// defer it until it can be done in a separate thread
	void Chunk::Allocate()
	{
		if (m_IsAllocated)
		{
			return;
		}

		size_t total = m_XSize * m_ZSize * m_YSize;
		m_Blocks.resize(total);
		m_BlockHeights.resize(m_XSize * m_ZSize, 0);

		m_Mesh->AllocateVertexBuffer(0, 12 * total);
		m_Mesh->AllocateVertexBuffer(m_NormalBuffer, 12 * total);
		m_Mesh->AllocateVertexBuffer(m_ColorBuffer, 12 * total);
		m_Mesh->AllocateVertexBuffer(m_TextureCoordBuffer, 12 * total);
		m_Mesh->AllocateIndexBuffer(6 * total);

		m_IsAllocated = true;
		m_MeshReady = false;
	}

	void Chunk::SetHeight(int h)
	{
		m_Height = h;
		m_YSize = h;
		m_MappingData.MaxHeight = m_YSize;
	}

	void Chunk::SetPopulationFunction(std::function<float(int, int)> PopFunc)
	{
		m_PopulationFunction = PopFunc;
	}

	void Chunk::HookMapper(Ref<ChunkMapper> mapper)
	{
		m_Mapper = std::move(mapper);
	}

	void Chunk::Populate()
	{
		PR_ASSERT(m_PopulationFunction, "(Chunk) No population function present!");
		PR_SCOPE_TIMER_US("Chunk Population");
		m_MeshReady = false;
		int zOffset = m_ZSize * m_YOffset;
		int xOffset = m_XSize * m_XOffset;
		for (int x = 0; x < m_XSize; x++)
		{
			int xTranslated = x + xOffset;
			for (int z = 0; z < m_ZSize; z++)
			{
				int height = ceil(m_PopulationFunction(xTranslated, z + zOffset) * m_Height);
				height = glm::clamp(height, 0, m_YSize);
				for (int i = 0; i < height; i++)
				{
					m_Blocks[_GetBlockLoc(x, z, i)].Type = BlockType::BLOCK;
				}
				m_BlockHeights[_GetLoc(x, z)] = height;
			}
		}

		m_Populated = true;
	}

	void Chunk::_CreateQuad(
		int v0x, int v0y, int v0z,
		int v1x, int v1y, int v1z,
		int v2x, int v2y, int v2z,
		int v3x, int v3y, int v3z)
	{
		glm::vec3 p0 = { v0x, v0y, v0z };
		glm::vec3 p1 = { v1x, v1y, v1z };
		glm::vec3 p2 = { v2x, v2y, v2z };
		glm::vec3 p3 = { v3x, v3y, v3z };

		auto v0p = m_Mesh->AddVertex(p0);
		auto v1p = m_Mesh->AddVertex(p1);
		auto v2p = m_Mesh->AddVertex(p2);
		auto v3p = m_Mesh->AddVertex(p3);

		glm::vec3 normal = glm::cross((p2 - p0), (p3 - p1));

		m_Mesh->ConnectVertices(v0p, v1p, v3p);
		m_Mesh->ConnectVertices(v3p, v1p, v2p);

		_PassVertParam(m_NormalBuffer, normal);
		//_TexCord();
	}

	void Chunk::_PassVertParam(uint32_t buffer, const glm::vec3& param)
	{
		m_Mesh->AddVertex(buffer, param);
		m_Mesh->AddVertex(buffer, param);
		m_Mesh->AddVertex(buffer, param);
		m_Mesh->AddVertex(buffer, param);
	}

	void Chunk::_PassBlockParam(const glm::vec3& param)
	{
	}

	void Chunk::SetWorldOffset(int x, int y)
	{
		m_XOffset = x;
		m_YOffset = y;
		m_Position = glm::vec3{
			x * (m_BlockSize * m_XSize - 2 * m_BlockSize),
			0.f,
			y * m_BlockSize * m_ZSize
		};

		_CreateDebugRect();

		m_Transform = glm::translate(glm::mat4(1.f), m_Position);
	}

	void Chunk::SendToGpu()
	{
		bool isNotSent = false;

		if (m_DataSentToGpu.compare_exchange_strong(isNotSent, true))
		{
			m_Mesh->Flush();
		} 
	}

	void Chunk::Clear()
	{
		m_Blocks.clear();
		m_BlockHeights.clear();
		m_Mesh->ClearBuffers();
		m_Mesh->ClearGpuBuffers();
	}

	void Chunk::Destroy()
	{
		PrepareForClearing();
		Clear();
	}

	void Chunk::PrepareForClearing()
	{
		m_MeshReady = false;
	}

	void Chunk::Render()
	{
		m_Mesh->DrawIndexed();
	}

	void Chunk::_CreateDebugRect()
	{
		int xStart = m_XSize * m_XOffset;
		int xEnd = xStart + m_XSize;
		int yStart = m_YSize * m_YOffset;
		int yEnd = yStart + m_YSize;

		//m_DebugMesh->AddVertex();
	}

	void Chunk::RenderDebug()
	{

	}

	void Chunk::GenerateMesh2()
	{
		if (!m_Populated) return;
		PR_SCOPE_TIMER_US("Chunk Mesh Gen");

		int yStart;
		int yEnd;
		int xStart;
		int xEnd;
		int zStart;
		int zEnd;

		int height;

		int PossibleSides[4] = {
			-1,
			-1,
			-1,
			-1,
		};

		int* VertexOffsets[48] = {
			// Left
			&xEnd, &yEnd, &zStart,
			&xEnd, &yStart, &zStart,
			&xEnd, &yStart, &zEnd,
			&xEnd, &yEnd, &zEnd,
			// Right
			&xStart, &yEnd, &zStart,
			&xStart, &yStart, &zStart,
			&xStart, &yStart, &zEnd,
			&xStart, &yEnd, &zEnd,
			// Front reverse order to cull
			&xStart, &yEnd, &zEnd,
			&xStart, &yStart, &zEnd,
			&xEnd, &yStart, &zEnd,
			&xEnd, &yEnd, &zEnd,
			// Back
			&xStart, &yEnd, &zStart,
			&xStart, &yStart, &zStart,
			&xEnd, &yStart, &zStart,
			&xEnd, &yEnd, &zStart
		};

		for (int x = 0; x < m_XSize; x++)
		{
			for (int z = 0; z < m_ZSize; z++)
			{
				height = m_BlockHeights[_GetLoc(x, z)];

				xStart = x * m_BlockSize;
				xEnd = xStart + m_BlockSize;
				zStart = z * m_BlockSize;
				zEnd = zStart + m_BlockSize;

				yStart = height * m_BlockSize;
				yEnd = yStart + m_BlockSize;

				_CreateQuad(
					xStart, yEnd, zEnd,
					xStart, yEnd, zStart,
					xEnd, yEnd, zStart,
					xEnd, yEnd, zEnd
				);
				glm::vec3 clr = { 0.f, 0.8f, 0.1f };

				// Mapper should be already initialized
				m_Mapper->Map(m_MappingData, x, z, height, ChunkMapper::FaceSide::TOP);

				int positions[8] = {
					x + 1, z, // Left
					x - 1, z, // Right
					x, z + 1, // Front
					x, z - 1, // Back
				};

				ChunkMapper::FaceSide faceSides[4] = {
					ChunkMapper::FaceSide::LEFT,
					ChunkMapper::FaceSide::RIGHT,
					ChunkMapper::FaceSide::FRONT,
					ChunkMapper::FaceSide::BACK,
				};

				for (int h = height; h >= 0; h--)
				{
					int sides = 4;
					for (int i = 0; i < 4; i++)
					{
						int* nPos = &positions[i * 2];

						if (_Check2DBounds(nPos[0], nPos[1]))
						{
							auto blockType = m_Blocks[_GetBlockLoc(nPos[0], nPos[1], h)].Type;

							if (blockType != BlockType::NONE)
							{
								sides--;
								continue;
							}

							yStart = h * m_BlockSize;
							yEnd = yStart + m_BlockSize;

							int** Vertex = &VertexOffsets[i * 12];

							_CreateQuad(
								*Vertex[0], *Vertex[1], *Vertex[2],
								*Vertex[3], *Vertex[4], *Vertex[5],
								*Vertex[6], *Vertex[7], *Vertex[8],
								*Vertex[9], *Vertex[10], *Vertex[11]
							);
							m_Mapper->Map(m_MappingData, x, z, h, faceSides[i]);
						}
						else
						{
							continue; // Check neighbouring chunk
						}
					}

					if (sides == 0)
					{
						break;
					}
				}
			}
		}

		m_MeshReady = true;
	}

	void Chunk::GenerateMesh()
	{
		if (!m_Populated) return;
		PR_SCOPE_TIMER_US("Chunk Mesh Gen");

		int yStart;
		int yEnd;
		int xStart;
		int xEnd;
		int zStart;
		int zEnd;

		int height;

		int PossibleSides[4] = {
			-1,
			-1,
			-1,
			-1,
		};

		 int* VertexOffsets[48] = {
			// Left
			&xEnd, &yEnd, &zStart,
			&xEnd, &yStart, &zStart,
			&xEnd, &yStart, &zEnd,
			&xEnd, &yEnd, &zEnd,
			// Right
			&xStart, &yEnd, &zStart,
			&xStart, &yStart, &zStart,
			&xStart, &yStart, &zEnd,
			&xStart, &yEnd, &zEnd,
			// Front reverse order to cull
			&xStart, &yEnd, &zEnd,
			&xStart, &yStart, &zEnd,
			&xEnd, &yStart, &zEnd,
			&xEnd, &yEnd, &zEnd,
			// Back
			&xStart, &yEnd, &zStart,
			&xStart, &yStart, &zStart,
			&xEnd, &yStart, &zStart,
			&xEnd, &yEnd, &zStart
		};

		for (int x = 0; x < m_XSize; x++)
		{
			for (int z = 0; z < m_ZSize; z++)
			{
				height = m_BlockHeights[_GetLoc(x, z)];

				xStart = x * m_BlockSize;
				xEnd = xStart + m_BlockSize;
				zStart = z * m_BlockSize;
				zEnd = zStart + m_BlockSize;

				int positions[8] = {
					x + 1, z, // Left
					x - 1, z, // Right
					x, z + 1, // Front
					x, z - 1, // Back
				};

				for (int i = 0; i < 4; i++)
				{
					int nh = -1;
					int* PosPtr = &positions[i * 2];
					if (_Check2DBounds(PosPtr[0], PosPtr[1]))
					{
						nh = m_BlockHeights[_GetLoc(PosPtr[0], PosPtr[1])];
					}
					else
					{
						nh = _FetchNeighbour(PosPtr[0], PosPtr[1]);
					}
					if (nh > height)
					{
						PossibleSides[i] = -1;
						continue;
					}
					PossibleSides[i] = nh;
				}

				yStart = height * m_BlockSize;
				yEnd = yStart + m_BlockSize;

				_CreateQuad(
					xStart, yEnd, zEnd,
					xStart, yEnd, zStart,
					xEnd, yEnd, zStart,
					xEnd, yEnd, zEnd
				);

				glm::vec3 clr = { 0.f, 0.8f, 0.1f };
				_PassVertParam(m_ColorBuffer, clr);

				for (int i = 0; i < 4; i++)
				{
					if (PossibleSides[i] == -1)
						continue;

					for (int fl = height; fl > PossibleSides[i]; fl--)
					{
						yStart = fl * m_BlockSize;
						yEnd = yStart + m_BlockSize;

						int** Vertex = &VertexOffsets[i * 12];
						_CreateQuad(
							*Vertex[0], *Vertex[1], *Vertex[2],
							*Vertex[3], *Vertex[4], *Vertex[5],
							*Vertex[6], *Vertex[7], *Vertex[8],
							*Vertex[9], *Vertex[10], *Vertex[11]
						);
						_PassVertParam(m_ColorBuffer, clr);
					}
				}

			}
		}

		m_MeshReady = true;
	}
}
