#pragma once
#include <type_traits>

#include "Pointers.h"
#include <future>

namespace Prism::Core
{
	template<typename T>
	using AssetLoadResult = std::pair<Ref<T>, bool>;
	/*
	 * Created as a template class so you can create loaders for multiple types of assets
	 */
	template<
		typename T,
		typename LoadArgs,
		AssetLoadResult<T>(*LoadFunc)(const LoadArgs& args),
		bool ShouldAssert = false>
	class AssetLoader
	{
	public:
		AssetLoader(const std::string& name)
			:
			m_Name(name)
		{	
		}

		// For immediate loading synced
		bool LoadAsset(const std::string& name, const LoadArgs& args)
		{
			return _LoadAsset(name, args);
		}

		// For async loading
		void QueueLoad(const std::string& name, const LoadArgs& args)
		{
			PR_CORE_INFO("Assets ({0})\t(Queued)\t{1}", m_Name, name);
			m_QueuedLoad.emplace_back(name, args);
		}

		bool SyncLoad()
		{
			return _LoadFunc();
		}

		std::future<bool> AsyncLoad()
		{
			return std::async(std::launch::async, std::bind(&AssetLoader::_LoadFunc, this)); 
		}

		float GetProgress()
		{
			{
				std::lock_guard<std::mutex> g(m);
				PR_CORE_ERROR("Assets ({0})\tNo loading job found", m_Name);
				PR_ASSERT(m_UnloadedAssetCount != 0, "There is no loading job active");
			}
			
			return 1.f / (m_UnloadedAssetCount * 1.f / m_LoadedAssetCount);
		}

		// Will search only on loaded assets
		Ref<T> Get(const std::string& name)
		{
			if (auto i = m_Assets.find(name); i !=  m_Assets.end())
			{
				return i->second;
			}
			PR_CORE_ERROR("Assets ({0})\tAsset not found: {1}", m_Name, name);
			PR_CONST_ASSERT(!ShouldAssert, "Asset not found: " + name);
			return nullptr;
		}

		// Will search only if all assets are loading if not will force sync load on assets
		Ref<T> MustGet(const std::string& name)
		{
			if (m_QueuedLoad.size() != 0)
			{
				SyncLoad();
			}
			if (auto i = m_Assets.find(name); i != m_Assets.end())
			{
				return i->second;
			}
			PR_CORE_ERROR("Assets ({0})\tAsset not found: {1}", m_Name, name);
			PR_CONST_ASSERT(!ShouldAssert, "Asset not found: " + name);
			return nullptr;
		}

		void ClearLoaded()
		{
			m_Assets.clear();
		}

		void ClearQueue()
		{
			m_QueuedLoad.clear();
		}

		// Won't work if asset is used in multiple threads
		void SafeDestroyLoaded()
		{
			PR_CORE_WARN("Assets ({0})\tSafe Destroying loadind assets", m_Name);
			for (auto i : m_Assets)
			{
				if (i->second.use_count == 1)
				{
					m_Assets.erase(i);
				}
			}
		}
		
		void DestoryLoaded()
		{
			PR_CORE_WARN("Assets ({0})\tDestroying loadind assets", m_Name);
			for (auto i : m_Assets)
			{
				i->second.reset();
			}
			m_Assets.clear();
		}
	private:
		bool _LoadAsset(const std::string& name, const LoadArgs& args)
		{
			if (auto i = m_Assets.find(name); i == m_Assets.end())
			{
				PR_CORE_INFO("Assets ({0})\t(Loading)\t{1}", m_Name, name);
				auto [LoadedAsset, err] = LoadFunc(args);
				if (err)
				{
					PR_CORE_ERROR("Assets ({0})\tError loading asset:{1}", m_Name, name);
					PR_CONST_ASSERT(!ShouldAssert, "Error loading asset");
					return err;
				}

				m_Assets.emplace(name, LoadedAsset);
				PR_CORE_INFO("Assets ({0})\t(Loaded)\t{1}", m_Name, name);
				return false;
			}
			PR_CORE_ERROR("Assets ({0})\tAsset already loaded:{1}", m_Name, name);
			PR_CONST_ASSERT(!ShouldAssert, "Asset already loaded: " + name);
			return true;
		}
		
		bool _LoadFunc()
		{
			{
				std::lock_guard<std::mutex> g(m_M);
				m_UnloadedAssetCount = m_Assets.size();
				m_LoadedAssetCount = 0;
			}
			
			for (auto& Asset : m_QueuedLoad)
			{
				auto& [Name, Args] = Asset;

				bool err = _LoadAsset(Name, Args);
				if (err)
				{
					return err;
				}

				m_LoadedAssetCount++;
			}

			{
				std::lock_guard<std::mutex> g(m_M);
				m_UnloadedAssetCount = 0;
			}
			m_QueuedLoad.clear();
			PR_CORE_INFO("Assets ({0})\tFinished Loading", m_Name);
			return false;
		}

		int m_UnloadedAssetCount{ 0 };
		int m_LoadedAssetCount{ 0 };
		std::unordered_map<std::string, Ref<T>> m_Assets;
		std::vector<std::pair<std::string, LoadArgs>> m_QueuedLoad;
		std::mutex m_M;
		std::string m_Name;
	};
}
