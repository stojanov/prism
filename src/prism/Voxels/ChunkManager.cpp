#include "ChunkManager.h"

#include "prism/System/ScopeTimer.h"

namespace Prism::Voxel
{
	ChunkManager::ChunkManager(Core::SharedContextRef ctx, int ChunkSize, int BlockSize)
		:
		m_Ctx(ctx),
		m_ChunkSize(ChunkSize),
		m_BlockSize(BlockSize),
		m_ChunkWorldSize(ChunkSize* BlockSize),
		m_Height(ChunkSize * 8),
		m_ChunkMapper(nullptr)
	{
		
	}

	void ChunkManager::Initial(const Vec2& StartPoint)
	{
		m_CenterPosition = StartPoint;

		_Create(StartPoint);
	}

	void ChunkManager::AttachChunkMapper(Ref<ChunkMapper>&& mapper)
	{
		m_ChunkMapper = mapper;
	}

	void ChunkManager::PopulationFunction(std::function<float(int, int)> func)
	{
		m_PopFunc = std::move(func);
	}

	void ChunkManager::Clear()
	{
		for (auto& i : m_Map)
		{
			i.second->PrepareForClearing();
			i.second->Clear();
		}
		m_Map.clear();
	}

	void ChunkManager::_Create(const Vec2& position)
	{
		std::vector<Chunk*> Additions;
#ifdef BENCHMARK_MESH_GENERATION
        auto start = std::chrono::high_resolution_clock::now();
#endif
		for (int x = position.x - m_Radius; x < position.x + m_Radius; x++)
		{
			for (int y = position.y - m_Radius; y < position.y + m_Radius; y++)
			{
				if (HasBlock({ x, y }))
				{
					continue;
				}
				auto chunk = MakePtr<Chunk>(m_ChunkSize, m_BlockSize);

				chunk->SetPopulationFunction(m_PopFunc);
				chunk->SetHeight(m_Height);
				chunk->SetWorldOffset(x, y);

				if (m_ChunkMapper)
				{
					chunk->HookMapper(m_ChunkMapper);
				}

				auto chPtr = chunk.get();
				m_Ctx->tasks->GetWorker("bg")->QueueTask([this, chPtr]()
					{
						chPtr->Allocate();
						chPtr->Populate();
						chPtr->GenerateMesh2();
					});

				m_Map.emplace(Vec2{ x, y }, std::move(chunk));
			}
		}

#ifdef BENCHMARK_MESH_GENERATION
        m_Ctx->tasks->GetWorker("bg")->WaitToFinishTasks();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        PR_CORE_WARN("Generating mesh took: {}ms", duration.count());
#endif
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

				m_Ctx->tasks->GetWorker("bg")->QueueTask([this, chunk]()
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
	
	// Gets coords in world space
	void ChunkManager::MoveXY(const glm::vec3& pos)
	{
		auto tx = (int) (pos.x / m_ChunkWorldSize);
		auto ty = (int) (pos.z / m_ChunkWorldSize);

		if (tx != m_CenterPosition.x || ty != m_CenterPosition.y)
		{
			PR_CORE_INFO("MOVED TO ({0}, {1}) -> ({2}, {3})", m_CenterPosition.x, m_CenterPosition.y, tx, ty);
			ProcessFromPosition(tx, ty);
		}
	}

	void ChunkManager::GenerateFromPosition(const glm::vec3& pos)
	{
		auto tx = (int)(pos.x / m_ChunkWorldSize);
		auto ty = (int)(pos.z / m_ChunkWorldSize);

		ProcessFromPosition(tx, ty);
	}


	void ChunkManager::_CleanMap()
	{
		PR_SCOPE_TIMER_MS("Chunk cleanup");
		PR_CORE_WARN("Remove Count {0}", m_ToRemove.size());

        for (auto& pos : m_ToRemove)
        {
            if (auto i = m_Map.find(pos); i != m_Map.end())
            {
                i->second->PrepareForClearing();
                i->second->Clear(); // Needs access to an opengl initialized thread
                m_Map.erase(i);
            }
        }

        m_ToRemove.clear();
	}

	void ChunkManager::ProcessFromPosition(int xOffset, int yOffset)
	{
		PR_CORE_WARN("CALCULATING NEW SECTOR");
		PR_SCOPE_TIMER_US("New Sector");

		for (auto& i : m_Map)
		{
			auto& [pos, chunk] = i;
			if (
				pos.x > m_CenterPosition.x + m_Radius || pos.x < m_CenterPosition.x - m_Radius ||
				pos.y > m_CenterPosition.y + m_Radius || pos.y < m_CenterPosition.y - m_Radius)
			{
				m_ToRemove.push_back(pos);
			}
		}

		_CleanMap(); // TODO: Make this multithreaded, causes stutters
		_Create(Vec2{ xOffset, yOffset });

		m_CenterPosition.x = xOffset;
		m_CenterPosition.y = yOffset;
		
	}

	void ChunkManager::_QueueDispose()
	{
		
	}

	// Radius in block count
	void ChunkManager::SetRadius(int r)
	{
		m_Radius = r;
	}

	void ChunkManager::SetHeight(int height)
	{
		m_Height = height;
	}

}
