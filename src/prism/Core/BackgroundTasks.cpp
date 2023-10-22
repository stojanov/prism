#include "BackgroundTasks.h"

namespace Prism::Core
{
	BackgroundTasks::BackgroundTasks()
	{
		
	}
	
	BackgroundTasks::~BackgroundTasks()
	{
		Finish();
	}
	
	void BackgroundTasks::RegisterWorker(const std::string& name, int count)
	{
		auto p = MakeRef<System::ThreadPool>();
		p->Start(count);
		PR_CORE_WARN("(BackgroundTasks) Registering worker {0}", name);
		m_Workers.emplace(name, std::move(p));
	}

	void BackgroundTasks::RegisterWorker(const std::string& name, int count, System::VoidCallback StartCallback)
	{
		auto p = MakeRef<System::ThreadPool>(StartCallback);
		p->Start(count);
		PR_CORE_WARN("(BackgroundTasks) Registering worker {0}", name);
		m_Workers.emplace(name, std::move(p));
	}

	void BackgroundTasks::RegisterWorker(const std::string& name, int count, System::VoidCallback StartCallback, System::VoidCallback EndCallback)
	{
		auto p = MakeRef<System::ThreadPool>(StartCallback, EndCallback);
		p->Start(count);
		PR_CORE_WARN("(BackgroundTasks) Registering worker {0}", name);
		m_Workers.emplace(name, std::move(p));
	}

	Ref<System::ThreadPool> BackgroundTasks::GetWorker(const std::string& name)
	{
		if (auto itr = m_Workers.find(name); itr != m_Workers.end())
		{
			return itr->second;
		}
		PR_CORE_ERROR("Couldn't find Background Job ({0})", name);
		PR_ASSERT(0, "Couldn't find Background Job");
		return nullptr;
	}

	void BackgroundTasks::Finish()
	{
		for (auto& itr : m_Workers)
		{
			itr.second->Finish();
		}
	}

	void BackgroundTasks::Abort()
	{
		for (auto& itr : m_Workers)
		{
			itr.second->Abort();
		}
	}

}