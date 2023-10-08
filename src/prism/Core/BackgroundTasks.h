#pragma once

#include <string>
#include <vector>

#include "Pointers.h"
#include "prism/System/ThreadPool.h"

namespace Prism::Core
{
	class BackgroundTasks
	{
	public:
		BackgroundTasks();
		~BackgroundTasks();
		void RegisterWorker(const std::string& name, int count);
		void RegisterWorker(const std::string& name, int count, System::VoidCallback StartCallback);
		void RegisterWorker(const std::string& name, int count, System::VoidCallback StartCallback, System::VoidCallback EndCallback);
		Ref<System::ThreadPool> GetWorker(const std::string& name);

		void Finish();
		void Abort();
	private:
		std::unordered_map<std::string, Ref<System::ThreadPool>> m_Workers;
	};
}
