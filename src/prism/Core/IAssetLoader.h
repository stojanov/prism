#pragma once

#include <future>

#include "Pointers.h"

namespace Prism::Core
{
	template<typename T, typename LoadArgs>
	class IAssetLoader
	{
	public:
		IAssetLoader() = default;
		virtual ~IAssetLoader() = default;

		virtual bool LoadAsset(const std::string& name, const LoadArgs& args) = 0;
		virtual void QueueLoad(const std::string& name, const LoadArgs& args) = 0;
		virtual bool SyncLoad() = 0;
		virtual void AsyncLoad() = 0;
		virtual float GetProgress() = 0;
		virtual Ref<T> Get(const std::string& name) = 0;
		virtual Ref<T> MustGet(const std::string& name) = 0;
		virtual void ClearLoaded() = 0;
		virtual void ClearQueue() = 0;
		virtual void SafeDestroyLoaded() = 0;
		virtual void DestroyLoaded() = 0;
	};
}