#pragma once

#include <mutex>
#include <queue>

namespace Prism::Utils
{
	template<typename T>
	class SyncQueue
	{
	public:
		SyncQueue()
		{
			
		}
		~SyncQueue()
		{
			
		}

		size_t Size()
		{
			std::lock_guard<std::mutex> lck(m_Mut);
			return m_Queue.size();
		}

		void Put(const T& item)
		{
			std::lock_guard<std::mutex> lck(m_Mut);
			m_Queue.emplace(std::move(item));
			m_Signal.notify_one();
		}
		
		bool Get(T& item)
		{
			std::unique_lock<std::mutex> lck(m_Mut);

			m_Signal.wait(lck, [&]
				{
					return !m_Queue.empty() || m_IsFinished;
				});

			if (m_Queue.empty())
			{
				return false;
			}

			item = std::move(m_Queue.front());
			m_Queue.pop();

			return true;
		}

		void Finish()
		{
			std::unique_lock<std::mutex> lck(m_Mut);
			m_IsFinished = true;
			
			m_Signal.notify_all();
			m_Signal.wait(lck, [&]
				{
					return m_Queue.empty();
				});

			m_IsFinished = false;
		}
		
	private:
		bool m_IsFinished{ false };
		std::mutex m_Mut;
		std::condition_variable m_Signal;
		std::queue<T> m_Queue;
	};
}