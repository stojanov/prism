#pragma once
#include <functional>

namespace Prism::System
{
	using VoidCallback = std::function<void()>;
	using TaskFuture = std::future<void>;
}
