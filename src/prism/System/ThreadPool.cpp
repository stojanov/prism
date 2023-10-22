#include "ThreadPool.h"

#include "Log.h"

namespace Prism::System
{
	ThreadPool::ThreadPool()
	{
	}

	ThreadPool::ThreadPool(VoidCallback StartCallback)
		:
		m_StartCallback(std::move(StartCallback))
	{}

	ThreadPool::ThreadPool(VoidCallback StartCallback, VoidCallback EndCallback)
		:
		m_StartCallback(std::move(StartCallback)),
		m_EndCallback(std::move(EndCallback))
	{}

	ThreadPool::~ThreadPool()
	{
		try
		{
			Finish();
		} catch (std::exception& e)
		{
			PR_CORE_ERROR("Error while shutting down the thread pool: {0}", e.what());
		}
		
	}

	void ThreadPool::Start(size_t N)
	{
		for (int i = 0; i < N; i++)
		{
			m_RunningThreads.push_back(
				std::async(
					std::launch::async,
					[this] { _ThreadWork();  }
			));
		}
	}

	void ThreadPool::StartSync(size_t N)
	{
		for (int i = 0; i < N; i++)
		{
			m_RunningThreads.push_back(
				std::async(
					std::launch::async,
					[this] { _ThreadWork(true);  }
			));
		}

		std::unique_lock<std::mutex> lck(m_M);
		m_AllThreadsStarted.wait(lck, [&]
			{
				return m_StartedThreads == N;
			});
	}

	void ThreadPool::CancelPendingTasks()
	{
		std::unique_lock<std::mutex> lck(m_M);
		m_Queue.clear();
	}

	void ThreadPool::Finish()
	{
		{
			std::unique_lock<std::mutex> lck(m_M);
			for ([[maybe_unused]] auto&& unsused : m_RunningThreads)
			{
				m_Queue.emplace_back();
			}
		}
		m_Signal.notify_all();
		m_RunningThreads.clear();
	}

    void ThreadPool::WaitToFinishTasks()
    {
        std::unique_lock lck(m_M);

        m_Signal.wait(lck, [this]
        {
            return m_Queue.empty();
        });
    }

	void ThreadPool::Abort()
	{
		CancelPendingTasks();
		Finish();
	}

	void ThreadPool::_ThreadWork(bool StartingSync)
	{
		if (m_StartCallback)
		{
			m_StartCallback();
		}

		if (StartingSync)
		{
			{
				std::unique_lock<std::mutex> lck(m_StartMut);
				m_StartedThreads++;
			}
			m_AllThreadsStarted.notify_one();
		}

		while (true)
		{
			std::packaged_task<void()> f;
			{
				std::unique_lock<std::mutex> lck(m_M);
				if (m_Queue.empty())
				{
					m_Signal.wait(lck, [&] { return !m_Queue.empty(); });
				}

				f = std::move(m_Queue.front());
				m_Queue.pop_front();
			}

			if (!f.valid())
			{
				break;
			}

			f();

            m_Signal.notify_all();
		}

		if (m_EndCallback)
		{
			m_EndCallback();
		}
	}
}
