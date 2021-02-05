#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <future>
#include "Types.h"

namespace Prism::System
{
	class ThreadPool
	{

	public:
		ThreadPool();
		ThreadPool(VoidCallback StartCallback);
		ThreadPool(VoidCallback StartCallback, VoidCallback EndCallback);
		~ThreadPool();

		template<typename F>
		std::future<void> QueueTask(F f)
		{
			std::packaged_task<void()> task(std::forward<F>(f));

			auto r = task.get_future();

			{
				std::unique_lock<std::mutex> lck(m_M);
				m_Queue.emplace_back(std::move(task));
			}

			m_Signal.notify_one();

			return r;
		}

		void CancelPendingTasks();
		void Abort();
		void Finish();
		void Start(size_t N = 1);
		void StartSync(size_t N = 1);
	private:
		void _ThreadWork(bool StartingSync = false);
		std::mutex m_StartMut;
		std::condition_variable m_AllThreadsStarted;
		int m_StartedThreads{ 0 };
		VoidCallback m_StartCallback, m_EndCallback;
		std::mutex m_M;
		std::condition_variable m_Signal;
		std::deque<std::packaged_task<void()>> m_Queue;
		std::vector<std::future<void>> m_RunningThreads;
	};
}