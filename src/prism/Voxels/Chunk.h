#pragma once
#include <functional>
#include <vector>

#include "prism/Renderer/DynamicMesh.h"
#include "prism/Core/SharedContext.h"
#include "prism/Renderer/AllocatedMesh.h"

namespace Prism::Voxel
{
	struct Vec2
	{
		int x;
		int y;

		bool operator==(const Prism::Voxel::Vec2& other) const noexcept
		{
			return (x == other.x) && (y == other.y);
		}
	};


	class Chunk
	{
		using MeshType = Renderer::DynamicMesh;
	public:
		enum class BlockType
		{
			NONE = 0,
			BLOCK,// TEMP
			DIRT
		};

		struct BlockData
		{
			BlockType Type{ BlockType::NONE };
			Vec2 Pos;
		};

		enum class ChunkBlockPosition
		{
			NONEXIST = 0,
			BODY,

			COUNT
		};

		Chunk(int Size, int blockSize);

		void Allocate();
		void Populate();
		void SetPopulationFunction(std::function<float(int, int)> PopFunc);
		void SetMappingFunction(std::function<void()> MapFunc);
		void GenerateMesh();
		void SendToGpu();
		void SetOffset(int x, int y);
		void RebuildMesh();
		void UpdateGpu(); // Will update only if rebuild has been called

		const Vec2& GetOffset()
		{
			return Vec2{ m_XOffset, m_YOffset };
		}

		glm::mat4& GetTransform()
		{
			return m_Transform;
		}

		glm::vec3& GetPosition()
		{
			return m_Position;
		}

		const glm::vec3& Size()
		{
			return glm::vec3{
				m_XSize * m_BlockSize,
				m_YSize * m_BlockSize,
				m_ZSize * m_BlockSize,
			};
		}

		bool MeshReady()
		{
			return *m_MeshReady;
		}

		// Will prepare for destruction
		void Clear();
		void PrepareForClearing();
		void Render();
		void RenderDebug();
	private:
		void _CreateDebugRect();
		void _CreateQuad(
			int v0x, int v0y, int v0z,
			int v1x, int v1y, int v1z,
			int v2x, int v2y, int v2z,
			int v4x, int v3y, int v3z
		);
		void _PassBlockParam(const glm::vec3& param);
		void _PassVertParam(uint32_t buffer, const glm::vec3& param);

		int _GetLoc(int x, int y) const
		{
			return m_XSize * y + x;
		}

		int _GetBlockLoc(int x, int y, int z) const
		{
			return m_XSize * (y + m_ZSize * z) + x;
		}

		bool _Check2DBounds(int x, int y)
		{
			return x >= 0 && x < m_XSize&& y >= 0 && y < m_ZSize;
		}

		int _FetchNeighbourBlock(int x, int y)
		{

		}

		int _FetchNeighbour(int x, int y)
		{
			int xOffset = x + m_XSize * m_XOffset;
			int zOffset = y + m_ZSize * m_YOffset;
			int ySize = m_YSize - 1;
			int height = ceil(m_PopulationFunction(xOffset, zOffset) * ySize);
			//PR_INFO("{0} {1} {2}", xOffset, zOffset, height);
			return glm::clamp(height, 0, m_YSize);
		}

		ChunkBlockPosition _GetBlockState(int x, int y, int z)
		{
			if (!(x > m_XSize && z > m_ZSize && y > m_YSize))
			{
				return ChunkBlockPosition::NONEXIST;
			}

			static ChunkBlockPosition ChunkBlockSelection[2] = {
				ChunkBlockPosition::NONEXIST,
				ChunkBlockPosition::BODY,
			};

			return ChunkBlockSelection[(int)m_Blocks[_GetBlockLoc(x, y, z)].Type];
		}

		bool _BodyBlockExists(ChunkBlockPosition b)
		{
			return ((int)b) >= 2;
		}

		Ptr<MeshType> m_Mesh;
		Ptr<MeshType> m_DebugMesh;
		std::vector<BlockData> m_Blocks;
		// Will be used once the mesh is created to create a more
	   //  optimized mesh for adding and removing blocks
		std::vector<int> m_BlockHeights;
		std::function<void()> m_MappingFunction;
		std::function<float(int, int)> m_PopulationFunction;
		uint32_t m_NormalBuffer;
		uint32_t m_ColorBuffer;
		Ptr<std::atomic_bool> m_MeshReady;
		glm::vec3 m_Position;
		glm::mat4 m_Transform{ 1.f };
		int m_CreatedFaces{ 0 };
		bool m_IsAllocated{ false };
		bool m_DataSentToGpu{ false };

		int m_BlockSize;
		int m_XSize;
		int m_YSize;
		int m_ZSize;
		int m_XOffset{ 0 };
		int m_YOffset{ 0 };
	};

}
