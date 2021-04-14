#include "ChunkManager.h"

namespace Prism::Voxel
{
	ChunkManager::ChunkManager(Core::SharedContextRef ctx, int ChunkSize, int BlockSize)
		:
		m_Ctx(ctx),
		m_ChunkSize(ChunkSize),
		m_BlockSize(BlockSize),
		m_ChunkWorldSize(ChunkSize * BlockSize)
	{
		
	}

	void ChunkManager::Initial(const Vec2& StartPoint)
	{
		m_CenterPosition = StartPoint;

		_Create(0);
		_Create(1);
	}

	void ChunkManager::PopulationFunction(std::function<float(int, int)> func)
	{
		m_PopFunc = func;
	}

	void ChunkManager::_Create(int selection)
	{
		static int nummbersellection[2] = {
			1, // 0
			-1 // 1
		};
		int direction = nummbersellection[selection];
		
		std::vector<Chunk*> Additions;
		for (int x = m_CenterPosition.x - m_Radius; x < m_CenterPosition.x + m_Radius; x++)
		{
			float s = sqrt(m_Radius * m_Radius - x * x);
			int endPoint = ceil(s);
			endPoint = m_CenterPosition.y == 0 ? direction * endPoint : endPoint + direction * m_CenterPosition.y;

			int* choiceAr[2] = {
				&endPoint,
				&m_CenterPosition.y
			};
			
			int sel1 = selection | 0;
			int sel2 = selection ^ 1;
			
			while (*choiceAr[selection | 0] > *choiceAr[selection ^ 1])
			{
				endPoint -= direction;
				if (HasBlock({x, endPoint}))
				{
					break;
				}
				
				auto chunk = MakePtr<Chunk>(m_ChunkSize, m_BlockSize);
				chunk->SetPopulationFunction(m_PopFunc);
				chunk->SetOffset(x, endPoint);

				Additions.push_back(chunk.get());
				m_Map.emplace(Vec2{ x, endPoint }, std::move(chunk));
			}
		}
		
		for (auto* chunk : Additions)
		{
			chunk->Allocate();
			chunk->Populate();
			chunk->GenerateMesh();
			/*
			m_Ctx->Tasks->GetWorker("bg")->QueueTask([this, chunk]()
			{

			});
			*/
		}

		PR_CORE_WARN("(ChunkManager) Added {0} new chunks", Additions.size());
	}
	
	// Gets cords in world space
	void ChunkManager::MoveXY(const glm::vec3& pos)
	{
		auto x = m_CenterPosition.x * m_ChunkWorldSize;
		auto y = m_CenterPosition.y * m_ChunkWorldSize;

		auto tx = (int) (pos.x / m_ChunkWorldSize);
		auto ty = (int) (pos.z / m_ChunkWorldSize);

		if (tx != x || ty != y)
		{
			//PR_INFO("{0}, {1} {2}, {3}", x, y, tx, ty);
			ProcessFromPosition(tx, ty);
		}
	}

	void ChunkManager::_CleanMap()
	{
		for (auto& pos : m_ToRemove)
		{
			m_Map.erase(pos);
		}
	}

	void ChunkManager::ProcessFromPosition(int xOffset, int yOffset)
	{
		if (m_CenterPosition.x == xOffset && m_CenterPosition.y == yOffset)
		{
			return;
		}
		PR_INFO("CALCULATING NEW SECTOR");
		for (auto& [pos, chunk] : m_Map)
		{
			auto dist = _Dist(xOffset, yOffset, pos.x, pos.y);
			if (dist > m_Radius)
			{
				m_ToRemove.push_back(pos);
			}
		}

		_CleanMap();

		int tempY = m_CenterPosition.y;
		m_CenterPosition.x = xOffset;
		m_CenterPosition.y = yOffset;

		_Create(yOffset - tempY > 0 ? 0 : 1);
	}

	void ChunkManager::_QueueDispose()
	{
		
	}

	// Radius in block count
	void ChunkManager::SetRadius(int r)
	{
		m_Radius = r;
	}
}