#pragma once

#include <unordered_map>

#include "Chunk.h"
#include <vec2.hpp>
#include "prism/Core/Pointers.h"
#include "phmap.h"

namespace Prism::Voxel
{
	struct Vec2Hasher
	{
		size_t operator()(const Vec2& vec) const noexcept
		{
			std::hash<int> hasher;
			const auto a = hasher(vec.x);
			const auto b = hasher(vec.y);
			// (a ^ b) >> 2
			return b + 0x9e3779b9 + (a << 6) + (a >> 2); // boost library way to combine two hashes
		}
	};
	
	class ChunkManager
	{
	using Map = std::unordered_map<Vec2, Ptr<Chunk>, Vec2Hasher>;
	// Parallel hash map causes crashes due to the multithreading nature of the code
	// will need more time to fix this problem
	//using Map = phmap::parallel_flat_hash_map<Vec2, Ref<Chunk>, Vec2Hasher>;
	//using Map = phmap::node_hash_map<Vec2, Ptr<Chunk>, Vec2Hasher>;
	public:
		ChunkManager(Core::SharedContextRef ctx, int ChunksSize, int BlockSize);

		void Initial(const Vec2& StartPoint);
		void PopulationFunction(std::function<float(int, int)> func);
		void SetRadius(int r);
		void SetHeight(int height);
		void ProcessFromPosition(int xOffset, int yOffset);
		void GenerateFromPosition(const glm::vec3& pos);
		void MoveXY(const glm::vec3& pos);
		bool HasBlock(const Vec2& pos)
		{
			return m_Map.find(pos) != m_Map.end();
		}

		void Clear();

		/*
		Chunk& GetChunk(const Vec2& pos)
		{
			if (auto chunk = m_Map.find(pos); chunk != m_Map.end())
			{
				return chunk->second;
			}
			PR_ERROR("Can't find chunk at {0},{1}", pos.x, pos.y);
		}
		*/
		Map::iterator begin() { return m_Map.begin();  }
		Map::iterator end() { return m_Map.end(); }
		Map::const_iterator begin() const { return m_Map.begin(); }
		Map::const_iterator end() const{ return m_Map.end(); }
	private:
		void _CleanMap();
		void _QueueDispose();
		void _Create(const Vec2& pos);
		float _Dist(int x1, int y1, int x2, int y2)
		{
			auto a = (x2 - x1);
			auto b = (y2 - y1);
			return sqrt(a * a + b * b);
		}

		std::mutex m_Mutex;
		std::condition_variable m_CleanSignal;
		bool m_CleanFinished{ false };

		Core::SharedContextRef m_Ctx;
		int m_Radius; // In blocks
		std::function<float(int, int)> m_PopFunc;
		Map m_Map;
		std::vector<Vec2> m_ToRemove;
		Vec2 m_CenterPosition;
		int m_Height;
		int m_ChunkWorldSize;
		int m_ChunkSize;
		int m_BlockSize;
	};
}
