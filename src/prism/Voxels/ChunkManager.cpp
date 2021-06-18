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
	}

	void ChunkManager::PopulationFunction(std::function<float(int, int)> func)
	{
		m_PopFunc = func;
	}
	
	void ChunkManager::_Create(int selection)
	{
		std::vector<Chunk*> Additions;

		for (int x = m_CenterPosition.x - m_Radius; x < m_CenterPosition.x + m_Radius; x++)
		{
			for (int y = m_CenterPosition.y - m_Radius; y < m_CenterPosition.y + m_Radius; y++)
			{
				if (HasBlock({ x, y }))
				{
					continue;
				}
				auto chunk = MakePtr<Chunk>(m_ChunkSize, m_BlockSize);
				chunk->SetPopulationFunction(m_PopFunc);
				chunk->SetWorldOffset(x, y);
				Additions.push_back(chunk.get());

				m_NewAdditions.push_back({ x, y });
				m_Map.emplace(Vec2{ x, y }, std::move(chunk));
			}
		}

		for (auto chunk : Additions)
		{
			m_Ctx->Tasks->GetWorker("bg")->QueueTask([this, chunk]()
				{
					chunk->Allocate();
					chunk->Populate();
					chunk->GenerateMesh();
;				});
		}
	}

	/*
	void ChunkManager::_Create(int selection)
	{
		static int nummbersellection[2] = {
			1, // 0
			-1 // 1
		};
		int direction = nummbersellection[selection];
		
		std::vector<Ref<Chunk>> Additions;

		for (int x = m_CenterPosition.x - m_Radius; x < m_CenterPosition.x + m_Radius; x++)
		{
			float s = sqrt(abs(sqrt(m_Radius) - x * x));
			int endPoint = ceil(s);
			PR_INFO("ENDPOINT {0} RADIUS {1} X {2}", endPoint, m_Radius, x);
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
				
				auto chunk = MakeRef<Chunk>(m_ChunkSize, m_BlockSize);
				
				chunk->SetPopulationFunction(m_PopFunc);
				chunk->SetWorldOffset(x, endPoint);
				Additions.push_back(chunk);

				m_Ctx->Tasks->GetWorker("bg")->QueueTask([this, chunk]()
				{
					chunk->Allocate();
					chunk->Populate();
					chunk->GenerateMesh();
				});


				m_Map.emplace(Vec2{ x, endPoint }, (chunk));
			}
		}
		
		for (auto chunk : Additions)
		{

		}

		PR_CORE_WARN("(ChunkManager) Added {0} new chunks", Additions.size());
	}
	*/
	
	// Gets cords in world space
	void ChunkManager::MoveXY(const glm::vec3& pos)
	{
		auto tx = (int) (pos.x / m_ChunkWorldSize);
		auto ty = (int) (pos.z / m_ChunkWorldSize);

		if (tx != m_CenterPosition.x || ty != m_CenterPosition.y)
		{
			PR_INFO("MOVED TO ({0}, {1}) -> ({2}, {3})", m_CenterPosition.x, m_CenterPosition.y, tx, ty);
			ProcessFromPosition(tx, ty);
		}
	}

	void ChunkManager::_CleanMap()
	{
		for (auto& pos : m_ToRemove)
		{
			if (auto i = m_Map.find(pos); i != m_Map.end())
			{
				i->second->PrepareForClearing();
				i->second->Clear();
				m_Map.erase(pos);
				PR_WARN("ERASING {0} {1}", pos.x, pos.y);
			}
		}
		m_ToRemove.clear();
	}

	void ChunkManager::ProcessFromPosition(int xOffset, int yOffset)
	{
		PR_INFO("CALCULATING NEW SECTOR");
		for (auto& [pos, chunk] : m_Map)
		{
			if (
				pos.x > m_CenterPosition.x + m_Radius || pos.x < m_CenterPosition.x - m_Radius||
				pos.y > m_CenterPosition.y + m_Radius || pos.y < m_CenterPosition.y - m_Radius)
			{
				m_ToRemove.push_back(pos);
			}
		/*
			auto dist = _Dist(xOffset, yOffset, pos.x, pos.y);
			if (dist > m_Radius)
			{
				m_ToRemove.push_back(pos);
			}
			*/
		}
		_CleanMap();

		m_CenterPosition.x = xOffset;
		m_CenterPosition.y = yOffset;
		_Create(0);
		
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